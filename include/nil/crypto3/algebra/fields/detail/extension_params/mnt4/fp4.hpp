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

#ifndef CRYPTO3_ALGEBRA_FIELDS_MNT4_FP4_EXTENSION_PARAMS_HPP
#define CRYPTO3_ALGEBRA_FIELDS_MNT4_FP4_EXTENSION_PARAMS_HPP

#include <nil/crypto3/algebra/fields/params.hpp>
#include <nil/crypto3/algebra/fields/mnt4/base_field.hpp>
#include <nil/crypto3/algebra/fields/fp2.hpp>

#include <nil/crypto3/detail/literals.hpp>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace fields {
                namespace detail {

                    template<typename BaseField>
                    class fp4_extension_params;

                    /************************* MNT4 ***********************************/

                    template<std::size_t Version>
                    class fp4_extension_params<fields::mnt4_base_field<Version>>
                        : public params<fields::mnt4_base_field<Version>> {

                        typedef fields::mnt4_base_field<Version> base_field_type;
                        typedef params<base_field_type> policy_type;

                    public:
                        typedef typename policy_type::integral_type integral_type;

                        constexpr static const integral_type modulus = policy_type::modulus;

                        typedef base_field_type non_residue_field_type;
                        typedef typename non_residue_field_type::value_type non_residue_type;
                        typedef fields::fp2<base_field_type> underlying_field_type;
                        typedef typename underlying_field_type::value_type underlying_type;
                        // typedef element_fp2<fp2_extension_params<field_type>> underlying_type;

                        /*constexpr static const std::array<non_residue_type, 4> Frobenius_coeffs_c1 =
                           {non_residue_type(0x01),
                            non_residue_type(0xF73779FE09916DFDCC2FD1F968D534BEB17DAF7518CD9FAE5C1F7BDCF94DD5D7DEF6980C4_cppui292),
                            non_residue_type(0x3BCF7BCD473A266249DA7B0548ECAEEC9635D1330EA41A9E35E51200E12C90CD65A71660000_cppui298),
                            non_residue_type(0x3AD84453493094F44C0E4B334F83D9B7D7845383998B4CFE8788F285043342F78DC81FC7F3D_cppui298)};*/

                        constexpr static const std::array<integral_type, 4> Frobenius_coeffs_c1 = {
                            0x01, 0xF73779FE09916DFDCC2FD1F968D534BEB17DAF7518CD9FAE5C1F7BDCF94DD5D7DEF6980C4_cppui292,
                            0x3BCF7BCD473A266249DA7B0548ECAEEC9635D1330EA41A9E35E51200E12C90CD65A71660000_cppui298,
                            0x3AD84453493094F44C0E4B334F83D9B7D7845383998B4CFE8788F285043342F78DC81FC7F3D_cppui298};

                        constexpr static const non_residue_type non_residue = non_residue_type(0x11);
                    };

                    template<std::size_t Version>
                    constexpr typename fp4_extension_params<mnt4_base_field<Version>>::non_residue_type const
                        fp4_extension_params<mnt4_base_field<Version>>::non_residue;

                    template<std::size_t Version>
                    constexpr typename fp4_extension_params<mnt4_base_field<Version>>::integral_type const
                        fp4_extension_params<mnt4_base_field<Version>>::modulus;

                    template<std::size_t Version>
                    constexpr std::array<typename fp4_extension_params<mnt4_base_field<Version>>::integral_type,
                                         4> const fp4_extension_params<mnt4_base_field<Version>>::Frobenius_coeffs_c1;

                }    // namespace detail
            }        // namespace fields
        }            // namespace algebra
    }                // namespace crypto3
}    // namespace nil

#endif    // CRYPTO3_ALGEBRA_FIELDS_MNT4_FP4_EXTENSION_PARAMS_HPP
