#include "heap.h"

#include <cassert>
#include <concepts>
#include <algorithm>

template <typename T>
requires Comparable<T>
class MinHeap : public BaseHeap<T> {
public:
    explicit MinHeap(int cap);

    void put(T value) override;

    T pop() override;

    bool is_leaf(int idx) override;
private:
    int cap_;

    std::vector<T> heap_;

    void sift_down(int, int);

    void sift_up(int, int);
};

template <typename T>
requires Comparable<T>
MinHeap<T>::MinHeap(int cap): cap_(cap) {
    std::vector<T> vec;
    heap_ = vec;
}

template <typename T>
requires Comparable<T>
void MinHeap<T>::put(T value) {
    if (heap_.size() == cap_) {
        throw std::runtime_error("min_heap is already at max capacity");
    }

    // If heap size is 0 then it is only a root node
    // we can insert a new value and then sift it up if needed
    if (heap_.size() == 0) {
        std::cout << "adding first value to heap\n";
        heap_.emplace_back(value);
        assert(heap_.size() == 1);
        return;
    }

    heap_.emplace_back(value);
    int cur_index = heap_.size();
    auto parent_index = this->parent_node(cur_index);

    if (parent_index.has_value()) {
        sift_up(parent_index.value(), cur_index);
    }
}

template <typename T>
requires Comparable<T>
T MinHeap<T>::pop() {
    return heap_.front();
}

template<typename T>
requires Comparable<T>
bool MinHeap<T>::is_leaf(int idx) {
    if (idx < 0) { throw std::runtime_error("index must be 0 or greater"); };
    auto lc = this->left_child_node(idx, heap_.capacity());
    auto rc = this->right_child_node(idx, heap_.capacity());
    if (!rc.has_value() && !lc.has_value()) {
        return false;
    }

    return true;
}

//* Sift up will move an element from the bottom node [last index]
// up the tree and swap its parent element out.*/
template<typename T>
requires Comparable<T>
void MinHeap<T>::sift_up(int parent, int cur_index) {
    if (heap_.size() < parent) { throw std::runtime_error("first index is larger then heap size"); };
    if (heap_.size() < cur_index) { throw std::runtime_error("second index is larger then heap size"); };

    while (heap_[parent] > heap_[cur_index]) {
        std::swap(heap_[parent], heap_[cur_index]);
        cur_index = parent;
        if (this->parent_node(parent).has_value()) {
            int val = this->parent_node(parent).value();
            parent = val;
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
void MinHeap<T>::sift_down(int a, int b) {
    if (heap_.size() < a) { throw std::runtime_error("first index is larger then heap size"); };
    if (heap_.size() < b) { throw std::runtime_error("second index is larger then heap size"); };
    std::swap(heap_[b], heap_[a]);
}
