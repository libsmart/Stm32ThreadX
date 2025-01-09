/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "BaseSemaphore.hpp"

#if __EXCEPTIONS
#include <stdexcept>
#define LIBSMART_HANDLE_ERROR(fmt, ...)                                          \
do {                                                                    \
char buffer[snprintf(nullptr, 0, fmt, __VA_ARGS__) + 1]{};              \
snprintf(buffer, sizeof(buffer), fmt, __VA_ARGS__);                     \
log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer); \
throw std::runtime_error(buffer);                                       \
} while (0);
#else
#define LIBSMART_HANDLE_ERROR(fmt, ...)                                          \
do {                                                                    \
char buffer[snprintf(nullptr, 0, fmt, __VA_ARGS__) + 1]{};              \
snprintf(buffer, sizeof(buffer), fmt, __VA_ARGS__);                     \
log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer); \
return ret;                                                             \
} while (0);
#endif

using namespace Stm32ThreadX;

UINT BaseSemaphore::create(CHAR *name_ptr, ULONG initial_count) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::create(\"%s\", %d)\r\n",
                     getName(), name_ptr, initial_count);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_create
    const auto ret = tx_semaphore_create(
        this,
        name_ptr,
        initial_count
    );

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_create() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseSemaphore::del() {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::del()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_delete
    const auto ret = tx_semaphore_delete(this);

    std::memset(static_cast<TX_SEMAPHORE *>(this), 0, sizeof(TX_SEMAPHORE));

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_delete() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseSemaphore::ceiling_put(ULONG ceiling) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::ceiling_put(%d)\r\n", getName(), ceiling);

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_ceiling_put
    const auto ret = tx_semaphore_ceiling_put(this, ceiling);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_ceiling_put() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseSemaphore::get(ULONG wait_option) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::get()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_get
    const auto ret = tx_semaphore_get(this, wait_option);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseSemaphore::info_get(CHAR **name, ULONG *current_value, TX_THREAD **first_suspended, ULONG *suspended_count,
                             TX_SEMAPHORE **next_semaphore) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::info_get()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_info_get
    const auto ret = tx_semaphore_info_get(this, name, current_value,
                                           first_suspended, suspended_count, next_semaphore);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_info_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

#if defined(TX_SEMAPHORE_ENABLE_PERFORMANCE_INFO)
UINT BaseSemaphore::performance_info_get(ULONG *puts, ULONG *gets, ULONG *suspensions, ULONG *timeouts) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::performance_info_get()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_performance_info_get
    const auto ret = tx_semaphore_performance_info_get(this, puts, gets, suspensions, timeouts);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_performance_info_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}


UINT BaseSemaphore::performance_system_info_get(ULONG *puts, ULONG *gets, ULONG *suspensions, ULONG *timeouts) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::performance_system_info_get()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_performance_system_info_get
    const auto ret = tx_semaphore_performance_system_info_get(puts, gets, suspensions, timeouts);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_performance_system_info_get() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}
#endif

UINT BaseSemaphore::prioritize() {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::prioritize()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_prioritize
    const auto ret = tx_semaphore_prioritize(this);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_prioritize() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseSemaphore::put() {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::put()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_put
    const auto ret = tx_semaphore_put(this);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_put() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}

UINT BaseSemaphore::put_notify(semaphore_put_notify_callback semaphore_put_notify) {
    log(Stm32ItmLogger::LoggerInterface::Severity::DEBUGGING)
            ->printf("Stm32ThreadX::BaseSemaphore[%s]::put_notify()\r\n", getName());

    // @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_semaphore_put_notify
    const auto ret = tx_semaphore_put_notify(this, semaphore_put_notify);

    if (ret != TX_SUCCESS) {
        constexpr char fmt[] = "Stm32ThreadX::BaseSemaphore[%s]: tx_semaphore_put_notify() = 0x%02x";
        LIBSMART_HANDLE_ERROR(fmt, getName(), ret);
    }
    return ret;
}
