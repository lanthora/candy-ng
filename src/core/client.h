// SPDX-License-Identifier: MIT
#ifndef CANDY_CORE_CLIENT_H
#define CANDY_CORE_CLIENT_H

#include "core/message.h"
#include "peer/peer.h"
#include "tun/tun.h"
#include "websocket/client.h"
#include <queue>
#include <string>

namespace Candy {

/* 各模块之间通过消息队列通信 */
class MsgQueue {
public:
    // 阻塞读,当收到 MsgKind::SHUTDOWN 后执行退出流程,不能再读
    Msg read();
    // 向队列中写入消息,在进程退出时模块清空自身的消息队列并写入 SHUTDOWN
    void write(Msg msg);

private:
    std::queue<Msg> msgQueue;
    std::mutex msgMutex;
    std::condition_variable msgCondition;
};

/* 客户端只负责维护模块间通信的消息队列,以及进程启动时的参数透传,本身不提供实质性的功能 */
class Client {
public:
    // 通过配置文件或命令行设置的参数
    void setName(const std::string &name);
    void setPassword(const std::string &password);
    void setWebSocket(const std::string &uri);
    void setTunAddress(const std::string &cidr);
    void setStun(const std::string &stun);
    void setDiscoveryInterval(int interval);
    void setRouteCost(int cost);
    void setPort(int port);
    void setLocalhost(std::string ip);
    void setMtu(int mtu);

    // 期望使用的地址,当地址可用时服务端优先分配这个地址
    void setExptTunAddress(const std::string &cidr);
    // 虚拟的硬件地址,在程序第一次启动时随机生成的 16 位随机字符串,用于和最近一次使用的地址绑定
    // 当相同虚拟硬件地址的设备登录时,判定为前一个客户端已断开,踢出前一个客户端并分配为与前一个客户端相同的 IP
    void setVirtualMac(const std::string &vmac);

    // 获取手动设置或服务端分配的虚拟地址,可以保存下来用于下次启动时设置期望的地址
    std::string tunAddress();

    // 启动客户端,非阻塞
    int run();
    // 关闭客户端,阻塞,直到所有子模块退出
    int shutdown();

public:
    //  三个消息队列,子模块使用这些队列通信
    MsgQueue tunMsgQueue, peerMsgQueue, wsMsgQueue;

private:
    // TUN 模块,与本机通信
    Tun tun;
    // PEER 模块,用于建立 P2P 连接,以及在 P2P 连接之上的客户端中继功能
    Peer peer;
    // WS 模块,主要处理与服务端之间的控制信息,在 P2P 无法使用时提供服务端中继
    WebSocketClient ws;
};

} // namespace Candy

#endif