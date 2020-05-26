/*
 * Copyright (c) 2018-2020 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <exosphere.hpp>
#include "secmon_error.hpp"

namespace ams::diag {

    void AbortImpl() {
        secmon::SetError(pkg1::ErrorInfo_UnknownAbort);
        secmon::ErrorReboot();
    }

}

namespace ams::secmon {

    void SetError(pkg1::ErrorInfo info) {
        const uintptr_t address = secmon::MemoryRegionVirtualDevicePmc.GetAddress() + PKG1_SECURE_MONITOR_PMC_ERROR_SCRATCH;

        if (reg::Read(address) == pkg1::ErrorInfo_None) {
            reg::Write(address, info);
        }
    }

    NORETURN void ErrorReboot() {
        /* Lockout the security engine. */
        se::Lockout();

        /* TODO: Lockout fuses. */

        /* TODO: Disable SE Crypto Operations. */

        while (true) {
            wdt::Reboot();
        }
    }

}