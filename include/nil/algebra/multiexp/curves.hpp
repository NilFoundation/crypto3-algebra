//---------------------------------------------------------------------------//
// Copyright (c) 2020 Ilia Shirobokov <i.shirobokov@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef ALGEBRA_MULTIEXP_CURVE_HPP
#define ALGEBRA_MULTIEXP_CURVE_HPP

#include <cstdint>
#include <math.h>

#include <nil/algebra/multiexp/default.hpp>

namespace nil {
    namespace algebra {
        namespace curves {
            template<typename T, typename S> 
            T eval_multi_exp_naive(typename std::vector<T>::const_iterator vec_start,
                                typename std::vector<S>::const_iterator scalar_start, 
                                const size_t n) {
                using value_type = T;
                using field_value_type = typename T::underlying_field_type_value;

                std::function<value_type (value_type, value_type)> base_op = [](value_type a, value_type b) -> value_type {
                    if (a == b) {
                        return a.doubled();
                    } else {
                        return a + b;
                    }
                };
                std::function<value_type (int, value_type)> s_op = [](int s, value_type a) -> value_type {
                    return a * s;
                };
                std::function<value_type (value_type)> dbl_op = [](value_type a) -> value_type { return a.doubled(); };
                nil::algebra::multiexp::operation_set<value_type, int> op_set(base_op, s_op, dbl_op);

                return nil::algebra::multiexp::eval_multi_exp_naive<T, S>(vec_start, scalar_start, n, value_type::zero(), op_set);
            }
            
            template<typename T, typename S>
            T eval_multi_exp(typename std::vector<T>::const_iterator vec_start,
                            typename std::vector<S>::const_iterator scalar_start, size_t num_groups,
                            const size_t bucket_size, const size_t workers_in_subgroup, const size_t n,
                            const size_t scalar_size) {
                using value_type = T;
                using field_value_type = typename T::underlying_field_type_value;

                std::function<value_type (value_type, value_type)> base_op = [](value_type a, value_type b) -> value_type {
                    if (a == b) {
                        return a.doubled();
                    } else {
                        return a + b;
                    }
                };
                std::function<value_type (int, value_type)> s_op = [](int s, value_type a) -> value_type {
                    value_type res = value_type::zero();
                    value_type acc = a;
                    while (s > 0) {
                        if (s & 1) {
                            res = res + acc;
                        }
                        acc = acc.doubled();
                        s >>= 1;
                    }
                    return res;
                };
                std::function<value_type (value_type)> dbl_op = [](value_type a) -> value_type { return a.doubled(); };
                nil::algebra::multiexp::operation_set<value_type, int> op_set(base_op, s_op, dbl_op);

                return nil::algebra::multiexp::eval_multi_exp<T, S>(vec_start, scalar_start, num_groups, bucket_size, workers_in_subgroup, n, value_type::zero(), scalar_size, op_set);
            }
        } // namespace curves
    }    // namespace algebra
}    // namespace nil

#endif    // ALGEBRA_MULTIEXP_CURVE_HPP