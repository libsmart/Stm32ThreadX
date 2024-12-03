/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Loggable.hpp"
#include "Nameable.hpp"
#include "tx_api.h"

namespace Stm32ThreadX {
    class BaseQueue : protected TX_QUEUE, public Stm32ItmLogger::Loggable, public Stm32Common::Nameable {
    public:
        BaseQueue() : BaseQueue(&Stm32ItmLogger::emptyLogger) { ; }

        explicit BaseQueue(const char *name)
            : BaseQueue(name, &Stm32ItmLogger::emptyLogger) { ; }

        explicit BaseQueue(Stm32ItmLogger::LoggerInterface *logger)
            : BaseQueue("Stm32ThreadX::Queue", logger) { ; }

        BaseQueue(const char *name, Stm32ItmLogger::LoggerInterface *logger)
            : TX_QUEUE(), Loggable(logger), Nameable(name) { ; }

        /**
         * @brief Creates a new queue with the specified parameters.
         *
         * This method initializes a queue with a given name, message size, starting address, and total size.
         * It logs the creation process at the debugging level and any errors at the error level. If the queue creation fails,
         * an exception is thrown, provided exceptions are enabled.
         *
         * @param name_ptr A pointer to a CHAR array representing the name of the queue.
         * @param message_size The size of each message that the queue will hold, specified as a UINT.
         * @param queue_start A pointer to the starting address of the memory the queue will occupy.
         * @param queue_size The total size of the queue memory in ULONG units.
         *
         * @return Returns a UINT status code. NX_SUCCESS is returned if the queue is successfully created;
         * otherwise, an error code is returned. If exceptions are enabled and the operation fails, a
         * `std::runtime_error` is thrown.
         */
        virtual UINT create(CHAR *name_ptr, UINT message_size, VOID *queue_start, ULONG queue_size);

        /**
         * @brief Deletes the queue and resets its memory.
         *
         * This method utilizes `tx_queue_delete()` to delete the queue associated with the current object
         * and subsequently clears the memory associated with it. It logs the deletion process at the
         * debugging level and any errors at the error level. If the deletion fails, an exception is thrown,
         * provided exceptions are enabled.
         *
         * @return Returns a UINT status code. NX_SUCCESS is returned if the queue is successfully deleted;
         * otherwise, an error code is returned. If exceptions are enabled and the operation fails, a
         * `std::runtime_error` is thrown.
         */
        virtual UINT del();

        /**
         * @brief Clears all messages from the queue.
         *
         * This method uses `tx_queue_flush()` to remove all messages currently held in the queue.
         * It logs actions at both debugging and error levels, providing feedback depending on
         * the success or failure of the flushing operation.
         *
         * @return Returns a UINT status code. NX_SUCCESS is returned if the queue is successfully
         * flushed; otherwise, an error code is returned. If exceptions are enabled and the operation
         * fails, a `std::runtime_error` is thrown.
         */
        virtual UINT flush();

        /**
         * @brief Sends a message to the front of the queue.
         *
         * This method employs the `tx_queue_front_send()` function to place a message at the
         * front of the queue. If the queue is full, the function can suspend the calling thread
         * for a specified amount of time, determined by the `wait_option`.
         *
         * @param source_ptr Pointer to the message to be sent to the queue.
         * @param wait_option Amount of time, in ticks, the calling thread should be suspended
         * if the queue is full. Constants defining wait options, such as TX_NO_WAIT or TX_WAIT_FOREVER,
         * can also be used.
         *
         * @return Returns a UINT status code. NX_SUCCESS is returned if the message is successfully
         * placed at the front of the queue; otherwise, an error code is returned indicating the reason
         * for failure.
         */
        virtual UINT front_send(VOID *source_ptr, ULONG wait_option);

        /**
         * @brief Retrieves detailed information about the current state of the queue.
         *
         * This method utilizes the ThreadX API `tx_queue_info_get()` to obtain
         * various metrics and details concerning the specified queue. The gathered
         * data includes the name of the queue, the number of messages currently
         * enqueued, available storage for additional messages, and information about
         * threads suspended on the queue due to it being full or empty.
         *
         * @param name Pointer to a CHAR pointer where the name of the queue will be stored.
         * @param enqueued Pointer to an ULONG where the number of messages currently enqueued will be stored.
         * @param available_storage Pointer to an ULONG where the available storage space for additional messages will be stored.
         * @param first_suspended Pointer to a TX_THREAD pointer where the first suspended thread on the queue will be stored, if any.
         * @param suspended_count Pointer to an ULONG where the number of threads currently suspended on the queue will be stored.
         * @param next_queue Pointer to a TX_QUEUE pointer where the next queue in the linked list of queues will be stored, if any.
         *
         * @return Returns a UINT status code indicating the result of the operation. NX_SUCCESS is returned if the operation is successful; otherwise, an error code is returned.
         */
        virtual UINT info_get(CHAR **name, ULONG *enqueued, ULONG *available_storage, TX_THREAD **first_suspended,
                              ULONG *suspended_count, TX_QUEUE **next_queue);

