//---------------------------------------------------------------------------//
// Copyright (c) 2020-2021 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020-2021 Nikita Kaskov <nbering@nil.foundation>
// Copyright (c) 2020-2021 Ilias Khairullin <ilias@nil.foundation>
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

#define BOOST_TEST_MODULE algebra_curves_test

#include <iostream>
#include <vector>
#include <array>

#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <nil/crypto3/multiprecision/cpp_int.hpp>

#include <nil/crypto3/algebra/curves/alt_bn128.hpp>
#include <nil/crypto3/algebra/curves/bls12.hpp>
#include <nil/crypto3/algebra/curves/mnt4.hpp>
#include <nil/crypto3/algebra/curves/mnt6.hpp>

#include <nil/crypto3/algebra/pairing/alt_bn128.hpp>
#include <nil/crypto3/algebra/pairing/bls12.hpp>
#include <nil/crypto3/algebra/pairing/mnt4.hpp>
#include <nil/crypto3/algebra/pairing/mnt6.hpp>

#include <nil/crypto3/algebra/algorithms/pair.hpp>
#include <nil/crypto3/algebra/random_element.hpp>

#include <nil/crypto3/algebra/fields/detail/element/fp.hpp>
#include <nil/crypto3/algebra/fields/detail/element/fp4.hpp>
#include <nil/crypto3/algebra/fields/detail/element/fp6_2over3.hpp>
#include <nil/crypto3/algebra/fields/detail/element/fp6_3over2.hpp>
#include <nil/crypto3/algebra/fields/detail/element/fp12_2over3over2.hpp>

using namespace nil::crypto3::algebra::pairing;
using namespace nil::crypto3::algebra;
using namespace nil::crypto3::multiprecision;

template<typename FieldParams>
void print_field_element(std::ostream &os, const typename fields::detail::element_fp<FieldParams> &e) {
//    os << std::hex <<"0x"<< std::setw((FieldParams::modulus_bits+7)/4) << std::setfill('0') << e.data << "_cppui" << std::dec << FieldParams::modulus_bits << " ";
    os << '"' << e.data << '"' ;
}

template<typename FieldParams>
void print_field_element(std::ostream &os, const typename fields::detail::element_fp2<FieldParams> &e) {
    os << "[";
    print_field_element(os, e.data[0]);
    os << ", ";
    print_field_element(os, e.data[1]);
    os << "]";
}

template<typename FieldParams>
void print_field_element(std::ostream &os, const typename fields::detail::element_fp3<FieldParams> &e) {
    os << "[";
    print_field_element(os, e.data[0]);
    os << ", ";
    print_field_element(os, e.data[1]);
    os << ", ";
    print_field_element(os, e.data[2]);
    os << "]";
}

template<typename FieldParams>
void print_field_element(std::ostream &os, const typename fields::detail::element_fp4<FieldParams> &e) {
    os << "[";
    print_field_element(os, e.data[0]);
    os << ", ";
    print_field_element(os, e.data[1]);
    os << "]";
}

template<typename FieldParams>
void print_field_element(std::ostream &os, const typename fields::detail::element_fp6_2over3<FieldParams> &e) {
    os << "[";
    print_field_element(os, e.data[0]);
    os << ", ";
    print_field_element(os, e.data[1]);
    os << "]";
}

template<typename FieldParams>
void print_field_element(std::ostream &os, const typename fields::detail::element_fp6_3over2<FieldParams> &e) {
    os << "[";
    print_field_element(os, e.data[0]);
    os << ", ";
    print_field_element(os, e.data[1]);
    os << ", ";
    print_field_element(os, e.data[2]);
    os << "]";
}


template<typename FieldParams>
void print_field_element(std::ostream &os, const fields::detail::element_fp12_2over3over2<FieldParams> &e) {
    os << "[";
    print_field_element(os, e.data[0]);
    os << ", ";
    print_field_element(os, e.data[1]);
    os << "]";
}

template<typename CurveGroupValue>
void print_curve_group_element_affine(std::ostream &os, const CurveGroupValue &e) {
    os << "[";
    print_field_element(os, e.X);
    os << ",";
    print_field_element(os, e.Y);
    os << "]";
    /*
    auto affine = e.to_affine();
    os << "affine (";
    print_field_element(os, affine.X);
    os << ",";
    print_field_element(os, affine.Y);
 
    os << "), projective (";
    print_field_element(os, e.X);
    os << ",";
    print_field_element(os, e.Y);
    os << ",";
    print_field_element(os, e.Z);
    os << ")"
    */
    /* << std::endl*/;
}


template<typename CurveGroupValue>
void print_curve_group_element(std::ostream &os, const CurveGroupValue &e) {
    os << "[";
    print_field_element(os, e.X);
    os << ",";
    print_field_element(os, e.Y);
    os << ",";
    print_field_element(os, e.Z);
    os << "]";
    /*
    auto affine = e.to_affine();
    os << "affine (";
    print_field_element(os, affine.X);
    os << ",";
    print_field_element(os, affine.Y);
 
    os << "), projective (";
    print_field_element(os, e.X);
    os << ",";
    print_field_element(os, e.Y);
    os << ",";
    print_field_element(os, e.Z);
    os << ")"
    */
    /* << std::endl*/;
}

void print_g1_precomp_element(std::ostream &os, const typename pairing::pairing_policy<curves::alt_bn128<254>>::g1_precomputed_type &e) {
    os << "{\"PX\": ";
    print_field_element(os, e.PX);
    os << ", \"PY\": ";
    print_field_element(os, e.PY);
    os << "}" << std::endl;
}

void print_g1_precomp_element(std::ostream &os, const typename pairing::pairing_policy<curves::bls12<381>>::g1_precomputed_type &e) {
    os << "{\"PX\": ";
    print_field_element(os, e.PX);
    os << ", \"PY\": ";
    print_field_element(os, e.PY);
    os << "}" << std::endl;
}

