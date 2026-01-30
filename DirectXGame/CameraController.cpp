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

	// 強制スクロール - カメラを自動的に右方向へ移動
	camera_.translation_.x += kAutoScrollSpeed;

	// プレイヤーの位置を参考にY座標を更新
	camera_.translation_.y = targetWorldTransform.translation_.y + target0ffset_.y;
	camera_.translation_.z = targetWorldTransform.translation_.z + target0ffset_.z;

	// プレイヤーとカメラの相対位置を制限
	float playerCameraDistance = targetWorldTransform.translation_.x - camera_.translation_.x;
	if (playerCameraDistance < targetMargin.left) {
		camera_.translation_.x = targetWorldTransform.translation_.x - targetMargin.left;
	}
	if (playerCameraDistance > targetMargin.right) {
		camera_.translation_.x = targetWorldTransform.translation_.x - targetMargin.right;
	}
	 
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
