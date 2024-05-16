/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "BytePool.hpp"

using namespace Stm32ThreadX;

UINT BytePool::create(VOID *pool_start, ULONG pool_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32ThreadX::BytePool::create()");

    UINT ret = TX_SUCCESS;
    CHAR bytePool_name[] = "Stm32ThreadX::BytePool";

    // Create the IP instance
    ret = tx_byte_pool_create(bytePool, bytePool_name, pool_start, pool_size);
    if (ret != TX_SUCCESS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Byte pool creation failed. tx_byte_pool_create() = 0x%02x\r\n", ret);
    }

    return ret;
}

void BytePool::setBytePoolStruct(TX_BYTE_POOL *txBytePool) {
    bytePool = txBytePool;
}


UCHAR *BytePool::allocate(const ULONG memory_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::BytePool::allocate(%d)\r\n", memory_size);

    UINT ret = TX_SUCCESS;
    UCHAR *memPtr = nullptr;

    ret = tx_byte_allocate(bytePool,
                           reinterpret_cast<void **>(&memPtr),
                           memory_size,
                           TX_NO_WAIT);
    if (ret != TX_SUCCESS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Byte allocation failed. tx_byte_allocate() = 0x%02x\r\n", ret);
        return nullptr;
    }
    return memPtr;
}
