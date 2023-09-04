// #include "utils/parse.h"
// #include "utils/containers.h"

// namespace FlexFlow{

// std::string parseKey(std::string const &arg) {
//   if (arg.substr(0, 2) == "--") {
//     return arg.substr(2);
//   } else {
//     return arg;
//   }
// }


// void ArgsParser::parse_args(int argc, char **argv) {
//     for (int i = 1; i < argc; i += 2) {
//       std::string key = parseKey(argv[i]);
//       if (key == "help" || key == "h") {
//         showDescriptions();
//         exit(0);
//       }
//       mArgs[key] = argv[i + 1];
//     }
//   }

//   ArgumentReference ArgsParser::add_argument(std::string const &key,
//                     AllowedArgTypes const &value,
//                     std::string const &description) const {
//     mDefaultValues[parseKey(key)] = value;
//     mDescriptions[key] = description;
//     //AllowedArgTypes v = value;
//     return ArgumentReference{value, key};
// }

//   template <typename T>
//   T ArgsParser::get(ArgumentReference const &arg_ref) const {
//     std::string key = arg_ref.key; 
//     auto it = mArgs.find(key);
//     if (it != mArgs.end()) {
//       return convert<T>(it->second);
//     } else {
//       auto def_it = mDefaultValues.find(key);
//       if (def_it != mDefaultValues.end()) {
//         return get_from_variant<T>(def_it->second);
//       }
//     throw mk_runtime_error("Key not found: " + arg_ref.key);
//   }
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