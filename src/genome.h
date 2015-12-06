/*
 * genome.h
 *
 *  Created on: Feb 3, 2012
 *      Author: mws022
 */

#ifndef GENOME_H_
#define GENOME_H_

void initializeArrays(int AT[], int CG[], int ATtoCG[], int CGtoAT[]);
char getNucleotide(int val);
int getRandomNumber(int highest);

#endif /* GENOME_H_ */
