/*
 * Copyright (c) 2018 Atmosphère-NX
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
 
#include <switch.h>
#include "debug.hpp"

#include "service.hpp"
#include "ldn_icommunication.hpp"

namespace ams::mitm::amz {
    Result LdnMitMService::CreateUserLocalCommunicationService(sf::Out<std::shared_ptr<ICommunicationInterface>> out) {
        LogFormat("CreateUserLocalCommunicationService: enabled %d", static_cast<u32>(LdnConfig::getEnabled()));

        if (LdnConfig::getEnabled()) {
            auto comm = std::make_shared<ICommunicationInterface>();
            out.SetValue(std::move(comm));
            return 0;
        }

        return sm::mitm::ResultShouldForwardToSession();
    }
    Result LdnMitMService::CreateLdnMitmConfigService(sf::Out<std::shared_ptr<LdnConfig>> out) {
        out.SetValue(std::move(std::make_shared<LdnConfig>()));

        return 0;
    }
}
