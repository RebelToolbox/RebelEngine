// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "register_types.h"

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/os/dir_access.h"
#include "core/os/os.h"
#include "core/project_settings.h"
#include "pluginscript_language.h"
#include "pluginscript_script.h"
#include "scene/main/scene_tree.h"

#include <pluginscript/rebel_pluginscript.h>

static List<PluginScriptLanguage*> pluginscript_languages;

static Error _check_language_desc(const rebel_pluginscript_language_desc* desc
) {
    ERR_FAIL_COND_V(!desc->name || desc->name == String(), ERR_BUG);
    ERR_FAIL_COND_V(!desc->type || desc->type == String(), ERR_BUG);
    ERR_FAIL_COND_V(!desc->extension || desc->extension == String(), ERR_BUG);
    ERR_FAIL_COND_V(
        !desc->recognized_extensions || !desc->recognized_extensions[0],
        ERR_BUG
    );
    ERR_FAIL_COND_V(!desc->init, ERR_BUG);
    ERR_FAIL_COND_V(!desc->finish, ERR_BUG);

    // desc->reserved_words is not mandatory
    // desc->comment_delimiters is not mandatory
    // desc->string_delimiters is not mandatory

    // desc->get_template_source_code is not mandatory
    // desc->validate is not mandatory

    // desc->get_template_source_code is not mandatory
    // desc->validate is not mandatory
    // desc->find_function is not mandatory
    // desc->make_function is not mandatory
    // desc->complete_code is not mandatory
    // desc->auto_indent_code is not mandatory
    ERR_FAIL_COND_V(!desc->add_global_constant, ERR_BUG);
    // desc->debug_get_error is not mandatory
    // desc->debug_get_stack_level_count is not mandatory
    // desc->debug_get_stack_level_line is not mandatory
    // desc->debug_get_stack_level_function is not mandatory
    // desc->debug_get_stack_level_source is not mandatory
    // desc->debug_get_stack_level_locals is not mandatory
    // desc->debug_get_stack_level_members is not mandatory
    // desc->debug_get_globals is not mandatory
    // desc->debug_parse_stack_level_expression is not mandatory
    // desc->profiling_start is not mandatory
    // desc->profiling_stop is not mandatory
    // desc->profiling_get_accumulated_data is not mandatory
    // desc->profiling_get_frame_data is not mandatory
    // desc->profiling_frame is not mandatory

    ERR_FAIL_COND_V(!desc->script_desc.init, ERR_BUG);
    ERR_FAIL_COND_V(!desc->script_desc.finish, ERR_BUG);

    ERR_FAIL_COND_V(!desc->script_desc.instance_desc.init, ERR_BUG);
    ERR_FAIL_COND_V(!desc->script_desc.instance_desc.finish, ERR_BUG);
    ERR_FAIL_COND_V(!desc->script_desc.instance_desc.set_prop, ERR_BUG);
    ERR_FAIL_COND_V(!desc->script_desc.instance_desc.get_prop, ERR_BUG);
    ERR_FAIL_COND_V(!desc->script_desc.instance_desc.call_method, ERR_BUG);
    ERR_FAIL_COND_V(!desc->script_desc.instance_desc.notification, ERR_BUG);
    // desc->script_desc.instance_desc.refcount_incremented is not mandatory
    // desc->script_desc.instance_desc.refcount_decremented is not mandatory
    return OK;
}

void GDAPI rebel_pluginscript_register_language(
    const rebel_pluginscript_language_desc* language_desc
) {
    Error ret = _check_language_desc(language_desc);
    if (ret) {
        ERR_FAIL();
    }
    PluginScriptLanguage* language =
        memnew(PluginScriptLanguage(language_desc));
    ScriptServer::register_language(language);
    ResourceLoader::add_resource_format_loader(language->get_resource_loader());
    ResourceSaver::add_resource_format_saver(language->get_resource_saver());
    pluginscript_languages.push_back(language);
}

void register_pluginscript_types() {
    ClassDB::register_class<PluginScript>();
}

void unregister_pluginscript_types() {
    for (List<PluginScriptLanguage*>::Element* e =
             pluginscript_languages.front();
         e;
         e = e->next()) {
        PluginScriptLanguage* language = e->get();
        ScriptServer::unregister_language(language);
        ResourceLoader::remove_resource_format_loader(
            language->get_resource_loader()
        );
        ResourceSaver::remove_resource_format_saver(
            language->get_resource_saver()
        );
        memdelete(language);
    }
}
