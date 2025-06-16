#pragma once
#include "KamataEngine.h"

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

		const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }



private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kAcceleration = 1.0;

	static inline const float kAttenuation = 0.5;

	static inline const float kLimitRunSpeed = 0.5;

	static inline const float kTimeTurn = 0.3f;

	static inline const float kGravityAcceleration = 0.1f;

	static inline const float kLimitFallSpeed = 1.0;

	static inline const float kJumpAcceleration = 1.0f;


	bool onGround_ = true;

	enum class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
};
