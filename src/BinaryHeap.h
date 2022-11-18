#pragma once

#include <cstdio>
#include <functional>
#include <stdexcept>

template <typename T, typename K = std::less<T>>
class BinaryHeap {
public:
    T GetFront() const {
        if (size_ == 0) {
            throw std::out_of_range("error: heap is empty");
        }
        return data_[0];
    }

    void Push(const T& element) {
        if (size_ == capacity_) {
            size_t new_capacity = 1;
            if (capacity_ != 0) {
                new_capacity = capacity_ * 2;
            }

            T* new_data = new T[new_capacity];
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }

            if (capacity_ != 0) {
                delete[] data_;
            }
            data_ = new_data;
            capacity_ = new_capacity;
        }
        data_[size_] = element;

        size_t current_element = size_;
        while (current_element > 0 && K()(data_[current_element], data_[(current_element - 1) / 2])) {
            std::swap(data_[current_element], data_[(current_element - 1) / 2]);
            current_element = (current_element - 1) / 2;
        }

        ++size_;
    }

    void Pop() {
        std::swap(data_[0], data_[size_ - 1]);
        --size_;

        size_t current_element = 0;
        while (current_element * 2 + 1 < size_) {
            size_t left_index = current_element * 2 + 1;
            size_t right_index = left_index + 1;

            size_t next_element = left_index;
            if (right_index < size_ && K()(data_[right_index], data_[left_index])) {
                next_element = right_index;
            }

            if (K()(data_[current_element], data_[next_element])) {
                break;
            }
            std::swap(data_[current_element], data_[next_element]);
            current_element = next_element;
        }
    }

    size_t Size() const {
        return size_;
    }

    ~BinaryHeap() {
        if (size_ != 0) {
            delete[] data_;
        }
    }

private:
    size_t capacity_ = 0;
    size_t size_ = 0;
    T* data_;
};
