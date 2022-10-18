# Manual # {#algebra_manual}

@tableofcontents

# Installation

Like most crypto3 libraries, `algebra` can either be used stand alone or be used as
part of the entire crypto3 suite. See instructions [here](https://github.com/NilFoundation/crypto3#usage) on how to include the
library in your project.

# Elliptic curves

The following curves are implemented in the library

- Barreto-Naehrig
- Babyjubjub
- BLS12 variants
- Brainpool
- [Curve25519](https://datatracker.ietf.org/doc/html/rfc7748#section-4.1) 
- ed25519
- Edwards
- FRP?? TODO
- GOST
- Jubjub
- mnt4/mnt6
- NIST P-192/P-224/P-384/P-521
- [Pallas](https://zips.z.cash/protocol/protocol.pdf#pallasandvesta)
- secpk1
- secpr1
- secpv1
- sm2p
- vesta
- X9.62

## Usage

Curves are defined under the namespace `nil::crypto3::algebra::curves`
A curve can be instantiated as follows and by including the relevant header.

```c++
curves::bls12<377>
```

Curves encompass one or more `field` types which respect the curve specific constants
and domain. Curves are generally used along with the 
[pubkey](https://github.com/NilFoundation/crypto3-pubkey) library which enables
a user to create public/private keys and perform cryptographic operations.

# Field Type

`field` type is a generic type which is specialised for the above curves.
This type can be extended to any curves and operators are overloaded to perform
arithmetic over their domain. They are optimised for arithmetic in the finite field.

## Usage
A field can be instantiated as 
```c++
field<254> //254 - is the Modulus Bits
```

Specialised for curves as base fields and scalar fields are usually defined as
```c++
//fields/secp/secp_k1
struct secp_k1_base_field<256> : public field<256>
struct secp_k1_scalar_field<256> : public field<256>
```

# Matrix

`matrix` is a generic type which allows creation of 2D matrices. The type also
supports common transformations which can be applied to matrices ex: transpose,
matrix multiplication etc.

## Usage
Matrix is defined under the namespace `nil::crypto3::algebra`
A matrix can be instantiated as 

```c++
constexpr matrix<double, 3, 3> m1 = {1., 2., 3., 4., 5., 6., 7., 8., 9.};
constexpr matrix m2 = {{{1., 2.}}};
```





# Examples

More examples of usage of the library can be found in the folder `example/`


