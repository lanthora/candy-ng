#include "core/client.h"

namespace Candy {

Msg MsgQueue::read() {
    std::unique_lock lock(msgMutex);
    msgCondition.wait(lock, [this] { return !msgQueue.empty(); });

    Msg msg = std::move(msgQueue.front());
    msgQueue.pop();
    return msg;
}

void MsgQueue::write(Msg msg) {
    {
        std::unique_lock lock(this->msgMutex);
        while (msg.kind == MsgKind::SHUTDOWN && !msgQueue.empty()) {
            msgQueue.pop();
        }
        msgQueue.push(std::move(msg));
    }
    msgCondition.notify_one();
}

void Client::setName(const std::string &name) {
    tun.setName(name);
}

void Client::setPassword(const std::string &password) {
    ws.setPassword(password);
    peer.setPassword(password);
}

void Client::setWebSocket(const std::string &uri) {
    ws.setWsServerUri(uri);
}

void Client::setTunAddress(const std::string &cidr) {
    tun.setAddress(cidr);
}

void Client::setExptTunAddress(const std::string &cidr) {
    ws.setExptTunAddress(cidr);
}

void Client::setVirtualMac(const std::string &vmac) {
    ws.setVirtualMac(vmac);
}

void Client::setStun(const std::string &stun) {
    peer.setStun(stun);
}

void Client::setDiscoveryInterval(int interval) {
    peer.setDiscoveryInterval(interval);
}

void Client::setRouteCost(int cost) {
    peer.setRouteCost(cost);
}

void Client::setPort(int port) {
    peer.setPort(port);
}

void Client::setLocalhost(std::string ip) {
    peer.setLocalhost(ip);
}

void Client::setMtu(int mtu) {
    tun.setMTU(mtu);
}

std::string Client::tunAddress() {
    return tun.getIP().toString();
};

int Client::run() {
    ws.run(this);
    tun.run(this);
    peer.run(this);
    return 0;
}

int Client::shutdown() {
    ws.shutdown();
    tun.shutdown();
    peer.shutdown();
    return 0;
}

} // namespace Candy
