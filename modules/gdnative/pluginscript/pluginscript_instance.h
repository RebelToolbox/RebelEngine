// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PLUGINSCRIPT_INSTANCE_H
#define PLUGINSCRIPT_INSTANCE_H

// Rebel imports
#include "core/script_language.h"

// PluginScript imports
#include <pluginscript/rebel_pluginscript.h>

class PluginScript;

class PluginScriptInstance : public ScriptInstance {
    friend class PluginScript;

private:
    Ref<PluginScript> _script;
    Object* _owner;
    Variant _owner_variant;
    rebel_pluginscript_instance_data* _data;
    const rebel_pluginscript_instance_desc* _desc;

public:
    _FORCE_INLINE_ Object* get_owner() override {
        return _owner;
    }

    bool set(const StringName& p_name, const Variant& p_value) override;
    bool get(const StringName& p_name, Variant& r_ret) const override;
    void get_property_list(List<PropertyInfo>* p_properties) const override;
    Variant::Type get_property_type(
        const StringName& p_name,
        bool* r_is_valid = nullptr
    ) const override;

    void get_method_list(List<MethodInfo>* p_list) const override;
    bool has_method(const StringName& p_method) const override;

    Variant call(
        const StringName& p_method,
        const Variant** p_args,
        int p_argcount,
        Variant::CallError& r_error
    ) override;

    // Rely on default implementations provided by ScriptInstance for the
    // moment. Note that multilevel call could be removed in 3.0 release, so
    // stay tuned (see
    // https://godotengine.org/qa/9244/can-override-the-_ready-and-_process-functions-child-classes)
    // virtual void call_multilevel(const StringName& p_method,const Variant**
    // p_args,int p_argcount); virtual void call_multilevel_reversed(const
    // StringName& p_method,const Variant** p_args,int p_argcount);

    void notification(int p_notification) override;

    Ref<Script> get_script() const override;

    ScriptLanguage* get_language() override;

    void set_path(const String& p_path);

    MultiplayerAPI::RPCMode get_rpc_mode(
        const StringName& p_method
    ) const override;
    MultiplayerAPI::RPCMode get_rset_mode(
        const StringName& p_variable
    ) const override;

    void refcount_incremented() override;
    bool refcount_decremented() override;

    PluginScriptInstance();
    bool init(PluginScript* p_script, Object* p_owner);
    ~PluginScriptInstance() override;
};

#endif // PLUGINSCRIPT_INSTANCE_H
