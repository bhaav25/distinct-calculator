// File: main.cpp
#include <cstdlib>
#include <iostream>
#include "MyFloat.h"

using namespace std;
int main(int argc, char** argv){

  float f1, f2;
  MyFloat mfres;
  cout.precision(1000);

  if(argc != 4){
    cout<< "Invalid number of args. Must be: (fpArithmetic) (float a) (+/-) (float b)"<< endl;
  } else{

    f1 = strtof(argv[1],NULL);
    f2 = strtof(argv[3],NULL);      
    MyFloat mf1(f1);
    MyFloat mf2(f2);

    cout << mf1 << ' '<< argv[2][0] << ' ' << mf2 << endl;

    // addition
    if(argv[2][0] == '+'){
      mfres = mf1.operator+(mf2);
      cout << "My Add: " << mfres << endl;
    
    // subtraction
    } else if(argv[2][0] == '-'){
        mfres = mf1 - mf2;
        cout << "My Subtraction: " << mfres << endl;

    } else{
        cout << "You did not enter + or -" << endl;
    }
  }
  return 0;
}
