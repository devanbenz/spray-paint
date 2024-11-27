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
    SprayPaintNode() = default;

    SprayPaintNode(const SprayPaintNode &spn) {
        weight_ = spn.weight_;
        value_ = spn.value_;
        leaf_ = spn.leaf_;
        if (spn.left_ != nullptr) {
            left_ = spn.left_->clone();
        } else {
            left_ = nullptr;
        }

        if (spn.right_ != nullptr) {
            right_ = spn.right_->clone();
        } else {
            right_ = nullptr;
        }
    }

    SprayPaintNode(SprayPaintNode&&) noexcept = default;
    SprayPaintNode& operator=(SprayPaintNode&&) noexcept = default;

    SprayPaintNode& operator=(const SprayPaintNode&) = delete;

    [[nodiscard]] bool leaf() const {
        return leaf_;
    };

    [[nodiscard]] int weight() const {
        return weight_;
    }

    [[nodiscard]] char value() const {
        return value_;
    }


    std::unique_ptr<SprayPaintNode> clone() {
        auto spn = SprayPaintNode{*this};
        return std::make_unique<SprayPaintNode>(spn);
    }

    std::unique_ptr<SprayPaintNode> left() {
        auto a = std::move(left_);
        return a;
    };

    std::unique_ptr<SprayPaintNode> right() {
        auto a = std::move(right_);
        return a;
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

    void serialize(std::ofstream& os);

    std::unique_ptr<SprayPaintNode> deserialize(std::ifstream& is);
protected:
    int weight_;

    bool leaf_;

    char value_;

    std::unique_ptr<SprayPaintNode> left_;

    std::unique_ptr<SprayPaintNode> right_;
};

class InternalNode : public SprayPaintNode {
public:
    explicit InternalNode(int weight, std::unique_ptr<SprayPaintNode> l, std::unique_ptr<SprayPaintNode> r) : SprayPaintNode(*this) {
        this->weight_ = weight;
        this->leaf_ = false;
        this->right_ = std::move(r);
        this->left_ = std::move(l);
    };
};

class LeafNode : public SprayPaintNode {
public:
    LeafNode(int weight, char value) : SprayPaintNode(*this) {
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

    SprayPaintTree(const SprayPaintTree& t) {
        this->root_ = t.root_->clone();
        this->charset_ = t.charset_;
    }

    SprayPaintTree(int weight, char val) {
        auto ln = LeafNode(weight, val);
        this->root_ = std::move(std::make_unique<LeafNode>(std::move(ln)));
    };

    SprayPaintTree(SprayPaintNode l, SprayPaintNode r) {
        int weight = l.weight() + r.weight();
        auto in = InternalNode(weight,
                               std::make_unique<SprayPaintNode>(l),
                               std::make_unique<SprayPaintNode>(r));

        this->root_ = std::move(std::make_unique<InternalNode>(std::move(in)));
    };


    // Explicitly default move operations
    SprayPaintTree(SprayPaintTree&&) noexcept = default;
    SprayPaintTree& operator=(SprayPaintTree&&) noexcept = default;

    // Prevent copying if not needed
    SprayPaintTree& operator=(const SprayPaintTree&) = delete;

    [[nodiscard]] std::unique_ptr<SprayPaintTree> clone() const {
        auto spt = SprayPaintTree{*this};
        return std::make_unique<SprayPaintTree>(spt);
    }

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

        return std::make_optional(*r);
    }

    // Reset will destroy the tree and completely reset it. This is destructive!
    void reset() {
        this->root_ = nullptr;
    };

    void build();

    void encode(const std::string&);

    void register_charset(std::unordered_map<char, int> charset){
        this->charset_.emplace(std::move(charset));
    };

    void serialize(std::ofstream& os);

    static SprayPaintTree deserialize(std::ifstream& is);
private:
    std::unique_ptr<SprayPaintNode> root_;

    std::optional<std::unordered_map<char, int>> charset_;
};

class SprayPaintFile {
private:

};