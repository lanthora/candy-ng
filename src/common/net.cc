#include "common/net.h"
#include <exception>

namespace Candy {
IP4 IP4::operator=(const std::string &ip) {
    fromString(ip);
    return *this;
}

IP4::operator std::string() const {
    return toString();
}

IP4 IP4::operator|(IP4 another) const {
    for (int i = 0; i < this->raw.size(); ++i) {
        another.raw[i] |= this->raw[i];
    }
    return another;
}

bool IP4::operator==(IP4 another) const {
    return this->raw == another.raw;
}

IP4 IP4::operator&(IP4 another) const {
    for (int i = 0; i < this->raw.size(); ++i) {
        another.raw[i] &= this->raw[i];
    }
    return another;
}

int IP4::fromString(const std::string &ip) {
    this->raw = Poco::Net::IPAddress(ip).toV4Bytes();
    return 0;
}

std::string IP4::toString() const {
    return Poco::Net::IPAddress(this->raw.data(), sizeof(this->raw)).toString();
}

int IP4::fromPrefix(int prefix) {
    std::memset(raw.data(), 0, sizeof(raw));
    for (int i = 0; i < prefix; ++i) {
        raw[i / 8] |= (0x80 >> (i % 8));
    }
    return 0;
}

int IP4::toPrefix() {
    int i;
    for (i = 0; i < 32; ++i) {
        if (!(raw[i / 8] & (0x80 >> (i % 8)))) {
            break;
        }
    }
    return i;
}

Address::Address() {}

Address::Address(const std::string &cidr) {
    fromCidr(cidr);
}

IP4 Address::Host() const {
    return this->host;
}

IP4 Address::Mask() const {
    return this->mask;
}

IP4 Address::Net() const {
    return this->host & this->mask;
}

int Address::fromCidr(const std::string &cidr) {
    try {
        std::size_t pos = cidr.find('/');
        host.fromString(cidr.substr(0UL, pos));
        mask.fromPrefix(std::stoi(cidr.substr(pos + 1)));
    } catch (std::exception &e) {
        spdlog::debug("parse cidr failed: {}", e.what());
        return -1;
    }
    return 0;
}

std::string Address::toCidr() {
    return host.toString() + "/" + std::to_string(mask.toPrefix());
}

bool Address::checkCidr(const std::string &cidr) {
    // TODO: 检查 CIDR 格式是否合法
    return true;
}

} // namespace Candy
