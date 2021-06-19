#include <random>
#include <list>
#include <set>
#include <fstream>
#include <sstream>

using namespace std;
/********************************************************************/
/**************************    CONSTANTS    *************************/
/********************************************************************/
#include "data.h"

/******************************************************************************/
/**************************     RANDOM GENERATOR    ***************************/
/******************************************************************************/
double rand_param(double Amp)
{ 
  return (drand48() > 0.5)? Amp*drand48() : -Amp*drand48();
}

/******************************************************************************/
/**************************   PRINT MODEL IN TERMINAL   ***********************/
/*****************************    ALL INTERACTIONS    *************************/
/******************************************************************************/
double std_M(double av_M, double Nd)
{
  return (Nd==1) ? 0 : sqrt( (1.+av_M)*(1-av_M)/Nd )/2.;
}

void PrintTerm_ListInteraction(list<Interaction> list_I, unsigned int N=1)
{
  list<Interaction>::iterator it;
  double Nd = (double) N;

  cout << "# In case a dataset was generated: N=" << N << endl;
  cout << "# 1:Op \t 2:order \t 3:g_Ising \t 4:Model_av \t 5:Data_av \t 6:Std_of_avD_from_avM(for dataset with N datapoints)" << endl;
  for (it=list_I.begin(); it!=list_I.end(); it++)
  {
    cout << "Op = " << (*it).Op << " = " << bitset<n>((*it).Op)  << "\t " << bitset<n>((*it).Op).count() << "\t g_Ising = " << (*it).g;
    cout << "\t Model_av = " << (*it).av_M;
    cout << "\t Data_av = " << (*it).av_D;
    cout << "\t Std_of_avD_from_avM = " << std_M((*it).av_M, Nd) << endl;
  }
  cout << endl;
}

void PrintFile_ListInteraction(list<Interaction> list_I, string output_filename, unsigned int N=1)
{
  list<Interaction>::iterator it;
  double Nd = (double) N;

  fstream file((OUTPUT_directory + output_filename).c_str(), ios::out);

  file << "# In case a dataset was generated: N=" << N << endl;
  file << "# 1:Operator \t 2:Order \t 3:g_Ising \t 4:Model_average \t 5:Data_average";
  file << "\t 6:Std_of_avD_from_avM(for dataset with N datapoints)";
  file << "\t 7:Operator_bit_representation" << endl;
  file << "### " << endl;
  for (it=list_I.begin(); it!=list_I.end(); it++)
  {
    file << (*it).Op << "\t " << bitset<n>((*it).Op).count() << "\t " << (*it).g;
    file << "\t " << (*it).av_M << "\t " << (*it).av_D;
    file << "\t " << std_M((*it).av_M, Nd);
    file << "\t " << bitset<n>((*it).Op) << endl;
  }
  file << endl;

  file.close();
}

/******************************************************************************/
/**************************     INDEPENDENT MODEL    **************************/
/******************************************************************************/
list<Interaction> IndepModel(double h=1)
{
  Interaction I;
  list<Interaction> list_I;

  uint32_t Op1 = 1;

  for(int i=1; i<=n; i++) // n fields
  {
    I.Op = Op1;
    I.g = rand_param(h);
    I.av_M = 0;         I.av_D = 0;
    list_I.push_back(I);

    Op1 = Op1 << 1;
  }

  return list_I;
}

/******************************************************************************/
/*********************     FULLY CONNECTED PAIRWISE    ************************/
/******************************************************************************/
list<Interaction> FullyConnectedPairwiseModel(double h=1, double J=1)
{
  Interaction I;
  list<Interaction> list_I;

  uint32_t Op1 = 1, Op2 = 1;

  for(int i=1; i<=n; i++) // n fields
  {
    I.Op = Op1;
    I.g = rand_param(h);
    I.av_M = 0;         I.av_D = 0;
    list_I.push_back(I);

    Op1 = Op1 << 1;
  }

  Op1 = 1;

  for(int i=1; i<=n; i++) // n(n-1)/2 pairwise interactions
  {    
    Op2 = Op1 << 1;
    for (int j=i+1; j<=n; j++)
    {
      I.Op = Op1 + Op2;
      I.g = rand_param(J);
      I.av_M = 0;         I.av_D = 0;
      list_I.push_back(I);
      
      Op2 = Op2 << 1; 
    }
    Op1 = Op1 << 1;      
  }
  return list_I;
}

