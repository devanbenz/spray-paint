#pragma once

#include <vector>
#include <optional>
#include <cassert>

///The formulae for calculating the array indices of the various relatives of a node are as follows.
// The total number of nodes in the tree is n.
// The index of the node in question is r, which must fall in the range 0 to n−1

//Parent(r) =⌊(r−1)/2⌋ if r≠0
//Left child(r) =2r+1 if 2r+1<n
//Right child(r) =2r+2 if 2r+2<n
//Left sibling(r) =r−1 if r is even and r≠0
//Right sibling(r) =r+1; if r is odd and r+1<n

template <typename T>
concept Comparable = requires (T a, T b) {
    {a > b} -> std::convertible_to<bool>;
    {a < b} -> std::convertible_to<bool>;
    {a == b} -> std::convertible_to<bool>;
};

template <typename T>
requires Comparable<T>
class BaseHeap {
public:
    virtual void put(T value) = 0;

    virtual T pop() = 0;

    virtual bool is_leaf(int idx) = 0;
protected:
    std::optional<int> parent_node(int);

    std::optional<int> left_child_node(int, int);

    std::optional<int> right_child_node(int, int);

    std::optional<int> left_sibling_node(int, int);

    std::optional<int> right_sibling_node(int, int);
};

template <typename T>
requires Comparable<T>
std::optional<int> BaseHeap<T>::parent_node(int idx) {
    if (idx < 0) { throw std::runtime_error("index must be 0 or greater"); };
    if (idx == 0) {
        return {};
    }
    return ((idx - 1)/2);
}

template <typename T>
requires Comparable<T>
std::optional<int> BaseHeap<T>::left_child_node(int idx, int cap) {
    if (idx < 0) { throw std::runtime_error("index must be 0 or greater"); };
    if ((2 * idx) + 1 < cap) {
        return (2 * idx) + 1;
    }
    return {};
}

template <typename T>
requires Comparable<T>
std::optional<int> BaseHeap<T>::right_child_node(int idx, int cap) {
    if (idx < 0) { throw std::runtime_error("index must be 0 or greater"); };
    if ((2 * idx) + 2 < cap) {
        return (2 * idx) + 2;
    }
    return {};
}


//Left sibling(r) =r−1 if r is even and r≠0
template <typename T>
requires Comparable<T>
std::optional<int> BaseHeap<T>::left_sibling_node(int, int) {

}

//Right sibling(r) =r+1; if r is odd and r+1<n
template <typename T>
requires Comparable<T>
std::optional<int> BaseHeap<T>::right_sibling_node(int, int) {

}