// #include "parse.h"
// #include "containers.h"

// namespace FlexFlow{

// ArgsParser::parse_arg(int argc, char **argv) {
//     for (int i = 1; i < argc; i += 2) {
//       std::string key = parseKey(argv[i]);
//       if (key == "help" || key == "h") {
//         showDescriptions();
//         exit(0);
//       }
//       mArgs[key] = argv[i + 1];
//     }
//   }

//   template <typename T>
//   ArgumentReference<T> ArgsParser::add_argument(std::string const &key,
//                     AllowedArgTypes const &value,
//                     std::string const &description) {
//     mDefaultValues[parseKey(key)] = value;
//     mDescriptions[key] = description;
//     AllowedArgTypes v = value;
//     return ArgumentReference<T>(v,  description, key);
// }

//   template <typename T>
//   T ArgsParser::get(ArgumentReference<T> const &arg_ref) const {
//     if (contains(mArgs, arg_ref.default_key())) {
//       return convert<T>(it->second);
//     } else {
//       if (contains(mDefaultValues, arg_ref.default_key())) {
//         return get_from_variant<T>(arg_ref.default_value());
//       }
//     }
//     throw mk_runtime_error("Key not found: " + key);
//   }

//   void ArgsParser::showDescriptions() const {
//     for (auto const &kv : mDescriptions) {
//       std::cerr << kv.first << ": " << kv.second << std::endl;
//     }
//   }

//   template <>
// int ArgsParser::convert<int>(std::string const &s) const {
//   return std::stoi(s);
// }

// template <>
// float ArgsParser::convert<float>(std::string const &s) const {
//   return std::stof(s);
// }

// template <>
// bool ArgsParser::convert<bool>(std::string const &s) const {
//   return s == "true" || s == "1" || s == "yes";
// }

// template <>
// std::string ArgsParser::convert<std::string>(std::string const &s) const {
//   return s;
// }

// template <>
// int ArgsParser::get_from_variant<int>(AllowedArgTypes const &v) const {
//   return mpark::get<int>(v);
// }

// template <>
// float ArgsParser::get_from_variant<float>(AllowedArgTypes const &v) const {
//   return mpark::get<float>(v);
// }

// template <>
// bool ArgsParser::get_from_variant<bool>(AllowedArgTypes const &v) const {
//   return mpark::get<bool>(v);
// }

// template <>
// std::string
//     ArgsParser::get_from_variant<std::string>(AllowedArgTypes const &v) const {
//   return mpark::get<std::string>(v);
// }

// std::ostream &operator<<(std::ostream &out, ArgsParser const &args) {
//   args.showDescriptions();
//   return out;
// }

// } // namespace FlexFlow