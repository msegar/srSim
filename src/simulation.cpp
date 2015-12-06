/*
 * simulation.cpp
 */

#include "defs.h"
#include "genome.h"
#include "gauss.h"

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

void call_from_thread(int tid, int tid2, int tid3) {
	//mtx.lock();
	cout << "Launched by thread " << tid << endl;
	//mtx.unlock();
}

int startSimulation(){
	srand (time(NULL));

	int numReads = 0;
	struct dualReads reads;
	string read1;
	string read2;
	int length;
	string dnaSeq;
	string qv;
	int QVscore;
	bool verbose = confDB.getKey("verbose").boolVal;
	bool mutate = confDB.getKey("mutation").boolVal;
	int defQV = confDB.getKey("qvScore").intVal;
	bool printQV = confDB.getKey("printQV").boolVal;
	string genomeName = confDB.getKey("genome").stringVal;
	bool reverseComplement = confDB.getKey("reverseComplement").boolVal;
	int nThreads = 1; // originally set nThreads to 1
	int percent = 0;
	
	nThreads = confDB.getKey("threads").intVal; // override with actual value
	
	if (verbose)
		confDB.printConfigDB();
	
	vector<thread> vThreads;
	
	ifstream input;
	//ofstream output;
	input.open(genomeName.c_str());
	t_Chromosome chromosome;
	
	// if the filename exists
	if (input){
		string line;
		getline(input, line);
		bool first = true;
		
		for (string row; getline(input, row);){
			if (row[0] == '>'){
				if (first){
					chromosome.header = row.substr(1);
					chromosome.sequence = "";
					first = false;
				} else {
					genome.push_back(chromosome);
					chromosome.header = row;
					chromosome.sequence = "";
				}
			} else if (row[0] == ' ')
				continue; 
			else {
				chromosome.sequence += row;
				genLength += row.length();
			}
		}
		genome.push_back(chromosome);
		cout << "Successfully imported genome length " << genLength << " with " << genome.size() << " chromosomes" << endl;
	}
	else {
		createGenome();
		genLength = genome[0].sequence.length();
	}
		
	//genLength = genome.length();
	numReads = getNumReads();
	
	//output.open("reads.fastq");
	
	//int step = numReads * 0.10;

	int start = 0;
	int end = 0;
	for (int i = 0; i < nThreads; ++i){
		start = (numReads / nThreads) * i;
		end = ((numReads / nThreads ) * (i+1)) - 1;
		//vThreads.push_back(thread(call_from_thread, start, end, numReads));
		vThreads.push_back(thread(threadReads, start, end, numReads, i));
		cout << "Starting thread " << i+1 << " from " << start << " to " << end << "..." << endl;
	}
	
	for (int i = 0; i < nThreads; ++i)
		vThreads[i].join();
	
	// After threads have finished, concatenate tmp reads into one file
	string command = "cat ";
/*	
	for (int i = 0; i < nThreads; ++i){
		command += "reads_";
		command += to_string(i);
		command += ".tmp ";
	}
	command += "> reads.fastq";
	
	system(command.c_str());
*/
	system("cat reads_*.tmp > reads.fastq");
	system("rm *.tmp");

	return 0;
}

