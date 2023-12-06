// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef RASTERIZER_STORAGE_COMMON_H
#define RASTERIZER_STORAGE_COMMON_H

class RasterizerStorageCommon {
public:
    enum FVF {
        FVF_UNBATCHED,
        FVF_REGULAR,
        FVF_COLOR,
        FVF_LIGHT_ANGLE,
        FVF_MODULATED,
        FVF_LARGE,
    };

    // these flags are specifically for batching
    // some of the logic is thus in rasterizer_storage.cpp
    // we could alternatively set bitflags for each 'uses' and test on the fly
    enum BatchFlags : uint32_t {
        PREVENT_COLOR_BAKING  = 1 << 0,
        PREVENT_VERTEX_BAKING = 1 << 1,

        // custom vertex shaders using BUILTINS that vary per item
        PREVENT_ITEM_JOINING = 1 << 2,

        USE_MODULATE_FVF = 1 << 3,
        USE_LARGE_FVF    = 1 << 4,
    };

    enum BatchType : uint16_t {
        BT_DEFAULT = 0,
        BT_RECT    = 1,
        BT_LINE    = 2,
        BT_LINE_AA = 3,
        BT_POLY    = 4,
        BT_DUMMY   = 5, // dummy batch is just used to keep the batch creation
                        // loop simple
    };

    enum BatchTypeFlags : uint32_t {
        BTF_DEFAULT = 1 << BT_DEFAULT,
        BTF_RECT    = 1 << BT_RECT,
        BTF_LINE    = 1 << BT_LINE,
        BTF_LINE_AA = 1 << BT_LINE_AA,
        BTF_POLY    = 1 << BT_POLY,
    };
};

#endif // RASTERIZER_STORAGE_COMMON_H
