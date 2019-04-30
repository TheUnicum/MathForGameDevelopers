#include "T01_FirstPrototipe_01.h"
#include "GLFW/glfw3.h"

#include "mfgd/mfgd_Classes.h"

namespace test {

	T01_FirstPrototipe_01::T01_FirstPrototipe_01()
		: m_f_fov(45.0f),
		m_b_depth_test_active(true), m_b_depth_test_active_i_1(false),
		m_b_firstMouse(true),
		m_mouse_lock(true),		// <--- for MFGD
		m_b_face_culling_enabled(false), m_b_CullFaceFront(false),
		m_b_VSync_disabled(true), m_b_VSync_disabled_i_1(false),

		m_enable_LERP(true),
		m_MFGD_EulerAngle_active(true)

	{
		// Initialize camera
		m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 2.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f, -30.f);
		//m_camera->ResetYawPitch();
		m_camera->ProcessMouseMovement(0, 0);

		#include "E00_cube_vertices.h"

		// NEW mesh----------------------------------------------------------------------------------------
		std::vector<Vertex> vertices_3v_3n_2t;
		glm::vec3 temp_pos, temp_norm;
		glm::vec2 temp_texC;
		int i = 0;
		for (float val : cube_Unitario_vertices_3v_3n_2t)
		{
			switch (i)
			{
			case 0:	// Position
				temp_pos.x = val;
				break;
			case 1:
				temp_pos.y = val;
				break;
			case 2:
				temp_pos.z = val;
				break;
			case 3:	// Normal
				temp_norm.x = val;
				break;
			case 4:
				temp_norm.y = val;
				break;
			case 5:
				temp_norm.z = val;
				break;
			case 6:	// TexCoords
				temp_texC.x = val;
				break;
			case 7:
				temp_texC.y = val;
				vertices_3v_3n_2t.emplace_back(temp_pos, temp_norm, temp_texC);
				i = -1; // Restart counter...
				break;
			default:
				break;
			}
			i++;
		}
		// To check the correct format of vertec:position/normal/textCoords
		//for (auto& ele : vertices_3v_3n_2t)
		//	std::cout << ele << std::endl;

		// Textures - Shared pointer
		msp_mTexture0 = std::make_shared<Texture>("res/textures/marble.jpg", TextureType::DIFFUSE); // ontainer2.png
		msp_Textures.push_back(msp_mTexture0);

		std::vector<unsigned int> indices0;
		m_mesh = std::make_unique<Mesh>(vertices_3v_3n_2t, indices0, msp_Textures);
		m_ShaderMesh = std::make_unique<Shader>("src/tests/01_FirstPrototipe/S01_FirstPrototipe_01.Shader");



		// NEW mesh-  index quad----------------------------------------------------------------------------------
		float positions_quad[] = {
			// positions          // normals           // texture coords	// index
			-0.5f, -0.5f,  0.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,			// 0
			 0.5f, -0.5f,  0.0f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,			// 1
			 0.5f,  0.5f,  0.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,			// 2
			-0.5f,  0.5f,  0.0f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,			// 3
		};

		unsigned int uint_quad_indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		std::vector<Vertex> vertices_quad_3v_3n_2t;
		std::vector<unsigned int> vec_quad_indices;
		i = 0;
		for (float val : positions_quad)
		{
			switch (i)
			{
			case 0:	// Position
				temp_pos.x = val;
				break;
			case 1:
				temp_pos.y = val;
				break;
			case 2:
				temp_pos.z = val;
				break;
			case 3:	// Normal
				temp_norm.x = val;
				break;
			case 4:
				temp_norm.y = val;
				break;
			case 5:
				temp_norm.z = val;
				break;
			case 6:	// TexCoords
				temp_texC.x = val;
				break;
			case 7:
				temp_texC.y = val;
				vertices_quad_3v_3n_2t.emplace_back(temp_pos, temp_norm, temp_texC);
				i = -1; // Restart counter...
				break;
			default:
				break;
			}
			i++;
		}

		for (unsigned int val : uint_quad_indices)
			vec_quad_indices.push_back(val);
		
