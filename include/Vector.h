//
// Created by 鸢一折纸 on 2021/5/24.
//

#pragma once

#include <cstdint>
#include <memory>
#include <exception>

template<typename T>
class Vector {

private:
    uint32_t size_;
    uint32_t capacity;
    std::shared_ptr<T> head;

    static void deleter(T *element){
        delete []element;
    }

public:
    class iterator {

    private:
        T* element; // 防止数组中元素被析构，使用plain pointer

    public:
        iterator() = default;
        explicit iterator(T *);
        T &operator*();
        iterator &operator++();
        iterator operator++(int);
        iterator &operator--();
        iterator operator--(int);
        iterator operator+(int);
        iterator operator-(int);
        int operator-(iterator);
        bool operator==(iterator);
        bool operator!=(iterator);
        iterator &operator=(iterator);
    };


    Vector();
    ~Vector();
    void push_back(T);
    void push_front(T);
    void pop_front();
    void pop_back();
    size_t size();
    bool isEmpty();
    iterator begin();
    iterator end();
    void insert(iterator, T);
    void erase(iterator);
    void resize();
    T &operator[](int);
};

/// ----------- Vector implementation -----------

template<typename T>
Vector<T>::Vector(): size_(0), capacity(1), head(new T[1], deleter) {}

template<typename T>
Vector<T>::~Vector() {
    head.reset();
}

template<typename T>
void Vector<T>::push_back(T data_) {
    if (size_ == capacity)
        resize();

    *end() = data_;
    size_++;
}

template<typename T>
void Vector<T>::push_front(T data_) {
    insert(begin(), data_);
}

template<typename T>
void Vector<T>::pop_front() {
    erase(begin());
}

template<typename T>
void Vector<T>::pop_back() {
    if (isEmpty())
        return;

    size_--;
    if (size_ < capacity / 2)
        resize();
}

template<typename T>
size_t Vector<T>::size() {
    return size_;
}

template<typename T>
bool Vector<T>::isEmpty() {
    return size_ == 0;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::begin() {
    return iterator(head.get());
}

template<typename T>
typename Vector<T>::iterator Vector<T>::end() {
    return iterator(head.get()) + size_;
}

template<typename T>
void Vector<T>::insert(Vector::iterator ite, T data_) {
    if(ite == end()) {
        push_back(data_);
        return;
    }

    if(size_ == capacity)
        resize();

    iterator back = end();
    while (back != ite) {
        *back = *(back-1);
        back--;
    }

    *ite = data_;
    size_++;
}

template<typename T>
void Vector<T>::erase(Vector::iterator ite) {
    if (isEmpty())
        return;

    if (ite == end()) {
        pop_back();
        return;
    }

    while (ite != end() - 1) {
        *ite = *(ite+1);
        ite++;
    }

    size_--;
    if (size_ < capacity / 2)
        resize();
}

template<typename T>
void Vector<T>::resize() {
    uint32_t temp = 1;
    while (temp <= size_)
        temp <<= 1;

    capacity = temp;
    std::shared_ptr<T> tempPtr(new T[capacity], deleter);
    memcpy(tempPtr.get(), head.get(), sizeof(T) * size_);
    head.swap(tempPtr);
    tempPtr.reset();
}

template<typename T>
T &Vector<T>::operator[](int i) {
    if (size_ <= i || i < 0)
        throw std::out_of_range("index out-of-range");

    return *(begin() + i);
}

/// ----------- Iterator implementation -----------

template<typename T>
Vector<T>::iterator::iterator(T *ptr) {
    element = ptr;
}

template<typename T>
T &Vector<T>::iterator::operator*() {
    return *element;
}

template<typename T>
typename Vector<T>::iterator &Vector<T>::iterator::operator++() {
    element++;
    return *this;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator++(int) {
    iterator ite = *this;
    element++;
    return ite;
}

template<typename T>
typename Vector<T>::iterator &Vector<T>::iterator::operator--() {
    element--;
    return *this;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator--(int) {
    iterator ite = *this;
    element--;
    return ite;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator+(int number) {
    T* tempPtr = element + number;
    return iterator(tempPtr);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::iterator::operator-(int number) {
    T* tempPtr = element - number;
    return iterator(tempPtr);
}

template<typename T>
int Vector<T>::iterator::operator-(Vector::iterator ite) {
    return element - ite.element;
}

template<typename T>
bool Vector<T>::iterator::operator==(Vector::iterator ite) {
    return element == ite.element;
}

template<typename T>
bool Vector<T>::iterator::operator!=(Vector::iterator ite) {
    return element != ite.element;
}

template<typename T>
typename Vector<T>::iterator &Vector<T>::iterator::operator=(Vector::iterator ite) {
    element = ite.element;
    return *this;
}