void print_g1_precomp_element(std::ostream &os, const typename pairing::pairing_policy<curves::bls12<377>>::g1_precomputed_type &e) {
    os << "{\"PX\": ";
    print_field_element(os, e.PX);
    os << ", \"PY\": ";
    print_field_element(os, e.PY);
    os << "}" << std::endl;
}

void print_g1_precomp_element(std::ostream &os, const typename pairing::pairing_policy<curves::mnt4<298>>::g1_precomputed_type &e) {
    os << "{\"PX\": ";
    print_field_element(os, e.PX);
    os << ", \"PY\": ";
    print_field_element(os, e.PY);
    os << ", \"PX_twist\": ";
    print_field_element(os, e.PX_twist);
    os << ", \"PY_twist\": ";
    print_field_element(os, e.PY_twist);
    os << "}" << std::endl;
}

void print_g1_precomp_element(std::ostream &os, const typename pairing::pairing_policy<curves::mnt6<298>>::g1_precomputed_type &e) {
    os << "{\"PX\": ";
    print_field_element(os, e.PX);
    os << ", \"PY\": ";
    print_field_element(os, e.PY);
    os << ", \"PX_twist\": ";
    print_field_element(os, e.PX_twist);
    os << ", \"PY_twist\": ";
    print_field_element(os, e.PY_twist);
    os << "}" << std::endl;
}

template<typename curve>
void print_g2_precomp_element_ell(
        std::ostream &os, 
        const typename pairing::pairing_policy<curve>::g2_precomputed_type &e) 
{
    os << "{" << std::endl;
    os << "\"coordinates\": [" << std::endl;
    print_field_element(os, e.QX);
    os << "," << std::endl;
    print_field_element(os, e.QY);
    os << "]," << std::endl;

    auto print_coeff = [&os](const auto &c) {
        os << "{" << std::endl;
        os << "\"ell_0\":";
        print_field_element(os, c.ell_0);
        os << "," << std::endl;
        os << "\"ell_VW\":";
        print_field_element(os, c.ell_VW);
        os << "," << std::endl;
        os << "\"ell_VV\":";
        print_field_element(os, c.ell_VV);
        os << "}" << std::endl;
    };
    os << "\"coefficients\": [" << std::endl;
    for (auto c = e.coeffs.begin(); c != e.coeffs.end(); ++c) {
        print_coeff(*c);
        if (c != e.coeffs.end()-1)
            os << ",";
        os << std::endl;
    }
    os << "]" << std::endl;
    os << "}" << std::endl;
}

void print_g2_precomp_element(
        std::ostream &os, 
        const typename pairing::pairing_policy<curves::alt_bn128<254>>::g2_precomputed_type &e) 
{
    print_g2_precomp_element_ell<curves::alt_bn128<254>>(os, e);
    /*
    os << "{" << std::endl;
    os << "\"coordinates\": [" << std::endl;
    print_field_element(os, e.QX);
    os << "," << std::endl;
    print_field_element(os, e.QY);
    os << "]," << std::endl;

    auto print_coeff = [&os](const auto &c) {
        os << "{" << std::endl;
        os << "\"ell_0\":";
        print_field_element(os, c.ell_0);
        os << "," << std::endl;
        os << "\"ell_VW\":";
        print_field_element(os, c.ell_VW);
        os << "," << std::endl;
        os << "\"ell_VV\":";
        print_field_element(os, c.ell_VV);
        os << "}" << std::endl;
    };
    os << "\"coefficients\": [" << std::endl;
    for (auto c = e.coeffs.begin(); c != e.coeffs.end(); ++c) {
        print_coeff(*c);
        if (c != e.coeffs.end()-1)
            os << ",";
        os << std::endl;
    }
    os << "]" << std::endl;
    os << "}" << std::endl;
    */
}

void print_g2_precomp_element(std::ostream &os,
        const typename pairing::pairing_policy<curves::bls12<381>>::g2_precomputed_type &e)
{
    print_g2_precomp_element_ell<curves::bls12<381>>(os, e);
}

void print_g2_precomp_element(std::ostream &os,
        const typename pairing::pairing_policy<curves::bls12<377>>::g2_precomputed_type &e)
{
    print_g2_precomp_element_ell<curves::bls12<377>>(os, e);
}

template<typename CurveType>
void print_g2_precomp_element(std::ostream &os, const typename pairing::pairing_policy<CurveType /*curves::mnt4<298>*/>::g2_precomputed_type &e) {
    os << "{" << std::endl;

    os << "\"coordinates\": {" << std::endl;
    os << "\"QX\": ";            print_field_element(os, e.QX);            os << "," << std::endl;
    os << "\"QY\": ";            print_field_element(os, e.QY);            os << "," << std::endl;
    os << "\"QY2\": ";           print_field_element(os, e.QY2);           os << "," << std::endl;
    os << "\"QX_over_twist\": "; print_field_element(os, e.QX_over_twist); os << "," << std::endl;
    os << "\"QY_over_twist\": "; print_field_element(os, e.QY_over_twist); os << std::endl;
    os << "}," << std::endl;

    auto print_dbl_coeff = [&os](const auto &c) {
        os << "{\"c_H\": ";
        print_field_element(os, c.c_H);
        os << ", \"c_4C\": ";
        print_field_element(os, c.c_4C);
        os << ", \"c_J\": ";
        print_field_element(os, c.c_J);
        os << ", \"c_L\": ";
        print_field_element(os, c.c_L);
        os << "}" << std::endl;
    };
    auto print_add_coeff = [&os](const auto &c) {
        os << "{\"c_L1\": ";
        print_field_element(os, c.c_L1);
        os << ", \"c_RZ\": ";
        print_field_element(os, c.c_RZ);
        os << "}" << std::endl;
    };

    os << "\"dbl_coeffs\": ["<< std::endl;;
    int c;
    for (c = 0; c< e.dbl_coeffs.size(); ++c){
        print_dbl_coeff(e.dbl_coeffs[c]);
        if ( c != e.dbl_coeffs.size()-1 ) {
            os << "," << std::endl;
        }
    }
    os << "]," << std::endl;

    os << "\"add_coeffs\": [";
    for (c = 0; c< e.add_coeffs.size(); ++c){
        print_add_coeff(e.add_coeffs[c]);
        if ( c != e.add_coeffs.size()-1 ) {
            os << "," << std::endl;
        }
    }
    os << "]" << std::endl;
    os << "}" << std::endl;
}

