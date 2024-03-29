#include <fstream>
#include <map>
#include <list>
#include <bitset> 
using namespace std;

//For learning:
#include <Eigen/Core>
#include <LBFGS.h>
using namespace Eigen;  //cf Eigen::VectorXd
using namespace LBFGSpp;

/**************************    CONSTANTS    *************************/
#include "data.h"

/******************************************************************************/
/*********************   OPERATOR EVALUATED on a STATE   **********************/
/***********************   depend on the convention    ************************/
/******************************************************************************/

/****************************   !! Convention !!   ****************************/
/****     For Ising:      {1 in data <--> -1 in model}                     ****/
/****                     {0 in data <--> +1 in model}                     ****/
/******************************************************************************/
// s_i in {-1;1}   !! Convention !! {0,1} in data <-> {1,-1} in model
int Op_Ising(uint32_t Op, uint32_t state)
  {  return ( (bitset<n>(Op & state).count())%2 )?(-1):1;   } 

/******************************************************************************/
/*************************     EMPIRICAL AVERAGES     *************************/
/******************************************************************************/
//Number of time an operator is equal to 1 ( = <phi> in the {0,1} representation )
unsigned int k1_op(map<uint32_t, unsigned int> Nset, uint32_t op);  // Complexity = O(|Nset|)

double op_av_Ising(map<uint32_t, unsigned int> Nset, uint32_t op, unsigned int N)
{
  return ( ((double) N) - 2.*k1_op(Nset, op) ) / ((double) N); // [ [-1 * k1] + [+1 * (N-k1)] ] / N
}
/************************    Empirical averages all op    *************************/
void empirical_averages_Ising(map<uint32_t, unsigned int> Nset, list<Interaction> &list_I, unsigned int N) 
{
  list<Interaction>::iterator I;

  for (I = list_I.begin(); I != list_I.end(); I++)
  {
    (*I).av_D = op_av_Ising(Nset, (*I).Op, N);
  }
}
/******************************************************************************/
/**************************     MODEL AVERAGES    *****************************/
/******************************************************************************/
void Model_averages_Ising(double *P, double Z, list<Interaction> &list_I) 
{
  int Op_s = 1; // value of the operator for the state s ; \in {-1; 1}

  list<Interaction>::iterator I;

  for (I = list_I.begin(); I != list_I.end(); I++)
  {
    (*I).av_M = 0.;
    for (uint32_t state = 0; state <= NOp_tot; state++)
    {
      Op_s = Op_Ising((*I).Op, state);
      (*I).av_M += Op_s * P[state];
    }
    (*I).av_M = (*I).av_M / Z;
  }
}
/******************************************************************************/
/************* PARTITION FUNCTION and PROBA of ALL STATES *********************/
/******************************************************************************/
// return the probability (not normalised) of all the states and compute the partition function

double* Probability_AllStates_Ising(list<Interaction> list_I, double *Z)  // Convention {-1;1} 
//!! Convention !!:  {1 in data <--> -1 in model}  and  {0 in data <--> 1 in model} 
{
  double H = 0; // -energy of the state
  int Op_s = 1; // value of the operator for the state s ; \in {-1; 1}

  list<Interaction>::iterator I;
  double* all_P = (double*)malloc((NOp_tot+1)*sizeof(double));
  (*Z) = 0;

  for (uint32_t state = 0; state <= NOp_tot; state++)
  {
    H=0;  // here H is (H = -Hamiltonian)
    for (I = list_I.begin(); I != list_I.end(); I++)
    {
      Op_s = Op_Ising((*I).Op, state);
      H += (*I).g_Ising * Op_s;
    }
    all_P[state] = exp(H);
    (*Z) += all_P[state];
  }

  return all_P;
}

/******************************************************************************/
/****************************  LogLikelihood  *********************************/
/******************************************************************************/
double LogL_Ising(list<Interaction> list_I, double Z, unsigned int N)
{
  double LogLi = 0;
  list<Interaction>::iterator I;

  for (I = list_I.begin(); I != list_I.end(); I++)
  {    LogLi += (*I).g_Ising * (*I).av_D;   } 

  return ((double) N) * (LogLi-log(Z));
}
/******************************************************************************/
/****************************  Boltzmann Learning  ****************************/
/******************************************************************************/

/************************     ISING MODEL {+1; -1}    *************************/
class LogL_class_Ising
{
private:
  list<Interaction> li_I;
  unsigned int N;
public:
  LogL_class_Ising(list<Interaction> list_I, unsigned int N_) : li_I(list_I), N(N_) {}
  double operator()(const VectorXd& x, VectorXd& grad)
  {
    list<Interaction>::iterator I;

    int i=0;
    for (I = li_I.begin(); I != li_I.end(); I++)
      {    (*I).g_Ising = x[i];   i++;  }

    double Z = 0; double *P = Probability_AllStates_Ising(li_I, &Z);
    Model_averages_Ising(P, Z, li_I);

    double LogLi = LogL_Ising(li_I, Z, N);

    i=0;
    for (I = li_I.begin(); I != li_I.end(); I++)
    {
      grad[i] = -((double) N) * ( (*I).av_D - (*I).av_M); 
      i++;
    } 
    free(P);

    return -LogLi;
  }
};


void Print_Proba(double *P , double Z)  // Convention {-1;1} 
//!! Convention !!:  {1 in data <--> -1 in model}  and  {0 in data <--> 1 in model} 
{
  for (uint32_t state = 0; state <= NOp_tot; state++)
  {
    cout << state << "\t" << bitset<n>(state) << "\t" << P[state]/Z << endl;
  }
}


double BoltzmannLearning_Ising(map<uint32_t, unsigned int> Nset, list<Interaction> &list_I, unsigned int N)
{
  unsigned int K = list_I.size();
  empirical_averages_Ising(Nset, list_I, N);

  // Set up parameters:
  LBFGSParam<double> param;
  param.epsilon = 1e-6;
  param.max_iterations = 100;
 
  // Create solver and function object:
  LBFGSSolver<double> solver(param);
  LogL_class_Ising LogLi(list_I, N);

  // Initial guess:
  VectorXd g = VectorXd::Zero(K);

  // g will be overwritten to be the best point found:
  double maxLogLi;
  int niter = solver.minimize(LogLi, g, maxLogLi);

  //updating the parameters:
  int i=0;  list<Interaction>::iterator I;
  for (I = list_I.begin(); I != list_I.end(); I++)
  {    
    (*I).g_Ising = g[i];    i++;
  }

  //updating the model averages:
  double Z = 0; double *P = Probability_AllStates_Ising(list_I, &Z);
  Model_averages_Ising(P, Z, list_I);
  free(P);

  cout << "Number of iterations: " << niter << " iterations" << std::endl;

  return -maxLogLi;
}





