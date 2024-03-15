//---------------------------------------------------------------------------//
// Copyright (c) 2020-2021 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020-2021 Nikita Kaskov <nbering@nil.foundation>
// Copyright (c) 2024 Vasiliy Olekhov <vasiliy.olekhov@nil.foundation>
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

#ifndef CRYPTO3_ALGEBRA_PAIRING_ALT_BN128_PAIRING_PARAMS_HPP
#define CRYPTO3_ALGEBRA_PAIRING_ALT_BN128_PAIRING_PARAMS_HPP

#include <nil/crypto3/algebra/curves/alt_bn128.hpp>
#include <nil/crypto3/algebra/pairing/pairing_policy.hpp>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace pairing {
                namespace detail {

                    template<typename CurveType>
                    class pairing_params;

                    template<>
                    class pairing_params<curves::alt_bn128<254>> {
                        using curve_type = curves::alt_bn128<254>;

                    public:
                        using integral_type = typename curve_type::base_field_type::integral_type;
                        using extended_integral_type = typename curve_type::base_field_type::extended_integral_type;

                        constexpr static const std::size_t integral_type_max_bits =
                            curve_type::base_field_type::modulus_bits;

                        constexpr static const integral_type ate_loop_count = 0x19D797039BE763BA8_cppui254;
                        constexpr static const bool ate_is_loop_count_neg = false;

                        using g2_field_type = typename curve_type::g2_type<>::params_type::field_type;

                        constexpr static const typename g2_field_type::value_type
                            TWIST_MUL_BY_Q_X = typename g2_field_type::value_type(
                                0x2FB347984F7911F74C0BEC3CF559B143B78CC310C2C3330C99E39557176F553D_cppui254,
                                0x16C9E55061EBAE204BA4CC8BD75A079432AE2A1D0B7C9DCE1665D51C640FCBA2_cppui254);

                        constexpr static const typename g2_field_type::value_type
                            TWIST_MUL_BY_Q_Y = typename g2_field_type::value_type(
                                0x63CF305489AF5DCDC5EC698B6E2F9B9DBAAE0EDA9C95998DC54014671A0135A_cppui254,
                                0x7C03CBCAC41049A0704B5A7EC796F2B21807DC98FA25BD282D37F632623B0E3_cppui254);

                        /* signed bit representation */
                        constexpr static const std::array<int8_t,65> ate_loop_count_sbit = {
                            0, 0, 0, 1, 0, 1, 0,-1, 0, 0, 1,-1, 0, 0, 1, 0,
                            0, 1, 1, 0,-1, 0, 0, 1, 0,-1, 0, 0, 0, 0, 1, 1,
                            1, 0, 0,-1, 0, 0, 1, 0, 0, 0, 0, 0,-1, 0, 0, 1,
                            1, 0, 0,-1, 0, 0, 0, 1, 1, 0,-1, 0, 0, 1, 0, 1,
                            1,
                        };

                        /* python3:
                         *
                         * ate_loop_count = 0x19D797039BE763BA8
                         * sum = 0
                         * for (i,v) in enumerate(ate_loop_count_sbit):
                         *     sum+=v*2**i
                         *
                         * assert(sum == ate_loop_count)
                         *
                         * */

                        constexpr static const integral_type final_exponent_z =
                            integral_type(0x44E992B44A6909F1);
                        constexpr static const bool final_exponent_is_z_neg = false;

                        using g2_field_type_value = typename curve_type::g2_type<>::field_type::value_type;

                        constexpr static const g2_field_type_value twist =
                            curve_type::g2_type<>::params_type::twist;

                        constexpr static const g2_field_type_value twist_coeff_b =
                            curve_type::template g2_type<>::params_type::b;

                        constexpr static const twist_type twist_type = twist_type::TWIST_TYPE_D;

                    };

                    constexpr typename pairing_params<curves::alt_bn128<254>>::integral_type const
                        pairing_params<curves::alt_bn128<254>>::ate_loop_count;

                    constexpr typename pairing_params<curves::alt_bn128<254>>::integral_type const
                        pairing_params<curves::alt_bn128<254>>::final_exponent_z;

                    constexpr bool const pairing_params<curves::alt_bn128<254>>::ate_is_loop_count_neg;
                    constexpr bool const pairing_params<curves::alt_bn128<254>>::final_exponent_is_z_neg;

                }    // namespace detail
            }        // namespace pairing
        }            // namespace algebra
    }                // namespace crypto3
}    // namespace nil

#endif    // CRYPTO3_ALGEBRA_PAIRING_ALT_BN128_PAIRING_PARAMS_HPP
