#include "huffman.h"

std::unordered_map<char, int> build_char_map(std::ifstream& is) {
    std::unordered_map<char, int> char_map;
    char c;
    while (is.get(static_cast<char &>(c))) {
        auto it = char_map.find(c);
        if (it == char_map.end()) {
            char_map.insert(std::make_pair(c, 1));
        } else {
            it->second = ++it->second;
        }
    }

    if (char_map.empty()) {
        throw std::runtime_error("Input file is empty.");
    };

    return char_map;
}

void SprayPaintTree::build() {
    if (!this->charset_.has_value()) {
        throw std::runtime_error("charset is not registered; please use .register_charset() to register a character set for encoding.");
    }

    auto charset = this->charset_.value();
    auto min_heap = MinHeap<SprayPaintTree>(charset.size());

    /* First we will build the min heap
     * and then loop through all min_heap values
     * adding them to the max_heap. This will
     * ensure that not too much memory is used during compression.*/
    for (auto i : charset) {
        auto nt = SprayPaintTree(i.second, i.first);
        min_heap.put(nt);
    }

    /* For now I will assume that there are always two pop'ed variables available
     * I will adjust if needed to account for odd numbered heaps.*/
    while (min_heap.size() > 1) {
        auto tmp1 = min_heap.pop().root().value();
        auto tmp2 = min_heap.pop().root().value();
        auto tmp3 = SprayPaintTree(std::move(tmp1), std::move(tmp2));
        min_heap.put(std::move(tmp3));
    }
    auto tree = std::move(min_heap.pop().root_);
    this->root_.swap(tree);
}

void SprayPaintNode::serialize(std::ofstream& os) {
    os.write(reinterpret_cast<char*>(&this->weight_), sizeof(this->weight_));
    os.write(reinterpret_cast<char*>(&this->value_), sizeof(this->value_));
    os.write(reinterpret_cast<char*>(&this->leaf_), sizeof(this->leaf_));

    bool has_left = (left_ != nullptr);
    bool has_right = (right_ != nullptr);
    os.write(reinterpret_cast<const char*>(&has_left), sizeof(has_left));
    os.write(reinterpret_cast<const char*>(&has_right), sizeof(has_right));

    if (has_left) left_->serialize(os);
    if (has_right) right_->serialize(os);
}

void SprayPaintTree::serialize(std::ofstream& os) {
    this->root_->serialize(os);
}

std::unique_ptr<SprayPaintNode> SprayPaintNode::deserialize(std::ifstream& in) {
    auto node = std::make_unique<SprayPaintNode>();

    in.read(reinterpret_cast<char*>(&node->weight_), sizeof(node->weight_));
    in.read(reinterpret_cast<char*>(&node->value_), sizeof(node->value_));
    in.read(reinterpret_cast<char*>(&node->leaf_), sizeof(node->leaf_));

    bool has_left = false, has_right = false;
    in.read(reinterpret_cast<char*>(&has_left), sizeof(has_left));
    in.read(reinterpret_cast<char*>(&has_right), sizeof(has_right));

    if (has_left) node->left_ = deserialize(in);
    if (has_right) node->right_ = deserialize(in);

    return node;
}

SprayPaintTree SprayPaintTree::deserialize(std::ifstream& in) {
    SprayPaintTree spray_paint_tree;
    auto root = std::make_unique<SprayPaintNode>();
    spray_paint_tree.root_ = root->deserialize(in);
    return spray_paint_tree;
}

void generate_encodings(SprayPaintNode* node, std::unordered_map<char, std::vector<int>>& map, std::vector<int> bits) {
    if (node == nullptr) {
        return;
    }

    if (node->leaf()) {
        map[node->value()] = bits;
        return;
    }

    bits.push_back(0);
    generate_encodings(node->left_ref(), map, bits);

    bits.back() = 1;
    generate_encodings(node->right_ref(), map, bits);
}

/* To encode data depth first search will be used
 * to generate a string of 0's and 1's associated with
 * each individual character. This may be a lot of work :thinking:
 * It's currently my best idea though.*/
std::unordered_map<char, std::vector<int>> SprayPaintTree::encode() {
    if (!this->root().has_value()) {
        throw std::runtime_error("There is not root value. Please build a huffman code tree using build() before trying to encode data.");
    }
    std::unordered_map<char, std::vector<int>> ret;
    std::vector<int> bits;
    auto cloned_tree = this->clone();
    generate_encodings(cloned_tree->root_ref(), ret, bits);

    return ret;
}

