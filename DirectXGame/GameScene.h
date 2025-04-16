#pragma once
#include "KamataEngine.h"

class GameScene {

	uint32_t textureHandle_ = 0;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;

	public:

	~GameScene();

	void Initialize();


	void Update();


	void Draw();





};


