/*
 * SPDX-FileCopyrightText: 2026 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "BaseQueue.hpp"
#include <ctime>

using namespace Stm32ThreadX;
using Severity = Stm32ItmLogger::LoggerInterface::Severity;

UINT BaseQueue::create(CHAR *name_ptr, UINT message_size, void *queue_start, ULONG queue_size) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::create(\"%s\", %d, %p, %lu)\r\n", COMPONENT_NAME, CLASS_NAME,
                                     getName(), name_ptr, message_size, queue_start, queue_size);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_create
    const auto ret = tx_queue_create(
        this,
        name_ptr,
        message_size,
        queue_start,
        queue_size
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_queue_create() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseQueue::del() {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::del()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_delete
    const auto ret = tx_queue_delete(this);

    std::memset(static_cast<TX_QUEUE *>(this), 0, sizeof(TX_QUEUE));

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_queue_delete() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseQueue::flush() {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::flush()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_flush
    const auto ret = tx_queue_flush(this);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_queue_flush() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseQueue::front_send(void *source_ptr, ULONG wait_option) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::front_send(%p, %lu)\r\n", COMPONENT_NAME, CLASS_NAME,
                                     getName(), source_ptr, wait_option);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_front_send
    const auto ret = tx_queue_front_send(
        this,
        source_ptr,
        wait_option
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_queue_front_send() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseQueue::info_get(CHAR **name, ULONG *enqueued, ULONG *available_storage, TX_THREAD **first_suspended,
                         ULONG *suspended_count, TX_QUEUE **next_queue) {
    // log(Severity::DEBUGGING)->printf("%s::%s[%s]::info_get()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_info_get
    const auto ret = tx_queue_info_get(
        this,
        name,
        enqueued,
        available_storage,
        first_suspended,
        suspended_count,
        next_queue
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_queue_info_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}


#if defined(TX_QUEUE_ENABLE_PERFORMANCE_INFO)
UINT BaseQueue::performance_info_get(ULONG *messages_sent, ULONG *messages_received, ULONG *empty_suspensions,
                                     ULONG *full_suspensions, ULONG *full_errors, ULONG *timeouts) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseQueue[%s]::performance_info_get()\r\n",
                     getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_performance_info_get
    const auto ret = tx_queue_performance_info_get(
        this,
        messages_sent,
        messages_received,
        empty_suspensions,
        full_suspensions,
        full_errors,
        timeouts
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseQueue[%s]: tx_queue_performance_info_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}


UINT BaseQueue::performance_system_info_get(ULONG *messages_sent, ULONG *messages_received, ULONG *empty_suspensions,
                                            ULONG *full_suspensions, ULONG *full_errors, ULONG *timeouts) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseQueue[%s]::performance_system_info_get()\r\n",
                     getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_performance_system_info_get
    const auto ret = tx_queue_performance_system_info_get(
        messages_sent,
        messages_received,
        empty_suspensions,
        full_suspensions,
        full_errors,
        timeouts
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseQueue[%s]: tx_queue_performance_system_info_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}
#endif


UINT BaseQueue::prioritize() {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::prioritize()\r\n", COMPONENT_NAME, CLASS_NAME, getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_prioritize
    const auto ret = tx_queue_prioritize(
        this
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_queue_prioritize() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseQueue::receive(void *destination_ptr, ULONG wait_option) {
    // log(Severity::DEBUGGING)->printf("%s::%s[%s]::receive(%p, %lu)\r\n", COMPONENT_NAME, CLASS_NAME, getName(),
    // destination_ptr, wait_option);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_receive
    const auto ret = tx_queue_receive(
        this,
        destination_ptr,
        wait_option
    );

    if (ret != TX_SUCCESS && ret != TX_DELETED && ret != TX_QUEUE_EMPTY && ret != TX_WAIT_ABORTED) {
        constexpr char fmt[] = "%s::%s[%s]: tx_queue_receive() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseQueue::send(void *source_ptr, ULONG wait_option) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::send(%p, %lu)\r\n", COMPONENT_NAME, CLASS_NAME, getName(),
                                     source_ptr, wait_option);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_send
    const auto ret = tx_queue_send(
        this,
        source_ptr,
        wait_option
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_queue_send() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}

UINT BaseQueue::send_notify(send_notify_callback queue_send_notify) {
    log(Severity::DEBUGGING)->printf("%s::%s[%s]::send(%p)\r\n", COMPONENT_NAME, CLASS_NAME, getName(),
                                     queue_send_notify);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_queue_send_notify
    const auto ret = tx_queue_send_notify(
        this,
        queue_send_notify
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "%s::%s[%s]: tx_queue_send_notify() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, COMPONENT_NAME, CLASS_NAME, getName(), ret);
    }
    return ret;
}
