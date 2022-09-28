#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * vec4(aPos, 1.0);
	gl_PointSize = 5.0f;
	//gl_Position = vec4(aPos, 1.0);
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
	FragColor = u_Color;
}