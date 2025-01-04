/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "EventFlags.hpp"
#if __EXCEPTIONS
#include <stdexcept>
#endif

using namespace Stm32ThreadX;

UINT EventFlags::create() {
    return BaseEventFlags::create(getNameNonConst());
}

UINT EventFlags::deleteFlags() {
    return BaseEventFlags::del();
}

UINT EventFlags::get(const ULONG requestedFlags, getOption_t getOption, waitOption_t waitOption) {
    ULONG actualFlagsRef{};
    return get(requestedFlags, getOption, actualFlagsRef, waitOption);
}

UINT EventFlags::get(ULONG requestedFlags, getOption_t getOption, ULONG &actualFlagsRef, waitOption_t waitOption) {
    actualFlags = 0;
    const auto ret = BaseEventFlags::get(requestedFlags, static_cast<UINT>(getOption), &actualFlags, waitOption.timeout);
    actualFlagsRef = actualFlags;
    return ret;
}

UINT EventFlags::get(const ULONG requestedFlags) {
    return get(requestedFlags, getOption_t::AND, waitOption_t{waitOption_t::NO_WAIT});
}

ULONG EventFlags::getFlags() {
    get(0, getOption_t::OR, waitOption_t{waitOption_t::NO_WAIT});
    return actualFlags;
}

bool EventFlags::isSet(const ULONG requestedFlags) {
    return isSet(requestedFlags, getOption_t::AND);
}

bool EventFlags::isSet(const ULONG requestedFlags, const getOption_t getOption) {
    return get(requestedFlags, getOption, waitOption_t{waitOption_t::NO_WAIT}) == TX_SUCCESS;
}

UINT EventFlags::await(const ULONG requestedFlags) {
    return await(requestedFlags, getOption_t::AND, waitOption_t{waitOption_t::WAIT_FOREVER});
}

UINT EventFlags::awaitClear(ULONG requestedFlags) {
    return await(requestedFlags, getOption_t::AND_CLEAR, waitOption_t{waitOption_t::WAIT_FOREVER});
}

UINT EventFlags::await(const ULONG requestedFlags, const waitOption_t waitOption) {
    return await(requestedFlags, getOption_t::AND, waitOption);
}

UINT EventFlags::await(const ULONG requestedFlags, const getOption_t getOption) {
    return await(requestedFlags, getOption, waitOption_t{waitOption_t::WAIT_FOREVER});
}

UINT EventFlags::await(const ULONG requestedFlags, const getOption_t getOption, const waitOption_t waitOption) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::EventFlags[%s]::await(0x%08x)\r\n", getName(), requestedFlags);

    return get(requestedFlags, getOption, waitOption);
}

UINT EventFlags::set(const ULONG flagsToSet, setOption_t setOption) {
    return BaseEventFlags::set(flagsToSet, static_cast<UINT>(setOption));
}
