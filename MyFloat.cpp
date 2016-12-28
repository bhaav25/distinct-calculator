// File: MyFloat.cpp
#include "MyFloat.h"

MyFloat::MyFloat(){
  sign = 0;
  exponent = 0;
  mantissa = 0;
}

MyFloat::MyFloat(float f){
  unpackFloat(f);
}

MyFloat::MyFloat(const MyFloat & rhs){
  sign = rhs.sign;
  exponent = rhs.exponent;
  mantissa = rhs.mantissa;
}

ostream& operator<<(std::ostream &strm, const MyFloat &f){
  strm << f.packFloat();
  return strm;
}

// handles addition of two floats
MyFloat MyFloat::operator+(const MyFloat& rhs) const{

    MyFloat *answer = new MyFloat;

    // store exponent, mantissa, sign 
    unsigned int e1 = this -> exponent;
    unsigned int e2 = rhs.exponent;
    unsigned int m1 = this -> mantissa;
    unsigned int m2 = rhs.mantissa;
    unsigned int s1 = this -> sign;
    unsigned int s2 = rhs.sign;

    unsigned int e1_e2; // e1 - e2
    unsigned int e2_e1; // e2 - e1

    unsigned int sum; // answer's mantissa
    unsigned int exp; // answer's exponent

    
    // LHS is 0
    if (e1 == 0 && m1 == 0){
      answer -> sign = (e2 == 0 && m2 == 0) ? 0 : s2;
      answer -> exponent = e2;
      answer -> mantissa = m2;
    // RHS is 0
    } else if (e2 == 0 && m2 == 0){
      answer -> sign = s1;
      answer -> exponent = e1;
      answer -> mantissa = m1;  
    // if LHS or RHS not 0
    } else {

      unsigned int one = 1 << 23;
      m1 += one;
      m2 += one;
  
      if (e1 > e2){  
        e1_e2 = e1 - e2;
        if (e1_e2 <= 8){
          m1 <<= e1_e2;
          exp = e2;
        } else {
          m2 >>= e1_e2;
          exp = e1;
        }
      }
      
      if (e2 > e1){
        e2_e1 = e2 - e1;
        if (e2_e1 <= 8){
          m2 <<= e2_e1;
          exp = e1;
        } else {
          m1 >>= e2_e1;
          exp = e2;
        }
      }

      if (e1 == e2) exp = e1;
  
      // both floats are positive or negative
      if (((s1 == 0) && (s2 == 0)) || ((s1 == 1) && (s2 == 1))){

        // add mantissa
        sum = m1 + m2;
        
        // carry only happens if
        // exponent difference is 8
        if (carryWouldHappen(m1, m2)){
          // correcting mantissa
          sum >>= 1;
          exp += 1;
          sum += (one << 8);
          sum >>= 8;
          exp += 8;
          // delete leading 1
          sum ^= one;
        } else {

          if (e1 > e2 && e1_e2 < 8){
            // "carry (without overflow)"
            if ( ((sum >> (24 + e1_e2) ) & (one >> 23)) == 1){
              // correct mantissa
              sum >>= e1_e2 + 1;
              exp += e1_e2 + 1;
              // delete leading 1
              sum ^= one;
            // no "carry (without overflow)"
            } else {  
              // correct mantissa
              sum >>= e1_e2;
              exp += e1_e2;
              // delete leading 1
              sum ^= one;
            }
          }

          if (e1 > e2 && e1_e2 > 8){
            // "carry (without overflow)"
            if ( ((sum >> 24) & (one >> 23)) == 1 ){
              // correct mantissa
              sum >>= 1;
              exp += 1;
              // delete leading 1
              sum ^= one;
            // no "carry (without overflow)"
            } else {
              // delete leading 1
              sum ^= one;
            }
          }

          if (e2 > e1 && e2_e1 < 8){
            if ( ((sum >> (24 + e2_e1) ) & (one >> 23)) == 1){
              sum >>= e2_e1 + 1;
              exp += e2_e1 + 1;
              sum ^= one;
            } else {
              sum >>= e2_e1;
              exp += e2_e1;
              sum ^= one;
            } 
          }

          if (e2 > e1 && e2_e1 > 8){
            if ( ((sum >> 24) & (one >> 23)) == 1 ){
              sum >>= 1;
              exp += 1;
              sum ^= one;
            } else {
              sum ^= one;
            }
          }

          // if exponents are same 
          if (e1 == e2){
            // correct mantissa
            sum >>= 1;
            exp += 1;
            // delete leading 1
            sum ^= one;
          }
        }

        // set answer's sign,
        // exponent, mantissa
        if (s1 == 0)
          answer -> sign = 0;
        else
          answer -> sign = 1;
        answer -> exponent = exp;
        answer -> mantissa = sum;

      // different signs
      } else {

        if (m1 > m2){

          // assign answer's sign
          if (s1 == 0)
            answer -> sign = 0;
          else
            answer -> sign = 1;

          // subtract mantissa          
          sum = m1 - m2;

          // exponents are different
          if (e1 != e2){

            if (e1_e2 <= 8){
              // MSB is 1 after subtraction
              if ( ((sum >> (24 + e1_e2 - 1) ) & (one >> 23)) == 1){
                // correct mantissa
                sum >>= e1_e2;
                exp += e1_e2;
                // delete leading 1
                sum ^= one;
              // MSB is 0
              } else {
                // handles case where e1 - e2 = 1, ie. 4 - 3.875
                if(e1_e2 == 1){
                  while( ((sum >> (24 + e1_e2 - 1) ) & (one >> 23)) != 1){
                    // find leading 1
                    sum <<= 1;
                    exp -= 1;
                  }
                } else {
                  // find leading 1
                  sum <<= 1;
                  exp -= 1;
                }
                // correct mantissa
                sum >>= e1_e2;
                exp += e1_e2;
                // delete leading 1
                sum ^= one;
              }  
            }

            if(e1_e2 > 8){
              // MSB is 1 after subtraction
              if ( ((sum >> 23) & (one >> 23)) == 1 ){
                // delete leading 1
                sum ^= one;
              // MSB is 0
              } else {  
                if(e1_e2 == 1){
                  while( ((sum >> 23) & (one >> 23)) != 1){
                    // find leading 1
                    sum <<= 1;
                    exp -= 1;
                  }
                } else {
                  // find leading 1
                  sum <<= 1;
                  exp -= 1;
                }
                // delete leading 1
                sum ^= one;
              }
            }

          // exponents are same
          } else {

            // find leading 1
            while (sum < one){
              sum <<= 1;
              exp -= 1;
            }
            // delete leading 1
            sum ^= one;
          }

          // assign answer's exponent
          // and mantissa
          answer -> exponent = exp;
          answer -> mantissa = sum;
        }

         if (m1 < m2){

          if (s1 == 0)
            answer -> sign = 1;
          else
            answer -> sign = 0;

          sum = m2 - m1;

          if (e1 != e2){

            if (e2_e1 <= 8){
              if ( ((sum >> (24 + e2_e1 - 1) ) & (one >> 23)) == 1){
                sum >>= e2_e1;
                exp += e2_e1;
                sum ^= one;
              } else {
                if(e2_e1 == 1){
                  while( ((sum >> (24 + e2_e1 - 1 ) ) & (one >> 23)) != 1){
                    sum <<= 1;
                    exp -= 1;
                  }
                } else {
                  sum <<= 1;
                  exp -= 1;
                }
                sum >>= e2_e1;
                exp += e2_e1;
                sum ^= one;
              }  
            } 

            if(e2_e1 > 8){ 
               if ( ((sum >> 23) & (one >> 23)) == 1 ){
                sum ^= one;
              } else {
                if(e2_e1 == 1){
                  while( ((sum >> 23) & (one >> 23)) != 1){
                    sum <<= 1;
                    exp -= 1;
                  }
                } else {
                  sum <<= 1;
                  exp -= 1;
                }
                sum ^= one;
              }
            }

          } else {

            while (sum < one){
              sum <<= 1;
              exp -= 1;
            }
            sum ^= one;
          }
          answer -> exponent = exp;
          answer -> mantissa = sum;   
        }

        // same sign and mantissa
        if (m1 == m2){
          answer -> sign = 0;
          answer -> exponent = 0;
          answer -> mantissa = 0;
        }
      }
    }
    return *answer;
}

