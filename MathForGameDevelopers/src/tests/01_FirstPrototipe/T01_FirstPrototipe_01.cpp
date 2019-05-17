#include "T01_FirstPrototipe_01.h"
#include "GLFW/glfw3.h"

#include "mfgd/mfgd_Classes.h"
#include "mfgd/AABB.h"
#include "mfgd/Vector4d.h"

#include "mfgd/axisangle.h"
#include "mfgd/quaternion.h"

namespace test {

	T01_FirstPrototipe_01::T01_FirstPrototipe_01()
		: m_f_fov(45.0f),
		m_b_depth_test_active(true), m_b_depth_test_active_i_1(false),
		m_b_firstMouse(true),
		m_mouse_lock(true),		// <--- for MFGD
		m_b_face_culling_enabled(false), m_b_CullFaceFront(false),
		m_b_VSync_disabled(true), m_b_VSync_disabled_i_1(false),

		m_enable_LERP(true)
	{
		#include "E00_cube_vertices.h"


		// NEW mesh----------------------------------------------------------------------------------------
		std::vector<Vertex> vertici;
		unsigned int lay[] = { 3,3,4 };
		fill_vertex_from_array(vertici, cube_Unitario_vertices_3v_3n_2t, sizeof(cube_Unitario_vertices_3v_3n_2t)/sizeof(float), lay, 3);

		// Textures - Shared pointer
		msp_mTexture0 = std::make_shared<Texture>("res/textures/monster.png", TextureType::DIFFUSE); // ontainer2.png
		msp_Textures.push_back(msp_mTexture0);

		std::vector<unsigned int> indices0;
		
		// Test sempli
		//m_mesh = std::make_unique<Mesh>(vertices_3v_3n_2t, indices0, msp_Textures);
		m_mesh = std::make_unique<Mesh>(vertici, indices0, msp_Textures);

		m_ShaderMesh = std::make_unique<Shader>("src/tests/01_FirstPrototipe/S01_FirstPrototipe_01.Shader");
		m_ShaderLine = std::make_unique<Shader>("src/tests/01_FirstPrototipe/S00_Light.Shader");


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
		glm::vec3 temp_pos, temp_norm;
		glm::vec2 temp_texC;
		int i = 0;
		
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

		// --------------------- New My Player--------------------------
		m_Player_01 = std::make_shared<Player>(Point(+0.0f, 0.0f, 0.0f), Vector(0.22f, 0.44f, 0.22f), 0.0f, Vector(1.5f, 0.5f, 1.5f));

		// --------------------- first Target with AABBB ---------------		
		//Box::CreateCharacter(Point(+6.0f, 1.5f, 4.0f), Vector(1.5f, 0.5f, 1.5f), 30.0f, Vector(1.5f, 0.5f, 1.5f));
		//Box::CreateCharacter(Point(+5.5f, 1.5f, -2.0f), Vector(1.5f, 1.5f, 0.5f), 160.0f, Vector(1.5f, 0.5f, 1.5f));
		//Box::CreateCharacter(Point(-5.5f, 1.5f, -2.0f), Vector(0.5f, 0.5f, 0.5f), 40.0f, Vector(1.5f, 0.5f, 1.5f));

		Box::CreateCharacter(Point(+6.0f, 1.5f, 4.0f), Vector(1.0f, 1.0f, 1.0f), 30.0f, Vector(1.5f, 0.5f, 1.5f));
		Box::CreateCharacter(Point(+5.5f, 1.5f, -2.0f), Vector(1.0f, 1.0f, 1.0f), 160.0f, Vector(1.5f, 0.5f, 1.5f));
		Box::CreateCharacter(Point(-5.5f, 1.5f, -2.0f), Vector(1.0f, 1.0f, 1.0f), 40.0f, Vector(1.5f, 0.5f, 1.5f));


		//Box::CreateCharacter(Point(-1.f, 1.0f, 0.0f), Vector(0.2f, .75f, 0.2f), 00.0f, Vector(1.0f, 0.0f, 0.0f));
		//Box::CreateCharacter(Point(+1.f, 1.0f, 0.0f), Vector(0.2f, .75f, 0.2f), 00.0f, Vector(1.0f, 0.0f, 0.0f));



		//  VSync / Enabel & Disable
		glfwSwapInterval(1);
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
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
			m_Player_01->m_movement.x = Approch(m_Player_01->m_GoalVelocity.x,
				m_Player_01->m_movement.x, deltaTime * m_Player_01->m_LerpVelocity.x);
			m_Player_01->m_movement.z = Approch(m_Player_01->m_GoalVelocity.z,
				m_Player_01->m_movement.z, deltaTime * m_Player_01->m_LerpVelocity.z);
		}
		else
		{
			m_Player_01->m_movement.x = m_Player_01->m_GoalVelocity.x;
			m_Player_01->m_movement.z = m_Player_01->m_GoalVelocity.z;
		}
		m_Player_01->m_movement.y = m_Player_01->m_GoalVelocity.y;

