// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PLUGINSCRIPT_SCRIPT_H
#define PLUGINSCRIPT_SCRIPT_H

// Rebel imports
#include "core/script_language.h"
// PluginScript imports
#include "pluginscript_language.h"

#include <pluginscript/rebel_pluginscript.h>

class PluginScript : public Script {
    GDCLASS(PluginScript, Script);

    friend class PluginScriptInstance;
    friend class PluginScriptLanguage;

private:
    rebel_pluginscript_script_data* _data;
    const rebel_pluginscript_script_desc* _desc;
    PluginScriptLanguage* _language;
    bool _tool;
    bool _valid;

    Ref<Script> _ref_base_parent;
    StringName _native_parent;
    SelfList<PluginScript> _script_list{this};

    Map<StringName, int> _member_lines;
    Map<StringName, Variant> _properties_default_values;
    Map<StringName, PropertyInfo> _properties_info;
    Map<StringName, MethodInfo> _signals_info;
    Map<StringName, MethodInfo> _methods_info;
    Map<StringName, MultiplayerAPI::RPCMode> _variables_rset_mode;
    Map<StringName, MultiplayerAPI::RPCMode> _methods_rpc_mode;

    Set<Object*> _instances;
    // exported members
    String _source;
    String _path;
    StringName _name;

protected:
    static void _bind_methods();

    bool inherits_script(const Ref<Script>& p_script) const override;

    PluginScriptInstance* _create_instance(
        const Variant** p_args,
        int p_argcount,
        Object* p_owner,
        Variant::CallError& r_error
    );
    Variant _new(
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    );

#ifdef TOOLS_ENABLED
    Set<PlaceHolderScriptInstance*> placeholders;
    void _placeholder_erased(PlaceHolderScriptInstance* p_placeholder) override;
#endif // TOOLS_ENABLED

public:
    bool can_instance() const override;

    Ref<Script> get_base_script() const override;

    StringName get_instance_base_type() const override;
    ScriptInstance* instance_create(Object* p_this) override;
    bool instance_has(const Object* p_this) const override;

    bool has_source_code() const override;
    String get_source_code() const override;
    void set_source_code(const String& p_code) override;
    Error reload(bool p_keep_state = false) override;
    // TODO: load_source_code only allow utf-8 file, should handle bytecode as
    // well ?
    virtual Error load_source_code(const String& p_path);

    bool has_method(const StringName& p_method) const override;
    MethodInfo get_method_info(const StringName& p_method) const override;

    bool has_property(const StringName& p_method) const;
    PropertyInfo get_property_info(const StringName& p_property) const;

    bool is_tool() const override {
        return _tool;
    }

    bool is_valid() const override {
        return true;
    }

    ScriptLanguage* get_language() const override;

    bool has_script_signal(const StringName& p_signal) const override;
    void get_script_signal_list(List<MethodInfo>* r_signals) const override;

    bool get_property_default_value(
        const StringName& p_property,
        Variant& r_value
    ) const override;

    void update_exports() override;
    void get_script_method_list(List<MethodInfo>* r_methods) const override;
    void get_script_property_list(
        List<PropertyInfo>* r_properties
    ) const override;

    int get_member_line(const StringName& p_member) const override;

    MultiplayerAPI::RPCMode get_rpc_mode(const StringName& p_method) const;
    MultiplayerAPI::RPCMode get_rset_mode(const StringName& p_variable) const;

    PluginScript();
    void init(PluginScriptLanguage* language);
    ~PluginScript() override;
};

#endif // PLUGINSCRIPT_SCRIPT_H
