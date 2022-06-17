#include"lips.h"


Vertex vertices[] =
{
	//position  					//color						//textcoord				//normals
	glm::vec3(-0.5f, 0.5f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5, -0.5, 0.f),		glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5, 0.5, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(vertices);

GLuint indices[] =
{
	0, 1, 2,  // first triangle
	0, 2, 3  // first triangle
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateInput(GLFWwindow* window, Camera& camera)
{
	//exit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//Camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.move(FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.move(BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.move(LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.move(RIGHT);
	}
	/*if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->camPosition.y -= 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->camPosition.y += 0.05f;
	}*/
}

void  framebuffer_resize_callback(GLFWwindow* window, int fbw, int fbh) 
{
	glViewport(0, 0, fbw, fbh);
}

int main()
{
	//INIT GLFW

	glfwInit();

	//CREATE WINDOW

	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_WIDTH, "Open_GL", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window); //IMPORTAINT FOR GLUE

	//INIT GLEW

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw "error in glew init";
	}

	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//load shaders
	Shader core_program("vertex_core.glsl", "fragment_core.glsl");
	
	//load Mesh
	Mesh pyramid(
		new Pyramid(),
		glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f)
	);

	Mesh cube(
		new Cube(),
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f)
	);

	Mesh background(
		new Quad(),
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(5.f)
	);

	Mesh Start_btn(
		new Quad(),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f)
	);

	//create another Mesh
	Mesh test(vertices, nrOfVertices, indices, nrOfIndices, glm::vec3(0.f), glm::vec3(1.f), glm::vec3(1.f));

	//Load Textures
	Texture texture1("Images/image1.jpg", GL_TEXTURE_2D);
	Texture texture0("Images/sandbrick.jpg", GL_TEXTURE_2D);
	Texture texture2("Images/White.jpg", GL_TEXTURE_2D);

	//Create material
	//Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

	//move matrix
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);

	//create camera
	Camera camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f));

	//projectiong
	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 1000.f;
	glm::mat4 ProjectionMatrix(1.f);
	ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);

	core_program.setMat4fv(test.getViewMatrix(), "ModelMatrix");
	core_program.setMat4fv(camera.getViewMatrix(), "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	double mouseOffsetX;
	double mouseOffsetY;
	//Main Loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//glfwGetCursorPos(window, &mouseOffsetX, &mouseOffsetY);

		//move camera
		//camera.updateInput(-1, mouseOffsetX, mouseOffsetY);
		

		//update uniforms

		//core_program.set1f(1, "texture1");
		//core_program.set1f(1, "texture0");

		//update material
		//material0.sendToShader(core_program);

		//update window
		updateInput(window, camera);

		//core_program.setMat4fv(camera.getViewMatrix(), "ViewMatrix");
		//glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		//use program
		core_program.use();

		//activate texture
		texture1.bind();
		//pyramid.rotate(glm::vec3(0.f, 0.f, 0.f));
		Start_btn.render(&core_program);
		//render mesh
		//pyramid.render(&core_program);

		texture2.bind();
		background.render(&core_program);
		//cube.render(&core_program);
		//draw
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);


		//end
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//GLFWwindow* window = glfwCreateWindow();
	glfwDestroyWindow(window);
	glfwTerminate();


	return 0;
}