#include <iostream>
#include <math.h>
#include <stdio.h>

__global__ void add(int n, float *x, float *y) {
  int index = threadIdx.x;
  int stride = blockDim.x;
  for (int i = index; i < n; i += stride)
      y[i] = x[i] + y[i];
}

void FillWithData(int n, float* x, float* y) {
  for (int i = 0; i < n; i++) {
    x[i] = 1.0f;
    y[i] = 2.0f;
  }
} 

int main(void) {
  int N = 1<<20;
  float *x, *y;
  float *d_x, *d_y;
  int size = N * sizeof(float);

  x = (float*) malloc(size);
  y = (float*) malloc(size);
  FillWithData(N, x, y);

  cudaMalloc(&d_x, size);
  cudaMalloc(&d_y, size);
  cudaMemcpy(d_x, x, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_y, y, size, cudaMemcpyHostToDevice); 

  add<<<2, 256>>>(N, d_x, d_y);

  cudaMemcpy(x, d_x, size, cudaMemcpyDeviceToHost);
  cudaMemcpy(y, d_y, size, cudaMemcpyDeviceToHost); 

  int i = 0;
  int sample_rate = N / 10;
  for (i = 0; i < N; i=i+sample_rate) {
    printf("Value y[%d] = %f\n" , i, y[i]);
  } 

  // Free memory
  free(x); free(y);
  cudaFree(d_x); cudaFree(d_y);
  
  return 0;
}
