// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "export.h"

#include "core/io/image_loader.h"
#include "core/io/json.h"
#include "core/io/stream_peer_ssl.h"
#include "core/io/tcp_server.h"
#include "core/io/zip_io.h"
#include "editor/editor_export.h"
#include "editor/editor_node.h"
#include "main/splash.gen.h"
#include "platforms/web/logo.gen.h"
#include "platforms/web/run_icon.gen.h"

class EditorHTTPServer : public Reference {
private:
    Ref<TCP_Server> server;
    Map<String, String> mimes;
    Ref<StreamPeerTCP> tcp;
    Ref<StreamPeerSSL> ssl;
    Ref<StreamPeer> peer;
    Ref<CryptoKey> key;
    Ref<X509Certificate> cert;
    bool use_ssl  = false;
    uint64_t time = 0;
    uint8_t req_buf[4096];
    int req_pos = 0;

    void _clear_client() {
        peer = Ref<StreamPeer>();
        ssl  = Ref<StreamPeerSSL>();
        tcp  = Ref<StreamPeerTCP>();
        memset(req_buf, 0, sizeof(req_buf));
        time    = 0;
        req_pos = 0;
    }

    void _set_internal_certs(Ref<Crypto> p_crypto) {
        const String cache_path =
            EditorSettings::get_singleton()->get_cache_dir();
        const String key_path = cache_path.plus_file("html5_server.key");
        const String crt_path = cache_path.plus_file("html5_server.crt");
        bool regen =
            !FileAccess::exists(key_path) || !FileAccess::exists(crt_path);
        if (!regen) {
            key  = Ref<CryptoKey>(CryptoKey::create());
            cert = Ref<X509Certificate>(X509Certificate::create());
            if (key->load(key_path) != OK || cert->load(crt_path) != OK) {
                regen = true;
            }
        }
        if (regen) {
            key = p_crypto->generate_rsa(2048);
            key->save(key_path);
            cert = p_crypto->generate_self_signed_certificate(
                key,
                "CN=rebel-debug.local,O=A Game Dev,C=XXA",
                "20140101000000",
                "20340101000000"
            );
            cert->save(crt_path);
        }
    }

public:
    EditorHTTPServer() {
        mimes["html"] = "text/html";
        mimes["js"]   = "application/javascript";
        mimes["json"] = "application/json";
        mimes["pck"]  = "application/octet-stream";
        mimes["png"]  = "image/png";
        mimes["svg"]  = "image/svg";
        mimes["wasm"] = "application/wasm";
        server.instance();
        stop();
    }

    void stop() {
        server->stop();
        _clear_client();
    }

    Error listen(
        int p_port,
        IP_Address p_address,
        bool p_use_ssl,
        String p_ssl_key,
        String p_ssl_cert
    ) {
        use_ssl = p_use_ssl;
        if (use_ssl) {
            Ref<Crypto> crypto = Crypto::create();
            if (crypto.is_null()) {
                return ERR_UNAVAILABLE;
            }
            if (!p_ssl_key.empty() && !p_ssl_cert.empty()) {
                key       = Ref<CryptoKey>(CryptoKey::create());
                Error err = key->load(p_ssl_key);
                ERR_FAIL_COND_V(err != OK, err);
                cert = Ref<X509Certificate>(X509Certificate::create());
                err  = cert->load(p_ssl_cert);
                ERR_FAIL_COND_V(err != OK, err);
            } else {
                _set_internal_certs(crypto);
            }
        }
        return server->listen(p_port, p_address);
    }

    bool is_listening() const {
        return server->is_listening();
    }

    void _send_response() {
        Vector<String> psa = String((char*)req_buf).split("\r\n");
        int len            = psa.size();
        ERR_FAIL_COND_MSG(
            len < 4,
            "Not enough response headers, got: " + itos(len)
                + ", expected >= 4."
        );

        Vector<String> req = psa[0].split(" ", false);
        ERR_FAIL_COND_MSG(req.size() < 2, "Invalid protocol or status code.");

        // Wrong protocol
        ERR_FAIL_COND_MSG(
            req[0] != "GET" || req[2] != "HTTP/1.1",
            "Invalid method or HTTP version."
        );

        const int query_index = req[1].find_char('?');
        const String path =
            (query_index == -1) ? req[1] : req[1].substr(0, query_index);

        const String req_file = path.get_file();
        const String req_ext  = path.get_extension();
        const String cache_path =
            EditorSettings::get_singleton()->get_cache_dir().plus_file("web");
        const String filepath = cache_path.plus_file(req_file);

        if (!mimes.has(req_ext) || !FileAccess::exists(filepath)) {
            String s       = "HTTP/1.1 404 Not Found\r\n";
            s             += "Connection: Close\r\n";
            s             += "\r\n";
            CharString cs  = s.utf8();
            peer->put_data((const uint8_t*)cs.get_data(), cs.size() - 1);
            return;
        }
        const String ctype = mimes[req_ext];

        FileAccess* f = FileAccess::open(filepath, FileAccess::READ);
        ERR_FAIL_COND(!f);
        String s       = "HTTP/1.1 200 OK\r\n";
        s             += "Connection: Close\r\n";
        s             += "Content-Type: " + ctype + "\r\n";
        s             += "Access-Control-Allow-Origin: *\r\n";
        s             += "Cross-Origin-Opener-Policy: same-origin\r\n";
        s             += "Cross-Origin-Embedder-Policy: require-corp\r\n";
        s             += "Cache-Control: no-store, max-age=0\r\n";
        s             += "\r\n";
        CharString cs  = s.utf8();
        Error err =
            peer->put_data((const uint8_t*)cs.get_data(), cs.size() - 1);
        if (err != OK) {
            memdelete(f);
            ERR_FAIL();
        }

        while (true) {
            uint8_t bytes[4096];
            uint64_t read = f->get_buffer(bytes, 4096);
            if (read == 0) {
                break;
            }
            err = peer->put_data(bytes, read);
            if (err != OK) {
                memdelete(f);
                ERR_FAIL();
            }
        }
        memdelete(f);
    }

