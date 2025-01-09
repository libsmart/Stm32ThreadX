/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32THREADX_EVENTFLAGS_HPP
#define LIBSMART_STM32THREADX_EVENTFLAGS_HPP

#include <limits.h>
#include <libsmart_config.hpp>
#include "Loggable.hpp"
#include "Nameable.hpp"
#include "tx_api.h"
#include "BaseEventFlags.hpp"

namespace Stm32ThreadX {
    /**
     * @class EventFlags
     * @brief A class that represents an event flags group.
     *
     * The EventFlags class provides methods for creating, deleting, retrieving, awaiting, and setting event flags in an event
     * flags group.
     *
     * @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter3.md#event-flags
     */
    class EventFlags : public BaseEventFlags {
    public:
        EventFlags() = default;

        explicit EventFlags(Stm32ItmLogger::LoggerInterface *logger)
            : BaseEventFlags(logger) { ; }

        explicit EventFlags(const char *name)
            : BaseEventFlags(name, &Stm32ItmLogger::emptyLogger) { ; }

        EventFlags(const char *name, Stm32ItmLogger::LoggerInterface *logger)
            : BaseEventFlags(name, logger) { ; }


        UINT create();

        using BaseEventFlags::create;

        UINT deleteFlags();

        /**
         * @union waitOption_t
         * @brief A union that represents the wait options for suspending while waiting for event flags.
         *
         * The waitOption_t union defines two options for suspending while waiting for event flags: timeout and waitOption.
         * - timeout: Specifies the maximum number of timer-ticks to stay suspended while waiting for the event flags.
         * - waitOption: An enumeration of wait options with the following values:
         *   - NO_WAIT (0): Immediate return from the service.
         *   - WAIT_FOREVER (0xFFFFFFFF): Suspend indefinitely until the event flags are available.
         *
         * @see EventFlags.hpp
         */
        union waitOption_t {
            /** Specifies the maximum number of timer-ticks to stay suspended while waiting for the event flags. */
            ULONG timeout;

            enum waitOption : ULONG {
                /** Immediate return from this service */
                NO_WAIT = 0,
                /** Suspend indefinitely until the event flags are available */
                WAIT_FOREVER = 0xFFFFFFFF
            };
        };


        /**
         * @enum getOption_t
         * @brief An enumeration that represents the options for retrieving event flags.
         *
         * The getOption_t enumeration defines the options for retrieving event flags from an event flags group.
         *
         * The possible values are:
         * - getOption_t::AND: All event flags must be present in the group.
         * - getOption_t::AND_CLEAR: All event flags must be present in the group. Event flags that satisfy the request are cleared.
         * - getOption_t::OR: Any event flag is satisfactory.
         * - getOption_t::OR_CLEAR: Any event flag is satisfactory. Event flags that satisfy the request are cleared.
         *
         * @see EventFlags
         * @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter3.md#event-flags
         */
        enum class getOption_t : UINT {
            /** All event flags must be present in the group. */
            AND = 0x02,
            /** All event flags must be present in the group. Event flags that satisfy the request are cleared */
            AND_CLEAR = 0x03,
            /** Any event flag is satisfactory. */
            OR = 0x00,
            /** Any event flag is satisfactory. Event flags that satisfy the request are cleared. */
            OR_CLEAR = 0x01
        };


        /**
         * @brief Get event flags from event flags group.
         *
         * This service retrieves event flags from the specified event flags group. Each event flags group contains 32
         * event flags. Each flag is represented by a single bit. This service can retrieve a variety of event flag
         * combinations, as selected by the input parameters.
         *
         * @param requestedFlags The requested event flags to retrieve.
         * @param getOption The get option to determine how the event flags are retrieved. Available options are:
         *                  - getOption_t::AND: All requested event flags must be present in the group.
         *                  - getOption_t::AND_CLEAR: All requested event flags must be present in the group. The
         *                    event flags that satisfy the request are cleared.
         *                  - getOption_t::OR: Any requested event flag is satisfactory.
         *                  - getOption_t::OR_CLEAR: Any requested event flag is satisfactory. The event flags that
         *                    satisfy the request are cleared.
         * @param waitOption The wait option to specify the maximum number of timer-ticks to stay suspended while
         *                   waiting for the event flags. The available options are:
         *                  - waitOption_t::timeout: Specifies the maximum number of timer-ticks to wait.
         *                  - NO_WAIT: Immediate return from this service regardless of whether or not it was successful
         *                  - WAIT_FOREVER: Suspend indefinitely until the event flags are available
         *
         * @return The return value of the method. It can be one of the following:
         *         - TX_SUCCESS: The event flags were retrieved successfully.
         *         - TX_NO_EVENTS: Service was unable to get the specified events within the specified time to wait.
         *         - Other error codes: If an error occurred while retrieving the event flags.
         *
         * @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_get
         */
        UINT get(ULONG requestedFlags, getOption_t getOption, waitOption_t waitOption);