        /**
         * @brief Retrieves information on queue operation performance.
         *
         * This function invokes the ThreadX API `tx_queue_performance_info_get()` to collect
         * performance data about queue operations associated with this `BaseQueue` instance.
         * It gathers metrics such as messages sent and received, suspensions due to the queue
         * being empty or full, full errors, and timeouts. The collected data is primarily used
         * for debugging and monitoring purposes. If the operation encounters any errors, the
         * function logs the error details and may throw an exception if exceptions are enabled.
         *
         * @param messages_sent Pointer to an ULONG to store the number of messages sent by the queue.
         * @param messages_received Pointer to an ULONG to store the number of messages received by the queue.
         * @param empty_suspensions Pointer to an ULONG to store the count of suspensions when the queue was empty.
         * @param full_suspensions Pointer to an ULONG to store the count of suspensions when the queue was full.
         * @param full_errors Pointer to an ULONG to store the count of errors due to the queue being full.
         * @param timeouts Pointer to an ULONG to store the count of timeouts that occurred.
         *
         * @return Returns a UINT status code indicating the result of the operation. NX_SUCCESS is returned
         * if the operation is successful; otherwise, an error code is returned.
         */
        /*
        virtual UINT performance_info_get(ULONG *messages_sent, ULONG *messages_received, ULONG *empty_suspensions,
                                          ULONG *full_suspensions, ULONG *full_errors, ULONG *timeouts);
                                          */

        /**
         * @brief Retrieves performance information on system-wide queue operations.
         *
         * This function calls the ThreadX API `tx_queue_performance_system_info_get()` to obtain
         * various performance metrics related to queue operations, such as the number of messages
         * sent and received, the number of suspensions due to queues being empty or full, the
         * number of times a queue was full when a message was attempted to be sent, and the number
         * of timeouts that have occurred. The information is logged, and any errors encountered
         * during the process are also logged, potentially throwing an exception if exceptions
         * are enabled.
         *
         * @param messages_sent Pointer to unsigned long to store the number of messages sent.
         * @param messages_received Pointer to unsigned long to store the number of messages received.
         * @param empty_suspensions Pointer to unsigned long to store the count of empty suspensions.
         * @param full_suspensions Pointer to unsigned long to store the count of full suspensions.
         * @param full_errors Pointer to unsigned long to store the count of full errors.
         * @param timeouts Pointer to unsigned long to store the count of timeouts.
         *
         * @return Returns an unsigned integer (UINT) status code indicating the result
         * of the operation. The possible return values include NX_SUCCESS if the operation
         * is successful, or an error code if the operation fails.
         */
        /*
        virtual UINT performance_system_info_get(ULONG *messages_sent, ULONG *messages_received,
                                                 ULONG *empty_suspensions, ULONG *full_suspensions, ULONG *full_errors,
                                                 ULONG *timeouts);
                                                 */

        /**
         * @brief Prioritize queue suspension list.
         *
         * This service places the highest priority thread suspended for a message (or to place a message) on this
         * queue at the front of the suspension list. All other threads remain in the same FIFO order they were
         * suspended in.
         *
         * @return An unsigned integer (UINT) indicating the result of the prioritization operation.
         * Returns NX_SUCCESS if the operation is successful or an error code if it fails.
         */
        virtual UINT prioritize();

        /**
         * @brief Receives a message from the queue.
         *
         * This function attempts to receive a message from the queue using the ThreadX API `tx_queue_receive()`.
         * The message is copied into the memory location pointed to by `destination_ptr`. The function also handles
         * logging of the operation and errors, and can throw an exception if the operation fails and exceptions are enabled.
         *
         * @param destination_ptr Pointer to the memory location where the received message will be stored.
         * @param wait_option Specifies the behavior if the queue is empty. Determines how long the call should wait for a message.
         *
         * @return Returns an unsigned integer status code. The possible return value is NX_SUCCESS if a message is successfully
         * received, or an error code indicating the result of the operation.
         */
        virtual UINT receive(VOID *destination_ptr, ULONG wait_option);

        /**
         * @brief Sends a message to the queue.
         *
         * This function attempts to send a message from the specified source pointer
         * to the queue. It uses the ThreadX API `tx_queue_send()` to perform the operation.
         * The function logs the activity including any errors encountered, and if exceptions
         * are enabled, it may throw an exception in the event of a failure.
         *
         * @param source_ptr Pointer to the message to be sent to the queue.
         * @param wait_option Specifies how the service should behave if the queue is full.
         *                    Options include waiting indefinitely or returning immediately.
         *
         * @return Returns an unsigned integer (UINT) status code indicating the outcome of the operation.
         * The return value can be NX_SUCCESS if the message is successfully sent,
         * or an error code if the operation fails.
         */
        virtual UINT send(VOID *source_ptr, ULONG wait_option);


        using send_notify_callback = VOID (*)(TX_QUEUE *);

        /**
         * @brief Registers a callback function to be invoked on a queue send event.
         *
         * This method allows setting up a notification callback function that is called
         * whenever a message is sent to the queue. It provides a mechanism for users
         * to execute custom processing upon queue operations.
         *
         * @param queue_send_notify A callback function of type `send_notify_callback` that
         * will be executed when a send operation is performed on the queue.
         *
         * @return Returns an unsigned integer (UINT) status code indicating the success
         * or failure of setting the notification. Possible return values include NX_SUCCESS
         * if the operation is successful, or an error code denoting any issues.
         */
        virtual UINT send_notify(send_notify_callback queue_send_notify);
    };
}
