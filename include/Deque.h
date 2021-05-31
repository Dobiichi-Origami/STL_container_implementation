//
// Created by 鸢一折纸 on 2021/5/24.
//

#pragma once

#include "Vector.h"
#include <exception>
#include <typeinfo>


template<typename T>
class Deque {

private:

    class Block {

    private:
        T* head;
        T* tail;
        T* nowTail;
        T* nowHead;
        bool isLeft = false, isRight = false;
        static const size_t BLOCKSIZE = 64;

    public:
        Block() {
            head = new T[BLOCKSIZE];
            tail = head + BLOCKSIZE;
            nowTail = head + 31;
            nowHead = nowTail + 1;
            bzero(head, sizeof(T) * BLOCKSIZE);
        }

        void setDirection() {
            if (isLeft == isRight)
                return;
            if (isLeft) {
                nowHead = tail;
                nowTail = nowHead - 1;
            } else {
                nowTail = head - 1;
                nowHead = head;
            }
        }

        ~Block() {
            head = tail = nowTail = nowHead = nullptr;
        }

        void deleteAll() {
            delete []head;
        }

        bool isTailPtrAtTail() { return nowTail == tail - 1; }
        bool isHeadPtrAtHead() { return nowHead == head; }
        bool isEmpty() { return nowHead > nowTail; }

        bool pushBack(T data_) {
            if (isTailPtrAtTail())
                return false;

            nowTail++;
            *nowTail = data_;
            return true;
        }

        bool pushFront(T data_) {
            if (isHeadPtrAtHead())
                return false;

            nowHead--;
            *nowHead = data_;
            return true;
        }

        bool popBack() {
            if (isEmpty())
                return false;

            nowTail--;
            return true;
        }

        bool popFront() {
            if (isEmpty())
                return false;

            nowHead++;
            return true;
        }

        T* getTail() { return nowTail; }
        T* getHead() { return nowHead; }

        void setLeft() { isLeft = true; }
        void setRight() { isRight = true; }
        bool getLeft() { return isLeft; }
        bool getRight() { return isRight; }

        int getSize() { return nowTail - nowHead + 1; }

    };


    static void deleter(Block *element) { delete []element; }

    size_t size_ = 0;
    size_t blockSize = 1;
    size_t leftCapacityFactor = 1;
    size_t rightCapacityFactor = 1;
    std::shared_ptr<Block> memoMap;
    Block *head, *tail;

    void addSpaceAtRight() {
        size_t distanceL = head - memoMap.get(), distanceR = tail - memoMap.get();
        std::shared_ptr<Block> tempPtr(new Block[blockSize + rightCapacityFactor], deleter);
        memcpy(tempPtr.get(), memoMap.get(), sizeof(Block) * blockSize);
        memoMap.swap(tempPtr);
        head = memoMap.get() + distanceL, tail = memoMap.get() + distanceR;
        blockSize += rightCapacityFactor;
        rightCapacityFactor *= 2;
    }

    void addSpaceAtLeft() {
        size_t distanceL = head - memoMap.get() + leftCapacityFactor, distanceR = tail - memoMap.get() + leftCapacityFactor;
        std::shared_ptr<Block> tempPtr(new Block[blockSize + leftCapacityFactor], deleter);
        memcpy(tempPtr.get() + leftCapacityFactor, memoMap.get(), sizeof(Block) * blockSize);
        memoMap.swap(tempPtr);
        head = memoMap.get() + distanceL, tail = memoMap.get() + distanceR;
        blockSize += leftCapacityFactor;
        leftCapacityFactor *= 2;
    }

    bool isGetHead() { return memoMap.get() == head; }
    bool isGetTail() { return memoMap.get() + blockSize - 1 == tail; }


public:

    class iterator {

    private:
        T* element;
        int pos = 0;
        Deque<T> *deque = nullptr;

    public:
        iterator() = default;
        iterator &operator=(iterator &);
        explicit iterator(T *, int, Deque<T> *);
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
    };


    Deque();
    ~Deque();
    void push_front(T);
    void push_back(T);
    void pop_front();
    void pop_back();
    iterator begin();
    iterator end();
    size_t size();
    void insert(iterator, T);
    void erase(iterator);
    T &operator[](int);
    bool isEmpty() { return size_ == 0; }

};

/// ----------- Deque implementation -----------

template<typename T>
Deque<T>::Deque(): memoMap(new Block[1], deleter), head(memoMap.get()), tail(memoMap.get()) {}

template<typename T>
void Deque<T>::push_front(T data_) {
    size_++;
    if (!head->pushFront(data_)) {
        if (isGetHead())
            addSpaceAtLeft();

        head--;
        head->setLeft();
        head->setDirection();
        head->pushFront(data_);
    }

}