        /**
         * @brief Retrieves the current value of the event flag group.
         *
         * Check the status of all the requested flags and returns immediately.
         *
         * @param requestedFlags The event flags to be retrieved.
         * @return The return value of the method.
         *         - TX_SUCCESS: all requested flags are set
         *         - TX_NO_EVENTS: not all requested flags are set
         */
        UINT get(ULONG requestedFlags);

        using BaseEventFlags::get;

        /**
         * @brief Retrieve the current state of the event flags.
         *
         * This method retrieves the current state of the event flags in the event flags group.
         *
         * @return The current state of the event flags.
         *
         * @see EventFlags
         */
        ULONG getFlags();


        /**
         * @brief Checks if the specified flags are set in the event flags group.
         *
         * This method checks if the specified flags are set in the event flags group.
         *
         * @param requestedFlags The flags to check.
         *
         * @return True if ALL the specified flags are set in the event flags group, false otherwise.
         */
        bool isSet(ULONG requestedFlags);


        /**
         * @brief Checks if the specified event flags are set.
         *
         * This method checks if the specified event flags are set in the event flags group.
         *
         * @param requestedFlags The event flags to check for. This parameter should be a combination of the event flags
         *                       to check for, using bitwise OR operator.
        * @param getOption The get option to determine how the event flags are retrieved. Available options are:
         *                  - getOption_t::AND: All requested event flags must be present in the group.
         *                  - getOption_t::AND_CLEAR: All requested event flags must be present in the group. The
         *                    event flags that satisfy the request are cleared.
         *                  - getOption_t::OR: Any requested event flag is satisfactory.
         *                  - getOption_t::OR_CLEAR: Any requested event flag is satisfactory. The event flags that
         *                    satisfy the request are cleared.
         *
         * @return `true` if the requested event flags are set, `false` otherwise.
         */
        bool isSet(ULONG requestedFlags, getOption_t getOption);


        /**
         * @brief Awaits the specified event flags.
         *
         * This method blocks the current thread until ALL the event flags specified by `requestedFlags` are set.
         *
         * @param requestedFlags The event flags to await.
         *
         * @return The result of the await operation.
         *         - TX_SUCCESS: Successfully awaited ALL the flags
         *
         * @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter3.md#event-flags
         */
        UINT await(ULONG requestedFlags);


        /**
         * @brief Awaits the specified event flags to be set.
         *
         * This method waits for the specified event flags to be set in the event flags group. The method will block until the
         * requested flags are set or the specified wait option is satisfied.
         *
         * @param requestedFlags The event flags to wait for.
         * @param waitOption The wait option to specify the maximum number of timer-ticks to stay suspended while
         *                   waiting for the event flags. The available options are:
         *                  - waitOption_t::timeout: Specifies the maximum number of timer-ticks to wait.
         *                  - NO_WAIT: Immediate return from this service regardless of whether or not it was successful
         *                  - WAIT_FOREVER: Suspend indefinitely until the event flags are available
         *
         * @return The result of the await operation.
         *         - TX_SUCCESS: Successfully awaited ALL the flags.
         *         - TX_NO_EVENTS: Service was unable to get the specified events within the specified time to wait.
         *
         * @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter3.md#event-flags
         */
        UINT await(ULONG requestedFlags, waitOption_t waitOption);


