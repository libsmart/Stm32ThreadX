/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Semaphore.hpp"
#include "Callback/CallbackCapable.hpp"
#include "Process/ProcessInterface.hpp"

using namespace Stm32Common;

namespace Stm32ThreadX {
    class IsrSemaphore : Process::ProcessInterface, public Nameable, public CallbackCapable<void> {
    public:
        IsrSemaphore() = delete;

        explicit IsrSemaphore(const char *name) : Nameable(name) { ; }

        UINT get(ULONG wait_option);

        void setup() override;

        void loop() override;

        void end() override;

        void errorHandler() override;

    protected:
        [[nodiscard]] const char *getDefaultName() const override;

    private:
        void callback() override;

        Semaphore semaphore{};

        static constexpr char defaultName[] = "IsrSemaphore";
    };
}
