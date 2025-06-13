# dimeta-tests &middot; [![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

Test environment for llvm-dimeta allocation type detection.

## Overview

- [testsuite](testsuite/): Unit test set with (a) MPI-BugBench datatype tests, (b) MPI mini-app type layout tests.
- [MPI mini-apps](miniapps/): A set of MPI mini-apps that are executed with MUST+TypeART using llvm-dimeta type extraction.
- [dimeta-verifier-pass](dimeta-verifier-pass/): A LLVM plugin for testing type extraction with llvm-dimeta. Used in the [testsuite](testsuite/).
- [support](support/): A set of bootstrap scripts to set up the required software environment.

### Requirements

[CMakeLists](CMakeLists.txt) facilitates the execution of our tests by generating appropriate targets.
[CMakeLists](CMakeLists.txt) will require the following environment variables:
- `MUST_PATH`: PATH to the MUST installation, to detect `mustrun` and TypeART

To setup, execute from the root project folder:

1. Bootstrap script to download & install MUST, follow the instruction to set `MUST_PATH` at the end:
    ```shell
    $ ./support/must-bootstrap.sh
    ```

2. Create the CMake-based test environment. We use `ctest` as the driver for the test target execution.
    ```shell
    $ mkdir build && cd build
    $ cmake ..
    ```

3. Build the [dimeta-verifier-pass](dimeta-verifier-pass/), from within the created `build` folder:
    ```shell
    $ make -j8
    ```


#### Software requirements
- System modules: `1) clang/14 2) openmpi/4.1.6 3) git/2.4 4) python/3`
- MUST: [bootstrap](support/must-bootstrap.sh)
- testsuite requirements: llvm-lit (installable via Python `pip`), FileCheck (included with Clang)

### MPI BugBench tests

In [root level](./) created `build` folder `cmake ..` generates:
- Targets to build, and run the MPI BugBench tests on the current node, see [CMakeLists](testsuite/CMakeLists.txt): 
    - llvm-dimeta mode for type detection, requires clean temp folder `testsuite-exec`: 
        ```shell
        $ make ditest-clean-testsuite
        $ ctest -V -R ditests-mpi-bugbench
        ```
    - LLVM IR mode for type detection, requires clean temp folder `testsuite-exec`:
        ```shell
        $ make ditest-clean-testsuite
        $ export TYPEART_TYPEGEN_IR=1
        $ ctest -V -R ditests-mpi-bugbench
        ```
#### Expected output
- llvm-dimeta mode should print at the end:
  ```shell
    1: Total Discovered Tests: 91
    1:   Passed           : 89 (97.80%)
    1:   Expectedly Failed:  2 (2.20%)
  ```

- LLVM IR mode should print at the end:
  ```shell
    1: Total Discovered Tests: 91
    1:   Passed: 70 (76.92%)
    1:   Failed: 21 (23.08%)
  ```


### MPI Mini-Apps

In [root level](./) created `build` folder `cmake ..` generates:
- Targets to build, and run on the current node, see [CMakeLists](miniapps/CMakeLists.txt): 
    - LULESH: `ctest -V -R 'lulesh$'`
    - 104.milc: `ctest -V -R milc`
    - 122.tachyon: `ctest -V -R tachyon`
    - mag2013: `ctest -V -R amg`
    - TeaLeaf: `ctest -V -R tealeaf`

**Note**: Executing `make ditest-clean-miniapp-exec-folder` cleans mini-app results (only needed for a fresh run).

#### Example TeaLeaf
Execute within `build` folder, expected output:
```shell
$ ctest -V -R tealeaf

Test project dimeta-tests/build
    Start 27: test_clean_tealeaf
1/5 Test #27: test_clean_tealeaf ...............   Passed    0.01 sec
    Start 26: test_build_tealeaf
2/5 Test #26: test_build_tealeaf ...............   Passed   19.56 sec
    Start 30: tealeaf
3/5 Test #30: tealeaf ..........................   Passed   21.11 sec
    Start 28: test_verifier_tealeaf
4/5 Test #28: test_verifier_tealeaf ............   Passed    0.01 sec
    Start 29: test_log_parser_tealeaf
5/5 Test #29: test_log_parser_tealeaf ..........   Passed    0.04 sec

100% tests passed, 0 tests failed out of 5
```

Afterwards, folder `miniapp-exec/` contains evaluation numbers:
```shell
$ cat miniapp-exec/tealeaf-counter.json
{
  "Total heap": 48.0,
  "Total stack": 404.0,
  "Total global": 0.0,
  "Addresses checked": 309.0,
  "Distinct addresses checked": 21.0,
  "Addresses missed": 0.0
}
```
