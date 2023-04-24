//---------------------------------------------------------------------------//
// Copyright 2022 =nil; Foundation
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//---------------------------------------------------------------------------//
#ifndef CRYPTO3_FP_EVM_H
#define CRYPTO3_FP_EVM_H

#include <nil/crypto3/multiprecision/number.hpp>
#include "evm_sdk.h"

namespace nil::crypto3::algebra::fields {
namespace detail {
    template<typename FieldParams>
    class element_fp {
        typedef FieldParams policy_type;

    public:
        typedef typename policy_type::field_type field_type;

        typedef typename policy_type::integral_type integral_type;
        typedef typename policy_type::modular_backend modular_backend;
        typedef typename policy_type::modular_params_type modular_params_type;

        static_assert(nil::crypto3::multiprecision::backends::is_trivial_cpp_int<modular_backend>::value);

        constexpr static const integral_type modulus = policy_type::modulus;

        using data_type = nil::crypto3::multiprecision::number<modular_backend>;

        data_type data;

        constexpr element_fp() : data(data_type(0)) {};

        constexpr element_fp(const data_type &data) : data(data) {};

        template<typename Number>
        constexpr element_fp(const Number &data) : data(data) {};

        constexpr element_fp(const element_fp &B) {
            data = B.data;
        };

        constexpr inline static element_fp zero() {
            return element_fp(0);
        }

        constexpr inline static element_fp one() {
            return element_fp(1);
        }

        constexpr bool is_zero() const {
            return data == data_type(0);
        }

        constexpr bool is_one() const {
            return data == one();
        }

        constexpr bool operator==(const element_fp &B) const {
            return data == B.data;
        }

        constexpr bool operator!=(const element_fp &B) const {
            return data != B.data;
        }

        constexpr element_fp &operator=(const element_fp &B) {
            data = B.data;

            return *this;
        }

        constexpr element_fp operator+(const element_fp &B) const {
            // TODO(msherstennikov): honor sign
            auto a = get_trivial_value(data);
            auto b = get_trivial_value(B.data);
            return __evm_builtin_modadd(a, b, get_trivial_value(modulus));
        }

        constexpr element_fp operator-(const element_fp &B) const {
//            return *this + -B;
            auto a = get_trivial_value(data);
            auto b = get_trivial_value(B.data);
            auto mod = get_trivial_value(modulus);
            if (a > b) {
                return (a - b) % mod;
            }
            __uint256_t tmp = (b - a) % mod;
            return mod - tmp;
        }

        constexpr element_fp &operator-=(const element_fp &B) {
            *this = *this - B;
            return *this;
        }

        constexpr element_fp &operator+=(const element_fp &B) {
            *this = *this + B;
            return *this;
        }

        constexpr element_fp &operator*=(const element_fp &B) {
            *this = *this * B;
            return *this;
        }

        constexpr element_fp &operator/=(const element_fp &B) {
            *this = *this / B;
            return *this;
        }

        constexpr element_fp operator-() const {
            return element_fp(-data);
        }

        constexpr element_fp operator*(const element_fp &B) const {
            return __evm_builtin_modmul(get_trivial_value(data), get_trivial_value(B.data), get_trivial_value(modulus));
        }

        constexpr element_fp operator/(const element_fp &B) const {
            return *this * B.inversed();
        }

        constexpr bool operator<(const element_fp &B) const {
            return data < B.data;
        }

        constexpr bool operator>(const element_fp &B) const {
            return data > B.data;
        }

        constexpr bool operator<=(const element_fp &B) const {
            return data <= B.data;
        }

        constexpr bool operator>=(const element_fp &B) const {
            return data >= B.data;
        }

        constexpr element_fp &operator++() {
            data = data + one();
            return *this;
        }

        constexpr element_fp operator++(int) {
            element_fp temp(*this);
            ++*this;
            return temp;
        }

        constexpr element_fp &operator--() {
            data = data - one();
            return *this;
        }

        constexpr element_fp operator--(int) {
            element_fp temp(*this);
            --*this;
            return temp;
        }

        constexpr element_fp doubled() const {
            return element_fp(data + data);
        }

        // TODO: maybe incorrect result here
        constexpr element_fp sqrt() const {
            std::abort();
        }

        constexpr element_fp inversed() const {
            decltype(data) tmp_mod(modulus);
            return element_fp(inverse_extended_euclidean_algorithm(data, tmp_mod));
        }

        constexpr element_fp squared() const {
            return element_fp(data * data);    // maybe can be done more effective
        }

        // TODO: maybe error here
        constexpr bool is_square() const {
            return (this->sqrt() != -1);    // maybe can be done more effective
        }

        template<typename PowerType,
                 typename = typename std::enable_if<boost::is_integral<PowerType>::value>::type>
        constexpr element_fp pow(const PowerType pwr) const {
            return __evm_builtin_modpow(get_value(), pwr, get_trivial_value(modulus));
        }

        template<typename Backend>
        constexpr element_fp pow(const multiprecision::number<Backend> &pwr) const {
            return __evm_builtin_modpow(get_value(), get_trivial_value(pwr), get_trivial_value(modulus));
        }

        auto get_value() const {
            return get_trivial_value(data);
        }
    private:
        template<typename Number>
        static inline auto get_trivial_value(const Number& v) {
            static_assert(nil::crypto3::multiprecision::backends::is_trivial_cpp_int<typename Number::backend_type>::value);
            return v.backend().limbs()[0];
        }
    };

    template<typename FieldParams>
    constexpr typename element_fp<FieldParams>::integral_type const element_fp<FieldParams>::modulus;

}    // namespace detail

}  // nil::crypto3::algebra::fields

#endif    // CRYPTO3_FP_EVM_H
