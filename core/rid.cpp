// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "rid.h"

RID_Data::~RID_Data() {}

SafeRefCount RID_OwnerBase::refcount;

void RID_OwnerBase::init_rid() {
    refcount.init();
}