		// Math for Game Developers - Character Movement 7 (Cross Product)-----------------------------------------
		Vector vecForward = m_Player_01->m_EAngle.ToVector();	// m_Box_angView.ToVector();
		vecForward.y = 0;
		vecForward.Normalized();

		Vector vecUp(0.0f, 1.0f, 0.0f);
		Vector vecRight = vecForward.Cross(vecUp);

		Vector box_velocity_Temp = vecForward * (-m_Player_01->m_movement.z) + vecRight * m_Player_01->m_movement.x;
		
		m_Player_01->m_velocity = box_velocity_Temp;
		m_Player_01->m_position = m_Player_01->m_position + m_Player_01->m_velocity * deltaTime;
		
		//m_Player_01->Gravity(m_Player_01->m_Gravity, deltaTime);

		// new version with MFGD AEuler class ( Remeber I set m_mouse_lock(true),		// <--- for MFGD on CONSTRUCTOR
		m_Player_01->m_Cam_Position = m_Player_01->m_position - m_Player_01->m_EAngle.ToVector() * 4;

	
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
		//GLCall(glDisable(GL_BLEND));

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


		Vector vec_target = m_Player_01->m_EAngle.ToVector();
		glm::vec3 target_Vec3_ = glm::vec3(vec_target.x, vec_target.y, vec_target.z);

		Vector vecUp(0.0f, 1.0f, 0.0f);
		Vector vecRight = vec_target.Cross(vecUp);
		Vector vecFoward = vecUp.Cross(vecRight);
		vecRight = vecRight.Normalized();
		vecFoward = vecFoward.Normalized();

		glm::vec3 Vec3DirRight = glm::vec3(vecRight.x, 0.0f, vecRight.z) * .5f;
		glm::vec3 Vec3DirForward = glm::vec3(vecFoward.x, 0.0f, vecFoward.z) * -1.0f;

		glm::vec3 pos_Vec3 = glm::vec3(m_Player_01->m_position.x, m_Player_01->m_position.y, m_Player_01->m_position.z) 
				+ glm::vec3(0.0, .7f, 0.0) + Vec3DirRight + Vec3DirForward;
		view = glm::lookAt(pos_Vec3, pos_Vec3 + target_Vec3_, glm::vec3(0.0f, 1.0f, 0.0f));
		proj = glm::perspective(glm::radians(m_f_fov), inv_ratio_aspect, 0.1f, 100.0f);


		// my_View matrix----------------------------------------------------------------------
		Point temp_pos = Point(pos_Vec3.x, pos_Vec3.y, pos_Vec3.z);
		Vector pos_plus_target = temp_pos + vec_target;
		vecUp = Vector(0.0f, 1.0f, 0.0f);
		Matrix4x4 myView = GetView(temp_pos, pos_plus_target, vecUp);


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


		//
		renderCube(m_Player_01, glm::vec3(0.0, 0.0, 1.0f), proj, view);
		//renderCube(m_Player_01, glm::vec3(0.0, 0.0, 1.0f), proj, glm::transpose(myView.ToGlm()));
		//renderCube(m_Player_01, glm::vec3(0.0, 0.0, 1.0f), proj, myView.ToGlm());


		// slerp euler
		float flTime = (float)sin(glfwGetTime());
		float t = RemapClamp(flTime, -0.8f, 0.8f, 0, 1);

		EAngle angStart(0.0f, 90.0f, 0.0f);
		EAngle angEnd(90.0f, 0.0f, 90.0f);

