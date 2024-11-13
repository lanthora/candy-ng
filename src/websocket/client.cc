// SPDX-License-Identifier: MIT
#include "websocket/client.h"
#include "core/client.h"
#include "core/message.h"
#include "utility/time.h"
#include "websocket/message.h"
#include <Poco/Exception.h>
#include <Poco/Net/HTTPMessage.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/SocketDefs.h>
#include <Poco/Timespan.h>
#include <Poco/URI.h>
#include <memory>
#include <spdlog/spdlog.h>

namespace Candy {

int WebSocketClient::setPassword(const std::string &password) {
    this->password = password;
    return 0;
}

int WebSocketClient::setWsServerUri(const std::string &uri) {
    this->wsServerUri = uri;
    return 0;
}

int WebSocketClient::setExptTunAddress(const std::string &cidr) {
    this->exptTunCidr = cidr;
    return 0;
}

int WebSocketClient::setAddress(const std::string &cidr) {
    this->tunCidr = cidr;
    return 0;
}

int WebSocketClient::setVirtualMac(const std::string &vmac) {
    return 0;
}

int WebSocketClient::run(Client *client) {
    this->client = client;
    this->running = true;
    this->msgThread = std::thread([&] {
        while (this->running) {
            handleWsQueue();
        }
    });
    return 0;
}

int WebSocketClient::shutdown() {
    this->running = false;
    if (this->msgThread.joinable()) {
        this->msgThread.join();
    }
    return 0;
}

void WebSocketClient::handleWsQueue() {
    Msg msg = this->client->wsMsgQueue.read();
    switch (msg.kind) {
    case MsgKind::NONE:
        break;
    case MsgKind::PACKET:
        handlePacket(std::move(msg));
        break;
    default:
        spdlog::warn("unexcepted websocket message type: {}", static_cast<int>(msg.kind));
        break;
    }
}

void WebSocketClient::handlePacket(Msg msg) {
    msg.data.insert(0, 1, WsMsgKind::FORWARD);
    this->ws->sendFrame(msg.data.data(), msg.data.size(), Poco::Net::WebSocket::FRAME_BINARY);
}

int WebSocketClient::connect() {
    std::shared_ptr<Poco::URI> uri;
    try {
        uri = std::make_shared<Poco::URI>(wsServerUri);
    } catch (std::exception &e) {
        spdlog::critical("invalid websocket server: {}: {}", wsServerUri, e.what());
        return -1;
    }

    try {
        const std::string path = uri->getPath().empty() ? "/" : uri->getPath();
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
        Poco::Net::HTTPResponse response;
        if (uri->getScheme() == "wss") {
            using Poco::Net::Context;
            Context::Ptr context = new Context(Context::TLS_CLIENT_USE, "", "", "", Context::VERIFY_NONE);
            Poco::Net::HTTPSClientSession cs(uri->getHost(), uri->getPort(), context);
            this->ws = std::make_shared<Poco::Net::WebSocket>(cs, request, response);
        } else if (uri->getScheme() == "ws") {
            Poco::Net::HTTPClientSession cs(uri->getHost(), uri->getPort());
            this->ws = std::make_shared<Poco::Net::WebSocket>(cs, request, response);
        } else {
            spdlog::critical("invalid websocket scheme: {}", wsServerUri);
            return -1;
        }
        this->timestamp = bootTime();
        return 0;
    } catch (std::exception &e) {
        spdlog::critical("websocket connect failed: {}", e.what());
        return -1;
    }
}

int WebSocketClient::disconnect() {
    try {
        if (this->ws) {
            this->ws->shutdown();
            this->ws->close();
            this->ws.reset();
        }
    } catch (std::exception &e) {
        spdlog::debug("websocket disconnect failed: {}", e.what());
    }
    return 0;
}

} // namespace Candy
