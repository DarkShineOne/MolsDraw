#include "executor.h"

extern int count_indices;
extern int count_positions;

executor::executor() {


}
float* executor::DataToVertex(float *positions)
{
	std::ifstream f;
	f.open("atomslist.txt");
	int num;
	float x, y, z;
	std::cout << vertices_vector.size() << std::endl;
	while (f >> num >> x >> y >> z) {
		vertices_vector.push_back(x);
		vertices_vector.push_back(y);
		vertices_vector.push_back(z);
	}

	for (int i = 0; i != vertices_vector.size(); i++) {
		positions[i] = vertices_vector[i];
	}
	count_positions = vertices_vector.size();
	f.close();
	return positions;
}

glm::vec3 executor::GetTheCenter(float* positions)
{
	glm::vec3 xPointMin, xPointMax, yPointMin, yPointMax, zPointMin, zPointMax;

	float xMin = 500., xMax = -500., yMin = 500., yMax=-500., zMin=500., zMax=-500.;
	float xTemp, yTemp, zTemp;

	for (unsigned int i = 0; i != count_positions; i += 3) {
		xTemp = positions[i]; yTemp = positions[i + 1]; zTemp = positions[i + 2];
		//std::cout << xTemp << " " << yTemp << " " << zTemp << std::endl;
		if (xTemp < xMin && xTemp > -500) { xPointMin = glm::vec3(positions[i], positions[i + 1], positions[i + 2]); xMin = xTemp; }
		if (xTemp > xMax) { xPointMax = glm::vec3(positions[i], positions[i + 1], positions[i + 2]); xMax = xTemp; }
		if (yTemp < yMin && xTemp > -500){ yPointMin = glm::vec3(positions[i], positions[i + 1], positions[i + 2]); yMin = yTemp; }
		if (yTemp > yMax) { yPointMax = glm::vec3(positions[i], positions[i + 1], positions[i + 2]); yMax = yTemp; }
		if (zTemp < zMin && xTemp > -500) { zPointMin = glm::vec3(positions[i], positions[i + 1], positions[i + 2]); zMin = zTemp; }
		if (zTemp > zMax) { zPointMax = glm::vec3(positions[i], positions[i + 1], positions[i + 2]); zMax = zTemp; }
	}

	glm::vec3 yPointMid = (yPointMax + yPointMin) * 0.5f;
	glm::vec3 xPointMid = (xPointMax + xPointMin) * 0.5f;
	glm::vec3 zPointMid = (zPointMax + zPointMin) * 0.5f;

	glm::vec3 centerPoint = glm::vec3(xPointMid[0], yPointMid[1], zPointMid[2]);
	//std::cout << centerPoint[0] << " " << centerPoint[1] << " " << centerPoint[2] << std::endl;
	return centerPoint;
}

unsigned int* executor::DataToIndices(unsigned int*indices)
{

	std::ifstream file;
	file.open("atoms.txt");
	int temp0;
	int temp1;
	unsigned int b = 0;
	while (file >> temp0) {
		indices[b] = temp0-1;
		b += 1;
	}
	count_indices = b;
	//std::cout << b << std::endl;
	file.close();
	return indices;
}

