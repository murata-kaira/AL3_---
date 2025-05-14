#include "Player.h"
#include "MyMath.h"

using namespace KamataEngine;


void Player::Initialize(Model* model,  Camera* camera)
{

	assert(model);

	camera_ = camera;

	model_ = model;

	worldTransform_.Initialize();

}


void Player::Update() { 
worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.
	rotation_, worldTransform_.translation_);

worldTransform_.TransferMatrix(); 


}

void Player::Draw() {

	model_->Draw(worldTransform_, *camera_);

}
