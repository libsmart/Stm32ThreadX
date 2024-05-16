/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32THREADX_BYTEPOOL_HPP
#define LIBSMART_STM32THREADX_BYTEPOOL_HPP

#include <libsmart_config.hpp>
#include "Loggable.hpp"
#include "tx_api.h"

namespace Stm32ThreadX {
    class BytePool : public Stm32ItmLogger::Loggable, public TX_BYTE_POOL {
    public:
        BytePool() = default;

        explicit BytePool(const TX_BYTE_POOL &other)
            : TX_BYTE_POOL(other) {
        }

        UINT create(VOID *pool_start, ULONG pool_size);

        UCHAR *allocate(ULONG memory_size);

    };
}
#endif //LIBSMART_STM32THREADX_BYTEPOOL_HPP