// handles subtraction of two floats
MyFloat MyFloat::operator-(const MyFloat& rhs) const{
  
  // subtraction of x - RHS is 
  // same as x + (negation of RHS)
  MyFloat *negation = new MyFloat;

  if (rhs.sign == 0)
      negation -> sign = 1;
  else
      negation -> sign = 0;

  negation -> exponent = rhs.exponent;
  negation -> mantissa = rhs.mantissa; 
  return *this + negation -> packFloat();
}

void MyFloat::unpackFloat(float f) {

  __asm__(
    "movl %[f], %%ecx;"
    "andl $0x7fffff, %%ecx;"
    "movl %%ecx, %[mant];"
    "movl %[f], %%ecx;"
    "andl $0x7f800000, %%ecx;"
    "shrl $23, %%ecx;"
    "movl %%ecx, %[exp];"
    "shrl $31, %[f];"
    "movl %[f], %[sign]" :

    [sign] "=r"  (sign), [exp] "=r" (exponent), [mant] "=r" (mantissa):
    [f] "r" (f):
    "cc"
  );
}

float MyFloat::packFloat() const{

  float f = 0;

  __asm__(
    "shll $31, %[si];"
    "shll $23, %[exp1];"
    "addl %[mant1], %[si];"
    "addl %[si], %[exp1];"
    "movl %[exp1], %[output]" :

    [output] "=r" (f):
    [si] "r" (sign), [exp1] "r" (exponent), [mant1] "r" (mantissa):
    "cc"
  );
  return f;
}

bool MyFloat::carryWouldHappen(unsigned int m1, unsigned int m2){
  
  bool carry = false;

  __asm__(
    "addl %[a1], %[b1];"
    "jc carry;"
    "movb $0, %[isTrue];"
    "jmp done;"
    "carry:;"
    "movb $1, %[isTrue];"
    "done:":

    [isTrue] "=r" (carry):
    [a1] "r" (m1), [b1] "r" (m2):
    "cc"
  );
  return carry;
}