    void poll() {
        if (!server->is_listening()) {
            return;
        }
        if (tcp.is_null()) {
            if (!server->is_connection_available()) {
                return;
            }
            tcp  = server->take_connection();
            peer = tcp;
            time = OS::get_singleton()->get_ticks_usec();
        }
        if (OS::get_singleton()->get_ticks_usec() - time > 1000000) {
            _clear_client();
            return;
        }
        if (tcp->get_status() != StreamPeerTCP::STATUS_CONNECTED) {
            return;
        }

        if (use_ssl) {
            if (ssl.is_null()) {
                ssl  = Ref<StreamPeerSSL>(StreamPeerSSL::create());
                peer = ssl;
                ssl->set_blocking_handshake_enabled(false);
                if (ssl->accept_stream(tcp, key, cert) != OK) {
                    _clear_client();
                    return;
                }
            }
            ssl->poll();
            if (ssl->get_status() == StreamPeerSSL::STATUS_HANDSHAKING) {
                // Still handshaking, keep waiting.
                return;
            }
            if (ssl->get_status() != StreamPeerSSL::STATUS_CONNECTED) {
                _clear_client();
                return;
            }
        }

        while (true) {
            char* r = (char*)req_buf;
            int l   = req_pos - 1;
            if (l > 3 && r[l] == '\n' && r[l - 1] == '\r' && r[l - 2] == '\n'
                && r[l - 3] == '\r') {
                _send_response();
                _clear_client();
                return;
            }

            int read = 0;
            ERR_FAIL_COND(req_pos >= 4096);
            Error err = peer->get_partial_data(&req_buf[req_pos], 1, read);
            if (err != OK) {
                // Got an error
                _clear_client();
                return;
            } else if (read != 1) {
                // Busy, wait next poll
                return;
            }
            req_pos += read;
        }
    }
};

class WebEditorExportPlatform : public EditorExportPlatform {
    GDCLASS(WebEditorExportPlatform, EditorExportPlatform);

    Ref<ImageTexture> logo;
    Ref<ImageTexture> run_icon;
    Ref<ImageTexture> stop_icon;
    int menu_options = 0;

    Ref<EditorHTTPServer> server;
    bool server_quit = false;
    Mutex server_lock;
    Thread server_thread;

    enum ExportMode {
        EXPORT_MODE_NORMAL   = 0,
        EXPORT_MODE_THREADS  = 1,
        EXPORT_MODE_GDNATIVE = 2,
    };

    String _get_template_name(ExportMode p_mode, bool p_debug) const {
        String name = "webassembly";
        switch (p_mode) {
            case EXPORT_MODE_THREADS:
                name += "_threads";
                break;
            case EXPORT_MODE_GDNATIVE:
                name += "_gdnative";
                break;
            default:
                break;
        }
        if (p_debug) {
            name += "_debug.zip";
        } else {
            name += "_release.zip";
        }
        return name;
    }

    Ref<Image> _get_project_icon() const {
        Ref<Image> icon;
        icon.instance();
        const String icon_path =
            String(GLOBAL_GET("application/config/icon")).strip_edges();
        if (icon_path.empty()
            || ImageLoader::load_image(icon_path, icon) != OK) {
            return EditorNode::get_singleton()
                ->get_editor_theme()
                ->get_icon("DefaultProjectIcon", "EditorIcons")
                ->get_data();
        }
        return icon;
    }

    Ref<Image> _get_project_splash() const {
        Ref<Image> splash;
        splash.instance();
        const String splash_path =
            String(GLOBAL_GET("application/boot_splash/image")).strip_edges();
        if (splash_path.empty()
            || ImageLoader::load_image(splash_path, splash) != OK) {
            return Ref<Image>(memnew(Image(boot_splash_png)));
        }
        return splash;
    }

    Error _extract_template(
        const String& p_template,
        const String& p_dir,
        const String& p_name,
        bool pwa
    );
    void _replace_strings(
        Map<String, String> p_replaces,
        Vector<uint8_t>& r_template
    );
    void _fix_html(
        Vector<uint8_t>& p_html,
        const Ref<EditorExportPreset>& p_preset,
        const String& p_name,
        bool p_debug,
        int p_flags,
        const Vector<SharedObject> p_shared_objects,
        const Dictionary& p_file_sizes
    );
    Error _add_manifest_icon(
        const String& p_path,
        const String& p_icon,
        int p_size,
        Array& r_arr
    );
    Error _build_pwa(
        const Ref<EditorExportPreset>& p_preset,
        const String p_path,
        const Vector<SharedObject>& p_shared_objects
    );
    Error _write_or_error(const uint8_t* p_content, int p_len, String p_path);

