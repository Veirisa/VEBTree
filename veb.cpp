#include <utility>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <iostream>
#include <assert.h>
#include <unordered_map>
#include "veb.h"

template<unsigned int S>
class VEBTree: AbstractVEBTree<S> {
private:

    static const ull HALF = S / 2;
    static const ull DEG_HALF = (ull)1 << HALF;

    mutable std::unordered_map<ull, VEBTree<HALF>*> children;
    VEBTree<HALF>* existing;
    ull tree_min;
    ull tree_max;

    inline ull high(const ull& x) const {
        return x >> HALF;
    }

    inline ull low(const ull& x) const {
        return x & (DEG_HALF - 1);
    }

    inline ull create_number(const ull& high, const ull& low) const {
        return (high << HALF) | low;
    }

    inline bool existing_is_null_or_empty() const {
        return existing == nullptr || existing->is_empty();
    }

    inline bool child_in_map(const ull& key) const {
        return children.find(key) != children.cend();
    }

public:

    VEBTree() : tree_min(NO), tree_max(), existing(nullptr), children() {};

    ~VEBTree() {
        if (existing != nullptr) {
            for (auto it = children.cbegin(); it != children.cend(); ++it) {
                delete it->second;
            }
            delete existing;
        }
    }

    inline bool is_empty() const {
        return tree_min == NO;
    }

    void add(ull x) {
        assert(x != NO);
        if (is_empty()) {
            tree_min = x;
            tree_max = x;
            return;
        }
        if (x == tree_min || x == tree_max) {
            return;
        }
        if (tree_min == tree_max) {
            if (tree_min > x) {
                tree_min = x;
            } else {
                tree_max = x;
            }
            return;
        }
        if (tree_min > x) {
            std::swap(tree_min, x);
        }
        if (tree_max < x) {
            std::swap(tree_max, x);
        }
        ull high_bits = high(x);
        if (!child_in_map(high_bits)) {
            children[high_bits] = new VEBTree<HALF>();
            if (existing == nullptr) {
                existing = new VEBTree<HALF>();
            }
        }
        if (children[high_bits]->is_empty()) {
            existing->add(high_bits);
        }
        children[high_bits]->add(low(x));
    }

    void remove(ull  x) {
        bool no_existing = existing_is_null_or_empty();
        if (tree_min == x) {
            if (tree_max == x) {
                tree_min = NO;
                return;
            }
            if (no_existing) {
                tree_min = tree_max;
                return;
            }
            x = create_number(existing->getMin(), children[existing->getMin()]->getMin());
            tree_min = x;
        }
        if (tree_max == x) {
            if (tree_min == x) {
                tree_min = NO;
                return;
            }
            if (no_existing) {
                tree_max = tree_min;
                return;
            }
            x = create_number(existing->getMax(), children[existing->getMax()]->getMax());
            tree_max = x;
        }
        if (no_existing) {
            return;
        }
        ull high_bits = high(x);
        if (child_in_map(high_bits)) {
            children[high_bits]->remove(low(x));
            if (children[high_bits]->is_empty()) {
                delete children[high_bits];
                children.erase(high_bits);
                existing->remove(high_bits);
            }
        }
    }

    ull next(ull  x) const {
        if (is_empty() || tree_max <= x) {
            return NO;
        }
        if (tree_min > x) {
            return tree_min;
        }
        if (existing_is_null_or_empty()) {
            return tree_max;
        }
        ull high_bits = high(x);
        ull low_bits = low(x);
        if (child_in_map(high_bits) && !children[high_bits]->is_empty() &&
            children[high_bits]->getMax() > low_bits) {
            return create_number(high_bits, children[high_bits]->next(low_bits));
        }
        ull nextHigh = existing->next(high_bits);
        if (nextHigh == NO) {
            return tree_max;
        }
        return create_number(nextHigh, children[nextHigh]->getMin());
    }

    ull prev(ull  x) const {
        if (is_empty() || tree_min >= x) {
            return NO;
        }
        if (tree_max < x) {
            return tree_max;
        }
        if (existing_is_null_or_empty()) {
            return tree_min;
        }
        ull high_bits = high(x);
        ull low_bits = low(x);
        if (child_in_map(high_bits) && !children[high_bits]->is_empty() &&
            children[high_bits]->getMin() < low_bits) {
            return create_number(high_bits, children[high_bits]->prev(low_bits));
        }
        ull prevHigh = existing->prev(high_bits);
        if (prevHigh == NO) {
            return tree_min;
        }
        return create_number(prevHigh, children[prevHigh]->getMax());

    }

    ull getMin() const {
        assert(!is_empty());
        return tree_min;
    }

    ull getMax() const {
        assert(!is_empty());
        return tree_max;
    }
};

