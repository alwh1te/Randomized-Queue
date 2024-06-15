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

    template <bool IsConst>
    class iterator_impl {
        using queue_type = std::conditional_t<IsConst, const RandomizedQueue, RandomizedQueue>;
        using ref_type = std::conditional_t<IsConst, const_reference, reference>;
        using ptr_type = std::conditional_t<IsConst, const_pointer, pointer>;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = ptr_type;
        using reference = ref_type;

        iterator_impl() : m_queue(nullptr), m_pos(0) {}
        iterator_impl(queue_type* queue, size_type pos) : m_queue(queue), m_pos(pos) {
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

        iterator_impl& operator++() {
            ++m_pos;
            return *this;
        }

        iterator_impl operator++(int) {
            iterator_impl temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator_impl& other) const {
            return m_queue == other.m_queue && m_pos == other.m_pos;
        }

        bool operator!=(const iterator_impl& other) const {
            return !(*this == other);
        }

    private:
        queue_type* m_queue;
        size_type m_pos;
        std::vector<size_type> m_permuted_indices;
    };

public:
    using iterator = iterator_impl<false>;
    using const_iterator = iterator_impl<true>;

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
