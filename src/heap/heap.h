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
class BaseHeap {
public:
    virtual void put(T value) = 0;

    virtual T pop() = 0;

    virtual bool is_leaf(int idx) = 0;

    virtual void sift_down(T value) = 0;
protected:
    std::optional<int> parent_node(int);

    std::optional<int> left_child_node(int, int);

    std::optional<int> right_child_node(int, int);
};

template <typename T>
std::optional<int> BaseHeap<T>::parent_node(int idx) {
    assert(idx >= 0);
    if (idx == 0) {
        return {};
    }
    return ((idx - 1)/2);
}

template <typename T>
std::optional<int> BaseHeap<T>::left_child_node(int idx, int cap) {
    assert(idx >= 0);
    if ((2 * idx) + 1 < cap) {
        return (2 * idx) + 1;
    }
    return {};
}

template <typename T>
std::optional<int> BaseHeap<T>::right_child_node(int idx, int cap) {
    assert(idx >= 0);
    return 0;
}
