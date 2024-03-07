//---------------------------------------------------------------------------//
// Copyright (c) 2020-2021 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020-2021 Ilias Khairullin <ilias@nil.foundation>
// Copyright (c) 2020-2021 Nikita Kaskov <nbering@nil.foundation>
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

#ifndef CRYPTO3_ALGEBRA_CURVES_WEIERSTRASS_HPP
#define CRYPTO3_ALGEBRA_CURVES_WEIERSTRASS_HPP

#include <nil/crypto3/algebra/type_traits.hpp>

#include <nil/crypto3/multiprecision/number.hpp>

#include <cstdint>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace curves {
                /**
                 * For short Weierstrass curves y^2 = x^3 + b mod r, if there exists a cube root of unity mod r,
                 * we can take advantage of an enodmorphism to decompose a 254 bit scalar into 2 128 bit scalars.
                 * \beta = cube root of 1, mod q (q = order of fq)
                 * \lambda = cube root of 1, mod r (r = order of fr)
                 *
                 * For a point P1 = (X, Y), where Y^2 = X^3 + b, we know that
                 * the point P2 = (X * \beta, Y) is also a point on the curve
                 * We can represent P2 as a scalar multiplication of P1, where P2 = \lambda * P1
                 *
                 * For a generic multiplication of P1 by a 254 bit scalar k, we can decompose k
                 * into 2 127 bit scalars (k1, k2), such that k = k1 - (k2 * \lambda)
                 *
                 * We can now represent (k * P1) as (k1 * P1) - (k2 * P2), where P2 = (X * \beta, Y).
                 * As k1, k2 have half the bit length of k, we have reduced the number of loop iterations of our
                 * scalar multiplication algorithm in half
                 *
                 * To find k1, k2, We use the extended euclidean algorithm to find 4 short scalars [a1, a2], [b1, b2]
                 * such that modulus = (a1 * b2) - (b1 * a2) We then compute scalars c1 = round(b2 * k / r), c2 =
                 * round(b1 * k / r), where k1 = (c1 * a1) + (c2 * a2), k2 = -((c1 * b1) + (c2 * b2)) We pre-compute
                 * scalars g1 = (2^256 * b1) / n, g2 = (2^256 * b2) / n, to avoid having to perform long division on
                 * 512-bit scalars
                 **/
                template<typename FieldType>
                static void split_into_endomorphism_scalars(const typename FieldType::value_type &k,
                                                            typename FieldType::value_type &k1,
                                                            typename FieldType::value_type &k2) {
                    typedef typename FieldType::value_type value_type;

                    value_type input = k.reduce_once();
                    // uint64_t lambda_reduction[4] = { 0 };
                    // __to_montgomery_form(lambda, lambda_reduction);

                    // TODO: these parameters only work for the bn254 coordinate field.
                    // Need to shift into Params and calculate correct constants for the subgroup field
                    constexpr value_type endo_g1 = {Params::endo_g1_lo, Params::endo_g1_mid, Params::endo_g1_hi, 0};

                    constexpr value_type endo_g2 = {Params::endo_g2_lo, Params::endo_g2_mid, 0, 0};

                    constexpr value_type endo_minus_b1 = {Params::endo_minus_b1_lo, Params::endo_minus_b1_mid, 0, 0};

                    constexpr value_type endo_b2 = {Params::endo_b2_lo, Params::endo_b2_mid, 0, 0};

                    // compute c1 = (g2 * k) >> 256
                    wide_array c1 = endo_g2.mul_512(input);
                    // compute c2 = (g1 * k) >> 256
                    wide_array c2 = endo_g1.mul_512(input);

                    // (the bit shifts are implicit, as we only utilize the high limbs of c1, c2

                    // TODO remove data duplication
                    value_type c1_hi = {c1.data[4], c1.data[5], c1.data[6],
                                        c1.data[7]};    // *(field*)((uintptr_t)(&c1) + (4 * sizeof(uint64_t)));
                    value_type c2_hi = {c2.data[4], c2.data[5], c2.data[6],
                                        c2.data[7]};    // *(field*)((uintptr_t)(&c2) + (4 * sizeof(uint64_t)));

                    // compute q1 = c1 * -b1
                    wide_array q1 = c1_hi.mul_512(endo_minus_b1);
                    // compute q2 = c2 * b2
                    wide_array q2 = c2_hi.mul_512(endo_b2);

                    // TODO: this doesn't have to be a 512-bit multiply...
                    value_type q1_lo {q1.data[0], q1.data[1], q1.data[2], q1.data[3]};
                    value_type q2_lo {q2.data[0], q2.data[1], q2.data[2], q2.data[3]};

                    value_type t1 = (q2_lo - q1_lo).reduce_once();
                    value_type t2 = (t1 * beta() + input).reduce_once();
                    k2.data[0] = t1.data[0];
                    k2.data[1] = t1.data[1];
                    k1.data[0] = t2.data[0];
                    k1.data[1] = t2.data[1];
                }
            }    // namespace curves
        }        // namespace algebra
    }            // namespace crypto3
}    // namespace nil
#endif    // CRYPTO3_ALGEBRA_CURVES_SCALAR_MUL_HPP
