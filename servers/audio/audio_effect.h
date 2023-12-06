// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef AUDIOEFFECT_H
#define AUDIOEFFECT_H

#include "core/math/audio_frame.h"
#include "core/resource.h"

class AudioEffectInstance : public Reference {
    GDCLASS(AudioEffectInstance, Reference);

public:
    virtual void process(
        const AudioFrame* p_src_frames,
        AudioFrame* p_dst_frames,
        int p_frame_count
    ) = 0;

    virtual bool process_silence() const {
        return false;
    }
};

class AudioEffect : public Resource {
    GDCLASS(AudioEffect, Resource);

public:
    virtual Ref<AudioEffectInstance> instance() = 0;
    AudioEffect();
};

#endif // AUDIOEFFECT_H
