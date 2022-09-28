#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Camera.h"
#include "executor.h"
#include "interfaceM.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "atomiclib.h"

extern ifstream opened_file;
float positions[9000];
unsigned int indices[8000];
int count_positions;
int count_indices;
std::string atom;
bool tab = false;
bool pressedCaps;
bool fileloaded = false;
extern bool the_bool;
extern float Azimut;
extern float Polar;
extern float Radius;
const auto polarCap = 3.14159265359 / 2.0f - 0.01f;
float fakePolar;

int main(void)
{
	interfaceM inter;
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1400, 800, "LearningSomethingNew", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	glewInit();

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	while (!fileloaded) {
	again:
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplGlfwGL3_NewFrame();
		inter.gui(atom);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	{
		executor ex;
		fileloaded = false;
		float* p = ex.DataToVertex(positions);
		unsigned int* p1 = ex.DataToIndices(indices);

		VertexArray va;
		VertexBuffer vb(positions, sizeof(unsigned int) * count_positions);
		VertexBufferLayout layout;
		layout.Push(3);
		va.AddBuffer(vb,layout);
		GLCall(IndexBuffer ib(indices, count_indices)); // 6 is a count of indices
		glViewport(10, 10, 800, 800);
		Shader shader("basic.shader");

		shader.Bind();
		shader.SetUniform4f("u_Color", 1.0f, 0.3f, 0.8f, 1.0f);
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		Renderer renderer;

		glm::vec3 cameraPosition = ex.GetTheCenter(positions);
		glm::vec3 cameraCenter = cameraPosition;
		cameraPosition[2] += 40.f;

		Camera camera(800, 800, cameraPosition);
		camera.OrientationClassic = cameraCenter;

		glm::vec3 viewVector = cameraPosition;
		while (!glfwWindowShouldClose(window)) {

			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();

			shader.Bind();
			shader.SetUniform4f("u_Color", 0.8, 1.0f, 0.8f, 1.0f);

			if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_RELEASE) {
				pressedCaps = false;
			}
			if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS && pressedCaps == false) {
				pressedCaps = true;
				tab = (tab != true) ? true : false;
			}
			if (the_bool) {
				viewVector[0] =	cameraCenter[0] + (-Radius) * 2*cos(Polar) * cos(Azimut);
				viewVector[1] = cameraCenter[1] + (-Radius) * 2*sin(Polar);
				viewVector[2] = cameraCenter[2] + (-Radius) * 2*cos(Polar) * sin(Azimut);

				//camera.Orientation = glm::normalize(viewVector - cameraCenter);
				//cout << viewVector[0] << " " << viewVector[1] << " " << viewVector[2] << endl;
				GLCall(camera.MatrixClassic(45.f, 0.01f, 150.1f, shader, "camMatrix", viewVector));
			}
			else
				if (tab == true) {
					camera.Inputs(window);
					GLCall(camera.Matrix(45.f, 0.01f, 150.1f, shader, "camMatrix"));
					//cout << "default: " << camera.Position[0] <<" "<< camera.Position[1] << " " << camera.Position[2] << endl;
				}
			
			atom = camera.GetVertexPosition(window, positions, count_positions);

			renderer.Draw(va, ib, shader);
			inter.gui(atom);
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
			glfwPollEvents();
			if (fileloaded == true) {
				shader.Unbind();
				goto again;
			}
		}
		shader.Unbind();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}