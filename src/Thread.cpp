/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 *
 * This file is part of libsmart/Stm32ThreadxThread, which is distributed under the terms
 * of the BSD 3-Clause License. You should have received a copy of the BSD 3-Clause
 * License along with libsmart/Stm32ThreadxThread. If not, see <https://spdx.org/licenses/BSD-3-Clause.html>.
 *
 * ----------------------------------------------------------------------------
 * Portions of the code are derived from Benedek Kupper's work,
 * which is licensed under the MIT License. You can find the original work at:
 * <https://github.com/IntergatedCircuits/threadx-mcpp/>
 *
 * Portions of the code are derived from Embedded Artistry's work,
 * which is dedicated to the public domain under the CC0 1.0 Universal (CC0 1.0) Public Domain Dedication.
 * You can find the original work at: <https://github.com/embeddedartistry/embedded-resources>
 * ----------------------------------------------------------------------------
 */

#include <cassert>
#include "Thread.hpp"
#include "main.hpp"

using namespace Stm32ThreadX;
using namespace Stm32ThreadX::native;

void Thread::createThread() {
    // https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_thread_create
    assert_param(pstack != nullptr);
    assert_param(stack_size > 0);
    auto result = tx_thread_create(
        this, // TX_THREAD *thread_ptr
        const_cast<char *>(name), // CHAR *name_ptr
        func, // VOID (*entry_function)(ULONG id)
        param, // ULONG entry_input
        pstack, // VOID *stack_start
        stack_size, // ULONG stack_size
        prio, // UINT priority
        prio, // UINT preempt_threshold
        TX_NO_TIME_SLICE, // ULONG time_slice
        TX_DONT_START); // UINT auto_start
    assert_param(result == TX_SUCCESS);
}


Thread::~Thread() {
    if (tx_thread_state != TX_COMPLETED) {
        const auto result = tx_thread_terminate(this);
        assert_param(result == TX_SUCCESS);
    }
    const auto result = tx_thread_delete(this);
    assert_param(result == TX_SUCCESS);
}


void Thread::suspend() {
    tx_thread_suspend(this);
}

void Thread::resume() {
    const auto result = tx_thread_resume(this);
    assert_param(result == TX_SUCCESS);
}

void Thread::terminate() {
    tx_thread_terminate(this);
}

void Thread::reset() {
    tx_thread_reset(this);
}

Thread::priority Thread::getPriority() const {
    return tx_thread_user_priority;
}

void Thread::setPriority(priority prio) {
    priority::value_type old_prio;
    tx_thread_priority_change(this, prio, &old_prio);
}

void Thread::setStack(void *stackPointer, const std::uint32_t stackSize) {
    assert_param(stackPointer != nullptr);
    assert_param(stackSize > 0);
    pstack = stackPointer;
    stack_size = stackSize;
}

Thread::id Thread::getId() const {
    return id(this);
}

const char *Thread::getName() {
    return const_cast<const char *>(tx_thread_name);
}


Thread::state Thread::getState() const {
    state s;
    switch (tx_thread_state) {
        case TX_READY:
            s = (getCurrent() == this) ? state::running : state::ready;
            break;
        case TX_COMPLETED:
            s = state::completed;
            break;
        case TX_TERMINATED:
            s = state::terminated;
            break;
        default:
            s = state::suspended;
            break;
    }
    return s;
}

Thread *Thread::getCurrent() {
    return reinterpret_cast<Thread *>(tx_thread_identify());
}

void this_thread::yield() {
    tx_thread_relinquish();
}

Thread::id this_thread::getId() {
    return Thread::getCurrent()->getId();
}

void this_thread::sleepFor(tick_timer::duration rel_time) {
    auto result = tx_thread_sleep(toTicks(rel_time));
    assert(result == TX_SUCCESS);
}


#ifndef TX_DISABLE_NOTIFY_CALLBACKS

void thread::set_entry_exit_callback(entry_exit_callback func, void* param)
    {
        if (TX_SUCCESS == tx_thread_entry_exit_notify(this, reinterpret_cast<void(*)(TX_THREAD *, unsigned)>(func)))
        {
            // TODO: make sure that tx_user.h contains this:
            // #define TX_THREAD_USER_EXTENSION     void* entry_exit_param_;
            entry_exit_param_ = param;
        }
    }

// fun name collision between macro used right above, and member variable used below
#undef tx_thread_entry_exit_notify

    Thread::entry_exit_callback thread::get_entry_exit_callback() const
    {
        return reinterpret_cast<entry_exit_callback>(this->tx_thread_entry_exit_notify);
    }

    void* Thread::get_entry_exit_param() const
    {
        return this->entry_exit_param_;
    }

    bool Thread::joinable() const
    {
        auto state = get_state();
        return (state != state::completed) && (state != state::terminated) && (nullptr == get_entry_exit_param());
    }

    void Thread::join_exit_callback(thread *t, UINT id)
    {
        if (id == TX_THREAD_EXIT)
        {
            auto *exit_cond = reinterpret_cast<semaphore*>(t->get_entry_exit_param());

            exit_cond->release();
        }
    }

    void Thread::join()
    {
        assert(joinable()); // else invalid_argument
        assert(this->get_id() != this_thread::get_id()); // else resource_deadlock_would_occur

        binary_semaphore exit_cond;
        set_entry_exit_callback(&thread::join_exit_callback, reinterpret_cast<void*>(&exit_cond));

        // wait for signal from thread exit
        exit_cond.acquire();

        // signal received, thread is deleted, return
    }

#endif // !TX_DISABLE_NOTIFY_CALLBACKS
