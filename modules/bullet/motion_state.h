// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MOTION_STATE_H
#define MOTION_STATE_H

#include "rigid_body_bullet.h"

#include <LinearMath/btMotionState.h>

class RigidBodyBullet;

// This class is responsible to move kinematic actor
// and sincronize rendering engine with Bullet
/// DOC:
/// http://www.bulletphysics.org/mediawiki-1.5.8/index.php/MotionStates#What.27s_a_MotionState.3F
class MotionState : public btMotionState {
    /// This data is used to store the new world position for kinematic body
    btTransform bodyKinematicWorldTransf;
    /// This data is used to store last world position
    btTransform bodyCurrentWorldTransform;

    RigidBodyBullet* owner;

public:
    MotionState(RigidBodyBullet* p_owner) :
        bodyKinematicWorldTransf(
            btMatrix3x3(1., 0., 0., 0., 1., 0., 0., 0., 1.),
            btVector3(0., 0., 0.)
        ),
        bodyCurrentWorldTransform(
            btMatrix3x3(1., 0., 0., 0., 1., 0., 0., 0., 1.),
            btVector3(0., 0., 0.)
        ),
        owner(p_owner) {}

    /// IMPORTANT DON'T USE THIS FUNCTION TO KNOW THE CURRENT BODY TRANSFORM
    /// This class is used internally by Bullet
    /// Use MotionState::getCurrentWorldTransform to know current position
    ///
    /// This function is used by Bullet to get the position of object in the
    /// world if the body is kinematic Bullet will move the object to this
    /// location if the body is static Bullet doesn't move at all
    virtual void getWorldTransform(btTransform& worldTrans) const {
        worldTrans = bodyKinematicWorldTransf;
    }

    /// IMPORTANT: to move the body use: moveBody
    /// IMPORTANT: DON'T CALL THIS FUNCTION, IT IS CALLED BY BULLET TO UPDATE
    /// RENDERING ENGINE
    ///
    /// This function is called each time by Bullet and set the current position
    /// of body inside the physics world. Don't allow Rebel rendering scene
    /// takes world transform from this object because the correct transform is
    /// set by Bullet only after the last step when there are sub steps This
    /// function must update Rebel transform rendering scene for this object.
    virtual void setWorldTransform(const btTransform& worldTrans) {
        bodyCurrentWorldTransform = worldTrans;

        owner->notify_transform_changed();
    }

public:
    /// Use this function to move kinematic body
    /// -- or set initial transform before body creation.
    void moveBody(const btTransform& newWorldTransform) {
        bodyKinematicWorldTransf = newWorldTransform;
    }

    /// It returns the current body transform from last Bullet update
    const btTransform& getCurrentWorldTransform() const {
        return bodyCurrentWorldTransform;
    }
};

#endif // MOTION_STATE_H