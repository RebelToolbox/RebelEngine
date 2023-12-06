// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MODEL_ABSTRACTION_H
#define MODEL_ABSTRACTION_H

#include "modules/fbx/fbx_parser/FBXDocument.h"

struct ModelAbstraction {
    mutable const FBXDocParser::Model* fbx_model = nullptr;

    void set_model(const FBXDocParser::Model* p_model) {
        fbx_model = p_model;
    }

    bool has_model() const {
        return fbx_model != nullptr;
    }

    const FBXDocParser::Model* get_model() const {
        return fbx_model;
    }
};

#endif // MODEL_ABSTRACTION_H
