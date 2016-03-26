#ifndef _UKF_LIBRARY
#define _UKF_LIBRARY

#include "application.h"

//based on generic UKF from https://github.com/mumblepins/ukf-spark-library/blob/master/firmware/ukf-spark-library.h

class UkfLib {
  typedef void (* FuncPtr) (float *, float *);
public:
  UkfLib();
  void ukf(
          FuncPtr f, float *x, float *P, FuncPtr hmeas, float *z, float *Q, float *R,
          int L, int m);
  void printArray(float* A, int m, int n, String label);

private:
  void unscentedTransform(
          FuncPtr f, float *X, float *Wc, float *R,
          int n, int L,
          float *y, float *Y, float *P, float *Y1);
  void sigmas(float *x, float *P, float c, int n, float *X);
  void cholesky(float* A, float* L, int n);
  void copy(float* A, int n, int m, float* B);
  void multiply(float* A, float* B, int m, int p, int n, float* C);
  void add(float* A, float* B, int m, int n, float* C);
  void subtract(float* A, float* B, int m, int n, float* C);
  void transpose(float* A, int m, int n, float* C);
  void scale(float* A, int m, int n, float k);
  int invert(float* A, int n);

  const float ALPHA = 0.1; //default, tunable
  const float KI = 0; //default, tunable
  const float BETA = 2; //default, tunable

};



#endif
