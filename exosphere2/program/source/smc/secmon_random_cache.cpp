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
#include "secmon_smc_common.hpp"
#include "secmon_random_cache.hpp"

namespace ams::secmon::smc {

    namespace {

        constexpr inline size_t MaxRandomBytes  = sizeof(SmcArguments) - sizeof(SmcArguments{}.r[0]);

        constinit int g_random_offset_low  = 0;
        constinit int g_random_offset_high = 0;

        void FillRandomCache(int offset, int size) {
            /* Get the cache. */
            u8 * const random_cache_loc = GetRandomBytesCache() + offset;

            /* Flush the region we're about to fill to ensure consistency with the SE. */
            hw::FlushDataCache(random_cache_loc, size);
            hw::DataSynchronizationBarrierInnerShareable();

            /* Generate random bytes. */
            se::GenerateRandomBytes(random_cache_loc, size);
            hw::DataSynchronizationBarrierInnerShareable();

            /* Flush to ensure the CPU sees consistent data for the region. */
            hw::FlushDataCache(random_cache_loc, size);
            hw::DataSynchronizationBarrierInnerShareable();
        }

    }

    void FillRandomCache() {
        /* Fill the cache. */
        FillRandomCache(0, GetRandomBytesCacheSize());

        /* Set the extents. */
        g_random_offset_low  = 0;
        g_random_offset_high = GetRandomBytesCacheSize() - 1;
    }

    void RefillRandomCache() {
        /* Check that we need to do any refilling. */
        if (const int used_start = (g_random_offset_high + 1) % GetRandomBytesCacheSize(); used_start != g_random_offset_low) {
            if (used_start < g_random_offset_low) {
                /* The region we need to fill is after used_start but before g_random_offset_low. */
                const auto size = g_random_offset_low - used_start;
                FillRandomCache(used_start, size);
                g_random_offset_high += size;
            } else {
                /* We need to fill the space from high to the end and from low to start. */
                const int high_size = GetRandomBytesCacheSize() - used_start;
                if (high_size > 0) {
                    FillRandomCache(used_start, high_size);
                    g_random_offset_high += high_size;
                }

                const int low_size  = g_random_offset_low;
                if (low_size > 0) {
                    FillRandomCache(0, low_size);
                    g_random_offset_high += low_size;
                }

            }

            g_random_offset_high %= GetRandomBytesCacheSize();
        }
    }

    void GetRandomFromCache(void *dst, size_t size) {
        u8 * const cache = GetRandomBytesCache();
        u8 * cur_dst     = static_cast<u8 *>(dst);

        /* NOTE: Nintendo does not do bounds checking here, and does not do multiple reads when the get would wrap around. */
        while (size > 0) {
            const size_t copy_size = std::min(size, static_cast<size_t>(GetRandomBytesCacheSize() - g_random_offset_low));
            std::memcpy(cur_dst, cache + g_random_offset_low, copy_size);

            cur_dst += copy_size;
            size    -= copy_size;

            if (g_random_offset_low + copy_size >= GetRandomBytesCacheSize()) {
                g_random_offset_low = 0;
            }
        }
    }

}
