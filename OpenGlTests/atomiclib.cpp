#include "AtomicLib.h"


void done()
{
	cout << "Done." << endl;
}


void load_atoms() {
	bonds.erase(bonds.begin(), bonds.end());
	string temp_s;
	string fd;
	atom temp;
	vector <string> buff;
	int val;
	while (getline(opened_file, temp_s)) {// selection of atoms from the file to obtain and calculate bonds
		stringstream s{ temp_s };
		s >> fd;
		s >> val;
		if (fd == "ATOM" || fd == "HETATM") {
			while (s >> fd) {
				buff.push_back(fd);
			}
			reverse(buff.begin(), buff.end()); //the better way to getting atom coordination cause of their struction in file
			if (buff.empty()) cout << "EMPTY";
			temp.number = val;
			temp.z = stod(buff[3]);
			temp.y = stod(buff[4]);
			temp.x = stod(buff[5]);

			bonds.push_back(temp);
		}
		else if (fd == "TER") {
			atom fake;
			fake.number = val;
			fake.notAtom = true;
			bonds.push_back(fake);
		}
	}
	opened_file.close();
	ofstream file; //file of sorted atoms
	//string f = global_direct;
	//f.erase(f.end() - 4, f.end());
	//f += ".atom";
	string f = "atomslist.txt";
	file.open(f);
	for (const auto& i : bonds) { //printing atoms in file 
		file << i.number << " ";
		file << " " << i.x << " " << i.y << " " << i.z << endl;
	}
	file.close();

	return;
}

void copy_atoms() { //from .atom files

	string temp_s;
	string fd;
	atom temp;

	while (getline(opened_file, temp_s)) { // selection of atoms from the file to obtain and calculate bonds
		stringstream s{ temp_s };
		s >> temp.number;
		s >> temp.x;
		s >> temp.y;
		s >> temp.z;
		bonds.push_back(temp);
	}
}

void load(string filename) { 
	
	string iatom = filename;
	iatom.erase(iatom.begin(), iatom.end() - 4);
	string filedir = "ENT/";
	//string filedir = "ENT/"; unix os
	filedir += filename;
	const char* f = filedir.c_str();
	if (!(iatom == "atom")) {
		gzFile infileZ = gzopen(f, "rb");
		if (infileZ == nullptr) return;
		filedir.erase(filedir.end() - 3, filedir.end());
		unsigned char unzipBuffer[8000];
		unsigned int unzippedBytes;
		std::vector<unsigned char> unzippedData;
		while (true) {
			unzippedBytes = gzread(infileZ, unzipBuffer, 8000);
			if (unzippedBytes > 0) {
				unzippedData.insert(unzippedData.end(), unzipBuffer, unzipBuffer + unzippedBytes);
			}
			else {
				break;
			}
		}
		gzclose(infileZ);
		std::ofstream file;
		file.open(filedir);
		for (auto&& i : unzippedData) {
			file << i;
		}
	}
	global_direct = filedir;
	opened_file.open(filedir);
	if (!(iatom == "atom")) load_atoms();
	else copy_atoms();
	done();
	opened_file.close();
}


bool sort_atom(atom i, atom j) { // function for sort(); 
	return sqrt(pow(i.x, 2) + pow(i.y, 2) + pow(i.z, 2)) < sqrt(pow(j.x, 2) + pow(j.y, 2) + pow(j.z, 2));
}

double distance(atom atom1, atom atom2) { // distance through sqrt of coordination
	double res;
	res = sqrt(pow((atom2.x - atom1.x), 2) + pow((atom2.y - atom1.y), 2) + pow((atom2.z - atom1.z), 2));
	return res;
}

vector  <atom> calc_it(vector  <atom> bonds) { // loop for calculation bonds through their distance
	for (int i = 0; i != bonds.size() - 1; i++) {
		for (int j = i + 1; j != bonds.size(); j++) {
			if (distance(bonds[i], bonds[j]) <= 2. && bonds[i].notAtom == false && bonds[j].notAtom == false) {
				bonds[i].bond.push_back(bonds[j].number);
				bonds[j].bond.push_back(bonds[i].number);
			}
			if (bonds[j].sq - bonds[i].sq > 2) break;
		}
	}
	return bonds;
}

vector  <atom> powing(vector  <atom> bonds) { // debug function for checking of calc_bonds() func working
	for (auto& i : bonds) {
		i.sq = sqrt(pow(i.x, 2) + pow(i.y, 2) + pow(i.z, 2));
	}
	return bonds;
}

void calc_bonds() {

	if (bonds.empty()) { cout << "something got wrong..." << endl; return; }
	//sort(bonds.begin(), bonds.end(), sort_atom); //sorting atoms
	bonds = calc_it(bonds); //calc it!

	ofstream file; //file of sorted atoms
	file.open("atoms.txt");

	for (const auto& i : bonds) {
		for (const auto& j : i.bond) {
			file << i.number << " " << j << endl;
		}
	}
	file.close();
	done();
}

