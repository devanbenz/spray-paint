#include "heap.h"

#include <cassert>
#include <concepts>
#include <algorithm>
#include <iostream>

template <typename T>
requires Comparable<T>
class MaxHeap : public BaseHeap<T> {
public:
    explicit MaxHeap(int cap);

    void put(T value) override;

    T pop() override;

    T pop_min();

    bool is_leaf(int idx) override;

    T root();

    [[nodiscard]] int size() const;

    void display();
private:
    int cap_;

    std::vector<T> heap_;

    void sift_down(int, int);

    void sift_up(int, int);
};

template<typename T>
requires Comparable<T>
T MaxHeap<T>::root() {
    if (heap_.size() == 0) {
        return nullptr;
    }
    return heap_[0];
}

template <typename T>
requires Comparable<T>
void MaxHeap<T>::display() {
    for (int i = 0; i < this->heap_.size(); i++) {
        std::cout << this->heap_[i] << "; ";
        if (this->parent_node(i).has_value()) {
            int pn = this->parent_node(i).value();
            std::cout << "parent=" << this->heap_[pn] << "; ";
        }
        if (this->left_child_node(i, cap_).has_value()) {
            int lc = this->left_child_node(i, cap_).value();
            std::cout << "left-child=" << this->heap_[lc] << "; ";
        }
        if (this->right_child_node(i, cap_).has_value()) {
            int rc = this->right_child_node(i, cap_).value();
            std::cout << "right-child=" << this->heap_[rc] << "; ";
        }
        std::cout << std::endl;
    }
}

template <typename T>
requires Comparable<T>
MaxHeap<T>::MaxHeap(int cap): cap_(cap) {
    std::vector<T> vec;
    heap_ = std::move(vec);
}

template <typename T>
requires Comparable<T>
int MaxHeap<T>::size() const {
    return this->heap_.size();
}

template <typename T>
requires Comparable<T>
void MaxHeap<T>::put(T value) {
    if (heap_.size() == cap_) {
        throw std::runtime_error("min_heap is already at max capacity");
    }

    // If heap size is 0 then it is only a root node
    // we can insert a new value and then sift it up if needed
    if (heap_.size() == 0) {
        heap_.emplace_back(std::move(value));
        assert(heap_.size() == 1);
        return;
    }

    heap_.emplace_back(std::move(value));
    int cur_index = heap_.size() - 1;
    auto parent_index = this->parent_node(cur_index);

    if (parent_index.has_value()) {
        sift_up(cur_index, parent_index.value());
    }
}

template <typename T>
requires Comparable<T>
T MaxHeap<T>::pop() {
    // Need to swap the 0th element with the last element
    if (heap_.size() <= 0) {
        throw std::runtime_error("there is no root node");
    }
    if (heap_.size() == 1) {
        auto root = std::move(heap_.back());
        heap_.pop_back();
        return root;
    }
    std::swap(heap_[0], heap_[heap_.size() - 1]);
    auto lowest_elem = std::move(heap_.back());
    heap_.pop_back();

    auto left_child_node = this->left_child_node(0, heap_.size());
    auto right_child_node = this->right_child_node(0, heap_.size());
    if (left_child_node.has_value() && heap_[0] < heap_[left_child_node.value()]) {
        sift_down(0, left_child_node.value());
    } else if (right_child_node.has_value() && heap_[0] < heap_[right_child_node.value()]) {
        sift_down(0, right_child_node.value());
    }

    return lowest_elem;
}

template <typename T>
requires Comparable<T>
T MaxHeap<T>::pop_min() {
    // Need to swap the 0th element with the last element
    if (heap_.size() <= 0) {
        throw std::runtime_error("there is no root node");
    }
    if (heap_.size() == 1) {
        auto root = std::move(heap_.back());
        heap_.pop_back();
        return root;
    }
    auto lowest_elem = std::move(heap_.back());
    heap_.pop_back();

    return lowest_elem;
}

template<typename T>
requires Comparable<T>
bool MaxHeap<T>::is_leaf(int idx) {
    if (idx < 0) { throw std::runtime_error("index must be 0 or greater"); };
    auto lc = this->left_child_node(idx, heap_.capacity());
    auto rc = this->right_child_node(idx, heap_.capacity());
    if (!rc.has_value() && !lc.has_value()) {
        return false;
    }

    return true;
}

//* Sift up will move an element from the bottom node [last index]
// up the tree and swap its idx1 element out.*/
template<typename T>
requires Comparable<T>
void MaxHeap<T>::sift_up(int idx1, int idx2) {
    if (heap_.size() < idx1) { throw std::runtime_error("first index is larger then heap size"); };
    if (heap_.size() < idx2) { throw std::runtime_error("second index is larger then heap size"); };

    while (heap_[idx1] > heap_[idx2]) {
        std::swap(heap_[idx1], heap_[idx2]);
        idx1 = idx2;
        if (this->parent_node(idx1).has_value()) {
            int val = this->parent_node(idx1).value();
            idx2 = val;
        }
    }
}
//* Sift down will be used when pop'ing the smallest element out
// of the min heap. When an element is pop'ed the root node [first index]
// is swapped with the last node. After swapping the new root is
// sifted down the tree.
// */
template<typename T>
requires Comparable<T>
void MaxHeap<T>::sift_down(int root, int lowest_leaf) {
    if (heap_.size() < root) { throw std::runtime_error("first index is larger then heap size"); };
    if (heap_.size() < lowest_leaf) { throw std::runtime_error("second index is larger then heap size"); };
    std::swap(heap_[root], heap_[lowest_leaf]);

    while (heap_[root] < heap_[lowest_leaf]) {
        std::swap(heap_[root], heap_[lowest_leaf]);
        root = lowest_leaf;
        if (this->right_child_node(root, heap_.size()).has_value()
            && this->right_child_node(root, heap_.size()).value() > root) {
            int val = this->right_child_node(root, heap_.size()).value();
            lowest_leaf = val;
        }else if (this->left_child_node(root, heap_.size()).has_value()
                  && this->left_child_node(root, heap_.size()).value() > root) {
            int val = this->left_child_node(root, heap_.size()).value();
            lowest_leaf = val;
        }
    }
}
