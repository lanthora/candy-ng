// SPDX-License-Identifier: MIT
#ifndef CANDY_UTILITY_TIME_H
#define CANDY_UTILITY_TIME_H

#include "utility/type.h"
#include <string>

namespace Candy {

extern bool useSystemTime;
extern std::string ntpServer;

int64 unixTime();
int64 bootTime();

} // namespace Candy

#endif