void threadReads(int startReads, int endRead, int numReads, int threadNumber){
	struct dualReads reads;
	string read1;
	string read2;
	int length;
	string dnaSeq;
	string qv;
	int QVscore;
	bool mutate = confDB.getKey("mutation").boolVal;
	int defQV = confDB.getKey("qvScore").intVal;
	bool printQV = confDB.getKey("printQV").boolVal;
	string genomeName = confDB.getKey("genome").stringVal;
	bool reverseComplement = confDB.getKey("reverseComplement").boolVal;
	int nThreads = confDB.getKey("threads").intVal;
	int percent = 0;
	
	// create new tmp filename and open file
	string filename = "reads_";
	filename += to_string(threadNumber);
	filename += ".tmp";
	ofstream output;
	output.open(filename.c_str());

	for (int i = startReads; i < endRead; ++i){
		reads = getReads();
		read1 = reads.word1;
		length = read1.length();
		qv = "";

		for (int j = 0; j < length; ++j){
			if (mutate == false)
				qv += defQV;
			else {
				QVscore = getQV(j, length);
				if (checkMutation(QVscore)){
					// mutate based on purine/pyrimidine
					if (read1[j] == 'A')
						read1[j] = 'G';
					else if (read1[j] == 'G')
						read1[j] = 'A';
					else if (read1[j] == 'C')
						read1[j] = 'T';
					else if (read1[j] == 'T')
						read1[j] = 'C';

					// slash QVscore in 2
					QVscore = QVscore * 2 / 3;
				}
				qv += QVscore;
			}
		}
		
		if (reverseComplement && rand() % 2 == 0){ // if reverseComplement is true, on 50% of the time reverse it
			string temp = read1;
			read1 = getReverseComplement(temp);
			
			//write_lock_mutex.lock(); // lock mutex;
			if (printQV)
				output << "@Simulation[r]." << i+1 << "/" << numReads << " length=" << + reads.word1.length() << endl;
			else
				output << ">Simulation[r]." << i+1 /*<< "/" << numReads << " length=" << + reads.word1.length()*/ << endl;
			//write_lock_mutex.unlock(); // unlock mutex
		} else {
			//write_lock_mutex.lock(); // lock mutex;
			if (printQV)
				output << "@Simulation." << i+1 << "/" << numReads << " length=" << + reads.word1.length() << endl;
			else
				output << ">Simulation." << i+1 /*<< "/" << numReads << " length=" << + reads.word1.length()*/ << endl;
			//write_lock_mutex.unlock(); // unlock mutex
		}
		
		// print read
		//write_lock_mutex.lock(); // lock mutex;
		output << read1 << endl;
		
		if (printQV){
			output << "+ " << /*Simulation dna fragment data length=" << + length <<*/ endl;
			output << qv << endl;
		}
		//write_lock_mutex.unlock(); // unlock mutex
		
		qv = "";
		read1 = "";
	}

	output.close();

	return;
}

dualReads getReads(){
	struct dualReads reads;
	reads.word1 = "";
	reads.word2 = "";
	double mean = confDB.getKey("avgRead").doubleVal;
	double stdev = mean * 0.2;
	int length = gauss(mean, stdev);
	int randomChromosome = getRandomNumber(genome.size() - 1);
	int chrLength = genome[randomChromosome].sequence.length();
	int start = getRandomNumber(chrLength + (0.5 * length)) - length;
	int absoluteMin = mean - (2 * (0.2 * mean));
	
	if (start < 0)
		start = 0;
	else if (start+length >= chrLength)
		start = chrLength - length - 1;

	reads.word1 = genome[randomChromosome].sequence.substr(start, length);
	
	return reads;
}

int getNumReads(){
	int coverage = confDB.getKey("coverage").intVal;
	double mean = confDB.getKey("avgRead").doubleVal;
	int length = genLength;
	
	cout << "L=" << genLength << ", M=" << mean << ", C=" << coverage << ", R=" << (length/mean) * coverage << endl;

	return (length/mean) * coverage;
}

int getQV(double position, double length){
	int num;
	num = ((double) position/length) * 500;
	return dataset[getRandomNumber(1000)][num];
}

bool checkMutation(int score){
	string machine = confDB.getKey("machine").stringVal;
	int offset = 0;

	// get offset based on machine type
	if (machine.compare("454") == 0)
		offset = 33;
	else if (machine.compare("illumina") == 0)
		offset = 64;

	score -= offset;
	double prob = pow(10, ((-1 * score)/ 10));
	double randVal = rand() / double(RAND_MAX);

	if (randVal < prob)
		return true;
	else
		return false;
}


string getReverseComplement(string &str){
	string rev;

	for (unsigned int i = 0; i < str.length(); ++i){
		if (str[i] == 'A')
			rev = 'T' + rev;
		else if (str[i] == 'C')
			rev = 'G' + rev;
		else if (str[i] == 'G')
			rev = 'C' + rev;
		else if (str[i] == 'T')
			rev = 'A' + rev;
		else
			rev = 'N' + rev;
	}

	return rev;
}
