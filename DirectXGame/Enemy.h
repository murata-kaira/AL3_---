#pragma once
#include "KamataEngine.h"

class MapChipField;
class Enemy {

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }


private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;
	
	MapChipField* mapChipField_ = nullptr;

	KamataEngine::Vector3 velocity_ = {};


	float walkTimer_ = 0.0f;


	static inline const float kWalkSpeed = 0.1f;

	
};
