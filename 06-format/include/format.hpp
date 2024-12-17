#include <string>
#include <vector>

template <typename T>
std::string toString(T value) {
  return std::to_string(value);
}

std::string toString(char value) { return std::string(1, value); }

template <>
std::string toString<const char*>(const char* value) {
  return std::string(value);
}

template <typename T>
void addString(std::vector<std::string>& strings, T arg) {
  strings.push_back(toString(arg));
}

template <typename T, typename... Args>
void addString(std::vector<std::string>& strings, T arg, Args... args) {
  addString(strings, arg);
  addString(strings, args...);
}

void handleString(std::string& str, const std::vector<std::string>& strings) {
  for (std::size_t i = 1; i <= strings.size(); ++i) {
    std::string placeholder = '{' + std::to_string(i) + '}';
    std::size_t pos = 0;
    while ((pos = str.find(placeholder, pos)) != std::string::npos) {
      str.replace(pos, placeholder.length(), strings[i - 1]);
      pos += strings[i - 1].length();
    }
  }
}

template <typename... Args>
std::string format(const std::string_view& str, Args... args) {
  std::vector<std::string> strings;
  addString(strings, args...);
  std::string result(str);
  handleString(result, strings);
  return result;
}

std::string format(const std::string_view& str) { return std::string(str); }
