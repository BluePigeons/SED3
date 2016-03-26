#include "ukfbatt.h"


// Based on example code at https://github.com/mumblepins/ukf-spark-library/blob/master/firmware/examples/simple-ukf-example.cpp

#define n  5 //number of state variables (SOC_k-1 , u_c , w_s , w_c , and v)
#define pts  11 // ???
int m = 1; // number of measurement variables (current?))


//float To = 4; // outside temp
//float Ua = 4; // Insulation value
//float Mt = 700; // thermal mass value
//float u = 75; // heater percent on)

float SOC_k-1 = 
float i_k = 
float 

float max_time = 5; //simulation time (minutes)
float t[pts]; // time points
float r = 0.1; // std dev of measurement

float Q[] = {40, 0, 0,
  0, 40, 0,
  0, 0, 40}; //covariance of process


float R[] = {r * r}; //covariance of measurement
float x[] = {15, 2, 200}; // current/inital state (starting ata point off of real values to display convergence))
float P[] = {50, 0, 0,
  0, 50, 0,
  0, 0, 50}; // current/initial state covariance

float xV[n*pts];
float sV[n*pts];
float zV[1 * pts];

float T[pts];

UkfLib ukf;

void stateFunc(float *in, float *out)
{
  float dt = (max_time / (float) (pts - 1));
  out[0] = in[0] + (dt * (u + in[1]*(To - in[0]))) / in[2];
  out[1] = in[1];
  out[2] = in[2];
}

void measFunc(float *in, float *out)
{
  out[0] = in[0];
}

void setup()
{
  Serial.begin(9600);
  while (!Serial.available()); //Spark.process(); - ???
  float delta_t = (max_time / (float) (pts - 1)); // delta_t time
  T[0] = 15; //Starting SOC

  // Calculate actual SOC
  for (int i = 1; i < pts; i++) {
    T[i] = T[i - 1] + delta_t * (u - Ua * (T[i - 1] - To)) / Mt;
  }
  for (int i = 0; i < pts; i++) {
    Serial.print(T[i]);
  }
  Serial.println();

  for (int k = 0; k < pts; k++) {
    float z[] = {T[k]}; 
    ukf.ukf(stateFunc, x, P, measFunc, z, Q, R, n, m);
    for (int i = 0; i < n; i++) {
      xV[i * pts + k] = x[i];
    }
  }
  ukf.printArray(xV, 3, pts, "xv");

}

void loop()
{

}