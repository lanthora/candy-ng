#include "core/client.h"
#include "core/message.h"

namespace Candy {

Msg MsgQueue::read() {
    std::unique_lock lock(msgMutex);
    if (!msgCondition.wait_for(lock, std::chrono::milliseconds(100), [this] { return !msgQueue.empty(); })) {
        return Msg();
    }

    Msg msg = std::move(msgQueue.front());
    msgQueue.pop();
    return msg;
}

void MsgQueue::write(Msg msg) {
    {
        std::unique_lock lock(this->msgMutex);
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
    ws.setAddress(cidr);
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
    peer.setForwardCost(cost);
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
    // TODO: 获取虚拟网卡地址
    // 从 TUN 模块取? 还是保存 Auth 使用的地址?
    return "";
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
