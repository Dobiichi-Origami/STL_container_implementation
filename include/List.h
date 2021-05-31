//
// Created by 鸢一折纸 on 2021/5/23.
//

#pragma once

#include <cstdint>
#include <memory>
#include <iostream>


template<typename T>
class List {

private:
    struct listNode {
        T data;
        std::shared_ptr<listNode> before;
        std::shared_ptr<listNode> behind;
    } ;


    std::shared_ptr<listNode> start, back;
    uint32_t size_ = 0;

public:
    class iterator {

    private:
        std::shared_ptr<listNode> ptr;

    public:
        iterator() = default;
        explicit iterator(std::shared_ptr<listNode> &);
        iterator &operator=(iterator &);
        iterator &operator++();
        iterator operator++(int);
        iterator &operator--();
        iterator operator--(int);
        T &operator*();
        bool operator==(iterator);
        bool operator!=(iterator);
        std::shared_ptr<listNode> getPtr();

    };


    List();
    ~List();
    void push_back(T);
    void push_front(T);
    void pop_back();
    void pop_front();
    iterator begin();
    iterator end();
    bool isEmpty();
    size_t size();
    void clear();
    T &front();
    T &tail();
    void insert(iterator, T);
    void remove(iterator);
};

/// ----------- List implementation -----------

template<typename T>
List<T>::List() {
    start = back = std::shared_ptr<listNode>(new listNode);
    start->before = start->behind = start;
}

template<typename T>
void List<T>::push_back(T data_) {
    size_++;
    std::shared_ptr<listNode> sharedPtr(new listNode);
    sharedPtr->data = data_;

    sharedPtr->behind = start;
    sharedPtr->before = back;
    back->behind = sharedPtr;
    start->before = sharedPtr;
    back = sharedPtr;
}

template<typename T>
void List<T>::push_front(T data_) {
    size_++;
    std::shared_ptr<listNode> sharedPtr(new listNode);
    sharedPtr->data = data_;

    sharedPtr->before = start;
    sharedPtr->behind = start->behind;
    start->behind = sharedPtr;
    sharedPtr->behind->before = sharedPtr;
}

template<typename T>
void List<T>::pop_back() {
    if (size_ == 0)
        return;

    std::shared_ptr<listNode> tempPtr = back->before;
    tempPtr->behind = start;
    start->before = tempPtr;
    back->behind.reset();
    back->before.reset();
    back = tempPtr;
    size_--;
}

template<typename T>
void List<T>::pop_front() {
    if (size_ == 0)
        return;

    std::shared_ptr<listNode> tempPtr = start->behind;
    start->behind = tempPtr->behind;
    tempPtr->behind->before = start;
    tempPtr->behind.reset();
    tempPtr->before.reset();
    size_--;
}

template<typename T>
List<T>::~List() {
    while (size_ != 0)
        pop_front();

    back->behind.reset();
    start->before.reset();
}

template<typename T>
typename List<T>::iterator List<T>::begin() {
    iterator ite(start->behind);
    return ite;
}

template<typename T>
typename List<T>::iterator List<T>::end() {
    iterator ite(start);
    return ite;
}

template<typename T>
bool List<T>::isEmpty() {
    return size_ == 0;
}

template<typename T>
size_t List<T>::size() {
    return size_;
}

template<typename T>
void List<T>::clear() {
    while (isEmpty())
        pop_front();
}

template<typename T>
T &List<T>::front() {
    if (isEmpty())
        return NULL;
    return start->behind->data;
}

template<typename T>
T &List<T>::tail() {
    if (isEmpty())
        return NULL;
    return start->before->data;
}

template<typename T>
void List<T>::insert(iterator ite, T data_) {

    if (ite == begin()) {
        push_front(data_);
        return;
    }

    if (ite == end()) {
        push_back(data_);
        return;
    }

    size_++;
    std::shared_ptr<listNode> tempPtr(new listNode);
    std::shared_ptr<listNode> itePtr(ite.getPtr());
    tempPtr->data = data_;
    tempPtr->before = itePtr->before;
    tempPtr->behind = itePtr;
    itePtr->before->behind = tempPtr;
    itePtr->before = tempPtr;
}

template<typename T>
void List<T>::remove(iterator ite) {
    if (isEmpty())
        return;

    if (ite == begin()){
        pop_front();
        return;
    }

    if (ite == end())
        return;

    std::shared_ptr<listNode> itePtr(ite.getPtr());
    itePtr->behind->before = itePtr->before;
    itePtr->before->behind = itePtr->behind;
    itePtr->behind.reset();
    itePtr->before.reset();
    size_--;
}

/// ----------- iterator implementation -----------

template<typename T>
List<T>::iterator::iterator(std::shared_ptr<listNode> &ptr_) {
    ptr = ptr_;
}

template<typename T>
typename List<T>::iterator &List<T>::iterator::operator=(iterator &ite) {
    ptr = ite.ptr;
    return *this;
}

template<typename T>
typename List<T>::iterator &List<T>::iterator::operator++() {
    ptr = ptr->behind;
    return *this;
}

template<typename T>
typename List<T>::iterator List<T>::iterator::operator++(int) {
    iterator temp = *this;
    ptr = ptr->behind;
    return temp;
}

template<typename T>
typename List<T>::iterator &List<T>::iterator::operator--() {
    ptr = ptr->before;
    return *this;
}

template<typename T>
typename List<T>::iterator List<T>::iterator::operator--(int) {
    iterator itr = *this;
    ptr = ptr->before;
    return itr;
}

template<typename T>
T &List<T>::iterator::operator*() {
    return ptr->data;
}

template<typename T>
bool List<T>::iterator::operator==(iterator ite) {
    return ite.ptr == ptr;
}

template<typename T>
bool List<T>::iterator::operator!=(iterator ite) {
    return ite.ptr != ptr;
}

template<typename T>
std::shared_ptr<typename List<T>::listNode> List<T>::iterator::getPtr() {
    return std::shared_ptr<listNode>(ptr);
}
