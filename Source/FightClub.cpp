#pragma once
#include <FightClub.h>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <Core/Engine.h>
using namespace std;
using namespace glm;

FightClub::FightClub()
{
}

FightClub::~FightClub()
{
}

void FightClub::Init()
{
	const string firstPlayer = "Source/Textures/First_Player/";
	const string secondPlayer = "Source/Textures/Second_Player/";

	// Idle Animation
	for (size_t i = 0; i < 5; i++)
	{
		stringstream strs;
		strs << i;
		string str = strs.str();

		Texture2D* texture = new Texture2D();
		texture->Load2D((firstPlayer + "Idle_" + str + ".png").c_str(), GL_REPEAT);
		mapTextures["(1)Idle_" + str] = texture;
	}
	for (size_t i = 0; i < 4; i++)
	{
		stringstream strs;
		strs << i;
		string str = strs.str();

		Texture2D* texture = new Texture2D();
		texture->Load2D((secondPlayer + "Idle_" + str + ".png").c_str(), GL_REPEAT);
		mapTextures["(2)Idle_" + str] = texture;
	}
	
	// Move Animation
	for (size_t i = 0; i < 5; i++)
	{
		stringstream strs;
		strs << i;
		string str = strs.str();

		Texture2D* texture = new Texture2D();
		texture->Load2D((firstPlayer + "Move_" + str + ".png").c_str(), GL_REPEAT);
		mapTextures["(1)Move_" + str] = texture;
	}
	for (size_t i = 0; i < 5; i++)
	{
		stringstream strs;
		strs << i;
		string str = strs.str();

		Texture2D* texture = new Texture2D();
		texture->Load2D((secondPlayer + "Move_" + str + ".png").c_str(), GL_REPEAT);
		mapTextures["(2)Move_" + str] = texture;
	}

	// Hit Animation
	for (size_t i = 0; i < 4; i++)
	{
		stringstream strs;
		strs << i;
		string str = strs.str();

		Texture2D* texture = new Texture2D();
		texture->Load2D((firstPlayer + "Hit_" + str + ".png").c_str(), GL_REPEAT);
		mapTextures["(1)Hit_" + str] = texture;
	}
	for (size_t i = 0; i < 5; i++)
	{
		stringstream strs;
		strs << i;
		string str = strs.str();

		Texture2D* texture = new Texture2D();
		texture->Load2D((secondPlayer + "Hit_" + str + ".png").c_str(), GL_REPEAT);
		mapTextures["(2)Hit_" + str] = texture;
	}

	// Fall
	for (size_t i = 0; i < 4; i++)
	{
		stringstream strs;
		strs << i;
		string str = strs.str();

		Texture2D* texture = new Texture2D();
		texture->Load2D((firstPlayer + "Fall_" + str + ".png").c_str(), GL_REPEAT);
		mapTextures["(1)Fall_" + str] = texture;
	}
	for (size_t i = 0; i < 3; i++)
	{
		stringstream strs;
		strs << i;
		string str = strs.str();

		Texture2D* texture = new Texture2D();
		texture->Load2D((secondPlayer + "Fall_" + str + ".png").c_str(), GL_REPEAT);
		mapTextures["(2)Fall_" + str] = texture;
	}

	// Block Animation
	Texture2D* texture = new Texture2D();
	texture->Load2D((firstPlayer + "Block.png").c_str(), GL_REPEAT);
	mapTextures["(1)Block"] = texture;
	texture = new Texture2D();
	texture->Load2D((secondPlayer + "Block.png").c_str(), GL_REPEAT);
	mapTextures["(2)Block"] = texture;

	// Background
	texture = new Texture2D();
	texture->Load2D("Source/Textures/Background.jpg", GL_REPEAT);
	mapTextures["Background"] = texture;

	// Load mesh
	{
		Mesh* mesh = new Mesh("Plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple quad
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		vector<glm::vec2> textureCoords
		{
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1),
			glm::vec2(0, 0)
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("MyShader");
		shader->AddShader("Source/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Set players initial position
	pos_p1 = vec3(-1, -0.5, 0.1);
	pos_p2 = vec3(1, -0.5, 0.1);
}

void FightClub::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void FightClub::Update(float deltaTimeSeconds)
{
	// Draw Background
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = translate(modelMatrix, vec3(0, 1.25f, -0.5f));
	modelMatrix = scale(modelMatrix, vec3(0.1f));
	modelMatrix = rotate(modelMatrix, RADIANS(75), vec3(1, 0, 0));
	RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["Background"]);

	// Player 1
	if (IsCollision(pos_p1, pos_p2) && hit_2 && !block_1)
		SwitchToP1("falling");

	if (idle_1)
	{
		idle_ix1++;
		left_ix1 = right_ix1 = hit_ix1 = falling_ix1 = 0;
	}
	else if (left_1)
	{
		direction_1 = 1;
		left_ix1++;
		idle_ix1 = right_ix1 = hit_ix1 = falling_ix1 = 0;
	}
	else if (right_1)
	{
		direction_1 = 0;
		right_ix1++;
		idle_ix1 = left_ix1 = hit_ix1 = falling_ix1 = 0;
	}
	else if (hit_1)
	{
		hit_ix1++;
		idle_ix1 = left_ix1 = right_ix1 = falling_ix1 = 0;
	}
	else if (block_1)
	{
		idle_ix1 = left_ix1 = right_ix1 = hit_ix1 = falling_ix1 = 0;
	}
	else if (falling_1)
	{
		falling_ix1++;
		hit_ix1 = idle_ix1 = left_ix1 = right_ix1 = 0;
	}

	modelMatrix = glm::mat4(1);
	modelMatrix = translate(modelMatrix, pos_p1);
	modelMatrix = scale(modelMatrix, vec3(0.01f));
	modelMatrix = rotate(modelMatrix, RADIANS(75), vec3(1, 0, 0));

	if (direction_1)
		modelMatrix = rotate(modelMatrix, RADIANS(180), vec3(0, 0, -1));
	if (run_1) speed_1 = 1.5;
	else speed_1 = 1;

	if (idle_1)
	{
		if (idle_ix1 < 10)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Idle_0"]);
		else if (idle_ix1 < 20)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Idle_1"]);
		else if (idle_ix1 < 30)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Idle_2"]);
		else if (idle_ix1 < 40)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Idle_3"]);
		else if (idle_ix1 < 50)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Idle_4"]);

		if (idle_ix1 >= 49) idle_ix1 = 0;
	}
	else if (right_1)
	{
		if (!IsCollision(pos_p1, pos_p2))
			pos_p1.x += deltaTimeSeconds * 0.75f * speed_1;

		if (right_ix1 < 5 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_0"]);
		else if (right_ix1 < 10 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_1"]);
		else if (right_ix1 < 15 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_2"]);
		else if (right_ix1 < 20 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_3"]);
		else if (right_ix1 < 25 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_4"]);

		if (right_ix1 >= 24 / speed_1) right_ix1 = 0;
	}
	else if (left_1)
	{
		if (pos_p1.x > -2.1)
			pos_p1.x -= deltaTimeSeconds * 0.75f * speed_1;

		if (left_ix1 < 5 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_0"]);
		else if (left_ix1 < 10 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_1"]);
		else if (left_ix1 < 15 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_2"]);
		else if (left_ix1 < 20 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_3"]);
		else if (left_ix1 < 25 / speed_1)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Move_4"]);

		if (left_ix1 >= 24 / speed_1) left_ix1 = 0;
	}
	else if (hit_1)
	{
		if (hit_ix1 < 5)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Hit_0"]);
		else if (hit_ix1 < 10)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Hit_1"]);
		else if (hit_ix1 < 15)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Hit_2"]);
		else if (hit_ix1 < 20)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Hit_1"]);
		else if (hit_ix1 < 25)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Hit_0"]);

		if (hit_ix1 >= 24)
		{
			hit_ix1 = 0;
			SwitchToP1("idle");
		}
	}
	else if (block_1)
	{
		RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Block"]);
	}
	else if (falling_1)
	{
		if (falling_ix1 < 5)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Fall_0"]);
		else if (falling_ix1 < 10)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Fall_1"]);
		else if (falling_ix1 < 15)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Fall_2"]);
		else if (falling_ix1 < 20)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Fall_1"]);
		else if (falling_ix1 < 25)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Fall_0"]);
		else if (falling_ix1 < 30)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix, mapTextures["(1)Fall_3"]);

		if (falling_ix1 >= 29)
		{
			falling_ix1 = 0;
			SwitchToP1("idle");
		}
	}

	// Player 2
	if (IsCollision(pos_p1, pos_p2) && hit_1 && !block_2)
		SwitchToP2("falling");

	if (idle_2)
	{
		idle_ix2++;
		left_ix2 = right_ix2 = hit_ix2 = falling_ix2 = 0;
	}
	else if (left_2)
	{
		direction_2 = 0;
		left_ix2++;
		idle_ix2 = right_ix2 = hit_ix2 = falling_ix2 = 0;
	}
	else if (right_2)
	{
		direction_2 = 1;
		right_ix2++;
		idle_ix2 = left_ix2 = hit_ix2 = falling_ix2 = 0;
	}
	else if (hit_2)
	{
		hit_ix2++;
		idle_ix2 = left_ix2 = right_ix2 = falling_ix2 = 0;
	}
	else if (block_2)
	{
		idle_ix2 = left_ix2 = right_ix2 = hit_ix2 = falling_ix2 = 0;
	}
	else if (falling_2)
	{
		falling_ix2++;
		hit_ix2 = idle_ix2 = left_ix2 = right_ix2 = 0;
	}

	glm::mat4 modelMatrix2 = glm::mat4(1);
	modelMatrix2 = translate(modelMatrix2, pos_p2);
	modelMatrix2 = scale(modelMatrix2, vec3(0.01f));
	modelMatrix2 = rotate(modelMatrix2, RADIANS(75), vec3(1, 0, 0));
	modelMatrix2 = rotate(modelMatrix2, RADIANS(180), vec3(0, 0, -1));

	if (direction_2)
		modelMatrix2 = rotate(modelMatrix2, RADIANS(180), vec3(0, 0, -1));
	if (run_2) speed_2 = 1.5;
	else speed_2 = 1;

	if (idle_2)
	{
		if (idle_ix2 < 10)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Idle_0"]);
		else if (idle_ix2 < 20)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Idle_1"]);
		else if (idle_ix2 < 30)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Idle_2"]);
		else if (idle_ix2 < 40)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Idle_3"]);

		if (idle_ix2 >= 39) idle_ix2 = 0;
	}
	else if (right_2)
	{
		if(pos_p2.x < 2.1)
			pos_p2.x += deltaTimeSeconds * 0.75f * speed_2;

		if (right_ix2 < 5 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_0"]);
		else if (right_ix2 < 10 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_1"]);
		else if (right_ix2 < 15 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_2"]);
		else if (right_ix2 < 20 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_3"]);
		else if (right_ix2 < 25 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_4"]);

		if (right_ix2 >= 24 / speed_2) right_ix2 = 0;
	}
	else if (left_2)
	{
		if (!IsCollision(pos_p1, pos_p2))
			pos_p2.x -= deltaTimeSeconds * 0.75f * speed_2;

		if (left_ix2 < 5 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_0"]);
		else if (left_ix2 < 10 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_1"]);
		else if (left_ix2 < 15 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_2"]);
		else if (left_ix2 < 20 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_3"]);
		else if (left_ix2 < 25 / speed_2)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Move_4"]);

		if (left_ix2 >= 24 / speed_2) left_ix2 = 0;
	}
	else if (hit_2)
	{
		if (hit_ix2 < 5)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Hit_0"]);
		else if (hit_ix2 < 10)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Hit_1"]);
		else if (hit_ix2 < 15)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Hit_2"]);
		else if (hit_ix2 < 20)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Hit_3"]);
		else if (hit_ix2 < 25)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Hit_4"]);

		if (hit_ix2 >= 24)
		{
			hit_ix2 = 0;
			SwitchToP2("idle");
		}
	}
	else if (block_2)
	{
		RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Block"]);
	}
	else if (falling_2)
	{
		if (falling_ix2 < 5)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Fall_0"]);
		else if (falling_ix2 < 10)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Fall_1"]);
		else if (falling_ix2 < 15)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Fall_2"]);
		else if (falling_ix2 < 20)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Fall_1"]);
		else if (falling_ix2 < 25)
			RenderSimpleMesh(meshes["Plane"], shaders["MyShader"], modelMatrix2, mapTextures["(2)Fall_0"]);

		if (falling_ix2 >= 24)
		{
			falling_ix2 = 0;
			SwitchToP2("idle");
		}
	}
}

void FightClub::FrameEnd()
{
}

void FightClub::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	
	if (texture2)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}
	

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void FightClub::SwitchToP1(string state)
{
	if (state == "idle")
	{
		left_1 = false;
		right_1 = false;
		hit_1 = false;
		block_1 = false;
		falling_1 = false;
		idle_1 = true;		
	}
	else if (state == "left")
	{
		idle_1 = false;
		right_1 = false;
		hit_1 = false;
		block_1 = false;
		left_1 = true;
	}
	else if (state == "right")
	{
		idle_1 = false;
		left_1 = false;
		hit_1 = false;
		block_1 = false;
		right_1 = true;
	}
	else if (state == "hit")
	{
		left_1 = false;
		right_1 = false;
		idle_1 = false;
		block_1 = false;
		hit_1 = true;
	}
	else if (state == "block")
	{
		left_1 = false;
		right_1 = false;
		idle_1 = false;
		hit_1 = false;
		block_1 = true;
	}
	else if (state == "falling")
	{
		left_1 = false;
		right_1 = false;
		idle_1 = false;
		block_1 = false;
		hit_1 = false;
		falling_1 = true;
	}
}

void FightClub::SwitchToP2(string state)
{
	if (state == "idle")
	{
		left_2 = false;
		right_2 = false;
		hit_2 = false;
		block_2 = false;
		falling_2 = false;
		idle_2 = true;
	}
	else if (state == "left")
	{
		idle_2 = false;
		right_2 = false;
		hit_2 = false;
		block_2 = false;
		left_2 = true;
	}
	else if (state == "right")
	{
		idle_2 = false;
		left_2 = false;
		hit_2 = false;
		block_2 = false;
		right_2 = true;
	}
	else if (state == "hit")
	{
		left_2 = false;
		right_2 = false;
		idle_2 = false;
		block_2 = false;
		hit_2 = true;
	}
	else if (state == "block")
	{
		left_2 = false;
		right_2 = false;
		idle_2 = false;
		hit_2 = false;
		block_2 = true;
	}
	else if (state == "falling")
	{
		left_2 = false;
		right_2 = false;
		idle_2 = false;
		hit_2 = false;
		block_2 = false;
		falling_2 = true;
	}
}

bool FightClub::IsCollision(vec3 p1, vec3 p2)
{
	if (abs(p1.x - p2.x) < 0.4)
		return true;

	return false;
}

void FightClub::OnInputUpdate(float deltaTime, int mods)
{
}

void FightClub::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_LEFT_SHIFT)
		run_1 = true;
	if (key == GLFW_KEY_RIGHT_CONTROL)
		run_2 = true;

	if (key == GLFW_KEY_A && !hit_1)
		SwitchToP1("left");
	if (key == GLFW_KEY_D && !hit_1)
		SwitchToP1("right");
	if (key == GLFW_KEY_W && !hit_1)
		SwitchToP1("hit");
	if (key == GLFW_KEY_S && !hit_1)
		SwitchToP1("block");

	if (key == GLFW_KEY_LEFT && !hit_2)
		SwitchToP2("left");
	if (key == GLFW_KEY_RIGHT && !hit_2)
		SwitchToP2("right");
	if (key == GLFW_KEY_UP && !hit_2)
		SwitchToP2("hit");
	if (key == GLFW_KEY_DOWN && !hit_2)
		SwitchToP2("block");
}

void FightClub::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_LEFT_SHIFT)
		run_1 = false;
	if (key == GLFW_KEY_RIGHT_CONTROL)
		run_2 = false;

	if (key == GLFW_KEY_A && !hit_1)
		SwitchToP1("idle");
	if (key == GLFW_KEY_D && !hit_1)
		SwitchToP1("idle");
	if (key == GLFW_KEY_S && !hit_1)
		SwitchToP1("idle");

	if (key == GLFW_KEY_LEFT && !hit_2)
		SwitchToP2("idle");
	if (key == GLFW_KEY_RIGHT && !hit_2)
		SwitchToP2("idle");
	if (key == GLFW_KEY_DOWN && !hit_2)
		SwitchToP2("idle");
}

void FightClub::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void FightClub::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void FightClub::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void FightClub::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void FightClub::OnWindowResize(int width, int height)
{
}