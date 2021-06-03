# Generating data from a spin model with high order interactions

This simple program generates binary data from a spin model with any chosen spin interactions. Interactions can be of any order.

The program can use: 
 - pre-defined spin models (i.e., the independent model, and the fully connected pairwise model);
 - randomly generated spin models (see description below);
 - or, chosen spin models specified by the user through an input file (section to be added).

The program then exactly calculate the cumulative (multi-dimensional) distribution for the chosen spin model, and randomly samples a dataset from it.
The system is assumed stationary, and all datapoints are independently randomly sampled from that same distribution.

In some sense, it is a very basic code, which computes the probability of all the `2^n` states accessible to a system with `n` spin variables.
For that reason the program doesn't really work for systems that are too large or for models with too many interactions. However, it can still reasonably sample datasets for systems of `n=15 to 20` spins, and for models with `K=100 to 500` interactions. 
For too large values of `n`, the precision on the uniform sampling will drop.
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

### Define a spin models:

A spin model is stored in a list of `Interaction`:  `list<Interaction>`.
For more information, the structure `Interaction` is defined in `data.h`. 
Each `Interaction I` contains the information about:
 - the operator associated to that interaction: stored as an integer in `I.Op`;
 - the value of the real parameter `I.g` associated to the interaction: stored in `double I.g`;
 - the value of the model average of the operator `I.Op`: stored in `I.av_M`;  --> this value is initially set to `0`, and can be computed after definition of the model 
 - the value of the empirical average of the operator `I.Op`: stored in `I.av_D` --> this value is initially set to `0`, and can be computed when a dataset is generated.

The program offers different ways to define a spin model (i.e., a list of interactions `list<Interaction>`). Here is a list.

#### Pre-defined spin models:
 - **Independent model**: the function `list<Interaction> IndepModel(double h=1)` creates an independent model with one field on each spin variable; the value of each field parameters is uniformly sampled over `[-h; +h]`.
 - **Fully connected pairwise model**: the function `list<Interaction> FullyConnectedPairwiseModel(double h=1, double J=1)` creates a fully connected pairwise model, i.e., with a field on each spin  and all the pairwise interactions; the resulting model has `K=n(n+1)/2` interactions. The value of each field parameters is uniformly sampled over `[-h, +h]`; the values of each pairwise parameter is uniformly sampled over `[-J, +J]`.

#### Random spin models:
 - **Random pairwise model**: the function `list<Interaction> Random_PairwiseModel(int Kpair, double h=1, double J=1)` creates a model with all the `n` fields and `Kpair` randomly picked pairwise interactions; in total the model has `K = n+Kpair` interactions; the value of each field parameters is uniformly sampled over `[-h, +h]`; the values of each pairwise parameter is uniformly sampled over `[-J, +J]`.
 - **Random spin model**: the function `list<Interaction> Random_Model(int K, double g=1)` creates a model with `K` randomly picked spin interactions (interactions can be of any order); the value of each parameters is uniformly sampled over `[-g, +g]`.

#### Spin models specified by the user through an input file: (to come)

#### Changing the values of the parameters:
The function 

### Get information on your model:

### Generate data:

## Examples:

For hands-on and simple tests of the program, check the two examples in the function `int main()` of the `main.cpp` file.

### Ouput files:

All the output files will be stored in the output folder whose name can be specified in `data.h`.

The output file can be of two types:
 - a datafile;
 - the corresponding model used to generate the data.

Creating the datafile and exporting the model is done through two separate functions. However I recommand to always save the model used together with the generated dataset (for instance by using matching filenames -- see example in the `main()` function).

