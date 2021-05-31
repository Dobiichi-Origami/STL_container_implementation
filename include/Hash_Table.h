//
// Created by 鸢一折纸 on 2021/5/27.
//

#pragma once

#include <memory>

template<typename Key_t, typename Value_t, typename Hash_Function_t>
class HashTable {

private:

    struct elementNode {
        Key_t key = NULL;
        Value_t value = NULL;
        std::shared_ptr<elementNode>next = nullptr;
        std::shared_ptr<elementNode>before = nullptr;
    };


    constexpr static const unsigned long primes[29] =
            {
                    5ul,          53ul,         97ul,         193ul,       389ul,
                    769ul,        1543ul,       3079ul,       6151ul,      12289ul,
                    24593ul,      49157ul,      98317ul,      196613ul,    393241ul,
                    786433ul,     1572869ul,    3145739ul,    6291469ul,   12582917ul,
                    25165843ul,   50331653ul,   100663319ul,  201326611ul, 402653189ul,
                    805306457ul,  1610612741ul, 3221225473ul, 4294967291ul
            };

    size_t findNextPrime(size_t capacity) {
        for (int i = 0; i < 29; ++i)
            if (primes[i] > capacity)
                return primes[i];

        return 1;
    }

    void resize();

    size_t size_ = 0;
    size_t capacity_ = 5;
    std::shared_ptr<elementNode> *head;

public:

    class iterator {

    private:
        elementNode* node = nullptr;
        HashTable *table;

    public:
        iterator() = default;
        explicit iterator(elementNode *, HashTable *);
        iterator &operator=(iterator &);
        iterator &operator++();
        iterator operator++(int);
        iterator &operator--();
        iterator operator--(int);
        bool operator==(iterator ite);
        bool operator!=(iterator ite);
        Value_t &getValue();
        Key_t &getKey();
    };

    HashTable(size_t capacity = 1ul);
    ~HashTable();
    void insert(Key_t, Value_t);
    void erase(key_t);
    iterator find(Key_t);
    iterator end();
    iterator begin();
    size_t capacity() { return capacity_; }
    size_t size() { return size_; }
    bool isEmpty() { return size_ == 0; }
};

/// ----------- HashTable implementation -----------

template<typename Key_t, typename Value_t, typename Hash_Function_t>
void HashTable<Key_t, Value_t, Hash_Function_t>::resize() {
    HashTable<Key_t, Value_t, Hash_Function_t> tempT(capacity_);
    for (auto ite = this->begin(); ite != this->end(); ++ite) {
        tempT.insert(ite.getKey(), ite.getValue());
    }
    capacity_ = findNextPrime(capacity_);
    swap(head, tempT.head);
};

