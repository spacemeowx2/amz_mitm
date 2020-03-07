#include "ISystemSettingsServer.hpp"

namespace ams::mitm::amz {
    // https://reswitched.github.io/SwIPC/ifaces.html#nn::settings::ISystemSettingsServer

    Result ISystemSettingsServer::GetT() {
        LogFormat("ISystemSettingsServer::GetT");

        return ResultSuccess();
    }
}
