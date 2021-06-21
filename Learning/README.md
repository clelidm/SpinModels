# Inference

Program that fits binary datasets with a chosen spin model with any high order interactions.


g++ -I /usr/local/include/eigen3/ -I ./LBFGSpp-master/include -O2 main.cpp Models.cpp ReadDataFile.cpp BoltzmannLearning_Ising.cpp BoltzmannLearning_ACE.cpp
