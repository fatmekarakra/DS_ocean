#ifndef AVLTREE_H
#define AVLTREE_H

#include <memory>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>
template<typename KeyType, typename ValueType>
class KV {
    KeyType key;
    std::shared_ptr<ValueType> valuePtr;
public:
    // Constructors
    KV(const KeyType& k):key(k),valuePtr(nullptr){}
    KV(const KeyType& k, const ValueType& v) : key(k), valuePtr(std::make_shared<ValueType>(v)) {}
    KV(const KV& other) : key(other.key), valuePtr(other.valuePtr) {}
    KV() : key(KeyType()), valuePtr(std::make_shared<ValueType>(ValueType())) {}
    ~KV()=default;
    // Getters and Setters
    KeyType getKey() const { return key; }
    void setKey(const KV& otherKey) { key = otherKey; }
    ValueType& getValue() { return *valuePtr; }

    // Comparison
    bool operator==(const KV& other) const { return key == other.key; }
    bool operator>(const KV& other) const { return key > other.key; }
    bool operator<(const KV& other) const { return key < other.key; }
    bool operator>=(const KV& other) const { return key >= other.key; }
    bool operator<=(const KV& other) const { return key <= other.key; }

    // So it can be used in the tree

    template<typename K, typename V>
    friend bool areEqual(const KV<K, V>& k1, const KV<K, V>& k2);

    template<typename K, typename V>
    friend std::ostream& operator<<(std::ostream& os, const KV<K, V>& kv);
};

template<typename KeyType, typename ValueType>
bool areEqual(const KV<KeyType, ValueType>& k1, const KV<KeyType, ValueType>& k2){
    return k1.valuePtr->isEqual(*(k2.valuePtr));
}
template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const KV<K, V>& kv) {
    os << "(" << kv.key << ", " << *(kv.valuePtr) <<  ")";
    return os;
}

template<typename T>
class AVLTree {
//public:
private:
    class Node;

    std::shared_ptr<Node> root;
    int size;

    // Private helper functions
    static int getHeight(std::shared_ptr<Node> node);
    static int getBalanceFactor(std::shared_ptr<Node> node);
    static void updateHeight(std::shared_ptr<Node> node);
    static std::shared_ptr<Node> LLRotation(std::shared_ptr<Node> y);
    static std::shared_ptr<Node> RRRotation(std::shared_ptr<Node> x);
    static std::shared_ptr<Node> insertUtil(std::shared_ptr<Node> node, const T& value);
    static std::shared_ptr<Node> eraseUtil(std::shared_ptr<Node> node, const T& value,bool firstEntery);
    static std::shared_ptr<Node> minValueNode(std::shared_ptr<Node> node);
    static std::shared_ptr<Node> maxValueNode(std::shared_ptr<Node> node);
    static std::shared_ptr<Node> findNode(std::shared_ptr<Node> node, const T& value);

public:
    AVLTree() : root(nullptr), size(0) {}

    void insert(const T& value);
    void erase(const T& value);

    /* returns 'value' if:
    - 'value' is duplicated
    - 'value' wasn't found
    - there is less than 2 values in the tree */

    // find
    bool find(const T& value) const;
    T& get(const T& value);//must only be used if we are sure that the node exists in the tree!
    const T& getMaxValue() const;
    const T& getMinValue() const;
    int getSize() const;
    // printing the tree
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const AVLTree<U>& pair);
    template<typename U>
    friend void printTree(std::ostream& os, std::shared_ptr<typename AVLTree<U>::Node> node,int indent);
};

template<typename T>
class AVLTree<T>::Node {
public:
    std::shared_ptr<T> data;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    int height;

    Node(const std::shared_ptr<T>& valPtr) : data(valPtr), left(nullptr), right(nullptr), height(0) {}
};

