    static void _server_thread_poll(void* data);

public:
    virtual void get_preset_features(
        const Ref<EditorExportPreset>& p_preset,
        List<String>* r_features
    );

    virtual void get_export_options(List<ExportOption>* r_options);

    virtual String get_name() const;
    virtual String get_os_name() const;
    virtual Ref<Texture> get_logo() const;

    virtual bool can_export(
        const Ref<EditorExportPreset>& p_preset,
        String& r_error,
        bool& r_missing_templates
    ) const;
    virtual List<String> get_binary_extensions(
        const Ref<EditorExportPreset>& p_preset
    ) const;
    virtual Error export_project(
        const Ref<EditorExportPreset>& p_preset,
        bool p_debug,
        const String& p_path,
        int p_flags = 0
    );

    virtual bool poll_export();
    virtual int get_options_count() const;

    virtual String get_option_label(int p_index) const {
        return p_index ? TTR("Stop HTTP Server") : TTR("Run in Browser");
    }

    virtual String get_option_tooltip(int p_index) const {
        return p_index
                 ? TTR("Stop HTTP Server")
                 : TTR("Run exported HTML in the system's default browser.");
    }

    virtual Ref<ImageTexture> get_option_icon(int p_index) const;
    virtual Error run(
        const Ref<EditorExportPreset>& p_preset,
        int p_option,
        int p_debug_flags
    );
    virtual Ref<Texture> get_run_icon() const;

    virtual void get_platform_features(List<String>* r_features) {
        r_features->push_back("web");
        r_features->push_back(get_os_name());
    }

    virtual void resolve_platform_feature_priorities(
        const Ref<EditorExportPreset>& p_preset,
        Set<String>& p_features
    ) {}

    WebEditorExportPlatform();
    ~WebEditorExportPlatform();
};

Error WebEditorExportPlatform::_extract_template(
    const String& p_template,
    const String& p_dir,
    const String& p_name,
    bool pwa
) {
    FileAccess* src_f    = nullptr;
    zlib_filefunc_def io = zipio_create_io_from_file(&src_f);
    unzFile pkg          = unzOpen2(p_template.utf8().get_data(), &io);

    if (!pkg) {
        EditorNode::get_singleton()->show_warning(
            TTR("Could not open template for export:") + "\n" + p_template
        );
        return ERR_FILE_NOT_FOUND;
    }

    if (unzGoToFirstFile(pkg) != UNZ_OK) {
        EditorNode::get_singleton()->show_warning(
            TTR("Invalid export template:") + "\n" + p_template
        );
        unzClose(pkg);
        return ERR_FILE_CORRUPT;
    }

    do {
        // get filename
        unz_file_info info;
        char fname[16384];
        unzGetCurrentFileInfo(pkg, &info, fname, 16384, nullptr, 0, nullptr, 0);

        String file = String::utf8(fname);

        // Skip service worker and offline page if not exporting pwa.
        if (!pwa
            && (file == "rebel.service.worker.js"
                || file == "rebel.offline.html")) {
            continue;
        }
        Vector<uint8_t> data;
        data.resize(info.uncompressed_size);

        // read
        unzOpenCurrentFile(pkg);
        unzReadCurrentFile(pkg, data.ptrw(), data.size());
        unzCloseCurrentFile(pkg);

        // write
        String dst    = p_dir.plus_file(file.replace("rebel", p_name));
        FileAccess* f = FileAccess::open(dst, FileAccess::WRITE);
        if (!f) {
            EditorNode::get_singleton()->show_warning(
                TTR("Could not write file:") + "\n" + dst
            );
            unzClose(pkg);
            return ERR_FILE_CANT_WRITE;
        }
        f->store_buffer(data.ptr(), data.size());
        memdelete(f);

    } while (unzGoToNextFile(pkg) == UNZ_OK);
    unzClose(pkg);
    return OK;
}

Error WebEditorExportPlatform::_write_or_error(
    const uint8_t* p_content,
    int p_size,
    String p_path
) {
    FileAccess* f = FileAccess::open(p_path, FileAccess::WRITE);
    if (!f) {
        EditorNode::get_singleton()->show_warning(
            TTR("Could not write file:") + "\n" + p_path
        );
        return ERR_FILE_CANT_WRITE;
    }
    f->store_buffer(p_content, p_size);
    memdelete(f);
    return OK;
}

void WebEditorExportPlatform::_replace_strings(
    Map<String, String> p_replaces,
    Vector<uint8_t>& r_template
) {
    String str_template = String::utf8(
        reinterpret_cast<const char*>(r_template.ptr()),
        r_template.size()
    );
    String out;
    Vector<String> lines = str_template.split("\n");
    for (int i = 0; i < lines.size(); i++) {
        String current_line = lines[i];
        for (Map<String, String>::Element* E = p_replaces.front(); E;
             E                               = E->next()) {
            current_line = current_line.replace(E->key(), E->get());
        }
        out += current_line + "\n";
    }
    CharString cs = out.utf8();
    r_template.resize(cs.length());
    for (int i = 0; i < cs.length(); i++) {
        r_template.write[i] = cs[i];
    }
}

