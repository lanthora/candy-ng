// SPDX-License-Identifier: MIT
#ifndef CANDY_TUN_TUN_H
#define CANDY_TUN_TUN_H

#include "core/net.h"
#include <any>
#include <string>

namespace Candy {

class Client;

class Tun {
public:
    Tun();
    ~Tun();

    int setName(const std::string &name);
    int setAddress(const std::string &cidr);
    IP4 getIP();
    int setMTU(int mtu);
    int setTimeout(int timeout);

    int run(Client *client);
    int shutdown();

private:
    int up();
    int down();

    int read(std::string &buffer);
    int write(const std::string &buffer);

    int setSysRtTable(IP4 dst, IP4 mask, IP4 nexthop);

private:
    std::any impl;

private:
    Client *client;
};

} // namespace Candy

#endif
