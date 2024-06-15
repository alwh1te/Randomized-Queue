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
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;


        iterator() : m_queue(nullptr), m_pos(0) {}
        iterator(RandomizedQueue* queue, size_type pos) : m_queue(queue), m_pos(pos) {
            if (queue) {
                m_permuted_indices = queue->get_permuted_indexes();
            }
        }

        reference operator*() const {
            return m_queue->m_data[m_permuted_indices[m_pos]];
        }

        pointer operator->() const {
            return &m_queue->m_data[m_permuted_indices[m_pos]];
        }

        iterator& operator++() {
            ++m_pos;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator& other) const {
            return m_queue == other.m_queue && m_pos == other.m_pos;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

    private:
        RandomizedQueue* m_queue;
        size_type m_pos;
        std::vector<size_type> m_permuted_indices;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;

        const_iterator() : m_queue(nullptr), m_pos(0) {}
        const_iterator(const RandomizedQueue* queue, size_type pos) : m_queue(queue), m_pos(pos) {
            if (queue) {
                m_permuted_indices = queue->get_permuted_indexes();
            }
        }

        const_reference operator*() const {
            return m_queue->m_data[m_permuted_indices[m_pos]];
        }

        const_pointer operator->() const {
            return &m_queue->m_data[m_permuted_indices[m_pos]];
        }

        const_iterator& operator++() {
            ++m_pos;
            return *this;
        }

        const_iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const const_iterator& other) const {
            return m_queue == other.m_queue && m_pos == other.m_pos;
        }

        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }

    private:
        const RandomizedQueue* m_queue;
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

    void enqueue(value_type&& item) {
        m_data.push_back(std::move(item));
    }

    value_type dequeue() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        std::uniform_int_distribution<> dist(0, m_data.size() - 1);
        int idx = dist(m_gen);
        value_type item = std::move(m_data[idx]);
        std::swap(m_data[idx], m_data.back());
        m_data.pop_back();
        return item;
    }

    reference sample() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        std::uniform_int_distribution<> dist(0, m_data.size() - 1);
        return m_data[dist(m_gen)];
    }

    const_reference sample() const {
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

    RandomizedQueue& operator+=(value_type&& item) {
        enqueue(std::move(item));
        return *this;
    }

    iterator begin() noexcept { return iterator(this, 0); }
    iterator end() noexcept { return iterator(this, m_data.size()); }
    const_iterator begin() const noexcept { return const_iterator(this, 0); }
    const_iterator end() const noexcept { return const_iterator(this, m_data.size()); }
    const_iterator cbegin() const noexcept { return const_iterator(this, 0); }
    const_iterator cend() const noexcept { return const_iterator(this, m_data.size()); }

private:
    std::vector<size_type> get_permuted_indexes() const noexcept {
        std::vector<size_type> indexes(m_data.size());
        std::iota(indexes.begin(), indexes.end(), 0);
        std::shuffle(indexes.begin(), indexes.end(), m_gen);
        return indexes;
    }

    std::vector<value_type> m_data;
    mutable std::mt19937 m_gen;
};