void WebEditorExportPlatform::_fix_html(
    Vector<uint8_t>& p_html,
    const Ref<EditorExportPreset>& p_preset,
    const String& p_name,
    bool p_debug,
    int p_flags,
    const Vector<SharedObject> p_shared_objects,
    const Dictionary& p_file_sizes
) {
    // Engine.js config
    Dictionary config;
    Array libs;
    for (int i = 0; i < p_shared_objects.size(); i++) {
        libs.push_back(p_shared_objects[i].path.get_file());
    }
    Vector<String> flags;
    gen_export_flags(
        flags,
        p_flags & (~DEBUG_FLAG_REMOTE_DEBUG) & (~DEBUG_FLAG_DUMB_CLIENT)
    );
    Array args;
    for (int i = 0; i < flags.size(); i++) {
        args.push_back(flags[i]);
    }
    config["canvasResizePolicy"] = p_preset->get("html/canvas_resize_policy");
    config["experimentalVK"] =
        p_preset->get("html/experimental_virtual_keyboard");
    config["focusCanvas"]  = p_preset->get("html/focus_canvas_on_start");
    config["gdnativeLibs"] = libs;
    config["executable"]   = p_name;
    config["args"]         = args;
    config["fileSizes"]    = p_file_sizes;

    String head_include;
    if (p_preset->get("html/export_icon")) {
        head_include +=
            "<link id='-gd-engine-icon' rel='icon' type='image/png' href='"
            + p_name + ".icon.png' />\n";
        head_include += "<link rel='apple-touch-icon' href='" + p_name
                      + ".apple-touch-icon.png'/>\n";
    }
    if (p_preset->get("progressive_web_app/enabled")) {
        head_include +=
            "<link rel='manifest' href='" + p_name + ".manifest.json'>\n";
        head_include +=
            "<script "
            "type='application/javascript'>window.addEventListener('load', () "
            "=> {if ('serviceWorker' in navigator) "
            "{navigator.serviceWorker.register('"
            + p_name + ".service.worker.js');}});</script>\n";
    }

    // Replaces HTML string
    const String str_config          = JSON::print(config);
    const String custom_head_include = p_preset->get("html/head_include");
    Map<String, String> replaces;
    replaces["$GAME_URL"] = p_name + ".js";
    replaces["$REBEL_PROJECT_NAME"] =
        ProjectSettings::get_singleton()->get_setting("application/config/name"
        );
    replaces["$HEAD_INCLUDE"] = head_include + custom_head_include;
    replaces["$GAME_CONFIG"]  = str_config;
    _replace_strings(replaces, p_html);
}

Error WebEditorExportPlatform::_add_manifest_icon(
    const String& p_path,
    const String& p_icon,
    int p_size,
    Array& r_arr
) {
    const String name      = p_path.get_file().get_basename();
    const String icon_name = vformat("%s.%dx%d.png", name, p_size, p_size);
    const String icon_dest = p_path.get_base_dir().plus_file(icon_name);

    Ref<Image> icon;
    if (!p_icon.empty()) {
        icon.instance();
        const Error err = ImageLoader::load_image(p_icon, icon);
        if (err != OK) {
            EditorNode::get_singleton()->show_warning(
                TTR("Could not read file:") + "\n" + p_icon
            );
            return err;
        }
        if (icon->get_width() != p_size || icon->get_height() != p_size) {
            icon->resize(p_size, p_size);
        }
    } else {
        icon = _get_project_icon();
        icon->resize(p_size, p_size);
    }
    const Error err = icon->save_png(icon_dest);
    if (err != OK) {
        EditorNode::get_singleton()->show_warning(
            TTR("Could not write file:") + "\n" + icon_dest
        );
        return err;
    }
    Dictionary icon_dict;
    icon_dict["sizes"] = vformat("%dx%d", p_size, p_size);
    icon_dict["type"]  = "image/png";
    icon_dict["src"]   = icon_name;
    r_arr.push_back(icon_dict);
    return err;
}

