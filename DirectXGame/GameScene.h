#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>

class GameScene {

	uint32_t textureHandle_ = 0;

	KamataEngine::Model* modelBlock_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;

	Player* player_ = nullptr;

	bool isDebugCameraActive_ = false;

	KamataEngine::DebugCamera* debugCamera_ = nullptr;


	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

public:
	~GameScene();

	void Initialize();

	void Update();

	void Draw();
};
