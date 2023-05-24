#ifndef SRC_CMD_ARGS_H_
#define SRC_CMD_ARGS_H_

#include <queue>

#include "arguments/argument.h"
#include "arguments/flag.h"

namespace hhullen {

using Str = std::string;

class CMDArgs {
  using Argument = hhullen::Argument;
  using Flag = hhullen::Flag;
  using StrStrPair = std::pair<Str, Str>;
  using Tokens = std::queue<Str>;

  using ArgumentsStruct = std::queue<Argument>;
  using ParsedArguments = std::map<Str, Str>;
  using ArgumentsListIterator = std::list<Argument>::const_iterator;

  using FlagsStruct = std::map<StrStrPair, Flag>;
  using FlagsStructElement = std::pair<StrStrPair, Flag>;
  using ParsedFlags = std::map<StrStrPair, std::list<Str>>;
  using ParsedFlagsElement = std::pair<StrStrPair, std::list<Str>>;
  using FlagsStructIterator = FlagsStruct::iterator;
  using ParsedFlagsIterator = ParsedFlags::iterator;

 public:
  CMDArgs();
  ~CMDArgs();

  void AddArguments(const std::initializer_list<Argument>& args);
  void AddFlags(const std::initializer_list<Flag>& flags);
  Str GetArgument(const Str& name);
  std::list<Str> GetFlagValues(const Str& name);
  void Read(int argc, const char* argv[]);

 private:
  ParsedArguments positional_;
  ParsedFlags optional_;

  ArgumentsStruct arguments_;
  FlagsStruct flags_;

  Tokens tokens_;
  Str search_token_;

  std::function<bool(const FlagsStructElement&)> class_flag_search_func;
  std::function<bool(const ParsedFlagsElement&)> parsed_flag_search_func;

  void CopyToThis(int argc, const char* argv[]);
  bool IsArgExists(const Str& name);
  Flag GetFlagFromToken(const Str& token);
  void ReadFlag(Flag& flag);
  void CheckFlagValuesAbsence(Flag& flag);
  void SetParsedValueForFlag(const Str& value, Flag& flag);
  void CheckFlagUniqueness(const Str& name_long, const Str& name_short);
  void ReadArgumentFromToken(Argument& argument, const Str& token);
  void CheckRemainsArguments();
  void ThrowNoSpecidiedName(const Str& name);
};

CMDArgs::CMDArgs() {
  class_flag_search_func = [this](const FlagsStructElement& element) {
    return element.first.first == search_token_ ||
           element.first.second == search_token_;
  };
  parsed_flag_search_func = [this](const ParsedFlagsElement& element) {
    return element.first.first == search_token_ ||
           element.first.second == search_token_;
  };
}
CMDArgs::~CMDArgs() {}

void CMDArgs::AddArguments(const std::initializer_list<Argument>& args) {
  for (Argument arg : args) {
    arguments_.push(arg);
  }
}

void CMDArgs::AddFlags(const std::initializer_list<Flag>& flags) {
  for (Flag flag : flags) {
    Str name_long = Str("--") + flag.GetLongName();
    Str name_short = Str("-") + flag.GetShortName();
    flags_.insert({{name_long, name_short}, flag});
  }
}

Str CMDArgs::GetArgument(const Str& name) {
  if (!IsArgExists(name)) {
    ThrowNoSpecidiedName(name);
  }
  return positional_[name];
}

std::list<Str> CMDArgs::GetFlagValues(const Str& name) {
  search_token_ = name;
  ParsedFlagsIterator iter =
      std::find_if(optional_.begin(), optional_.end(), parsed_flag_search_func);
  search_token_.clear();
  if (iter == optional_.end()) {
    ThrowNoSpecidiedName(name);
  }
  return (*iter).second;
}

void CMDArgs::Read(int argc, const char* argv[]) {
  CopyToThis(argc, argv);
  for (; !tokens_.empty();) {
    Str token = tokens_.front();
    if (Argument::IsArgument(token) && !arguments_.empty()) {
      Argument argument = arguments_.front();
      ReadArgumentFromToken(argument, token);
      arguments_.pop();
      tokens_.pop();
    } else if (Flag::IsFlag(token)) {
      Flag flag = GetFlagFromToken(token);
      ReadFlag(flag);
    } else {
      throw std::invalid_argument("Unknown argument \"" + token +
                                  "\" specified.");
    }
  }
  CheckRemainsArguments();
}

void CMDArgs::CopyToThis(int argc, const char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    tokens_.push(argv[i]);
  }
}

