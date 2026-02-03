#include "CameraController.h"
#include "Player.h"

using namespace KamataEngine;
using namespace MathUtility;

void CameraController::Initialize() { camera_.Initialize(); }

void CameraController::Update() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	targetPosition_ = targetWorldTransform.translation_ + target0ffset_;

	const Vector3& targetVelocity = target_->GetVelocity();
	targetPosition_ = targetWorldTransform.translation_ + targetVelocity * kVelocityBias;

		// 強制スクロールが有効な場合
	if (isForcedScrollEnabled_) {
		// カメラを自動的に右方向へスクロール
		camera_.translation_.x += scrollSpeed_;
		// プレイヤーも一緒にスクロール
		target_->ApplyScrollMovement(scrollSpeed_);
		// Y軸はプレイヤーに追従
		camera_.translation_.y = Lerp(camera_.translation_.y, targetPosition_.y, kInterpolationRate);
	} else {
		// 通常のプレイヤー追従スクロール
		camera_.translation_ = targetWorldTransform.translation_ + target0ffset_;
		camera_.translation_.x = Lerp(camera_.translation_.x, targetPosition_.x, kInterpolationRate);
		camera_.translation_.y = Lerp(camera_.translation_.y, targetPosition_.y, kInterpolationRate);
	}


	camera_.translation_.x = max(camera_.translation_.x, camera_.translation_.x + targetMargin.left);
	camera_.translation_.x = min(camera_.translation_.x, camera_.translation_.x + targetMargin.right);
	camera_.translation_.y = max(camera_.translation_.y, camera_.translation_.y + targetMargin.bottom);
	camera_.translation_.y = min(camera_.translation_.y, camera_.translation_.y + targetMargin.top);
	 
	camera_.translation_.x = max(camera_.translation_.x,movableArea_.left);
	camera_.translation_.x = min(camera_.translation_.x, movableArea_.right);
	camera_.translation_.y = max(camera_.translation_.y, movableArea_.bottom);
	camera_.translation_.y = min(camera_.translation_.y, movableArea_.top);


	camera_.UpdateMatrix();
}

void CameraController::Reset() {

	const KamataEngine::WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	camera_.translation_ = targetWorldTransform.translation_ + target0ffset_;
}
