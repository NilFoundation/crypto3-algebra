//---------------------------------------------------------------------------//
// Copyright (c) 2020 Ilia Shirobokov <i.shirobokov@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef ALGEBRA_MULTIEXP_FIELDS_HPP
#define ALGEBRA_MULTIEXP_FIELDS_HPP

#include <cstdint>
#include <math.h>

#include <nil/algebra/multiexp/default.hpp>

namespace nil {
    namespace algebra {
        namespace fields {
            template<typename T, typename S> 
            T eval_multi_exp_naive(typename std::vector<T>::const_iterator vec_start,
                                typename std::vector<S>::const_iterator scalar_start, 
                                const size_t n) {
                using value_type = T;

                std::function<value_type (value_type, value_type)> base_op = [](value_type a, value_type b) -> value_type { return a * b; };
                std::function<value_type (int, value_type)> s_op = [](int s, value_type a) -> value_type { return a.pow(s); };
                std::function<value_type (value_type)> dbl_op = [](value_type a) -> value_type { return a * a; };
                nil::algebra::multiexp::operation_set<value_type, int> op_set(base_op, s_op, dbl_op);

                return nil::algebra::multiexp::eval_multi_exp_naive<T, S>(vec_start, scalar_start, n, value_type::one(), op_set);
            }
            
            template<typename T, typename S>
            T eval_multi_exp(typename std::vector<T>::const_iterator vec_start,
                            typename std::vector<S>::const_iterator scalar_start, size_t num_groups,
                            const size_t bucket_size, const size_t workers_in_subgroup, const size_t n,
                            const size_t scalar_size) {
                using value_type = T;

                std::function<value_type (value_type, value_type)> base_op = [](value_type a, value_type b) -> value_type { return a * b; };
                std::function<value_type (int, value_type)> s_op = [](int s, value_type a) -> value_type { return a.pow(s); };
                std::function<value_type (value_type)> dbl_op = [](value_type a) -> value_type { return a * a; };
                nil::algebra::multiexp::operation_set<value_type, int> op_set(base_op, s_op, dbl_op);

                return nil::algebra::multiexp::eval_multi_exp<T, S>(vec_start, scalar_start, num_groups, bucket_size, workers_in_subgroup, n, value_type::one(), scalar_size, op_set);
            }
        } // namespace fields
    }    // namespace algebra
}    // namespace nil

#endif    // ALGEBRA_MULTIEXP_FIELDS_HPP