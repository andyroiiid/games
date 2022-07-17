//
// Created by Andrew Huang on 7/10/2022.
//

#ifndef GAMES_MOVABLE_H
#define GAMES_MOVABLE_H

#include <utility>

#include "non_copyable.h"

template<typename T>
class Movable : NonCopyable {
public:
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_trivially_copy_constructible_v<T>);
    static_assert(std::is_trivially_copy_assignable_v<T>);
    static_assert(std::is_trivially_move_constructible_v<T>);
    static_assert(std::is_trivially_move_assignable_v<T>);
    static_assert(std::is_swappable_v<T>);

    Movable() = default;

    Movable(const T &val) noexcept { // NOLINT(google-explicit-constructor)
        m_val = val;
    }

    Movable(Movable &&other) noexcept {
        m_val = std::exchange(other.m_val, T{});
    }

    Movable &operator=(Movable &&other) noexcept {
        std::swap(m_val, other.m_val);
        return *this;
    }

    operator const T &() const { // NOLINT(google-explicit-constructor)
        return m_val;
    }

    Movable &operator=(const T &val) {
        m_val = val;
        return *this;
    }

    // Movable<T> has the same memory layout as T
    // So it's perfectly safe to overload operator&
    T *operator&() { // NOLINT(google-runtime-operator)
        return &m_val;
    }

    T *operator->() {
        return &m_val;
    }

private:
    T m_val{};
};

static_assert(sizeof(int) == sizeof(Movable<int>));

#endif //GAMES_MOVABLE_H
