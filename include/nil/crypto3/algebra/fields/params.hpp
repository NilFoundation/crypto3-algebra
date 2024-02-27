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

#ifndef CRYPTO3_ALGEBRA_FIELDS_PARAMS_HPP
#define CRYPTO3_ALGEBRA_FIELDS_PARAMS_HPP

#include <cstddef>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace fields {

                template<typename FieldType>
                struct params {
                    typedef FieldType field_type;
                    typedef typename field_type::modular_type modular_type;
                    typedef typename field_type::modular_backend modular_backend;
                    typedef typename field_type::modular_params_type modular_params_type;

                    constexpr static const std::size_t modulus_bits = field_type::modulus_bits;
                    typedef typename field_type::integral_type integral_type;

                    typedef typename field_type::extended_integral_type extended_integral_type;

                    constexpr static const integral_type modulus = field_type::modulus;
                    constexpr static const integral_type group_order_minus_one_half = field_type::group_order_minus_one_half;

                    constexpr static const modular_params_type modulus_params = field_type::modulus_params;
                };

                template<typename FieldType>
                constexpr typename params<FieldType>::integral_type const params<FieldType>::modulus;

                template<typename FieldType>
                constexpr typename params<FieldType>::modular_params_type const params<FieldType>::modulus_params;

                template<typename FieldType>
                constexpr typename std::size_t const params<FieldType>::modulus_bits;

                template<typename FieldType>
                struct arithmetic_params;

            }    // namespace fields
        }        // namespace algebra
    }            // namespace crypto3
}    // namespace nil

#endif    // CRYPTO3_ALGEBRA_FIELDS_PARAMS_HPP
