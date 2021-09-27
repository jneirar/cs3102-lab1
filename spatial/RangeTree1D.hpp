#pragma once

#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include "SpatialBase.h"

using namespace std;

namespace utec {
namespace spatial {

/**
 * RangeTree1D implementation
 */


template <class T> class RTree_Node;
template <class T> class RTree;

template <class T>
class RTree_Node {
    friend class RTree<T>;

private:
    T data;
    RTree_Node<T>* parent;
    RTree_Node<T>* left;
    RTree_Node<T>* right;
    int balanceFactor;
    bool leaf;

public:
    RTree_Node(T d = T(), RTree_Node<T>* p = nullptr, RTree_Node<T>* l = nullptr, RTree_Node<T>* r = nullptr);

};

template <class T>
RTree_Node<T>::RTree_Node(T d, RTree_Node<T>* p, RTree_Node<T>* l, RTree_Node<T>* r){
    data = d;
    parent = p;
    left = l;
    right = r;
    balanceFactor = 0;
    leaf = true;
}

template <class T>
class RTree {
private:
    RTree_Node<T>* root;
    int size;
    int setBalance(RTree_Node<T>* node);
    int getHeight(RTree_Node<T>* node) const;
    void rebalance(RTree_Node<T>* node);
    void rotate(RTree_Node<T>* parent);
    void LLR(RTree_Node<T>* parent, RTree_Node<T>* node, RTree_Node<T>* child);
    void LRR(RTree_Node<T>* parent, RTree_Node<T>* node, RTree_Node<T>* child);
    void RRR(RTree_Node<T>* parent, RTree_Node<T>* node, RTree_Node<T>* child);
    void RLR(RTree_Node<T>* parent, RTree_Node<T>* node, RTree_Node<T>* child);
    void printInOrder(RTree_Node<T>* node) const;
    void printPostOrder(RTree_Node<T>* node) const;
    void printPreOrder(RTree_Node<T>* node) const;
    void printLevelOrder(RTree_Node<T>* node) const;
    RTree_Node<T>* copy(RTree_Node<T>* rhs) const;
    void remove(RTree_Node<T>* node);

    void rangeSearch(T start, T end, std::vector<T> &result, RTree_Node<T>* node);

    RTree_Node<T>* findSplitNode(T min, T max){
        RTree_Node<T>* v = root;
        while(!v->leaf && (max <= v->data || min > v->data)){
            if(max <= v->data)
                v = v->left;
            else
                v = v->right;
        }
        return v;
    }
    void reportSubTree(T min, T max, std::vector<T> &result, RTree_Node<T>* node){
        if(node->leaf){
            if(min <= node->data && node->data <= max)
                result.push_back(node->data);
        }else{
            reportSubTree(min, max, result, node->left);
            reportSubTree(min, max, result, node->right);
        }

    }

    void printBT(const std::string& prefix, const RTree_Node<T>* node, bool isR)
    {
        if( node != nullptr )
        {
            std::cout << prefix;

            std::cout << (isR ? "├──" : "└──" );

            std::cout << node->data << std::endl;

            // enter the next tree level - left and right branch
            printBT( prefix + (isR ? "│   " : "    "), node->right, 1);
            printBT( prefix + (isR ? "│   " : "    "), node->left, 0);
        }
    }

public:
    RTree(): root(nullptr), size(0){}
    RTree(const RTree<T>& rhs): root(nullptr) { *this = rhs; }
    ~RTree();
    RTree<T>& operator =(const RTree<T>& rhs);

    bool contains(const T& data) const;
    bool isEmpty() const { return size == 0; }
    void clear();
    void insert(T data);
    RTree_Node<T>* search(const T& data) const;
    void remove(const T& data) { remove(search(data)); }
    void printInOrder() const;
    void printPostOrder() const;
    void printPreOrder() const;
    void printLevelOrder() const { printLevelOrder(root); }
    int getHeight() const { return getHeight(root); }
    int getSize() const { return size; }

