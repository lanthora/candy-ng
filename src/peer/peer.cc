// SPDX-License-Identifier: MIT
#include "peer/peer.h"
#include "core/client.h"
#include "core/message.h"
#include <spdlog/spdlog.h>

namespace Candy {

int Peer::setPassword(const std::string &password) {
    return 0;
}

int Peer::setStun(const std::string &stun) {
    return 0;
}

int Peer::setDiscoveryInterval(int interval) {
    return 0;
}

int Peer::setForwardCost(int cost) {
    return 0;
}

int Peer::setPort(int port) {
    return 0;
}

int Peer::setLocalhost(std::string ip) {
    return 0;
}

int Peer::run(Client *client) {
    this->client = client;
    this->msgThread = std::thread([&] {
        while (this->client->running) {
            handlePeerQueue();
        }
    });
    return 0;
}

int Peer::shutdown() {
    if (this->msgThread.joinable()) {
        this->msgThread.join();
    }
    return 0;
}

void Peer::handlePeerQueue() {
    Msg msg = this->client->tunMsgQueue.read();
    switch (msg.kind) {
    case MsgKind::TIMEOUT:
        break;
    case MsgKind::PACKET:
        handlePacket(std::move(msg));
        break;
    default:
        spdlog::warn("unexcepted peer message type: {}", static_cast<int>(msg.kind));
        break;
    }
}

void Peer::handlePacket(Msg msg) {
    // TODO: 尝试建立 P2P 连接,并通过 P2P 转发流量

    // 无法通过 P2P 转发流量,交给 WS 模块通过服务端转发
    this->client->wsMsgQueue.write(std::move(msg));
}

} // namespace Candy
