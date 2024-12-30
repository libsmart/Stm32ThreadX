/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "BaseEventFlags.hpp"
#if __EXCEPTIONS
#include <stdexcept>
#endif

using namespace Stm32ThreadX;

UINT BaseEventFlags::create(CHAR *name_ptr) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseEventFlags[%s]::create(\"%s\")\r\n", getName(), name_ptr);

    if (isCreated()) return TX_SUCCESS;

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_create
    const auto ret = tx_event_flags_create(this, name_ptr);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseEventFlags[%s]: tx_event_flags_create() = 0x%02x";
        char buffer[snprintf(nullptr, 0, fmt, getName(), ret) + 1]{};
        snprintf(buffer, sizeof(buffer), fmt, getName(), ret);
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer);
#if __EXCEPTIONS
        throw std::runtime_error(buffer);
#endif
        return ret;
    }
    return ret;
}

UINT BaseEventFlags::del() {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseEventFlags[%s]::del(\"%s\")\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_delete
    const auto ret = tx_event_flags_delete(this);

    std::memset(static_cast<TX_EVENT_FLAGS_GROUP *>(this), 0, sizeof(TX_EVENT_FLAGS_GROUP));

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseEventFlags[%s]: tx_event_flags_delete() = 0x%02x";
        char buffer[snprintf(nullptr, 0, fmt, getName(), ret) + 1]{};
        snprintf(buffer, sizeof(buffer), fmt, getName(), ret);
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer);
#if __EXCEPTIONS
        throw std::runtime_error(buffer);
#endif
        return ret;
    }
    return ret;
}

UINT BaseEventFlags::get(ULONG requested_flags, UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option) {
    // log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
    // ->printf("Stm32ThreadX::BaseEventFlags[%s]::get(0x%08x, 0x%02x, %p, %d)\r\n", getName(), requested_flags, get_option, actual_flags_ptr, wait_option);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_get
    const auto ret = tx_event_flags_get(this, requested_flags, get_option, actual_flags_ptr, wait_option);

    if (ret != NX_SUCCESS && ret != TX_NO_EVENTS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseEventFlags[%s]: tx_event_flags_get() = 0x%02x";
        char buffer[snprintf(nullptr, 0, fmt, getName(), ret) + 1]{};
        snprintf(buffer, sizeof(buffer), fmt, getName(), ret);
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer);
#if __EXCEPTIONS
        throw std::runtime_error(buffer);
#endif
        return ret;
    }
    return ret;
}

UINT BaseEventFlags::info_get(CHAR **name, ULONG *current_flags, TX_THREAD **first_suspended, ULONG *suspended_count,
                              TX_EVENT_FLAGS_GROUP **next_group) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseEventFlags[%s]::info_get(%p, %p, %p, %p, %p)\r\n", getName(), name,
                     current_flags, first_suspended, suspended_count, next_group);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_info_get
    const auto ret = tx_event_flags_info_get(this, name, current_flags, first_suspended, suspended_count, next_group);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseEventFlags[%s]: tx_event_flags_info_get() = 0x%02x";
        char buffer[snprintf(nullptr, 0, fmt, getName(), ret) + 1]{};
        snprintf(buffer, sizeof(buffer), fmt, getName(), ret);
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer);
#if __EXCEPTIONS
        throw std::runtime_error(buffer);
#endif
        return ret;
    }
    return ret;
}

UINT BaseEventFlags::set(ULONG flags_to_set, UINT set_option) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseEventFlags[%s]::set(0x%08x, 0x%02x)\r\n", getName(), flags_to_set, set_option);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_set
    const auto ret = tx_event_flags_set(this, flags_to_set, set_option);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseEventFlags[%s]: tx_event_flags_set() = 0x%02x";
        char buffer[snprintf(nullptr, 0, fmt, getName(), ret) + 1]{};
        snprintf(buffer, sizeof(buffer), fmt, getName(), ret);
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer);
#if __EXCEPTIONS
        throw std::runtime_error(buffer);
#endif
        return ret;
    }
    return ret;
}

UINT BaseEventFlags::set_notify(events_set_notify_cb events_set_notify) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseEventFlags[%s]::set(%p)\r\n", getName(), events_set_notify);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_set_notify
    const auto ret = tx_event_flags_set_notify(this, events_set_notify);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseEventFlags[%s]: tx_event_flags_set_notify() = 0x%02x";
        char buffer[snprintf(nullptr, 0, fmt, getName(), ret) + 1]{};
        snprintf(buffer, sizeof(buffer), fmt, getName(), ret);
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer);
#if __EXCEPTIONS
        throw std::runtime_error(buffer);
#endif
        return ret;
    }
    return ret;
}

#if defined(TX_EVENT_FLAGS_ENABLE_PERFORMANCE_INFO)
UINT BaseEventFlags::performance_info_get(ULONG *sets, ULONG *gets, ULONG *suspensions, ULONG *timeouts) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseEventFlags[%s]::performance_info_get(%p, %p, %p, %p)\r\n", getName(), sets,
                     gets, suspensions, timeouts);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_performance_info_get
    const auto ret = tx_event_flags_performance_info_get(this, sets, gets, suspensions, timeouts);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseEventFlags[%s]: tx_event_flags_performance_info_get() = 0x%02x";
        char buffer[snprintf(nullptr, 0, fmt, getName(), ret) + 1]{};
        snprintf(buffer, sizeof(buffer), fmt, getName(), ret);
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer);
#if __EXCEPTIONS
        throw std::runtime_error(buffer);
#endif
        return ret;
    }
    return ret;
}

UINT BaseEventFlags::performance_system_info_get(ULONG *sets, ULONG *gets, ULONG *suspensions, ULONG *timeouts) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
        ->printf("Stm32ThreadX::BaseEventFlags[%s]::performance_system_info_get(%p, %p, %p, %p)\r\n", getName(), sets,
                 gets, suspensions, timeouts);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_performance_system_info_get
    const auto ret = tx_event_flags_performance_system_info_get(sets, gets, suspensions, timeouts);

    if (ret != NX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseEventFlags[%s]: tx_event_flags_performance_system_info_get() = 0x%02x";
        char buffer[snprintf(nullptr, 0, fmt, getName(), ret) + 1]{};
        snprintf(buffer, sizeof(buffer), fmt, getName(), ret);
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer);
#if __EXCEPTIONS
        throw std::runtime_error(buffer);
#endif
        return ret;
    }
    return ret;
}
#endif

bool BaseEventFlags::isCreated() {
    return tx_event_flags_group_id != 0;
}