/*
void print_g2_precomp_element(std::ostream &os, const typename pairing::pairing_policy<curves::mnt6<298>>::g2_precomputed_type &e) {
    os << "{" << std::endl;

    os << "\"coordinates\": {" << std::endl << "\"QX\": ";
    print_field_element(os, e.QX);
    os << "," << std::endl << "\"QY\": ";
    print_field_element(os, e.QY);
    os << "," << std::endl << "\"QY2\": ";
    print_field_element(os, e.QY2);
    os << "," << std::endl << "\"QX_over_twist\": ";
    print_field_element(os, e.QX_over_twist);
    os << "," << std::endl << "\"QY_over_twist\": ";
    print_field_element(os, e.QY_over_twist);
    os << "}" << std::endl;

    auto print_dbl_coeff = [&os](const auto &c) {
        os << "{\"c_H\": ";
        print_field_element(os, c.c_H);
        os << ", \"c_4C\": ";
        print_field_element(os, c.c_4C);
        os << ", \"c_J\": ";
        print_field_element(os, c.c_J);
        os << ", \"c_L\": ";
        print_field_element(os, c.c_L);
        os << "}" << std::endl;
    };
    auto print_add_coeff = [&os](const auto &c) {
        os << "{\"c_L1\": ";
        print_field_element(os, c.c_L1);
        os << ", \"c_RZ\": ";
        print_field_element(os, c.c_RZ);
        os << "}" << std::endl;
    };

    os << "\"dbl_coeffs\": [" << std::endl ;
    for (int c = 0; c < e.dbl_coeffs.size(); ++c ) {
        print_dbl_coeff(e.dbl_coeffs[c]);
        if (c!=e.dbl_coeffs.size()-1)
            os << "," << std::endl;
    }
    os << "]," << std::endl;

    os << "\"add_coeffs\": [" << std::endl;
    for (int c = 0; c < e.add_coeffs.size(); ++c ) {
        print_add_coeff(e.add_coeffs[c]);
        if (c!=e.add_coeffs.size()-1)
            os << "," << std::endl;
    }
    os << "]" << std::endl;
    os << "}" << std::endl;
}
*/
namespace boost {
    namespace test_tools {
        namespace tt_detail {
            template<typename FieldParams>
            struct print_log_value<typename fields::detail::element_fp<FieldParams>> {
                void operator()(std::ostream &os, typename fields::detail::element_fp<FieldParams> const &e) {
                    print_field_element(os, e);
                    std::cout << std::endl;
                }
            };

            template<typename FieldParams>
            struct print_log_value<typename fields::detail::element_fp2<FieldParams>> {
                void operator()(std::ostream &os, typename fields::detail::element_fp2<FieldParams> const &e) {
                    print_field_element(os, e);
                    std::cout << std::endl;
                }
            };

            template<>
            struct print_log_value<curves::alt_bn128<254>::g1_type<>::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::alt_bn128<254>::g1_type<>::value_type &e) {
                    print_curve_group_element(os, e);
                }
            };

            template<>
            struct print_log_value<curves::alt_bn128<254>::g2_type<>::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::alt_bn128<254>::g2_type<>::value_type &e) {
                    print_curve_group_element(os, e);
                }
            };


            template<>
            struct print_log_value<curves::bls12<381>::g1_type<>::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::bls12<381>::g1_type<>::value_type &e) {
                    print_curve_group_element(os, e);
                }
            };

            template<>
            struct print_log_value<curves::bls12<381>::g2_type<>::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::bls12<381>::g2_type<>::value_type &e) {
                    print_curve_group_element(os, e);
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::alt_bn128<254>>::g1_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::alt_bn128<254>>::g1_precomputed_type &e) {
                    print_g1_precomp_element(os, e);
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::alt_bn128<254>>::g2_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::alt_bn128<254>>::g2_precomputed_type &e) {
                    print_g2_precomp_element(os, e);
                }
            };

            template<>
            struct print_log_value<curves::alt_bn128<254>::gt_type::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::alt_bn128<254>::gt_type::value_type &e) {
                    print_field_element(os, e);
                    std::cout << std::endl;
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::bls12<381>>::g1_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::bls12<381>>::g1_precomputed_type &e) {
                    print_g1_precomp_element(os, e);
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::bls12<381>>::g2_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::bls12<381>>::g2_precomputed_type &e) {
                    print_g2_precomp_element(os, e);
                }
            };

            template<>
            struct print_log_value<curves::bls12<381>::gt_type::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::bls12<381>::gt_type::value_type &e) {
                    print_field_element(os, e);
                    std::cout << std::endl;
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::bls12<377>>::g1_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::bls12<377>>::g1_precomputed_type &e) {
                    print_g1_precomp_element(os, e);
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::bls12<377>>::g2_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::bls12<377>>::g2_precomputed_type &e) {
                    print_g2_precomp_element(os, e);
                }
            };

            template<>
            struct print_log_value<curves::bls12<377>::gt_type::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::bls12<377>::gt_type::value_type &e) {
                    print_field_element(os, e);
                    std::cout << std::endl;
                }
            };

            template<>
            struct print_log_value<curves::mnt4<298>::g1_type<>::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::mnt4<298>::g1_type<>::value_type &e) {
                    print_curve_group_element(os, e);
                }
            };

            template<>
            struct print_log_value<curves::mnt4<298>::g2_type<>::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::mnt4<298>::g2_type<>::value_type &e) {
                    print_curve_group_element(os, e);
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::mnt4<298>>::g1_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::mnt4<298>>::g1_precomputed_type &e) {
                    print_g1_precomp_element(os, e);
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::mnt4<298>>::g2_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::mnt4<298>>::g2_precomputed_type &e) {
                    print_g2_precomp_element<curves::mnt4<298>>(os, e);
                }
            };

            template<>
            struct print_log_value<curves::mnt4<298>::gt_type::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::mnt4<298>::gt_type::value_type &e) {
                    print_field_element(os, e);
                    std::cout << std::endl;
                }
            };

            template<>
            struct print_log_value<curves::mnt6<298>::g1_type<>::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::mnt6<298>::g1_type<>::value_type &e) {
                    print_curve_group_element(os, e);
                }
            };

            template<>
            struct print_log_value<curves::mnt6<298>::g2_type<>::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::mnt6<298>::g2_type<>::value_type &e) {
                    print_curve_group_element(os, e);
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::mnt6<298>>::g1_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::mnt6<298>>::g1_precomputed_type &e) {
                    print_g1_precomp_element(os, e);
                }
            };

            template<>
            struct print_log_value<pairing::pairing_policy<curves::mnt6<298>>::g2_precomputed_type> {
                void operator()(std::ostream &os, const typename pairing::pairing_policy<curves::mnt6<298>>::g2_precomputed_type &e) {
                    print_g2_precomp_element<curves::mnt6<298>>(os, e);
                }
            };

            template<>
            struct print_log_value<curves::mnt6<298>::gt_type::value_type> {
                void operator()(std::ostream &os,
                                const typename curves::mnt6<298>::gt_type::value_type &e) {
                    print_field_element(os, e);
                    std::cout << std::endl;
                }
            };

            template<template<typename, typename> class P, typename K, typename V>
            struct print_log_value<P<K, V>> {
                void operator()(std::ostream &, P<K, V> const &) {
                }
            };
        }    // namespace tt_detail
    }        // namespace test_tools
}    // namespace boost