template<typename T>
void AVLTree<T>::insert(const T& value) {
    root = insertUtil(root, value);
    size++;
}
template<typename T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::insertUtil(std::shared_ptr<Node> node, const T& value) {
    // Perform standard insertion
    if (!node){
        return std::make_shared<Node>(std::make_shared<T>(value));
    }
    if (value < *(node->data)) {
        node->left = insertUtil(node->left, value);
    } 
    else if (value > *(node->data)) {
        node->right = insertUtil(node->right, value);
    } 
    else {
        return node;
    }

    // Update height of this ancestor node
    updateHeight(node);

    // Get the balance factor to check whether this node became unbalanced
    int balance = getBalanceFactor(node);
    int balanceL, balanceR;
    if (node->left){
        balanceL = getBalanceFactor(node->left);
    }
    if (node->right){
        balanceR = getBalanceFactor(node->right);
    }

    // Left Left Case
    if (balance == 2 && node->left && balanceL >= 0){
        return LLRotation(node);
    }
    // Right Right Case
    if (balance == -2 && node->right && balanceR <= 0){
        return RRRotation(node);
    }
    // Left Right Case
    if (balance == 2 && node->left && balanceL == -1) {
        node->left = RRRotation(node->left);
        return LLRotation(node);
    }
    // Right Left Case
    if (balance == -2 && node->right && balanceR == 1) {
        node->right = LLRotation(node->right);
        return RRRotation(node);
    }
    // Return the unchanged node pointer
    return node;
}
template<typename T>
void AVLTree<T>::erase(const T& value) {
    root = eraseUtil(root, value,true);
    size--;
}
template<typename T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::eraseUtil(std::shared_ptr<Node> node, const T& value,bool firstEntery) {
    if(firstEntery){
        if(findNode(node,value)==nullptr){
            throw std::runtime_error("value doesnt exist in the tree hence cant be deleted");
        }
    }
    if (!node){
        return node;
    }
    if (value < *(node->data)){
        node->left = eraseUtil(node->left, value,false);
    }
    else if (value > *(node->data)){
        node->right = eraseUtil(node->right, value,false);
    }
    else {
        if (!node->left) {
            std::shared_ptr<Node> temp = node->right;
            node = nullptr;
            return temp;
        } else if (!node->right) {
            std::shared_ptr<Node> temp = node->left;
            node = nullptr;
            return temp;
        }
        else{
            std::shared_ptr<Node> temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = eraseUtil(node->right, *(temp->data),false);
        }
    }

    if (!node){
        return node;
    }

    updateHeight(node);

    // Get the balance factor of this node (to check whether this node became unbalanced)
    int balance = getBalanceFactor(node);
    int balanceL, balanceR;
    if (node->left){
        balanceL = getBalanceFactor(node->left);
    }
    if (node->right){
        balanceR = getBalanceFactor(node->right);
    }
    // Left Left Case
    if (balance == 2 && node->left && balanceL >= 0){
        return LLRotation(node);
    }

    // Right Right Case
    if (balance == -2 && node->right && balanceR <= 0){
        return RRRotation(node);
    }
    // Left Right Case
    if (balance == 2 && node->left && balanceL == -1) {
        node->left = RRRotation(node->left);
        return LLRotation(node);
    }
    // Right Left Case
    if (balance == -2 && node->right && balanceR == 1) {
        node->right = LLRotation(node->right);
        return RRRotation(node);
    }
    return node;
}

template<typename T>
bool AVLTree<T>::find(const T& value) const {
    return (findNode(root, value) != nullptr);
}

template<typename T>
T& AVLTree<T>::get(const T& value) {
    if(findNode(root, value)==nullptr){
        throw std::runtime_error("cant find the value your trying to get");
    }
    auto node = this->findNode(root, value);
    return *(node->data);
}
template<typename T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::findNode(std::shared_ptr<Node> node, const T& value) {
    if (!node){
        return nullptr;
    }
    if (value == *(node->data)){
        return node;
    }
    if (value < *(node->data)){
        return findNode(node->left, value);
    }
    return findNode(node->right, value);
}
template<typename T>
const T&  AVLTree<T>::getMaxValue() const {
    if (!root){
        throw std::out_of_range("Tree is empty");
    }
    return *(maxValueNode(root)->data);
}
template<typename T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::maxValueNode(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> temp = node;

    // Loop down to find the rightmost leaf
    while (temp->right){
        temp = temp->right;
    }
    return temp;
}
template<typename T>
const T&  AVLTree<T>::getMinValue() const {
    if (!root){
        throw std::out_of_range("Tree is empty");
    }
    return *(minValueNode(root)->data);
}
template<typename T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::minValueNode(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> temp = node;

    // Loop down to find the leftmost leaf
    while (temp->left){
        temp = temp->left;
    }
    return temp;
}
template<typename T>
int AVLTree<T>::getSize() const {
    return size;
}

template<typename T>
int AVLTree<T>::getHeight(std::shared_ptr<Node> node) {
    if (!node){
        return -1;
    }
    return node->height;
}

template<typename T>
int AVLTree<T>::getBalanceFactor(std::shared_ptr<Node> node) {
    if (!node){
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

template<typename T>
void AVLTree<T>::updateHeight(std::shared_ptr<Node> node) {
    if (!node){
        return;
    }
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
}

template<typename T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::LLRotation(std::shared_ptr<Node> b) {
    std::shared_ptr<Node> a = b->left;
    std::shared_ptr<Node> T2 = a->right;

    // Perform rotation
    a->right = b;
    b->left = T2;

    // Update heights
    updateHeight(b);
    updateHeight(a);

    return a;
}

template<typename T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::RRRotation(std::shared_ptr<Node> a) {
    std::shared_ptr<Node> b = a->right;
    std::shared_ptr<Node> T2 = b->left;

    // Perform rotation
    b->left = a;
    a->right = T2;

    // Update heights
    updateHeight(a);
    updateHeight(b);

    return b;
}
// printing the tree
// Friend function definition
template<typename U>
void printTree(std::ostream& os, std::shared_ptr<typename AVLTree<U>::Node> node,int indent) {
    if (!node) {
        return;
    }

    printTree<U>(os, node->right, indent + 4);
    os << std::setw(indent) << " " << "- " << *(node->data) << " ["<< node->height <<"]" << std::endl;
    printTree<U>(os, node->left, indent + 4);
}
template<typename U>
std::ostream& operator<<(std::ostream& os, const AVLTree<U>& tree) {
    printTree<U>(os, tree.root, 0);
    return os;
}
#endif // AVLTREE_H