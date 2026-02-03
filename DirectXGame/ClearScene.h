#pragma once

#include "KamataEngine.h"
#include "Fade.h"

class ClearScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~ClearScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

private:
	KamataEngine::Model* modelClear_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransformClear_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	float rotate_ = 0.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;
};
