# Inference

Program that fits binary datasets with a chosen spin model with any high order interactions.

## Requirements

The code uses the C++11 version of C++.

It also uses the two header-only C++ libraries: Eigen and LBFGS++ (L-BFGS solver).
Remember to specify the paths to the two libraries to your compiler (see below).

**To compile:**  `g++ -I /usr/local/include/eigen3/ -I ./LBFGSpp-master/include -O2 main.cpp Models.cpp ReadDataFile.cpp BoltzmannLearning_Ising.cpp BoltzmannLearning_ACE.cpp`

**To execute:** `./a.out`

## Examples

All the functions that can be used from `int main()` are declared at the beginning of the `main.cpp` file and described below.

For hands-on and simple tests of the program, check the examples in the `int main()` function of the `main.cpp` file.


---

## Usage

### Set global variables in the file `data.h`:

Specify:
 - the number of variables on which the model is based in `const unsigned int n`. This number can be smaller or equal to the number of variables in the input dataset. If this number is smaller, then only in the first `n` bits (per line) of the dataset will be read;
 - the location of the input dataset in `const string INPUT_filename`; the input file should be written as strings of `0`'s and `1`'s, with one datapoint per line; see example in `INPUT/INPUT/n15_K20_RandM_dataset_N10000.dat`;
 - the output directory in `const string OUTPUT_directory`: all generated files will be placed in that folder;
 - the output filename in `const string filename`; the name of all generated files will start with this same string.

