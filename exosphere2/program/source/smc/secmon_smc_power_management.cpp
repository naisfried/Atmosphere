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
#include "../secmon_error.hpp"
#include "secmon_smc_power_management.hpp"

namespace ams::secmon::smc {

    namespace {

        constinit bool g_charger_hi_z_mode_enabled = false;

    }

    SmcResult SmcPowerOffCpu(const SmcArguments &args) {
        /* TODO */
        return SmcResult::NotImplemented;
    }

    SmcResult SmcPowerOnCpu(const SmcArguments &args) {
        /* TODO */
        return SmcResult::NotImplemented;
    }

    SmcResult SmcSuspendCpu(const SmcArguments &args) {
        /* TODO */
        return SmcResult::NotImplemented;
    }

    bool IsChargerHiZModeEnabled() {
        return g_charger_hi_z_mode_enabled;
    }

    void SetChargerHiZModeEnabled(bool en) {
        g_charger_hi_z_mode_enabled = en;
    }

}
