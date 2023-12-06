// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef LISTENER_H
#define LISTENER_H

#include "scene/3d/spatial.h"
#include "scene/main/viewport.h"

class Listener : public Spatial {
    GDCLASS(Listener, Spatial);

private:
    bool force_change;
    bool current;

    RID scenario_id;

    friend class Viewport;
    void _update_audio_listener_state();

protected:
    void _update_listener();
    virtual void _request_listener_update();

    bool _set(const StringName& p_name, const Variant& p_value);
    bool _get(const StringName& p_name, Variant& r_ret) const;
    void _get_property_list(List<PropertyInfo>* p_list) const;
    void _notification(int p_what);

    static void _bind_methods();

public:
    void make_current();
    void clear_current();
    bool is_current() const;

    virtual Transform get_listener_transform() const;

    void set_visible_layers(uint32_t p_layers);
    uint32_t get_visible_layers() const;

    Listener();
    ~Listener();
};

#endif
