#pragma once
#include "Fade.h"
#include "KamataEngine.h"
class GameOverScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};
	~GameOverScene();
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; }

private:
	KamataEngine::Model* modelGameOver_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransformGameOver_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	bool finished_ = false;
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
};