/******************************************************************************/
/************************     RANDOM PAIRWISE MODEL    ************************/
/******************************************************************************/
list<Interaction> Random_PairwiseModel_aux(mt19937 gen, int Kpair, double h=0.5, double J=0.5)  // !important!  K < n(n-1)/2
// model with all the "n" fields
// and "Kpair" pairwise interactions chosen randomly
// Very basic version
{
  list<Interaction> list_I;
  int Kpair_tot = n*(n-1)/2;
  //cout << "Kpair_tot = " << Kpair_tot << endl;

  if(Kpair > Kpair_tot) 
  {  
    cout << "Error in call of function *Random_PairwiseModel(int Kpair)*:" << endl;
    cout << " Kpair must be smaller than n*(n-1)/2 = " << Kpair_tot << endl; 
  }

  else
  {
  Interaction I;
  uint32_t Op = 1;

  for(int i=1; i<=n; i++) // n fields
  {
    I.Op = Op;
    I.g = rand_param(h);
    I.av_M = 0;         I.av_D = 0;
    list_I.push_back(I);

    Op = Op << 1;
  }

  Op = 0;
  uint32_t Op1 = 1, Op2 = 1;

  int index=0;

  double* v_allPairwise = (double *)malloc(Kpair_tot*sizeof(double));
  bool* v_selected = (bool *)malloc(Kpair_tot*sizeof(bool));

  for(int i=0; i<n; i++) // all n(n-1)/2 pairwise interactions
  {    
    Op2 = Op1 << 1;
    index = i*n - i*(i+1)/2;
    for (int j=0; j<n-i-1; j++)
    {
      Op = Op1 + Op2;
      v_allPairwise[index+j]=Op;
      v_selected[index+j]=false;
      //cout << i << "\t " <<  j << "\t " << index+j << "\t " << bitset<n>(Op) << endl;
      Op2 = Op2 << 1; 
    }
    Op1 = Op1 << 1;      
  }

  // Select Kmin elements
  int Kmin = (Kpair < Kpair_tot/2)? Kpair : (Kpair_tot-Kpair);
  int count=0;

  std::uniform_int_distribution<int> uni(0.,Kpair_tot-1);
  
  while(count < Kmin)
  { 
    if( !(v_selected[uni(gen)]) ) 
    { 
      v_selected[uni(gen)]=true;
      count++;
    }
  }
  //cout << "Kmin = " << Kmin << endl;

  bool select_bool = (Kmin == Kpair)? true : false;

  for(int i=0; i<Kpair_tot; i++)
  {
    if(v_selected[i] == select_bool)
    {
      I.Op = v_allPairwise[i];
      I.g = rand_param(J); 
      I.av_M = 0;         I.av_D = 0;
      list_I.push_back(I);
    }
  }
  //cout << list_I.size() << "\t " << Kpair + n << endl;
  }

  return list_I;
}

/******************************************************************************/
/****************************     RANDOM  MODEL    ****************************/
/******************************************************************************/
list<Interaction> Random_Model_aux(mt19937 gen, int K, double g=1)
// model with "K" interactions randomly chosen
// Very dumb version
{
  list<Interaction> list_I;

  if(K > NOp_tot) // NOp_tot = total number of interactions in the system
  {  
    cout << "Error in call of function *Random_Model(int K)*:" << endl;
    cout << " K must be smaller than 2**n - 1 = " << NOp_tot << endl; 
  }
  else
  {
    Interaction I;
    std::uniform_int_distribution<int> uni(1, NOp_tot);

    set<uint32_t> selected_Op;

    while(selected_Op.size() < K)
    {
      selected_Op.insert(uni(gen));
    }

    for(set<uint32_t>::iterator it=selected_Op.begin(); it!=selected_Op.end(); it++)
    {
      I.Op = *it;
      I.g = rand_param(g);
      I.av_M = 0;         I.av_D = 0;
      list_I.push_back(I);
    }
  }

  //cout << "K = " << K << "\t nb of Op = " << list_I.size() << endl;
  return list_I;
} 

