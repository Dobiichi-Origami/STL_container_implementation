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
    RBTreeNode *halfPODErase(Key_t);
    void fixUpTree(RBTreeNode *);
    void spinLeft(RBTreeNode *);
    void spinRight(RBTreeNode *);
    void flipColor(RBTreeNode *node) { node->color = !(node->color); }

    RBTreeNode *head = nullptr;
    size_t size_ = 0;

public:

    Red_Black_Tree() = default;
    void insert(key_t ,Value_t);
    void erase(key_t);
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
        return head;
    }

    RBTreeNode* tempPtr = head;

    while (tempPtr) {
        if (tempPtr->key > key) {
            if (tempPtr->left == nullptr) {
                tempPtr->left = node;
                node->father = tempPtr;
                return node;
            } else
                tempPtr = tempPtr->left;
        } else if (tempPtr->key < key) {
            if (tempPtr->right == nullptr) {
                tempPtr->right = node;
                node->father = tempPtr;
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
typename Red_Black_Tree<Key_t, Value_t>::RBTreeNode *Red_Black_Tree<Key_t, Value_t>::halfPODErase(Key_t key) {
    RBTreeNode *node = head;

    while (node) {
        if (node->key < key)
            node = node->right;
        else if (node->key > key)
            node = node->left;
        else {
            RBTreeNode *replace;

            replace = findFrontNode(node);
            if (replace) {
                std::swap(replace->key, node->key);
                std::swap(replace->value, node->value);
                return replace;
            }
            replace = findBackNode(node);
            if (replace) {
                std::swap(replace->key, node->key);
                std::swap(replace->value, node->value);
                return replace;
            }

            if (node->father->left == node)
                node->father->left = nullptr;
            else
                node->father->right = nullptr;

            return nullptr;

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
void Red_Black_Tree<Key_t, Value_t>::fixUpTree(Red_Black_Tree::RBTreeNode *node) {
    if (node == head) {
        node->color = false;
    } else if (node->father->color == false) {
        return;
    } else {
       RBTreeNode *grandpa = node->father->father;
       RBTreeNode *father = node->father;
       RBTreeNode *uncle;
        if (grandpa->left == father)
            uncle = grandpa->right;
        else
            uncle = grandpa->left;

        if (uncle != nullptr && uncle->color == true) {
            uncle->color = father->color = false;
            grandpa->color = true;
            fixUpTree(grandpa);
            return;
        } else {
            if (father->right == node) {
                spinLeft(father);
                fixUpTree(father);
            } else {
                father->color = false;
                grandpa->color = true;
                spinRight(grandpa);
                fixUpTree(grandpa);
            }
        }
    }
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::insert(key_t key, Value_t value) {
    RBTreeNode *node = PODInsert(key, value);
    fixUpTree(node);
}

template<typename Key_t, typename Value_t>
void Red_Black_Tree<Key_t, Value_t>::erase(key_t key) {
    RBTreeNode *node = halfPODErase(key);

    if (node == nullptr)
        return;

}





