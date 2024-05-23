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
            ->printf("Stm32ThreadX::EventFlags[%s]::create()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_create
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
            ->printf("Stm32ThreadX::EventFlags[%s]::deleteFlags()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_delete
    const auto ret = tx_event_flags_delete(this);
    if (ret != TX_SUCCESS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Event flags group '%s' deletion failed. tx_event_flags_delete() = 0x%02x\r\n",
                         getName(), ret);
    }

    return ret;
}

UINT EventFlags::get(ULONG requestedFlags, getOption_t getOption, waitOption_t waitOption) {
    // log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
    //         ->printf("Stm32ThreadX::EventFlags[%s]::get()\r\n", getName());

    actualFlags = 0;

    const auto ret = tx_event_flags_get(
        this,
        requestedFlags,
        static_cast<UINT>(getOption),
        &actualFlags,
        waitOption.timeout
    );
    if (ret != TX_SUCCESS && ret != TX_NO_EVENTS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Event flags group '%s' get failed. tx_event_flags_get() = 0x%02x\r\n",
                         getName(), ret);
    }
    return ret;
}

UINT EventFlags::get(ULONG requestedFlags) {
    return get(requestedFlags, getOption_t::AND, waitOption_t{waitOption_t::NO_WAIT});
}

ULONG EventFlags::getFlags(ULONG requestedFlags) {
    get(requestedFlags, getOption_t::OR, waitOption_t{waitOption_t::NO_WAIT});
    return actualFlags;
}

bool EventFlags::isSet(ULONG requestedFlags) {
    return isSet(requestedFlags, getOption_t::AND);
}

bool EventFlags::isSet(ULONG requestedFlags, getOption_t getOption) {
    return get(requestedFlags, getOption, waitOption_t{waitOption_t::NO_WAIT}) == TX_SUCCESS;
}

UINT EventFlags::await(ULONG requestedFlags) {
    return await(requestedFlags, waitOption_t{waitOption_t::WAIT_FOREVER});
}

UINT EventFlags::await(ULONG requestedFlags, waitOption_t waitOption) {
    return await(requestedFlags, getOption_t::AND, waitOption);
}

UINT EventFlags::await(ULONG requestedFlags, getOption_t getOption, waitOption_t waitOption) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::EventFlags[%s]::await(0x%08x)\r\n", getName(), requestedFlags);

    return get(requestedFlags, getOption, waitOption);
}

UINT EventFlags::set(ULONG flagsToSet, setOption_t setOption) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::EventFlags[%s]::set(0x%08x)\r\n", getName(), flagsToSet);

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
