//
// Created by 鸢一折纸 on 2021/5/30.
//

#pragma once

#include "Vector.h"


template<typename Key_t, typename Value_t>
class BinaryTree {

private:

    struct TreeNode {
        Key_t key;
        Value_t value;
        TreeNode *father = nullptr;
        TreeNode *left = nullptr, *right = nullptr;
    };


    TreeNode *findFrontNode(TreeNode *);
    TreeNode *findBackNode(TreeNode *);
    void deleteNode(TreeNode *child, TreeNode *father);
    void mid(TreeNode* node) {
        if (node == nullptr)
            return;

        mid(node->left);
        std::cout << node->key << " ";
        mid(node->right);
    }

    TreeNode *head = nullptr;
    size_t size_ = 0;

public:
    BinaryTree() = default;
    void insert(Key_t, Value_t);
    void erase(key_t);
    void midd() { mid(head); std::cout << std::endl; }
};


template<typename Key_t, typename Value_t>
typename BinaryTree<Key_t, Value_t>::TreeNode *BinaryTree<Key_t, Value_t>::findFrontNode(BinaryTree::TreeNode *node) {
    TreeNode *root = node;
    TreeNode *tempPtr = nullptr;

    Vector<TreeNode *>stack;
    while (true) {
        while (node != nullptr) {
            stack.push_back(node);
            node = node->left;
        }

        node = *(stack.end() - 1);
        if (node == root)
            return tempPtr;
        tempPtr = node;
        node = node->right;
        stack.pop_back();
    }
}

template<typename Key_t, typename Value_t>
typename BinaryTree<Key_t, Value_t>::TreeNode *BinaryTree<Key_t, Value_t>::findBackNode(BinaryTree::TreeNode *node) {
    TreeNode *root = node;
    TreeNode *tempPtr = nullptr;

    Vector<TreeNode *>stack;
    while (true) {
        while (node != nullptr) {
            stack.push_back(node);
            node = node->left;
        }

        node = *(stack.end() - 1);
        if (tempPtr == root)
            return node;
        tempPtr = node;
        node = node->right;
        stack.pop_back();
    }
}


template<typename Key_t, typename Value_t>
void BinaryTree<Key_t, Value_t>::deleteNode(BinaryTree::TreeNode *child, BinaryTree::TreeNode *father) {

    if (father == nullptr) {
        delete child;
        return;
    }

    if (father->left == child) {
        if (child->left != nullptr) {
            father->left = child->left;
            child->left->father = father;
        } else if (child->right != nullptr) {
            father->left = child->right;
            child->right->father = father;
        } else
            father->left = nullptr;
    } else {
        if (child->left != nullptr) {
            father->right = child->left;
            child->left->father = father;
        } else if (child->right != nullptr) {
            father->right = child->right;
            child->right->father = father;
        } else
            father->right = nullptr;
    }
    delete child;
}

template<typename Key_t, typename Value_t>
void BinaryTree<Key_t, Value_t>::insert(Key_t key, Value_t value) {
    TreeNode *node = new TreeNode;
    node->key = key;
    node->value = value;
    size_++;

    if (head == nullptr) {
        head = node;
        return;
    }

    TreeNode* tempPtr = head;

    while (tempPtr) {
        if (tempPtr->key > key) {
            if (tempPtr->left == nullptr) {
                tempPtr->left = node;
                node->father = tempPtr;
                return;
            } else
                tempPtr = tempPtr->left;
        } else if (tempPtr->key < key) {
            if (tempPtr->right == nullptr) {
                tempPtr->right = node;
                node->father = tempPtr;
                return;
            } else
                tempPtr = tempPtr->right;
        } else {
            tempPtr->value = value;
            delete node;
            size_--;
            return;
        }
    }
}

template<typename Key_t, typename Value_t>
void BinaryTree<Key_t, Value_t>::erase(key_t key) {

    TreeNode *tempPtr = head;

    while (tempPtr) {
        if (tempPtr->key < key) {
            tempPtr = tempPtr->right;
        } else if (tempPtr->key > key) {
            tempPtr = tempPtr->left;
        } else {
            if (tempPtr->left == nullptr && tempPtr->right == nullptr) {
                deleteNode(tempPtr, tempPtr->father);
                if (tempPtr == head) {
                    head = nullptr;
                }
            } else {
                TreeNode *target;
                if (tempPtr->left != nullptr)
                    target = findFrontNode(tempPtr);
                else
                    target = findBackNode(tempPtr);

                std::swap(target->key, tempPtr->key);
                std::swap(target->value, tempPtr->value);
                deleteNode(target, target->father);
            }
            size_--;
            return;
        }
    }

};

///     Red_Black_Tree Implementation

template<typename Key_t, typename Value_t>
class Red_Black_Tree {

private:

    struct RBTreeNode {
        Key_t key;
        Value_t value;
        bool color;  // true代表红色，false代表黑色
        RBTreeNode *father = nullptr;
        RBTreeNode *left = nullptr, *right = nullptr;
    };

