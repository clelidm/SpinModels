# Generating data from a spin model with high order interactions

This simple program generates data from a spin model with any chosen spin interactions. Interactions can be of any order.

The program can use: 
 - pre-defined spin models (i.e., the independent model, and the fully connected pairwise model);
 - randomly generated spin models (see section below);
 - or, chosen spin models specified by the user through an input file (section to be added).

The program then exactly calculate the cumulative (multi-dimensional) distribution for the chosen spin model, and samples a dataset from it.

In some sense, it is a very basic code, which involves computing the probability of all the `2^n` states of the system with `n` spin variables.
For that reason the program doesn't really work for too large systems. However, it can still reasonably sample datasets for systems of `n=15 to 20` spins, and for models with `K=100 to 500` interactions. 
With too large values of `n` the precision (on the uniform sampling) will drop.
For large systems the computing time will also quickly increase.

For large systems (large values of `n`) it is recommended to use methods such as Metropolis-Hastings algorithm. 

## Requirements

The code uses the C++11 version of C++.

## Usage

### Set global constants in the file `data.h`:

 - Specify the Output directory in `const string OUTPUT_directory`. All the generated files will be placed in this folder.
 - Specify the number of variables on which the model is based in `const unsigned int n`. The generated dataset will have the same number of variables.

### Set local variables in the file `main.cpp`, in the function `int main()`:

 - `int K` = number of interactions of the model;
 - `unsigned int N` = number of datapoints of the generated dataset.

### Available functions from the `main.cpp`:

For hands-on and simple tests of the program, check the two examples in the function `int main()`.

#### Functions available to define a spin models:

The program offers different ways to define the chosen spin models. 

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

### Ouput files:

### To compile:

`g++ -O3 main.cpp Models_Ex.cpp Generate_data_exact.cpp`

