#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;
// デストラクト
GameScene::~GameScene() {
	delete modelBlock_;
	delete player_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete modelPlayer_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	modelBlock_ = Model::CreateFromOBJ("block",true);

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	worldTransform_.Initialize();

	camera_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	player_ = new Player();
	player_->Initialize(modelPlayer_, &camera_);

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	const uint32_t KNumBlockVirtical = 10;
	const uint32_t KNumBlockHorizontal = 20;

	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	worldTransformBlocks_.resize(KNumBlockVirtical);

	for (uint32_t i = 0; i < KNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(KNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < KNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < KNumBlockHorizontal; ++j) {
			if ((i + j) % 2 == 0)
				continue;
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}
}

void GameScene::Update() {

	player_->Update();
	debugCamera_->Update();


#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

	

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			worldTransformBlock->TransferMatrix();
		}
	}
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	
	player_->Draw();

	skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}
	Model::PostDraw();
}