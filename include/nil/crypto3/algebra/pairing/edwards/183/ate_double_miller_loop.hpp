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

#ifndef CRYPTO3_ALGEBRA_PAIRING_EDWARDS_183_ATE_DOUBLE_MILLER_LOOP_HPP
#define CRYPTO3_ALGEBRA_PAIRING_EDWARDS_183_ATE_DOUBLE_MILLER_LOOP_HPP

#include <boost/multiprecision/number.hpp>
#include <nil/crypto3/multiprecision/cpp_int_modular.hpp>

#include <nil/crypto3/algebra/curves/edwards.hpp>
#include <nil/crypto3/algebra/pairing/detail/edwards/183/params.hpp>
#include <nil/crypto3/algebra/pairing/detail/edwards/183/types.hpp>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace pairing {

                template<std::size_t Version = 183>
                class edwards_ate_double_miller_loop;

                template<>
                class edwards_ate_double_miller_loop<183> {
                    using curve_type = curves::edwards<183>;

                    using params_type = detail::pairing_params<curve_type>;
                    typedef detail::types_policy<curve_type> policy_type;
                    using gt_type = typename curve_type::gt_type;

                public:
                    static typename gt_type::value_type
                        process(const typename policy_type::ate_g1_precomputed_type &prec_P1,
                                const typename policy_type::ate_g2_precomputed_type &prec_Q1,
                                const typename policy_type::ate_g1_precomputed_type &prec_P2,
                                const typename policy_type::ate_g2_precomputed_type &prec_Q2) {

                        typename gt_type::value_type f = gt_type::value_type::one();

                        bool found_one = false;
                        std::size_t idx = 0;

                        const typename policy_type::integral_type &loop_count = params_type::ate_loop_count;

                        for (long i = params_type::integral_type_max_bits - 1; i >= 0; --i) {
                            const bool bit = boost::multiprecision::bit_test(loop_count, i);
                            if (!found_one) {
                                /* this skips the MSB itself */
                                found_one |= bit;
                                continue;
                            }

                            /* code below gets executed for all bits (EXCEPT the MSB itself) of
                               param_p (skipping leading zeros) in MSB to LSB
                               order */
                            typename policy_type::Fq3_conic_coefficients cc1 = prec_Q1[idx];
                            typename policy_type::Fq3_conic_coefficients cc2 = prec_Q2[idx];
                            ++idx;

                            typename gt_type::value_type g_RR_at_P1 = typename gt_type::value_type(
                                prec_P1.P_XY * cc1.c_XY + prec_P1.P_XZ * cc1.c_XZ, prec_P1.P_ZZplusYZ * cc1.c_ZZ);

                            typename gt_type::value_type g_RR_at_P2 = typename gt_type::value_type(
                                prec_P2.P_XY * cc2.c_XY + prec_P2.P_XZ * cc2.c_XZ, prec_P2.P_ZZplusYZ * cc2.c_ZZ);
                            f = f.squared() * g_RR_at_P1 * g_RR_at_P2;

                            if (bit) {
                                cc1 = prec_Q1[idx];
                                cc2 = prec_Q2[idx];
                                ++idx;
                                typename gt_type::value_type g_RQ_at_P1 = typename gt_type::value_type(
                                    prec_P1.P_ZZplusYZ * cc1.c_ZZ, prec_P1.P_XY * cc1.c_XY + prec_P1.P_XZ * cc1.c_XZ);
                                typename gt_type::value_type g_RQ_at_P2 = typename gt_type::value_type(
                                    prec_P2.P_ZZplusYZ * cc2.c_ZZ, prec_P2.P_XY * cc2.c_XY + prec_P2.P_XZ * cc2.c_XZ);
                                f = f * g_RQ_at_P1 * g_RQ_at_P2;
                            }
                        }

                        return f;
                    }
                };
            }    // namespace pairing
        }        // namespace algebra
    }            // namespace crypto3
}    // namespace nil
#endif    // CRYPTO3_ALGEBRA_PAIRING_EDWARDS_183_ATE_DOUBLE_MILLER_LOOP_HPP
