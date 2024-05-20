/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "EventFlags.hpp"

using namespace Stm32ThreadX;

EventFlags::~EventFlags() {
    deleteFlags();
}

UINT EventFlags::create() {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::EventFlags::create(%s)\r\n", getName());

    const auto ret = tx_event_flags_create(this, const_cast<char *>(getName()));
    if (ret != TX_SUCCESS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Event flags group '%s' creation failed. tx_event_flags_create() = 0x%02x\r\n",
                         getName(), ret);
    }
    return ret;
}

UINT EventFlags::deleteFlags() {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::EventFlags::deleteFlags(%s)\r\n", getName());

    const auto ret = tx_event_flags_delete(this);
    if (ret != TX_SUCCESS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Event flags group '%s' deletion failed. tx_event_flags_delete() = 0x%02x\r\n",
                         getName(), ret);
    }
    return ret;
}

UINT EventFlags::get(ULONG requestedFlags, getOption_t getOption, waitOption_t waitOption) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::EventFlags::get(%s)\r\n", getName());

    actualFlags = 0;

    const auto ret = tx_event_flags_get(
        this,
        requestedFlags,
        static_cast<UINT>(getOption),
        &actualFlags,
        waitOption.timeout
    );
    if (ret != TX_SUCCESS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Event flags group '%s' get failed. tx_event_flags_get() = 0x%02x\r\n",
                         getName(), ret);
    }
    return ret;
}

UINT EventFlags::set(ULONG flagsToSet, setOption_t setOption) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::EventFlags::set(%s)\r\n", getName());

    const auto ret = tx_event_flags_set(
        this,
        flagsToSet,
        static_cast<UINT>(setOption)
    );
    if (ret != TX_SUCCESS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Event flags group '%s' set failed. tx_event_flags_set() = 0x%02x\r\n",
                         getName(), ret);
    }
    return ret;
}
