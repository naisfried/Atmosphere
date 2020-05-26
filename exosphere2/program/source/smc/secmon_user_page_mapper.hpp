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
#pragma once
#include <exosphere.hpp>
#include "secmon_smc_common.hpp"

namespace ams::secmon::smc {

    class UserPageMapper {
        private:
            uintptr_t physical_address;
            uintptr_t virtual_address;
        public:
            constexpr UserPageMapper(uintptr_t phys) : physical_address(util::AlignDown(phys, 4_KB)), virtual_address() { /* ... */ }

            bool Map();
            void *GetPointerTo(uintptr_t phys, size_t size) const;
            bool CopyToUser(uintptr_t dst_phys, const void *src, size_t size) const;
            bool CopyFromUser(void *dst, uintptr_t src_phys, size_t size) const;
    };

}
