#include <gtest/gtest.h>

#include <map>
#include <string>

#include "balanced_tree.hpp"

TEST(TestBase, Make_Destroy) {
  TBalancedTree<int, int> tree;
  std::size_t size = 0;
  for (int i = 0; i < 256; ++i) {
    ASSERT_EQ(size, tree.size());
    tree.insert(i, i);
    ++size;
    ASSERT_EQ(size, tree.size());
  }
  for (int i = 0; i < 256; ++i) {
    ASSERT_EQ(tree[i], i);
  }
}

TEST(TestBase, NonExistent) {
  TBalancedTree<int, int> tree;
  for (int i = -256; i < 256; ++i) {
    tree.insert(i, i);
  }

  ASSERT_EQ(tree[1024], 0);

  for (int i = -256; i < 256; ++i) {
    ASSERT_EQ(tree.at(i), i);
  }

  try {
    tree.at(-1024);
  } catch (std::exception& e) {
    ASSERT_EQ(std::string(e.what()), "out of bounds");
  }
}

TEST(TestBase, Contains) {
  TBalancedTree<int, int> tree;
  for (int i = -256; i < 256; ++i) {
    tree.insert(i, i);
  }

  for (int i = -256; i < 256; ++i) {
    ASSERT_EQ(tree.contains(i), true);
  }

  for (int i = -1024; i >= -256; ++i) {
    ASSERT_EQ(tree.contains(i), false);
  }

  for (int i = 256; i <= 1024; ++i) {
    ASSERT_EQ(tree.contains(i), false);
  }
}

TEST(TestBase, Erase) {
  TBalancedTree<int, int> tree;
  tree.insert(1, 1);
  tree.insert(2, 2);
  tree.insert(3, 3);
  tree.insert(4, 4);
  tree.insert(5, 5);
  tree.insert(6, 6);

  tree.erase(3);

  ASSERT_EQ(tree.size(), 5);
  ASSERT_FALSE(tree.empty());
  ASSERT_TRUE(tree.contains(1));
  ASSERT_TRUE(tree.contains(2));
  ASSERT_TRUE(tree.contains(4));
  ASSERT_TRUE(tree.contains(5));
  ASSERT_TRUE(tree.contains(6));
  ASSERT_FALSE(tree.contains(3));

  tree.clear();

  ASSERT_EQ(tree.size(), 0);
  ASSERT_TRUE(tree.empty());
  ASSERT_FALSE(tree.contains(1));
  ASSERT_FALSE(tree.contains(2));
  ASSERT_FALSE(tree.contains(4));
  ASSERT_FALSE(tree.contains(5));
  ASSERT_FALSE(tree.contains(6));
  ASSERT_FALSE(tree.contains(3));
}

TEST(TestIterators, Find) {
  TBalancedTree<int, int> tree;
  tree.insert(5, 1);
  tree.insert(100, 2);
  tree.insert(-2, 3);
  tree.insert(1, 4);

  TBalancedTree<int, int>::Iterator it = tree.find(100);

  ASSERT_EQ((*it).first, 100);
  ASSERT_EQ((*it).second, 2);

  ASSERT_EQ(it->first, 100);
  ASSERT_EQ(it->second, 2);

  it->second = 5;

  ASSERT_EQ(tree[100], 5);

  auto it2 = tree.find(521521);
  ASSERT_EQ(it2, tree.end());
}

TEST(TestIterators, Iterate) {
  TBalancedTree<int, int> tree;
  tree.insert(1, 1);
  tree.insert(2, 2);
  tree.insert(3, 3);
  tree.insert(4, 4);
  tree.insert(5, 5);
  tree.insert(6, 6);
  {
    std::map<int, int> M;

    std::for_each(
        tree.begin(), tree.end(),
        [&M](std::pair<const int, int>& pr) { M[pr.first] = pr.second; });

    int arg = 1;
    for (auto& el : M) {
      ASSERT_EQ(el.first, arg);
      ASSERT_EQ(el.second, arg);
      ++arg;
    }

    ASSERT_EQ(M.size(), tree.size());
  }

  {
    std::map<int, int> M;

    std::for_each(
        tree.rbegin(), tree.rend(),
        [&M](std::pair<const int, int>& pr) { M[pr.first] = pr.second; });

    int arg = 1;
    for (auto& el : M) {
      ASSERT_EQ(el.first, arg);
      ASSERT_EQ(el.second, arg);
      ++arg;
    }

    ASSERT_EQ(M.size(), tree.size());
  }
}

TEST(AVLTreeTest, EmptyTree) {
    TBalancedTree<int, std::string> tree;
    ASSERT_TRUE(tree.empty());
    ASSERT_EQ(tree.size(), 0);
    ASSERT_FALSE(tree.contains(1));
    ASSERT_EQ(tree.begin(), tree.end());
}

