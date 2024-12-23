#include <cstdint>
#include <memory>

#include <iostream>

template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class TBalancedTree {

  struct Node { // NODE
    using traits = std::allocator_traits<Allocator>;

    Allocator allocator;
    std::pair<const Key, T>* val_;
    int64_t height;
    Node* left;
    Node* right;

    explicit Node(const Key& key, const T& val) : val_(allocator.allocate(1)), 
                                                  height(1), left(nullptr), right(nullptr) {
      traits::construct(allocator, val_, key, val);
    }

    ~Node() {
      traits::destroy(allocator, val_);
      allocator.deallocate(val_, 1);
    }
  }; // END Node
  
  struct Iterator {
    using iterator_category = std::input_iterator_tag;
    using value_type = std::pair<const Key, T>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::pair<const Key, T>*;
    using reference = std::pair<const Key, T>&;

    Node* current;
    Node* root;

    Iterator() : current(nullptr), root(nullptr) {}
    Iterator(Node* node, Node* tree_root) : current(node), root(tree_root) {
      if (current == nullptr && root != nullptr) {
        // Find the leftmost node to start iteration
        current = root;
        while (current->left != nullptr) {
          current = current->left;
        }
      }
    }

    reference operator*() const { return *(current->val_); }
    pointer operator->() { return current->val_; }

    Iterator& operator++() {
      if (current == nullptr) return *this;
      
      // If we have right subtree, go to the leftmost node of right subtree
      if (current->right != nullptr) {
        current = current->right;
        while (current->left != nullptr) {
          current = current->left;
        }
      } else {
        // Find the first ancestor where current node is in left subtree
        Node* parent = root;
        Node* successor = nullptr;
        while (parent != nullptr) {
          if (parent->val_->first > current->val_->first) {
            successor = parent;
            parent = parent->left;
          } else if (parent->val_->first < current->val_->first) {
            parent = parent->right;
          } else {
            break;
          }
        }
        current = successor;
      }
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) { return a.current == b.current; }
    friend bool operator!=(const Iterator& a, const Iterator& b) { return a.current != b.current; }
  };

public:

  TBalancedTree() : root_(nullptr), cmp(Compare{}) {}

  ~TBalancedTree() = default;

  void insert(const Key& key, const T& value) {
    ++size_;
    root_ = insertToNode(root_, key, value);
  }

  void erase(const Key& key) {
    root_ = eraseNode(root_, key);
  }

  T& operator[] (const Key& key) {
    Node* found = findNode(root_, key);
    if (!found) {
      insert(key, T());
      found = findNode(root_, key);
    }
    return found->val_->second;
  }

  T& at(const Key& key) {
    Node* found = findNode(root_, key);
    if (!found) {
      throw std::out_of_range("out of bounds");
    }
    return found->val_->second;
  }

  bool contains(const Key& key) const {
    return findNode(root_, key) ? true : false;
  }

  Iterator begin() {
    return Iterator(root_, root_);
  }

  Iterator end() {
    return Iterator(nullptr, root_);
  }

  std::size_t size() const {
    return size_;
  }

private:

  Node* getMinLeaf(Node* current) {
    while (!current->left) {
      current = current->left;
    }
    return current;
  }

  Node* insertToNode(Node* current, const Key& key, const T& value) {
    if (!current) {
      return new Node(key, value);
    }
    updateHeight(current);
    if (cmp(key, current->val_->first)) {
      current->left = insertToNode(current->left, key, value);
    } else {
      current->right = insertToNode(current->right, key, value);
    }
    return balance(current);
  }

  Node* eraseNode(Node* current, const Key& key) {
    if (!current) {
      return nullptr;
    }
    updateHeight(current);
    if (current->val_->first == key) {
      --size_;
      Node* right = current->right;
      Node* left = current->left;
      delete current;
      Node* min = getMinLeaf(right);
      min->left = left;
      balance(min);
      current = right;
    }
    if (cmp(key, current->val_->first)) {
      current->left = findNode(current->left, key);
    } else {
      current->right = findNode(current->right, key);
    }
    balance(current);
    return current;
  }

  Node* findNode(Node* current, const Key& key) {
    if (!current) {
      return nullptr;
    }
    if (current->val_->first == key) {
      return current;
    }
    if (cmp(key, current->val_->first)) {
      return findNode(current->left, key);
    } else {
      return findNode(current->right, key);
    }
  }

  int64_t getBF(Node* current) {
    if (!current) 
      return 0;

    return (current->left ? current->left->height : 0) -
           (current->right ? current->right->height : 0);
  }

  void updateHeight(Node* current) {
    current->height = std::max( (current->left ? current->left->height : 0),
                           (current->right ? current->right->height : 0) ) + 1;
  }

  Node* rotateLeft(Node* current) {
    Node* newRoot = current->right;
    current->right = current->right->left;
    newRoot->left = current;
    updateHeight(current);
    updateHeight(newRoot);
    return newRoot;
  }

  Node* rotateRight(Node* current) {
    Node* newRoot = current->left;
    current->left = current->left->right;
    newRoot->right = current;
    updateHeight(current);
    updateHeight(newRoot);
    return newRoot;
  }

  Node* balance(Node* current) {
    updateHeight(current);
    int64_t bf = getBF(current);
    if (bf > 1) {
      if (getBF(current->left) < 0) {
        current->left = rotateLeft(current->left);
      }
      return rotateRight(current);
    }
    if (bf < -1) {
      if (getBF(current->right) > 0) {
        current->right = rotateRight(current->right);
      }
      return rotateLeft(current);
    }
    return current;
  }

  Node* root_;
  std::size_t size_;
  Compare cmp;
};
