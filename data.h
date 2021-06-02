#include <iostream>
#include <vector>
#include <cmath>
//#include <random>

using namespace std;

/********************************************************************/
/**************************    CONSTANTS    *************************/
/******************    To Adapt: at each run    **********************/
/********************************************************************/
  const unsigned int n = 15;    // number of spins: only for function "print_basis" to use "bitset<n>()"

  //OUTPUT FILE:
  const string OUTPUT_directory = "OUTPUT/";
  //const string filename = "data.dat";

/********************************************************************/
/**************************    CONSTANTS    *************************/
/*************************    Don't modify   ************************/
/********************************************************************/
  //const string OUTPUT_filename = OUTPUT_directory + filename;
  
  const uint32_t un = 1;
  const uint32_t NOp_tot = (un << n) - 1;     // total number of operators = 2^n - 1

/********************************************************************/
/**************************    STRUCTURES    ************************/
/********************************************************************/
struct Interaction
{
  uint32_t Op;      // binary operator associated to the interaction
  double g;   // parameter of the interaction in {-1,+1} representation
  double av_M;      // average in the Model
  double av_D;      // average in the generated Data
};
