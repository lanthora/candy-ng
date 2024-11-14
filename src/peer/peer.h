// SPDX-License-Identifier: MIT
#ifndef CANDY_PEER_PEER_H
#define CANDY_PEER_PEER_H

#include "core/message.h"
#include <string>
#include <thread>

namespace Candy {

class Client;

class Peer {
public:
    int setPassword(const std::string &password);
    int setStun(const std::string &stun);
    int setDiscoveryInterval(int interval);
    int setForwardCost(int cost);
    int setPort(int port);
    int setLocalhost(std::string ip);

    int run(Client *client);
    int shutdown();

private:
    // 处理来自消息队列的数据
    void handlePeerQueue();
    void handlePacket(Msg msg);

    std::thread msgThread;

private:
    Client *client;
};

} // namespace Candy

#endif