		Quaternion qStart(Vector(0, 1, 0), 90);
		Quaternion qEnd = Quaternion(Vector(0, 0, 1), 90) * Quaternion(Vector(1, 0, 0), 90);
		
		// I'm converting the Eulers to Vectors so that I can do a "slerp" on them.
		// I'm not creating operator overloads for EAngle since we really shouldn't
		// be doing this operation on mthe anyways.
		Vector vecStart(angStart.p, angStart.y, angStart.r);
		Vector vecEnd(angEnd.p, angEnd.y, angEnd.r);
		Vector vecCurrent = vecStart + (vecEnd - vecStart) * t; // Here's the "slerp"
		EAngle angCurrent(vecCurrent.x, vecCurrent.y, vecCurrent.z);

		//Box::m_apEntityList[3]->m_v3_rotation
		if (Box::m_apEntityList[3])
			Box::m_apEntityList[3]->SetRotation(angCurrent);
		if (Box::m_apEntityList[4])
			Box::m_apEntityList[4]->SetRotation(qStart.Slerp(qEnd, t));


		// Target Box
		//renderCube(m_Target_1, glm::vec3(0.6, 0.6, 1.0f), proj, view, false);
		//renderCube(m_Target_2, glm::vec3(0.6, 0.6, 1.0f), proj, view, false);
		//renderCube(m_Target_3, glm::vec3(0.6, 0.6, 1.0f), proj, view, false);
		for (size_t i = 0; i < MAX_CHARACTERS; i++)
		{
			if (Box::m_apEntityList[i])
				renderCube(Box::m_apEntityList[i], glm::vec3(0.6, 0.6, 1.0f), proj, view, false);
		}



		
		if (m_Shot_active)
		{

			Vector p_start = m_Player_01->m_position;
			Vector p_end = m_Player_01->m_EAngle.ToVector() * 100;
			
			Vector vecIntersection;
			//TraceLine
		
			Box* pHit = nullptr;
			if (TraceLine(p_start, p_start + p_end, vecIntersection, pHit))
			{
				float flPuffTime = 0.05f;
				float flTimeOver = m_flTimeCreated + flPuffTime;
				
				float flStartSize = 0.2f;
				float flEndSize = 0.5f;
				float time_actual = (float)glfwGetTime();
				float flSize = Remap(time_actual, m_flTimeCreated, flTimeOver, flStartSize, flEndSize);
				float iOrange = Remap(time_actual, m_flTimeCreated, flTimeOver, 0.2f, 1.0f);


				//std::cout << "BANG!" << std::endl;
				renderShot(p_start, vecIntersection, proj, view);

				// Pop at the end if not collision
				Vector pos_end = vecIntersection;
				//renderPop(glm::vec3(pos_end.x, pos_end.y, pos_end.z), glm::vec3(0.05f), glm::vec3(1.0, 1.0, .0f), proj, view);
				renderPop(glm::vec3(pos_end.x, pos_end.y, pos_end.z), glm::vec3(flSize), glm::vec3(1.0, iOrange, 0.0f), proj, view);

				if (pHit && !m_Shot_active_i_1)
					pHit->TakeDamage(100);
	

			}
			else
			{
				renderShot(p_start, p_start + p_end, proj, view);

				// Pop at the end if not collision
				Vector pos_end = p_start + p_end;
				m_flTimeCreated = (float)glfwGetTime();
			}
		}
		else
			m_flTimeCreated = (float)glfwGetTime();


		m_Shot_active_i_1 = m_Shot_active;

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
		//ImGui::SliderFloat3("Velo LERP:", &m_box_LerpVelocity.x, 1.0f, 15.0f);
		ImGui::SliderFloat3("Velo LERP:", &m_Player_01->m_LerpVelocity.x, 1.0f, 15.0f);
		ImGui::SliderFloat("Velo box:", &m_velocity, 2.0f, 10.0f);

