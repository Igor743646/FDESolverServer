#pragma once
#include <cstdint>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <map>
#include <numbers>
#include <numeric>
#include <unordered_map>
#include <ranges>
#include <random>
#include <cassert>
#include <optional>

#if defined(__GNUC__)
    #define Y_UNLIKELY(exp) __builtin_expect(!!(exp), 0)
#else
    #define Y_UNLIKELY(exp) (exp)
#endif

using i8  = int8_t;
using ui8 = uint8_t;
using i16  = int16_t;
using ui16 = uint16_t;
using i32  = int32_t;
using ui32 = uint32_t;
using i64  = int64_t;
using ui64 = uint64_t;
using usize = size_t;
using f32 = float;
using f64 = double;

constexpr f64 EPSILON = static_cast<f64>(0.00000001);

namespace NFunctions {
    f64 Gamma(f64);
    f64 Beta(f64, f64);
}

namespace std {
    template<class T, class U>
    std::ostream& operator<<(std::ostream& out, const std::unordered_map<T, U>& map) {
        out << "{\n";

        for (auto&& [k, v] : map) {
            out << "\t" << k << ": " << v << '\n';
        }

        out << "}";
        return out;
    }

    template<class T>
    std::ostream& operator<<(std::ostream& out, const std::vector<T>& map) {
        out << "[ ";

        for (auto& v : map) {
            out << v << " ";
        }

        out << "]";
        return out;
    }

    template<class T>
    std::ostream& operator<<(std::ostream& out, const std::optional<T>& op) {
        if (op.has_value()) {
            out << op.value();
        } else {
            out << "std::nullopt";
        }
        return out;
    }

    template<class TupType, size_t... I>
    void Print(std::ostream& out, const TupType& tup, std::index_sequence<I...>)
    {
        out << "Tupple:";
        (..., (out << "\nIndex " << I << ":\n" << std::get<I>(tup)));
    }

    template<class... Args>
    std::ostream& operator<<(std::ostream& out, const std::tuple<Args...>& tup)
    {
        _print(out, tup, std::make_index_sequence<sizeof...(Args)>());

        return out;
    }
}