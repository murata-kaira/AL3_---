#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class Player;
class Golf {

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	void Hit(const Player* player);

	KamataEngine::Vector3 GetWorldPosition();

	bool IsInHole() const { return isInHole_; }

	void SetGoalPosition(const KamataEngine::Vector3& goalPosition) { goalPosition_ = goalPosition; }

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3 velocity_ = {};

	KamataEngine::Vector3 goalPosition_ = {};

	bool isInHole_ = false;

	static inline const float kGravityAcceleration = 0.05f;

	static inline const float kFriction = 0.98f;

	static inline const float kBallRadius = 0.5f;

	static inline const float kGoalRadius = 0.5f;

	static inline const float kGroundHeight = 1.0f;

	static inline const float kBounceDamping = 0.6f;

	void ApplyPhysics();

	void CheckGoal();
};
