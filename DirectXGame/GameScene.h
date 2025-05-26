#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>
#include "MapChipField.h"

class GameScene {

	KamataEngine::Model* modelBlock_ = nullptr;

	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;

	Player* player_ = nullptr;

	bool isDebugCameraActive_ = false;

	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	Skydome* skydome_ = nullptr;

	MapChipField* mapChipField_;

public:
	~GameScene();

	void Initialize();

	void Update();

	void Draw();

	void GenerateBlocks();

};
