// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef MINIZIP_ENABLED

#include "file_access_zip.h"

#include "core/os/file_access.h"

ZipArchive* ZipArchive::instance = nullptr;

extern "C" {

static void* zopen_file(void* data, const char* p_fname, int mode) {
    if (mode & ZLIB_FILEFUNC_MODE_WRITE) {
        return nullptr;
    }

    FileAccess* f = FileAccess::open(p_fname, FileAccess::READ);
    ERR_FAIL_COND_V(!f, nullptr);

    return f;
}

static uLong zread_file(void* data, void* fdata, void* buf, uLong size) {
    FileAccess* f = (FileAccess*)fdata;
    f->get_buffer((uint8_t*)buf, size);
    return size;
}

static uLong zwrite_file(
    voidpf opaque,
    voidpf stream,
    const void* buf,
    uLong size
) {
    return 0;
}

static long ztell_file(voidpf opaque, voidpf stream) {
    FileAccess* f = (FileAccess*)stream;
    return f->get_position();
}

static long zseek_file(voidpf opaque, voidpf stream, uLong offset, int origin) {
    FileAccess* f = (FileAccess*)stream;

    uint64_t pos = offset;
    switch (origin) {
        case ZLIB_FILEFUNC_SEEK_CUR:
            pos = f->get_position() + offset;
            break;
        case ZLIB_FILEFUNC_SEEK_END:
            pos = f->get_len() + offset;
            break;
        default:
            break;
    }

    f->seek(pos);
    return 0;
}

static int zclose_file(voidpf opaque, voidpf stream) {
    FileAccess* f = (FileAccess*)stream;
    if (f) {
        f->close();
        memdelete(f);
        f = nullptr;
    }
    return 0;
}

static int zerror_file(voidpf opaque, voidpf stream) {
    FileAccess* f = (FileAccess*)stream;
    return f->get_error() != OK ? 1 : 0;
}

static voidpf zalloc_mem(voidpf opaque, uInt items, uInt size) {
    return memalloc(items * size);
}

static void zfree_mem(voidpf opaque, voidpf address) {
    memfree(address);
}

} // extern "C"

void ZipArchive::close_handle(unzFile p_file) const {
    ERR_FAIL_COND_MSG(!p_file, "Cannot close a file if none is open.");
    unzCloseCurrentFile(p_file);
    unzClose(p_file);
}

unzFile ZipArchive::get_file_handle(String p_file) const {
    ERR_FAIL_COND_V_MSG(
        !file_exists(p_file),
        nullptr,
        "File '" + p_file + " doesn't exist."
    );
    File file = files[p_file];

    zlib_filefunc_def io;
    memset(&io, 0, sizeof(io));

    io.opaque      = nullptr;
    io.zopen_file  = zopen_file;
    io.zread_file  = zread_file;
    io.zwrite_file = zwrite_file;

    io.ztell_file  = ztell_file;
    io.zseek_file  = zseek_file;
    io.zclose_file = zclose_file;
    io.zerror_file = zerror_file;

    io.alloc_mem = zalloc_mem;
    io.free_mem  = zfree_mem;

    unzFile pkg =
        unzOpen2(packages[file.package].filename.utf8().get_data(), &io);
    ERR_FAIL_COND_V_MSG(
        !pkg,
        nullptr,
        "Cannot open file '" + packages[file.package].filename + "'."
    );
    int unz_err = unzGoToFilePos(pkg, &file.file_pos);
    if (unz_err != UNZ_OK || unzOpenCurrentFile(pkg) != UNZ_OK) {
        unzClose(pkg);
        ERR_FAIL_V(nullptr);
    }

    return pkg;
}

