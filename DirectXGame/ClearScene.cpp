#include "ClearScene.h"
#include "MyMath.h"
#include <numbers>
using namespace KamataEngine;
ClearScene::~ClearScene() {
	delete modelPlayer_;
	delete sprite_;
	delete fade_;
}
void ClearScene::Initialize() {
	// クリア用のモデル（タイトルフォントを再利用）
	modelPlayer_ = Model::CreateFromOBJ("player");
	textureHandle_ = TextureManager::Load("Resources/gameclear.png");

	camera_.Initialize();
	worldTransformClear_.Initialize();
	worldTransformClear_.scale_ = {2, 2, 2};
	worldTransformClear_.translation_ = {0, 8, 0};
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {10, 10, 10};
	worldTransformPlayer_.translation_ = {0, -8, 0};
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
	sprite_ = Sprite::Create(textureHandle_, {0, 0});
}
void ClearScene::Update() {
	switch (phase_) {
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
	worldTransformClear_.matWorld_ = MakeAffineMatrix(worldTransformClear_.scale_, worldTransformClear_.rotation_, worldTransformClear_.translation_);
	worldTransformClear_.TransferMatrix();
	// プレイヤーモデルを回転させる（勝利のポーズ）
	rotate_ += 0.1f;
	worldTransformPlayer_.rotation_.y = rotate_ + std::numbers::pi_v<float>;
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_, worldTransformPlayer_.translation_);
	worldTransformPlayer_.TransferMatrix();
}
void ClearScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Sprite::PreDraw(dxCommon->GetCommandList());
	Model::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	modelPlayer_->Draw(worldTransformPlayer_, camera_);
	Model::PostDraw();
	Sprite::PostDraw();
	fade_->Draw();
}
