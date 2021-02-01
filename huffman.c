#include "huffman.h"

struct HuffmanNode {
    bool is_leaf;
    ssize_t frequency;
    // Either a character if it's a leaf or two huffman nodes
    union {
        char c;
        struct HuffmanNodeTwo *two;
    } u;
};

struct HuffmanNodeTwo {
    struct HuffmanNode left;
    struct HuffmanNode right;
};

String generate_huffman_tree(ssize_t *frequencies) {
    // Huffman tree storage:
    //  For leaf nodes: 1-bit + character
    //  Otherwise: 0-bit + left + right
    //
    // Huffman tree generation:
    // while there are several trees:
    //  Find the two minimal trees
    //  Make a bigger tree out of them

    String result;
    struct HuffmanNode *nodes = malloc(255 * sizeof(struct HuffmanNode));

    // make a node for each non-zero frequency
    ssize_t nodes_len = 0;
    char i = '\0';
    while (true) {
        if (frequencies[i] != 0) {
            nodes[nodes_len].is_leaf = true;
            nodes[nodes_len].frequency = frequencies[i];
            nodes[nodes_len].u.c = i;
            ++nodes_len;
        }
        // increase i until we loop back to \0
        if (++i == '\0') {
            break;
        }
    }

    // join 2 least frequent nodes until there is only one
    while (nodes_len > 1) {
        struct HuffmanNode *minimal = &nodes[0];
        struct HuffmanNode *next_minimal = &nodes[1];

        if (next_minimal->frequency < minimal->frequency) {
            struct HuffmanNode *temp = minimal;
            minimal = next_minimal;
            next_minimal = temp;
        }
        // find minimal and next_minimal
        for (ssize_t i = 2; i < nodes_len; ++i) {
            ssize_t freq = nodes[i].frequency;
            if (freq < minimal->frequency) {
                next_minimal = minimal;
                minimal = &nodes[i];
            } else if (freq < next_minimal->frequency) {
                next_minimal = &nodes[i];
            }
        }

        // make a new node with minimal and next_minimal
        struct HuffmanNode new_node;
        new_node.frequency = minimal->frequency + next_minimal->frequency;
        new_node.is_leaf = false;
        new_node.u.two->left = *minimal;
        new_node.u.two->right = *next_minimal;
        // delete minimal and next_minimal
        if (next_minimal > minimal) {
            // delete next_minimal, then put new node in minimal
            memmove(next_minimal, next_minimal + 1, &nodes[nodes_len] - next_minimal);
            *minimal = new_node;
        } else {
            // delete minimal, then put new node in next_minimal
            memmove(minimal, minimal + 1, &nodes[nodes_len] - minimal);
            *next_minimal = new_node;
        }
        --nodes_len;
    }

    // nodes[0] is now the tree. make a binary representation of it
    result.length = 0;
    result.s = malloc(0);

    struct HuffmanNode *parents = malloc(nodes_len * sizeof(struct HuffmanNode));
    ssize_t parents_depth = 0;

    free(nodes);
    return result;
}
