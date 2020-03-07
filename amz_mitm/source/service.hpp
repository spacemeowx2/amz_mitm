#pragma once
#include <switch.h>
#include <stratosphere.hpp>
#include <cstring>
#include <atomic>
#include "debug.hpp"

namespace ams::mitm::amz {
    class SetMitMService : public sf::IMitmServiceObject {
        private:
            enum class CommandId {
                GetT = 181,
            };
        public:
            SF_MITM_SERVICE_OBJECT_CTOR(SetMitMService) { /* ... */ }
            
            static bool ShouldMitm(const sm::MitmProcessInfo &client_info) {
                LogFormat("should_mitm pid: %" PRIu64 " tid: %" PRIx64, client_info.process_id, client_info.program_id);
                return true;
            }
        protected:
            /* Overridden commands. */
            Result GetT();
        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(GetT),
            };
    };
}
