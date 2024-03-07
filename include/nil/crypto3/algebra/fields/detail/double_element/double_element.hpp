//---------------------------------------------------------------------------//
// Copyright (c) 2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Nikita Kaskov <nbering@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_ALGEBRA_FIELDS_DOUBLE_ELEMENT_HPP
#define CRYPTO3_ALGEBRA_FIELDS_DOUBLE_ELEMENT_HPP

namespace nil {
    namespace crypto3 {
        namespace algebra {
            namespace fields {
                namespace detail {

                    template<typename Field>
                    struct double_element {
                        typedef extension_params<Field> params_type;
                    };

                }    // namespace detail
            }        // namespace fields
        }            // namespace algebra
    }                // namespace crypto3
}    // namespace nil
#endif    // ALGEBRA_FIELDS_DOUBLE_ELEMENT_HPP
