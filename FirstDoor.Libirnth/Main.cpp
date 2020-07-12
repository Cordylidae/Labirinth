#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include<vector>
#include<ctime>
#include<cmath>
#include<stack>
//#include"Windows.h"


//#define STB_IMAGE_IMPLEMENTATION
//#include"stb_image.h"

#include"ShaderClass.h"
#include"Camera.h"
#include"Model.h"

const int SizeMaze = 24;
char mazeChar[(SizeMaze + 1) * 2][(SizeMaze + 1) * 2];

class Player
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	glm::mat4 model = glm::mat4(1.0f);

	void Translate(glm::vec3 Vec)
	{
		model = glm::translate(model, Vec);

		x += Vec.x;
		y += Vec.y;
		z += Vec.z;
	}
	void Scale(glm::vec3 Vec)
	{
		model = glm::scale(model, Vec);
	}
};

Player player;


struct Cell
{
	int x;
	int y;

	char state;
};


void Maze();
void DeleteWall(int *x1, int *y1, int *x2, int *y2);
void GenerateMaze();



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera camera(glm::vec3(1.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float alfa = 0.0f;

glm::mat4 view = glm::mat4(1.0f);


const float M_PI = 3.14159265358979323846f;
const float speedSun = 2.0f;

glm::vec3 lightPos(3.5f, 10.0f, 3.5f);


int main()
{

	////////////MAZE/////////////////

	srand(time(0));

	Maze();

	///////////////////////

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Son of Satana", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	

	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);
	
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);



	Shader Deth("Deth.vs", "Deth.frs");
	Shader lightingShader("Block.vs", "Block.frs");
	Shader lampShader("Sun.vs", "Sun.frs");

	
	player.Translate(glm::vec3(9.0f, 0.0f, 9.0f));
	player.Scale(glm::vec3(2.0f, 2.0f, 2.0f));

	//camera.Position.x = player.x - 1.0f;
	//camera.Position.y = player.y + 0.5f;
	//camera.Position.z = player.z ;

	//Model ourModel("resources/Tree/EU43_1.obj");

	
	GLfloat vertices2[] = {
		// positions          // normals      // texture coords    //colors
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   1.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,   1.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   1.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,   1.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f
	};

	
	/*glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};*/

	GLuint VBO, cubeVAO;//, EBO[1];
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &cubeVAO);
	//glGenBuffers(1, EBO);

	

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	
		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
		glBindVertexArray(cubeVAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);
		
		unsigned int lightVAO;
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);

		// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		////////////////Textures///////////////////////////
		// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
		unsigned int diffuseMap = loadTexture("container2.png");
		unsigned int specularMap = loadTexture("container2_specular.png");


		// shader configuration
		// --------------------

		lightingShader.use();
		lightingShader.setInt("material.diffuse", 0);
		lightingShader.setInt("material.specular", 1);

		
	while (!glfwWindowShouldClose(window))
	{
		
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*alfa += deltaTime;

		if (alfa >= 1000 * deltaTime)
		{
			Maze();
			alfa = 0.0f;
		}*/

		processInput(window);

		//std::cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z<<endl;
		

		glClearColor(0.83f, 1.00f, 0.99f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		lightPos.x = (float) sin(currentFrame * M_PI / speedSun);
		lightPos.y = (float) cos(currentFrame * M_PI / speedSun);
		lightPos.z = (float)(sin(2 * currentFrame * 2 * M_PI / speedSun) * 0.5f + 0.5f);
		
		//Deth.use();
		lightingShader.use();
		lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
		//lightingShader.setVec3("light.position", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);

		lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//lightingShader.setFloat("light.constant", 1.0f);
		//lightingShader.setFloat("light.linear", 0.09f);
		//lightingShader.setFloat("light.quadratic", 0.032f);
		
		
		//lightingShader.setVec3("material.ambient", 0.24725f, 0.1995f, 0.0745f);
		//lightingShader.setVec3("material.diffuse", 0.75164f, 0.60648f, 0.22648f);
		//lightingShader.setVec3("material.specular", 0.628281f, 0.555802f, 0.366065f);
		
		lightingShader.setFloat("material.shininess", 0.4f * 80.0f);

		// activate shader
		//ourShader.use();
		//ourShader.setFloat("alfa", alfa);
		
		// pass projection matrix to shader (note that in this case it could change every frame)

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
		glm::mat4 view = camera.GetViewMatrix();
	

		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		//Deth.setMat4("projection", projection);
		//Deth.setMat4("view", view);

		//// render the loaded model
		//glm::mat4 model2 = glm::mat4(1.0f);
		//model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		

		
		/*lightingShader.setMat4("model", player.model);
		ourModel.Draw(lightingShader);*/
		

		// render box

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f)); // a smaller cube
		
		lightingShader.setMat4("model", model);
		//Deth.setMat4("model", model);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		int dist = 2;

		
		glBindVertexArray(cubeVAO);

		/*glm::mat4 model2 = glm::mat4(78.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.1f, 0.0f, 0.0f,
			0.0f, 0.0f, 78.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		model2 = glm::translate(model2, glm::vec3(0.5f, -30.0f, 0.5f));

		lightingShader.setMat4("model", model2);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

		//glBindVertexArray(cubeVAO);


		for (int x = 0; x < (SizeMaze + 1) * 2; x++)
		{
			for (int y = 0; y < (SizeMaze + 1) * 2; y++)
			{
				if (mazeChar[x][y] == '#')
				{
					glm::mat4 model = glm::mat4(10.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 50.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 10.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
					model = glm::translate(model, glm::vec3(x, 0.0f, y));
					lightingShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				if (mazeChar[x][y] == ' ')
				{
					glm::mat4 model = glm::mat4(10.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 10.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
					model = glm::translate(model, glm::vec3(x, -50.0f, y));
					lightingShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}		
			
			}
		}

		//for (float i = 0; i < 27; i+=1)
		//{
		//	
		//	// calculate the model matrix for each object and pass it to shader before drawing
		//	glm::mat4 model = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f, 
		//								0.0f, 4.0f, 0.0f, 0.0f, 
		//								0.0f, 0.0f, 2.0f, 0.0f, 
		//								0.0f, 0.0f, 0.0f, 1.0f);
		//	model = glm::translate(model, glm::vec3(float(int(i) % 3) * dist, float((int(i) / 9)) * dist  , float(int(i) % 9 / 3) * dist));
		//	float angle = 20.0f * i;
		//	//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//	
		//	lightingShader.setMat4("model", model);
		//	//Deth.setMat4("model", model);

		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);

		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();


	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		alfa += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (alfa >= 1.0f)
			alfa = 1.0f;
	}
	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		alfa -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (alfa <= 0.0f)
			alfa = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//player.Translate(4 * deltaTime * glm::vec3(1.0f,0.0f,0.0f));
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//player.Translate(-4 * deltaTime * glm::vec3(1.0f, 0.0f, 0.0f));
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//player.Translate(-4 * deltaTime * glm::vec3(0.0f, 0.0f, 1.0f));
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//player.Translate(4 * deltaTime * glm::vec3(0.0f, 0.0f, 1.0f));
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Maze()
{

	Cell maze[SizeMaze][SizeMaze];

	for (int x = 0; x < SizeMaze; x++)
	{
		for (int y = 0; y < SizeMaze; y++)
		{

			maze[x][y].x = x;
			maze[x][y].y = y;
			maze[x][y].state = 0;


			if (x >= 0 && x < 3 && y >= 0 && y < 3) { maze[x][y].state = 2; }
			if (x >= 3 && x < 5 && y >= 8 && y < 10) { maze[x][y].state = 3; }
			if (x >= 5 && x < 7 && y >= 3 && y < 5) { maze[x][y].state = 4; }
			if (x >= 9 && x < 12 && y >= 3 && y < 6) { maze[x][y].state = 5; }

		}
	}


	char State = 97;

	GenerateMaze();

	maze[rand() % 2 + 3][rand() % 2 + 8].state = 0;
	maze[rand() % 2 + 5][rand() % 2 + 3].state = 0;

	if (rand() % 2) {
		maze[2][rand() % 2].state = 0;
	}
	else maze[rand() % 2][2].state = 0;

	//maze[rand() % 2 + 8][rand() % 2 + 3].state = 0;
	maze[0][3].state = State;

	stack<Cell> mazeVisit;
	mazeVisit.push(maze[0][3]);



	while (!mazeVisit.empty())
	{
		vector<Cell> unvisitedNeighbours;

		int x = mazeVisit.top().x;
		int y = mazeVisit.top().y;

		if ((x > 0) && (!maze[x - 1][y].state)) unvisitedNeighbours.push_back(maze[x - 1][y]);
		if ((y > 0) && (!maze[x][y - 1].state)) unvisitedNeighbours.push_back(maze[x][y - 1]);
		if ((x < SizeMaze - 1) && (!maze[x + 1][y].state)) unvisitedNeighbours.push_back(maze[x + 1][y]);
		if ((y < SizeMaze - 1) && (!maze[x][y + 1].state)) unvisitedNeighbours.push_back(maze[x][y + 1]);

		if (unvisitedNeighbours.size() > 0)
		{
			Cell Chosen = unvisitedNeighbours[rand() % (unvisitedNeighbours.size())];
			Chosen.state = mazeVisit.top().state;
			maze[Chosen.x][Chosen.y].state = Chosen.state;
			DeleteWall(&mazeVisit.top().x, &mazeVisit.top().y, &Chosen.x, &Chosen.y);
			mazeVisit.push(Chosen);
		}
		else
		{
			mazeVisit.pop();
			if (mazeVisit.empty())break;
			State++;
			mazeVisit.top().state = State;
		}

	}

}

void GenerateMaze()
{
	for (int x = 0; x < (SizeMaze + 1) * 2; x++)
	{
		for (int y = 0; y < (SizeMaze + 1) * 2; y++)
		{
			mazeChar[x][y] = '#';

			if ((x % 2 == 1 && y % 2 == 1) || (x == SizeMaze * 2 + 1) || (y == SizeMaze * 2 + 1))
			{

				mazeChar[x][y] = ' ';
			}

			{
				if (x >= 1 && x < 6 && y >= 1 && y < 6) { mazeChar[x][y] = ' '; }
				if (x >= 7 && x < 10 && y >= 17 && y < 20) { mazeChar[x][y] = ' '; }
				if (x >= 11 && x < 14 && y >= 7 && y < 10) { mazeChar[x][y] = ' '; }
				if (x >= 19 && x < 24 && y >= 7 && y < 12) { mazeChar[x][y] = ' '; }
			}
		}
	}
}

void DeleteWall(int *x1, int *y1, int *x2, int *y2)
{
	int x, y;

	if ((*x1) == (*x2))
	{
		x = (*x1) * 2 + 1;
		y = min((*y1), (*y2)) * 2 + 2;
	}
	if ((*y1) == (*y2))
	{
		x = min((*x1), (*x2)) * 2 + 2;
		y = (*y1) * 2 + 1;
	}

	mazeChar[x][y] = ' ';
}
