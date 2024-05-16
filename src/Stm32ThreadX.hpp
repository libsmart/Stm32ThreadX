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

#ifndef LIBSMART_STM32THREADX_STM32THREADX_HPP
#define LIBSMART_STM32THREADX_STM32THREADX_HPP

#include "tx_api.h"

namespace Stm32ThreadX {
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
#endif //LIBSMART_STM32THREADX_STM32THREADX_HPP
