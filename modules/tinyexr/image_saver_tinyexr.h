// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IMAGE_SAVER_TINYEXR_H
#define IMAGE_SAVER_TINYEXR_H

#include "core/os/os.h"

Error save_exr(const String& p_path, const Ref<Image>& p_img, bool p_grayscale);

#endif // IMAGE_SAVER_TINYEXR_H
