// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/os/os.h"
#include "nativescript.h"

NativeScriptLanguage* native_script_language;

Ref<ResourceFormatLoaderNativeScript> resource_loader_gdns;
Ref<ResourceFormatSaverNativeScript> resource_saver_gdns;

void register_nativescript_types() {
    native_script_language = memnew(NativeScriptLanguage);

    ClassDB::register_class<NativeScript>();

    native_script_language->set_language_index(ScriptServer::get_language_count(
    ));
    ScriptServer::register_language(native_script_language);

    resource_saver_gdns.instance();
    ResourceSaver::add_resource_format_saver(resource_saver_gdns);

    resource_loader_gdns.instance();
    ResourceLoader::add_resource_format_loader(resource_loader_gdns);
}

void unregister_nativescript_types() {
    ResourceLoader::remove_resource_format_loader(resource_loader_gdns);
    resource_loader_gdns.unref();

    ResourceSaver::remove_resource_format_saver(resource_saver_gdns);
    resource_saver_gdns.unref();

    if (native_script_language) {
        ScriptServer::unregister_language(native_script_language);
        memdelete(native_script_language);
    }
}
