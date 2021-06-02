# Generating data from a spin model with high order interactions

This simple program generates binary data from a spin model with any chosen spin interactions. Interactions can be of any order.

The program can use: 
 - pre-defined spin models (i.e., the independent model, and the fully connected pairwise model);
 - randomly generated spin models (see section below);
 - or, chosen spin models specified by the user through an input file (section to be added).

The program then exactly calculate the cumulative (multi-dimensional) distribution for the chosen spin model, and randomly samples a dataset from it.
The system is assumed stationary, and all datapoints are independently randomly sampled from that same distribution.

In some sense, it is a very basic code, which involves computing the probability of all the `2^n` states of the system with `n` spin variables.
For that reason the program doesn't really work for too large systems. However, it can still reasonably sample datasets for systems of `n=15 to 20` spins, and for models with `K=100 to 500` interactions. 
With too large values of `n` the precision (on the uniform sampling) will drop.
For large systems the computing time will also quickly increase.

For large number `n` of variables, it is recommended to use methods such as a Metropolis-Hastings algorithm. 

## Requirements

The code uses the C++11 version of C++.

### To compile:

`g++ -O3 main.cpp Models_Ex.cpp Generate_data_exact.cpp`

## Usage

### Set global constants in the file `data.h`:

 - Specify the Output directory in `const string OUTPUT_directory`. All the generated files will be placed in this folder.
 - Specify the number of variables on which the model is based in `const unsigned int n`. The generated dataset will have the same number of variables.

### Set local variables in the file `main.cpp`, in the function `int main()`:

 - `int K` = number of interactions of the model;
 - `unsigned int N` = number of datapoints of the generated dataset.

### Available functions from the file `main.cpp`:

For hands-on and simple tests of the program, check the two examples in the function `int main()`.

#### Define a spin models:

A spin model is stored in a list of `Interaction`:  `list<Interaction>`.
For more information, the structure `Interaction` is defined in `data.h`. 
Each `Interaction I` contains the information about:
 - the operator associated to that interaction: stored as an integer in `I.Op`;
 - the value of the real parameter `I.g` associated to the interaction: stored in `double I.g`;
 - the value of the model average of the operator `I.Op`: stored in `I.av_M`;  --> this value is initially set to `0`, and can be computed after definition of the model 
 - the value of the empirical average of the operator `I.Op`: stored in `I.av_D` --> this value is initially set to `0`, and can be computed when a dataset is generated.

The program offers different ways to define a spin model (i.e., a list of interactions `list<Interaction>`):
 - the function `list<Interaction> IndepModel(double h=1)` creates an independent model with one field on each spin variable; the value of each field parameters is uniformly sampled over `[-h; +h]`.
 - the function `list<Interaction> FullyConnectedPairwiseModel(double h=1, double J=1)` creates a fully connected pairwise model, i.e., with a field on each spin  and all the pairwise interactions; the resulting model has `K=n(n+1)/2` interactions. The value of each field parameters is uniformly sampled over `[-h, +h]`; the values of each pairwise parameter is uniformly sampled over `[-J, +J]`.
 -      
 -            

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


