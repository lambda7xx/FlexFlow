#include "utils/parse.h"
#include "utils/containers.h"
#include "utils/variant.h"

namespace FlexFlow{

void ArgsParser::add_argument(std::string const &key,
                    AllowedArgTypes const &value,
                    std::string const &description) {
    mDefaultValues[parseKey(key)] = value;
    mDescriptions[key] = description;
  }

template <>
int ArgsParser::convert<int>(std::string const &s) const {
  return std::stoi(s);
}

template <>
float ArgsParser::convert<float>(std::string const &s) const {
  return std::stof(s);
}

template <>
bool ArgsParser::convert<bool>(std::string const &s) const {
  return s == "true" || s == "1" || s == "yes";
}

template <>
std::string ArgsParser::convert<std::string>(std::string const &s) const {
  return s;
}

template <>
int ArgsParser::get_from_variant<int>(AllowedArgTypes const &v) const {
  return mpark::get<int>(v);
}

template <>
float ArgsParser::get_from_variant<float>(AllowedArgTypes const &v) const {
  return mpark::get<float>(v);
}

template <>
bool ArgsParser::get_from_variant<bool>(AllowedArgTypes const &v) const {
  return mpark::get<bool>(v);
}

template <>
std::string
    ArgsParser::get_from_variant<std::string>(AllowedArgTypes const &v) const {
  return mpark::get<std::string>(v);
}

void ArgsParser::showDescriptions() const {
    for (auto const &kv : mDescriptions) {
      std::cerr  << kv.first << ": " << kv.second << std::endl;
    }
  }

std::ostream &operator<<(std::ostream &out, ArgsParser const &args) {
  args.showDescriptions();
  return out;
}

} // namespace FlexFlow
