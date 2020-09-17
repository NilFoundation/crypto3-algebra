//---------------------------------------------------------------------------//
// Copyright (c) 2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Nikita Kaskov <nbering@nil.foundation>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//---------------------------------------------------------------------------//

#include <iostream>

#include <boost/multiprecision/cpp_modular.hpp>
#include <boost/multiprecision/number.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/modular/modular_adaptor.hpp>

#include <nil/algebra/curves/alt_bn128.hpp>
#include <nil/algebra/curves/bls12.hpp>
#include <nil/algebra/curves/bn128.hpp>
#include <nil/algebra/curves/edwards.hpp>
#include <nil/algebra/curves/mnt4.hpp>
#include <nil/algebra/curves/mnt6.hpp>

#include <nil/algebra/multiexp/curves.hpp>

using namespace nil::algebra;

template<typename FpCurveGroup>
void print_fp_curve_group_element(FpCurveGroup e) {
    std::cout << e.p[0].data << " " << e.p[1].data << " " << e.p[2].data << std::endl;
}

template<typename Fp2CurveGroup>
void print_fp2_curve_group_element(Fp2CurveGroup e) {
    std::cout << "(" << e.p[0].data[0].data << " " << e.p[0].data[1].data << ") (" << e.p[1].data[0].data << " "
              << e.p[1].data[1].data << ") (" << e.p[2].data[0].data << " " << e.p[2].data[1].data << ")" << std::endl;
}

template<typename Fp3CurveGroup>
void print_fp3_curve_group_element(Fp3CurveGroup e) {
    std::cout << "(" << e.p[0].data[0].data << " " << e.p[0].data[1].data << e.p[0].data[2].data << ") ("
              << e.p[1].data[0].data << " " << e.p[1].data[1].data << e.p[1].data[2].data << ") ("
              << e.p[2].data[0].data << " " << e.p[2].data[1].data << e.p[2].data[2].data << ")" << std::endl;
}

// print dunctions can be made using arity in fields

template<typename FpCurveGroup>
void fp_curve_group_basic_math_examples() {
    using policy_type = FpCurveGroup;
    using field_value_type = typename policy_type::underlying_field_type_value;

    field_value_type e1 = field_value_type(2), e2(3), e3(5), e4(3), e5(5), e6(7);
    policy_type c1(e1, e2, e3), c2(e4, e5, e6);

    std::cout << "Curve element values: " << std::endl;
    std::cout << "c1 value: ";
    print_fp_curve_group_element(c1);

    std::cout << "c2 value: ";
    print_fp_curve_group_element(c2);

    std::cout << "c1 + c2 value: ";
    print_fp_curve_group_element(c1 + c2);

    std::cout << "c1 - c2 value: ";
    print_fp_curve_group_element(c1 - c2);

    std::cout << "Doubled c1 value: ";
    print_fp_curve_group_element(c1.doubled());

    policy_type cd = c1.doubled();

    // policy_type cn = c1.normalize();

    // std::cout << "c1 normalized value: ";
    // print_fp_curve_group_element(cn);
}

template<typename Fp2CurveGroup>
void fp2_curve_group_basic_math_examples() {
    using policy_type = Fp2CurveGroup;
    using field_value_type = typename policy_type::underlying_field_type_value;

    policy_type c1 = policy_type::one(), c2 = policy_type::one().doubled();

    std::cout << "Curve element values: " << std::endl;
    std::cout << "c1 value: ";
    print_fp2_curve_group_element(c1);

    std::cout << "c2 value: ";
    print_fp2_curve_group_element(c2);

    std::cout << "c1 + c2 value: ";
    print_fp2_curve_group_element(c1 + c2);

    std::cout << "c1 - c2 value: ";
    print_fp2_curve_group_element(c1 - c2);

    std::cout << "Doubled c1 value: ";
    print_fp2_curve_group_element(c1.doubled());

    policy_type cd = c1.doubled();

    // policy_type cn = c1.normalize();

    // std::cout << "c1 normalized value: ";
    // print_fp2_curve_group_element(cn);
}

template<typename Fp3CurveGroup>
void fp3_curve_group_basic_math_examples() {
    using policy_type = Fp3CurveGroup;
    using field_value_type = typename policy_type::underlying_field_type_value;

    policy_type c1 = policy_type::one(), c2 = policy_type::one().doubled();

    std::cout << "Curve element values: " << std::endl;
    std::cout << "c1 value: ";
    print_fp3_curve_group_element(c1);

    std::cout << "c2 value: ";
    print_fp3_curve_group_element(c2);

    std::cout << "c1 + c2 value: ";
    print_fp3_curve_group_element(c1 + c2);

    std::cout << "c1 - c2 value: ";
    print_fp3_curve_group_element(c1 - c2);

    std::cout << "Doubled c1 value: ";
    print_fp3_curve_group_element(c1.doubled());

    policy_type cd = c1.doubled();

    // policy_type cn = c1.normalize();

    // std::cout << "c1 normalized value: ";
    // print_fp3_curve_group_element(cn);
}

