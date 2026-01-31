/*
 * SPDX-FileCopyrightText: 2026 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "BaseEventFlags.hpp"

using namespace Stm32ThreadX;
using Severity = Stm32ItmLogger::LoggerInterface::Severity;

UINT BaseEventFlags::create(CHAR *name_ptr) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::create(\"%s\")\r\n", COMPONENT_NAME, CLASS_NAME, getName(), name_ptr);

    if (isCreated()) return TX_SUCCESS;

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_create
    const auto ret = tx_event_flags_create(this, name_ptr);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_event_flags_create() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseEventFlags::del() {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::del(\"%s\")\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_delete
    const auto ret = tx_event_flags_delete(this);

    std::memset(static_cast<TX_EVENT_FLAGS_GROUP *>(this), 0, sizeof(TX_EVENT_FLAGS_GROUP));

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_event_flags_delete() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseEventFlags::get(ULONG requested_flags, UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option) {
    // log(Severity::DEBUGGING)
    // ->printf("%s::%s[%s]::get(0x%08x, 0x%02x, %p, %d)\r\n", COMPONENT_NAME, CLASS_NAME, getName(), requested_flags, get_option, actual_flags_ptr, wait_option);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_get
    const auto ret = tx_event_flags_get(this, requested_flags, get_option, actual_flags_ptr, wait_option);

    if (ret != TX_SUCCESS && ret != TX_NO_EVENTS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_event_flags_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseEventFlags::info_get(CHAR **name, ULONG *current_flags, TX_THREAD **first_suspended, ULONG *suspended_count,
                              TX_EVENT_FLAGS_GROUP **next_group) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::info_get(%p, %p, %p, %p, %p)\r\n",
                                     COMPONENT_NAME, CLASS_NAME, getName(), name,
                                     current_flags, first_suspended, suspended_count, next_group);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_info_get
    const auto ret = tx_event_flags_info_get(this, name, current_flags, first_suspended, suspended_count, next_group);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_event_flags_info_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseEventFlags::set(ULONG flags_to_set, UINT set_option) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::set(0x%08x, 0x%02x)\r\n", COMPONENT_NAME, CLASS_NAME, getName(),
                                     flags_to_set, set_option);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_set
    const auto ret = tx_event_flags_set(this, flags_to_set, set_option);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_event_flags_set() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseEventFlags::set_notify(events_set_notify_cb events_set_notify) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::set(%p)\r\n", COMPONENT_NAME, CLASS_NAME, getName(),
                                     events_set_notify);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_set_notify
    const auto ret = tx_event_flags_set_notify(this, events_set_notify);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_event_flags_set_notify() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

#if defined(TX_EVENT_FLAGS_ENABLE_PERFORMANCE_INFO)
UINT BaseEventFlags::performance_info_get(ULONG *sets, ULONG *gets, ULONG *suspensions, ULONG *timeouts) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::performance_info_get(%p, %p, %p, %p)\r\n",
                                     COMPONENT_NAME, CLASS_NAME, getName(), sets, gets, suspensions, timeouts);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_performance_info_get
    const auto ret = tx_event_flags_performance_info_get(this, sets, gets, suspensions, timeouts);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_event_flags_performance_info_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseEventFlags::performance_system_info_get(ULONG *sets, ULONG *gets, ULONG *suspensions, ULONG *timeouts) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::performance_system_info_get(%p, %p, %p, %p)\r\n",
                                     COMPONENT_NAME, CLASS_NAME, getName(), sets, gets, suspensions, timeouts);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_performance_system_info_get
    const auto ret = tx_event_flags_performance_system_info_get(sets, gets, suspensions, timeouts);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_event_flags_performance_system_info_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}
#endif

bool BaseEventFlags::isCreated() {
    return tx_event_flags_group_id != 0;
}
