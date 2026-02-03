#include "GameOver.h"
#include "MyMath.h"
#include <numbers>
using namespace KamataEngine;
GameOverScene::~GameOverScene() {
	delete sprite_;
	delete modelPlayer_;
	delete fade_;
}
void GameOverScene::Initialize() {
	// ゲームオーバー用のモデル（タイトルフォントを再利用）

	modelPlayer_ = Model::CreateFromOBJ("player");
	textureHandle_ = TextureManager::Load("titleFont/gameover.png");

	

	camera_.Initialize();
	worldTransformGameOver_.Initialize();
	worldTransformGameOver_.scale_ = {2, 2, 2};
	worldTransformGameOver_.translation_ = {0, 8, 0};
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {10, 10, 10};
	worldTransformPlayer_.translation_ = {0, -8, 0};
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
	sprite_ = Sprite::Create(textureHandle_, {0, 0});
}
void GameOverScene::Update() {
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
	worldTransformGameOver_.matWorld_ = MakeAffineMatrix(worldTransformGameOver_.scale_, worldTransformGameOver_.rotation_, worldTransformGameOver_.translation_);
	worldTransformGameOver_.TransferMatrix();
	// プレイヤーモデルを少し動かす（死んだ状態を表現）
	worldTransformPlayer_.rotation_.x = std::numbers::pi_v<float> / 2.0f;
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_, worldTransformPlayer_.translation_);
	worldTransformPlayer_.TransferMatrix();
}
void GameOverScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	fade_->Draw();

	Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();

	Model::PreDraw(dxCommon->GetCommandList());
	modelPlayer_->Draw(worldTransformPlayer_, camera_);
	Model::PostDraw();




	Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();

	
}
