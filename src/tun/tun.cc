#include "tun/tun.h"
#include "core/client.h"

namespace Candy {

int Tun::run(Client *client) {
    this->client = client;
    return 0;
}

int Tun::shutdown() {
    this->client->tunMsgQueue.write(Msg(MsgKind::SHUTDOWN));
    return 0;
}

} // namespace Candy
