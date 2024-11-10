#include "tun/tun.h"
#include "core/client.h"
#include "core/message.h"
#include "core/net.h"
#include <spdlog/fmt/bin_to_hex.h>

namespace Candy {

int Tun::run(Client *client) {
    this->client = client;
    this->running = true;
    this->msgThread = std::thread([&] {
        while (this->running) {
            handleTunQueue();
        }
    });
    return 0;
}

int Tun::shutdown() {
    this->running = false;
    if (this->tunThread.joinable()) {
        this->tunThread.join();
    }
    if (this->msgThread.joinable()) {
        this->msgThread.join();
    }
    return 0;
}

void Tun::handleTunDevice() {}

void Tun::handleTunQueue() {
    Msg msg = this->client->tunMsgQueue.read();
    switch (msg.kind) {
    case MsgKind::PACKET:
        handlePacket(std::move(msg));
        break;
    case MsgKind::TUNADDR:
        handleTunAddr(std::move(msg));
        break;
    }
}

void Tun::handlePacket(Msg msg) {
    if (msg.data.size() < sizeof(IP4Header)) {
        spdlog::warn("invalid IPv4 packet: {:n}", spdlog::to_hex(msg.data));
        return;
    }
    IP4Header *header = (IP4Header *)msg.data.data();
    if (header->protocol == 0x04) {
        msg.data.erase(0, sizeof(IP4Header));
    }
    write(msg.data);
}

void Tun::handleTunAddr(Msg msg) {
    setAddress(msg.data);
}

} // namespace Candy
