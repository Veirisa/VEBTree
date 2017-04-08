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
    static const ull MAX_VALUE = (S == 64 ? NO : (ull)1 << S) - 1;
    typedef std::unique_ptr<VEBTree<HALF>> veb_ptr;

    mutable std::unordered_map<ull, veb_ptr> children;
    veb_ptr existing;
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

    ~VEBTree() {};

    inline bool is_empty() const {
        return tree_min == NO;
    }

    void add(ull x) {
        assert(x <= MAX_VALUE);
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
            children[high_bits] = veb_ptr(new VEBTree<HALF>());
            if (existing == nullptr) {
                existing.reset(new VEBTree<HALF>());
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
        ull next_high = existing->next(high_bits);
        if (next_high == NO) {
            return tree_max;
        }
        return create_number(next_high, children[next_high]->getMin());
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
        ull prev_high = existing->prev(high_bits);
        if (prev_high == NO) {
            return tree_min;
        }
        return create_number(prev_high, children[prev_high]->getMax());

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