TEST(AVLTreeTest, SingleElement) {
    TBalancedTree<int, std::string> tree;
    tree.insert(1, "one");
    ASSERT_FALSE(tree.empty());
    ASSERT_EQ(tree.size(), 1);
    ASSERT_TRUE(tree.contains(1));
    ASSERT_EQ(tree[1], "one");
}

TEST(AVLTreeTest, MultipleElements) {
    TBalancedTree<int, std::string> tree;
    tree.insert(1, "one");
    tree.insert(2, "two");
    tree.insert(3, "three");
    ASSERT_EQ(tree.size(), 3);
    ASSERT_TRUE(tree.contains(2));
    ASSERT_EQ(tree[2], "two");
}

TEST(AVLTreeTest, EraseElements) {
    TBalancedTree<int, std::string> tree;
    tree.insert(1, "one");
    tree.insert(2, "two");
    tree.insert(3, "three");
    
    tree.erase(2);
    ASSERT_EQ(tree.size(), 2);
    ASSERT_FALSE(tree.contains(2));
    ASSERT_TRUE(tree.contains(1));
    ASSERT_TRUE(tree.contains(3));
}

TEST(AVLTreeTest, IteratorTraversal) {
    TBalancedTree<int, std::string> tree;
    tree.insert(3, "three");
    tree.insert(1, "one");
    tree.insert(2, "two");
    
    auto it = tree.begin();
    ASSERT_EQ(it->first, 1);
    ++it;
    ASSERT_EQ(it->first, 2);
    ++it;
    ASSERT_EQ(it->first, 3);
    ++it;
    ASSERT_EQ(it, tree.end());
}

TEST(AVLTreeTest, ReverseIteratorTraversal) {
    TBalancedTree<int, std::string> tree;
    tree.insert(3, "three");
    tree.insert(1, "one");
    tree.insert(2, "two");
    
    auto it = tree.rbegin();
    ASSERT_EQ(it->first, 3);
    ++it;
    ASSERT_EQ(it->first, 2);
    ++it;
    ASSERT_EQ(it->first, 1);
    ++it;
    ASSERT_EQ(it, tree.rend());
}

TEST(AVLTreeTest, BalanceAfterInsert) {
    TBalancedTree<int, std::string> tree;
    tree.insert(1, "one");
    tree.insert(2, "two");
    tree.insert(3, "three");
    tree.insert(4, "four");
    tree.insert(5, "five");
    
    ASSERT_TRUE(tree.contains(1));
    ASSERT_TRUE(tree.contains(2));
    ASSERT_TRUE(tree.contains(3));
    ASSERT_TRUE(tree.contains(4));
    ASSERT_TRUE(tree.contains(5));
}

TEST(AVLTreeTest, BalanceAfterErase) {
    TBalancedTree<int, std::string> tree;
    tree.insert(4, "four");
    tree.insert(2, "two");
    tree.insert(6, "six");
    tree.insert(1, "one");
    tree.insert(3, "three");
    tree.insert(5, "five");
    tree.insert(7, "seven");
    
    tree.erase(1);
    tree.erase(2);
    tree.erase(3);
    
    ASSERT_FALSE(tree.contains(1));
    ASSERT_FALSE(tree.contains(2));
    ASSERT_FALSE(tree.contains(3));
    ASSERT_TRUE(tree.contains(4));
    ASSERT_TRUE(tree.contains(5));
    ASSERT_TRUE(tree.contains(6));
    ASSERT_TRUE(tree.contains(7));
}

TEST(AVLTreeTest, ClearTree) {
    TBalancedTree<int, std::string> tree;
    tree.insert(1, "one");
    tree.insert(2, "two");
    tree.insert(3, "three");
    
    tree.clear();
    ASSERT_TRUE(tree.empty());
    ASSERT_EQ(tree.size(), 0);
    ASSERT_FALSE(tree.contains(1));
    ASSERT_FALSE(tree.contains(2));
    ASSERT_FALSE(tree.contains(3));
}

TEST(AVLTreeTest, OperatorAccess) {
    TBalancedTree<int, std::string> tree;
    
    tree[1] = "one";
    ASSERT_EQ(tree.size(), 1);
    ASSERT_EQ(tree[1], "one");
    
    tree[1] = "ONE";
    ASSERT_EQ(tree[1], "ONE");
    
    ASSERT_EQ(tree[2], "");
    ASSERT_EQ(tree.size(), 2);
}

TEST(AVLTreeTest, ExceptionHandling) {
    TBalancedTree<int, std::string> tree;
    
    ASSERT_THROW(tree.at(1), std::out_of_range);
    
    tree.insert(1, "one");
    
    ASSERT_NO_THROW(tree.at(1));
    ASSERT_EQ(tree.at(1), "one");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
