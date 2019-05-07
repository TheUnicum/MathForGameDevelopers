#pragma once

#include "tests/Test.h"

#include <iostream>

#include "opengl/Shader.h"
#include "opengl/Texture.h"

#include <memory>

//#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//#include "opengl/Camera.h"	

#include "opengl/Mesh.h"
#include "opengl/Model.h"

#include "mfgd/mfgd_EulerAngle.h"
#include "mfgd/Box.h"
#include "mfgd/matrix.h"

namespace test {

	class T01_FirstPrototipe_01 : public Test
	{
	public:
		T01_FirstPrototipe_01();
		~T01_FirstPrototipe_01();

		void OnUpdate(float deltaTime) override;
		void OnRender(GLFWwindow* window) override;
		void OnImGuiRender() override;
		void OnProcessInput(GLFWwindow* window, float deltaTime) override;
		// callbacks functions
		void framebuffer_size_callback(GLFWwindow* window, int width, int height) override;
		void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
	private:
		float m_f_fov;
		bool m_b_depth_test_active, m_b_depth_test_active_i_1;

		bool m_b_firstMouse;
		float m_lastX, m_lastY;
		//std::unique_ptr<Camera> m_camera;
		// keys pressed options
		bool m_mouse_disable, m_mouse_disable_i_1;
		bool m_mouse_lock;
		bool m_key_pressed = false;

		// MESH members
		std::unique_ptr<Mesh> m_mesh, m_mesh_quad, m_mesh_grass, m_mesh_grass_Clamped;
		std::shared_ptr<Shader> m_ShaderMesh, m_ShaderLine;
		std::shared_ptr<Texture> msp_mTexture0, msp_mTexture1, msp_mTexture2;
		std::vector<std::shared_ptr<Texture>> msp_Textures;
		// Floor
		std::shared_ptr<Texture> msp_mTextureFloor;
		std::vector<std::shared_ptr<Texture>> msp_mTextures2;

		// MODEL
		std::unique_ptr<Model> m_model;

		// Face Culling
		bool m_b_face_culling_enabled, m_b_CullFaceFront;

		// Disable VSync 
		bool m_b_VSync_disabled, m_b_VSync_disabled_i_1;

		// MFGD
		glm::vec3 m_box_gravity = glm::vec3(0, -20, 0);

		bool m_enable_LERP;
		float m_velocity = 5;

		void renderPop(glm::vec3 position, glm::vec3 scale, glm::vec3 color, glm::mat4 proj, glm::mat4 view);

		glm::vec3 m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 m_lightPos = glm::vec3(-4.0f, 4.0f, 2.0f);

		//std::shared_ptr<Box> m_Box_01, m_Box_02;
		void renderCube(std::shared_ptr<Box> box, glm::vec3 color, glm::mat4 proj, glm::mat4 view, bool sprite_on = false);

		std::shared_ptr<Player> m_Player_01;

		void renderShot(Vector p_start, Vector p_end, glm::mat4 proj, glm::mat4 view);
		void renderBillboard(float flRadius, Vector vecUp, Vector vecRight, glm::mat4 proj, glm::mat4 view, glm::mat4 model);

		bool m_Shot_active;
		std::shared_ptr<Box> m_Target_1, m_Target_2;

		// Trace a line through the world to simulate a bullet
		bool TraceLine(const Vector& v0, const Vector& v1, Vector& vecIntersection);

		float m_flTimeCreated = 0.0f;

	};

}

