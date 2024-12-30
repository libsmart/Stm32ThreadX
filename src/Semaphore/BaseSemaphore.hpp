/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Loggable.hpp"
#include "Nameable.hpp"
#include "tx_api.h"

namespace Stm32ThreadX {
    class BaseSemaphore : protected TX_SEMAPHORE, public Stm32ItmLogger::Loggable, public Stm32Common::Nameable {
    public:
        BaseSemaphore() : BaseSemaphore(&Stm32ItmLogger::emptyLogger) { ; }

        explicit BaseSemaphore(const char *name)
            : BaseSemaphore(name, &Stm32ItmLogger::emptyLogger) { ; }

        explicit BaseSemaphore(Stm32ItmLogger::LoggerInterface *logger)
            : BaseSemaphore("Stm32ThreadX::Semaphore", logger) { ; }

        BaseSemaphore(const char *name, Stm32ItmLogger::LoggerInterface *logger)
            : TX_SEMAPHORE(), Loggable(logger), Nameable(name) { ; }


        /**
         * @brief Creates a semaphore with the specified name and initial count.
         *
         * This method initializes a semaphore with the given name and initial count,
         * leveraging the ThreadX semaphore creation API. It logs the creation event
         * and error messages if the creation fails.
         *
         * @param name_ptr A pointer to a character string that represents the name of the semaphore.
         * @param initial_count The initial count of the semaphore, representing the number of resources available.
         * @return A UINT value that indicates the success or error code of the operation.
         *         Returns NX_SUCCESS if the semaphore is successfully created, otherwise an error code.
         */
        virtual UINT create(CHAR *name_ptr, ULONG initial_count);

        /**
         * @brief Deletes the semaphore and clears its associated memory.
         *
         * This method deletes an existing semaphore using the ThreadX semaphore deletion API.
         * It logs the operation, as well as any errors encountered if the deletion fails.
         * The method also clears the memory associated with the semaphore object after deletion.
         *
         * @return A UINT value that indicates the success or error code of the operation.
         *         Returns NX_SUCCESS if the semaphore is successfully deleted, otherwise an error code.
         */
        virtual UINT del();

        /**
         * @brief Place an instance in counting semaphore with ceiling.
         *
         * This service puts an instance into the specified counting semaphore, which in reality increments the
         * counting semaphore by one. If the counting semaphore's current value is greater than or equal to the
         * specified ceiling, the instance will not be put and a TX_CEILING_EXCEEDED error will be returned.
         * @param ceiling Maximum limit allowed for the semaphore.
         * @return A UINT value indicating the success or error code of the operation.
         *         Returns NX_SUCCESS if the operation is successful, otherwise an error code.
         */
        virtual UINT ceiling_put(ULONG ceiling);

        /**
         * @brief Acquires the semaphore, blocking for a specified time if necessary.
         *
         * This method attempts to acquire the semaphore using the ThreadX semaphore get
         * API. It logs the operation for debugging purposes and records any errors in case
         * of failure. If exceptions are enabled and the operation fails, an exception
         * is thrown.
         *
         * @param wait_option Specifies the maximum time to wait for the semaphore.
         *                    Can be TX_WAIT_FOREVER, TX_NO_WAIT, or a specific timeout value.
         * @return A UINT value indicating the success or error code of the operation.
         *         Returns NX_SUCCESS if the semaphore is successfully acquired, otherwise an error code.
         */
        virtual UINT get(ULONG wait_option);

