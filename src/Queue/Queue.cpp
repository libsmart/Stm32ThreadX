/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Queue.hpp"

using namespace Stm32ThreadX;

UINT Queue::create(UINT message_size) {
    return create(getNameNonConst(), message_size, queueMem, queueMemSize);
}

bool Queue::isEmpty() {
    return getEnqueuedCount() == 0;
}

ULONG Queue::getEnqueuedCount() {
    ULONG enqueued{};
    info_get(nullptr, &enqueued, nullptr, nullptr, nullptr, nullptr);
    return enqueued;
}