		msp_mTextureFloor = std::make_shared<Texture>("res/textures/metal.png", TextureType::DIFFUSE);
		msp_mTextures2.push_back(msp_mTextureFloor);
		m_mesh_quad = std::make_unique<Mesh>(vertices_quad_3v_3n_2t, vec_quad_indices, msp_mTextures2);


		// --------------------- New My Boxes--------------------------
		m_Box_01 = std::make_shared<Box>(Vector(-1.0f, 1.0f, -1.0f), Vector(0.2f, 0.2f, 0.2f));
		m_Box_02 = std::make_shared<Box>(Vector(+1.0f, 1.0f, -1.5f), Vector(0.2f, 0.5f, 0.2f));

		// --------------------- New My Player--------------------------
		m_Player_01 = std::make_shared<Player>(Vector(+0.0f, 0.0f, 0.0f), Vector(0.22f, 0.44f, 0.22f));

		//  VSync / Enabel & Disable
		glfwSwapInterval(1);
	}

	T01_FirstPrototipe_01::~T01_FirstPrototipe_01()
	{
		glfwSwapInterval(0);

		GLCall(glDisable(GL_CULL_FACE));
		GLCall(glCullFace(GL_BACK));
		GLCall(glFrontFace(GL_CCW));
	}

	void T01_FirstPrototipe_01::OnUpdate(float deltaTime)
	{
		if (m_enable_LERP)
		{
			m_box_movement.x = Approch(m_box_GoalVelocity.x, m_box_movement.x, deltaTime * m_box_LerpVelocity.x);
			m_box_movement.z = Approch(m_box_GoalVelocity.z, m_box_movement.z, deltaTime * m_box_LerpVelocity.z);
			//m_box_velocity.x = Approch(m_box_GoalVelocity.x, m_box_velocity.x, deltaTime * m_box_LerpVelocity.x);
			//m_box_velocity.z = Approch(m_box_GoalVelocity.z, m_box_velocity.z, deltaTime * m_box_LerpVelocity.z);

			m_Player_01->m_movement.x = Approch(m_Player_01->m_GoalVelocity.x,
				m_Player_01->m_movement.x, deltaTime * m_Player_01->m_LerpVelocity.x);
			m_Player_01->m_movement.y = Approch(m_Player_01->m_GoalVelocity.y,
				m_Player_01->m_movement.y, deltaTime * m_Player_01->m_LerpVelocity.y);

		}
		else
		{
			m_box_movement.x = m_box_GoalVelocity.x;
			m_box_movement.z = m_box_GoalVelocity.z;
			//m_box_movement.x = m_box_GoalVelocity.x;
			//m_box_velocity.z = m_box_GoalVelocity.z;
		}

		// Math for Game Developers - Character Movement 7 (Cross Product)-----------------------------------------
		//m_box_movement
		Vector vecForward = m_Box_angView.ToVector();
		vecForward.y = 0;
		vecForward.Normalized();

		Vector vecUp(0.0f, 1.0f, 0.0f);
		Vector vecRight = vecForward.Cross(vecUp);

		Vector box_velocity_Temp = vecForward * (-m_box_movement.z) + vecRight * m_box_movement.x;
		m_box_velocity = glm::vec3(box_velocity_Temp.x, box_velocity_Temp.y, box_velocity_Temp.z);

		m_Player_01->m_velocity = box_velocity_Temp;
		//
		m_Player_01->m_position = m_Player_01->m_position + m_Player_01->m_velocity * deltaTime;

		// ---------------------------------------------------------------------------------------------------------

		// MFGD 13
		m_box_position += m_box_velocity * deltaTime;

		m_box_velocity += m_box_gravity * deltaTime;


		if (m_box_position.y <= 0)
		{
			m_box_position.y = 0;
			m_box_velocity.y = 0;
		}

		if (!m_MFGD_EulerAngle_active)
		{
			// old version with cam class
			glm::vec3 cam = m_box_position + glm::vec3(0, 2, 3);//  -3.0f * m_camera->GetCamFront() + glm::vec3(0.0f, 1.0f, 0.0f);
			m_camera->SetCamPosition(cam);
		}
		else
		{
			// new version with MFGD AEuler class ( Remeber I set m_mouse_lock(true),		// <--- for MFGD on CONSTRUCTOR
			Vector mfgd_vector = m_Box_angView.ToVector() * 4;
			m_box_cam_position = m_box_position - glm::vec3(mfgd_vector.x, mfgd_vector.y, mfgd_vector.z);
		}

		

		// Mybox
		m_Box_01->Update(deltaTime);
		m_Box_01->Gravity(m_box_gravity, deltaTime);
		m_Box_02->Update(deltaTime);
		m_Box_02->Gravity(m_box_gravity, deltaTime);


		// MyPlayer
		m_Player_01->Boundaries();
	}

	void T01_FirstPrototipe_01::OnRender(GLFWwindow* window)
	{
		if (m_b_depth_test_active != m_b_depth_test_active_i_1)
		{
			if (m_b_depth_test_active)
			{
				// Remember to glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
				GLCall(glEnable(GL_DEPTH_TEST));
				//GLCall(glDepthMask(GL_FALSE)); //OpenGL allows us to disable writing to the depth buffer by setting its depth mask to GL_FALSE
			}
			else
			{
				GLCall(glDisable(GL_DEPTH_TEST));
			}
			m_b_depth_test_active_i_1 = m_b_depth_test_active;
		}


		if (m_mouse_disable != m_mouse_disable_i_1)
		{
			if (m_mouse_disable)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			//std::cout << "Enable changed" << std::endl;
			m_mouse_disable_i_1 = m_mouse_disable;
		}

		// VSync
		if (m_b_VSync_disabled != m_b_VSync_disabled_i_1)
		{
			glfwSwapInterval(m_b_VSync_disabled ? 0 : 1);
			m_b_VSync_disabled_i_1 = m_b_VSync_disabled;
		}

		// Disable blending for manual discard-----------------------------
		GLCall(glDisable(GL_BLEND));

		glClearColor(0.73f, 0.845f, 0.918f, 1.0f);
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glm::vec3 m_color_floor = glm::vec3(0.435f, 0.52f, 0.68f);
		glm::vec3 m_color_box = glm::vec3(0.239f, 0.545f, 0.310f);
		glm::vec3 m_color_player = glm::vec3(0.5f, 0.235f, 0.086f);

		//  MODEL MATRIX Correct order
		// - translation
		// - rotation
		// - scale
		float inv_ratio_aspect = 960.0f / 540.0f;

		// Global Variables
		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		glm::mat4 proj(1.0f);
		glm::mat4 mvp;

		if (!m_MFGD_EulerAngle_active)
		{
			//old version with cam class
			view = m_camera->GetViewMatrix();
		}
		else
		{
			// convert MFGD Vector to glm::vec3 to retrive lookAt matrix
			Vector mfgd_vector_temp = m_Box_angView.ToVector();
			glm::vec3 target = m_box_cam_position + glm::vec3(mfgd_vector_temp.x, mfgd_vector_temp.y, mfgd_vector_temp.z);
			view = glm::lookAt(m_box_cam_position, target , glm::vec3(0.0f, 1.0f, 0.0f));
		}
		

		proj = glm::perspective(glm::radians(m_f_fov), inv_ratio_aspect, 0.1f, 100.0f);

		if (m_b_face_culling_enabled)
		{
			GLCall(glEnable(GL_CULL_FACE));
			if (m_b_CullFaceFront)
			{
				GLCall(glCullFace(GL_FRONT));
				GLCall(glFrontFace(GL_CCW));
			}
			else
			{
				GLCall(glCullFace(GL_BACK));
				GLCall(glFrontFace(GL_CCW));
			}
		}
		else
		{
			GLCall(glDisable(GL_CULL_FACE));
			GLCall(glCullFace(GL_BACK));
			GLCall(glFrontFace(GL_CCW));
		}

		// Mesh 1st Cube
		renderCube(glm::vec3(-2.0f, 0.0f, -1.0f), glm::vec3(0.2f), m_color_box, proj, view);

		// Mesh 2nd Cube
		renderCube(glm::vec3(2.f, 0.0f, -2.5f), glm::vec3(0.2f), m_color_box, proj, view);

		// Mesh 3th Cube
		renderCube(glm::vec3(-0.7f, 0.0f, 1.0f), glm::vec3(0.2f), m_color_box, proj, view);

		// Mesh Player Box
		renderCube(m_box_position, glm::vec3(0.2f, 0.4f, 0.2f), m_color_player, proj, view);

		// 
		renderCube(m_Box_01, glm::vec3(1.0f, 0.0f, 0.0f), proj, view);
		renderCube(m_Box_02, glm::vec3(0.0f, 1.0f, 0.0f), proj, view);

		//
		renderCube(m_Player_01, glm::vec3(0.0, 0.0, 1.0f), proj, view);

		// Disable face culling for bidimensional object
		GLCall(glDisable(GL_CULL_FACE));
		{	// Floor
			m_ShaderMesh->Bind();

			// Model Matrix
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(10.0f));

			//MVP
			mvp = proj * view * model;
			m_ShaderMesh->SetUniformMat4f("u_mvp", mvp);
			m_ShaderMesh->SetUniformMat4f("u_model", model);
			m_ShaderMesh->SetUniformMat3f("u_transInvers_model", glm::mat3(glm::transpose(glm::inverse(model))));

			m_ShaderMesh->SetUniform3fv("u_Box_color", m_color_floor);
			

			m_mesh_quad->Draw(m_ShaderMesh);
		}
	}

	void T01_FirstPrototipe_01::OnImGuiRender()
	{
		ImGui::Text("Face culling!");
		IMGUI_FPS;

		ImGui::Checkbox("Depth Test", &m_b_depth_test_active);
		ImGui::Text("Press M to active/disable mouse!");
		ImGui::Text("Press N to active/disable mouse!");
		ImGui::Text("Press F to fix player to ground!");
		ImGui::SliderFloat("FOV", &m_f_fov, 20.0f, 80.0f);

		ImGui::Text("Face culling options:");
		ImGui::Checkbox("Enabel face culling", &m_b_face_culling_enabled);
		ImGui::Checkbox("Cull Front Faces", &m_b_CullFaceFront);

		ImGui::Text("");
		ImGui::Checkbox("Enable LERP", &m_enable_LERP);
		ImGui::SliderFloat3("Velo LERP:", &m_box_LerpVelocity.x, 1.0f, 15.0f);
		ImGui::SliderFloat("Velo box:", &m_velocity, 2.0f, 10.0f);
		ImGui::Checkbox("EULER ANGLE MFGD", &m_MFGD_EulerAngle_active);

		ImGui::Checkbox("Disable VSync", &m_b_VSync_disabled);
	}

	void T01_FirstPrototipe_01::OnProcessInput(GLFWwindow * window, float deltaTime)
	{
		if (false)	// old version without LERP
		{
			//m_box_velocity = glm::vec3(0);
			m_box_velocity.x = 0;
			m_box_velocity.z = 0;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				m_box_velocity.z -= m_velocity;
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				m_box_velocity.z += m_velocity;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				m_box_velocity.x -= m_velocity;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				m_box_velocity.x += m_velocity;
		}


		m_box_GoalVelocity.x = 0;
		m_box_GoalVelocity.z = 0;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_box_GoalVelocity.z = -(float)m_velocity;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_box_GoalVelocity.z = (float)m_velocity;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_box_GoalVelocity.x = -(float)m_velocity;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_box_GoalVelocity.x = (float)m_velocity;

		m_Player_01->m_GoalVelocity.x = 0;
		m_Player_01->m_GoalVelocity.z = 0;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_Player_01->m_GoalVelocity.z = -(float)m_velocity;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_Player_01->m_GoalVelocity.z = (float)m_velocity;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_Player_01->m_GoalVelocity.x = -(float)m_velocity;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_Player_01->m_GoalVelocity.x = (float)m_velocity;
		

		// Jumping only if at zero position
		if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (abs(m_box_velocity.y) < 0.25))
			m_box_velocity.y = 7;


		glm::vec3 direction(0.0f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			direction.z -= 1;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			direction.z += 1;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			direction.x -= 1;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			direction.x += 1;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			direction.y += 1;
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			direction.y -= 1;
		
		//m_camera->ProcessKeyboard(direction, deltaTime);
		//glm::vec3 front = m_camera->GetCamFront();
		


		// Mouse pressed
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			if (!m_key_pressed)
				m_mouse_disable = !m_mouse_disable;
			m_key_pressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		{
			if (!m_key_pressed)
				m_mouse_lock = !m_mouse_lock;
			m_key_pressed = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			if (!m_key_pressed)
				m_camera->SetFixToGround(!m_camera->GetFixToGround());
			m_key_pressed = true;
		}
		else
			m_key_pressed = false;
	}

	void T01_FirstPrototipe_01::framebuffer_size_callback(GLFWwindow * window, int width, int height)
	{
	}

	void T01_FirstPrototipe_01::mouse_callback(GLFWwindow * window, double xpos, double ypos)
	{
		//std::cout << xpos << " " << ypos << std::endl;

		if (m_b_firstMouse)
		{
			//m_camera->ResetYawPitch();
			m_lastX = (float)xpos;
			m_lastY = (float)ypos;
			m_b_firstMouse = false;
		}

		float xoffset = (float)xpos - m_lastX;
		float yoffset = -((float)ypos - m_lastY);
		m_lastX = (float)xpos;
		m_lastY = (float)ypos;
		if (!m_mouse_lock)
			m_camera->ProcessMouseMovement(xoffset, yoffset);

		// MFGD
		float flSensitivity = 0.005f;
		m_Box_angView.p += yoffset * flSensitivity;
		m_Box_angView.y += xoffset * flSensitivity;
		m_Box_angView.Normalize();

		// myPlayer
		m_Player_01->SetEAngle(yoffset * flSensitivity, xoffset * flSensitivity, 0);

	}
	void T01_FirstPrototipe_01::renderCube(glm::vec3 position, glm::vec3 scale, glm::vec3 color, glm::mat4 proj, glm::mat4 view)
	{
		m_ShaderMesh->Bind();
		// Model Matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);	// 0.01 to avoid Z-fighting
		glm::vec3 scale_temp = glm::vec3(0.0f, scale.y, 0.0f);
		model = glm::translate(model, scale_temp);
		model = glm::scale(model, scale);
		// MVP
		glm::mat4 mvp = proj * view * model;
		m_ShaderMesh->SetUniformMat4f("u_mvp", mvp);
		m_ShaderMesh->SetUniformMat4f("u_model", model);
		m_ShaderMesh->SetUniformMat3f("u_transInvers_model", glm::mat3(glm::transpose(glm::inverse(model))));

		m_ShaderMesh->SetUniform3fv("u_Box_color", color);

		m_ShaderMesh->SetUniform3fv("u_lightColor", m_lightColor);
		m_ShaderMesh->SetUniform3fv("u_lightPos", m_lightPos);
		m_ShaderMesh->SetUniform3fv("u_viewPos", m_camera->GetCamPosition());


		// Draw MESH
		m_mesh->Draw(m_ShaderMesh);
	}
	void T01_FirstPrototipe_01::renderCube(std::shared_ptr<Box> box, glm::vec3 color, glm::mat4 proj, glm::mat4 view)
	{
		m_ShaderMesh->Bind();
		// Model Matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, box->GetPosVec3());
		model = glm::scale(model, box->GetScaleVec3());
		// MVP
		// MVP
		glm::mat4 mvp = proj * view * model;
		m_ShaderMesh->SetUniformMat4f("u_mvp", mvp);
		m_ShaderMesh->SetUniformMat4f("u_model", model);
		m_ShaderMesh->SetUniformMat3f("u_transInvers_model", glm::mat3(glm::transpose(glm::inverse(model))));

		m_ShaderMesh->SetUniform3fv("u_Box_color", color);

		m_ShaderMesh->SetUniform3fv("u_lightColor", m_lightColor);
		m_ShaderMesh->SetUniform3fv("u_lightPos", m_lightPos);
		m_ShaderMesh->SetUniform3fv("u_viewPos", m_camera->GetCamPosition());

		// Draw MESH
		m_mesh->Draw(m_ShaderMesh);
	}
}