    std::vector<T> rangeSearch(T start, T end){ 
      std::vector<T> result;
      rangeSearch(start, end, result, root);
      return result;
    }
    void printBT() {
        printBT("", root, false);    
    }
};

template <class T>
int RTree<T>::setBalance(RTree_Node<T>* node){
    if (node == nullptr) return 0;
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->balanceFactor = rightHeight - leftHeight;
    return node->balanceFactor;
}

template <class T>
int RTree<T>::getHeight(RTree_Node<T>* node) const{
    if (node == nullptr) return -1;
    return 1 + max(getHeight(node->left), getHeight(node->right));
}

template <class T>
void RTree<T>::rebalance(RTree_Node<T>* node){
    while (node != nullptr){
        setBalance(node);
        if (node->balanceFactor >= 2 || node->balanceFactor <= -2)
            rotate(node);
        node = node->parent;
    }
}

template <class T>
void RTree<T>::rotate(RTree_Node<T>* node){
    RTree_Node<T>* child;
    if (node->balanceFactor < -1){
        child = node->left;
        setBalance(child);
        if (child->balanceFactor == 1)
            LRR(node, child, child->right);
        else
            LLR(node, child, child->left);
    } else {
        child = node->right;
        setBalance(child);
        if (child->balanceFactor == -1)
            RLR(node, child, child->left);
        else
            RRR(node, child, child->right);
    }
}

template <class T>
void RTree<T>::LLR(RTree_Node<T>* parent, RTree_Node<T>* node, RTree_Node<T>* child){
    RTree_Node<T>* grandParent = parent->parent;
    node->parent = grandParent;
    RTree_Node<T>* nodeRight = node->right;
    if (nodeRight != nullptr) nodeRight->parent = parent;
    node->right = parent;
    parent->parent = node;
    parent->left = nodeRight;

    if(parent->left == nullptr && parent->right == nullptr){
        parent->leaf = true;
    } else{
        parent->leaf = false;
    }
    if(node->left == nullptr && node->right == nullptr){
        node->leaf = true;
    } else{
        node->leaf = false;
    }
    if(child->left == nullptr && child->right == nullptr){
        child->leaf = true;
    } else{
        child->leaf = false;
    }

    if (grandParent == nullptr)
        root = node;
    else if (grandParent->left == parent)
        grandParent->left = node;
    else
        grandParent->right = node;
}

template <class T>
void RTree<T>::LRR(RTree_Node<T>* parent, RTree_Node<T>* node, RTree_Node<T>* child){
    RRR(node, child, child->right);
    LLR(parent, child, node);
}

template <class T>
void RTree<T>::RRR(RTree_Node<T>* parent, RTree_Node<T>* node, RTree_Node<T>* child){
    RTree_Node<T>* grandParent = parent->parent;
    node->parent = grandParent;
    RTree_Node<T>* nodeLeft = node->left;
    if (nodeLeft != nullptr) nodeLeft->parent = parent;
    node->left = parent;
    parent->parent = node;
    parent->right = nodeLeft;

    if(node->left == nullptr && node->right == nullptr){
        node->leaf = true;
    } else{
        node->leaf = false;
    }

    if (grandParent == nullptr)
        root = node;
    else if (grandParent->left == parent)
        grandParent->left = node;
    else
        grandParent->right = node;
}

template <class T>
void RTree<T>::RLR(RTree_Node<T>* parent, RTree_Node<T>* node, RTree_Node<T>* child){
    LLR(node, child, child->left);
    RRR(parent, child, node);
}

template <class T>
void RTree<T>::printInOrder(RTree_Node<T>* node) const {
    if (node == nullptr) return;
    printInOrder(node->left);
    cout << node->data << ' ';
    printInOrder(node->right);
}

template <class T>
void RTree<T>::printPostOrder(RTree_Node<T>* node) const {
    if (node == nullptr) return;
    printPostOrder(node->left);
    printPostOrder(node->right);
    cout << node->data << ' ';
}

template <class T>
void RTree<T>::printPreOrder(RTree_Node<T>* node) const {
    if (node == nullptr) return;
    cout << node->data << ' ';
    printPreOrder(node->left);
    printPreOrder(node->right);
}

template <class T>
void RTree<T>::printLevelOrder(RTree_Node<T>* node) const {
    if (node == nullptr) return;
    list<RTree_Node<T>*> nodeQueue;
    list<int> levelQueue;
    nodeQueue.push_back(node);
    levelQueue.push_back(0);
    while (!nodeQueue.empty()){
        RTree_Node<T>* currRTree_Node = nodeQueue.front();
        int currLevel = levelQueue.front();
        nodeQueue.pop_front();
        levelQueue.pop_front();
        if (currRTree_Node->left != nullptr){
            nodeQueue.push_back(currRTree_Node->left);
            levelQueue.push_back(currLevel + 1);
        }
        if (currRTree_Node->right != nullptr){
            nodeQueue.push_back(currRTree_Node->right);
            levelQueue.push_back(currLevel + 1);
        }
        for (int i = 0; i < currLevel; i++)
            cout << '\t';
        cout << currRTree_Node->data << endl;
    }
}

template <class T>
RTree_Node<T>* RTree<T>::copy(RTree_Node<T>* rhs) const {
    if (rhs == nullptr) return nullptr;
    RTree_Node<T>* ret = new RTree_Node<T>(rhs->data, nullptr, copy(rhs->left), copy(rhs->right));
    if (ret->left != nullptr)
        ret->left->parent = ret;
    if (ret->right != nullptr)
        ret->right->parent = ret;
    return ret;
}

template <class T>
void RTree<T>::remove(RTree_Node<T>* node){
    if (node == nullptr) return;
    if (root == nullptr) return;

    RTree_Node<T>* parent = node->parent;

    if (node->left == nullptr && node->right == nullptr){
        if (parent == nullptr)
            root = nullptr;
        else if (parent->left == node)
            parent->left = nullptr;
        else
            parent->right = nullptr;
        delete node;
        rebalance(parent);
        size--;
    }

    else if (node->right == nullptr){
        if (parent == nullptr){
            root = node->left;
            root->parent = nullptr;
        }
        else if (parent->left == node){
            parent->left = node->left;
            node->left->parent = parent;
        }
        else {
            parent->right = node->left;
            node->left->parent = parent;
        }
        delete node;
        rebalance(parent);
        size--;
    }

    else if (node->left == nullptr){
        if (parent == nullptr){
            root = node->right;
            root->parent = nullptr;
        }
        else if (parent->left == node){
            parent->left = node->right;
            node->right->parent = parent;
        }
        else {
            parent->right = node->right;
            node->right->parent = parent;
        }
        delete node;
        rebalance(parent);
        size--;
    }

    else {
        RTree_Node<T>* temp = node->right;
        while (temp->left != nullptr)
            temp = temp->left;
        node->data = temp->data;
        remove(temp);
    }
}

template <class T>
RTree<T>::~RTree(){
    clear();
}

template <class T>
RTree<T>& RTree<T>::operator =(const RTree<T>& rhs){
    if (this == &rhs) return *this;
    clear();
    root = copy(rhs.root);
    size = rhs.size;
    return *this;
}

template <class T>
bool RTree<T>::contains(const T& data) const {
    if (search(data) == nullptr) return false;
    return true;
}

template <class T>
void RTree<T>::clear(){
    while (root != nullptr)
        remove(root);
}

template <class T>
void RTree<T>::insert(T data){
    if (root == nullptr){
        root = new RTree_Node<T>(data);
        return;
    }
    RTree_Node<T>* curr = root;
    RTree_Node<T>* prev = curr;
    while (curr != nullptr){
        prev = curr;
        if (data <= curr->data)
            curr = curr->left;
        else 
            curr = curr->right;
    }
    size++;
    RTree_Node<T>* newRTree_Node = new RTree_Node<T>(data, prev);
    if (data <= prev->data){
        prev->leaf = false;
        prev->left = newRTree_Node;
        RTree_Node<T>* newRTree_Node2 = new RTree_Node<T>(data, newRTree_Node);
        newRTree_Node->left = newRTree_Node2;
    }else{
        prev->leaf = false;
        prev->right = newRTree_Node;
        RTree_Node<T>* newRTree_Node2 = new RTree_Node<T>(prev->data, prev);
        prev->left = newRTree_Node2;
    }
    rebalance(newRTree_Node);
}

template <class T>
RTree_Node<T>* RTree<T>::search(const T& data) const {
    RTree_Node<T>* temp = root;
    while (temp != nullptr){
        if (temp->data == data) return temp;
        if (temp->data < data) temp = temp->right;
        else temp = temp->left;
    }
    return nullptr;
}

template <class T>
void RTree<T>::printInOrder() const {
    printInOrder(root);
    cout << endl;
}

template <class T>
void RTree<T>::printPostOrder() const {
    printPostOrder(root);
    cout << endl;
}

template <class T>
void RTree<T>::printPreOrder() const {
    printPreOrder(root);
    cout << endl;
}

template <class T>
void RTree<T>::rangeSearch(T start, T end, std::vector<T> &result, RTree_Node<T>* node){
  if(node == nullptr) return;
  RTree_Node<T>* vsplit = findSplitNode(start, end);
  
  if(vsplit->leaf){
    if(start <= vsplit->data && vsplit->data <= end)    result.push_back(vsplit->data);
  }else{
    RTree_Node<T>* vleft = vsplit->left;
    while(!vleft->leaf){
        if(start <= vleft->data){
            reportSubTree(start, end, result, vleft->right);
            vleft = vleft->left;
        }else
            vleft = vleft->right;
        if(vleft->leaf){
            if(start <= vleft->data && vleft->data <= end)  result.push_back(vleft->data);
        }
    }
    RTree_Node<T>* vright = vsplit->right;
    while(!vright->leaf){
        if(vright->data <= end){
            reportSubTree(start, end, result, vright->left);
            vright = vright->right;
        }else
            vright = vright->left;
        if(vright->leaf){
            if(start <= vright->data && vright->data <= end)  result.push_back(vright->data);
        }
    }

  }

}

template <typename Point>
class RangeTree1D : public SpatialBase<Point> {
 private:
  RTree<Point> rangeTree_point;
 public:
  RangeTree1D(){};
  void insert(const Point& new_point) override {rangeTree_point.insert(new_point);}

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override { return Point({0}); }
  std::vector<Point> range(const Point& min, const Point& max) override {
    std::vector<Point> result = rangeTree_point.rangeSearch(min, max);
    return result;
  };
};

}  // namespace spatial
}  // namespace utec
