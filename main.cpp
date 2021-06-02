// g++ -O2 main.cpp Models_Ex.cpp Generate_data_exact.cpp 
// ./a.out

#include <random>
#include <iostream>
#include <fstream>
#include <string> 
#include <list>

using namespace std;
/********************************************************************/
/**************************    CONSTANTS    *************************/
/********************************************************************/
#include "data.h"

/******************************************************************************/
/**************************     RANDOM GENERATOR    ***************************/
/******************************************************************************/
std::mt19937 gen;       //only for mt19937    //std::default_random_engine gen;
void initialise_generator()
{
          int seed = (unsigned)time(NULL);
          srand48(seed);      //for drand48
          gen.seed(seed); //for mt19937
}

/********************************************************************/
/**************************    MODELS.CPP    ************************/
/********************************************************************/
// Print information about the model in list_I and the generated data (if applies):
void PrintTerm_ListInteraction (list<Interaction> list_I, unsigned int N=1);
void PrintFile_ListInteraction(list<Interaction> list_I, string output_filename, unsigned int N=1);

// Defines specific models:  ************************************** //
list<Interaction> IndepModel(double h=1);     // model with all the "n" fields
list<Interaction> FullyConnectedPairwiseModel(double h=1, double J=1);  // model with all the fields and pairwise interactions

// Defines a random pairwise model:  ****************************** //
  // model = [ all fields + "Kpair" random pairwise interactions]
list<Interaction> Random_PairwiseModel_aux(mt19937 generator, int Kpair, double h=1, double J=1); // auxiliary function

list<Interaction> Random_PairwiseModel(int Kpair, double h=1, double J=1) // main function
  {  return Random_PairwiseModel_aux(gen, Kpair, h, J);  }

// Defines a random model:  *************************************** //
  // model = [ "K" random interactions]
list<Interaction> Random_Model_aux(mt19937 generator, int K, double g=1);  // auxiliary function

list<Interaction> Random_Model(int K, double g=1)  // main function
  {  return Random_Model_aux(gen, K, g);  }

// Randomize the parameter values of a given model:  ************** //
void Randomize_param(list<Interaction>& list_I, double g=1);

/********************************************************************/
/*****************    GENERATE_DATA_EXACT.CPP    ********************/
/********************************************************************/
/*******************    SAMPLES A DATASET    ************************/
/********************************************************************/
// list_I = list of operators and parameters of the model
// N = nb of datapoints

// Samples a dataset with "N" datapoints:  ************************ //
void Sample_dataset(list<Interaction> list_I, string output_filename, unsigned int N=1000);

// **************************************************************** //
// Samples a dataset with "N" datapoints,  ************************ //
// and compute the model averages and data averages  ************** //
// of all the model operators:                       ************** //
// fill this information in "list_I"                 ************** //
// **************************************************************** //
void Sample_dataset_AND_Print_ModelData_Info(list<Interaction>& list_I, string output_filename, unsigned int N=1000);

// INFORMATION ABOUT THE MODEL:
double* Probability_AllStates_Ising(list<Interaction> list_I, double *Z);
void Model_averages_Ising(list<Interaction> &list_I);

/********************************************************************/
/*****************************    MAIN    ***************************/
/********************************************************************/
int main()
{
// ********************  Do not modify   *********************** //
  // Initialize random generators:
  initialise_generator();

// Output filename: --> Check in the file "data.h"
  cout << "--->> Create OUTPUT Folder: (if needed) ";
  system( ("mkdir -p " + OUTPUT_directory).c_str() );


// ******************************************************************* //
// ***********************  EXAMPLE 1   ****************************** //
// Choose a model:
  int K=20; // total number of interactions
  list<Interaction> list_I = Random_PairwiseModel(K); // Random_PairwiseModel(K)

// Compute the model average of the selected operators:
  // (* This function is a bit time consuming and it is not necessary *)
  // (* Function here just to give more info on the model *)
  Model_averages_Ising(list_I);

// Print info about the generated model and the data:
  PrintTerm_ListInteraction(list_I);  // print in the terminal

// Sample a dataset:
  unsigned int N = 10000; //number of datapoints
  string data_filename = "n"+to_string(n)+"_K"+to_string(K)+"_RandM_dataset_N"+to_string(N)+".dat";
  
  Sample_dataset_AND_Print_ModelData_Info(list_I, data_filename, N);
  // OR: Run faster if you don't care about information on printed dataset:
  //Sample_dataset(list_I, data_filename, N);

// Print info about the generated model and the data:
  PrintTerm_ListInteraction(list_I, N);  // print in the terminal
  PrintFile_ListInteraction(list_I, "n"+to_string(n)+"_K"+to_string(K)+"_RandM_Info.dat", N);  // print model in a file


// ******************************************************************* //
// ***********************  EXAMPLE 2   ****************************** //
// *****  FIXED MODEL ARCHITECTURE + RANDOMIZE PARAMETER VALUES  ***** //
// ******************************************************************* //
  int Ntot_randM = 10;
  N = 1000; //number of datapoints
  string file_extension;

  for(int i=0; i<Ntot_randM; i++)
  {
    Randomize_param(list_I);
    file_extension = "n"+to_string(n)+"_K"+to_string(K)+"_RandM"+to_string(i);

  // Sample a dataset:
    data_filename = file_extension + "_dataset_N" + to_string(N) + ".dat";
    
    Sample_dataset_AND_Print_ModelData_Info(list_I, data_filename, N);
    // OR: Run faster if you don't care about information on printed dataset:
    //Sample_dataset(list_I, data_filename, N);

  // Print model and data info:
    //Model_averages_Ising(list_I);
    PrintFile_ListInteraction(list_I, file_extension+"_Info.dat", N);  // print model in a file
  }
  

  return 0;
}