// TODO: add affine_pair_reduceding test
template<typename CurveType>
void check_pairing_operations() {

    using curve_type = CurveType;
    using scalar_field_type = typename curve_type::scalar_field_type;
    using base_field_type = typename curve_type::base_field_type;
    using g1_type = typename curve_type::template g1_type<>;
    using g2_type = typename curve_type::template g2_type<>;
    using gt_type = typename curve_type::gt_type;
    using g1_field_value_type = typename g1_type::field_type::value_type;
    using g2_field_value_type = typename g2_type::field_type::value_type;
    using gt_field_value_type = typename gt_type::value_type;
    using integral_type = typename base_field_type::integral_type;

#if 0
    gt_field_value_type T( {0,0},{1,0});
//    g2_field_value_type T(0,1,0);
    auto Q = T.pow(integral_type("3"));

    std::cout << "T: " << std::endl;
    print_field_element(std::cout, T);
    std::cout << std::endl;

    std::cout << "Q: " << std::endl;
    print_field_element(std::cout, Q);
    std::cout << std::endl;

/*
    std::cout << "G2.a: " << std::endl;
    print_field_element(std::cout, g2_type::params_type::a);
    std::cout << std::endl;

    std::cout << "G2.b: " << std::endl;
    print_field_element(std::cout, g2_type::params_type::b);
    std::cout << std::endl;

    auto p = base_field_type::modulus;
    auto group_order_minus_one_half = (base_field_type::modulus*base_field_type::modulus - 1)/2;
    std::cout << "P         : " << std::hex << base_field_type::modulus << std::endl;
    std::cout << "(P^2-1)/2 : " << std::hex << group_order_minus_one_half << std::endl;

    auto nr_check = g2_type::params_type::b;
    auto b = g2_type::params_type::b;

    auto tmp = b.pow((p-1)/2);

    nr_check = tmp.pow(p).pow(p);
    nr_check = nr_check*tmp.pow(p)*tmp;


    std::cout << "nr_check: " << std::endl;
    print_field_element(std::cout, nr_check);
    std::cout << std::endl;

    auto bsq = g2_type::params_type::b.sqrt();
    std::cout << "bsq: " << std::endl;
    print_field_element(std::cout, bsq);
    std::cout << std::endl;

    return;
*/

    return;
#endif

/*
    std::cout << "G2 generator:" << std::endl;
    print_curve_group_element_affine(std::cout, g2_type::value_type::one());
    std::cout << std::endl;
*/

    typename scalar_field_type::value_type
        VKx_poly, VKy_poly, VKz_poly,
        A1_poly, B1_poly, C1_poly,
        A2_poly, B2_poly, C2_poly;

#if 0
    /* generae random elements */
    A1_poly = random_element<scalar_field_type>();
    B1_poly = random_element<scalar_field_type>();

    A2_poly = random_element<scalar_field_type>();
    B2_poly = random_element<scalar_field_type>();

    VKx_poly = random_element<scalar_field_type>();
    VKy_poly = random_element<scalar_field_type>();
    VKz_poly = random_element<scalar_field_type>();
    
#else 

#if 0
    /* BN254 */
    A1_poly  = 0x0301b09c33d058dd7fc7a14abffac7c865c60a275c6d120b96cc030024706a5c6_cppui254 ;
    B1_poly  = 0x00092be7c111ccfeea8114c6a413f9ee67cafa5c02c407e2bd201a6fb23b4524f_cppui254 ;

    A2_poly  = 0x009d14c2cdb824c9d7dc7a958b47fd46866ffd0fb7c666acb9f8e492bdf68f1b6_cppui254 ;
    B2_poly  = 0x01e6cf977b9ac00a6058e67ddab4f3d75d880ef634e7d37cd804079c84e4dd6fa_cppui254 ;

    VKx_poly = 0x017ae0b5573ae7e2ffc4fcdbe4a742b753358b2f0fee7ccf50a8d305499941024_cppui254 ;
    VKy_poly = 0x01b599486661da6781b3bc0bd3a73d7c97a56a4e657abc1699664b64f2c274705_cppui254 ;
    VKz_poly = 0x0134ca23d099a7110cb8911d68be121d8d0cd634990b46b169d01b65f14c46b23_cppui254 ;
#endif

#if 0
    /* BLS12-377 */
    VKx_poly = 0x5a0e34f4a03c673d8b257ea42d40d0db1ae777ebfaa7938be4d8076f2cf7c86_cppui377;
    VKy_poly = 0x66cc192146457adf44fa020cd2e8db8ce5854f5716995567945a901bc309df4_cppui377;
    VKz_poly = 0x126a4821fe71343a3f6134bb6abf81e56acc1a452f3dd3fec391eee89fd2c137_cppui377;
    A1_poly  = 0x4cb26a8ba2b71e86653f87955a42dc9055cc5183dc141f001b98a3be4743ddf_cppui377;
    B1_poly  = 0x2d3f2f86be37bc487b0b946bc278e417b9d78afcb62b3b79f951f858574750d_cppui377;
    C1_poly  = 0x1090d68251f4c61eee85adde85d2320709b883f1248e47c034a0a5fab20e7dbd_cppui377;
    A2_poly  = 0x11ab20238a2b0f143630446f4d524192f15e06b205c355275dbcd165d3ca9637_cppui377;
    B2_poly  = 0x6bf7648cfa789ee0d54903bc66dd745de612921185b85c951e3900c0f060ca7_cppui377;
    C2_poly  = 0x11120c1a5cf0f0243007df5b5bd653794ddec6538b20fd2732fb1b122db5ac2a_cppui377;
#endif

#if 1
    /* MNT4-298 */
    VKx_poly = 0x29794dc75706f12ea30a7fdeb9d43af81a45d68a7be01d297c78c778deec9cb93387ebe12bc_cppui298;
    VKy_poly = 0x1a29303c45031f874d683968a22ff4942e2f136fada87be10586433ed17f7bf23d151e43bf7_cppui298;
    VKz_poly = 0x2106f98adc98d263494efdcb141d9ec3194f189acae2c0d61c49b6e6e8df679a5a18c2355d1_cppui298;
    A1_poly  = 0x164b4a3d781b93b4f88c3800cb2aa593ae68c5fe6216a72c08d853dabe2e163608e1d720c30_cppui298;
    B1_poly  = 0x0609583bdd59f71f1e1816ee242e690395d534930d7b9745ba17ef4c1255fcc075b5a3d09d3_cppui298;
    C1_poly  = 0x212ea84d9b5f3fdda38dfdc5cbb84c330e69581dd21dbc92215c6693dcd0c4af1776c2b3c8d_cppui298;
    A2_poly  = 0x2ff6d5800c1a73bf1762fa4a2b1ee7ff5b28b1c59951fc2c6c75043e81ec2f4fe19ff3b0be7_cppui298;
    B2_poly  = 0x18de2898617bf7ff59c846b91b20d9ed688f722a8d071da97950a3bb35184c032e43a6ecb78_cppui298;
    C2_poly  = 0x3a4ead2b787501b521a04a42ef7381ca65a6d4d6e383d1f87e72cd7dc5a83beb12b7acf63ba_cppui298;
#endif


#endif

    C1_poly = (A1_poly * B1_poly - VKx_poly * VKy_poly) * VKz_poly.inversed();
    C2_poly = (A2_poly * B2_poly - VKx_poly * VKy_poly) * VKz_poly.inversed();
    /*

    std::cerr << "A1_poly  = "; print_field_element(std::cerr, A1_poly ); std::cerr << ";" << std::endl;
    std::cerr << "B1_poly  = "; print_field_element(std::cerr, B1_poly ); std::cerr << ";" << std::endl;
    std::cerr << "C1_poly  = "; print_field_element(std::cerr, C1_poly ); std::cerr << ";" << std::endl;

    std::cerr << "A2_poly  = "; print_field_element(std::cerr, A2_poly ); std::cerr << ";" << std::endl;
    std::cerr << "B2_poly  = "; print_field_element(std::cerr, B2_poly ); std::cerr << ";" << std::endl;
    std::cerr << "C2_poly  = "; print_field_element(std::cerr, C2_poly ); std::cerr << ";" << std::endl;

    std::cerr << "VKx_poly = "; print_field_element(std::cerr, VKx_poly); std::cerr << ";" << std::endl;
    std::cerr << "VKy_poly = "; print_field_element(std::cerr, VKy_poly); std::cerr << ";" << std::endl;
    std::cerr << "VKz_poly = "; print_field_element(std::cerr, VKz_poly); std::cerr << ";" << std::endl;
    */

    std::cout << "consistency checks:" << std::endl;
    auto r = (A1_poly * B1_poly - VKx_poly * VKy_poly) * VKz_poly.inversed();
    std::cout << "r       = "; print_field_element(std::cout, r); std::cout << std::endl;
    std::cout << "C1_poly = "; print_field_element(std::cout, C1_poly); std::cout << std::endl;

    if(r == C1_poly) {
        std::cout << "C1 check [32;1mSUCCESSFUL[0m!" << std::endl;
    } else {
        std::cout << "C1 check [31;1mUNSUCCESSFUL[0m!" << std::endl;
    }

    r = (A2_poly * B2_poly - VKx_poly * VKy_poly) * VKz_poly.inversed();
    std::cout << "r       = "; print_field_element(std::cout, r); std::cout << std::endl;
    std::cout << "C2_poly = "; print_field_element(std::cout, C2_poly); std::cout << std::endl;
    if(r == C2_poly) {
        std::cout << "C2 check [32;1mSUCCESSFUL[0m!" << std::endl;
    } else {
        std::cout << "C2 check [31;1mUNSUCCESSFUL[0m!" << std::endl;
    }

    typename g1_type::value_type G1, A1, C1, A2, C2, VKx;
    typename g2_type::value_type G2, B1, B2, VKy, VKz;

    G1 = g1_type::value_type::one();
    G2 = g2_type::value_type::one();

    A1 = G1*A1_poly;
    A2 = G1*A2_poly;

    C1 = G1*C1_poly;
    C2 = G1*C2_poly;

    B1 = G2*B1_poly;
    B2 = G2*B2_poly;

    VKx = G1*VKx_poly;
    VKy = G2*VKy_poly;
    VKz = G2*VKz_poly;

    auto base = [](integral_type x, int B) {
        std::vector<int> res = {(int)(x % B)};
        if (x > 0) {
            x /= B;
            while (x > 0) {
                res.insert(res.begin(), (int)(x % B) );
                x /= B;
            }
        }
        return res;
    };

    using params_type = nil::crypto3::algebra::pairing::detail::pairing_params<curve_type>;

    auto line_double_function = [](
            gt_field_value_type const& f,
            typename g2_type::value_type const& T,
            typename g1_type::value_type const& P)
    {
        /* GT = Fp4, X = (x00+u*x01) + v*( x10+u*x11)
         *
         * Q.X, Q.Y
         *
         *
         * For MNT4:
         * u^2 = 17 (0x11)
         *
         * v^2 = u
*
         * Untwisting: E'(Fpk/d) -> E(Fpk):
         *
         * quadratic:
         * D-twist: ("division")
         * twisted curve E': y^2 = x^3 +Ax/i^2 + B/i^3
         * (x,y) -> (ix, i^{3/2}y) = (u x, v^3 y)
         * M-twist: ("multiplication")
         * twisted curve E': y^2 = x^3 + i^2Ax + i^3B
         * (x,y) -> (x/i, i^{1/2}y/i)
         *
         */
        gt_field_value_type x1, y1, x, y, u, v;
        g2_field_value_type twist;
        auto aT = T.to_affine();
        auto aP = P.to_affine();


        twist = g2_field_value_type(0,1);
        u = gt_field_value_type({0,1},{0,0});
        v = gt_field_value_type({0,0},{1,0});

        aT.X *= twist.inversed();
        aT.Y *= twist.inversed();

        x1 = gt_field_value_type({aT.X.data[0], aT.X.data[1]}, {0,0})/* * (v*v).inversed()  */;
        y1 = gt_field_value_type({aT.Y.data[0], aT.Y.data[1]}, {0,0})/* * (v*v*v).inversed()*/;

        x = gt_field_value_type({aP.X, 0}, {0,0}) *u;
        y = gt_field_value_type({aP.Y, 0}, {0,0}) *u;

        auto three = gt_field_value_type({3,0},{0,0});
        auto two = gt_field_value_type({2,0},{0,0});

        auto l = three*x1*x1 * (two*y1).inversed();
        auto mul = ( l*(x-x1) - (y-y1) );
        
        return f*f*mul;
    };

    auto line_add_function = [](
            gt_field_value_type const& f,
            typename g2_type::value_type const& T,
            typename g2_type::value_type const& Q,
            typename g1_type::value_type const& P)
    {
        /* GT = Fp4, X = (x00+u*x01) + v*( x10+u*x11)
         *
         * T.X, T.Y
         * T.X/u T.Y/(v*u)
         *
         */
        gt_field_value_type x1, y1, x2, y2, x, y, u, v;
        auto aT = T.to_affine();
        auto aQ = Q.to_affine();
        auto aP = P.to_affine();

        g2_field_value_type twist;
        twist = g2_field_value_type(0,1);
        u = gt_field_value_type({0,1},{0,0});
        v = gt_field_value_type({0,0},{1,0});
        
        aT.X *= twist.inversed();
        aT.Y *= twist.inversed();
        aQ.X *= twist.inversed();
        aQ.Y *= twist.inversed();

        x1 = gt_field_value_type({aT.X.data[0], aT.X.data[1]}, {0,0})/* * (v*v).inversed()  */;
        y1 = gt_field_value_type({aT.Y.data[0], aT.Y.data[1]}, {0,0})/* * (v*v*v).inversed()*/;

        x2 = gt_field_value_type({aQ.X.data[0], aQ.X.data[1]}, {0,0})/* * (v*v).inversed()  */;
        y2 = gt_field_value_type({aQ.Y.data[0], aQ.Y.data[1]}, {0,0})/* * (v*v*v).inversed()*/;
        /*
        std::cout << " x1 :"; print_field_element(std::cout, x1);  std::cout << std::endl;
        std::cout << " x2 :"; print_field_element(std::cout, x2);  std::cout << std::endl;
        std::cout << " y1 :"; print_field_element(std::cout, y1);  std::cout << std::endl;
        std::cout << " y2 :"; print_field_element(std::cout, y2);  std::cout << std::endl;

        */
        x = gt_field_value_type({aP.X, 0}, {0,0}) * u;
        y = gt_field_value_type({aP.Y, 0}, {0,0}) * u;

        /*
        std::cout << " x :"; print_field_element(std::cout, x);  std::cout << std::endl;
        std::cout << " y :"; print_field_element(std::cout, y);  std::cout << std::endl;

        */
        if ( (x1 == x2) && (y1 == -y2)) {
            return f*(x-x1);
        } else {
            auto l = (y2-y1)*(x2-x1).inversed();
            //std::cout << " l  :"; print_field_element(std::cout, l);  std::cout << std::endl;
            return f*( l*(x-x1) - (y-y1) );
        }
    };


    auto miller_loop = [&base, &line_double_function, &line_add_function]
        (typename g1_type::value_type const& P, typename g2_type::value_type const& Q)
    {
        typename g2_type::value_type T = Q;
        gt_field_value_type f = gt_field_value_type::one();

        std::vector<int> C = base(params_type::ate_loop_count, 2);
        for(std::size_t i = 1; i < C.size(); ++i) {
            std::cout << i << " start :"; print_field_element(std::cout, f);  std::cout << std::endl;
            f = line_double_function(f, T, P);
            std::cout << i << " dbl   :"; print_field_element(std::cout, f);  std::cout << std::endl;
            T = T + T;
            if (1 == C[i]) {
                f = line_add_function(f, T, Q, P);
                std::cout << i << " add   :"; print_field_element(std::cout, f);  std::cout << std::endl;
                T = T + Q;
            }
        }
        return f;
    };

    auto final_exp = [](gt_field_value_type const& f) {
        return f.pow(params_type::final_exponent);
    };

    auto A1_B1 = final_exp(miller_loop(A1, B1));
    auto A2_B2 = miller_loop(A2, B2);

    /* C*VKz = A*B - VKx*VKy */
    auto C1_pair = final_exp(miller_loop(VKx, VKy)*miller_loop(C1, VKz));

    typename params_type::extended_integral_type fe, p, w0;
    p = g1_field_value_type::modulus;
    w0 = params_type::final_exponent_last_chunk_abs_of_w0;

    fe = (p*p-1)*(p+w0);

    if (fe == params_type::final_exponent) {
        std::cout << "final exp is [32;1mOK[0m!" << std::endl;
    } else {
        std::cout << "final exp is [31;1mNOT OK[0m!" << std::endl;
    }

    std::cout << "A1B1    :"; print_field_element(std::cout, A1_B1);  std::cout << std::endl;
    std::cout << "A2B2    :"; print_field_element(std::cout, A2_B2);  std::cout << std::endl;

    if(A1_B1 == C1_pair) {
        std::cout << "C2 check [32;1mSUCCESSFUL[0m!" << std::endl;
    } else {
        std::cout << "C2 check [31;1mUNSUCCESSFUL[0m!" << std::endl;
    }

//    auto A1_B1_p = pair_reduced<curve_type>(A1, B1);
//    auto A2_B2_p = pair_reduced<curve_type>(A2, B2);

#if 0

    auto prec_A1 = precompute_g1<curve_type>(A1);
    auto prec_A2 = precompute_g1<curve_type>(A2);
    auto prec_B1 = precompute_g2<curve_type>(B1);
    auto prec_B2 = precompute_g2<curve_type>(B2);
    //std::cerr << "------------" << std::endl;

    std::ofstream json("output.json");
    json << "{";
    json << "\"Fr\": [" << std::endl;
    print_field_element(json, VKx_poly); json << "," << std::endl;
    print_field_element(json, VKy_poly); json << "," << std::endl;
    print_field_element(json, VKz_poly); json << "," << std::endl;
    print_field_element(json, A1_poly ); json << "," << std::endl;
    print_field_element(json, B1_poly ); json << "," << std::endl;
    print_field_element(json, C1_poly ); json << "," << std::endl;
    print_field_element(json, A2_poly ); json << "," << std::endl;
    print_field_element(json, B2_poly ); json << "," << std::endl;
    print_field_element(json, C2_poly ); json << "]," << std::endl;

    json << "\"G1\": [" << std::endl;
    print_curve_group_element(json, A1 ); json << "," << std::endl;
    print_curve_group_element(json, C1 ); json << "," << std::endl;
    print_curve_group_element(json, A2 ); json << "," << std::endl;
    print_curve_group_element(json, C2 ); json << "," << std::endl;
    print_curve_group_element(json, VKx); json << "]," << std::endl;

    json << "\"G2\": [" << std::endl;
    print_curve_group_element(json, B1 ); json << "," << std::endl;
    print_curve_group_element(json, B2 ); json << "," << std::endl;
    print_curve_group_element(json, VKy); json << "," << std::endl;
    print_curve_group_element(json, VKz); json << "]," << std::endl;

    typename gt_type::value_type A1_B1 = pair<curve_type>(A1, B1);
    typename gt_type::value_type A2_B2 = pair<curve_type>(A2, B2);

    typename gt_type::value_type A1_B1_reduced = pair_reduced<curve_type>(A1, B1);
    typename gt_type::value_type A2_B2_reduced = pair_reduced<curve_type>(A2, B2);

    /* consistency check */
    auto C1_pair = pair_reduced<curve_type>(VKx, VKy)*pair_reduced<curve_type>(C1, VKz);
    auto C2_pair = pair_reduced<curve_type>(VKx, VKy)*pair_reduced<curve_type>(C2, VKz);
    BOOST_CHECK(C1_pair == A1_B1_reduced);
    BOOST_CHECK(C2_pair == A2_B2_reduced);

    auto vv = pair_reduced<curve_type>(VKx, VKy);
    auto cvz = pair_reduced<curve_type>(C1, VKz);

    std::cout << "A     :"; print_curve_group_element_affine(std::cout, A1 .to_affine());  std::cout << std::endl;
    std::cout << "B     :"; print_curve_group_element_affine(std::cout, B1 .to_affine());  std::cout << std::endl;
    std::cout << "C     :"; print_curve_group_element_affine(std::cout, C1 .to_affine());  std::cout << std::endl;
    std::cout << "VKx   :"; print_curve_group_element_affine(std::cout, VKx.to_affine());  std::cout << std::endl;
    std::cout << "VKy   :"; print_curve_group_element_affine(std::cout, VKy.to_affine());  std::cout << std::endl;
    std::cout << "VKz   :"; print_curve_group_element_affine(std::cout, VKz.to_affine());  std::cout << std::endl;
    std::cout << "AB    :"; print_field_element(std::cout, A1_B1_reduced);  std::cout << std::endl;
    std::cout << "vv    :"; print_field_element(std::cout, vv);  std::cout << std::endl;
    std::cout << "cvz   :"; print_field_element(std::cout, cvz); std::cout << std::endl;
    std::cout << "C1    :"; print_field_element(std::cout, C1_pair); std::cout << std::endl;

    std::cout << std::endl;


    auto A1_B1_x_A2_B2 = A1_B1_reduced*A2_B2_reduced;

    /* consistency check */
    auto AB = A1_B1_reduced * A2_B2_reduced;
    auto AB_check = pair_reduced<curve_type>(2*VKx, VKy) * pair_reduced<curve_type>(C1+C2, VKz);
    BOOST_CHECK(AB == AB_check);

    auto VKx_A1_B1 = pair_reduced<curve_type>(VKx_poly * A1, B1);

    /* consistency check */
    auto A1_VKx_B1 = pair_reduced<curve_type>(A1, VKx_poly * B1);
    BOOST_CHECK(VKx_A1_B1 == A1_VKx_B1);

    /* pow consistency check */
    BOOST_CHECK(VKx_A1_B1 == A1_B1_reduced.pow(cpp_int(VKx_poly.data)));

    auto miller_A1_B1 = miller_loop<curve_type>(prec_A1,prec_B1);
    auto miller_A2_B2 = miller_loop<curve_type>(prec_A2,prec_B2);
    auto double_miller_A1_B1_A2_B2 = double_miller_loop<curve_type>(prec_A1, prec_B1, prec_A2, prec_B2);

    /* consistency check */
    BOOST_CHECK(miller_A1_B1 * miller_A2_B2 == double_miller_A1_B1_A2_B2);

    json << "\"GT\": [" ;
    print_field_element(json, A1_B1); json << "," << std::endl;
    print_field_element(json, A2_B2); json << "," << std::endl;
    print_field_element(json, A1_B1_reduced); json << "," << std::endl;
    print_field_element(json, A2_B2_reduced); json << "," << std::endl;
    print_field_element(json, A1_B1_x_A2_B2); json << "," << std::endl;
    print_field_element(json, VKx_A1_B1); json << "," << std::endl;
    print_field_element(json, miller_A1_B1); json << "," << std::endl;
    print_field_element(json, miller_A2_B2); json << "," << std::endl;
    print_field_element(json, double_miller_A1_B1_A2_B2); json << "" << std::endl;
    json << "]," << std::endl;

    json << "\"g1_precomputed_type\": [" << std::endl;
    print_g1_precomp_element(json, prec_A1); json << "," << std::endl;
    print_g1_precomp_element(json, prec_A2);
    json << "]," << std::endl;

    json << "\"g2_precomputed_type\": [" << std::endl;
    print_g2_precomp_element<CurveType>(json, prec_B1); json << "," << std::endl;
    print_g2_precomp_element<CurveType>(json, prec_B2);
    json << "]" << std::endl;

    json << "}";
#endif

}

