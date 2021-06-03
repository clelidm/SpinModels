//g++ -I /usr/local/include/eigen3/ -I ./LBFGSpp-master/include -O2 main.cpp Models.cpp ReadDataFile.cpp BoltzmannLearning_Ising.cpp BoltzmannLearning_ACE.cpp
//time ./a.out
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <list>
#include <map>
#include <set> 
#include <ctime> // for chrono
#include <ratio> // for chrono
#include <chrono> // for chrono

using namespace std;


/********************************************************************/
/**************************    CONSTANTS    *************************/
/********************************************************************/
#include "data.h"

/******************************************************************************/
/**************************     READ FILE    **********************************/
/******************************************************************************/
map<uint32_t, unsigned int> read_datafile(string datafilename, unsigned int *N);   // O(N)  //N = data set size  //READ DATA and STORE them in Nset
void read_Nset (map<uint32_t, unsigned int> Nset, unsigned int N, string OUTPUTfilename); // PRINT Nset in a file  

//void separateFile_TrainTest(string OUTPUT_train, string OUTPUT_test, unsigned int *N_train, unsigned int *N_test);

/******************************************************************************/
/*************************  MODEL: LIST of INTERACTIONS ***********************/
/***************************   in Models.cpp     ******************************/
/******************************************************************************/

/************************** INDEPENDENT MODEL *********************************/
list<Interaction> IndepModel(map<uint32_t, unsigned int> Nset, unsigned int N, double *LogLi);

/**************************** OTHER MODELS ************************************/
list<Interaction> FullyConnectedPairwise();

/**************************** READ MODEL FROM FILE ****************************/
list<Interaction> Read_Op_IntegerRepresentation_fromFile(string datafilename);

/************************* PRINT MODEL in TERMINAL ****************************/
void PrintTerm_ListInteraction (list<Interaction> list_I);

/******************************************************************************/
/**************************** Boltzmann Learning ******************************/
/******************************************************************************/
double BoltzmannLearning_Ising(map<uint32_t, unsigned int> Nset, list<Interaction> &list_I, unsigned int N);
double BoltzmannLearning_ACE(map<uint32_t, unsigned int> Nset, list<Interaction> &list_I, unsigned int N);

/******************************************************************************/
/************************** MAIN **********************************************/
/******************************************************************************/
//Rem : ---> 2^30 ~ 10^9
int main()
{
//*******************************************
//********** READ DATA FILE:  ***************     -->  data are put in Nset:
//*******************************************    // Nset[mu] = # of time state mu appears in the data set

  cout << "--->> Create OUTPUT Folder: (if needed) ";
  system( ("mkdir -p " + OUTPUT_directory).c_str() );
  cout << endl;

  cout << endl << "*******************************************************************************************"; 
  cout << endl << "****************  TRAINING = TEST on the WHOLE dataset:  **********************************";
  cout << endl << "*******************************************************************************************" << endl;

  cout << endl << "************************************  Read dataset:  **************************************" << endl;

  unsigned int N = 0;  // set in 'read_datafile()'
  map<uint32_t, unsigned int> Nset = read_datafile(INPUT_filename, &N);  //  double Nd = (double) N;
  read_Nset(Nset, N, OUTPUT_filename + "_Nset.dat");   // print Nset in a file

  cout << endl << "***********************************  Independent model:  **********************************" << endl;
  cout << "--->> Inferred field for the best Independent Model associated to the whole dataset '" << INPUT_filename << "'" << endl << endl;

  double L_indep;  // LogLikelihood of the independent model
  list<Interaction> list_I_indep = IndepModel(Nset, N, &L_indep);

  cout << endl << "*******************************************************************************************";
  cout << endl << "**************************  EX. LogLikelihood and BIC :  **********************************";
  cout << endl << "*******************************************************************************************" << endl;
  int K=0; double L=0;

  cout << endl << "***********************************  Independent model:  **********************************" << endl;

  K = list_I_indep.size();
  L = L_indep;     
  
  cout << "Number of parameters, K = " << K << endl;
  cout << "Max Log-Likelihood, L = " << L << endl;
  cout << "BIC: " << L - K * log( ((double) N) /2.) / 2. <<  endl << endl;

  cout << endl << "****************************  Fully connected pairwise model:  *****************************" << endl;

  list<Interaction> list_I_pairwise = FullyConnectedPairwise();
  K = list_I_pairwise.size();
 

   // ********* TIME TEST with chrono:
  chrono::high_resolution_clock::time_point t1, t2;

  // ************** fit with {-1,1}-model:
  t1 = chrono::high_resolution_clock::now(); 
  cout << "--->> Finding best parameters (gradient descent), for {-1,1}-model.." << endl;
  L = BoltzmannLearning_Ising (Nset, list_I_pairwise, N);
  t2 = chrono::high_resolution_clock::now();
  cout << "Timings test: time machine = " << (chrono::duration_cast<chrono::nanoseconds>(t2-t1).count())/1e9 << " sec." << endl << endl;

  cout << "FullyConnectedPairwise: K = " << K << endl;
  cout << "Max Log-Likelihood, L = " << L << endl;
  cout << "BIC: " << L - K * log( ((double) N) /2.) / 2. <<  endl << endl;
  PrintTerm_ListInteraction (list_I_pairwise);


  // ************** fit with {0,1}-model:
  t1 = chrono::high_resolution_clock::now(); 
  cout << "--->> Finding best parameters (gradient descent), for {0,1}-model.." << endl;
  L = BoltzmannLearning_ACE (Nset, list_I_pairwise, N);
  t2 = chrono::high_resolution_clock::now();
  cout << "Timings test: time machine = " << (chrono::duration_cast<chrono::nanoseconds>(t2-t1).count())/1e9 << " sec." << endl << endl;

  cout << "FullyConnectedPairwise: K = " << K << endl;
  cout << "Max Log-Likelihood, L = " << L << endl;
  cout << "BIC: " << L - K * log( ((double) N) /2.) / 2. <<  endl << endl;
  PrintTerm_ListInteraction (list_I_pairwise);

// ************** Fit a model defined in input file:
  cout << "Read model from file:" << endl;
  list<Interaction> list_I_Model = Read_Op_IntegerRepresentation_fromFile("INPUT/n15_K10_RandM0_Info.dat");

  cout << "Number of interaction found: K = " << list_I_Model.size() << endl;

  cout << "--->> Finding best parameters (gradient descent), for {-1,1}-model.." << endl;
  L = BoltzmannLearning_Ising (Nset, list_I_Model, N);
  cout << "Max Log-Likelihood, L = " << L << endl;
  cout << "BIC: " << L - K * log( ((double) N) /2.) / 2. <<  endl << endl;
  PrintTerm_ListInteraction (list_I_Model);

  return 0;
}


