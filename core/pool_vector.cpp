// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "pool_vector.h"

Mutex pool_vector_lock;

PoolAllocator* MemoryPool::memory_pool = nullptr;
uint8_t* MemoryPool::pool_memory       = nullptr;
size_t* MemoryPool::pool_size          = nullptr;

MemoryPool::Alloc* MemoryPool::allocs    = nullptr;
MemoryPool::Alloc* MemoryPool::free_list = nullptr;
uint32_t MemoryPool::alloc_count         = 0;
uint32_t MemoryPool::allocs_used         = 0;
Mutex MemoryPool::alloc_mutex;

size_t MemoryPool::total_memory = 0;
size_t MemoryPool::max_memory   = 0;

void MemoryPool::setup(uint32_t p_max_allocs) {
    allocs      = memnew_arr(Alloc, p_max_allocs);
    alloc_count = p_max_allocs;
    allocs_used = 0;

    for (uint32_t i = 0; i < alloc_count - 1; i++) {
        allocs[i].free_list = &allocs[i + 1];
    }

    free_list = &allocs[0];
}

void MemoryPool::cleanup() {
    memdelete_arr(allocs);

    ERR_FAIL_COND_MSG(
        allocs_used > 0,
        "There are still MemoryPool allocs in use at exit!"
    );
}
