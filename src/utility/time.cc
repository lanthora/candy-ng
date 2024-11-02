// SPDX-License-Identifier: MIT
#include "utility/time.h"
#include "core/net.h"
#include "utility/type.h"
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Platform.h>
#include <chrono>
#include <netinet/in.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>
#include <string>
#include <unistd.h>

namespace Candy {

bool useSystemTime = false;
std::string ntpServer;

struct ntp_packet {
    uint8 li_vn_mode = 0x23;

    uint8 stratum;
    uint8 poll;
    uint8 precision;

    uint32 rootDelay;
    uint32 rootDispersion;
    uint32 refId;

    uint32 refTm_s;
    uint32 refTm_f;

    uint32 origTm_s;
    uint32 origTm_f;

    uint32 rxTm_s;
    uint32 rxTm_f;

    uint32 txTm_s;
    uint32 txTm_f;
};

int64 ntpTime() {
    try {
        Poco::Net::DatagramSocket socket;
        socket.connect(Poco::Net::SocketAddress(ntpServer, 123));

        struct ntp_packet packet = {};
        socket.sendBytes(&packet, sizeof(packet));

        socket.setReceiveTimeout(Poco::Timespan(1, 0));
        int len = socket.receiveBytes(&packet, sizeof(packet));

        if (len != sizeof(packet) || (packet.li_vn_mode & 0x07) != 4) {
            spdlog::warn("invalid ntp response");
            return 0;
        }

        int64 retval = (int64)(ntoh(packet.rxTm_s));
        if (retval == 0) {
            spdlog::warn("invalid ntp response buffer: {:n}", spdlog::to_hex(std::string((char *)(&packet), sizeof(packet))));
            return 0;
        }

        // Fix ntp 2036 problem
        if (!(retval & 0x80000000)) {
            retval += UINT32_MAX;
        }

        retval -= 2208988800U;
        return retval;
    } catch (std::exception &e) {
        spdlog::debug("ntp time failed: {}", e.what());
        return 0;
    }
}

int64 unixTime() {
    using namespace std::chrono;

    int64 sysTime;
    int64 netTime;

    if (useSystemTime || ntpServer.empty()) {
        sysTime = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        return sysTime;
    }

    netTime = 0;
    for (int i = 0; i < 3 && netTime == 0; ++i) {
        if (i > 0) {
            spdlog::debug("get time from ntp server failed: retry {}", i);
        }
        netTime = ntpTime();
    }

    sysTime = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    if (std::abs(netTime - sysTime) < 3) {
        useSystemTime = true;
        spdlog::debug("system time is accurate");
    }
    if (netTime) {
        return netTime;
    }

    spdlog::warn("request network time failed");
    return sysTime;
}

int64 bootTime() {
    using namespace std::chrono;
    auto now = steady_clock::now();
    return duration_cast<milliseconds>(now.time_since_epoch()).count();
}

} // namespace Candy
