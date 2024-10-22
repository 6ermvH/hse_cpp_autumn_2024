#include "parser.hpp"

#include <algorithm>
#include <sstream>
#include <string>

void TokenParser::SetStartCallback(CallBack function) {
  startcallback_ = function;
}

void TokenParser::SetEndCallback(CallBack function) { endcallback_ = function; }

void TokenParser::SetDigitTokenCallback(DigitCallBack function) {
  digitcallback_ = function;
}

void TokenParser::SetStringTokenCallback(StringCallBack function) {
  stringcallback_ = function;
}

void TokenParser::Parse(const std::string& string) const {
  if (startcallback_) {
    startcallback_();
  }
  std::stringstream ss(string);
  std::string token;
  while (ss >> token) {
    if (std::all_of(token.begin(), token.end(), [](unsigned char symbol) {
          return std::isdigit(symbol) != 0;
        })) {
      try {
        uint64_t num = std::stoull(token);
        HandlerToken(num);
      } catch (const std::exception&) {
        HandlerToken(token);
      }
    } else {
      HandlerToken(token);
    }
  }
  if (endcallback_) {
    endcallback_();
  }
}
