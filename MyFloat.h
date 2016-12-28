// File: MyFloat.h
#ifndef MY_FLOAT_H
#define MY_FLOAT_H
  
#include <iostream> 
using namespace std;
  
class MyFloat{
  public:
    // prototypes
    MyFloat();
    MyFloat(float f);
    MyFloat(const MyFloat & rhs);
    MyFloat operator+(const MyFloat& rhs) const;
    MyFloat operator-(const MyFloat& rhs) const;
    virtual ~MyFloat() {};
    friend ostream& operator<<(std::ostream& strm, const MyFloat& f);
    
  private:
    // variables
    unsigned int sign;
    unsigned int exponent;
    unsigned int mantissa;  

    // prototypes
    void unpackFloat(float f);
    float packFloat() const;
    static bool carryWouldHappen(unsigned int a, unsigned int b);
};
    
#endif
