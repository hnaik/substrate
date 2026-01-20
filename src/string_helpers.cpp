#include "string_helpers.h"

#include <sstream>
#include <string>
#include <vector>

namespace substrate {
std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> tokens;
    std::istringstream is{s};
    std::string token;
    while(std::getline(is, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}
} // namespace substrate