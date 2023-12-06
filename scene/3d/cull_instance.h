// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef CULL_INSTANCE_H
#define CULL_INSTANCE_H

#include "scene/3d/spatial.h"

class CullInstance : public Spatial {
    GDCLASS(CullInstance, Spatial);

public:
    enum PortalMode {
        PORTAL_MODE_STATIC,  // not moving within a room
        PORTAL_MODE_DYNAMIC, // moving within room
        PORTAL_MODE_ROAMING, // moving between rooms
        PORTAL_MODE_GLOBAL,  // frustum culled only
        PORTAL_MODE_IGNORE,  // don't show at all - e.g. manual bounds, hidden
                             // portals
    };

    void set_portal_mode(CullInstance::PortalMode p_mode);
    CullInstance::PortalMode get_portal_mode() const;

    void set_include_in_bound(bool p_enable) {
        _include_in_bound = p_enable;
    }

    bool get_include_in_bound() const {
        return _include_in_bound;
    }

    void set_portal_autoplace_priority(int p_priority) {
        _portal_autoplace_priority = p_priority;
    }

    int get_portal_autoplace_priority() const {
        return _portal_autoplace_priority;
    }

    CullInstance();

protected:
    virtual void _refresh_portal_mode() = 0;

    static void _bind_methods();

private:
    PortalMode _portal_mode;
    bool _include_in_bound;

    // Allows instances to prefer to be autoplaced
    // in specific RoomGroups. This allows building exteriors
    // to be autoplaced in outside RoomGroups, allowing a complete
    // exterior / interior of building in one reusable Scene.
    // The default value 0 gives no preference (chooses the highest priority).
    // All other values will autoplace in the selected RoomGroup priority by
    // preference.
    int _portal_autoplace_priority;
};

#endif
