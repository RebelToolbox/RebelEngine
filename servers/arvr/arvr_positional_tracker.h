// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef ARVR_POSITIONAL_TRACKER_H
#define ARVR_POSITIONAL_TRACKER_H

#include "core/os/thread_safe.h"
#include "scene/resources/mesh.h"
#include "servers/arvr_server.h"

/**
    @author Bastiaan Olij <mux213@gmail.com>

    The positional tracker object as an object that represents the position and
   orientation of a tracked object like a controller or headset. An AR/VR
   Interface will registered the trackers it manages with our AR/VR server and
   update its position and orientation. This is where potentially additional
   AR/VR interfaces may be active as there are AR/VR SDKs that solely deal with
   positional tracking.
*/

class ARVRPositionalTracker : public Reference {
    GDCLASS(ARVRPositionalTracker, Reference);
    _THREAD_SAFE_CLASS_

public:
    enum TrackerHand {
        TRACKER_HAND_UNKNOWN, /* unknown or not applicable */
        TRACKER_LEFT_HAND,    /* controller is the left hand controller */
        TRACKER_RIGHT_HAND    /* controller is the right hand controller */
    };

private:
    ARVRServer::TrackerType type; // type of tracker
    StringName name;              // (unique) name of the tracker
    int tracker_id;               // tracker index id that is unique per type
    int joy_id; // if we also have a related joystick entity, the id of the
                // joystick
    bool tracks_orientation; // do we track orientation?
    Basis orientation;       // our orientation
    bool tracks_position;    // do we track position?
    Vector3 rw_position;     // our position "in the real world, so without
                             // world_scale applied"
    Ref<Mesh>
        mesh; // when available, a mesh that can be used to render this tracker
    TrackerHand hand; // if known, the hand this tracker is held in
    real_t
        rumble; // rumble strength, 0.0 is off, 1.0 is maximum, note that we
                // only record here, arvr_interface is responsible for execution

protected:
    static void _bind_methods();

public:
    void set_type(ARVRServer::TrackerType p_type);
    ARVRServer::TrackerType get_type() const;
    void set_name(const String& p_name);
    StringName get_name() const;
    int get_tracker_id() const;
    void set_joy_id(int p_joy_id);
    int get_joy_id() const;
    bool get_tracks_orientation() const;
    void set_orientation(const Basis& p_orientation);
    Basis get_orientation() const;
    bool get_tracks_position() const;
    void set_position(const Vector3& p_position
    );                            // set position with world_scale applied
    Vector3 get_position() const; // get position with world_scale applied
    void set_rw_position(const Vector3& p_rw_position);
    Vector3 get_rw_position() const;
    ARVRPositionalTracker::TrackerHand get_hand() const;
    void set_hand(const ARVRPositionalTracker::TrackerHand p_hand);
    real_t get_rumble() const;
    void set_rumble(real_t p_rumble);
    void set_mesh(const Ref<Mesh>& p_mesh);
    Ref<Mesh> get_mesh() const;

    Transform get_transform(bool p_adjust_by_reference_frame) const;

    ARVRPositionalTracker();
    ~ARVRPositionalTracker();
};

VARIANT_ENUM_CAST(ARVRPositionalTracker::TrackerHand);

#endif
