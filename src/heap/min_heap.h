#include "heap.h"

#include <cassert>
#include <concepts>

template <typename T>
concept Comparable = requires (T a, T b) {
    {a > b} -> std::convertible_to<bool>;
    {a < b} -> std::convertible_to<bool>;
    {a == b} -> std::convertible_to<bool>;
};

template <typename T>
class MinHeap : public BaseHeap<T> {
public:
    explicit MinHeap(int cap);

    void put(T value) requires Comparable<T>;

    T pop() override;

    bool is_leaf(int idx) override;
private:
    int cap_;

    std::vector<T> heap_;

    void sift_down(T) requires Comparable<T>;

    void sift_up(T) requires Comparable<T>;
};

template <typename T>
MinHeap<T>::MinHeap(int cap): cap_(cap) {
    std::vector<T> vec(cap);
    heap_ = vec;
}

template <typename T>
void MinHeap<T>::put(T value) requires Comparable<T> {
    if (heap_.size() == cap_) {
        throw std::runtime_error("min_heap is already at max capacity");
    }

}

template <typename T>
T MinHeap<T>::pop() {
    return heap_.front();
}

template<typename T>
bool MinHeap<T>::is_leaf(int idx) {
    assert(idx >= 0);
    auto lc = this->left_child_node(idx, heap_.capacity());
    auto rc = this->right_child_node(idx, heap_.capacity());
    if (!rc.has_value() && !lc.has_value()) {
        return false;
    }

    return true;
}

template<typename T>
void MinHeap<T>::sift_up(T) requires Comparable<T> {

}

template<typename T>
void MinHeap<T>::sift_down(T) requires Comparable<T> {

}