        /**
         * @brief Retrieves information about a semaphore.
         *
         * This method gathers details about a semaphore's state, including its name, current value,
         * the first thread suspended on it (if any), the number of suspended threads, and the next semaphore
         * in the system semaphore list. It uses the ThreadX API to fetch the required data.
         *
         * @param name A pointer to a CHAR pointer to store the name of the semaphore.
         * @param current_value A pointer to an ULONG to store the semaphore's current count.
         * @param first_suspended A pointer to a TX_THREAD pointer to store the first thread suspended on the semaphore, if any.
         * @param suspended_count A pointer to an ULONG to store the number of threads currently suspended on the semaphore.
         * @param next_semaphore A pointer to a TX_SEMAPHORE pointer to store the next semaphore in the system semaphore list.
         * @return A UINT value that represents the operation's success or error code. Returns NX_SUCCESS if successful,
         *         or an error code if the operation fails.
         */
        virtual UINT info_get(CHAR **name,
                              ULONG *current_value,
                              TX_THREAD **first_suspended,
                              ULONG *suspended_count,
                              TX_SEMAPHORE **next_semaphore);

#if defined(TX_SEMAPHORE_ENABLE_PERFORMANCE_INFO)
        /**
         * @brief Retrieves performance statistics for the semaphore.
         *
         * This method fetches performance information related to the semaphore, including the number
         * of put operations, get operations, suspensions, and timeouts associated with the semaphore.
         * It uses the ThreadX API to gather the requested data and logs the results or errors if
         * the operation fails.
         *
         * @param puts A pointer to a ULONG variable where the number of put operations will be stored.
         * @param gets A pointer to a ULONG variable where the number of get operations will be stored.
         * @param suspensions A pointer to a ULONG variable where the number of suspensions will be stored.
         * @param timeouts A pointer to a ULONG variable where the number of timeouts will be stored.
         * @return A UINT value that indicates the success or error code of the operation.
         *         Returns NX_SUCCESS if successful; otherwise, returns an appropriate error code.
         */
        virtual UINT performance_info_get(ULONG *puts, ULONG *gets, ULONG *suspensions, ULONG *timeouts);

        /**
         * @brief Retrieves performance system information for the semaphore.
         *
         * This method gathers performance metrics related to the semaphore, including the total number
         * of successful "put" and "get" operations, as well as any suspension and timeout occurrences.
         * It utilizes the ThreadX API to perform the operation and logs the results or errors if
         * the operation fails.
         *
         * @param puts A pointer to a variable where the number of successful "put" operations will be stored.
         * @param gets A pointer to a variable where the number of successful "get" operations will be stored.
         * @param suspensions A pointer to a variable where the number of suspension occurrences will be stored.
         * @param timeouts A pointer to a variable where the number of timeout occurrences will be stored.
         * @return A UINT value representing the success or error code of the operation.
         *         Returns NX_SUCCESS if the performance information is successfully retrieved, otherwise an error code.
         */
        virtual UINT performance_system_info_get(ULONG *puts, ULONG *gets, ULONG *suspensions, ULONG *timeouts);
#endif

        /**
         * @brief Prioritize semaphore suspension list.
         *
         * This service places the highest priority thread suspended for an instance of the semaphore at the front of
         * the suspension list. All other threads remain in the same FIFO order they were suspended in.
         *
         * @return A UINT value indicating the success or error code of the operation.
         *         Returns NX_SUCCESS if the prioritization is successful, or an applicable error code otherwise.
         */
        virtual UINT prioritize();

        /**
         * @brief Releases a semaphore resource and increments its count.
         *
         * This method performs an operation to release a semaphore previously acquired,
         * increasing the semaphore's count. It logs the operation's success or failure
         * and throws an exception when enabled with exception handling, in case of an error.
         *
         * @return A UINT value indicating the result of the semaphore release operation.
         *         Returns NX_SUCCESS if the operation is successful, otherwise an error code.
         */
        virtual UINT put();

        using semaphore_put_notify_callback = VOID (*)(TX_SEMAPHORE *);

        /**
         * @brief Registers a notification callback for semaphore put operations.
         *
         * This method assigns a callback function that is invoked whenever the semaphore is put.
         * It utilizes the ThreadX API to register the specified callback and logs the process
         * for debugging and error handling purposes.
         *
         * @param semaphore_put_notify A callback function to be executed when the semaphore is put.
         * @return A UINT value indicating the result of the operation.
         *         Returns NX_SUCCESS if the callback is successfully registered, otherwise an error code.
         */
        virtual UINT put_notify(semaphore_put_notify_callback semaphore_put_notify);
    };
}
