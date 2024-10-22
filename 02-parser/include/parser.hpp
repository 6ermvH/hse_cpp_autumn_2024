#pragma once

#include <functional>
#include <string>

using StringCallBack = std::function<void(const std::string&)>;
using DigitCallBack = std::function<void(uint64_t)>;
using CallBack = std::function<void()>;

class TokenParser {
 public:
  TokenParser() = default;

  void SetStartCallback(CallBack function);
  void SetEndCallback(CallBack function);
  void SetDigitTokenCallback(DigitCallBack function);
  void SetStringTokenCallback(StringCallBack function);

  void Parse(const std::string& string) const;

 private:
  CallBack startcallback_ = nullptr, endcallback_ = nullptr;
  StringCallBack stringcallback_ = nullptr;
  DigitCallBack digitcallback_ = nullptr;

  void HandlerToken(uint64_t num) const {
    if (digitcallback_) {
      digitcallback_(num);
    }
  }

  void HandlerToken(const std::string& str) const {
    if (stringcallback_) {
      stringcallback_(str);
    }
  }
};
