#include "func.h"
#include "util.h"
#define NUMBER_THREADS 28

void func0(double *weights, double *arrayX, double *arrayY, int xr, int yr, int n)
{
	int i;
	double temp;
	temp = 1/((double)(n));
#pragma omp parallel for num_threads(NUMBER_THREADS) private(i) firstprivate(n, weights, temp, arrayX, arrayY, xr, yr)
	for(i = 0; i < n; i++){
		weights[i] = temp;
		arrayX[i] = xr;
		arrayY[i] = yr;
	}
}

void func1(int *seed, int *array, double *arrayX, double *arrayY,
			double *probability, double *objxy, int *index,
			int Ones, int iter, int X, int Y, int Z, int n)
{
	int i, j;
   	int index_X, index_Y;
	int max_size = X*Y*Z;
	int jx2;
	int index1, index2, value;
	double chances, dOne;
	dOne = (double)(Ones);

#pragma omp parallel for num_threads(NUMBER_THREADS) private(i) firstprivate(n, arrayX, seed, arrayY)
   	for(i = 0; i < n; i++){
   		arrayX[i] += 1 + 5*rand2(seed, i);
   		arrayY[i] += -2 + 2*rand2(seed, i);
   	}

#pragma omp parallel for num_threads(NUMBER_THREADS) private(i, j, jx2, index_X, index_Y, index1, index2, value, chances) firstprivate(n, Ones, arrayX, objxy, arrayY, index, Y, Z, iter, max_size, array)
   	for(i = 0; i<n; i++){
	  index1 = i * Ones;
   		for(j = 0; j < Ones; j++){
		  jx2 = j*2;
		  index2 = index1 + j;
   			index_X = round(arrayX[i]) + objxy[jx2 + 1];
   			index_Y = round(arrayY[i]) + objxy[jx2];
   			index[index2] = fabs(index_X*Y*Z + index_Y*Z + iter);
   			if(index[index2] >= max_size)
   				index[index2] = 0;
   		}
   		chances = 0;

   		for(j = 0; j < Ones; j++) {
		  value = array[index[index1 + j]];
   			chances += (pow((value - 100),2) -
   							  pow((value-228),2));
   		}
		chances = chances/50.0;
   		probability[i] = chances/dOne;
   	}
}

void func2(double *weights, double *probability, int n)
{
	int i;
	double sumWeights;
	double temp;
	sumWeights = 0;

#pragma omp parallel for num_threads(NUMBER_THREADS) private (i, temp) firstprivate(n, weights, probability) reduction(+:sumWeights)
	for(i = 0; i < n; i++){
   		temp = weights[i] * exp(probability[i]);
		weights[i] = temp;
   		sumWeights += weights[i];
	}
#pragma omp parallel for num_threads(NUMBER_THREADS) private(i) firstprivate(n, weights, sumWeights)
	for(i = 0; i < n; i++)
   		weights[i] = weights[i]/sumWeights;
}

void func3(double *arrayX, double *arrayY, double *weights, double *x_e, double *y_e, int n)
{
  double temp;
	double estimate_x=0.0;
	double estimate_y=0.0;
    int i;

#pragma omp parallel for num_threads(NUMBER_THREADS) private(i, temp) firstprivate(n, weights, arrayX, arrayY) reduction(+:estimate_x, estimate_y)
	for(i = 0; i < n; i++){
	  temp = weights[i];
	        estimate_x += arrayX[i] * temp;
   		estimate_y += arrayY[i] * temp;
   	}

	*x_e = estimate_x;
	*y_e = estimate_y;

}

void func4(double *u, double u1, int n)
{
	int i;
	double dn = (double)(n);
       
#pragma omp parallel for num_threads(NUMBER_THREADS) private(i) firstprivate(n, u, u1, dn)
	for(i = 0; i < n; i++){
   		u[i] = u1 + i/dn;
   	}
}

void func5(double *x_j, double *y_j, double *arrayX, double *arrayY, double *weights, double *cfd, double *u, int n)
{
	int i, j;
	int nMinus;
	double temp;
	nMinus = n - 1;
	temp = 1/((double)(n));
#pragma omp parallel for num_threads(NUMBER_THREADS) private(j, i) firstprivate(n, cfd, u, arrayX, arrayY, x_j, y_j, nMinus)
	for(j = 0; j < n; j++){
   		//i = findIndex(cfd, n, u[j]);
   		i = findIndexBin(cfd, 0, n, u[j]);
   		if(i == -1)
   			i = nMinus;
   		x_j[j] = arrayX[i];
   		y_j[j] = arrayY[i];

   	}

#pragma omp parallel for num_threads(NUMBER_THREADS) private(i) firstprivate(n, arrayX, x_j, arrayY, y_j, weights, temp)
	for(i = 0; i < n; i++){
		arrayX[i] = x_j[i];
		arrayY[i] = y_j[i];
		weights[i] = temp;
	}
}
