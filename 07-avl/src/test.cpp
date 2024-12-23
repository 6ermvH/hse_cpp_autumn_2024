#include "balanced_tree.hpp"

#include <string>

int main() {

  TBalancedTree<int, int> tree;
  tree.insert(5, 0);
  tree.insert(3, 1);
  tree.insert(6, 2);
  tree.insert(7, 3);
  tree.insert(9, 4);

  std::cout << tree[4];
}