bool ZipArchive::try_open_pack(
    const String& p_path,
    bool p_replace_files,
    uint64_t p_offset = 0
) {
    // printf("opening zip pack %ls, %i, %i\n", p_name.c_str(),
    // p_name.extension().nocasecmp_to("zip"),
    // p_name.extension().nocasecmp_to("pcz"));
    //  load with offset feature only supported for PCK files
    ERR_FAIL_COND_V_MSG(
        p_offset != 0,
        false,
        "Invalid PCK data. Note that loading files with a non-zero offset "
        "isn't supported with ZIP archives."
    );

    if (p_path.get_extension().nocasecmp_to("zip") != 0
        && p_path.get_extension().nocasecmp_to("pcz") != 0) {
        return false;
    }

    zlib_filefunc_def io;
    memset(&io, 0, sizeof(io));

    io.opaque      = nullptr;
    io.zopen_file  = zopen_file;
    io.zread_file  = zread_file;
    io.zwrite_file = zwrite_file;

    io.ztell_file  = ztell_file;
    io.zseek_file  = zseek_file;
    io.zclose_file = zclose_file;
    io.zerror_file = zerror_file;

    unzFile zfile = unzOpen2(p_path.utf8().get_data(), &io);
    ERR_FAIL_COND_V(!zfile, false);

    unz_global_info64 gi;
    int err = unzGetGlobalInfo64(zfile, &gi);
    ERR_FAIL_COND_V(err != UNZ_OK, false);

    Package pkg;
    pkg.filename = p_path;
    pkg.zfile    = zfile;
    packages.push_back(pkg);
    int pkg_num = packages.size() - 1;

    for (uint64_t i = 0; i < gi.number_entry; i++) {
        char filename_inzip[256];

        unz_file_info64 file_info;
        err = unzGetCurrentFileInfo64(
            zfile,
            &file_info,
            filename_inzip,
            sizeof(filename_inzip),
            nullptr,
            0,
            nullptr,
            0
        );
        ERR_CONTINUE(err != UNZ_OK);

        File f;
        f.package = pkg_num;
        unzGetFilePos(zfile, &f.file_pos);

        String fname = String("res://") + String::utf8(filename_inzip);
        files[fname] = f;

        uint8_t md5[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        PackedData::get_singleton()
            ->add_path(p_path, fname, 1, 0, md5, this, p_replace_files);
        // printf("packed data add path %ls, %ls\n", p_name.c_str(),
        // fname.c_str());

        if ((i + 1) < gi.number_entry) {
            unzGoToNextFile(zfile);
        }
    }

    return true;
}

bool ZipArchive::file_exists(String p_name) const {
    return files.has(p_name);
}

FileAccess* ZipArchive::get_file(
    const String& p_path,
    PackedData::PackedFile* p_file
) {
    return memnew(FileAccessZip(p_path, *p_file));
}

ZipArchive* ZipArchive::get_singleton() {
    if (instance == nullptr) {
        instance = memnew(ZipArchive);
    }

    return instance;
}

ZipArchive::ZipArchive() {
    instance = this;
    // fa_create_func = FileAccess::get_create_func();
}

ZipArchive::~ZipArchive() {
    for (int i = 0; i < packages.size(); i++) {
        unzClose(packages[i].zfile);
    }

    packages.clear();
}

Error FileAccessZip::_open(const String& p_path, int p_mode_flags) {
    close();

    ERR_FAIL_COND_V(p_mode_flags & FileAccess::WRITE, FAILED);
    ZipArchive* arch = ZipArchive::get_singleton();
    ERR_FAIL_COND_V(!arch, FAILED);
    zfile = arch->get_file_handle(p_path);
    ERR_FAIL_COND_V(!zfile, FAILED);

    int err = unzGetCurrentFileInfo64(
        zfile,
        &file_info,
        nullptr,
        0,
        nullptr,
        0,
        nullptr,
        0
    );
    ERR_FAIL_COND_V(err != UNZ_OK, FAILED);

    return OK;
}

void FileAccessZip::close() {
    if (!zfile) {
        return;
    }

    ZipArchive* arch = ZipArchive::get_singleton();
    ERR_FAIL_COND(!arch);
    arch->close_handle(zfile);
    zfile = nullptr;
}

bool FileAccessZip::is_open() const {
    return zfile != nullptr;
}

void FileAccessZip::seek(uint64_t p_position) {
    ERR_FAIL_COND(!zfile);

    unzSeekCurrentFile(zfile, p_position);
}

void FileAccessZip::seek_end(int64_t p_position) {
    ERR_FAIL_COND(!zfile);
    unzSeekCurrentFile(zfile, get_len() + p_position);
}

uint64_t FileAccessZip::get_position() const {
    ERR_FAIL_COND_V(!zfile, 0);
    return unztell(zfile);
}

uint64_t FileAccessZip::get_len() const {
    ERR_FAIL_COND_V(!zfile, 0);
    return file_info.uncompressed_size;
}

bool FileAccessZip::eof_reached() const {
    ERR_FAIL_COND_V(!zfile, true);

    return at_eof;
}

uint8_t FileAccessZip::get_8() const {
    uint8_t ret = 0;
    get_buffer(&ret, 1);
    return ret;
}

uint64_t FileAccessZip::get_buffer(uint8_t* p_dst, uint64_t p_length) const {
    ERR_FAIL_COND_V(!p_dst && p_length > 0, -1);
    ERR_FAIL_COND_V(!zfile, -1);

    at_eof = unzeof(zfile);
    if (at_eof) {
        return 0;
    }
    int64_t read = unzReadCurrentFile(zfile, p_dst, p_length);
    ERR_FAIL_COND_V(read < 0, read);
    if ((uint64_t)read < p_length) {
        at_eof = true;
    }
    return read;
}

Error FileAccessZip::get_error() const {
    if (!zfile) {
        return ERR_UNCONFIGURED;
    }
    if (eof_reached()) {
        return ERR_FILE_EOF;
    }

    return OK;
}

void FileAccessZip::flush() {
    ERR_FAIL();
}

void FileAccessZip::store_8(uint8_t p_dest) {
    ERR_FAIL();
}

bool FileAccessZip::file_exists(const String& p_name) {
    return false;
}

FileAccessZip::FileAccessZip(
    const String& p_path,
    const PackedData::PackedFile& p_file
) :
    zfile(nullptr) {
    _open(p_path, FileAccess::READ);
}

FileAccessZip::~FileAccessZip() {
    close();
}

#endif // MINIZIP_ENABLED
