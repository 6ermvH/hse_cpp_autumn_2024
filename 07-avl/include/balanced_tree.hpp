#include <algorithm>
#include <cstdint>
#include <iterator>
#include <memory>

template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class TBalancedTree {
  struct Node {  // NODE
    using traits = std::allocator_traits<Allocator>;

    Allocator allocator;
    std::pair<const Key, T>* val_;
    int64_t height_;
    Node* left;
    Node* right;
    Node* parent;

    explicit Node(const Key& key, const T& val)
        : val_(allocator.allocate(1)),
          height_(1),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {
      traits::construct(allocator, val_, key, val);
    }

    ~Node() {
      traits::destroy(allocator, val_);
      allocator.deallocate(val_, 1);
    }
  };  // END Node

 public:
  class Iterator {
   private:
    Node* node_;
    const TBalancedTree& tree_;

    using reference = std::pair<const Key, T>&;
    using pointer = std::pair<const Key, T>*;
    using value_type = std::pair<const Key, T>;
    using iterator_category = std::forward_iterator_tag;

   public:
    Iterator(Node* node, TBalancedTree& tree) : node_(node), tree_(tree) {}
    Iterator(Node* node, const TBalancedTree& tree)
        : node_(node), tree_(tree) {}

    bool operator==(const Iterator& it) const { return node_ == it.node_; }

    bool operator!=(const Iterator& it) const { return node_ != it.node_; }

    reference operator*() const { return *node_->val_; }

    pointer operator->() const { return node_->val_; }

    Iterator operator++(int) {
      Iterator result = Iterator(node_, tree_);
      ++(*this);
      return result;
    }

    Iterator& operator++() {
      if (!node_) {
        return *this;
      }
      Node* found = tree_.findBiggest(node_);
      node_ = found;
      return *this;
    }

    Iterator operator--(int) {
      Iterator result = Iterator(node_, tree_);
      --(*this);
      return result;
    }

    Iterator& operator--() {
      if (!node_) {
        node_ = tree_.getMaxLeaf(root_);
        return *this;
      }
      Node* found = tree_.findLowest(node_);
      node_ = found;
      return *this;
    }
  };

  class ReverseIterator {
   private:
    Node* node_;
    const TBalancedTree& tree_;

    using reference = std::pair<const Key, T>&;
    using pointer = std::pair<const Key, T>*;
    using value_type = std::pair<const Key, T>;
    using iterator_category = std::forward_iterator_tag;

   public:
    ReverseIterator(Node* node, const TBalancedTree& tree)
        : node_(node), tree_(tree) {}
    ReverseIterator(Node* node, TBalancedTree& tree)
        : node_(node), tree_(tree) {}

    bool operator==(const ReverseIterator& it) const {
      return node_ == it.node_;
    }

    bool operator!=(const ReverseIterator& it) const {
      return node_ != it.node_;
    }

    reference operator*() const { return *node_->val_; }

    pointer operator->() const { return node_->val_; }

    ReverseIterator operator--(int) {
      ReverseIterator result = ReverseIterator(node_, tree_);
      --(*this);
      return result;
    }

    ReverseIterator& operator--() {
      if (!node_) {
        node_ = tree_.getMinLeaf(root_);
        return *this;
      }
      Node* found = tree_.findBiggest(node_);
      node_ = found;
      return *this;
    }

    ReverseIterator operator++(int) {
      ReverseIterator result = ReverseIterator(node_, tree_);
      ++(*this);
      return result;
    }

    ReverseIterator& operator++() {
      if (!node_) {
        return *this;
      }
      Node* found = tree_.findLowest(node_);
      node_ = found;
      return *this;
    }
  };

  Iterator begin() const { return Iterator(getMinLeaf(root_), *this); }

  Iterator end() const { return Iterator(nullptr, *this); }

  ReverseIterator rbegin() const {
    return ReverseIterator(getMaxLeaf(root_), *this);
  }

  ReverseIterator rend() const { return ReverseIterator(nullptr, *this); }

 public:
  TBalancedTree() : root_(nullptr), size_(0), cmp(Compare{}) {}

  ~TBalancedTree() = default;

  void insert(const Key& key, const T& value) {
    ++size_;
    root_ = insertToNode(root_, key, value);
  }

  void erase(const Key& key) {
    Node* node = findNode(root_, key);
    if (node) {
      --size_;
      root_ = eraseNode(root_, key);
    }
  }

  T& operator[](const Key& key) {
    Node* found = findNode(root_, key);
    if (!found) {
      insert(key, T());
      found = findNode(root_, key);
    }
    return found->val_->second;
  }

  T& at(const Key& key) const {
    Node* found = findNode(root_, key);
    if (!found) {
      throw std::out_of_range("out of bounds");
    }
    return found->val_->second;
  }

  Iterator find(const Key& key) const {
    if (!findNode(root_, key)) return Iterator(nullptr, *this);
    return Iterator(findNode(root_, key), *this);
  }

  bool contains(const Key& key) const {
    return findNode(root_, key) ? true : false;
  }

  std::size_t size() const { return size_; }

  bool empty() const { return (root_ == nullptr); }

  void clear() {
    while (root_) {
      --size_;
      root_ = eraseNode(root_, root_->val_->first);
    }
  }

 private:
  Node* getMinLeaf(Node* current) const {
    if (!current) {
      return nullptr;
    }
    while (current->left) {
      current = current->left;
    }
    return current;
  }

  Node* getMaxLeaf(Node* current) const {
    if (!current) {
      return nullptr;
    }
    while (current->right) {
      current = current->right;
    }
    return current;
  }

  Node* findBiggest(Node* node) const {
    if (!node) {
      return nullptr;
    }
    if (node->right) {
      return getMinLeaf(node->right);
    } else {
      Node* current = node;
      while (current->parent &&
             !cmp(current->val_->first, current->parent->val_->first)) {
        current = current->parent;
      }
      Node* result = current->parent;
      if (cmp(result->val_->first, node->val_->first)) {
        return nullptr;
      }
      return result;
    }
  }

  Node* findLowest(Node* node) const {
    if (!node) {
      return nullptr;
    }
    if (node->left) {
      return getMaxLeaf(node->left);
    } else {
      Node* current = node;
      while (current->parent &&
             cmp(current->val_->first, current->parent->val_->first)) {
        current = current->parent;
      }
      Node* result = current->parent;
      if (!result || !cmp(result->val_->first, node->val_->first)) {
        return nullptr;
      }
      return result;
    }
  }

  Node* insertToNode(Node* current, const Key& key, const T& value) {
    if (!current) {
      return new Node(key, value);
    }
    updateNode(current);
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

    if (cmp(key, current->val_->first)) {
      current->left = eraseNode(current->left, key);
      if (current->left) {
        current->left->parent = current;
      }
    } else if (cmp(current->val_->first, key)) {
      current->right = eraseNode(current->right, key);
      if (current->right) {
        current->right->parent = current;
      }
    } else {
      if (!current->left) {
        Node* temp = current->right;
        delete current;
        return temp;
      }
      if (!current->right) {
        Node* temp = current->left;
        delete current;
        return temp;
      }

      Node* successor = getMinLeaf(current->right);

      Node* newNode = new Node(successor->val_->first, successor->val_->second);
      newNode->left = current->left;
      if (current->left) {
        current->left->parent = newNode;
      }
      newNode->right = current->right;
      if (current->right) {
        current->right->parent = newNode;
      }

      delete current;

      newNode->right = eraseNode(newNode->right, successor->val_->first);
      if (newNode->right) {
        newNode->right->parent = newNode;
      }

      current = newNode;
    }

    return balance(current);
  }

  Node* findNode(Node* current, const Key& key) const {
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

  int64_t getBF(Node* current) const {
    if (!current) return 0;

    return (current->left ? current->left->height_ : 0) -
           (current->right ? current->right->height_ : 0);
  }

  void updateHeight(Node* current) {
    current->height_ =
        std::max((current->left ? current->left->height_ : 0),
                 (current->right ? current->right->height_ : 0)) +
        1;
  }

  void updateParent(Node* current) {
    if (current->left) {
      current->left->parent = current;
    }
    if (current->right) {
      current->right->parent = current;
    }
  }

  void updateNode(Node* current) {
    updateParent(current);
    updateHeight(current);
  }

  Node* rotateLeft(Node* current) {
    Node* newRoot = current->right;
    current->right = current->right->left;
    newRoot->left = current;
    updateNode(current);
    updateNode(newRoot);
    return newRoot;
  }

  Node* rotateRight(Node* current) {
    Node* newRoot = current->left;
    current->left = current->left->right;
    newRoot->right = current;
    updateNode(current);
    updateNode(newRoot);
    return newRoot;
  }

  Node* balance(Node* current) {
    updateNode(current);
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
