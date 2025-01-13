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

        /**
         * @brief Checks if the queue is empty.
         *
         * This function retrieves the current state of the queue and determines
         * whether there are any enqueued elements. It accomplishes this by calling
         * the `info_get` function to query the queue's attributes, particularly
         * the number of enqueued elements. If the number of enqueued elements is
         * zero, the queue is considered empty.
         *
         * @return True if the queue is empty, false otherwise.
         */
        virtual bool isEmpty();

        using BaseQueue::create;

    private:
        uint8_t *queueMem{};
        size_t queueMemSize{};
    };
}
