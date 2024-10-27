#include <iostream>

#include "parser.hpp"

int main() {
  TokenParser parser;
  std::vector<uint64_t> nums;
  std::vector<std::string> strings;

  parser.SetStartCallback([]() { std::cout << "Start parsing\n"; });

  parser.SetEndCallback([]() { std::cout << "End parsing\n"; });

  parser.SetDigitTokenCallback([&nums](uint64_t num) {
    std::cout << "Digit token: " << num << '\n';
    nums.push_back(num);
  });

  parser.SetStringTokenCallback([&strings](const std::string& str) {
    std::cout << "String token: " << str << '\n';
    strings.push_back(str);
  });

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "exit") {
      break;
    }
    parser.Parse(line);
  }

  std::cout << "NUMS: ";
  for (auto& el : nums) {
    std::cout << el << ' ';
  }
  std::cout << '\n';

  std::cout << "STRINGS: ";
  for (auto& el : strings) {
    std::cout << el << ' ';
  }
  std::cout << '\n';
}
