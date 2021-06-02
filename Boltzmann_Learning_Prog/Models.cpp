#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <bitset> 

using namespace std;

/********************************************************************/
/**************************    CONSTANTS    *************************/
/********************************************************************/
#include "data.h"

/******************************************************************************/
/**************************   PRINT MODEL IN TERMINAL   ***********************/
/*****************************    ALL INTERACTIONS    *************************/
/******************************************************************************/
void PrintTerm_ListInteraction (list<Interaction> list_I)
{
  list<Interaction>::iterator it;

  cout << "# 1:Op \t 2:g_Ising \t 3:g_ACE \t 4:Emp_av \t 5:Model_av" << endl;
  for (it=list_I.begin(); it!=list_I.end(); it++)
  {
    cout << "Op = " << (*it).Op << " = " << bitset<n>((*it).Op) << "\t g_Ising = " << (*it).g_Ising << "\t\t" << "\t g_ACE = " << (*it).g_ACE;
    cout << "\t\t Emp_av = " << (*it).av_D << "\t Model_av = " << (*it).av_M << endl;
  }
  cout << endl;
}

/******************************************************************************/
/**************************     INDEPENDENT MODEL    **************************/
/******************************************************************************/
unsigned int k1_op(map<uint32_t, unsigned int> Nset, uint32_t op)  // Complexity = O(|Nset|)
{
  unsigned int k1=0;
  map<uint32_t, unsigned int>::iterator it;  // iterator on Nset

  for (it = Nset.begin(); it!=Nset.end(); ++it)
    {    k1 += (bitset<n>( ((*it).first) & op ).count() % 2)*((*it).second);   }

  return k1;
}

list<Interaction> IndepModel(map<uint32_t, unsigned int> Nset, unsigned int N, double *LogLi)
{
  Interaction I;
  list<Interaction> list_I;

  double Nd = (double) N;
  uint32_t Op = 1; double p1;
  (*LogLi) = 0;

  cout << "Fields associated to the independent model: " << endl;
  cout << "# 1:Op\t 2:p1 \t 3:h_Ising \t 4:h_ACE" << endl << endl;

  for (int i=0; i<n; i++)
  {
    I.Op = Op;      I.k = 1;
    p1 = ((double) k1_op(Nset, Op)) / Nd;
    (*LogLi) += p1 * log(p1) + (1-p1) * log(1-p1);

    I.g_Ising = 0.5*log( (1.- p1) / p1 );
    I.g_ACE = -2.*I.g_Ising;

    I.av_D = 0; I.av_M = 0;
    list_I.push_back(I); 
    cout << "Op = " << bitset<n>(I.Op) << "\t p1 = " << p1 << " \t\t h_Ising = " << I.g_Ising << " \t h_ACE = " << I.g_ACE << endl;

    Op = Op << 1;
  }
  cout << endl;

  (*LogLi) = Nd * (*LogLi);
  return list_I;
}

/******************************************************************************/
/*********************     FULLY CONNECTED PAIRWISE    ************************/
/******************************************************************************/
list<Interaction> FullyConnectedPairwise()
{
  Interaction I;
  list<Interaction> list_I;

  uint32_t Op1 = 1, Op2 = 1;

  for(int i=1; i<=n; i++) // n fields
  {
    I.Op = Op1;    I.k = 1;
    I.g_Ising = 0;  I.g_ACE = 0;
    I.av_D = 0; I.av_M = 0;
    list_I.push_back(I);

    Op1 = Op1 << 1;
  }

  Op1 = 1;

  for(int i=1; i<=n; i++) // n(n-1)/2 pairwise interactions
  {    
    Op2 = Op1 << 1;
    for (int j=i+1; j<=n; j++)
    {
      I.Op = Op1 + Op2;     I.k = 2;
      I.g_Ising = 0;  I.g_ACE = 0;
      I.av_D = 0; I.av_M = 0;
      list_I.push_back(I);
      
      Op2 = Op2 << 1; 
    }
    Op1 = Op1 << 1;      
  }
  return list_I;
}