/******************************************************************************/
/*********************     RANDOMIZE PARAMETER VALUES    **********************/
/******************************************************************************/
void Randomize_param(list<Interaction>& list_I, double g=1)
{
  for(list<Interaction>::iterator it=list_I.begin(); it!=list_I.end(); it++)
  {
    (*it).g = rand_param(g);
    (*it).av_M = 0;   // clean model averages
    (*it).av_D = 0;   // clean data averages
  }
}

/******************************************************************************/
/**********************     READ A MODEL from FILE   **************************/
/******************************************************************************/
/******  Structure of input file:    ******************************************/
/******  1) operators should be written under form a) or b) (see below) *******/
/******  2) First column  = operators (one per line)                         **/
/******  3) Second column = corresponding parameter value                    **/
/******************************************************************************/

/******************************************************************************/
/*** Operators/interactions can be written in two different versions:   *******/
/***   a) as a binary representation of the spin involved in the interaction; */
/***   b) as the integer value of that binary representation.                 */
/******************************************************************************/
/****  Ex. for a system with n=4 spin:  ***************************************/
/****      -- a field operator on the last digit would be encoded as 0001,   **/
/****      which has the integer representation 1  -->   0001 = 1      ********/
/****      -- a pairwise operator s1 and s2 would be written as 0011,  ********/
/****      which has the integer representation 3  -->   0011 = 3      ********/


/******************************************************************************/
/*** VERSION a) Operators are written using the binary       ******************/
/****           representation of the interactions           ******************/
/******************************************************************************/
list<Interaction> ReadFile_Model_BinaryRepresentation(string Model_inputfile)
{
  Interaction I;
  list<Interaction> list_I;

  ifstream myfile (Model_inputfile.c_str());
  string line, temp;    
  stringstream sstream; 

  uint32_t Op = 0;
  double g=0;

  if (myfile.is_open())
  {
    while ( getline (myfile,line))
    {
      line+=" ";
      sstream << line;   // Storing the whole string into string stream
      sstream >> temp;   // extracting first "word" from stream

      if ( temp.size() == n )   // Checking the given "word" is integer or not
      { 
        I.Op = bitset<n>(temp).to_ulong();      //cout << "Op = " << I.Op << "\t = " << bitset<n>(I.Op) << endl;

        temp = "";
        sstream >> temp;  // extracting second "word" from stream

        if (stringstream(temp) >> g) 
        {
          I.g = g;    //cout << "g = " << g << endl; // << "\t temp = " << temp << end;
        }
        else   {  I.g = 0;  }

        I.av_M = 0;         I.av_D = 0;
        list_I.push_back(I);
      }
      temp = "";
      sstream.str("");
    }
  }

  myfile.close();

  return list_I;
}

/******************************************************************************/
/*** VERSION b) Operators are written as the integer values of the binary *****/
/****           representation of the interactions           ******************/
/******************************************************************************/
list<Interaction> ReadFile_Model_IntegerRepresentation(string Model_inputfile)
{
  Interaction I;
  list<Interaction> list_I;

  ifstream myfile (Model_inputfile.c_str());
  string line, temp;    
  stringstream sstream; 

  uint32_t Op = 0;
  double g=0;

  if (myfile.is_open())
  {
    while ( getline (myfile,line))
    {
      line+=" ";
      sstream << line;   // Storing the whole string into string stream
      sstream >> temp;   // extracting first "word" from stream

      if (stringstream(temp) >> Op)   // Checking the given "word" is integer or not
      { 
        I.Op = Op;        //cout << "Op = " << Op << "\t = " << bitset<n>(Op) << "\t";

        temp = "";
        sstream >> temp;  // extracting second "word" from stream

        if (stringstream(temp) >> g) 
        {
          I.g = g;    //cout << "g = " << g << endl; // << "\t temp = " << temp << end;
        }
        else   {  I.g = 0;  }

        I.av_M = 0;         I.av_D = 0;
        list_I.push_back(I);
      }
      temp = "";
      sstream.str("");
    }
  }

  myfile.close();

  return list_I;
}


