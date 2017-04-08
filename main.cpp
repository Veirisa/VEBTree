#include <iostream>
#include "veb.cpp"

template <unsigned int S>
void write_tree_next(const VEBTree<S>& tree) {
    std::cout << "next: ";
    for (ull i = tree.getMin(); i < NO; i = tree.next(i)) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

template <unsigned int S>
void write_tree_prev(const VEBTree<S>& tree) {
    std::cout << "prev: ";
    for (ull i = tree.getMax(); i < NO; i = tree.prev(i)) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

int main() {
    VEBTree<9> tree;
    for (ull i = 100; i > 0; --i) {
        tree.add(i * 2 - 1);
    }
    write_tree_next(tree);
    // elements: 1 3 5 ... 199
    for (ull i = 0; i <= 100; ++i) {
        tree.add(i * 2);
    }
    write_tree_prev(tree);
    // elements: 0 ... 200
    for (ull i = 45; i <= 55; ++i) {
        tree.add(i);
    }
    for (ull i = 50; i < 150; ++i) {
        tree.remove(i);
    }
    write_tree_next(tree);
    // elements: 0 ... 49 150 ... 200
    tree.remove(0);
    tree.remove(100);
    tree.remove(150);
    tree.remove(200);
    write_tree_prev(tree);
    // elements: 2 .. 49 151 .. 198
    std::cout << "(tree) min: " << tree.getMin() << " max: " << tree.getMax();

    //fast constructor and destructor:
    VEBTree<64> big_tree;
    ull element = 0;
    ull add_value = 1000000000000000;
    for (size_t i = 0; i <= 10000; ++i) {
        big_tree.add(element);
        element += add_value;
    }
    std::cout << "\n(big_tree) min: " << big_tree.getMin() << " max: " << big_tree.getMax();
    std::cout << "\n";
    return 0;
}