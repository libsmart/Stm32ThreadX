/*
 * SPDX-FileCopyrightText: 2026 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Semaphore.hpp"
#include "Callback/CallbackCapable.hpp"
#include "Process/ProcessInterface.hpp"

namespace Stm32ThreadX {
    class IsrSemaphore : Stm32Common::Process::ProcessInterface,
                         public Stm32Common::Nameable,
                         public Stm32Common::CallbackCapable<void> {
    public:
        static constexpr const char *COMPONENT_NAME = Stm32ThreadX::COMPONENT_NAME;
        static constexpr char CLASS_NAME[] = "IsrSemaphore";
        const char *INSTANCE_NAME{getName()};

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
    };
}
