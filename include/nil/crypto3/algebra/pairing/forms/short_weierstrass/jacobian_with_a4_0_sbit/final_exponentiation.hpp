//---------------------------------------------------------------------------//
// Copyright (c) 2020-2021 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020-2021 Nikita Kaskov <nbering@nil.foundation>
// Copyright (c) 2024  Vasiliy Olekhov <vasiliy.olekhov@nil.foundation>
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

#ifndef CRYPTO3_ALGEBRA_PAIRING_SHORT_WEIERSTRASS_JACOBIAN_WITH_A4_0_SBIT_FINAL_EXPONENTIATION_HPP
#define CRYPTO3_ALGEBRA_PAIRING_SHORT_WEIERSTRASS_JACOBIAN_WITH_A4_0_SBIT_FINAL_EXPONENTIATION_HPP

#include <nil/crypto3/algebra/pairing/detail/forms/short_weierstrass/jacobian_with_a4_0/types.hpp>

namespace nil {
    namespace crypto3 {
        namespace algebra {
            /*
            namespace debug {

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

template<typename FieldParams>
void print_ell_coeffs(std::ostream &os, 
        fields::detail::element_fp2<FieldParams> const& ell_0,
        fields::detail::element_fp2<FieldParams> const& ell_VW,
        fields::detail::element_fp2<FieldParams> const& ell_VV)
{
    os << "{" << std::endl;
    os << "\"ell_0\" : "; print_field_element(os, ell_0 ); os << "," << std::endl;
    os << "\"ell_VW\": "; print_field_element(os, ell_VW); os << "," << std::endl;
    os << "\"ell_VV\": "; print_field_element(os, ell_VV); os << "" << std::endl;
    os << "}" << std::endl;
}
}
*/


            namespace pairing {

                template<typename CurveType>
                class short_weierstrass_jacobian_with_a4_0_sbit_final_exponentiation {
                    using curve_type = CurveType;

                    using params_type = detail::pairing_params<curve_type>;
                    typedef detail::short_weierstrass_jacobian_with_a4_0_types_policy<curve_type> policy_type;

                    using base_field_type = typename curve_type::base_field_type;
                    using gt_type = typename curve_type::gt_type;

                    static typename gt_type::value_type exp_by_z(const typename gt_type::value_type &elt) {
                        typename gt_type::value_type result = elt.cyclotomic_exp(params_type::final_exponent_z);
                        if (!params_type::final_exponent_is_z_neg) {
                            result = result.unitary_inversed();
                        }
                        return result;
                    }

                public:
                    /* https://link.springer.com/chapter/10.1007/978-3-642-28496-0_25#preview
                     * */
                    static typename gt_type::value_type process(const typename gt_type::value_type &elt) {

                        std::cout << "~~~~~ FINAL EXPONENTIATION ~~~~~~" << std::endl;
                        std::cout << "elt: "; debug::print_field_element(std::cout, elt); std::cout << std::endl;

#define DUMP_FP12(X) { std::cout << #X " :" ; debug::print_field_element(std::cout, X); std::cout << std::endl; }
                        /* TODO: check elt == 0 ? */
                        auto f1 = elt.unitary_inversed();
                        DUMP_FP12(f1);
                        auto f2 = elt.inversed();
                        DUMP_FP12(f2);

                        auto r = f1 * f2;
                        f2 = r;
                        r = r.Frobenius_map(2);
                        r *= f2;

                        DUMP_FP12(r);

                        auto y0 = exp_by_z(r);
                        DUMP_FP12(y0);
                        auto y1 = y0.cyclotomic_squared();
                        DUMP_FP12(y1);
                        auto y2 = y1.cyclotomic_squared();
                        DUMP_FP12(y2);
                        auto y3 = y2 * y1;
                        DUMP_FP12(y3);
                        auto y4 = exp_by_z(y3);
                        DUMP_FP12(y4);
                        auto y5 = y4.cyclotomic_squared();
                        DUMP_FP12(y5);
                        auto y6 = exp_by_z(y5);
                        DUMP_FP12(y6);
                        y3 = y3.unitary_inversed();
                        DUMP_FP12(y3);
                        y6 = y6.unitary_inversed();
                        DUMP_FP12(y6);
                        auto y7 = y6 * y4;
                        DUMP_FP12(y7);
                        auto y8 = y7 * y3;
                        DUMP_FP12(y8);
                        auto y9 = y8 * y1;
                        DUMP_FP12(y9);
                        auto y10 = y8 * y4;
                        DUMP_FP12(y10);
                        auto y11 = y10 * r;
                        DUMP_FP12(y11);
                        auto y12 = y9.Frobenius_map(1);
                        DUMP_FP12(y12);
                        auto y13 = y12 * y11;
                        DUMP_FP12(y13);
                        y8 = y8.Frobenius_map(2);
                        DUMP_FP12(y8);
                        auto y14 = y8 * y13;
                        DUMP_FP12(y14);
                        r = r.unitary_inversed();
                        DUMP_FP12(r);
                        auto y15 = r * y9;
                        DUMP_FP12(y15);
                        y15 = y15.Frobenius_map(3);
                        DUMP_FP12(y15);
                        auto result = y15 * y14;
                        DUMP_FP12(result);

                        return result;
                    }
                };
            }    // namespace pairing
        }        // namespace algebra
    }            // namespace crypto3
}    // namespace nil
#endif    // CRYPTO3_ALGEBRA_PAIRING_SHORT_WEIERSTRASS_JACOBIAN_WITH_A4_0_SBIT_FINAL_EXPONENTIATION_HPP