template<typename T, typename S> struct multiexp_test_case {
        typename std::vector<T> bases;
        typename std::vector<S> scalars;
        size_t num_groups;
        size_t bucket_size;
        size_t workers_in_group;

        multiexp_test_case(typename std::vector<T> bases, typename std::vector<S> scalars, size_t num_groups, size_t bucket_size, size_t workers_in_group) {
            this->bases = bases;
            this->scalars = scalars;
            this->num_groups = num_groups;
            this->bucket_size = bucket_size;
            this->workers_in_group = workers_in_group;
        }
};

template<typename curveGroup>
void multi_exp_test()
{
    using value_type = curveGroup;
    using field_value_type = typename curveGroup::underlying_field_type_value;

    field_value_type x(0x14cb3e9fa81afad130934716414d5f19cc6870e5d78fb54843db588b40d0007d_cppui254);
    field_value_type y(0x283200ac370621255c2eaa2254f616a216881ce6611df778578fce5dc28107c6_cppui254);
    field_value_type z(1);
    value_type gen(x, y, z);

    multiexp_test_case<value_type, int> test1(std::vector<value_type>{gen * 500, gen * 352546561, gen * 7},
                                            std::vector<int>{65465, 757, 2}, 2, 3, 2);
    multiexp_test_case<value_type, int> test2(std::vector<value_type>(300, gen * 500),
                                            std::vector<int>(300, 37), 5, 3, 5);
    multiexp_test_case<value_type, int> test3(std::vector<value_type>(700, gen * 5641651231),
                                            std::vector<int>(700, 13), 100, 10, 5);
    multiexp_test_case<value_type, int> test4(std::vector<value_type>{gen * 3525465617},
                                            std::vector<int>{1024}, 1, 10, 1);

    std::vector<multiexp_test_case<value_type, int>> tests{ test1, test2, test3, test4};

    for (size_t i = 0; i < tests.size(); i++) {
        typename std::vector<value_type> bases = tests[i].bases;
        std::vector<int> scalars = tests[i].scalars;

        typename std::vector<value_type>::const_iterator bases_iter = bases.begin();
        std::vector<int>::const_iterator scalaras_iter = scalars.begin();

        value_type res = nil::algebra::curves::eval_multi_exp<value_type, int>(bases_iter, scalaras_iter, tests[i].num_groups, tests[i].bucket_size, tests[i].workers_in_group, bases.size(), 31);
        value_type naive_res = nil::algebra::curves::eval_multi_exp_naive<value_type, int>(bases_iter, scalaras_iter, bases.size());

        if (res == naive_res) {
            std::cout << "Test " << i << " OK" << std::endl;
        }
        else {
            std::cout << "Test " << i << " FAILED" << std::endl;
        }   
    }
}

int main() {
    /*std::cout << "ALT_BN128-254 curve g1 group basic math:" << std::endl;
    fp_curve_group_basic_math_examples<curves::alt_bn128<254>::g1_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "ALT_BN128-254 curve g2 group basic math:" << std::endl;
    fp2_curve_group_basic_math_examples<curves::alt_bn128<254>::g2_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "BLS12-381 curve g1 group basic math:" << std::endl;
    fp_curve_group_basic_math_examples<curves::bls12<381>::g1_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "BLS12-381 curve g2 group basic math:" << std::endl;
    fp2_curve_group_basic_math_examples<curves::bls12<381>::g2_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "BLS12-377 curve g1 group basic math:" << std::endl;
    fp_curve_group_basic_math_examples<curves::bls12<377>::g1_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "BLS12-377 curve g2 group basic math:" << std::endl;
    fp2_curve_group_basic_math_examples<curves::bls12<377>::g2_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "BN128-254 curve g1 group basic math:" << std::endl;
    fp_curve_group_basic_math_examples<curves::bn128<254>::g1_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "BN128-254 curve g2 group basic math:" << std::endl;
    fp2_curve_group_basic_math_examples<curves::bn128<254>::g2_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "Edwards curve g1 group basic math:" << std::endl;
    fp_curve_group_basic_math_examples<curves::edwards<183>::g1_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "Edwards curve g2 group basic math:" << std::endl;
    fp3_curve_group_basic_math_examples<curves::edwards<183>::g2_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "Mnt4 curve g1 group basic math:" << std::endl;
    fp_curve_group_basic_math_examples<curves::mnt4<298>::g1_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "Mnt4 curve g2 group basic math:" << std::endl;
    fp2_curve_group_basic_math_examples<curves::mnt4<298>::g2_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "Mnt6 curve g1 group basic math:" << std::endl;
    fp_curve_group_basic_math_examples<curves::mnt6<298>::g1_type>();

    std::cout << "----------------------------" << std::endl;

    std::cout << "Mnt6 curve g2 group basic math:" << std::endl;
    fp3_curve_group_basic_math_examples<curves::mnt6<298>::g2_type>();

    std::cout << "----------------------------" << std::endl;*/

    std::cout << "----------------------------" << std::endl;

    std::cout << "MultiExpoinentation BN128: " << std::endl;

    multi_exp_test<curves::bn128<254>::g1_type>();

    return 0;
}