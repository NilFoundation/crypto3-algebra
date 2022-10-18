# Manual # {#algebra_manual}

@tableofcontents

# Elliptic curves implemented

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

# Field Type

the `field` type is a generic type which is specialised for the above curves.
This type can be extended to any curves and operators are overloaded to perform
arithmetic over their domain.


