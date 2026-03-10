/*
 * SPDX-FileCopyrightText: 2026 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "IsrSemaphore.hpp"

using namespace Stm32ThreadX;
using Severity = Stm32ItmLogger::LoggerInterface::Severity;

UINT IsrSemaphore::get(const ULONG wait_option) {
    return semaphore.get(wait_option);
}

void IsrSemaphore::setup() {
    semaphore.create(getName());
}

void IsrSemaphore::loop() { ; }

void IsrSemaphore::end() {
    semaphore.del();
}

void IsrSemaphore::errorHandler() { ; }

const char *IsrSemaphore::getDefaultName() const {
    return CLASS_NAME;
}

void IsrSemaphore::callback() {
    if (semaphore.isCreated()) semaphore.put();
}
