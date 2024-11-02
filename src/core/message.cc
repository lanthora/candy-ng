// SPDX-License-Identifier: MIT
#include "core/message.h"
#include <spdlog/spdlog.h>

namespace Candy {

Msg::Msg(MsgKind kind) {
    this->kind = kind;
}

Msg::Msg(Msg &&packet) {
    kind = packet.kind;
    data = std::move(packet.data);
}

Msg &Msg::operator=(Msg &&packet) {
    kind = packet.kind;
    data = std::move(packet.data);
    return *this;
}

} // namespace Candy
