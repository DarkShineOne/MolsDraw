#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class executor
{
private:
	std::vector <float> vertices_vector;

public:
	executor();
	float* DataToVertex(float* positions);
	unsigned int* DataToIndices(unsigned int* indices);
	glm::vec3 GetTheCenter(float* positions);
};

