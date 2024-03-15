//---------------------------------------------------------------------------//
// Copyright (c) 2020-2021 Mikhail Komarov <nemo@nil.foundation>
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

#ifndef CRYPTO3_ALGEBRA_PAIRING_BLS12_381_BASIC_PARAMS_HPP
#define CRYPTO3_ALGEBRA_PAIRING_BLS12_381_BASIC_PARAMS_HPP

#include <nil/crypto3/algebra/pairing/pairing_policy.hpp>
#include <nil/crypto3/algebra/curves/bls12.hpp>

#include <nil/crypto3/algebra/pairing/detail/forms/short_weierstrass/jacobian_with_a4_0/types.hpp>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace pairing {
                namespace detail {

                    template<>
                    class pairing_params<curves::bls12<381>> {
                        using curve_type = curves::bls12<381>;

                    public:
                        using integral_type = typename curve_type::base_field_type::integral_type;
                        using extended_integral_type = typename curve_type::base_field_type::extended_integral_type;

                        constexpr static const std::size_t integral_type_max_bits =
                            curve_type::base_field_type::modulus_bits;

                        constexpr static const integral_type ate_loop_count =
                            integral_type(0xD201000000010000_cppui64);
                        constexpr static const bool ate_is_loop_count_neg = true;

                        constexpr static const integral_type final_exponent_z =
                            integral_type(0xD201000000010000_cppui64);
                        constexpr static const bool final_exponent_is_z_neg = true;

                        using g2_field_type_value = typename curve_type::template g2_type<>::field_type::value_type;

                        constexpr static const g2_field_type_value twist =
                            curve_type::template g2_type<>::params_type::twist;

                        constexpr static const g2_field_type_value twist_coeff_b =
                            curve_type::template g2_type<>::params_type::b;

                        constexpr static const twist_type twist_type = twist_type::TWIST_TYPE_M;
                    };

                    constexpr typename pairing_params<curves::bls12<381>>::integral_type const
                        pairing_params<curves::bls12<381>>::ate_loop_count;

                    constexpr typename pairing_params<curves::bls12<381>>::integral_type const
                        pairing_params<curves::bls12<381>>::final_exponent_z;
                    constexpr typename pairing_params<curves::bls12<381>>::g2_field_type_value const
                        pairing_params<curves::bls12<381>>::twist;
                    constexpr typename pairing_params<curves::bls12<381>>::g2_field_type_value const
                        pairing_params<curves::bls12<381>>::twist_coeff_b;

                    constexpr bool const pairing_params<curves::bls12<381>>::final_exponent_is_z_neg;

                }    // namespace detail
            }        // namespace pairing
        }            // namespace algebra
    }                // namespace crypto3
}    // namespace nil

#endif    // CRYPTO3_ALGEBRA_PAIRING_BLS12_381_BASIC_PARAMS_HPP
