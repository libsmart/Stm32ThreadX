/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <libsmart_config.hpp>
#include "Loggable.hpp"
#include "Nameable.hpp"
#include "tx_api.h"


namespace Stm32ThreadX {
    class BaseEventFlags : protected TX_EVENT_FLAGS_GROUP,
                           public Stm32ItmLogger::Loggable,
                           public Stm32Common::Nameable {
    public:
        BaseEventFlags() : BaseEventFlags(&Stm32ItmLogger::emptyLogger) { ; }

        explicit BaseEventFlags(const char *name)
            : BaseEventFlags(name, &Stm32ItmLogger::emptyLogger) { ; }

        explicit BaseEventFlags(Stm32ItmLogger::LoggerInterface *logger)
            : BaseEventFlags("Stm32ThreadX::EventFlags", logger) { ; }

        BaseEventFlags(const char *name, Stm32ItmLogger::LoggerInterface *logger)
            : TX_EVENT_FLAGS_GROUP(), Loggable(logger), Nameable(name) { ; }


        /**
         * @brief Creates an event flags group with the specified name.
         *
         * This method initializes the event flags and associates it with the
         * given name if it is not already created. If the event flags group
         * creation fails, a log message is generated and the appropriate error code
         * is returned.
         *
         * @param name_ptr A pointer to a null-terminated string that specifies the name
         *                 of the event flags group.
         * @return TX_SUCCESS (0x00) if the event flags group is successfully created;
         *         otherwise, an error code indicating the failure reason.
         */
        virtual UINT create(CHAR *name_ptr);

        /**
         * @brief Deletes the event flags group associated with this object.
         *
         * This method performs cleanup for the event flags group referenced
         * by the object. It deletes the group using `tx_event_flags_delete` and
         * zeroes out its memory. If the deletion fails, an error message is logged.
         * Depending on runtime configuration, an exception may be thrown in case
         * of failure.
         *
         * @return TX_SUCCESS (0x00) if the event flags group is successfully deleted;
         *         otherwise, an error code indicating the failure reason.
         */
        virtual UINT del();

        /**
         * @brief Retrieves event flags from the specified event flags group.
         *
         * This method attempts to retrieve the requested event flags from the event flags group.
         * The behavior of this operation depends on the specified options, such as whether the
         * call blocks until the flags are set and whether all requested flags must be satisfied.
         * If the operation fails, an error message is logged. Depending on runtime configuration,
         * an exception may be thrown in case of failure.
         *
         * @param requested_flags The flags to be retrieved, specified as a bitwise OR of desired flags.
         * @param get_option Options for how the flags are retrieved (e.g., AND/OR logic, clear flags on retrieval).
         * @param actual_flags_ptr Pointer to a variable where the retrieved flags will be stored.
         * @param wait_option Defines how long to wait for the requested flags (in timer ticks).
         *                    A value of TX_WAIT_FOREVER indicates an indefinite wait.
         * @return NX_SUCCESS (0x00) if the flags are successfully retrieved; otherwise, an error code indicating the failure reason.
         */
        virtual UINT get(ULONG requested_flags, UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option);


        /**
         * @brief Retrieves detailed information about the event flags group.
         *
         * This method provides information about the current status of the event flags group,
         * including its name, current flag values, suspended threads, and the next event flags group.
         * If the operation fails, an error log is generated, and the appropriate error code is returned.
         *
         * @param name A pointer to a location where the address of the name of the event flags group
         *             will be written. The name is a null-terminated string.
         * @param current_flags A pointer to a variable where the current flag values for the event
         *                      flags group will be written.
         * @param first_suspended A pointer to a variable where the first thread waiting on the event
         *                        flags group (if any) will be written.
         * @param suspended_count A pointer to a variable where the number of threads suspended on the
         *                        event flags group will be written.
         * @param next_group A pointer to a variable where the address of the next event flags group
         *                   on the created list will be written.
         * @return TX_SUCCESS (0x00) if the information is successfully retrieved; otherwise, an error
         *         code indicating the reason for the failure.
         */
        virtual UINT info_get(CHAR **name, ULONG *current_flags, TX_THREAD **first_suspended,
                              ULONG *suspended_count, TX_EVENT_FLAGS_GROUP **next_group);

        /**
         * @brief Sets the specified event flags in the event flags group.
         *
         * This method sets one or more event flags within the specified event flags group.
         * The flags to be set are specified by the input parameter. If the operation fails,
         * an error code is returned indicating the reason for failure.
         *
         * @param group_ptr A pointer to the event flags group where the specified flags need to be set.
         * @param flags An unsigned value specifying the bits (event flags) to set in the group.
         * @return TX_SUCCESS (0x00) if the event flags are successfully set;
         *         otherwise, an error code indicating the failure reason.
         */
        virtual UINT set(ULONG flags_to_set, UINT set_option);

        using events_set_notify_cb = VOID (*)(TX_EVENT_FLAGS_GROUP *);

        /**
         * @brief Sets a notification callback for the event flags group.
         *
         * This method registers a function to be called whenever the state
         * of the event flags changes. Using this mechanism, the application
         * can be notified of any event flags updates.
         *
         * @param events_set_notify A callback function of type `events_set_notify_cb`
         *                          to be executed when the event flags change.
         * @return NX_SUCCESS (0x00) if the callback is successfully set;
         *         otherwise, an error code indicating the failure reason.
         */
        virtual UINT set_notify(events_set_notify_cb events_set_notify);

#if defined(TX_EVENT_FLAGS_ENABLE_PERFORMANCE_INFO)
        /**
         * @brief Retrieves performance metrics for the event flags group.
         *
         * This method obtains performance information related to the event flags
         * operations, such as the number of sets, gets, suspensions, and timeouts.
         *
         * @param sets A pointer to a variable that will receive the count of successful flag sets.
         * @param gets A pointer to a variable that will receive the count of successful flag retrievals.
         * @param suspensions A pointer to a variable that will receive the count of thread suspensions.
         * @param timeouts A pointer to a variable that will receive the count of timeouts.
         * @return NX_SUCCESS (0x00) if performance information is successfully retrieved;
         *         otherwise, an error code indicating the failure reason.
         */
        virtual UINT performance_info_get(ULONG *sets, ULONG *gets, ULONG *suspensions, ULONG *timeouts);

        /**
         * @brief Retrieves performance statistics for the event flags system.
         *
         * This method queries the system for performance metrics related to event flags,
         * such as the total number of sets, gets, suspensions, and timeouts that have occurred.
         * It logs the operation and returns the result of the underlying system call.
         *
         * @param sets A pointer to a ULONG variable where the total number of sets will be stored.
         * @param gets A pointer to a ULONG variable where the total number of gets will be stored.
         * @param suspensions A pointer to a ULONG variable where the total number of suspensions will be stored.
         * @param timeouts A pointer to a ULONG variable where the total number of timeouts will be stored.
         * @return NX_SUCCESS (0x00) on success, or an error code indicating the failure reason.
         */
        virtual UINT performance_system_info_get(ULONG *sets, ULONG *gets, ULONG *suspensions, ULONG *timeouts);
#endif

        virtual bool isCreated();
    };
}
