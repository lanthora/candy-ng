// SPDX-License-Identifier: MIT
#ifndef CANDY_PEER_MESSAGE_H
#define CANDY_PEER_MESSAGE_H

#include "core/net.h"

namespace Candy {
namespace PeerMsgKind {
constexpr uint8 HEARTBEAT = 0;
constexpr uint8 FORWARD = 1;
constexpr uint8 DELAY = 2;
// TODO: 遗漏了 3, 新功能时使用
constexpr uint8 ROUTE = 4;
} // namespace PeerMsgKind
} // namespace Candy

#endif
