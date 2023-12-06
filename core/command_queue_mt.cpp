// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "command_queue_mt.h"

#include "core/os/os.h"
#include "core/project_settings.h"

void CommandQueueMT::lock() {
    mutex.lock();
}

void CommandQueueMT::unlock() {
    mutex.unlock();
}

void CommandQueueMT::wait_for_flush() {
    // wait one millisecond for a flush to happen
    OS::get_singleton()->delay_usec(1000);
}

CommandQueueMT::SyncSemaphore* CommandQueueMT::_alloc_sync_sem() {
    int idx = -1;

    while (true) {
        lock();
        for (int i = 0; i < SYNC_SEMAPHORES; i++) {
            if (!sync_sems[i].in_use) {
                sync_sems[i].in_use = true;
                idx                 = i;
                break;
            }
        }
        unlock();

        if (idx == -1) {
            wait_for_flush();
        } else {
            break;
        }
    }

    return &sync_sems[idx];
}

bool CommandQueueMT::dealloc_one() {
tryagain:
    if (dealloc_ptr == (write_ptr_and_epoch >> 1)) {
        // The queue is empty
        return false;
    }

    uint32_t size = *(uint32_t*)&command_mem[dealloc_ptr];

    if (size == 0) {
        // End of command buffer wrap down
        dealloc_ptr = 0;
        goto tryagain;
    }

    if (size & 1) {
        // Still used, nothing can be deallocated
        return false;
    }

    dealloc_ptr += (size >> 1) + 8;
    return true;
}

CommandQueueMT::CommandQueueMT(bool p_sync) {
    read_ptr_and_epoch  = 0;
    write_ptr_and_epoch = 0;
    dealloc_ptr         = 0;

    command_mem_size = GLOBAL_DEF_RST(
        "memory/limits/command_queue/multithreading_queue_size_kb",
        DEFAULT_COMMAND_MEM_SIZE_KB
    );
    ProjectSettings::get_singleton()->set_custom_property_info(
        "memory/limits/command_queue/multithreading_queue_size_kb",
        PropertyInfo(
            Variant::INT,
            "memory/limits/command_queue/multithreading_queue_size_kb",
            PROPERTY_HINT_RANGE,
            "1,4096,1,or_greater"
        )
    );
    command_mem_size *= 1024;
    command_mem       = (uint8_t*)memalloc(command_mem_size);

    for (int i = 0; i < SYNC_SEMAPHORES; i++) {
        sync_sems[i].in_use = false;
    }
    if (p_sync) {
        sync = memnew(Semaphore);
    } else {
        sync = nullptr;
    }
}

CommandQueueMT::~CommandQueueMT() {
    if (sync) {
        memdelete(sync);
    }
    memfree(command_mem);
}
