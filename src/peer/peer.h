// SPDX-License-Identifier: MIT
#ifndef CANDY_PEER_PEER_H
#define CANDY_PEER_PEER_H

#include <string>

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
    bool running;

private:
    Client *client;
};

} // namespace Candy

#endif
