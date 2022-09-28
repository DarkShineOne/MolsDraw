#include"Camera.h"
#include "Renderer.h"

short int index = -1;

Camera::Camera(int width, int height, glm::vec3 position)
{
	GLCall(Camera::width = width);
	GLCall(Camera::height = height);
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	GLCall(glm::mat4 view = glm::mat4(1.0f));
	GLCall(glm::mat4 projection = glm::mat4(1.0f));
	GLCall(view = glm::lookAt(Position, Position + Orientation, Up));
	GLCall(projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, 10000.0f));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(shader.m_RendererID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view)));
}

void Camera::MatrixClassic(float FOV, float nearPlane, float farPlane, Shader& shader, const char* uniform, glm::vec3 viewVector)
{
	GLCall(glm::mat4 view = glm::mat4(1.0f));
	GLCall(glm::mat4 projection = glm::mat4(1.0f));
	GLCall(view = glm::lookAt(viewVector, OrientationClassic, Up));
//	GLCall(view = glm::lookAt(Position, Position + Orientation, Up));
	GLCall(projection = glm::perspective(glm::radians(FOV), (float)width / height, nearPlane, 10000.0f));
	GLCall(glUniformMatrix4fv(glGetUniformLocation(shader.m_RendererID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view)));

}



void Camera::Inputs(GLFWwindow* window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		//if (firstClick)
		//{
		//	glfwSetCursorPos(window, (width / 2), (height / 2));
		//	firstClick = false;
		//}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);
		
		float rotX = ((float)(mouseY)-width / 2) / (width/ 45.f);
		float rotY = ((float)(mouseX)-height / 2) / (height / 45.f);
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}

std::string Camera::GetVertexPosition(GLFWwindow* window, float* positions, int count)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		pressed = false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && pressed == false) {
		pressed = true;
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = ((float)(mouseY)-width / 2) / (width / 45.f);
		float rotY = ((float)(mouseX)-height / 2) / (height / 45.f);

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			newOrientation = glm::rotate(newOrientation, glm::radians(-rotY), Up);
		}
		glm::vec3 r1 = Position;
		glm::vec3 s1 = newOrientation;
		float distance;

		glm::vec3 check;
		if (s1[0] && s1[1] && s1[2] != 0.0f) {
			float t, x, y, z;
			float minimal = 1000000.0f;
			int indexOfMininal;
			for (int i = 0; i != count; i += 3)
			{
				t = (s1[0] * positions[i] + s1[1] * positions[i+1] + Orientation[2] * positions[i + 2] - (s1[0] * r1[0] + s1[1] * r1[1] + s1[2] * r1[2]))
					/ (pow(s1[0], 2) + pow(s1[1], 2) + pow(s1[2], 2));
				glm::vec3 ans = glm::vec3(s1[0] * t + r1[0], s1[1] * t + r1[1], s1[2] * t + r1[2]);
				distance = sqrt(pow(ans[0] - positions[i], 2) + pow(ans[1] - positions[i+1], 2) + pow(ans[2] - positions[i+2], 2));
				if (distance < minimal) { minimal = distance; indexOfMininal = i;x = positions[i]; 
				y = positions[i+1]; z = positions[i+2];
				}
			}

				index++;
				return  std::to_string((indexOfMininal / 3) + 1);
		}
	}
	return ("");
}