template<typename Key_t, typename Value_t, typename Hash_Function_t>
HashTable<Key_t, Value_t, Hash_Function_t>::HashTable(size_t capacity) {
    capacity_ = findNextPrime(capacity);
    head = new std::shared_ptr<elementNode>[capacity_];
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
HashTable<Key_t, Value_t, Hash_Function_t>::~HashTable() {
    delete []head;
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
void HashTable<Key_t, Value_t, Hash_Function_t>::insert(Key_t key, Value_t value) {
    if (size_ == capacity_)
        resize();

    Hash_Function_t func;

    unsigned long hashCode = func(key) % capacity_;
    std::shared_ptr<elementNode> *bucket = head + hashCode;
    if (bucket->get() == nullptr) {
        std::shared_ptr<elementNode> node(new elementNode);
        node->key = key;
        node->value = value;
        *bucket = node;
    } else {
        elementNode* node = (*bucket).get();
        while (node->next != nullptr) {
            if (node->key == key) {
                node->value = value;
                return;
            }
            node = (node->next).get();
        }

        elementNode* newNode = new elementNode;
        newNode->key = key;
        newNode->value = value;
        newNode->before = std::make_shared<elementNode>(*node);
        node->next = std::make_shared<elementNode>(*newNode);
    }
    size_++;
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
void HashTable<Key_t, Value_t, Hash_Function_t>::erase(key_t key) {
    Hash_Function_t func;
    unsigned long hashCode = func(key) % capacity_;
    elementNode* node = (head + hashCode)->get();
    while (node != nullptr) {
        if (node->key != key)
            node = (node->next).get();
        else {
            if (node->before != nullptr && node->next != nullptr) {
                node->next->before = node->before;
                node->before->next = node->next;
            } else if (node->before == nullptr && node->next == nullptr) {
                (head + hashCode)->reset();
            } else if (node->before == nullptr) {
                *(head + hashCode) = node->next;
                node->next->before = nullptr;
            } else {
                node->before->next = nullptr;
            }
            size_--;
            return;
        }
    }

}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
typename HashTable<Key_t, Value_t, Hash_Function_t>::iterator HashTable<Key_t, Value_t, Hash_Function_t>::end() {
    if (size_ == 0)
        return iterator(nullptr, this);

    std::shared_ptr<elementNode> *ptr = head + capacity_ - 1;
    while (ptr->get() == nullptr)
        ptr--;

    elementNode* node = ptr->get();
    while (node->next != nullptr)
        node = (node->next).get();

    return iterator(node+1, this);
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
typename HashTable<Key_t, Value_t, Hash_Function_t>::iterator HashTable<Key_t, Value_t, Hash_Function_t>::begin() {
    if (size_ == 0)
        return iterator(nullptr, this);


    std::shared_ptr<elementNode> *ptr = head;
    while (ptr->get() == nullptr)
        ptr++;

    return iterator(ptr->get(), this);
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
typename HashTable<Key_t, Value_t, Hash_Function_t>::iterator HashTable<Key_t, Value_t, Hash_Function_t>::find(Key_t key) {
    Hash_Function_t func;
    unsigned long hashCode = func(key) % capacity_;
    elementNode* node = (head + hashCode)->get();
    while (node != nullptr) {
        if (node->key == key) {
            return iterator(node, this);
        }

        node = (node->next).get();
    }

    return end();
}

/// ----------- Iterator implementation -----------

template<typename Key_t, typename Value_t, typename Hash_Function_t>
HashTable<Key_t, Value_t, Hash_Function_t>::iterator::iterator(HashTable::elementNode * node, HashTable* table) {
    this->node = node;
    this->table = table;
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
typename HashTable<Key_t, Value_t, Hash_Function_t>::iterator &HashTable<Key_t, Value_t, Hash_Function_t>::iterator::operator=(HashTable::iterator &ite) {
    this->node = ite.node;
    this->table = ite.table;
    return *this;
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
typename HashTable<Key_t, Value_t, Hash_Function_t>::iterator &HashTable<Key_t, Value_t, Hash_Function_t>::iterator::operator++() {
    if (*this == table->end())
        throw std::out_of_range("index out-of-range");

    if (node->next != nullptr)
        node = (node->next).get();
    else {
        elementNode* tempPtr = node;
        while (tempPtr->before != nullptr)
            tempPtr = (tempPtr->before).get();

        std::shared_ptr<elementNode> *tempHead = table->head;
        while ((*tempHead).get() != tempPtr)
            tempHead++;

        std::shared_ptr<elementNode> *tempHead2 = (table->head + table->capacity_ - 1);
        while ((*tempHead2).get() == nullptr)
            tempHead2--;

        if (tempHead == tempHead2) {
            node = node+1;
            return *this;
        }

       do node = (*(++tempHead)).get();
        while (node == nullptr);
    }

    return *this;
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
typename HashTable<Key_t, Value_t, Hash_Function_t>::iterator HashTable<Key_t, Value_t, Hash_Function_t>::iterator::operator++(int) {

    iterator ite = *this;
    ++(*this);
    return ite;
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
typename HashTable<Key_t, Value_t, Hash_Function_t>::iterator &HashTable<Key_t, Value_t, Hash_Function_t>::iterator::operator--() {
    if (*this == table->begin())
        throw std::out_of_range("index out-of-range");

    if (node->before != nullptr)
        node = node->before;
    else {
        std::shared_ptr<elementNode> *tempHead = table->head;
        while ((**tempHead) != node)
            tempHead++;

        node = *(tempHead-1);
        while (node->next != nullptr)
            node = node->next;
    }

    return *this;
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
typename HashTable<Key_t, Value_t, Hash_Function_t>::iterator HashTable<Key_t, Value_t, Hash_Function_t>::iterator::operator--(int) {
    iterator ite = *this;
    --(*this);
    return ite;
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
bool HashTable<Key_t, Value_t, Hash_Function_t>::iterator::operator==(HashTable::iterator ite) {
    return (node == ite.node) && (table == ite.table);
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
bool HashTable<Key_t, Value_t, Hash_Function_t>::iterator::operator!=(HashTable::iterator ite) {
    return (node != ite.node) || (table != ite.table);
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
Value_t &HashTable<Key_t, Value_t, Hash_Function_t>::iterator::getValue() {
    return node->value;
}

template<typename Key_t, typename Value_t, typename Hash_Function_t>
Key_t &HashTable<Key_t, Value_t, Hash_Function_t>::iterator::getKey() {
    return node->key;
}

