#pragma once
#include <KamataEngine.h>

class Player;

class CameraController {

public:
	void Initialize();

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	void SetMovableArea(Rect area) { movableArea_ = area; }

	const KamataEngine::Camera& GetViewProjection() const { return camera_; }

	// 強制スクロール設定
	void SetForcedScroll(bool enable) { isForcedScrollEnabled_ = enable; }
	void SetScrollSpeed(float speed) {
		// スクロール速度の範囲を制限 (0.0-1.0: ゲームのスケールに適した範囲)
		// 負の値は逆スクロールを防ぐため、1.0以上は過度に速い動きを防ぐため制限
		if (speed >= 0.0f && speed <= 1.0f) {
			scrollSpeed_ = speed;
		}
	}


private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera camera_;

	Player* target_ = nullptr;

	KamataEngine::Vector3 target0ffset_ = {0, 0, -15.0f};

	KamataEngine::Vector3 targetPosition_;

	static inline const float kInterpolationRate = 0.1f;

	static inline const float kVelocityBias = 10;

	Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};

	Rect movableArea_ = {0, 100, 0, 100};

	// 強制スクロール用パラメータ
	bool isForcedScrollEnabled_ = false;
	float scrollSpeed_ = 0.05f; // デフォルトのスクロール速度

};
