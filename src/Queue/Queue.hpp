/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "BaseQueue.hpp"

namespace Stm32ThreadX {
    class Queue : public BaseQueue {
    public:
        Queue() = default;

        explicit Queue(uint8_t *queue_mem, size_t queue_mem_size)
            : queueMem(queue_mem), queueMemSize(queue_mem_size) { ; }

        Queue(const char *name, uint8_t *queue_mem, size_t queue_mem_size)
            : BaseQueue(name), queueMem(queue_mem), queueMemSize(queue_mem_size) { ; }

        Queue(uint8_t *queue_mem, size_t queue_mem_size, Stm32ItmLogger::LoggerInterface *logger)
            : BaseQueue(logger), queueMem(queue_mem), queueMemSize(queue_mem_size) { ; }

        Queue(const char *name, uint8_t *queue_mem, size_t queue_mem_size, Stm32ItmLogger::LoggerInterface *logger)
            : BaseQueue(name, logger), queueMem(queue_mem), queueMemSize(queue_mem_size) { ; }


        virtual UINT create(UINT message_size);

        using BaseQueue::create;

    private:
        uint8_t *queueMem{};
        size_t queueMemSize{};
    };
}
