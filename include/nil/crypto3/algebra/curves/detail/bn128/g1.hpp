//---------------------------------------------------------------------------//
// Copyright (c) 2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Nikita Kaskov <nbering@nil.foundation>
// Copyright (c) 2020 Ilias Khairullin <ilias@nil.foundation>
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

#ifndef CRYPTO3_ALGEBRA_CURVES_BN128_G1_HPP
#define CRYPTO3_ALGEBRA_CURVES_BN128_G1_HPP

#include <nil/crypto3/algebra/curves/detail/bn128/g2.hpp>

#include <nil/crypto3/algebra/fields/bn128/base_field.hpp>
#include <nil/crypto3/algebra/fields/bn128/scalar_field.hpp>
#include <nil/crypto3/algebra/fields/fp2.hpp>

#include <nil/crypto3/detail/type_traits.hpp>
#include <nil/crypto3/detail/literals.hpp>

#include <nil/crypto3/algebra/curves/detail/scalar_mul.hpp>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace curves {
                namespace detail {

                    using namespace nil::crypto3::algebra;

                    template<std::size_t ModulusBits = 254>
                    struct bn128_g1 {

                        constexpr static const std::size_t g1_field_bits = ModulusBits;
                        typedef typename fields::bn128_fq<g1_field_bits>::value_type g1_field_type_value;

                        constexpr static const std::size_t g2_field_bits = ModulusBits;
                        typedef typename fields::fp2<fields::bn128_fq<g2_field_bits>>::value_type g2_field_type_value;

                        using underlying_field_value_type = g1_field_type_value;

                        // constexpr static const std::size_t value_bits =  policy_type::g1_field_type::value_bits;

                        underlying_field_value_type X;
                        underlying_field_value_type Y;
                        underlying_field_value_type Z;

                        /*************************  Constructors and zero/one  ***********************************/

                        bn128_g1() :
                            bn128_g1(underlying_field_value_type::one(), underlying_field_value_type::one(),
                                     underlying_field_value_type::zero()) {};
                        // must be
                        // bn128_g1() : bn128_g1(zero_fill[0], zero_fill[1], zero_fill[2]) {};
                        // when constexpr fields will be finished

                        bn128_g1(underlying_field_value_type X,
                                 underlying_field_value_type Y,
                                 underlying_field_value_type Z) {
                            this->X = X;
                            this->Y = Y;
                            this->Z = Z;
                        };

                        static bn128_g1 zero() {
                            return bn128_g1();
                        }

                        static bn128_g1 one() {
                            return bn128_g1(underlying_field_value_type(1), underlying_field_value_type(2),
                                            underlying_field_value_type(1));
                            // must be
                            // return bn128_g1(one_fill[0], one_fill[1], one_fill[2]);
                            // when constexpr fields will be finished
                        }

                        /*************************  Comparison operations  ***********************************/

                        bool operator==(const bn128_g1 &other) const {
                            if (this->is_zero()) {
                                return other.is_zero();
                            }

                            if (other.is_zero()) {
                                return false;
                            }

                            /* now neither is O */

                            underlying_field_value_type Z1sq, Z2sq, lhs, rhs;
                            Z1sq = (this->Z).squared();
                            Z2sq = other.Z.squared();
                            lhs = Z2sq * this->X;
                            rhs = Z1sq * other.X;

                            if (lhs != rhs) {
                                return false;
                            }

                            underlying_field_value_type Z1cubed, Z2cubed;
                            Z1cubed = Z1sq * this->Z;
                            Z2cubed = Z2sq * other.Z;
                            lhs = Z2cubed * this->Y;
                            rhs = Z1cubed * other.Y;

                            return (lhs == rhs);
                        }

                        bool operator!=(const bn128_g1 &other) const {
                            return !operator==(other);
                        }

                        bool is_zero() const {
                            return Z.is_zero();
                        }

                        bool is_special() const {
                            return (this->is_zero() || this->Z == 1);
                        }

                        /*************************  Arithmetic operations  ***********************************/

                        bn128_g1 operator=(const bn128_g1 &other) {
                            // handle special cases having to do with O
                            this->X = other.X;
                            this->Y = other.Y;
                            this->Z = other.Z;

                            return *this;
                        }

                        /*
                            Jacobi coordinate
                            (p_out[0], p_out[1], p_out[2]) = (X, Y, Z) + (other.X, other.Y, other.Z)
                        */
                        bn128_g1 operator+(const bn128_g1 &other) const {
                            // handle special cases having to do with O
                            if (this->is_zero()) {
                                return other;
                            }

                            if (other.is_zero()) {
                                return (*this);
                            }

                            if (*this == other) {
                                return this->doubled();
                            }

                            return this->add(other);
                        }

                        bn128_g1 operator-(const bn128_g1 &other) const {
                            return *this + (-other);
                        }

                        bn128_g1 operator-() const {
                            return bn128_g1(X, -Y, Z);
                        }

                        /*
                            Jacobi coordinate
                            (p_out[0], p_out[1], p_out[2]) = 2(X, Y, Z)
                        */
                        bn128_g1 doubled() const {
                            underlying_field_value_type p_out[3];

                            underlying_field_value_type A, B, C, D, E;
                            A = X.squared();
                            B = Y.squared();
                            C = B.squared();
                            D = ((X + B).squared() - A - C).doubled();
                            E = A.doubled() + A;

                            p_out[0] = E.squared() - D.doubled();
                            p_out[1] = E * (D - p_out[0]) - C.doubled().doubled().doubled();
                            p_out[2] = (Y * Z).doubled();

                            return bn128_g1(p_out[0], p_out[1], p_out[2]);
                        }

                        bn128_g1 mixed_add(const bn128_g1 &other) const {
                            if (this->is_zero()) {
                                return other;
                            }

                            if (other.is_zero()) {
                                return *this;
                            }

                            // no need to handle points of order 2,4
                            // (they cannot exist in a prime-order subgroup)

                            // check for doubling case

                            // using Jacobian pinates so:
                            // (X1:Y1:Z1) = (X2:Y2:Z2)
                            // iff
                            // X1/Z1^2 == X2/Z2^2 and Y1/Z1^3 == Y2/Z2^3
                            // iff
                            // X1 * Z2^2 == X2 * Z1^2 and Y1 * Z2^3 == Y2 * Z1^3

                            // we know that Z2 = 1

                            underlying_field_value_type Z1Z1 = this->Z.squared();

                            underlying_field_value_type U2 = other.X * Z1Z1;

                            underlying_field_value_type S2 = other.Y * this->Z * Z1Z1;
                            ;    // S2 = Y2*Z1*Z1Z1

                            if (this->X == U2 && this->Y == S2) {
                                // dbl case; nothing of above can be reused
                                return this->doubled();
                            }

                            bn128_g1 result;
                            underlying_field_value_type H, HH, I, J, r, V;
                            // H = U2-X1
                            H = U2 - this->X;
                            // HH = H^2
                            HH = H.squared();
                            // I = 4*HH
                            I = HH.doubled().doubled();
                            // J = H*I
                            J = H * I;
                            // r = 2*(S2-Y1)
                            r = (S2 - this->Y).doubled();
                            // V = X1*I
                            V = this->X * I;
                            // X3 = r^2-J-2*V
                            result.X = r.squared() - J - V.doubled();
                            // Y3 = r*(V-X3)-2*Y1*J
                            result.Y = r * (V - result.X) - (this->Y * J).doubled();
                            // Z3 = (Z1+H)^2-Z1Z1-HH
                            result.Z = (this->Z + H).squared() - Z1Z1 - HH;

                            return result;
                        }

                        template<typename NumberType>
                        bn128_g1 operator*(const NumberType &other) const {
                            return scalar_mul(*this, other);
                        }

                    private:
                        bn128_g1 add(const bn128_g1 &other) const {    // unfinished

                            underlying_field_value_type Z1Z1, Z2Z2, U1, U2, S1, S2, H, I, J, t3, r, V;

                            underlying_field_value_type X_out, Y_out, Z_out;

                            Z1Z1 = Z.squared();
                            Z2Z2 = other.Z.squared();
                            U1 = X * Z2Z2;
                            U2 = other.X * Z1Z1;

                            S1 = Y * other.Z * Z2Z2;
                            S2 = other.Y * Z * Z1Z1;

                            H = U2 - U1;
                            t3 = S2 - S1;

                            I = H.doubled().squared();
                            J = H * I;
                            r = t3.doubled();
                            V = U1 * I;

                            X_out = r.squared() - J - V.doubled();
                            Y_out = r * (V - X_out) - (S1 * J).doubled();
                            Z_out = ((Z + other.Z).squared() - Z1Z1 - Z2Z2) * H;

                            return bn128_g1(X_out, Y_out, Z_out);
                        }

                    public:
                        /*************************  Reducing operations  ***********************************/

                        bn128_g1 to_affine_coordinates() const {
                            underlying_field_value_type p_out[3];

                            if (is_zero() || Z == 1)
                                return *this;
                            underlying_field_value_type r, r2;
                            r = Z.inversed();
                            r2 = r.squared();
                            p_out[0] = X * r2;        // r2
                            p_out[1] = Y * r * r2;    // r3
                            p_out[2] = underlying_field_value_type::one();

                            return bn128_g1(p_out[0], p_out[1], p_out[2]);
                        }

                        bn128_g1 to_special() const {
                            return this->to_affine_coordinates();
                        }

                    private:
                        /*constexpr static const std::array<underlying_field_value_type, 3> zero_fill = {
                            underlying_field_value_type::one(), underlying_field_value_type::one(),
                            underlying_field_value_type::zero()};*/

                        /*constexpr static const std::array<underlying_field_value_type, 3> one_fill = {
                            underlying_field_value_type(1), underlying_field_value_type(2),
                           underlying_field_value_type(1)};*/
                    };

                }    // namespace detail
            }        // namespace curves
        }            // namespace algebra
    }                // namespace crypto3
}    // namespace nil
#endif    // CRYPTO3_ALGEBRA_CURVES_BN128_G1_HPP
