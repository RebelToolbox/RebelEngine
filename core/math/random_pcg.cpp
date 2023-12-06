// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "random_pcg.h"

#include "core/os/os.h"

RandomPCG::RandomPCG(uint64_t p_seed, uint64_t p_inc) :
    pcg(),
    current_inc(p_inc) {
    seed(p_seed);
}

void RandomPCG::randomize() {
    seed(
        (OS::get_singleton()->get_unix_time()
         + OS::get_singleton()->get_ticks_usec())
            * pcg.state
        + PCG_DEFAULT_INC_64
    );
}

double RandomPCG::random(double p_from, double p_to) {
    return randd() * (p_to - p_from) + p_from;
}

float RandomPCG::random(float p_from, float p_to) {
    return randf() * (p_to - p_from) + p_from;
}
