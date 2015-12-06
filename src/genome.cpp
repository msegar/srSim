/*
 * genome.cpp
 */

#include "defs.h"
#include "genome.h"

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>
#include <fstream>

using namespace std;

#define DOUBDIGS 20 // number of decimal places to calculate a rand double to
#define EPS_Q 1.19E-07

void createGenome(){
	srand((unsigned)time(NULL));
	ofstream output;
	output.open("genome.fasta");

	int length = confDB.getKey("genomeLength").intVal;
	bool state = (rand()%100) >= 50 ? true : false; // true = AT, false = CG
	int num = 0;
	string genomeSeq = "";
	
	cout << "Creating genome of length " << length << " with 1 chromosome..." << endl;
	
	t_Chromosome chromosome;

	// Hidden Markov Model arrays
	int AT[100] = {0}; // AT emission
	int CG[100] = {0}; // CG emission
	int ATtoCG[100] = {0};
	int CGtoAT[100] = {0};
	initializeArrays(AT, CG, ATtoCG, CGtoAT);

	for (int i = 0; i < length; ++i){
		if (state == true){
			num = getRandomNumber(100);
			genomeSeq += getNucleotide(AT[num]);

			num = getRandomNumber(100);
			state = ATtoCG[num] == 1 ? false : true;
		}
		else {
			num = getRandomNumber(100);
			genomeSeq += getNucleotide(CG[num]);

			num = getRandomNumber(100);
			state = CGtoAT[num] == 1 ? true : false;
		}
	}
	
	chromosome.header = "Simulation genome";
	chromosome.sequence = genomeSeq;
	genome.push_back(chromosome);

	output << ">Simulation genome length=" << genomeSeq.length() << endl;
	for (int i = 0; i < genomeSeq.length(); ++i){
		output << genomeSeq.substr(i, 80) << endl;
		i += 79;
	}
}

void initializeArrays(int AT[], int CG[], int ATtoCG[], int CGtoAT[]){
	// initialize AT
	for (int i = 0; i < 39; ++i)
		AT[i] = 1; // A
	for (int i = 39; i < 49; ++i)
		AT[i] = 2; // C
	for (int i = 49; i < 59; ++i)
		AT[i] = 3; // G
	for (int i = 59; i < 100; ++i)
		AT[i] = 4; // T

	// initialize CG
	for (int i = 0; i < 10; ++i)
		CG[i] = 1; // A
	for (int i = 10; i < 52; ++i)
		CG[i] = 2; // C
	for (int i = 52; i < 92; ++i)
		CG[i] = 3; // G
	for (int i = 90; i < 100; ++i)
		CG[i] = 4; // T

	// initialize ATtoCG
	for (int i = 0; i < 20; ++i)
		ATtoCG[i] = 1;

	// initialize CGtoAT
	for (int i = 0; i < 15; ++i)
		CGtoAT[i] = 1;
}

char getNucleotide(int val){
	if (val == 1)
		return 'A';
	else if (val == 2)
		return 'C';
	else if (val == 3)
		return 'G';
	else if (val == 4)
		return 'T';
	else
		return 'N';
}

int getRandomNumber(int highest){
	int randomInteger = 0;

	randomInteger = (int)(rand() / (((double)RAND_MAX + 1) / highest));

	return randomInteger;
}