template<typename T>
void Deque<T>::push_back(T data_) {
    size_++;
    if (!tail->pushBack(data_)) {
        if (isGetTail())
            addSpaceAtRight();

        tail++;
        tail->setRight();
        tail->setDirection();
        tail->pushBack(data_);
    }
}

template<typename T>
void Deque<T>::pop_front() {
    if (size_ == 0)
        return;

    size_--;
    while (!head->popFront())
        head++;

    if (head->isEmpty())
        head++;
}

template<typename T>
void Deque<T>::pop_back() {
    if (size_ == 0)
        return;

    size_--;
    while (!tail->popBack())
        tail--;

    if (tail->isEmpty())
        tail--;

}

template<typename T>
typename Deque<T>::iterator Deque<T>::begin() {
    return iterator(head->getHead(), 0, this);
}

template<typename T>
typename Deque<T>::iterator Deque<T>::end() {
    return Deque::iterator(tail->getTail() + 1, size_, this);
}

template<typename T>
T &Deque<T>::operator[](int i) {

    if (size_ <= i || i < 0)
        throw std::out_of_range("index out-of-range");

    Block *ptr = head;  // 取整个容器中第一块存放元素的内存块的首地址赋给ptr
    while (ptr->getSize() <= i) {   // 如果当前内存块的大小小于等于索引值
        i -= ptr->getSize();        // 说明这个元素不在这个索引块内
        ptr++;                      // 循环查找，直到到达含有元素的内存块
    }

    return *(ptr->getHead() + i);   // 返回对应值的引用
}

template<typename T>
size_t Deque<T>::size() {
    return size_;
}

template<typename T>
Deque<T>::~Deque() {
    Block *ptr = memoMap.get();
    for (int i = 0; i < blockSize; ++i)
        (ptr+i)->deleteAll();
}

template<typename T>
void Deque<T>::insert(Deque::iterator ite, T data_) {
    int posB = ite - begin(), posE = (end() - 1) - ite;

    if (posB <= posE) {
        push_front(data_);
        iterator itex = begin();
        while (itex != ite) {
            std::swap(*itex, *(itex+1));
            ++itex;
        }
    } else {
        push_back(data_);
        iterator itex = end() - 1;
        while (itex != ite) {
            std::swap(*itex, *(itex-1));
            --itex;
        }
    }
}

template<typename T>
void Deque<T>::erase(Deque::iterator ite) {
    Block* ptr = head;
    int pos = ite - begin();
    while (ptr->getSize() <= pos) {
        pos -= ptr->getSize();
        ptr++;
    }

    T* e = ptr->getHead() + pos;
    if (ptr->getLeft()) {
        while (e != ptr->getHead()) {
            *e = *(e - 1);
            e--;
        }
        ptr->popFront();
    } else {
        while (e != ptr->getTail() - 1) {
            *e = *(e + 1);
            e++;
        }
        ptr->popBack();
    }
}

/// ----------- Iterator implementation -----------

template<typename T>
Deque<T>::iterator::iterator(T* element, int pos, Deque<T> *dq) {
    this->element = element;
    this->pos = pos;
    deque = dq;
}

template<typename T>
typename Deque<T>::iterator &Deque<T>::iterator::operator=(Deque::iterator &ite) {
    element = ite.element;
    pos = ite.pos;
    this->deque = ite.deque;
}

template<typename T>
T &Deque<T>::iterator::operator*() {
    return *element;
}

template<typename T>
typename Deque<T>::iterator &Deque<T>::iterator::operator++() {
    pos++;
    element = &(*deque)[pos];
    return *this;
}

template<typename T>
typename Deque<T>::iterator Deque<T>::iterator::operator++(int) {
    iterator tempIte = *this;
    ++(*this);
    return tempIte;
}

template<typename T>
int Deque<T>::iterator::operator-(Deque::iterator ite) {
    return pos - ite.pos;
}

template<typename T>
typename Deque<T>::iterator &Deque<T>::iterator::operator--() {
    pos--;
    element = &(*deque)[pos];
    return *this;
}

template<typename T>
typename Deque<T>::iterator Deque<T>::iterator::operator--(int) {
    iterator ite = *this;
    --(*this);
    return ite;
}

template<typename T>
typename Deque<T>::iterator Deque<T>::iterator::operator+(int i) {
    iterator ite = *this;
    ite.pos += i;
    ite.element = &(*deque)[ite.pos];
    return ite;
}

template<typename T>
typename Deque<T>::iterator Deque<T>::iterator::operator-(int i) {
    return this->operator+(-i);
}

template<typename T>
bool Deque<T>::iterator::operator==(Deque::iterator ite) {
    return element == ite.element;
}

template<typename T>
bool Deque<T>::iterator::operator!=(Deque::iterator ite) {
    return element != ite.element;
}




