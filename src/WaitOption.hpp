/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32THREADX_WAITOPTION_HPP
#define LIBSMART_STM32THREADX_WAITOPTION_HPP

#include "tx_api.h"

namespace Stm32ThreadX {
    class WaitOption {
    public:
        explicit WaitOption(ULONG value)
            : value(value) {
        }

        ULONG operator()() const {
            return value;
        }

        enum : ULONG {
            /** Immediate return from this service */
            NO_WAIT = 0,
            /** Suspend indefinitely until the event flags are available */
            WAIT_FOREVER = 0xFFFFFFFF
        };

    private:
        ULONG value = WAIT_FOREVER;
    };
}
#endif //LIBSMART_STM32THREADX_WAITOPTION_HPP
