#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

template<class T>
class RandomizedQueue {
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using iterator_category = std::forward_iterator_tag;

    class iterator {
    public:
        iterator() : m_queue(nullptr), m_pos(0) {};
        iterator(const RandomizedQueue *queue, size_type pos) : m_queue(queue), m_pos(pos) {
            if (queue) {
                m_permuted_indices = queue->get_permuted_indexes();
            }
        }

        const value_type &operator*() {
            return m_queue->m_data[m_permuted_indices[m_pos]];
        }

        const value_type *operator->() {
            return &m_queue->m_data[m_permuted_indices[m_pos]];
        }

        iterator& operator+=(const value_type &value) {
            m_queue->m_data[m_permuted_indices[m_pos]] += value;
            return *this;
        }

        iterator& operator*= (difference_type n) {
            m_pos *= n;
            return *this;
        }

        iterator &operator++() {
            ++m_pos;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator &other) const {
            return m_queue == other.m_queue && m_pos == other.m_pos;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

    private:
        const RandomizedQueue *m_queue;
        size_type m_pos;
        std::vector<size_type> m_permuted_indices;
    };


public:
    RandomizedQueue() : m_gen(std::random_device{}()) {}

    bool empty() const noexcept {
        return m_data.empty();
    }

    size_type size() const noexcept {
        return m_data.size();
    }

    void enqueue(const_reference item) {
        m_data.push_back(item);
    }

    value_type dequeue() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        std::uniform_int_distribution<> dist(0, m_data.size() - 1);
        int idx = dist(m_gen);
        T item = m_data[idx];
        std::swap(m_data[idx], m_data.back());
        m_data.pop_back();
        return item;
    }

    value_type sample() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        std::uniform_int_distribution<> dist(0, m_data.size() - 1);
        return m_data[dist(m_gen)];
    }

    RandomizedQueue& operator+=(const_reference item) {
        enqueue(item);
        return *this;
    }

    iterator begin() const noexcept;
    iterator end() const noexcept;
    iterator cbegin() const noexcept;
    iterator cend() const noexcept;

private:
    std::vector<size_type> get_permuted_indexes() const noexcept;
    std::vector<value_type> m_data;
    mutable std::mt19937 m_gen;
};

template<typename T>
std::vector<size_t> RandomizedQueue<T>::get_permuted_indexes() const noexcept {
    std::vector<size_t> indexes(m_data.size());
    std::iota(indexes.begin(), indexes.end(), 0);
    std::shuffle(indexes.begin(), indexes.end(), m_gen);
    return indexes;
}

template<typename T>
typename RandomizedQueue<T>::iterator RandomizedQueue<T>::begin() const noexcept {
    return iterator(this, 0);
}

template<typename T>
typename RandomizedQueue<T>::iterator RandomizedQueue<T>::end() const noexcept {
    return iterator(this, m_data.size());
}

template<typename T>
typename RandomizedQueue<T>::iterator RandomizedQueue<T>::cbegin() const noexcept {
    return iterator(this, 0);
}

template<typename T>
typename RandomizedQueue<T>::iterator RandomizedQueue<T>::cend() const noexcept {
    return iterator(this, m_data.size());
}