# Generating data from a spin model with high order interactions

## Requirements

The code uses the C++11 version of C++.

## Usage

### Set global constants in the file `data.h`:

 - Specify the Output directory in `const string OUTPUT_directory`. All the generated files will be placed in this folder.
 - Specify the number of variables on which the model is based in `const unsigned int n`. The generated dataset will have the same number of variables.

### Set local variables in the file `main.cpp`, in the function `int main()`:

Specify the choice of the parameters:
 - `int K` = number of interactions of the model;
 - `unsigned int N` = number of datapoints.

### Available functions from the `main.cpp`:

For hands-on and simple tests of the program, check the two examples in the function `int main()`.

#### Functions available to define a spin models:

Examples of scale-free discrete payoff functions are provided:
 - `double r_linear(int m)` is a linear payoff function;
 - `double r_x2(int m)` is a quadratic payoff function;
 - `double r_exp(int m)` is an exponentially decaying payoff function.

One can write any other choice of payoff function in `double r(int m)` as long as the `r(m)` is a decreasing function of `m`. 
The function doesn't have to be scale-free. In this case, solutions for varying values of `N` can be computed, but will not be rescalable.

### Set the local parameters in the `int main()` function:

 - `double eta` = rate at which agents leave their home;
 - `double c` = cost for changing food spot, has a negative value.

Allows you to compute the solution for varying values of `eta` and `c`.

### To compile:

`g++ -O3 main.cpp Models_Ex.cpp Generate_data_exact.cpp`

