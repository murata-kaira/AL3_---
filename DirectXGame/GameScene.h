#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>
#include"Fade.h"



class GameScene {


	KamataEngine::Model* modelBlock_ = nullptr;

	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Model* modelEnemy_ = nullptr;

	KamataEngine::Model* modelDeathParticles_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;

	Player* player_ = nullptr;

	bool isDebugCameraActive_ = false;


	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	std::list<Enemy*> enemies_;

	Skydome* skydome_ = nullptr;

	MapChipField* mapChipField_;

	CameraController* cameraController_;

	DeathParticles* deathParticles_ = nullptr;


	enum class Phase {
		kPlay,
		kDeath,
		kFadeIn,
		kFadeOut
	};

	Phase phase_;

	bool finished_ = false;

	Fade* fade_ = nullptr;


public:
	~GameScene();

	void Initialize();

	void Update();

	void Draw();

	void GenerateBlocks();

	void CheckAllCollisions();

	void ChangePhase();

	bool IsFnished() const { return finished_; }
};
