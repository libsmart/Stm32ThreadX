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


#ifndef LIBSMART_STM32THREADX_RUNTHREADEVERY_HPP
#define LIBSMART_STM32THREADX_RUNTHREADEVERY_HPP

#include <libsmart_config.hpp>
#include <cstdint>
#include <functional>
#include "Helper.hpp"
#include "RunEvery.hpp"
#include "Stm32ThreadX.hpp"
#include "Thread.hpp"

namespace Stm32ThreadX {
    class RunThreadEvery : public Thread, public Stm32Common::RunEvery {
    public:
        explicit RunThreadEvery(uint32_t interval_and_delay_ms)
            : RunThreadEvery(interval_and_delay_ms, interval_and_delay_ms, 0) { ; }

        RunThreadEvery(uint32_t interval_ms, uint32_t delay_ms)
            : RunThreadEvery(interval_ms, delay_ms, 0) { ; }

        RunThreadEvery(uint32_t interval_ms, uint32_t delay_ms, uint32_t run_count_max)
            : Thread(BOUNCE(RunThreadEvery, loopThread),
                     reinterpret_cast<ULONG>(this), Thread::priority(),
                     "Stm32ThreadX::RunThreadEvery"),
              RunEvery(interval_ms, delay_ms, run_count_max) { ; }

#ifdef LIBSMART_ENABLE_STD_FUNCTION
        explicit RunThreadEvery(const fn_t &fn)
            : RunThreadEvery(0, 0, 0, fn) { ; }

        RunThreadEvery(uint32_t interval_and_delay_ms, const fn_t &fn)
            : RunThreadEvery(interval_and_delay_ms, interval_and_delay_ms, 0, fn) { ; }

        RunThreadEvery(uint32_t interval_ms, uint32_t delay_ms, const fn_t &fn)
            : RunThreadEvery(interval_ms, delay_ms, 0, fn) { ; }

        RunThreadEvery(uint32_t interval_ms, uint32_t delay_ms, uint32_t run_count_max, const fn_t &fn)
            : Thread(BOUNCE(RunThreadEvery, loopThread),
                     reinterpret_cast<ULONG>(this), Thread::priority(),
                     "Stm32ThreadX::RunThreadEvery"),
              RunEvery(interval_ms, delay_ms, run_count_max, fn) { ; }
#endif

    protected:
        [[noreturn]] void loopThread() {
            // tx_thread_sleep((_delay_ms * TX_TIMER_TICKS_PER_SECOND) / 1000);
            for (;;) {
                loop();
                // tx_thread_sleep(std::max((_interval_ms * TX_TIMER_TICKS_PER_SECOND) / 1000, 1UL));
                tx_thread_sleep(1);
            }
        }
    };
}

#endif
