# =nil; Foundation's Algebraic Constructions Module

Crypto3 Algebra library extends the =nil; Foundation's cryptography suite and provides a set algebraic constructions
being used mostly for public-key schemes
implemented in way C++ standard library implies: concepts, algorithms, predictable behavior, the latest standard features support and clean architecture without compromising security and performance.

The library contains numerous elliptic curves ,algorithms for curve operations (ex : add,mul), field types, matrix &
common matrix transformation operations.


## Repository structure
```
root
├── cmake: cmake sub-module with helper functions/macros to build crypto3 library umbrella-repository
├── docs: documentation , tutorials and guides
├── example: examples using the library.
├── test: Unit test cases
├── include/nil/crypto3: Library source 
│   ├── algebra :
│   │   ├── algorithms:Curve algorithms like miller loop 
│   │   ├── curves:Curves supported by library like secp256k1 & varaiants
│   │   ├── fields:field elements 
│   │   ├── matrix:matrixs' and operations
│   │   ├── multiexp:multi exponentiation algorithm
│   │   ├── pairing:pairing on elliptic cuvrves
│   │   ├── scalar:scalar math operations 
│   │   ├── vector:vector math operations
│   ├── detail : helper functions, constants , misc common utils.

```

## Building

This library uses Boost CMake build modules (https://github.com/BoostCMake/cmake_modules.git). 
To actually include this library in a project it is required to:

1. Add [CMake Modules](https://github.com/BoostCMake/cmake_modules.git) as submodule to target project repository.
2. Add all the internal dependencies using [CMake Modules](https://github.com/BoostCMake/cmake_modules.git) as submodules to target project repository.
3. Initialize parent project with [CMake Modules](https://github.com/BoostCMake/cmake_modules.git) (Look at [crypto3](https://github.com/nilfoundation/crypto3.git) for the example)

TODO : Add commands to build and run tests


## Dependencies

### Internal

* [Multiprecision](https://github.com/nilfoundation/crypto3-multiprecision.git).

### External
* [Boost](https://boost.org) (>= 1.73)
