// SPDX-License-Identifier: MIT
#ifndef CANDY_COMMON_NET_H
#define CANDY_COMMON_NET_H

#include "common/byteswap.h"
#include <Poco/Net/IPAddress.h>
#include <array>
#include <bit>
#include <cstdint>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <string>

namespace Candy {
template <typename T> T ntoh(T v) {
    if (std::endian::native == std::endian::little) {
        return byteswap(v);
    }
    return v;
}

class IP4 {
public:
    IP4 operator=(const std::string &ip);
    IP4 operator&(IP4 another) const;
    IP4 operator|(IP4 another) const;
    bool operator==(IP4 another) const;
    operator std::string() const;
    operator uint32_t() const;
    int fromString(const std::string &ip);
    std::string toString() const;
    int fromPrefix(int prefix);
    int toPrefix();

private:
    std::array<uint8_t, 4> raw;
};

class Address {
public:
    Address();
    Address(const std::string &cidr);
    IP4 Host() const;
    IP4 Mask() const;
    IP4 Net() const;
    int fromCidr(const std::string &cidr);
    std::string toCidr();

    static bool checkCidr(const std::string &cidr);

private:
    IP4 host;
    IP4 mask;
};

} // namespace Candy

#endif
