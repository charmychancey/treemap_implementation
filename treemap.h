#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <utility>

template <typename K, typename V>
class Treemap {
 public:
  // * Capacity
  // ret number of key-value mappings in map in O(1)
  size_t Size();
  // ret true if map is empty in O(1)
  bool Empty();

  // * Modifiers
  // Insert <key> in map in O(log N) avg
  void Insert(const K &key, const V &value);
  // Remove <key> from map in O(log N) avg
  void Remove(const K &key);

  // * Lookup
  // ret value corresponding to <key> in O(log N) avg
  const V& Get(const K &key);

  // ret greatest key less than or equal to <key> in O(log N) avg
  const K& FloorKey(const K &key);
  // ret least key greater than or equal to <key> in O(log N) avg
  const K& CeilKey(const K &key);

  // ret if <key> is found in map in O(log N) avg
  bool ContainsKey(const K& key);
  // ret if <value> is found in map in O(N)
  bool ContainsValue(const V& value);

  // ret max key in map in O(log N) avg
  const K& MaxKey();
  // ret min key in map in O(log N) avg
  const K& MinKey();

 private:
  struct Node {
    K key;
    V value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };

  // Private member variables
  std::unique_ptr<Node> root;
  size_t size = 0;
  bool has_value;

  // helper method; ret node w/ lowest key value
  Node* Min(Node *n) {
    if (n->left)
      return Min(n->left.get());
    else
      return n;
  }
  // helper method; insert <key/value> pair at key-ordered location
  void Insert(std::unique_ptr<Node> &n, const K &key, const V &value) {
    if (!n)
      n = std::unique_ptr<Node>(new Node{key, value});
    else if (key < n->key)
      Insert(n->left, key, value);
    else if (key > n->key)
      Insert(n->right, key, value);
    else
      throw std::invalid_argument("Duplicate key");
  }
  // helper method; remove @key and adjust nodes to maintain key-order
  void Remove(std::unique_ptr<Node> &n, const K &key) {
    if (!n)  // key not found
      return;

    if (key < n->key) {
      Remove(n->left, key);
    } else if (key > n->key) {
      Remove(n->right, key);
    } else {  // found node
      if (n->left && n->right) {  // 2 children; root = lowest right's subnode
        n->key = Min(n->right.get())->key;
        n->value = Min(n->right.get())->value;
        Remove(n->right, n->key);
      } else {  // replace w/ only child or nullptr
        n = std::move((n->left) ? n->left : n->right);
      }
    }
  }
  // helper method; sets bool value if treemap contains <value>
  void ContainsValue(Node *n, const V &value) {
    if (!n)
      return;
    if (value == n->value) {
      has_value = true;
    }
    ContainsValue(n->left.get(), value);
    ContainsValue(n->right.get(), value);
  }

  Node* FloorNode(Node *n, const K &key) {
    if (n == nullptr)
      return n;
    if (key == n->key)
      return n;
    if (key < n->key)
      return FloorNode(n->left.get(), key);
    Node* right = FloorNode(n->right.get(), key);
    if (right == nullptr)
      return n;
    else
      return right;
  }

  Node* CeilNode(Node *n, const K &key) {
    if (n == nullptr)
      return n;
    if (key == n->key)
      return n;
    if (key > n->key)
      return CeilNode(n->right.get(), key);
    Node* left = CeilNode(n->left.get(), key);
    if (left == nullptr)
      return n;
    else
      return left;
  }
};

template<typename K, typename V>
size_t Treemap<K, V>::Size() {
  return size;
}

template<typename K, typename V>
bool Treemap<K, V>::Empty() {
  if (size == 0)
    return true;
  return false;
}

template<typename K, typename V>
void Treemap<K, V>::Insert(const K &key, const V &value) {
  Insert(root, key, value);  // uses private helper method
  size++;
}

template<typename K, typename V>
void Treemap<K, V>::Remove(const K &key) {
  if (Empty())
    throw std::invalid_argument("Empty tree");
  Remove(root, key);  // uses private helper method
  size--;
}

template<typename K, typename V>
const V& Treemap<K, V>::Get(const K &key) {
  if (Empty())
    throw std::invalid_argument("Empty tree");

  Node* n = root.get();

  while (n) {
    if (key == n->key)
      return n->value;
    else if (key < n->key)
      n = n->left.get();
    else
      n = n->right.get();
  }
  // throws invalid argument exception if key not found: invalid key
  throw std::invalid_argument("Invalid key");
}

template<typename K, typename V>
const K& Treemap<K, V>::FloorKey(const K &key) {
  if (key < MinKey())
    throw std::invalid_argument("Invalid key");
  Node* n = root.get();

  return FloorNode(n, key)->key;
}

template<typename K, typename V>
const K& Treemap<K, V>::CeilKey(const K &key) {
  if (key > MaxKey())
    throw std::invalid_argument("Invalid key");
  Node* n = root.get();

  return CeilNode(n, key)->key;
}

template<typename K, typename V>
bool Treemap<K, V>::ContainsKey(const K &key) {
  Node* n = root.get();

  while (n) {
    if (key == n->key)
      return true;
    else if (key < n->key)
      n = n->left.get();
    else
      n = n->right.get();
  }
  return false;
}

template<typename K, typename V>
bool Treemap<K, V>::ContainsValue(const V &value) {
  has_value = false;
  Node* n = root.get();

  ContainsValue(n, value);  // uses private helper method
  return has_value;
}

template<typename K, typename V>
const K& Treemap<K, V>::MaxKey() {
  Node* n = root.get();

  while (n->right)
    n = n->right.get();
  return n->key;
}

template<typename K, typename V>
const K& Treemap<K, V>::MinKey() {
  return Min(root.get())->key;  // uses private helper method
}

#endif  // TREEMAP_H_
