// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef DENOISE_WRAPPER_H
#define DENOISE_WRAPPER_H

void* oidn_denoiser_init();
bool oidn_denoise(void* device, float* p_floats, int p_width, int p_height);
void oidn_denoiser_finish(void* device);

#endif // DENOISE_WRAPPER_H
