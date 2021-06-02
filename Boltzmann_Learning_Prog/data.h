#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/********************************************************************/
/**************************    CONSTANTS    *************************/
/******************    To Adapt: at each run    **********************/
/********************************************************************/
  const unsigned int n = 15;    // number of spins: only for function "print_basis" to use "bitset<n>()"

  //INPUT FILE:
  const string INPUT_filename = "INPUT/n15_K10_RandM0_dataset_N1000.dat";
  const string INPUT_test_logical = "INPUT/test_logical.dat";  // train = 0 ; test = 1

  //OUTPUT FILE:
  const string OUTPUT_directory = "OUTPUT/allBirdsM6_n19_10s_N10726_OldBasis/";
  const string filename = "allBirdsM6_n19_10s_N10726_OldBasis";  

/********************************************************************/
/**************************    CONSTANTS    *************************/
/*************************    Don't modify   ************************/
/********************************************************************/
  const string OUTPUT_filename = OUTPUT_directory + filename;
  
  const uint32_t un = 1;
  const uint32_t NOp_tot = (un << n) - 1;     // total number of operators = 2^n - 1

/********************************************************************/
/**************************    STRUCTURES    ************************/
/********************************************************************/
struct Interaction
{
  uint32_t Op;      // binary operator associated to the interaction
  unsigned int k;   // order of the interaction

  double g_Ising;   // parameter of the interaction in {-1,+1} representation  
  double g_ACE;     // parameter of the interaction in {0,1} representation

  double av_D;      // empirical average ("d" for data)
  double av_M;      // average in the Model
};



