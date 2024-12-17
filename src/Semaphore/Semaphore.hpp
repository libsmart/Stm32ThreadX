/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "BaseSemaphore.hpp"

namespace Stm32ThreadX {
    class Semaphore : public BaseSemaphore {
    public:
        Semaphore() = default;

        explicit Semaphore(const char *name)
            : BaseSemaphore(name) { ; }

        explicit Semaphore(Stm32ItmLogger::LoggerInterface *logger)
            : BaseSemaphore(logger) { ; }

        Semaphore(const char *name, Stm32ItmLogger::LoggerInterface *logger)
            : BaseSemaphore(name, logger) { ; }

        Semaphore(ULONG initial_count, const char *name, Stm32ItmLogger::LoggerInterface *logger)
            : BaseSemaphore(name, logger) {
            BaseSemaphore::create(Nameable::getNameNonConst(), initial_count);
        }
    };
}
