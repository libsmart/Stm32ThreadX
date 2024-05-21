/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32THREADX_EVENTFLAGS_HPP
#define LIBSMART_STM32THREADX_EVENTFLAGS_HPP

#include <libsmart_config.hpp>
#include "Loggable.hpp"
#include "Nameable.hpp"
#include "tx_api.h"

namespace Stm32ThreadX {
    class EventFlags : public Stm32ItmLogger::Loggable, public Stm32Common::Nameable, public TX_EVENT_FLAGS_GROUP {
    public:
        ~EventFlags() override;

        EventFlags()
            : EventFlags(nullptr, &Stm32ItmLogger::emptyLogger) { ; }

        explicit EventFlags(Stm32ItmLogger::LoggerInterface *logger)
            : EventFlags(nullptr, logger) { ; }

        explicit EventFlags(const char *name)
            : EventFlags(name, &Stm32ItmLogger::emptyLogger) { ; }

        EventFlags(const char *name, Stm32ItmLogger::LoggerInterface *logger)
            : Loggable(logger),
              Nameable(name),
              TX_EVENT_FLAGS_GROUP_STRUCT() { ; }

        UINT create();

        UINT deleteFlags();

        // enum class Flags : ULONG {
        //     NONE = 0
        // };

        using waitOption_t = union {
            /** Specifies the maximum number of timer-ticks to stay suspended while waiting for the event flags. */
            ULONG timeout;

            enum waitOption : ULONG {
                /** Immediate return from this service */
                NO_WAIT = 0,
                /** Suspend indefinitely until the event flags are available */
                WAIT_FOREVER = 0xFFFFFFFF
            };
        };

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

        UINT get(ULONG requestedFlags);

        ULONG getFlags(ULONG requestedFlags);

        bool isSet(ULONG requestedFlags);

        bool isSet(ULONG requestedFlags, getOption_t getOption);

        UINT await(ULONG requestedFlags);

        UINT await(ULONG requestedFlags, waitOption_t waitOption);

        UINT await(ULONG requestedFlags, getOption_t getOption, waitOption_t waitOption);


        enum class setOption_t : UINT {
            /** Specified event flags are ANDed into the current event flags in the group. */
            AND = 0x02,
            /** The specified event flags are ORed with the current event in the group. */
            OR = 0x00
        };

        UINT set(ULONG flagsToSet, setOption_t setOption);

        UINT set(ULONG flagsToSet) { return set(flagsToSet, setOption_t::OR); };

        UINT clear(ULONG flagsToClear) { return set(~flagsToClear, setOption_t::AND); };

    private:
        ULONG actualFlags = {};
    };
}

#endif //LIBSMART_STM32THREADX_EVENTFLAGS_HPP
