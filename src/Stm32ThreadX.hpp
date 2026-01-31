/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
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

#pragma once

#include <type_traits>
#include <utility>

extern "C" {
#include "tx_api.h"
}

namespace Stm32ThreadX {
    constexpr char COMPONENT_NAME[] = "Stm32ThreadX";

    /**
     * @brief Executes the provided callable object within a critical section.
     *
     * This function ensures that the execution of the provided callable object is atomic by temporarily
     * disabling interrupts. The interrupt state is restored to its original value after execution, even
     * in the event an exception is thrown.
     *
     * @tparam Fn The type of the callable object to be executed.
     *
     * @param fn A callable object to be executed in a critical section. The callable must have a `void` return type.
     *
     * @return void
     *
     * @throw Any exception that occurs during the execution of the callable object will be propagated
     *        after restoring the interrupt state.
     *
     * @note Uses `tx_interrupt_control` to manage the interrupt state.
     */
    template<class Fn>
    static std::enable_if_t<std::is_void_v<std::invoke_result_t<Fn> >, void>
    atomic(Fn &&fn) {
        const auto old_posture = tx_interrupt_control(TX_INT_DISABLE);
        try {
            std::forward<Fn>(fn)();
        } catch (...) {
            tx_interrupt_control(old_posture);
            throw;
        }
        tx_interrupt_control(old_posture);
    }


    /**
     * @brief Executes the provided function within a critical section and returns its result.
     *
     * This function temporarily disables interrupts to ensure atomicity while executing the provided
     * callable object. The interrupt state is restored to its original value after the function execution,
     * even in the case of an exception. If the function returns a value, it is returned to the caller.
     *
     * @param fn A callable object (function) representing the code to be executed in a critical section.
     *           The callable must return a non-void result.
     *
     * @return The result of the provided callable object.
     *
     * @throw Any exception thrown by the provided callable object will be propagated.
     *        The interrupt state will still be restored before the exception is re-thrown.
     *
     * @note The function uses `tx_interrupt_control` to disable and restore the interrupt state.
     */
    template<class Fn>
    static std::enable_if_t<!std::is_void_v<std::invoke_result_t<Fn> >, std::invoke_result_t<Fn> >
    atomic(Fn &&fn) {
        const auto old_posture = tx_interrupt_control(TX_INT_DISABLE);
        try {
            auto ret = std::forward<Fn>(fn)();
            tx_interrupt_control(old_posture);
            return ret;
        } catch (...) {
            tx_interrupt_control(old_posture);
            throw;
        }
    }


    /**
     * @brief Bounce Function
     *
     * This template function acts as a bounce function to call a member function (Method) on an object (T) passed as a
     * pointer using the input thread_input. It forwards any additional parameters to the member function call.
     *
     * @tparam T The class type of the object on which the member function will be called
     * @tparam Method The type of the member function
     * @tparam m Pointer to the member function
     * @tparam Params The types of additional parameters to forward to the member function call
     *
     * @param thread_input The integral value representing the pointer to the object on which the member function will be called
     * @param params The additional parameters to be forwarded to the member function
     *
     * @return The return value of the member function call
     *
     * @note The caller is responsible for ensuring that thread_input represents a valid pointer to an object of type T
     * and the member function pointed to by m is of correct signature and can be called with the provided parameters.
     */
    template<class T, class Method, Method m, class... Params>
    static auto bounce(ULONG thread_input, Params... params) ->
        decltype(((*reinterpret_cast<T *>(thread_input)).*m)(params...)) {
        return ((*reinterpret_cast<T *>(thread_input)).*m)(params...);
    }

#define BOUNCE(c, m) bounce<c, decltype(&c::m), &c::m>
}