Error WebEditorExportPlatform::_build_pwa(
    const Ref<EditorExportPreset>& p_preset,
    const String p_path,
    const Vector<SharedObject>& p_shared_objects
) {
    // Service worker
    const String dir  = p_path.get_base_dir();
    const String name = p_path.get_file().get_basename();
    const ExportMode mode =
        (ExportMode)(int)p_preset->get("variant/export_type");
    Map<String, String> replaces;
    replaces["@REBEL_VERSION@"] = "1";
    replaces["@REBEL_NAME@"]    = name;
    replaces["@OFFLINE_PAGE@"]  = name + ".offline.html";
    Array files;
    replaces["@REBEL_OPT_CACHE@"] = JSON::print(files);
    files.push_back(name + ".html");
    files.push_back(name + ".js");
    files.push_back(name + ".wasm");
    files.push_back(name + ".pck");
    files.push_back(name + ".offline.html");
    if (p_preset->get("html/export_icon")) {
        files.push_back(name + ".icon.png");
        files.push_back(name + ".apple-touch-icon.png");
    }
    if (mode == EXPORT_MODE_THREADS) {
        files.push_back(name + ".worker.js");
        files.push_back(name + ".audio.worklet.js");
    } else if (mode == EXPORT_MODE_GDNATIVE) {
        files.push_back(name + ".side.wasm");
        for (int i = 0; i < p_shared_objects.size(); i++) {
            files.push_back(p_shared_objects[i].path.get_file());
        }
    }
    replaces["@REBEL_CACHE@"] = JSON::print(files);

    const String sw_path = dir.plus_file(name + ".service.worker.js");
    Vector<uint8_t> sw;
    {
        FileAccess* f = FileAccess::open(sw_path, FileAccess::READ);
        if (!f) {
            EditorNode::get_singleton()->show_warning(
                TTR("Could not read file:") + "\n" + sw_path
            );
            return ERR_FILE_CANT_READ;
        }
        sw.resize(f->get_len());
        f->get_buffer(sw.ptrw(), sw.size());
        memdelete(f);
        f = nullptr;
    }
    _replace_strings(replaces, sw);
    Error err = _write_or_error(
        sw.ptr(),
        sw.size(),
        dir.plus_file(name + ".service.worker.js")
    );
    if (err != OK) {
        return err;
    }

    // Custom offline page
    const String offline_page =
        p_preset->get("progressive_web_app/offline_page");
    if (!offline_page.empty()) {
        DirAccess* da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
        const String offline_dest = dir.plus_file(name + ".offline.html");
        err                       = da->copy(
            ProjectSettings::get_singleton()->globalize_path(offline_page),
            offline_dest
        );
        if (err != OK) {
            EditorNode::get_singleton()->show_warning(
                TTR("Could not read file:") + "\n" + offline_dest
            );
            return err;
        }
    }

    // Manifest
    const char* modes[4] =
        {"fullscreen", "standalone", "minimal-ui", "browser"};
    const char* orientations[3] = {"any", "landscape", "portrait"};
    const int display =
        CLAMP(int(p_preset->get("progressive_web_app/display")), 0, 4);
    const int orientation =
        CLAMP(int(p_preset->get("progressive_web_app/orientation")), 0, 3);

    Dictionary manifest;
    String proj_name =
        ProjectSettings::get_singleton()->get_setting("application/config/name"
        );
    if (proj_name.empty()) {
        proj_name = "Rebel Game";
    }
    manifest["name"]        = proj_name;
    manifest["start_url"]   = "./" + name + ".html";
    manifest["display"]     = String::utf8(modes[display]);
    manifest["orientation"] = String::utf8(orientations[orientation]);
    manifest["background_color"] =
        "#"
        + p_preset->get("progressive_web_app/background_color")
              .
              operator Color()
              .to_html(false);

    Array icons_arr;
    const String icon144_path =
        p_preset->get("progressive_web_app/icon_144x144");
    err = _add_manifest_icon(p_path, icon144_path, 144, icons_arr);
    if (err != OK) {
        return err;
    }
    const String icon180_path =
        p_preset->get("progressive_web_app/icon_180x180");
    err = _add_manifest_icon(p_path, icon180_path, 180, icons_arr);
    if (err != OK) {
        return err;
    }
    const String icon512_path =
        p_preset->get("progressive_web_app/icon_512x512");
    err = _add_manifest_icon(p_path, icon512_path, 512, icons_arr);
    if (err != OK) {
        return err;
    }
    manifest["icons"] = icons_arr;

    CharString cs = JSON::print(manifest).utf8();
    err           = _write_or_error(
        (const uint8_t*)cs.get_data(),
        cs.length(),
        dir.plus_file(name + ".manifest.json")
    );
    if (err != OK) {
        return err;
    }

    return OK;
}

void WebEditorExportPlatform::get_preset_features(
    const Ref<EditorExportPreset>& p_preset,
    List<String>* r_features
) {
    if (p_preset->get("vram_texture_compression/for_desktop")) {
        r_features->push_back("s3tc");
    }

    if (p_preset->get("vram_texture_compression/for_mobile")) {
        String driver = ProjectSettings::get_singleton()->get(
            "rendering/quality/driver/driver_name"
        );
        if (driver == "GLES2") {
            r_features->push_back("etc");
        } else if (driver == "GLES3") {
            r_features->push_back("etc2");
            if (ProjectSettings::get_singleton()->get(
                    "rendering/quality/driver/fallback_to_gles2"
                )) {
                r_features->push_back("etc");
            }
        }
    }
    ExportMode mode = (ExportMode)(int)p_preset->get("variant/export_type");
    if (mode == EXPORT_MODE_THREADS) {
        r_features->push_back("threads");
    } else if (mode == EXPORT_MODE_GDNATIVE) {
        r_features->push_back("wasm32");
    }
}

