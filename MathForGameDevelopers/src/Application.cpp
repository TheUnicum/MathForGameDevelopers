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
#include "mfgd/quaternion.h"

// Functions Declaration
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void TestMatrixTia();

float MAX_DELTA_TIME = 0.5f;
int framebufferWidth = 0, framebufferHeight = 0;

float deltaTime = 0.0f;
float lastframe = 0.0f;

test::Test* currentTest = nullptr;

int main(int argc, char** args)
{
	Test_MFGD();
	TestMatrixTia();

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


void TestMatrixTia()
{
	// study glm::mat4
	glm::mat4 gIdentity(1.0f);
	std::cout << gIdentity << std::endl;

	// study glm::mat4
	Matrix4x4 mIdentity;
	std::cout << mIdentity << std::endl;


	// translate compare
	// ---------------------------------------
	glm::mat4 gScale(1.0f);
	gScale = glm::scale(gScale, glm::vec3(2.2f, 3.3f, 4.4f));
	std::cout << gScale << std::endl;

	Matrix4x4 mScale;
	mScale.SetScale(Vector(2.2f, 3.3f, 4.4f));
	std::cout << mScale << std::endl;

	std::cout << "Scale ok:: " << (mScale == gScale) << std::endl;


	// translate compare
	// ---------------------------------------
	glm::mat4 gTranslate(1.0f);
	gTranslate = glm::translate(gTranslate, glm::vec3(2.2f, 3.3f, 4.4f));
	std::cout << gTranslate << std::endl;

	Matrix4x4 mTranslate;
	mTranslate.SetTranslation(Vector(2.2f, 3.3f, 4.4f));
	std::cout << mTranslate << std::endl;
	std::cout << "Translate ok: " << (mTranslate == gTranslate) << std::endl;


	// scale & translate
	// ----------------------------------------
	glm::mat4 gTS(1.0f);
	gTS = gTranslate * gScale;
	std::cout << gTS << std::endl;

	Matrix4x4 mTS;
	mTS = mTranslate * mScale;
	std::cout << mTS << std::endl;

	std::cout << "Trans & Scale: " << (mTS == gTS) << std::endl;


	// LookAt
	// ----------------------------------------
	std::cout << "LookAt MATRIX" << std::endl;
	glm::mat4 gLkAt(1.0f);
	gLkAt = glm::lookAt(glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(3.0f, -2.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	std::cout << gLkAt << std::endl;

	Point _position = Point(1.0f, 2.0f, 3.0f);
	Vector _target = Vector(3.0f, -2.0f, 1.0f);
	Vector _worldUp = Vector(0.0f, 1.0f, 0.0f);
	Matrix4x4 mLkAt = GetView(_position, _target, _worldUp);
	std::cout << mLkAt << std::endl;

	std::cout << "LookAt MATRIX ok: " << (mLkAt == gLkAt) << std::endl;

	std::cout << "\n\n";

	// Rotation X
// ----------------------------------------
	std::cout << "Rotation AxX " << std::endl;
	glm::mat4 gRotX(1.0f);
	gRotX = glm::rotate(gRotX, glm::radians(55.5f), glm::vec3(1.0f, 0.0f, 0.0f));
	std::cout << gRotX << std::endl;

	Matrix4x4 mRotX;
	mRotX.SetRotation(55.5f, Vector(1.0f, 0.0f, 0.0f));
	std::cout << mRotX << std::endl;

	std::cout << "Rot Z MATRIX ok: " << (mRotX == gRotX) << "\n\n";


	// Rotation Y
// ----------------------------------------
	std::cout << "Rotation AxXY" << std::endl;
	glm::mat4 gRotY(1.0f);
	gRotY = glm::rotate(gRotY, glm::radians(55.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	std::cout << gRotY << std::endl;

	Matrix4x4 mRotY;
	mRotY.SetRotation(55.5f, Vector(0.0f, 1.0f, 0.0f));
	std::cout << mRotY << std::endl;

	std::cout << "Rot Y MATRIX ok: " << (mRotY == gRotY) << "\n\n";

	// Rotation Z
	// ----------------------------------------
	std::cout << "Rotation AxZ " << std::endl;
	glm::mat4 gRotZ(1.0f);
	gRotZ = glm::rotate(gRotZ, glm::radians(55.5f), glm::vec3(0.0f, 0.0f, 1.0f));
	std::cout << gRotZ << std::endl;

	Matrix4x4 mRotZ;
	mRotZ.SetRotation(55.5f, Vector(0.0f, 0.0f, 1.0f));
	std::cout << mRotZ << std::endl;
	
	std::cout << "Rot Z MATRIX ok: " << (mRotZ == gRotZ) << "\n\n";

	// Rotation X-Y-Z
// ----------------------------------------
	std::cout << "Rotation AxX-Y-Z " << std::endl;
	glm::mat4 gRotXYZ(1.0f);
	gRotXYZ = glm::rotate(gRotXYZ, glm::radians(55.5f), glm::vec3(1.23f, -5.78f, 0.04f));
	std::cout << gRotXYZ << std::endl;

	Matrix4x4 mRotXYZ;
	mRotXYZ.SetRotation(55.5f, Vector(1.23f, -5.78f, 0.04f));
	std::cout << mRotXYZ << std::endl;

	std::cout << "Rot XYZ MATRIX ok: " << (mRotXYZ == gRotXYZ) << "\n\n";

	// Matrix Multipe Moltiplication
	// ----------------------------------------
	std::cout << "Matrix Multipe Moltiplication " << std::endl;
	glm::mat4 gMult = gLkAt * gRotX * gRotY * gRotZ * gRotXYZ;
	std::cout << gMult << std::endl;

	Matrix4x4 mMult = mLkAt * mRotX * mRotY * mRotZ * mRotXYZ;
	std::cout << mMult << std::endl;

	std::cout << "Rot XYZ MATRIX ok: " << (mMult == gMult) << "\n\n";


	// Matrix Vector Moltiplicatin
	glm::vec4 gVec(12.3f, 45.6f, 78.9f, 5.5f);
	glm::vec4 gRes = gMult * gVec;
	std::cout << gRes << std::endl;
	
	Vector4d mVec = Vector4d(12.3f, 45.6f, 78.9f, 5.5f);
	Vector4d mRes = mMult * mVec;
	std::cout << mRes << std::endl;

	//std::cout << "Vector Translate ok: " << (mMult == gMult) << "\n\n";


	// Quaternions
	// ----------------------------------------
	std::cout << "\n--------Quaternions------ " << std::endl;
	std::cin.get();
	std::cout << "Rot   0 degrees around (1, 0, 0): ->" << Quaternion(Vector(1.0f, 0.0f, 0.0f), 0.0f) << std::endl;
	std::cin.get();
	std::cout << "Rot  90 degrees around (1, 0, 0): ->" << Quaternion(Vector(1.0f, 0.0f, 0.0f), 90.0f) << std::endl;
	std::cin.get();
	std::cout << "Rot 180 degrees around (0, 1, 0): ->" << Quaternion(Vector(1.0f, 0.0f, 0.0f), 180.0f) << std::endl;
	std::cin.get();
	std::cout << "Rot  90 degrees around (0, 1, 0): ->" << Quaternion(Vector(0.0f, 1.0f, 0.0f), 90.0f) << std::endl;
	std::cin.get();
	std::cout << "Rot 180 degrees around (0, 1, 0): ->" << Quaternion(Vector(0.0f, 1.0f, 0.0f), 180.0f) << std::endl;
	std::cin.get();

	std::cout << "\n---End-----Quaternions------ \n" << std::endl;
	// ---- End -------------
}