void SprayPaintFile::write() {
    std::ifstream input(this->input_file_name_);
    std::ofstream output(this->out_file_name_, std::ios::binary);
    std::vector<int> write_buffer;
    char write_char = 0;

    auto cm = build_char_map(input);
    this->header_.register_charset(std::move(cm));
    this->header_.build();
    auto encoder_lut = this->header_.encode();

    // Write the header first
    this->header_.serialize(output);
    output.clear();
    output.seekp(sizeof(this->header_), std::ios::beg);

    input.clear();
    input.seekg(0, std::ios::beg);

    char c;
    while (input.get(c)) {
        auto v = encoder_lut.find(c);
        if (v == encoder_lut.end()) {
            throw std::runtime_error("Character was not found, there is a problem with the encoder.");
        }

        for (auto bit : v->second) {
            // The write buffer can be flushed
            // a byte can be written to the file.
            if (write_buffer.size() == 8) {
                for (int i = 0; i < 8 && i < write_buffer.size(); ++i) {
                    write_char |= (write_buffer[i] << (7 - i));
                }

                output << write_char;

                // Reset buffer and char
                write_char = 0;
                write_buffer.clear();
            }

            write_buffer.push_back(bit);
        }
    }

    // If our write buffer is at a bad offset (i.e. less then 8 bits)
    // We will need to do a second loop through to pad the char
    // This padding offset will be in the metadata for our file that we write
    if (!write_buffer.empty()) {
        int offset = 0;
        for (int i = 0; i < 8; ++i) {
            if ((i + 1) > write_buffer.size()) {
                ++offset;
                write_char |= (0 << (7 - i));
            } else {
                write_char |= (write_buffer[i] << (7 - i));
            }
        }
        output << write_char;

        // Write the offset as the last byte of the file
        // This will be used when reading in the file
        write_char = offset;
        output << write_char;
    }

    input.close();
    output.close();
}

void SprayPaintFile::read() {
    this->header_.reset();

    std::ifstream input(this->input_file_name_);

    this->header_ = SprayPaintTree::deserialize(input);
    auto head = std::make_unique<SprayPaintNode>(this->header_.clone()->root().value());
    auto tail = std::make_unique<SprayPaintNode>(this->header_.clone()->root().value());

    // get length of file:
    input.seekg (0, input.end);
    int length = input.tellg();

    // Need to read offset bit
    char end_bit;
    input.seekg(-1, std::ios::end);
    input.get(end_bit);

    auto tree_size = sizeof(this->header_);
    input.seekg(tree_size, std::ios::beg);

    std::vector<char> tmp_chars;

    char c = 0;
    int curr_byte = tree_size;
    while (true) {
        // Increment current byte
        input.get(c);
        ++curr_byte;
        input.seekg(curr_byte, std::ios::beg);
        std::bitset<8> as_bits(c);

        if ((curr_byte - length) > (length - 1)) {
            break;
        } else if ((curr_byte - (length - 1)) == 0) {
            for (int i = as_bits.size() - 1; i >= (0 + (int)end_bit); --i) {
                if (as_bits[i] == 0) {
                    if (tail->left_ref()->leaf()) {
                        tmp_chars.push_back(tail->left_ref()->value());
                        tail = head->clone();
                        continue;
                    } else {
                        tail = tail->left();
                        continue;
                    }
                } else if (as_bits[i] == 1) {
                    if (tail->right_ref()->leaf()) {
                        tmp_chars.push_back(tail->right_ref()->value());
                        tail = head->clone();
                        continue;
                    } else {
                        tail = tail->right();
                        continue;
                    }
                }
            }
            break;
        }
        // Extracting individual bits from a byte
        // Char is a byte-sized type so it has 8 bits
        for (int i = as_bits.size() - 1; i >= 0; --i) {
            if (as_bits[i] == 0) {
                if (tail->left_ref()->leaf()) {
                    tmp_chars.push_back(tail->left_ref()->value());
                    tail = head->clone();
                    continue;
                } else {
                    tail = tail->left();
                    continue;
                }
            } else if (as_bits[i] == 1) {
                if (tail->right_ref()->leaf()) {
                    tmp_chars.push_back(tail->right_ref()->value());
                    tail = head->clone();
                    continue;
                } else {
                    tail = tail->right();
                    continue;
                }
            }
        }
    }

    std::cout << "\n";
    input.close();
}
