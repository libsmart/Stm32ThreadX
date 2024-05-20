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


        UINT get(ULONG requestedFlags, getOption_t getOption, waitOption_t waitOption);

        UINT get(ULONG requestedFlags) {
            return get(requestedFlags, getOption_t::AND, waitOption_t{waitOption_t::NO_WAIT});
        };

        ULONG getFlags(ULONG requestedFlags) {
            get(requestedFlags, getOption_t::AND, waitOption_t{waitOption_t::NO_WAIT});
            return actualFlags;
        };

        bool isSet(ULONG requestedFlags) {
            const auto flags = getFlags(requestedFlags);
            return flags & requestedFlags > 0;
        }

        UINT await(ULONG requestedFlags) {
            return await(requestedFlags, waitOption_t{waitOption_t::WAIT_FOREVER});
        };

        UINT await(ULONG requestedFlags, waitOption_t waitOption) {
            return await(requestedFlags, getOption_t::AND, waitOption);
        };

        UINT await(ULONG requestedFlags, getOption_t getOption, waitOption_t waitOption) {
            return get(requestedFlags, getOption, waitOption);
        };


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