    RBTreeNode *findFrontNode(RBTreeNode *);
    RBTreeNode *findBackNode(RBTreeNode *);
    RBTreeNode *PODInsert(Key_t, Value_t);
    void PODErase(Key_t);
    void fixUpTree(RBTreeNode *);
    void spinLeft(RBTreeNode *);
    void spinRight(RBTreeNode *);
    void fixUpStructure(RBTreeNode *, RBTreeNode *);
    void deleteNoChildNode(RBTreeNode *);
    void deleteWithOneChildNode(RBTreeNode *);
    void deleteWithDoubleChildNode(RBTreeNode *);
    std::pair<size_t, bool>detectFunc(RBTreeNode *);

    RBTreeNode *head = nullptr;
    size_t size_ = 0;

public:

    Red_Black_Tree() = default;
    void insert(key_t ,Value_t);
    void erase(key_t);
    std::pair<size_t, bool>check() { return detectFunc(head); }
};


template<typename Key_t, typename Value_t>
typename Red_Black_Tree<Key_t, Value_t>::RBTreeNode *Red_Black_Tree<Key_t, Value_t>::findFrontNode(Red_Black_Tree::RBTreeNode *node) {
    RBTreeNode *root = node;
    RBTreeNode *tempPtr = nullptr;

    Vector<RBTreeNode *>stack;
    while (true) {
        while (node != nullptr) {
            stack.push_back(node);
            node = node->left;
        }

        node = *(stack.end() - 1);
        if (node == root)
            return tempPtr;
        tempPtr = node;
        node = node->right;
        stack.pop_back();
    }
}

template<typename Key_t, typename Value_t>
typename Red_Black_Tree<Key_t, Value_t>::RBTreeNode *Red_Black_Tree<Key_t, Value_t>::findBackNode(Red_Black_Tree::RBTreeNode *node) {
    RBTreeNode *root = node;
    RBTreeNode *tempPtr = nullptr;

    Vector<RBTreeNode *>stack;
    while (true) {
        while (node != nullptr) {
            stack.push_back(node);
            node = node->left;
        }

        node = *(stack.end() - 1);
        if (tempPtr == root)
            return node;
        tempPtr = node;
        node = node->right;
        stack.pop_back();
    }
}