		ImGui::Checkbox("Disable VSync", &m_b_VSync_disabled);
	}

	void T01_FirstPrototipe_01::OnProcessInput(GLFWwindow * window, float deltaTime)
	{

		m_Player_01->m_GoalVelocity.x = 0;
		m_Player_01->m_GoalVelocity.z = 0;
		m_Player_01->m_GoalVelocity.y = 0;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_Player_01->m_GoalVelocity.z = -(float)m_velocity;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_Player_01->m_GoalVelocity.z = (float)m_velocity;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_Player_01->m_GoalVelocity.x = -(float)m_velocity;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_Player_01->m_GoalVelocity.x = (float)m_velocity;
		
		// Jumping only if at zero position
		if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (abs(m_Player_01->m_velocity.y) < 0.25))
			m_Player_01->m_GoalVelocity.y = 7;

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
		else
			m_key_pressed = false;


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			//std::cout << "mouse right clicked" << std::endl;
			m_Shot_active = true;
		}
		else
			m_Shot_active = false;


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
		//if (!m_mouse_lock)
		//	m_camera->ProcessMouseMovement(xoffset, yoffset);

		// MFGD
		float flSensitivity = 0.005f;
		m_Player_01->m_EAngle.p += yoffset * flSensitivity;
		m_Player_01->m_EAngle.y += xoffset * flSensitivity;
		m_Player_01->m_EAngle.Normalize();

	}


	void T01_FirstPrototipe_01::renderPop(glm::vec3 position, glm::vec3 scale, glm::vec3 color, glm::mat4 proj, glm::mat4 view)
	{
		m_ShaderMesh->Bind();
		// Model Matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		// MVP
		glm::mat4 mvp = proj * view * model;
		m_ShaderMesh->SetUniformMat4f("u_mvp", mvp);
		m_ShaderMesh->SetUniformMat4f("u_model", model);
		m_ShaderMesh->SetUniformMat3f("u_transInvers_model", glm::mat3(glm::transpose(glm::inverse(model))));

		m_ShaderMesh->SetUniform3fv("u_Box_color", color);

		m_ShaderMesh->SetUniform3fv("u_lightColor", m_lightColor);
		m_ShaderMesh->SetUniform3fv("u_lightPos", m_lightPos);
		//m_ShaderMesh->SetUniform3fv("u_viewPos", m_camera->GetCamPosition());

		m_ShaderMesh->SetUniform1i("u_Image", 0);

		// Draw MESH
		m_mesh->Draw(m_ShaderMesh);
	}

	void T01_FirstPrototipe_01::renderCube(std::shared_ptr<Box> box, glm::vec3 color, glm::mat4 proj, glm::mat4 view, bool sprite_on)
	{

		//if (sprite_on)
		if (false)
		{
			float flRadius = 1.0f;
			Vector vp = m_Player_01->m_position - box->m_position;// -
			vp = vp.Normalized();
			Vector vecUp = Vector(0.0f, 1.0f, 0.0f);
			Vector vecRight = vecUp.Cross(vp);
			vecUp = vp.Cross(vecRight);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, box->GetPosVec3());
			model = glm::scale(model, box->GetScaleVec3());
			renderBillboard(flRadius, vecUp, vecRight, proj, view, model);
		}
		else
		{
			m_ShaderMesh->Bind();
			// Model Matrix
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, box->GetPosVec3());
			model = glm::scale(model, box->GetScaleVec3());

			Matrix4x4 myScale, myRotation, myTranslate;
			myScale.SetScale(box->m_scale);
			myRotation.SetRotation(box->m_f_angle, box->m_v3_rotation);
			myTranslate.SetTranslation(box->m_position);
			box->m_Transform = myTranslate * myRotation * myScale;
			model = (box->m_Transform).ToGlm();

			// calculate TransformInverse for AABB line interception in Local-Space
			// T^-1 = (TRS)^-1 = (S^-1) * (R^-1) * (T^-1) = (
			Matrix4x4 myScaleInverse, myRotationInverse, myTranslateInverse;
			myScaleInverse.SetScale(1 / box->m_scale);
			myRotationInverse = myRotation.Transposed();
			myTranslateInverse.SetTranslation(-box->m_position);	//Point operator-() const;
			
			//box->m_TransformInverse = myScaleInverse * myRotationInverse * myTranslateInverse;
			box->m_TransformInverse = box->m_Transform.InvertedTR(); 

			glm::mat4 mvp;
			if (box == m_Player_01)
			//if (false)
			{
				// 
				Vector BaseVecF = -m_Player_01->m_EAngle.ToVector();
				BaseVecF.y = 0;
				BaseVecF.Normalized();
				Vector BaseVecU(0.0f, 1.0f, 0.0f);
				Vector BaseVecR = BaseVecF.Cross(BaseVecU);

				// ---- My Matrix 4d operations
				Matrix4x4 myRUF(BaseVecR, BaseVecU, BaseVecF);
				myRUF = myRUF.Transposed();

				mvp = proj * view * model * myRUF.ToGlm();
			}
			else
				mvp = proj * view * model;

			m_ShaderMesh->SetUniformMat4f("u_mvp", mvp);
			m_ShaderMesh->SetUniformMat4f("u_model", model);
			m_ShaderMesh->SetUniformMat3f("u_transInvers_model", glm::mat3(glm::transpose(glm::inverse(model))));

			m_ShaderMesh->SetUniform3fv("u_Box_color", color);

			m_ShaderMesh->SetUniform3fv("u_lightColor", m_lightColor);
			m_ShaderMesh->SetUniform3fv("u_lightPos", m_lightPos);
			//m_ShaderMesh->SetUniform3fv("u_viewPos", m_camera->GetCamPosition());

			// Draw MESH
			m_mesh->Draw(m_ShaderMesh);
		}

	}

	void T01_FirstPrototipe_01::renderCube(Box* box, glm::vec3 color, glm::mat4 proj, glm::mat4 view, bool sprite_on)
	{

		//if (sprite_on)
		if (true)
		{
			m_ShaderMesh->Bind();
			// Model Matrix
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, box->GetPosVec3());
			model = glm::scale(model, box->GetScaleVec3());

			Matrix4x4 myScale, myRotation, myTranslate;
			myScale.SetScale(box->m_scale);
			myRotation.SetRotation(box->m_f_angle, box->m_v3_rotation);
			myTranslate.SetTranslation(box->m_position);
			box->m_Transform = myTranslate * myRotation * myScale;
			model = (box->m_Transform).ToGlm();

			// calculate TransformInverse for AABB line interception in Local-Space
			// T^-1 = (TRS)^-1 = (S^-1) * (R^-1) * (T^-1) = (
			Matrix4x4 myScaleInverse, myRotationInverse, myTranslateInverse;
			myScaleInverse.SetScale(1 / box->m_scale);
			myRotationInverse = myRotation.Transposed();
			myTranslateInverse.SetTranslation(-box->m_position);	//Point operator-() const;
			
			box->m_TransformInverse = myScaleInverse * myRotationInverse * myTranslateInverse;
			//box->m_TransformInverse = box->m_Transform.InvertedTR();


			glm::mat4 mvp;
			mvp = proj * view * model;

			m_ShaderMesh->SetUniformMat4f("u_mvp", mvp);
			m_ShaderMesh->SetUniformMat4f("u_model", model);
			m_ShaderMesh->SetUniformMat3f("u_transInvers_model", glm::mat3(glm::transpose(glm::inverse(model))));

			m_ShaderMesh->SetUniform3fv("u_Box_color", color);

			m_ShaderMesh->SetUniform3fv("u_lightColor", m_lightColor);
			m_ShaderMesh->SetUniform3fv("u_lightPos", m_lightPos);
			//m_ShaderMesh->SetUniform3fv("u_viewPos", m_camera->GetCamPosition());

			// Draw MESH
			m_mesh->Draw(m_ShaderMesh);
		}


	}


	void T01_FirstPrototipe_01::renderShot(Vector p_start, Vector p_end, glm::mat4 proj, glm::mat4 view)
	{
		float positions[] = {
			p_start.x, p_start.y, p_start.z,
			p_end.x, p_end.y, p_end.z,
		};

		VertexBuffer VertexBuffer(positions, 2 * (3) * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		VertexArray VAO;
		VAO.AddBuffer(VertexBuffer, layout);

		// Molde
		glm::mat4 model = glm::mat4(1.0f);

		m_ShaderLine->Bind();
		//MVP
		glm::mat4 mvp = proj * view * model;
		m_ShaderLine->SetUniformMat4f("u_mvp", mvp);

		//m_Shader = std::make_unique<Shader>("src/tests/01_Getting_started/05_Shader/S05_Shader_01.Shader");
		glm::vec3 color_shot = glm::vec3(1.0f, 0.f, 0.0f);
		m_ShaderLine->SetUniform3fv("u_lightColor", color_shot);

		VAO.Bind();
		glLineWidth(2.0);
		GLCall(glDrawArrays(GL_LINE_STRIP, 0, 2));

	}

	void T01_FirstPrototipe_01::renderBillboard(float flRadius, Vector vecUp, Vector vecRight, glm::mat4 proj, glm::mat4 view, glm::mat4 model)
	{

		vecUp = vecUp * flRadius;
		vecRight = vecRight * flRadius;

		Vector dl = - vecUp - vecRight;
		Vector dr = - vecUp + vecRight;
		Vector ur =  vecUp + vecRight;
		Vector ul =  vecUp - vecRight;

		float positions[] = {
			dl.x, dl.y, dl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			dr.x, dr.y, dr.z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			ur.x, ur.y, ur.z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			ul.x, ul.y, ul.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		unsigned int uint_quad_indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexBuffer VertexBuffer(positions, 4 * (3 + 3 + 2) * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		VertexArray VAO;
		VAO.AddBuffer(VertexBuffer, layout);
		IndexBuffer IndexBuffer(uint_quad_indices, 6);


		// Molde
		//glm::mat4 model = glm::mat4(1.0f);

		m_ShaderMesh->Bind();
		//MVP
		glm::mat4 mvp = proj * view * model;
		m_ShaderMesh->SetUniformMat4f("u_mvp", mvp);

		//m_Shader = std::make_unique<Shader>("src/tests/01_Getting_started/05_Shader/S05_Shader_01.Shader");
		glm::vec3 color_shot = glm::vec3(1.0f, 0.f, 0.0f);
		m_ShaderMesh->SetUniform3fv("u_Box_color", color_shot);


		msp_mTexture0->Bind(0);
		m_ShaderMesh->SetUniform1i("u_TexMap", 0);
		m_ShaderMesh->SetUniform1i("u_Image", 1);

		VAO.Bind();
		IndexBuffer.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));


		m_ShaderMesh->SetUniform1i("u_Image", 0);
	}

	// Trace a line through the world to simulate, eg, a bullet
	bool T01_FirstPrototipe_01::TraceLine(const Vector & v0, const Vector & v1, Vector & vecIntersection, Box*& pHit)
	{
		float flLowestFraction = 1;

		Vector vecTestIntersection;
		float flTestFraction;
		pHit = nullptr;

		// Check Box collision
		for (size_t i = 0; i < MAX_CHARACTERS; i++)
		{
			if (Box::m_apEntityList[i])
			{
				if (LineAABBIntersection(Box::m_apEntityList[i]->m_aabbSize, Box::m_apEntityList[i]->m_TransformInverse*v0, Box::m_apEntityList[i]->m_TransformInverse*v1, vecTestIntersection, flTestFraction) && flTestFraction < flLowestFraction)
				{
					vecIntersection = Box::m_apEntityList[i]->m_Transform * vecTestIntersection;
					flLowestFraction = flTestFraction;
					pHit = Box::m_apEntityList[i];
				}
			}
		}

		// Chack plane collision
		if (LinePlaneIntersection(Vector(0.0f, 1.0f, 0.0f), Point(0.0f, 0.0f, 0.0f), v0, v1, vecTestIntersection, flTestFraction) && flTestFraction < flLowestFraction)
		{
			vecIntersection = vecTestIntersection;
			flLowestFraction = flTestFraction;
			pHit = nullptr;
		}

		if (flLowestFraction < 1)
			return true;

		return false;
	}

	void T01_FirstPrototipe_01::fill_vertex_from_array(std::vector<Vertex>& vertices, float data[], unsigned int dSize, unsigned int layout[], unsigned int lSize)
	{

		glm::vec3 temp_pos, temp_norm;
		glm::vec2 temp_texC;

		int i_stride = 0;
		//for (float val : data)
		for (unsigned int i = 0; i < dSize; i++)
		{
			float val = data[i];
			switch (i_stride)
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
				vertices.emplace_back(temp_pos, temp_norm, temp_texC);
				i_stride = -1; // Restart counter...
				break;
			default:
				break;
			}
			i_stride++;
		}
	}

}
