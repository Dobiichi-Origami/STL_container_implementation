//
// Created by 鸢一折纸 on 2021/5/28.
//

#pragma once

#include "Hash_Table.h"


template<typename Key_t,typename Hash_Function_t>
class Unordered_Set {

private:
    HashTable<Key_t, Key_t, Hash_Function_t> table;
    size_t size_ = 0;

public:

    class iterator {

    private:
        typename HashTable<Key_t, Key_t, Hash_Function_t>::iterator ite;

    public:
        iterator() = default;
        explicit iterator(typename HashTable<Key_t, Key_t, Hash_Function_t>::iterator itx);
        iterator &operator=(iterator &);
        iterator &operator++() { ++ite; return *this; }
        iterator operator++(int) { iterator i = *this; ite++; return i; }
        iterator &operator--() { --ite; return *this; }
        iterator operator--(int) { iterator i = *this; ite--; return i; }
        bool operator==(iterator other) { return ite == other.ite; }
        bool operator!=(iterator other) { return ite != other.ite; }
        Key_t &operator*();
    };


    Unordered_Set(size_t capacity = 1ul);
    void insert(Key_t);
    void erase(Key_t);
    iterator find(Key_t);
    size_t capacity() { return table.capacity(); }
    size_t size() { return size_; }
    bool isEmpty() { return table.isEmpty(); }
    iterator begin();
    iterator end();
};

/// ----------- UnorderedMap implementation -----------

template<typename Key_t, typename Hash_Function_t>
Unordered_Set<Key_t, Hash_Function_t>::Unordered_Set(size_t capacity): table(capacity) {}

template<typename Key_t, typename Hash_Function_t>
void Unordered_Set<Key_t, Hash_Function_t>::insert(Key_t key) {
    size_++;
    table.insert(key, key);
}

template<typename Key_t, typename Hash_Function_t>
void Unordered_Set<Key_t, Hash_Function_t>::erase(Key_t key) {
    size_--;
    table.erase(key);
}



template<typename Key_t, typename Hash_Function_t>
typename Unordered_Set<Key_t, Hash_Function_t>::iterator Unordered_Set<Key_t, Hash_Function_t>::begin() {
    return iterator(table.begin());
}

template<typename Key_t, typename Hash_Function_t>
typename Unordered_Set<Key_t, Hash_Function_t>::iterator Unordered_Set<Key_t, Hash_Function_t>::end() {
    return iterator(table.end());
}

template<typename Key_t, typename Hash_Function_t>
typename Unordered_Set<Key_t, Hash_Function_t>::iterator Unordered_Set<Key_t, Hash_Function_t>::find(Key_t key) {
    return iterator(table.find(key));
}

/// ----------- Iterator implementation -----------

template<typename Key_t, typename Hash_Function_t>
typename Unordered_Set<Key_t, Hash_Function_t>::iterator &Unordered_Set<Key_t, Hash_Function_t>::iterator::operator=(Unordered_Set::iterator &other) {
    ite = other.ite;
    return *this;
}

template<typename Key_t, typename Hash_Function_t>
Key_t &Unordered_Set<Key_t, Hash_Function_t>::iterator::operator*() {
    return ite.getValue();
}

template<typename Key_t, typename Hash_Function_t>
Unordered_Set<Key_t, Hash_Function_t>::iterator::iterator(typename HashTable<Key_t, Key_t, Hash_Function_t>::iterator itx) {
    this->ite = itx;
}