void WebEditorExportPlatform::get_export_options(List<ExportOption>* r_options
) {
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::STRING,
            "custom_template/debug",
            PROPERTY_HINT_GLOBAL_FILE,
            "*.zip"
        ),
        ""
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::STRING,
            "custom_template/release",
            PROPERTY_HINT_GLOBAL_FILE,
            "*.zip"
        ),
        ""
    ));

    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::INT,
            "variant/export_type",
            PROPERTY_HINT_ENUM,
            "Regular,Threads,GDNative"
        ),
        0
    )); // Export type.
    r_options->push_back(ExportOption(
        PropertyInfo(Variant::BOOL, "vram_texture_compression/for_desktop"),
        true
    )); // S3TC
    r_options->push_back(ExportOption(
        PropertyInfo(Variant::BOOL, "vram_texture_compression/for_mobile"),
        false
    )); // ETC or ETC2, depending on renderer

    r_options->push_back(
        ExportOption(PropertyInfo(Variant::BOOL, "html/export_icon"), true)
    );
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::STRING,
            "html/custom_html_shell",
            PROPERTY_HINT_FILE,
            "*.html"
        ),
        ""
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::STRING,
            "html/head_include",
            PROPERTY_HINT_MULTILINE_TEXT
        ),
        ""
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::INT,
            "html/canvas_resize_policy",
            PROPERTY_HINT_ENUM,
            "None,Project,Adaptive"
        ),
        2
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(Variant::BOOL, "html/focus_canvas_on_start"),
        true
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(Variant::BOOL, "html/experimental_virtual_keyboard"),
        false
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(Variant::BOOL, "progressive_web_app/enabled"),
        false
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::STRING,
            "progressive_web_app/offline_page",
            PROPERTY_HINT_FILE,
            "*.html"
        ),
        ""
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::INT,
            "progressive_web_app/display",
            PROPERTY_HINT_ENUM,
            "Fullscreen,Standalone,Minimal Ui,Browser"
        ),
        1
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::INT,
            "progressive_web_app/orientation",
            PROPERTY_HINT_ENUM,
            "Any,Landscape,Portrait"
        ),
        0
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::STRING,
            "progressive_web_app/icon_144x144",
            PROPERTY_HINT_FILE,
            "*.png,*.webp,*.svg"
        ),
        ""
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::STRING,
            "progressive_web_app/icon_180x180",
            PROPERTY_HINT_FILE,
            "*.png,*.webp,*.svg"
        ),
        ""
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::STRING,
            "progressive_web_app/icon_512x512",
            PROPERTY_HINT_FILE,
            "*.png,*.webp,*.svg"
        ),
        ""
    ));
    r_options->push_back(ExportOption(
        PropertyInfo(
            Variant::COLOR,
            "progressive_web_app/background_color",
            PROPERTY_HINT_COLOR_NO_ALPHA
        ),
        Color()
    ));
}

String WebEditorExportPlatform::get_name() const {
    return "Web";
}

String WebEditorExportPlatform::get_os_name() const {
    return "Web";
}

Ref<Texture> WebEditorExportPlatform::get_logo() const {
    return logo;
}

bool WebEditorExportPlatform::can_export(
    const Ref<EditorExportPreset>& p_preset,
    String& r_error,
    bool& r_missing_templates
) const {
    String err;
    bool valid      = false;
    ExportMode mode = (ExportMode)(int)p_preset->get("variant/export_type");

    // Look for export templates (first official, and if defined custom
    // templates).
    bool dvalid = exists_export_template(_get_template_name(mode, true), &err);
    bool rvalid = exists_export_template(_get_template_name(mode, false), &err);

    if (p_preset->get("custom_template/debug") != "") {
        dvalid = FileAccess::exists(p_preset->get("custom_template/debug"));
        if (!dvalid) {
            err += TTR("Custom debug template not found.") + "\n";
        }
    }
    if (p_preset->get("custom_template/release") != "") {
        rvalid = FileAccess::exists(p_preset->get("custom_template/release"));
        if (!rvalid) {
            err += TTR("Custom release template not found.") + "\n";
        }
    }

    valid               = dvalid || rvalid;
    r_missing_templates = !valid;

    // Validate the rest of the configuration.

    if (p_preset->get("vram_texture_compression/for_mobile")) {
        String etc_error = test_etc2();
        if (etc_error != String()) {
            valid  = false;
            err   += etc_error;
        }
    }

    if (!err.empty()) {
        r_error = err;
    }

    return valid;
}

List<String> WebEditorExportPlatform::get_binary_extensions(
    const Ref<EditorExportPreset>& p_preset
) const {
    List<String> list;
    list.push_back("html");
    return list;
}

