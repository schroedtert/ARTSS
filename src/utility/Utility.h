/// \file       Utility.h
/// \brief      Offers some tools
/// \date       October 01, 2019
/// \author     My Linh Würzburger
/// \copyright  <2015-2020> Forschungszentrum Juelich GmbH. All rights reserved.

#ifndef ARTSS_UTILITY_UTILITY_H_
#define ARTSS_UTILITY_UTILITY_H_

#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <iterator>

#include "Utility.h"
#include "GlobalMacrosTypes.h"

class Utility {
 public:
    static std::vector<size_t> coordinateFromLinearIndex(size_t idx,
                                                         size_t Nx,
                                                         size_t Ny);
    static std::vector<std::string> split(const char* text,
                                          char delimiter);
    static std::vector<std::string> split(const std::string& text,
                                          char delimiter);
 private:
    Utility() = default;
};


#endif // ARTSS_UTILITY_UTILITY_H_
