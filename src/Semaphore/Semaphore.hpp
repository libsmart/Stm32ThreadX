/*
 * SPDX-FileCopyrightText: 2026 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "BaseSemaphore.hpp"

namespace Stm32ThreadX {
    class Semaphore : public BaseSemaphore {
    public:
        static constexpr const char *COMPONENT_NAME = Stm32ThreadX::COMPONENT_NAME;
        static constexpr char CLASS_NAME[] = "Semaphore";
        const char *INSTANCE_NAME{getName()};

        Semaphore() = default;

        explicit Semaphore(const char *name)
            : BaseSemaphore(name) { ; }

        explicit Semaphore(Stm32ItmLogger::LoggerInterface *logger)
            : BaseSemaphore(logger) { ; }

        Semaphore(const char *name, Stm32ItmLogger::LoggerInterface *logger)
            : BaseSemaphore(name, logger) { ; }

        /**
         * @brief Constructs a Semaphore object with a specified initial count and name.
         *
         * This constructor initializes the Semaphore with the given initial count and name.
         * It also uses the provided logger for logging purposes. A semaphore object is created
         * with the specified name and initial count value.
         *
         * @warning Only use this in ThreadX context.
         *
         * @param initial_count The initial count value for the semaphore.
         * @param name The name assigned to the semaphore.
         * @param logger A pointer to a LoggerInterface instance used for logging.
         * @return None.
         */
        Semaphore(const ULONG initial_count, const char *name, Stm32ItmLogger::LoggerInterface *logger)
            : BaseSemaphore(name, logger) {
            BaseSemaphore::create(Nameable::getNameNonConst(), initial_count);
        }
    };
}
