// SPDX-License-Identifier: MIT
#ifndef CANDY_WEBSOCKET_SERVER_H
#define CANDY_WEBSOCKET_SERVER_H

#include "websocket/message.h"
#include <Poco/Net/HTTPServer.h>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <queue>
#include <string>

namespace Candy {

class WebSocketServer {
public:
    int listen(const std::string &host, uint16_t port);
    int stop();

    int setTimeout(int timeout);

    int read(WebSocketMessage &message);
    int write(const WebSocketMessage &message);

    int close(WebSocketConn conn);

    void push(const WebSocketMessage &msg);

    bool running;
    int timeout;

private:
    std::mutex mutex;
    std::condition_variable condition;
    std::queue<WebSocketMessage> queue;
    std::shared_ptr<Poco::Net::HTTPServer> server;
};

} // namespace Candy

#endif