Error WebEditorExportPlatform::export_project(
    const Ref<EditorExportPreset>& p_preset,
    bool p_debug,
    const String& p_path,
    int p_flags
) {
    ExportNotifier notifier(*this, p_preset, p_debug, p_path, p_flags);

    const String custom_debug   = p_preset->get("custom_template/debug");
    const String custom_release = p_preset->get("custom_template/release");
    const String custom_html    = p_preset->get("html/custom_html_shell");
    const bool export_icon      = p_preset->get("html/export_icon");
    const bool pwa              = p_preset->get("progressive_web_app/enabled");

    const String base_dir  = p_path.get_base_dir();
    const String base_path = p_path.get_basename();
    const String base_name = p_path.get_file().get_basename();

    // Find the correct template
    String template_path = p_debug ? custom_debug : custom_release;
    template_path        = template_path.strip_edges();
    if (template_path == String()) {
        ExportMode mode = (ExportMode)(int)p_preset->get("variant/export_type");
        template_path = find_export_template(_get_template_name(mode, p_debug));
    }

    if (!DirAccess::exists(base_dir)) {
        return ERR_FILE_BAD_PATH;
    }

    if (template_path != String() && !FileAccess::exists(template_path)) {
        EditorNode::get_singleton()->show_warning(
            TTR("Template file not found:") + "\n" + template_path
        );
        return ERR_FILE_NOT_FOUND;
    }

    // Export pck and shared objects
    Vector<SharedObject> shared_objects;
    String pck_path = base_path + ".pck";
    Error error     = save_pack(p_preset, pck_path, &shared_objects);
    if (error != OK) {
        EditorNode::get_singleton()->show_warning(
            TTR("Could not write file:") + "\n" + pck_path
        );
        return error;
    }
    DirAccess* da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    for (int i = 0; i < shared_objects.size(); i++) {
        String dst = base_dir.plus_file(shared_objects[i].path.get_file());
        error      = da->copy(shared_objects[i].path, dst);
        if (error != OK) {
            EditorNode::get_singleton()->show_warning(
                TTR("Could not write file:") + "\n"
                + shared_objects[i].path.get_file()
            );
            memdelete(da);
            return error;
        }
    }
    memdelete(da);
    da = nullptr;

    // Extract templates.
    error = _extract_template(template_path, base_dir, base_name, pwa);
    if (error) {
        return error;
    }

    // Parse generated file sizes (pck and wasm, to help show a meaningful
    // loading bar).
    Dictionary file_sizes;
    FileAccess* f = nullptr;
    f             = FileAccess::open(pck_path, FileAccess::READ);
    if (f) {
        file_sizes[pck_path.get_file()] = (uint64_t)f->get_len();
        memdelete(f);
        f = nullptr;
    }
    f = FileAccess::open(base_path + ".wasm", FileAccess::READ);
    if (f) {
        file_sizes[base_name + ".wasm"] = (uint64_t)f->get_len();
        memdelete(f);
        f = nullptr;
    }

    // Read the HTML shell file (custom or from template).
    const String html_path =
        custom_html.empty() ? base_path + ".html" : custom_html;
    Vector<uint8_t> html;
    f = FileAccess::open(html_path, FileAccess::READ);
    if (!f) {
        EditorNode::get_singleton()->show_warning(
            TTR("Could not read HTML shell:") + "\n" + html_path
        );
        return ERR_FILE_CANT_READ;
    }
    html.resize(f->get_len());
    f->get_buffer(html.ptrw(), html.size());
    memdelete(f);
    f = nullptr;

    // Generate HTML file with replaced strings.
    _fix_html(
        html,
        p_preset,
        base_name,
        p_debug,
        p_flags,
        shared_objects,
        file_sizes
    );
    Error err = _write_or_error(html.ptr(), html.size(), p_path);
    if (err != OK) {
        return err;
    }
    html.resize(0);

    // Export splash (why?)
    Ref<Image> splash            = _get_project_splash();
    const String splash_png_path = base_path + ".png";
    if (splash->save_png(splash_png_path) != OK) {
        EditorNode::get_singleton()->show_warning(
            TTR("Could not write file:") + "\n" + splash_png_path
        );
        return ERR_FILE_CANT_WRITE;
    }

    // Save a favicon that can be accessed without waiting for the project to
    // finish loading. This way, the favicon can be displayed immediately when
    // loading the page.
    if (export_icon) {
        Ref<Image> favicon            = _get_project_icon();
        const String favicon_png_path = base_path + ".icon.png";
        if (favicon->save_png(favicon_png_path) != OK) {
            EditorNode::get_singleton()->show_warning(
                TTR("Could not write file:") + "\n" + favicon_png_path
            );
            return ERR_FILE_CANT_WRITE;
        }
        favicon->resize(180, 180);
        const String apple_icon_png_path = base_path + ".apple-touch-icon.png";
        if (favicon->save_png(apple_icon_png_path) != OK) {
            EditorNode::get_singleton()->show_warning(
                TTR("Could not write file:") + "\n" + apple_icon_png_path
            );
            return ERR_FILE_CANT_WRITE;
        }
    }

    // Generate the PWA worker and manifest
    if (pwa) {
        err = _build_pwa(p_preset, p_path, shared_objects);
        if (err != OK) {
            return err;
        }
    }

    return OK;
}

bool WebEditorExportPlatform::poll_export() {
    Ref<EditorExportPreset> preset;

    for (int i = 0;
         i < EditorExport::get_singleton()->get_export_preset_count();
         i++) {
        Ref<EditorExportPreset> ep =
            EditorExport::get_singleton()->get_export_preset(i);
        if (ep->is_runnable() && ep->get_platform() == this) {
            preset = ep;
            break;
        }
    }

    int prev     = menu_options;
    menu_options = preset.is_valid();
    if (server->is_listening()) {
        if (menu_options == 0) {
            MutexLock lock(server_lock);
            server->stop();
        } else {
            menu_options += 1;
        }
    }
    return menu_options != prev;
}

Ref<ImageTexture> WebEditorExportPlatform::get_option_icon(int p_index) const {
    return p_index == 1 ? stop_icon
                        : EditorExportPlatform::get_option_icon(p_index);
}

int WebEditorExportPlatform::get_options_count() const {
    return menu_options;
}

