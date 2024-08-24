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

#ifndef LIBSMART_STM32THREADX_RUNTHREAD_HPP
#define LIBSMART_STM32THREADX_RUNTHREAD_HPP

#include <libsmart_config.hpp>
#include <cstdint>
#include <functional>
#include "Helper.hpp"
#include "RunOnce.hpp"
#include "Stm32ThreadX.hpp"
#include "Thread.hpp"

namespace Stm32ThreadX {
    class RunThreadOnce : public Thread, public Stm32Common::RunOnce {
    public:
        RunThreadOnce()
            : RunThreadOnce(0) { ; }

        explicit RunThreadOnce(uint32_t delay_ms)
            : Thread(BOUNCE(RunThreadOnce, loopThread),
                     reinterpret_cast<ULONG>(this), Thread::priority(),
                     "Stm32ThreadX::RunThreadEvery"),
              RunOnce(delay_ms) { ; }

#ifdef LIBSMART_ENABLE_STD_FUNCTION
        explicit RunThreadOnce(const fn_t &fn)
            : RunThreadOnce(0, fn) { ; }

        RunThreadOnce(uint32_t delay_ms, const fn_t &fn)
            : Thread(BOUNCE(RunThreadOnce, loopThread),
                     reinterpret_cast<ULONG>(this), Thread::priority(),
                     "Stm32ThreadX::RunThreadOnce"),
              RunOnce(delay_ms, fn) { ; }

#endif

    protected:
        void loopThread() {
            // tx_thread_sleep((_delay_ms * TX_TIMER_TICKS_PER_SECOND) / 1000);
            do {
                loop();
                // tx_thread_sleep(std::max((_interval_ms * TX_TIMER_TICKS_PER_SECOND) / 1000, 1UL));
                tx_thread_sleep(1);
            } while(_run_count < 1);
            terminate();
        }
    };
}
#endif
