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

`g++ -std=c++11 -O3 main.cpp Models_Ex.cpp Generate_data_exact.cpp`

### To execute:

`./a.out`

## Usage

### Set global variables in the file `data.h`:

 - Specify the Output directory in `const string OUTPUT_directory`. All the generated files will be placed in this folder.
 - Specify the number of variables on which the model is based in `const unsigned int n`. The generated dataset will have the same number of variables.

### Set local variables in the file `main.cpp`, in the function `int main()`:

 - `int K` = number of interactions of the model;
 - `unsigned int N` = number of datapoints of the generated dataset.

### Available functions:

All useful functions that can be called from `int main()` are declared at the beginning of the `main.cpp` file and described below.

### Define a spin models:

A spin model is stored in a list of `Interaction`:  `list<Interaction>`.
For more information, the structure `Interaction` is defined in `data.h`. 
Each `Interaction I` contains the following attributes:
 - the spin operator associated to that interaction, stored as an integer in `I.Op`;
 - the value of the real parameter `I.g` associated to the interaction, stored in `double I.g`;
 - the value of the model average of the operator `I.Op`, stored in `I.av_M`;  --> this value is initially set to `0`, and can be computed after definition of the model 
 - the value of the empirical average of the operator `I.Op`, stored in `I.av_D` --> this value is initially set to `0`, and can be computed when a dataset is generated.

list_I = list of operators and parameters of the model

The program offers different ways to define a spin model (i.e., a list of interactions `list<Interaction>`).  These functions are defined in the file `Models_Ex.cpp`. Here is a list:

#### Pre-defined spin models:
 - **Independent model**: the function `list<Interaction> IndepModel(double h=1)` creates an independent model with one field on each spin variable; the value of each field parameters is uniformly sampled over `[-h; +h]`.
 - **Fully connected pairwise model**: the function `list<Interaction> FullyConnectedPairwiseModel(double h=1, double J=1)` creates a fully connected pairwise model, i.e., with a field on each spin  and all the pairwise interactions; the resulting model has `K=n(n+1)/2` interactions. The value of each field parameters is uniformly sampled over `[-h, +h]`; the values of each pairwise parameter is uniformly sampled over `[-J, +J]`.

#### Random spin models:
 - **Random pairwise model**: the function `list<Interaction> Random_PairwiseModel(int Kpair, double h=1, double J=1)` creates a model with all the `n` fields and `Kpair` randomly picked pairwise interactions; in total the model has `K = n+Kpair` interactions; the value of each field parameters is uniformly sampled over `[-h, +h]`; the values of each pairwise parameter is uniformly sampled over `[-J, +J]`.
 - **Random spin model**: the function `list<Interaction> Random_Model(int K, double g=1)` creates a model with `K` randomly picked spin interactions (interactions can be of any order); the value of each parameters is uniformly sampled over `[-g, +g]`.

#### Spin models specified by the user through an input file: (to come)

#### Changing the values of the parameters:

The function `void Randomize_param(list<Interaction>& list_I, double g=1)` resamples the value of the parameters of the model given as an argument of the function as `list<Interaction> list_I`. Each parameter of the model is then resampled uniformly in `[-g, g]`.

### Get information on your model:

The two following functions can be found in `Models_Ex.cpp` and print information about the model defined by `list_I`:
 - `void PrintTerm_ListInteraction (list<Interaction> list_I, unsigned int N=1)` prints in the terminal;
 - `void PrintFile_ListInteraction(list<Interaction> list_I, string output_filename, unsigned int N=1)` prints in the file named `output_filename`.

**Information printed:** For each interaction `I` in the model defined by `list_I` is printed the following information:
 - **1:Operator Integer:** the integer representation `I.Op` of the operator associated to that interaction;
 - **2:Order:** the order of the interaction;
 - **3:Parameter:** the value `I.g` of the parameter associated to the interaction;
 - **4:Model Average:** the observable associated to that interaction, i.e., the model average of the operator `I.Op`, which is stored in `I.av_M`; This will be `0` if this value has not yet been computed.
 - **5:Data Average:** the empirical average of the operator `I.Op` computed on the last dataset generated with that model; This value is stored in `I.av_M`; This will be `0` if no dataset has been generated yet with the model, or if the value hasn't been computed.
 - **6:std:** standard deviation of the distribution of possible data averages of the operator `I.Op` around its model average if one samples random datasets with `N` datapoints; this standard deviation is calculated using:
 `std[<Op>_D] = \sqrt{\frac{(1+<Op>_M)(1-<Op>_D)}{4 N}}`;
 The value will be `0` if a number of datapoints `N` hasn't been specified (`N` is an optional argument for the two printing functions). 
 - **7:Operator Binary:** the binary representation of the operator `I.Op` associated to the interaction.

Additionally, to compute model and data averages of the spin operators:
 - the function `void Model_averages_Ising(list<Interaction> &list_I)` computes the model average of each operator `I` in `list_I` and store it in `I.av_M`; 
 - along generating with generating data, the function `Sample_dataset_AND_Print_ModelData_Info(list_I, output_filename, N)` computes the data average of each operators of `list_I` for the generated dataset, and store it in `I.av_D` (see next section).

### Generate data:

The following functions are defined in the file `Generate_data_exact.cpp`.

 - The function `void Sample_dataset(list<Interaction> list_I, string output_filename, unsigned int N=1000)` samples a dataset  with `N` datapoints from the model defined in `list_I`.

 - The function `void Sample_dataset_AND_Print_ModelData_Info(list<Interaction>& list_I, string output_filename, unsigned int N=1000)` does the same, while also filling in information about the model and the data in the Interactions of `list_I`. More precisely, the function computes the model and data averages of the operators of the model and fill this information in `list_I` (i.e., respectively in `I.av_M` and `I.av_D` for each interaction `I`).

## Examples

All the functions that can be used from `int main()` are declared at the beginning of the `main.cpp` file.

For hands-on and simple tests of the program, check the two examples in the function `int main()` of the `main.cpp` file.

## Ouput files

All the output files are stored in the output folder whose name is specified in `data.h`.

The output file can be of two types:
 - a binary datafile;
 - a file containing information on the model used to generate the data.

Creating the datafile and exporting the model is done through two separate functions (respectively `Sample_dataset` and `PrintFile_ListInteraction` -- see above). However I recommend to always save the model used together with the generated dataset (for instance by using matching filenames -- see example in the `main()` function).