Error WebEditorExportPlatform::run(
    const Ref<EditorExportPreset>& p_preset,
    int p_option,
    int p_debug_flags
) {
    if (p_option == 1) {
        MutexLock lock(server_lock);
        server->stop();
        return OK;
    }

    const String dest =
        EditorSettings::get_singleton()->get_cache_dir().plus_file("web");
    DirAccessRef da = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
    if (!da->dir_exists(dest)) {
        Error err = da->make_dir_recursive(dest);
        if (err != OK) {
            EditorNode::get_singleton()->show_warning(
                TTR("Could not create HTTP server directory:") + "\n" + dest
            );
            return err;
        }
    }
    const String basepath = dest.plus_file("tmp_js_export");
    Error err =
        export_project(p_preset, true, basepath + ".html", p_debug_flags);
    if (err != OK) {
        // Export generates several files, clean them up on failure.
        DirAccess::remove_file_or_error(basepath + ".html");
        DirAccess::remove_file_or_error(basepath + ".offline.html");
        DirAccess::remove_file_or_error(basepath + ".js");
        DirAccess::remove_file_or_error(basepath + ".worker.js");
        DirAccess::remove_file_or_error(basepath + ".audio.worklet.js");
        DirAccess::remove_file_or_error(basepath + ".service.worker.js");
        DirAccess::remove_file_or_error(basepath + ".pck");
        DirAccess::remove_file_or_error(basepath + ".png");
        DirAccess::remove_file_or_error(basepath + ".side.wasm");
        DirAccess::remove_file_or_error(basepath + ".wasm");
        DirAccess::remove_file_or_error(basepath + ".icon.png");
        DirAccess::remove_file_or_error(basepath + ".apple-touch-icon.png");
        return err;
    }

    const uint16_t bind_port = EDITOR_GET("export/web/http_port");
    // Resolve host if needed.
    const String bind_host   = EDITOR_GET("export/web/http_host");
    IP_Address bind_ip;
    if (bind_host.is_valid_ip_address()) {
        bind_ip = bind_host;
    } else {
        bind_ip = IP::get_singleton()->resolve_hostname(bind_host);
    }
    ERR_FAIL_COND_V_MSG(
        !bind_ip.is_valid(),
        ERR_INVALID_PARAMETER,
        "Invalid editor setting 'export/web/http_host': '" + bind_host
            + "'. Try using '127.0.0.1'."
    );

    const bool use_ssl    = EDITOR_GET("export/web/use_ssl");
    const String ssl_key  = EDITOR_GET("export/web/ssl_key");
    const String ssl_cert = EDITOR_GET("export/web/ssl_certificate");

    // Restart server.
    {
        MutexLock lock(server_lock);

        server->stop();
        err = server->listen(bind_port, bind_ip, use_ssl, ssl_key, ssl_cert);
    }
    if (err != OK) {
        EditorNode::get_singleton()->show_warning(
            TTR("Error starting HTTP server:") + "\n" + itos(err)
        );
        return err;
    }

    OS::get_singleton()->shell_open(String(
        (use_ssl ? "https://" : "http://") + bind_host + ":" + itos(bind_port)
        + "/tmp_js_export.html"
    ));
    // FIXME: Find out how to clean up export files after running the
    // successfully exported game. Might not be trivial.
    return OK;
}

Ref<Texture> WebEditorExportPlatform::get_run_icon() const {
    return run_icon;
}

void WebEditorExportPlatform::_server_thread_poll(void* data) {
    WebEditorExportPlatform* ej = (WebEditorExportPlatform*)data;
    while (!ej->server_quit) {
        OS::get_singleton()->delay_usec(1000);
        {
            MutexLock lock(ej->server_lock);
            ej->server->poll();
        }
    }
}

WebEditorExportPlatform::WebEditorExportPlatform() {
    server.instance();
    server_thread.start(_server_thread_poll, this);

    Ref<Image> img = memnew(Image(_web_logo));
    logo.instance();
    logo->create_from_image(img);

    img = Ref<Image>(memnew(Image(_web_run_icon)));
    run_icon.instance();
    run_icon->create_from_image(img);

    Ref<Theme> theme = EditorNode::get_singleton()->get_editor_theme();
    if (theme.is_valid()) {
        stop_icon = theme->get_icon("Stop", "EditorIcons");
    } else {
        stop_icon.instance();
    }
}

WebEditorExportPlatform::~WebEditorExportPlatform() {
    server->stop();
    server_quit = true;
    server_thread.wait_to_finish();
}

void register_web_exporter() {
    EDITOR_DEF("export/web/http_host", "localhost");
    EDITOR_DEF("export/web/http_port", 8060);
    EDITOR_DEF("export/web/use_ssl", false);
    EDITOR_DEF("export/web/ssl_key", "");
    EDITOR_DEF("export/web/ssl_certificate", "");
    EditorSettings::get_singleton()->add_property_hint(PropertyInfo(
        Variant::INT,
        "export/web/http_port",
        PROPERTY_HINT_RANGE,
        "1,65535,1"
    ));
    EditorSettings::get_singleton()->add_property_hint(PropertyInfo(
        Variant::STRING,
        "export/web/ssl_key",
        PROPERTY_HINT_GLOBAL_FILE,
        "*.key"
    ));
    EditorSettings::get_singleton()->add_property_hint(PropertyInfo(
        Variant::STRING,
        "export/web/ssl_certificate",
        PROPERTY_HINT_GLOBAL_FILE,
        "*.crt,*.pem"
    ));

    Ref<WebEditorExportPlatform> platform;
    platform.instance();
    EditorExport::get_singleton()->add_export_platform(platform);
}