/******************************************************************************/
/******************************     ANY MODEL    ******************************/
/*****************   from a vector of uint32_t (operators)   ******************/
/******************************************************************************/
list<Interaction> Build_Model(uint32_t Op_M[], int K)
{
  Interaction I;
  list<Interaction> list_I;
  int Kmax = sizeof(Op_M)/sizeof(uint32_t);

  if (K > Kmax) { cout << "Error, K > sizeof(Op_M)" << endl;   } else { Kmax = K; }

  for (int i=0; i<Kmax; i++)
  {
    I.Op = Op_M[i];
    I.k = bitset<n>(Op_M[i]).count();
    I.g_Ising = 0;  I.g_ACE = 0;
    I.av_D = 0; I.av_M = 0;
    list_I.push_back(I);    
  }

  return list_I;
}

/******************************************************************************/
/***************************     READ A MODEL    ******************************/
/********************   from a file:    ***************************************/
/******  1) operators should be written under form a) or b) (see below) *******/
/******  2) operators should be in the first column of each line of the file **/
/******  3) file should contain one operator per line   ***********************/
/******************************************************************************/


/******************************************************************************/
/*** Operators/interactions can be written in two different versions:   *******/
/***   a) as a binary representation of the spin involved in the interaction; */
/***   b) as the integer value of that binary representation.                 */
/******************************************************************************/
/****  Ex. for a system with n=4 spin:  ***************************************/
/****      -- a field operator on the last digid would be encoded as 0001,   **/
/****      which has the integer representation 1  -->   0001 = 1      ********/
/****      -- a pairwise operator s1 and s2 would be written as 0011,  ********/
/****      which has the integer representation 3  -->   0011 = 3      ********/
/******************************************************************************/


/******************************************************************************/
/*** VERSION a) Operators are written as the binary          ******************/
/****           representation of the interactions           ******************/
/******************************************************************************/

list<Interaction> Read_Op_BinaryRepresentation_fromFile(string datafilename)
{
  Interaction I;
  list<Interaction> list_I;

  ifstream myfile (datafilename.c_str());
  string line, line2;     
  uint32_t Op = 0;

  if (myfile.is_open())
  {
    while ( getline (myfile,line))
    {
      line2 = line.substr (0,n);          //take the n first characters of line

      I.Op = bitset<n>(line2).to_ulong();   //convert string line2 into a binary integer
      I.k = bitset<n>(I.Op).count();
      I.g_Ising = 0;  I.g_ACE = 0;
      I.av_D = 0; I.av_M = 0;
      list_I.push_back(I);   
    }
    myfile.close();
  }

  return list_I;
}

/******************************************************************************/
/*** VERSION b) Operators are written as the integer values of the binary *****/
/****           representation of the interactions           ******************/
/******************************************************************************/
list<Interaction> Read_Op_IntegerRepresentation_fromFile(string datafilename)
{
  Interaction I;
  list<Interaction> list_I;

  ifstream myfile (datafilename.c_str());
  string line, temp;    
  stringstream sstream; 
  uint32_t Op = 0;

  if (myfile.is_open())
  {
    while ( getline (myfile,line))
    {
      sstream << line;   // Storing the whole string into string stream
      sstream >> temp;   // extracting first "word" from stream

      if (stringstream(temp) >> Op)   // Checking the given "word" is integer or not
      { 
        //cout << Op << endl;
        I.Op = Op;   //convert string line2 into a binary integer
        I.k = bitset<n>(I.Op).count();
        I.g_Ising = 0;  I.g_ACE = 0;
        I.av_D = 0; I.av_M = 0;
        list_I.push_back(I);   
      }

      temp = "";
      sstream.str("");
    }
    myfile.close();
  }

  return list_I;
}