string bond(int num) { //showing bonds of atom that requested user (working after calc_bonds() function)
	string result;
	for (const auto& i : bonds) {
		if (i.number == num) {
			for (const auto& j : i.bond) {
				result += to_string(j) + " ";
			}
		}
	}
	return result;
}

float distances(int first, int second) { //showing distances between atoms (working after calc_bonds() function)
	atom temp1;
	atom temp2;
	for (const auto& i : bonds) {
		if (i.number == first) temp1 = i;
		if (i.number == second) temp2 = i;
	}

	if (temp1.number == -1 || temp2.number == -1) {
		cout << "There is no such atoms, try again." << endl;
		return 0;
	}
	double res = distance(temp1, temp2);
	return res;
}

float angle(int first, int second, int third) { //showing angle between atoms (working after calc_bonds() function)
	//This is a function of the bkv, all questions to him.
	atom temp1;
	atom temp2;
	atom temp3;
	for (const auto& i : bonds) {
		if (i.number == first) temp1 = i;
		if (i.number == second) temp2 = i;
		if (i.number == third) temp3 = i;
	}
	if (distance(temp1, temp2) > 2. || distance(temp2, temp3) > 2.)
	{
		cout << "Can't calculate for such atoms, try another." << endl;
		return -1;
	}
	double a[] = { temp1.x, temp1.y, temp1.z };
	double b[] = { temp2.x, temp2.y, temp2.z };
	double c[] = { temp3.x, temp3.y, temp3.z };

	double ab[3] = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
	double bc[3] = { c[0] - b[0], c[1] - b[1], c[2] - b[2] };

	double abVec = sqrt(ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2]);
	double bcVec = sqrt(bc[0] * bc[0] + bc[1] * bc[1] + bc[2] * bc[2]);

	double abNorm[3] = { ab[0] / abVec, ab[1] / abVec, ab[2] / abVec };
	double bcNorm[3] = { bc[0] / bcVec, bc[1] / bcVec, bc[2] / bcVec };

	double res = abNorm[0] * bcNorm[0] + abNorm[1] * bcNorm[1] + abNorm[2] * bcNorm[2];

	double result = acos(res) * 180.0 / M_PI;
	cout << result << endl;
	return result;
}

double* vector_mlt(double a[3], double b[3]) { //vector(vector) multiplication for torsion angle
	double* c = new double[3];
	//	a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
	return c;
}

double s_mlt(double a[3], double b[3]) { //vector(scalar) multiplication for torsion angle
	double result = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return result;
}

float torsion(int first, int second, int third, int four) { //torsion angle between atoms
	//I don't know if it works right, but just to check: https://en.wikipedia.org/wiki/Dihedral_angle
	//was taken arctan formula 
	atom temp1;
	atom temp2;
	atom temp3;
	atom temp4;
	for (const auto& i : bonds) {
		if (i.number == first) temp1 = i;
		if (i.number == second) temp2 = i;
		if (i.number == third) temp3 = i;
		if (i.number == four) temp4 = i;
	}
	if (distance(temp1, temp2) > 2. || distance(temp2, temp3) > 2. || distance(temp3, temp4) > 2.)
	{
		cout << "Can't calculate for such atoms, try another." << endl;
		return -1;
	}
	double p1[] = { temp1.x, temp1.y, temp1.z };
	double p2[] = { temp2.x, temp2.y, temp2.z };
	double p3[] = { temp3.x, temp3.y, temp3.z };
	double p4[] = { temp4.x, temp4.y, temp4.z };

	double a[3] = { p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2] };
	double b[3] = { p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2] };
	double c[3] = { p4[0] - p3[0], p4[1] - p3[1], p4[2] - p3[2] };

	double* multy_ab = vector_mlt(a, b);
	double* multy_bc = vector_mlt(b, c);
	double* multy_ab_bc = vector_mlt(multy_ab, multy_bc);
	double f = s_mlt(p2, multy_ab_bc);

	double s = s_mlt(multy_ab, multy_bc);
	double lengh_b = sqrt(pow(b[0], 2) + pow(b[1], 2) + pow(b[2], 2));
	s = s * lengh_b;
	double tors = atan2(f, s);
	//	cout << tors << " rad ";
	tors = tors * (180. / M_PI); // angle was calculated in radian form so that is the degrees form.
	return tors;
}

void del_file() {
	string s;
#ifdef _WIN32  //defining windows os
	s = "DEL " + global_direct;
	opened_file.close();
	system(s.c_str()); return;
#endif
	s = "rm " + global_direct;
	opened_file.close();
	system(s.c_str()); return;
}
