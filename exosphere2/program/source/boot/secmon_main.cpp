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
#include "secmon_boot.hpp"
#include "secmon_boot_functions.hpp"
#include "../smc/secmon_random_cache.hpp"
#include "../secmon_setup.hpp"
#include "../secmon_misc.hpp"

namespace ams::secmon {

    void Main() {
        /* Set library register addresses. */
        /*   actmon::SetRegisterAddress(MemoryRegionVirtualDeviceActivityMonitor.GetAddress()); */
             clkrst::SetRegisterAddress(MemoryRegionVirtualDeviceClkRst.GetAddress());
               flow::SetRegisterAddress(MemoryRegionVirtualDeviceFlowController.GetAddress());
               fuse::SetRegisterAddress(MemoryRegionVirtualDeviceFuses.GetAddress());
                gic::SetRegisterAddress(MemoryRegionVirtualDeviceGicDistributor.GetAddress(), MemoryRegionVirtualDeviceGicCpuInterface.GetAddress());
                i2c::SetRegisterAddress(i2c::Port_1, MemoryRegionVirtualDeviceI2c1.GetAddress());
                i2c::SetRegisterAddress(i2c::Port_5, MemoryRegionVirtualDeviceI2c5.GetAddress());
        /*   pinmux::SetRegisterAddress(); */
                pmc::SetRegisterAddress(MemoryRegionVirtualDevicePmc.GetAddress());
                 se::SetRegisterAddress(MemoryRegionVirtualDeviceSecurityEngine.GetAddress());
               uart::SetRegisterAddress(MemoryRegionVirtualDeviceUart.GetAddress());
                wdt::SetRegisterAddress(MemoryRegionVirtualDeviceTimer.GetAddress());
               util::SetRegisterAddress(MemoryRegionVirtualDeviceTimer.GetAddress());

        /* Get the secure monitor parameters. */
        auto &secmon_params = *reinterpret_cast<pkg1::SecureMonitorParameters *>(MemoryRegionVirtualDeviceBootloaderParams.GetAddress());

        /* Perform initialization. */
        {
            /* Perform initial setup. */
            /* This checks the security engine's validity, and configures common interrupts in the GIC. */
            /* This also initializes the global configuration context. */
            secmon::Setup1();

            /* Save the boot info. */
            secmon::SaveBootInfo(secmon_params);

            /* Perform cold-boot specific init. */
            secmon::boot::InitializeColdBoot();

            /* Setup the SoC security measures. */
            secmon::SetupSocSecurity();

            /* Setup the Cpu core context. */
            secmon::SetupCpuCoreContext();

            /* Clear the crt0 code that was present in iram. */
            secmon::boot::ClearIram();

            /* Alert the bootloader that we're initialized. */
            secmon_params.secmon_state = pkg1::SecureMonitorState_Initialized;

            /* Initialize the random cache. */
            secmon::smc::FillRandomCache();
        }

        /* Wait for NX Bootloader to finish loading the BootConfig. */
        secmon::boot::WaitForNxBootloader(secmon_params, pkg1::BootloaderState_LoadedBootConfig);
        hw::DataSynchronizationBarrierInnerShareable();

        /* Load the bootconfig. */
        secmon::boot::LoadBootConfig(MemoryRegionPhysicalIramBootConfig.GetPointer());

        /* Verify or clear the boot config. */
        secmon::boot::VerifyOrClearBootConfig();

        /* Get the boot config. */
        const auto &bc = secmon::GetBootConfig();

        /* Set the tsc value by the boot config. */
        {
            constexpr u64 TscMask = (static_cast<u64>(1) << 55) - 1;

            secmon::boot::EnableTsc(bc.data.GetInitialTscValue() & TscMask);
        }
    }

}