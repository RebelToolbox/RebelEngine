// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef PAIR_H
#define PAIR_H

template <class F, class S>
struct Pair {
    F first;
    S second;

    Pair() : first(), second() {}

    Pair(F p_first, const S& p_second) : first(p_first), second(p_second) {}
};

template <class F, class S>
bool operator==(const Pair<F, S>& pair, const Pair<F, S>& other) {
    return (pair.first == other.first) && (pair.second == other.second);
}

template <class F, class S>
bool operator!=(const Pair<F, S>& pair, const Pair<F, S>& other) {
    return (pair.first != other.first) || (pair.second != other.second);
}

template <class F, class S>
struct PairSort {
    bool operator()(const Pair<F, S>& A, const Pair<F, S>& B) const {
        return A.first < B.first;
    }
};

#endif // PAIR_H
