#include <fstream>
#include <map>
#include <bitset>

using namespace std;

/********************************************************************/
/**************************    CONSTANTS    *************************/
/********************************************************************/
#include "data.h"

/******************************************************************************/
/**************************     READ FILE    **********************************/
/******************************************************************************/
/**************    READ DATA and STORE them in Nset    ************************/
map<uint32_t, unsigned int> read_datafile(string datafilename, unsigned int *N)   // O(N)  //N = data set size
{
  string line, line2;     uint32_t nb = 0;
  (*N) = 0;            // N = dataset size (global variable)
  cout << endl << "--->> Read \"" << datafilename << "\",\t Build Nset..." << endl;

// ***** data is stored in Nset:  ********************************
  map<uint32_t, unsigned int> Nset; // Nset[mu] = #of time state mu appears in the data set

  ifstream myfile (datafilename.c_str());

  if (myfile.is_open())
  {
    while ( getline (myfile,line))
    {
      line2 = line.substr (0,n);          //take the n first characters of line
      nb = 
        <n>(line2).to_ulong();   //convert string line2 into a binary integer
      Nset[nb] += 1;
      (*N)++;
    }
    myfile.close();
  }
  else cout << "Unable to open file"; 

  cout << "\t\t data size, N = " << (*N) << endl;
  cout << "\t\t number of different states, Nset.size() = " << Nset.size() << endl;

  return Nset;
}

/****************    PRINT Nset in file:    ************************/
void read_Nset (map<uint32_t, unsigned int> Nset, unsigned int N, string OUTPUTfilename)
// map.second = nb of time that the state map.first appears in the data set
{
  map<uint32_t, unsigned int>::iterator it;
  int Ncontrol = 0;

  fstream file(OUTPUTfilename.c_str(), ios::out);
  file << "#N = " << N << endl;
  file << "#Total number of accessible states = " << NOp_tot << endl;
  file << "#Number of visited states, Nset.size() = " << Nset.size() << endl;
  file << "#" << endl;
  file << "#1: state \t #2: nb of pts in state \t #3: Pba state" << endl;

  for (it = Nset.begin(); it!=Nset.end(); ++it)
  {
    file << it->first << ":\t" << bitset<n>(it->first) << " => " << it->second; // << endl;
    file << "  \t  P = " << it->second / (float) N << endl;
    Ncontrol += it->second;
  }

  if (Ncontrol != N) { cout << "Error function \'read_Nset\': Ncontrol != N" << endl;  }

  file.close();
}

/******************************************************************************/
/*********************     SEPARATE TRAIN/TEST FILE    ************************/
/******************************************************************************/
void separateFile_TrainTest(string OUTPUT_train, string OUTPUT_test, unsigned int *N_train, unsigned int *N_test)
{
  string line, line2; 
  string line_test;

  bool test=0;

  // INPUT:
  ifstream myfile (INPUT_filename.c_str());
  ifstream myfile_logicaltest (INPUT_test_logical.c_str());

  // OUTPUT:
  fstream file_test(OUTPUT_test.c_str(), ios::out);
  fstream file_train(OUTPUT_train.c_str(), ios::out);

  if (myfile.is_open() && myfile_logicaltest.is_open() )
  {
    while ( getline (myfile,line))
    {
      getline (myfile_logicaltest,line_test);
      test = stoi(line_test);  //cout << test;
      if (test==1) 
        { file_test << line << endl; (*N_test)++; }
      else
        { file_train << line << endl; (*N_train)++; }        
    }
  }
  else cout << "Unable to open file"; 

  cout << "N_test = " << *N_test << "\t N_train = " << *N_train << endl;

  myfile.close();
  myfile_logicaltest.close();

  file_test.close();
  file_train.close();
}
