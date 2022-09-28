#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <exception>
#include <sstream>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include "zlib.h"

using namespace std;
struct atom // atomic structure 
{
	int number{ 0 };
	double x{ -1000 };
	double y{ -1000 };
	double z{ -1000 };
	double sq{ 0 };
	vector <int> bond; // bond linker 
	bool notAtom = false;
};

void done();

extern ifstream opened_file; //file that opened now
static string global_direct; //direction that contain files
static string global_file_name; //name of using file 
extern vector <atom> bonds; //vector that contains all atoms 

void alist();
void load_atoms();
void copy_atoms();
void load(string filename);
bool sort_atom(atom i, atom j);
double distance(atom atom1, atom atom2);
vector  <atom> calc_it(vector  <atom> bonds);
vector  <atom> powing(vector  <atom> bonds);
void calc_bonds();
string bond(int num);
float distances(int first, int second);
float angle(int first, int second, int third);
double* vector_mlt(double a[3], double b[3]);
double s_mlt(double a[3], double b[3]);
float torsion(int first, int second, int third, int four);
void del_file();