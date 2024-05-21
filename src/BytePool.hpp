/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32THREADX_BYTEPOOL_HPP
#define LIBSMART_STM32THREADX_BYTEPOOL_HPP

#include <libsmart_config.hpp>
#include "Loggable.hpp"
#include "Nameable.hpp"
#include "tx_api.h"

namespace Stm32ThreadX {
    class BytePool : public Stm32ItmLogger::Loggable, public Stm32Common::Nameable {
    public:
        BytePool() = default;

        explicit BytePool(TX_BYTE_POOL *txBytePool)
            : Nameable(txBytePool->tx_byte_pool_name), bytePool(txBytePool) {
        }

        explicit BytePool(const char *name)
            : Nameable(name) {
        }

        UINT create(VOID *pool_start, ULONG pool_size);

        void setBytePoolStruct(TX_BYTE_POOL *txBytePool);

        /**
         * @brief Allocate memory from the BytePool.
         *
         * This method allocates memory of the specified size from the BytePool.
         *
         * @param memory_size The size of the memory to allocate.
         *
         * @return A pointer to the allocated memory. Returns nullptr if allocation fails.
         */
        UCHAR *allocate(ULONG memory_size);

        UINT release(VOID *memory_ptr);

    protected:
        TX_BYTE_POOL *bytePool = {};
    };
}
#endif //LIBSMART_STM32THREADX_BYTEPOOL_HPP