bool CMDArgs::IsArgExists(const Str& name) {
  return positional_.find(name) != positional_.end();
}

Flag CMDArgs::GetFlagFromToken(const Str& token) {
  search_token_ = token;
  FlagsStructIterator iter =
      std::find_if(flags_.begin(), flags_.end(), class_flag_search_func);
  search_token_.clear();

  if (iter == flags_.end()) {
    throw std::invalid_argument("Unknown flag \"" + token + "\"specified.");
  }
  return (*iter).second;
}

void CMDArgs::ReadFlag(Flag& flag) {
  const std::list<Argument>& arguments = flag.GetArguments();
  ArgumentsListIterator iter = arguments.begin();
  tokens_.pop();

  CheckFlagValuesAbsence(flag);
  for (; !tokens_.empty() && iter != arguments.end(); tokens_.pop()) {
    Argument argument = *iter;
    Str token = tokens_.front();
    argument.ReadArgument(token);

    Str value = argument.GetValue();
    SetParsedValueForFlag(value, flag);
    ++iter;
  }

  if (iter != arguments.end()) {
    Argument argument = *iter;
    throw std::invalid_argument("Value \"" + argument.GetName() +
                                "\" of flag [--" + flag.GetLongName() + " -" +
                                flag.GetShortName() + "] was not specified.");
  }
}

void CMDArgs::CheckFlagValuesAbsence(Flag& flag) {
  const std::list<Argument>& arguments = flag.GetArguments();
  if (arguments.empty()) {
    SetParsedValueForFlag("true", flag);
  }
}

void CMDArgs::SetParsedValueForFlag(const Str& value, Flag& flag) {
  Str name_long = Str("--") + flag.GetLongName();
  Str name_short = Str("-") + flag.GetShortName();
  CheckFlagUniqueness(name_long, name_short);
  optional_[{name_long, name_short}].push_back(value);
}

void CMDArgs::CheckFlagUniqueness(const Str& name_long, const Str& name_short) {
  search_token_ = name_long;
  ParsedFlagsIterator iter_name_long =
      std::find_if(optional_.begin(), optional_.end(), parsed_flag_search_func);
  search_token_ = name_short;
  ParsedFlagsIterator iter_name_short =
      std::find_if(optional_.begin(), optional_.end(), parsed_flag_search_func);
  search_token_.clear();
  if (iter_name_long != optional_.end() || iter_name_short != optional_.end()) {
    throw std::invalid_argument(
        "CMD structure error: Double flag definition [" + name_long + " " +
        name_short + "].");
  }
}

void CMDArgs::ReadArgumentFromToken(Argument& argument, const Str& token) {
  argument.ReadArgument(token);
  Str value = argument.GetValue();
  Str name = argument.GetName();
  if (positional_.find(name) != positional_.end()) {
    throw std::invalid_argument(
        "CMD structure error: Double argument definition \"" + name + "\".");
  }
  positional_[name] = value;
}

void CMDArgs::CheckRemainsArguments() {
  if (!arguments_.empty()) {
    Str name = arguments_.front().GetName();
    ThrowNoSpecidiedName(name);
  }
}

void CMDArgs::ThrowNoSpecidiedName(const Str& name) {
  throw std::invalid_argument("\"" + name + "\" was not specified.");
}

}  // namespace hhullen

#endif  //  SRC_CMD_ARGS_H_