        /**
         * @brief Await the specified event flags.
         *
         * This method waits for the specified event flags to be set. It blocks until the specified flags are set.
         *
         * @param requestedFlags The event flags to be awaited.
         * @param getOption The get option to determine how the event flags are retrieved. Available options are:
         *                  - getOption_t::AND: All requested event flags must be present in the group.
         *                  - getOption_t::AND_CLEAR: All requested event flags must be present in the group. The
         *                    event flags that satisfy the request are cleared.
         *                  - getOption_t::OR: Any requested event flag is satisfactory.
         *                  - getOption_t::OR_CLEAR: Any requested event flag is satisfactory. The event flags that
         *                    satisfy the request are cleared.
         * @param waitOption The wait option to specify the maximum number of timer-ticks to stay suspended while
         *                   waiting for the event flags. The available options are:
         *                  - waitOption_t::timeout: Specifies the maximum number of timer-ticks to wait.
         *                  - NO_WAIT: Immediate return from this service regardless of whether or not it was successful
         *                  - WAIT_FOREVER: Suspend indefinitely until the event flags are available
         *
         * @return The result of the await operation.
         *         - TX_SUCCESS: Successfully awaited the flags.
         *         - TX_NO_EVENTS: Service was unable to get the specified events within the specified time to wait.
         *
         * @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter3.md#event-flags
         */
        UINT await(ULONG requestedFlags, getOption_t getOption, waitOption_t waitOption);


        /**
         * @brief Enum class representing options for setting event flags.
         *
         * This enum class is used to specify options for setting event flags in an event flags group. It contains two options:
         *     - AND: Specifies that the specified event flags should be ANDed into the current event flags in the group.
         *     - OR: Specifies that the specified event flags should be ORed with the current event flags in the group.
         *
         * @see Stm32ThreadX::EventFlags::set()
         */
        enum class setOption_t : UINT {
            /** Specified event flags are ANDed into the current event flags in the group. */
            AND = 0x02,
            /** The specified event flags are ORed with the current event in the group. */
            OR = 0x00
        };


        /**
         * @brief Set event flags in the event flags group.
         *
         * This method sets the specified event flags in the event flags group. The flags to set are determined by the
         * `flagsToSet` parameter. The `setOption` parameter specifies the behavior when setting the flags.
         *
         * @param flagsToSet The event flags to set. This is a bitmask value where each bit represents a flag.
         * @param setOption The behavior when setting the flags. This can be one of the following:
         *                  - `setOption_t::OR`: Sets the flags specified by `flagsToSet` and leaves the remaining flags unchanged.
         *                  - `setOption_t::AND`: Performs a logical AND operation between the existing flags and the flags specified by `flagsToSet`.
         *
         * @return The return value of the method. It can be one of the following:
         *         - `TX_SUCCESS`: The event flags were set successfully.
         *         - Other error codes: If an error occurred while setting the event flags.
         *
         * @see https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/threadx/chapter4.md#tx_event_flags_set
         */
        UINT set(ULONG flagsToSet, setOption_t setOption);


        /**
         * @brief Set event flags.
         *
         * This method sets the specified event flags in the event flags group.
         *
         * @param flagsToSet The flags to be set. The flagsToSet parameter is a combination of flag bits. Each flag bit
         *                   represents a specific event.
         *
         * @return The return value of the method. It can be one of the following:
         *         - TX_SUCCESS: The event flags were set successfully.
         *         - Other error codes: If an error occurred while setting the event flags.
         * @see Stm32ThreadX::EventFlags::set()
         */
        UINT set(const ULONG flagsToSet) { return set(flagsToSet, setOption_t::OR); }


        /**
         * @brief Clear specified event flags.
         *
         * This method clears the specified event flags in the event flags group. The event flags to clear are specified by the
         * `flagsToClear` parameter.
         *
         * @param flagsToClear The event flags to clear. The flags to be cleared are indicated by setting the corresponding bit(s)
         *                     to 1. Setting a bit to 0 will not clear the corresponding flag.
         *
         * @return The return value of the method.
         * @see Stm32ThreadX::EventFlags::set()
         */
        UINT clear(const ULONG flagsToClear) { return set(~flagsToClear, setOption_t::AND); }


        /**
         * @brief Clears all event flags in the event flags group.
         *
         * This method clears event flags in the group by calling the clear method with a default mask
         * that clears all flags.
         *
         * @return Completion status of the operation as a UINT. Returns TX_SUCCESS on success or an error code otherwise.
         */
        UINT clear() { return clear(ULONG_MAX); }

    private:
        /** Stores the last value of the actual flags returned by get() functions. */
        ULONG actualFlags = {};
    };
}

#endif //LIBSMART_STM32THREADX_EVENTFLAGS_HPP
