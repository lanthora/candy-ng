// SPDX-License-Identifier: MIT
#ifndef CANDY_WEBSOCKET_CLIENT_H
#define CANDY_WEBSOCKET_CLIENT_H

#include "core/message.h"
#include "utility/type.h"
#include <Poco/Net/WebSocket.h>
#include <string>

namespace Candy {

class Client;

class WebSocketClient {
public:
    int setPassword(const std::string &password);
    int setWsServerUri(const std::string &uri);
    int setExptTunAddress(const std::string &cidr);
    int setAddress(const std::string &cidr);
    int setVirtualMac(const std::string &vmac);

    int run(Client *client);
    int shutdown();

private:
    void handleWsQueue();
    void handlePacket(Msg msg);

    std::thread msgThread;

private:
    int connect();
    int disconnect();

    std::shared_ptr<Poco::Net::WebSocket> ws;
    std::string pingMessage;
    int64 timestamp;

private:
    std::string wsServerUri;
    std::string exptTunCidr;
    std::string tunCidr;
    std::string password;
    Client *client;
};

} // namespace Candy

#endif
