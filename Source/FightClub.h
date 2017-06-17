#pragma once
#include <vector>
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
using namespace glm;
using namespace std;

class FightClub : public SimpleScene
{
public:
	FightClub();
	~FightClub();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	void SwitchToP1(string state);
	void SwitchToP2(string state);
	bool IsCollision(vec3 p1, vec3 p2);

	std::unordered_map<std::string, Texture2D*> mapTextures;

	int idle_ix1, left_ix1, right_ix1, hit_ix1, falling_ix1;
	int idle_ix2, left_ix2, right_ix2, hit_ix2, falling_ix2;

	int direction_1, direction_2;
	float speed_1 = 1;
	float speed_2 = 1;

	bool idle_1 = true;
	bool idle_2 = true;
	bool left_1, right_1, hit_1, block_1, run_1, falling_1;
	bool left_2, right_2, hit_2, block_2, run_2, falling_2;
	vec3 pos_p1, pos_p2;
};
