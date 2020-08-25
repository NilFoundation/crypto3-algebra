//---------------------------------------------------------------------------//
// Copyright (c) 2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Pavel Kharitonov <ipavrus@nil.foundation>
// Copyright (c) 2020 Nikita Kaskov <nbering@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#ifndef ALGEBRA_MULTIEXP_HPP
#define ALGEBRA_MULTIEXP_HPP

#include <cstdint>
#include <math.h>

#include <nil/algebra/fields/detail/element/fp.hpp>

#include <nil/algebra/fields/detail/exponentiation.hpp>

namespace nil {
    namespace algebra {
        template<typename T, typename S> struct operation_set {
            std::function<T (T, T)> base_op;
            std::function<T (S, T)> scalar_op;
            std::function<T (T)> dbl_op;

            operation_set(std::function<T (T, T)> base_op, std::function<T (S, T)> scalar_op,
                          std::function<T (T)> dbl_op) {
                this->base_op = base_op;
                this->scalar_op = scalar_op;
                this->dbl_op = dbl_op;
            }
        };

        template<typename T, typename S> 
        T eval_multi_exp_naive(typename std::vector<T>::const_iterator vec_start,
                               typename std::vector<S>::const_iterator scalar_start, 
                               const size_t n, const T one, operation_set<T, S> op ) {
            T res = one;

            for (int i = 0; i < n; ++i) {
                res = op.base_op(res, op.scalar_op(*(scalar_start + i), *(vec_start + i)));
            }

            return res;
        }

        template<typename S>
        S get_bits(S scalar, const size_t start, const size_t end) {
            S mask = S(pow(2, end - start)) - S(1); // TO DO: MAKE NORMAL POWER HERE
            S res = (scalar >> start) & mask;
            return res;
        }

        template<typename T, typename S>
        std::vector<T> multi_exp_subgroup(typename std::vector<T>::const_iterator vec_start,
                                          typename std::vector<S>::const_iterator scalar_start, const size_t start,
                                          const size_t end, const size_t bucket_size, size_t workers_amount,
                                          const size_t n, const T one, const size_t scalar_size, operation_set<T, S> op) {
            size_t b = std::ceil((double)scalar_size / bucket_size);
            size_t c = std::ceil((double)b / workers_amount);

            if (workers_amount > b) {
                workers_amount = b;
            }

            typename std::vector<T> part_sum(b, one);

            size_t buckets_len = (size_t)pow(2, bucket_size);

            // do parallel for j
            for (size_t j = 0; j < workers_amount; ++j) {
                for (size_t k = 0; k <= c - 1; ++k) {

                    size_t bucket_start = j * bucket_size * c + k * bucket_size;
                    typename std::vector<T> buckets(buckets_len, one);

                    for (size_t i = start; i <= end; ++i) {
                        size_t idx = get_bits<S>(*(scalar_start + i), bucket_start, bucket_start + bucket_size);

                        if (idx > 0) {
                            buckets[idx - 1] = op.base_op((*(vec_start + i)), buckets[idx - 1]);
                        }
                    }

                    T acc = one;

                    for (size_t i = 0; i < buckets_len; ++i) {
                        acc = op.base_op(acc, buckets[buckets_len - i - 1]);
                        part_sum[j * c + k] = op.base_op(part_sum[j * c + k], acc);
                    }
                }
            }
            return part_sum;
        }

        template<typename T, typename S>
        T sum_serial(typename std::vector<std::vector<T>> elements, size_t idx, T one, operation_set<T, S> op) {
            T res = elements[0][idx];
            for (int i = 1; i < elements.capacity(); ++i) {
                res = op.base_op(res, elements[i][idx]);
            }

            return res;
        }

        template<typename T, typename S>
        T sum_parallel(typename std::vector<std::vector<T>> elements, size_t idx, T one, operation_set<T, S> op) {
            size_t n = elements.capacity();
            size_t log_n = std::log2(n);
            size_t h = std::floor((double)n / log_n);

            typename std::vector<T> part_res(h, one);

            // do parallel for i
            for (size_t i = 0; i <= h - 1; ++i) {
                for (size_t j = 0; j <= log_n - 1; ++j) {
                    part_res[i] = op.base_op(part_res[i], elements[i * log_n + j][idx]);
                }
            }

            size_t m = std::ceil((double)n / log_n);

            while (m > log_n + 1) {
                h = std::ceil(std::log2(m));

                // do parallel for i
                for (size_t i = 0; i <= (m / h) - 1; ++i) {
                    size_t d = h - 1;
                    if (i == (m / h) - 1) {
                        d = m - 1 - i * h;
                    }

                    for (size_t j = 1; j <= d; ++j) {
                        part_res[i * h] = op.base_op(part_res[i * h], part_res[i * h + j]);
                    }
                    part_res[i] = part_res[i * h];
                }

                m = std::ceil((double)m / h);
            }

            for (size_t i = 1; i <= m - 1; ++i) {
                part_res[0] = op.base_op(part_res[0], part_res[i]);
            }

            return part_res[0];
        }

        template<typename T, typename S>
        T result_aggregation(typename std::vector<std::vector<T>> &r, const size_t L, const size_t num_groups, const size_t bucket_size, const T one,
                            operation_set<T, S> op) {
            typename std::vector<T> part_res(L, one);
            std::cout<< r.capacity() << std::endl;
            for (size_t i = 0; i < L; ++i) {
                if (r.capacity() >= 50) {
                    part_res[i] = sum_serial(r, i, one, op);
                }
                else {
                    part_res[i] = sum_serial(r, i, one, op);
                }
            }

            T res = one;

            for (size_t i = 0; i < L; ++i) {
                for (int j = 0; j < bucket_size; j++) {
                    res = op.dbl_op(res);
                }
                res = op.base_op(res, part_res[L - i - 1]);
            }

            return res;
        }


        template<typename T, typename S>
        T eval_multi_exp(typename std::vector<T>::const_iterator vec_start,
                         typename std::vector<S>::const_iterator scalar_start, size_t num_groups,
                         const size_t bucket_size, const size_t workers_in_subgroup, const size_t n, const T one,
                         const size_t scalar_size, operation_set<T, S> op) {
            size_t chunk_len = std::ceil((double)n / num_groups);

            if (n < num_groups) {
                num_groups = n;
            }

            std::vector<std::vector<T>> part_res(num_groups);

            // do parallel for j
            for (size_t j = 0; j < num_groups; ++j) {
                size_t start = j * chunk_len;
                size_t end = std::min(start + chunk_len - 1, n - 1);
                part_res[j] = multi_exp_subgroup<T, S>(vec_start, scalar_start, start, end, bucket_size, workers_in_subgroup, n, one, scalar_size, op);
            }

            size_t L = std::ceil((double)scalar_size / bucket_size);
            return result_aggregation<T, S>(part_res, L, num_groups, bucket_size, one, op);
        }

    }    // namespace algebra
}    // namespace nil

#endif    // BOOST_MULTIPRECISION_MULTIEXP_HPP