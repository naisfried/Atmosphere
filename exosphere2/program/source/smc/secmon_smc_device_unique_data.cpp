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
#include "secmon_smc_device_unique_data.hpp"

namespace ams::secmon::smc {

    SmcResult SmcDecryptDeviceUniqueData(SmcArguments &args) {
        /* TODO */
        return SmcResult::NotImplemented;
    }

    SmcResult SmcReencryptDeviceUniqueData(SmcArguments &args) {
        /* TODO */
        return SmcResult::NotImplemented;
    }

    /* Legacy APIs. */
    SmcResult SmcDecryptAndImportEsDeviceKey(SmcArguments &args) {
        /* TODO */
        return SmcResult::NotImplemented;
    }

    SmcResult SmcDecryptAndImportLotusKey(SmcArguments &args) {
        /* TODO */
        return SmcResult::NotImplemented;
    }

}
