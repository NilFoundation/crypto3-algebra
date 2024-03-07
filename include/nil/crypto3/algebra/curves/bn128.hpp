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

#ifndef CRYPTO3_ALGEBRA_CURVES_BN128_HPP
#define CRYPTO3_ALGEBRA_CURVES_BN128_HPP

#include <nil/crypto3/algebra/curves/detail/bn128/g1.hpp>
#include <nil/crypto3/algebra/curves/detail/bn128/g2.hpp>

#include <nil/crypto3/algebra/fields/bn128/base_field.hpp>
#include <nil/crypto3/algebra/fields/bn128/scalar_field.hpp>
#include <nil/crypto3/algebra/fields/fp12_2over3over2.hpp>

//#include <nil/crypto3/algebra/pairing/bn128.hpp>

#include <nil/crypto3/detail/literals.hpp>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace curves {

                /*
                    The curve equation for a BN curve is:

                    E/Fp: y^2 = x^3 + b.
                */

                /*
                    Over Fp12_2over3over2
                    y^2 = x^3 + b
                    u^2 = -1
                    xi = xi_a + xi_b u
                    v^3 = xi
                    w^2 = v
                */
                template<std::size_t ModulusBits = 254>
                struct bn128 { };

                template<>
                struct bn128<254> {
                    constexpr static const std::size_t base_field_bits = 254;
                    typedef fields::bn128_base_field<base_field_bits> base_field_type;
                    typedef typename base_field_type::modulus_type number_type;
                    typedef typename base_field_type::extended_modulus_type extended_number_type;

                    constexpr static const number_type base_field_modulus = base_field_type::modulus;

                    constexpr static const std::size_t scalar_field_bits = 254;
                    typedef fields::bn128_fr<scalar_field_bits> scalar_field_type;
                    constexpr static const number_type scalar_field_modulus = scalar_field_type::modulus;

                    typedef typename detail::bn128_g1<254> g1_type;
                    typedef typename detail::bn128_g2<254> g2_type;

                    // typedef typename pairing::pairing_policy<bn128<ModulusBits>> pairing_policy;

                    typedef typename fields::fp12_2over3over2<base_field_type> gt_type;

                    constexpr static const number_type p = base_field_modulus;
                    constexpr static const number_type q = scalar_field_modulus;

                    constexpr static const number_type a = 0;
                    constexpr static const number_type b = 0x03;
                    constexpr static const number_type x = 0x09;
                    constexpr static const number_type y = 0x01;
                };

                typedef bn128<254> bn128_254;

            }    // namespace curves
        }        // namespace algebra
    }            // namespace crypto3
}    // namespace nil

#endif    // CRYPTO3_ALGEBRA_CURVES_BN128_HPP
