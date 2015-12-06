/*
 * gauss.cpp
 *
 * This code is licensed under free-use from:
 * http://cboard.cprogramming.com
 */

#include "gauss.h"

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>

using namespace std;

#define DOUBDIGS 20 // number of decimal places to calculate a rand double to
#define EPS_Q 1.19E-07

/*
 * This function takes two parameters - the mean and the standard deviation.
 * This will return a random variable which will follow the normal distribution
 * with the corresponding mean and standard deviation.
 */
int gauss(double mean, double stdev){

	//get a standard normal random variable,
	double d = invcgauss(randDouble());
	//cout << "\t" << randDouble() << endl;
	//multiply by the standard deviation,
	d *= stdev;
	//and add the mean
	d += mean;
	//cout << d << endl;
	return d;
}


double randDouble(){
	char value[DOUBDIGS + 3]= "0.";
	for (int i = 2; i < (DOUBDIGS + 2); i++){
		value[i] = (char)(rand() % 10 + '0');
	}
	value[DOUBDIGS + 2] = 0;
	return atof(value);
}


/* Inverse cumulative Gaussian transform.
 * This routine takes a proportion p and returns the corresponding
 * x-value of a cumulative Gaussian of zero mean and unit standard deviation.
 * Odeh, R. E. & Evans, J. O. (1974) Algorithm AS 70: Percentage points
 * of the normal distribution. Applied Statistics, 23, 96-97. Described in
 * Kennedy, W. J. & Gentle, J. E. (1980) Statistical Computing, New York:
 * Dekker, pp. 95-96.
 *
 * This function is not copyrighted
 */
double invcgauss(double p){
	double lim=EPS_Q;
	double p0=-0.322232431088, p1=-1.0, p2=-0.342242088547;
	double p3=-0.0204231210245, p4=-0.453642210148E-4;
	double q0=0.0993484626060, q1=0.588581570495;
	double q2=0.531103462366, q3=0.103537752850, q4=0.38560700634E-2;
	double xp, y, ptrue;

	ptrue = p;                       /* store value */
	if (p > 0.5) p = 1.0 - p;
	if (p < lim) p = lim;            /* minimum value of p */

	if (p == 0.5) return(0.0);
	y = sqrt(log(1.0/(p*p)));
	xp = y+((((y*p4+p3)*y+p2)*y+p1)*y+p0)/((((y*q4+q3)*y+q2)*y+q1)*y+q0);

	return(ptrue < 0.5 ? -xp : xp);
}


