/*
 * defs.h
 */

#ifndef DEFS_H_
#define DEFS_H_

#include "ConfigDB.h"

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <fstream>
#include <sstream>

struct dualReads {
	std::string word1;
	std::string word2;
};

struct t_Chromosome {
	std::string header;
	std::string sequence;
};

// globals
#ifdef MAIN_CPP_
ConfigDB confDB;
char dataset[1000][500];
std::vector<t_Chromosome> genome;
int genLength;
std::mutex write_lock_mutex;

#else
extern ConfigDB confDB;
extern char dataset[1000][500];
extern std::vector<t_Chromosome> genome;
extern int genLength;
extern std::mutex write_lock_mutex;
#endif /* MAIN_CPP_ */

// prototypes
int commandLine(int argc, char *argv[]);
int startSimulation();
void createGenome();
int getNumReads();
dualReads getReads();
void threadReads(int s1, int s2, int s3, int s4);
int getQV(double position, double length);
void initialize(char *argv[]);
bool checkMutation(int score);
std::string getReverseComplement(std::string &str);

#endif /* DEFS_H_ */
