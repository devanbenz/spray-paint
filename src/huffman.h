#pragma once

#include "heap/max_heap.h"
#include "heap/min_heap.h"

#include <unordered_map>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <utility>

std::unordered_map<char, int> build_char_map(std::ifstream);


class SprayPaintNode {
public:
    [[nodiscard]] bool leaf() const {
        return leaf_;
    };

    [[nodiscard]] int weight() const {
        return weight_;
    }

    [[nodiscard]] char value() const {
        return value_;
    }

    [[nodiscard]] SprayPaintNode* left() const {
        return left_.get();
    };

    [[nodiscard]] SprayPaintNode* right() const {
        return right_.get();
    };

    bool operator>(const SprayPaintNode& cmp) const {
        return this->weight_ > cmp.weight_;
    }

    bool operator<(const SprayPaintNode& cmp) const {
        return this->weight_ < cmp.weight_;
    }

    bool operator>=(const SprayPaintNode& cmp) const {
        return this->weight_ >= cmp.weight_;
    }

    bool operator<=(const SprayPaintNode& cmp) const {
        return this->weight_ <= cmp.weight_;
    }

    bool operator==(const SprayPaintNode& cmp) const {
        return this->weight_ == cmp.weight_;
    }

protected:
    int weight_;

    bool leaf_;

    char value_;

    std::unique_ptr<SprayPaintNode> left_;

    std::unique_ptr<SprayPaintNode> right_;
};

class InternalNode : public SprayPaintNode {
public:
    explicit InternalNode(int weight, std::unique_ptr<SprayPaintNode> l, std::unique_ptr<SprayPaintNode> r) : SprayPaintNode() {
        this->weight_ = weight;
        this->leaf_ = false;
        this->right_ = std::move(r);
        this->left_ = std::move(l);
    };

    SprayPaintNode* left() {
        return left_.get();
    }

    SprayPaintNode* right() {
        return right_.get();
    }
};

class LeafNode : public SprayPaintNode {
public:
    LeafNode(int weight, char value) : SprayPaintNode() {
        this->weight_ = weight;
        this->leaf_ = true;
        this->value_ = value;
    };
};

class SprayPaintTree {
public:
    SprayPaintTree() {
        this->root_ = nullptr;
    };

    SprayPaintTree(int weight, char val) {
        auto ln = LeafNode(weight, val);
        this->root_ = std::make_unique<LeafNode>(std::move(ln));
    };

    SprayPaintTree(SprayPaintNode l, SprayPaintNode r) {
        int weight = l.weight() + r.weight();
        auto in = InternalNode(weight,
                               std::make_unique<SprayPaintNode>(std::move(l)),
                               std::make_unique<SprayPaintNode>(std::move(r)));

        this->root_ = std::make_unique<InternalNode>(std::move(in));
    };

    friend std::ostream& operator<<(std::ostream& stream, const SprayPaintTree& o) {
        stream << "weight=" << o.root_->weight() << " value=" << o.root_->value();
        return stream;
    }

    bool operator>(const SprayPaintTree& cmp) const {
        if (this->root_ && cmp.root_) {
            return (this->root_->weight()) > (cmp.root_->weight());
        }
        return this->root_ != nullptr;
    }

    bool operator>=(const SprayPaintTree& cmp) const {
        if (this->root_ && cmp.root_) {
            return (this->root_->weight()) >= (cmp.root_->weight());
        }
        return this->root_ != nullptr;
    }

    bool operator<(const SprayPaintTree& cmp) const {
        if (this->root_ && cmp.root_) {
            return (this->root_->weight()) < (cmp.root_->weight());
        }
        return cmp.root_ != nullptr;
    }

    bool operator<=(const SprayPaintTree& cmp) const {
        if (this->root_ && cmp.root_) {
            return (this->root_->weight()) <= (cmp.root_->weight());
        }
        return cmp.root_ != nullptr;
    }

    bool operator==(const SprayPaintTree& cmp) const {
        if (this->root_ && cmp.root_) {
            return (this->root_->weight()) == (cmp.root_->weight());
        }
        return this->root_ == nullptr && cmp.root_ == nullptr;
    }

    std::optional<SprayPaintNode> root() {
        auto r = this->root_.get();
        if (r == nullptr) {
            return {};
        }

        return std::make_optional(std::move(*r));
    }

    // Reset will destroy the tree and completely reset it. This is destructive!
    void reset() {
        this->root_ = nullptr;
    };

    SprayPaintTree build();


    void register_charset(std::unordered_map<char, int> charset){
        this->charset_.emplace(std::move(charset));
    };
private:
    std::unique_ptr<SprayPaintNode> root_;

    std::optional<std::unordered_map<char, int>> charset_;
};