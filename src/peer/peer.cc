// SPDX-License-Identifier: MIT
#include "peer/peer.h"
#include "core/client.h"

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

int Peer::setRouteCost(int cost) {
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
    return 0;
}

int Peer::shutdown() {
    this->client->peerMsgQueue.write(Msg(MsgKind::SHUTDOWN));
    return 0;
}

} // namespace Candy