template<typename Key_t, typename Value_t>
typename Red_Black_Tree<Key_t, Value_t>::RBTreeNode *Red_Black_Tree<Key_t, Value_t>::PODInsert(Key_t key, Value_t value) {
    RBTreeNode *node = new RBTreeNode;
    node->key = key;
    node->value = value;
    node->color = true;
    size_++;

    if (head == nullptr) {
        head = node;
        head->color = false;
        return head;
    }

    RBTreeNode* tempPtr = head;

    while (tempPtr) {
        if (tempPtr->key > key) {
            if (tempPtr->left == nullptr) {
                tempPtr->left = node;
                node->father = tempPtr;
                fixUpTree(node);
                return node;
            } else
                tempPtr = tempPtr->left;
        } else if (tempPtr->key < key) {
            if (tempPtr->right == nullptr) {
                tempPtr->right = node;
                node->father = tempPtr;
                fixUpTree(node);
                return node;
            } else
                tempPtr = tempPtr->right;
        } else {
            tempPtr->value = value;
            delete node;
            size_--;
            return tempPtr;
        }
    }

    return nullptr;
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::PODErase(Key_t key) {
    RBTreeNode *node = head;

    while (node) {
        if (node->key < key)
            node = node->right;
        else if (node->key > key)
            node = node->left;
        else {
            if (node->left == nullptr && node->right == nullptr)
                deleteNoChildNode(node);
            else if (node->left != nullptr && node->right != nullptr)
                deleteWithDoubleChildNode(node);
            else
                deleteWithOneChildNode(node);

            return;
        }
    }

    throw std::out_of_range("找不到需要删除的节点");
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::spinLeft(Red_Black_Tree::RBTreeNode *father) {
    if (father->right == nullptr)
        return;

    RBTreeNode *node = father->right, *originLeft = node->left;
    node->father = father->father;
    node->left = father;
    father->father = node;
    father->right = originLeft;
    if (originLeft != nullptr)
        originLeft->father = father;
    if (node->father != nullptr) {
        if (node->father->left == father)
            node->father->left = node;
        else
            node->father->right = node;
    } else {
        head = node;
    }
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::spinRight(RBTreeNode *father) {
    if (father->left == nullptr)
        return;

    RBTreeNode *node = father->left, *originRight = node->right;
    node->father = father->father;
    node->right = father;
    father->father = node;
    father->left = originRight;
    if (originRight != nullptr)
        originRight->father = father;
    if (node->father != nullptr) {
        if (node->father->left == father)
            node->father->left = node;
        else
            node->father->right = node;
    } else {
        head = node;
    }
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::deleteNoChildNode(Red_Black_Tree::RBTreeNode *node) {
    bool color = node->color;
    RBTreeNode *father = node->father;
    if (father == nullptr) {
//        delete head;
        head = nullptr;
    } else {
        if (father->left == node)
            father->left = nullptr;
        else
            father->right = nullptr;
        if (!color)
            fixUpStructure(nullptr, father);
        delete node;
    }
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::deleteWithOneChildNode(Red_Black_Tree::RBTreeNode *node) {
    bool color = node->color;
    RBTreeNode *father = node->father;
    RBTreeNode *child;

    if (node->left != nullptr) {
        child = node->left;
    } else {
        child = node->right;
    }

    if (father == nullptr) {
        head = child;
        child->father = nullptr;
        child->color = false;
        delete node;
    } else {
        father->left == node ? father->left = child : father->right = child;
        child->father = father;
        if (!color)
            fixUpStructure(child, father);
        delete node;
    }
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::deleteWithDoubleChildNode(Red_Black_Tree::RBTreeNode *node) {
    RBTreeNode *replace = findBackNode(node);
    node->key = replace->key;
    node->value = replace->value;
    if (replace->left == nullptr && replace->right == nullptr)
        deleteNoChildNode(replace);
    else
        deleteWithOneChildNode(replace);
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::fixUpTree(Red_Black_Tree::RBTreeNode *node) {
    if (node == head) {
        node->color = false;
    } else if (node->father->color == false) {
        return;
    } else {
       RBTreeNode *grandpa = node->father->father;
       RBTreeNode *father = node->father;
       RBTreeNode *uncle;
       if (grandpa->left == father) {
           uncle = grandpa->right;
           if (uncle && uncle->color == true) {
               father->color = false;
               uncle->color = false;
               grandpa->color = true;
               fixUpTree(grandpa);
           } else {
               if (father->right == node) {
                   spinLeft(father);
                   fixUpTree(father);
               } else {
                   grandpa->color = true;
                   father->color = false;
                   spinRight(grandpa);
                   fixUpTree(node);
               }
           }
       } else {
           uncle = grandpa->left;
           if (uncle && uncle->color == true) {
               father->color = false;
               uncle->color = false;
               grandpa->color = true;
               fixUpTree(grandpa);
           } else {
               if (father->left == node) {
                   spinRight(father);
                   fixUpTree(father);
               } else {
                   grandpa->color = true;
                   father->color = false;
                   spinLeft(grandpa);
                   fixUpTree(node);
               }
           }

       }
    }
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::insert(key_t key, Value_t value) {
    PODInsert(key, value);
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::erase(key_t key) {
    PODErase(key);
    size_--;
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::fixUpStructure(Red_Black_Tree::RBTreeNode *child, Red_Black_Tree::RBTreeNode *father) {
    if (child != nullptr && child->color == true) {
        child->color = false;
        return;
    }

    if (child == head) {
        child->color = false;
        return;
    }

    RBTreeNode *bro;

    if (father->left == child) {
        bro = father->right;
        if (bro->color == true) {
            bro->color = false;
            father->color = true;
            spinLeft(father);
            fixUpStructure(child, father);
        } else if ((bro->left == nullptr || bro->left->color == false) &&
                   (bro->right == nullptr || bro->right->color == false)) {
            bro->color = true;
            fixUpStructure(father, father->father);
        } else if ((bro->left != nullptr && bro->left->color == true) &&
                   (bro->right == nullptr || bro->right->color == false)) {
            bro->left->color = false;
            bro->color = true;
            spinRight(bro);
            fixUpStructure(child, father);
        } else if (bro->right != nullptr && bro->right->color == true) {
            bro->color = father->color;
            father->color = false;
            bro->right->color = false;
            spinLeft(father);
            head->color = false;
            return;
        }
    } else {
        bro = father->left;
        if (bro->color == true) {
            bro->color = false;
            father->color = true;
            spinRight(father);
            fixUpStructure(child, father);
        } else if ((bro->left == nullptr || bro->left->color == false) &&
                   (bro->right == nullptr || bro->right->color == false)) {
            bro->color = true;
            fixUpStructure(father, father->father);
        } else if ((bro->right != nullptr && bro->right->color == true) &&
                   (bro->left == nullptr || bro->left->color == false)) {
            bro->right->color = false;
            bro->color = true;
            spinLeft(bro);
            fixUpStructure(child, father);
        } else if (bro->left != nullptr && bro->left->color == true) {
            bro->color = father->color;
            father->color = false;
            bro->left->color = false;
            spinRight(father);
            head->color = false;
            return;
        }
    }
}

template<typename Key_t, typename Value_t>
std::pair<size_t, bool> Red_Black_Tree<Key_t, Value_t>::detectFunc(RBTreeNode *node) {

    if (node == nullptr)
        return {1, true};

    std::pair<size_t , bool> left, right;
    left = detectFunc(node->left);
    right = detectFunc(node->right);

    if (!left.second || !right.second)
        return {0, false};

    if (left.first != right.first)
        return {0, false};

    if (head == node && node->color == true)
        return {0, false};

    if (node->color == true && (!node->father || node->father->color == true))
        return {0, false};

    if (node->color == false)
        left.first += 1;
    return left;
}








