// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AREA_BULLET_H
#define AREA_BULLET_H

#include "collision_object_bullet.h"
#include "core/vector.h"
#include "servers/physics_server.h"
#include "space_bullet.h"

class btGhostObject;

class AreaBullet : public RigidCollisionObjectBullet {
    friend void SpaceBullet::check_ghost_overlaps();

public:
    struct InOutEventCallback {
        ObjectID event_callback_id;
        StringName event_callback_method;

        InOutEventCallback() : event_callback_id(0) {}
    };

    enum OverlapState {
        OVERLAP_STATE_DIRTY = 0, // Mark processed overlaps
        OVERLAP_STATE_INSIDE,    // Mark old overlap
        OVERLAP_STATE_ENTER,     // Mark just enter overlap
        OVERLAP_STATE_EXIT       // Mark ended overlaps
    };

    struct OverlappingObjectData {
        CollisionObjectBullet* object;
        OverlapState state;

        OverlappingObjectData() : object(nullptr), state(OVERLAP_STATE_ENTER) {}

        OverlappingObjectData(
            CollisionObjectBullet* p_object,
            OverlapState p_state
        ) :
            object(p_object),
            state(p_state) {}

        OverlappingObjectData(const OverlappingObjectData& other) {
            operator=(other);
        }

        void operator=(const OverlappingObjectData& other) {
            object = other.object;
            state  = other.state;
        }
    };

private:
    // These are used by function callEvent. Instead to create this each call I
    // create if one time.
    Variant call_event_res[5];
    Variant* call_event_res_ptr[5];

    btGhostObject* btGhost;
    Vector<OverlappingObjectData> overlappingObjects;
    bool monitorable;

    PhysicsServer::AreaSpaceOverrideMode spOv_mode;
    bool spOv_gravityPoint;
    real_t spOv_gravityPointDistanceScale;
    real_t spOv_gravityPointAttenuation;
    Vector3 spOv_gravityVec;
    real_t spOv_gravityMag;
    real_t spOv_linearDump;
    real_t spOv_angularDump;
    int spOv_priority;

    bool isScratched;

    InOutEventCallback eventsCallbacks[2];

public:
    AreaBullet();
    ~AreaBullet();

    _FORCE_INLINE_ btGhostObject* get_bt_ghost() const {
        return btGhost;
    }

    int find_overlapping_object(CollisionObjectBullet* p_colObj);

    void set_monitorable(bool p_monitorable);

    _FORCE_INLINE_ bool is_monitorable() const {
        return monitorable;
    }

    bool is_monitoring() const;

    _FORCE_INLINE_ void set_spOv_mode(
        PhysicsServer::AreaSpaceOverrideMode p_mode
    ) {
        spOv_mode = p_mode;
    }

    _FORCE_INLINE_ PhysicsServer::AreaSpaceOverrideMode get_spOv_mode() {
        return spOv_mode;
    }

    _FORCE_INLINE_ void set_spOv_gravityPoint(bool p_isGP) {
        spOv_gravityPoint = p_isGP;
    }

    _FORCE_INLINE_ bool is_spOv_gravityPoint() {
        return spOv_gravityPoint;
    }

    _FORCE_INLINE_ void set_spOv_gravityPointDistanceScale(real_t p_GPDS) {
        spOv_gravityPointDistanceScale = p_GPDS;
    }

    _FORCE_INLINE_ real_t get_spOv_gravityPointDistanceScale() {
        return spOv_gravityPointDistanceScale;
    }

    _FORCE_INLINE_ void set_spOv_gravityPointAttenuation(real_t p_GPA) {
        spOv_gravityPointAttenuation = p_GPA;
    }

    _FORCE_INLINE_ real_t get_spOv_gravityPointAttenuation() {
        return spOv_gravityPointAttenuation;
    }

    _FORCE_INLINE_ void set_spOv_gravityVec(Vector3 p_vec) {
        spOv_gravityVec = p_vec;
    }

    _FORCE_INLINE_ const Vector3& get_spOv_gravityVec() const {
        return spOv_gravityVec;
    }

    _FORCE_INLINE_ void set_spOv_gravityMag(real_t p_gravityMag) {
        spOv_gravityMag = p_gravityMag;
    }

    _FORCE_INLINE_ real_t get_spOv_gravityMag() {
        return spOv_gravityMag;
    }

    _FORCE_INLINE_ void set_spOv_linearDump(real_t p_linearDump) {
        spOv_linearDump = p_linearDump;
    }

    _FORCE_INLINE_ real_t get_spOv_linearDamp() {
        return spOv_linearDump;
    }

    _FORCE_INLINE_ void set_spOv_angularDump(real_t p_angularDump) {
        spOv_angularDump = p_angularDump;
    }

    _FORCE_INLINE_ real_t get_spOv_angularDamp() {
        return spOv_angularDump;
    }

    _FORCE_INLINE_ void set_spOv_priority(int p_priority) {
        spOv_priority = p_priority;
    }

    _FORCE_INLINE_ int get_spOv_priority() {
        return spOv_priority;
    }

    virtual void main_shape_changed();
    virtual void reload_body();
    virtual void set_space(SpaceBullet* p_space);

    virtual void dispatch_callbacks();
    void call_event(
        CollisionObjectBullet* p_otherObject,
        PhysicsServer::AreaBodyStatus p_status
    );
    void set_on_state_change(
        ObjectID p_id,
        const StringName& p_method,
        const Variant& p_udata = Variant()
    );
    void scratch();

    void clear_overlaps(bool p_notify);
    // Dispatch the callbacks and removes from overlapping list
    void remove_overlap(CollisionObjectBullet* p_object, bool p_notify);

    virtual void on_collision_filters_change();

    virtual void on_collision_checker_start() {}

    virtual void on_collision_checker_end() {
        isTransformChanged = false;
    }

    void add_overlap(CollisionObjectBullet* p_otherObject);
    void put_overlap_as_exit(int p_index);
    void put_overlap_as_inside(int p_index);

    void set_param(
        PhysicsServer::AreaParameter p_param,
        const Variant& p_value
    );
    Variant get_param(PhysicsServer::AreaParameter p_param) const;

    void set_event_callback(
        Type p_callbackObjectType,
        ObjectID p_id,
        const StringName& p_method
    );
    bool has_event_callback(Type p_callbackObjectType);

    virtual void on_enter_area(AreaBullet* p_area);
    virtual void on_exit_area(AreaBullet* p_area);
};

#endif // AREA_BULLET_H
