/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "BytePool.hpp"

using namespace Stm32ThreadX;

UINT BytePool::create(VOID *pool_start, ULONG pool_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::BytePool[%s]::create()\r\n", getName());

    // Create the IP instance
    const auto ret = tx_byte_pool_create(bytePool, const_cast<CHAR *>(getName()), pool_start, pool_size);
    if (ret != TX_SUCCESS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Byte pool creation failed. tx_byte_pool_create() = 0x%02x\r\n", ret);
    }

    return ret;
}

void BytePool::setBytePoolStruct(TX_BYTE_POOL *txBytePool) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::BytePool[%s]::setBytePoolStruct()\r\n", txBytePool->tx_byte_pool_name);

    bytePool = txBytePool;
    setName(bytePool->tx_byte_pool_name);
}


UCHAR *BytePool::allocate(const ULONG memory_size) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::BytePool[%s]::allocate(%d)\r\n", getName(), memory_size);

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

UINT BytePool::release(void *memory_ptr) {
    log(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->printf("Stm32ThreadX::BytePool[%s]::release()\r\n", getName());

    const auto ret = tx_byte_release(memory_ptr);
    if (ret != TX_SUCCESS) {
        log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                ->printf("Byte release failed. tx_byte_release() = 0x%02x\r\n", ret);
    }
    return ret;
}
