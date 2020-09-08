//---------------------------------------------------------------------------//
// Copyright (c) 2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Nikita Kaskov <nbering@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef ALGEBRA_FIELDS_FFDHE_IETF_PARAMS_HPP
#define ALGEBRA_FIELDS_FFDHE_IETF_PARAMS_HPP

#include <nil/algebra/fields/detail/params/params.hpp>

#include <nil/algebra/fields/ffdhe_ietf.hpp>

#include <nil/algebra/detail/literals.hpp>

namespace nil {
    namespace algebra {
        namespace fields {
            namespace detail {

                template<std::size_t ModulusBits, std::size_t GeneratorBits = CHAR_BIT>
                struct extension_params<ffdhe_ietf<ModulusBits, GeneratorBits>>
                    : public params<modp_srp<ModulusBits, GeneratorBits>> {
                private:
                    typedef params<modp_srp<ModulusBits, GeneratorBits>> policy_type;
                    typedef extension_params<modp_srp<ModulusBits, GeneratorBits>> element_policy_type;

                public:
                    typedef typename policy_type::number_type number_type;

                    constexpr static const modulus_type q = 0;
                };

            }    // namespace detail
        }        // namespace fields
    }            // namespace algebra
}    // namespace nil

#endif    // ALGEBRA_FIELDS_FFDHE_IETF_PARAMS_HPP
