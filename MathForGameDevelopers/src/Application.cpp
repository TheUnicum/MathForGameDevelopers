#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "opengl/Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

// include tests
#include "tests/TestClearColor.h"

#include "tests/01_FirstPrototipe/T01_FirstPrototipe_01.h"

#include "mfgd/mfgd_Classes.h"

// Functions Declaration
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

float MAX_DELTA_TIME = 0.5f;
int framebufferWidth = 0, framebufferHeight = 0;

float deltaTime = 0.0f;
float lastframe = 0.0f;

test::Test* currentTest = nullptr;

int main(int argc, char** args)
{
	Test_MFGD();


	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //GLFW_OPENGL_COMPAT_PROFILE
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Anti Aliasing Hint
	// To Enable MultiSample Anti Aliasing uncomment this line before create a Window!
	//glfwWindowHint(GLFW_SAMPLES, 4);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Added this lines to get framebuffer width/height and set Viewport
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	framebuffer_size_callback(window, framebufferWidth, framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Mouse callback register fuctions
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);	// active or deactive syncronize frame rate to 60Hz

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	// GL info :
	std::cout << glGetString(GL_VERSION) << std::endl;
	int data;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &data);
	std::cout << "MAX Vertex Uniform components: " << data << std::endl;

	{	// make a scope to force delete a VertexBuffer before context windows get distroyed
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		//test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;


		//-----------------------------
		// 01_Getting_started
		//-----------------------------

		test::TestMenu* TM_01_Getting_started = new test::TestMenu(currentTest, "Getting started");
		TM_01_Getting_started->RegisterTest<test::TestClearColor>("Clear Color test");

		test::TestMenu* TM_01_FirstPrototipe = new test::TestMenu(currentTest, "01 First Prototipe");
		TM_01_Getting_started->RegisterTest<test::T01_FirstPrototipe_01>("First Prototipe");


		//-----------------------------
		//-----------------------------
		// Main Menu
		//-----------------------------
		//-----------------------------
		testMenu->RegisterMenu(*TM_01_Getting_started);


		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = (float)glfwGetTime();
			deltaTime = currentFrame - lastframe;
			lastframe = currentFrame;

			if (deltaTime > MAX_DELTA_TIME)
				deltaTime = MAX_DELTA_TIME;

			// Main Input------
			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();


			if (currentTest)
			{
				currentTest->OnProcessInput(window, deltaTime);
				currentTest->OnUpdate(deltaTime);// 0.0f);
				currentTest->OnRender();
				currentTest->OnRender(window);
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					if (dynamic_cast<test::TestMenu*>(currentTest) == nullptr)
					{
						delete currentTest;
						currentTest = testMenu;
					}
					else
						currentTest = dynamic_cast<test::TestMenu*>(currentTest)->getParentTest();
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}




			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;

}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Callback funcitons definitions
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimension; notes that width and
	// height will be significantly larger than specifies on retina display
	glViewport(0, 0, width, height);
	framebufferWidth = width;
	framebufferHeight = height;

	if (currentTest)
		currentTest->framebuffer_size_callback(window, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "MAIN -> mouse_callback : ( " << xpos << " : " << ypos << " )" << std::endl;
	if (currentTest)
		currentTest->mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "scroll_callback : (" << xoffset << " : " << yoffset << ")" << std::endl;
	if (currentTest)
		currentTest->scroll_callback(window, xoffset, yoffset);
}
