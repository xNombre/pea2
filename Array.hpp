#pragma once

#include <initializer_list>

template <typename T>
class Array
{
    std::size_t array_size = 0;
    T *array = nullptr;

public:
    Array(const std::size_t &size = 0);
    Array(const std::size_t &size, const std::size_t &value);
    Array(const std::initializer_list<T> &list);
    ~Array();
    Array(const Array &other);
    Array(Array &&) = delete;
    Array &operator=(const Array &other);
    Array &operator=(Array &&other);
    
    // Set all elements of the array to 0
    void clear();

    // Resize array size to specified parameter and clear new elements
    void resize(const std::size_t &size);

    // Insert data at specified index
    void insert(const T &val, const std::size_t &at);

    void push_back(const T &val);
    void push_front(const T &val);
    void pop_back();
    void pop_front();
    const std::size_t &size() const;
    bool contains(const T &val) const;
    T &operator[](const std::size_t &at);
    const T &operator[](const std::size_t &at) const;
    bool remove(const T &val);
    const bool empty() const;
};

// For template explicit instantiations
#include "Array.cpp"