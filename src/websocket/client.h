// SPDX-License-Identifier: MIT
#ifndef CANDY_WEBSOCKET_CLIENT_H
#define CANDY_WEBSOCKET_CLIENT_H

#include "websocket/message.h"
#include <Poco/Net/WebSocket.h>
#include <string>

namespace Candy {

class WebSocketClient {
public:
    int connect(const std::string &address);
    int disconnect();

    int setTimeout(int timeout);

    int read(WebSocketMessage &message);
    int write(const WebSocketMessage &message);

    int setPingMessage(const std::string &message);
    int sendPingMessage();

private:
    int sendPingMessage(WebSocketMessage &message);

    int timeout;
    std::shared_ptr<Poco::Net::WebSocket> ws;
    int64_t timestamp;
    std::string pingMessage;
};

} // namespace Candy

#endif
