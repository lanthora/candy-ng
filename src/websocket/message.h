// SPDX-License-Identifier: MIT
#ifndef CANDY_WEBSOCKET_MESSAGE_H
#define CANDY_WEBSOCKET_MESSAGE_H

#include "core/net.h"
#include "utility/type.h"
#include <Poco/Net/WebSocket.h>
#include <openssl/sha.h>

namespace Candy {

namespace WsMsgKind {
constexpr uint8 AUTH = 0;
constexpr uint8 FORWARD = 1;
constexpr uint8 EXPTTUN = 2;
constexpr uint8 UDP4CONN = 3;
constexpr uint8 VMAC = 4;
constexpr uint8 DISCOVERY = 5;
constexpr uint8 ROUTE = 6;
constexpr uint8 GENERAL = 255;
} // namespace WsMsgKind

namespace GeSubType {
constexpr uint8 LOCALUDP4CONN = 0;
}

namespace WsMsg {

struct __attribute__((packed)) Auth {
    uint8 type;
    IP4 ip;
    int64 timestamp;
    uint8 hash[SHA256_DIGEST_LENGTH];

    Auth(IP4 ip);
    void updateHash(const std::string &password);
    bool check(const std::string &password);
};

struct __attribute__((packed)) Forward {
    uint8 type;
    IP4Header iph;

    Forward();
};

struct __attribute__((packed)) ExptTun {
    uint8 type;
    int64 timestamp;
    char cidr[32];
    uint8 hash[SHA256_DIGEST_LENGTH];

    ExptTun(const std::string &cidr);
    void updateHash(const std::string &password);
    bool check(const std::string &password);
};

struct __attribute__((packed)) Udp4Conn {
    uint8 type;
    IP4 src;
    IP4 dst;
    IP4 ip;
    uint16 port;

    Udp4Conn();
};

struct __attribute__((packed)) VMac {
    uint8 type;
    uint8 vmac[16];
    int64 timestamp;
    uint8 hash[SHA256_DIGEST_LENGTH];

    VMac(const std::string &vmac);
    void updateHash(const std::string &password);
    bool check(const std::string &password);
};

struct __attribute__((packed)) Discovery {
    uint8 type;
    IP4 src;
    IP4 dst;

    Discovery();
};

struct __attribute__((packed)) SysRouteItem {
    IP4 dst;
    IP4 mask;
    IP4 nexthop;
};

struct __attribute__((packed)) SysRoute {
    uint8 type;
    uint8 size;
    uint16 reserved;
    SysRouteItem rtTable[0];
};

struct __attribute__((packed)) General {
    uint8 type;
    uint8 subtype;
    uint16 extra;
    IP4 src;
    IP4 dst;

    General();
};

struct __attribute__((packed)) LocalUDP4 {
    General ge;
    uint32 ip;
    uint16 port;
};

} // namespace WsMsg
} // namespace Candy

#endif
