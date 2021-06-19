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
          gen.seed(seed);     //for mt19937
}

/********************************************************************/
/**************************    MODELS.CPP    ************************/
/********************************************************************/
// *** Print information about the model in list_I and the generated data (if applies):
void PrintTerm_ListInteraction (list<Interaction> list_I, unsigned int N=1);
void PrintFile_ListInteraction(list<Interaction> list_I, string output_filename, unsigned int N=1);

// *** Defines specific models:  ************************************** //
list<Interaction> IndepModel(double h=1);     // model with all the "n" fields
list<Interaction> FullyConnectedPairwiseModel(double h=1, double J=1);  // model with all the fields and pairwise interactions

// *** Defines a random pairwise model:  ****************************** //
// *** model = [ all fields + "Kpair" random pairwise interactions]
list<Interaction> Random_PairwiseModel_aux(mt19937 generator, int Kpair, double h=1, double J=1); // auxiliary function

list<Interaction> Random_PairwiseModel(int Kpair, double h=1, double J=1) // main function
  {  return Random_PairwiseModel_aux(gen, Kpair, h, J);  }

// *** Defines a random model:  *************************************** //
// *** model = [ "K" random interactions]
list<Interaction> Random_Model_aux(mt19937 generator, int K, double g=1);  // auxiliary function

list<Interaction> Random_Model(int K, double g=1)  // main function
  {  return Random_Model_aux(gen, K, g);  }

// *** Randomize the parameter values of a given model:  ************** //
void Randomize_param(list<Interaction>& list_I, double g=1);

// *** Read model from an input file:  ******************************** //
list<Interaction> ReadFile_Model_IntegerRepresentation(string Model_inputfile);
list<Interaction> ReadFile_Model_BinaryRepresentation(string Model_inputfile);

/********************************************************************/
/*****************    GENERATE_DATA_EXACT.CPP    ********************/
/********************************************************************/
/*******************    SAMPLES A DATASET    ************************/
/********************************************************************/
// *** list_I = list of operators and parameters of the model
// *** N = nb of datapoints

// *** Samples a dataset with "N" datapoints:  ******************** //
void Sample_dataset(list<Interaction> list_I, string output_filename, unsigned int N=1000);

// **************************************************************** //
// *** Samples a dataset with "N" datapoints,  ******************** //
// *** and compute the model averages and data averages  ********** //
// *** of all the model operators:                       ********** //
// *** fill this information in "list_I"                 ********** //
// **************************************************************** //
void Sample_dataset_AND_Print_ModelData_Info(list<Interaction>& list_I, string output_filename, unsigned int N=1000);

/********************************************************************/
/************    PRINT INFORMATION ABOUT THE MODEL    ***************/
/**********    and the GENERATED DATA if there were    **************/
/********************************************************************/
double* Probability_AllStates_Ising(list<Interaction> list_I, double *Z);
void Model_averages_Ising(list<Interaction> &list_I);

// *** CREATE A GENERIC FILENAME EXTENSION:
string filename_extension(int K, int i=-1)
{
  if (i<0)
    {return "n"+to_string(n)+"_K"+to_string(K)+"_RandM";}
  else
    {return "n"+to_string(n)+"_K"+to_string(K)+"_RandM"+to_string(i);}
}

/********************************************************************/
/*****************************    MAIN    ***************************/
/********************************************************************/
int main()
{
// ********************  Do not modify   *********************** //
// *** Initialize random generators:
  initialise_generator();

// *** Output filename: !! Define `OUTPUT_directory` in the file "data.h"
  cout << "--->> Create OUTPUT Folder: (if needed) ";
  system( ("mkdir -p " + OUTPUT_directory).c_str() );

  cout << endl << "*************************************************************************************";
  cout << endl << "**********************************  EXAMPLE 1 :  ************************************";
  cout << endl << "*************************************************************************************" << endl;

// *** Choose a model:
  int K=20; // total number of interactions
  list<Interaction> list_I = Random_PairwiseModel(K); 

// *** Other examples of models: 
  //list<Interaction> list_I = IndepModel();
  //list<Interaction> list_I = FullyConnectedPairwiseModel();
  //list<Interaction> list_I = Random_Model(K);

// *** Compute the model average of the selected operators:
  // (* This function is a time consuming and it is not necessary *)
  // (* Function here just to give more info on the model *)
  Model_averages_Ising(list_I);

// *** Print info about the generated model and the data:
  PrintTerm_ListInteraction(list_I);  // print in the terminal

// *** Number of datapoints:
  unsigned int N = 10000;

// *** Create matching data filename and model filename:
  string data_filename = filename_extension(K)+"_dataset_N"+to_string(N)+".dat";
  string model_filename = filename_extension(K)+"_Info.dat";  
  
// *** Sample a dataset, and fill model and data info in `list_I`:
  Sample_dataset_AND_Print_ModelData_Info(list_I, data_filename, N);

//*** OR *** Sample a dataset, without saving additional info about model and printed data
// *** (runs faster if you don't care about information on printed dataset):
  //Sample_dataset(list_I, data_filename, N);

// Print info about the generated model and the data:
  PrintTerm_ListInteraction(list_I, N);  // print in the terminal
  PrintFile_ListInteraction(list_I, model_filename, N);  // print model in a file

  cout << endl << "*************************************************************************************";
  cout << endl << "**********************************  EXAMPLE 2 :  ************************************";
  cout << endl << "*************   FIXED MODEL ARCHITECTURE + RANDOMIZE PARAMETER VALUES   *************";
  cout << endl << "*************************************************************************************" << endl;

  int Ntot_randM = 10;
  N = 1000; //number of datapoints
  string file_extension;

  for(int i=1; i<=Ntot_randM; i++)
  {
    Randomize_param(list_I);
    file_extension = filename_extension(K, i);

  // Sample a dataset:
    data_filename = file_extension + "_dataset_N" + to_string(N) + ".dat";
    Sample_dataset_AND_Print_ModelData_Info(list_I, data_filename, N);

    // OR: Run faster if you don't care about information on printed dataset:
    //Sample_dataset(list_I, data_filename, N);

  // Print model and data info:
    //Model_averages_Ising(list_I);
    PrintFile_ListInteraction(list_I, file_extension + "_Info.dat", N);  // print model in a file
  }

  cout << endl << "*************************************************************************************";
  cout << endl << "**********************************  EXAMPLE 3 :  ************************************";
  cout << endl << "*************************   READ MODEL FROM INPUT FILE   ****************************";
  cout << endl << "*************************************************************************************" << endl;

  //list_I = ReadFile_Model_IntegerRepresentation("INPUT/Model_Ex_IntegerRepresentation.dat");
  list_I = ReadFile_Model_BinaryRepresentation("INPUT/Model_Ex_BinaryRepresentation.dat");
  PrintTerm_ListInteraction(list_I);  // print in the terminal
  
  return 0;
}


