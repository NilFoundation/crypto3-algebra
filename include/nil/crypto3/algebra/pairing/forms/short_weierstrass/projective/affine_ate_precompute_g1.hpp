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

#ifndef CRYPTO3_ALGEBRA_PAIRING_SHORT_WEIERSTRASS_PROJECTIVE_AFFINE_ATE_PRECOMPUTE_G1_HPP
#define CRYPTO3_ALGEBRA_PAIRING_SHORT_WEIERSTRASS_PROJECTIVE_AFFINE_ATE_PRECOMPUTE_G1_HPP

#include <nil/crypto3/algebra/pairing/detail/forms/short_weierstrass/projective/types.hpp>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace pairing {

                template<typename CurveType>
                class short_weierstrass_projective_affine_ate_precompute_g1 {
                    using curve_type = CurveType;

                    using params_type = detail::pairing_params<curve_type>;
                    typedef detail::short_weierstrass_projective_<curve_type> policy_type;
                    using g1_type = typename curve_type::template g1_type<>;
                    using g1_affine_type = typename curve_type::template g1_type<curves::coordinates::affine>;

                public:
                    using g1_precomputed_type = typename policy_type::affine_ate_g1_precomputation;

                    static g1_precomputed_type process(const typename g1_type::value_type &P) {

                        typename g1_affine_type::value_type Pcopy = P.to_affine();

                        if (P.is_zero()) {
                            Pcopy.X = g1_type::field_type::value_type::zero();
                            Pcopy.Y = g1_type::field_type::value_type::zero();
                        }

                        g1_precomputed_type result;
                        result.PX = Pcopy.X;
                        result.PY = Pcopy.Y;
                        result.PY_twist_squared = Pcopy.Y * params_type::twist.squared();

                        return result;
                    }
                };
            }    // namespace pairing
        }        // namespace algebra
    }            // namespace crypto3
}    // namespace nil
#endif    // CRYPTO3_ALGEBRA_PAIRING_SHORT_WEIERSTRASS_PROJECTIVE_AFFINE_ATE_PRECOMPUTE_G1_HPP