/*
template<typename CurveType>
void check_curve() {
    using curve_type = CurveType;
    using scalar_field_type = typename curve_type::scalar_field_type;
    using g1_type = typename curve_type::template g1_type<>;
    using g2_type = typename curve_type::template g2_type<>;
    using gt_type = typename curve_type::gt_type;
    using g1_field_value_type = typename g1_type::field_type::value_type;
    using g2_field_value_type = typename g2_type::field_type::value_type;

    typename scalar_field_type::value_type
        c1,c2;
 
    typename g1_type::value_type G1, A1, A2;
    typename g1_type::value_type p1 = {
        0x22c26a3c19d56fc8790485554be5dc4351961a5162c3634965dc8ae56701157e_cppui254,
        0x1e3305b98bf381650491b7b63559d20d662b70f1616e680a19170715b59a3426_cppui254,
        0x2dadec13affe7e4ad00a4c896773b96ce87fbcb232b8660721adc1f266defa45_cppui254,
    };
    typename g1_type::value_type p2 = {
        0x148a1f438a4cd0d807549cb7f9ec9f41dba3d8b14a6b0f2489d9b9f626d6fd31_cppui254,
        0x3cc907ef65b0eff91d027e4771e9116a0b125325627b6bdf55037702220b1b2_cppui254,
        0x1242df09f53bda3a3bfb5fb2054817c6cbb32a7217556a6c831113f4d3fee54c_cppui254,
    };
    G1 = g1_type::value_type::one();
    // G2 = g2_type::value_type::one();

    c1 = 345751109;
    c2 = 270382749;

    A1 = G1*c1;
    A2 = G1*c2;

    print_curve_group_element(std::cout, p1); std::cout << std::endl;
    print_curve_group_element(std::cout, p2); std::cout << std::endl;

    print_curve_group_element_affine(std::cout, p1.to_affine()); std::cout << std::endl;
    print_curve_group_element_affine(std::cout, p2.to_affine()); std::cout << std::endl;

}
*/

BOOST_AUTO_TEST_SUITE(pairing_debug_tests)
BOOST_AUTO_TEST_CASE(pairing_operation_test_atl_bn128_254) {
//    using curve_type = typename curves::alt_bn128_254;
    using curve_type = typename curves::mnt4<298>;
//    using curve_type = typename curves::mnt6<298>;
    //using curve_type = typename curves::bls12<381>;
//    using curve_type = typename curves::bls12<377>;

    // check_curve<curve_type>();
    
    check_pairing_operations<curve_type>();
}

/*
BOOST_AUTO_TEST_CASE(pairing_operation_test_bls12_381) {
    using curve_type = typename curves::bls12<381>;

    check_pairing_operations<curve_type>();
}
*/

/*
BOOST_AUTO_TEST_CASE(pairing_operation_test_mnt4_298) {
    using curve_type = typename curves::mnt4<298>;

    check_pairing_operations<curve_type>();
}
*/

/*
BOOST_AUTO_TEST_CASE(pairing_operation_test_mnt6_298) {
    using curve_type = typename curves::mnt6<298>;

    check_pairing_operations<curve_type>();
}*/

BOOST_AUTO_TEST_SUITE_END()
