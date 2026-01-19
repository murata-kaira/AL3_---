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
	delete mapChipField_;
	delete modelEnemy_;
	delete modelGolf_;
	delete golf_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	for (Enemy* enemy : enemies_) {

		delete enemy;
	}
}

void GameScene::Initialize() {

	modelBlock_ = Model::CreateFromOBJ("block", true);

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	modelGolf_ = Model::CreateFromOBJ("block", true); // Reuse block model for golf ball

	debugCamera_ = new DebugCamera(1280, 720);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);

	player_ = new Player();

	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(6 + i, 18);

		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	// Initialize golf ball
	golf_ = new Golf();
	Vector3 golfPosition = mapChipField_->GetMapChipPositionByIndex(15, 18);
	golfPosition.y += 2.0f; // Start above ground
	golf_->Initialize(modelGolf_, &camera_, golfPosition);
	
	// Set goal position
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(80, 18);
	golf_->SetGoalPosition(goalPosition);

	worldTransform_.Initialize();

	camera_.Initialize();

	GenerateBlocks();
}

void GameScene::Update() {

	player_->Update();
	debugCamera_->Update();
	cameraController_->Update();

	if (golf_) {
		golf_->Update();
	}

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

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
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		camera_.TransferMatrix();
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			worldTransformBlock->TransferMatrix();
		}
	}

	CheckAllCollisions();
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	player_->Draw();

	skydome_->Draw();

	if (golf_) {
		golf_->Draw();
	}

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}
	Model::PostDraw();

	// Draw UI elements
	DrawUI();
}

void GameScene::DrawUI() {
	// Draw shot counter
	if (golf_) {
		char shotText[64];
		sprintf_s(shotText, "SHOTS: %d", golf_->GetShotCount());
		DebugText::GetInstance()->Print(shotText, 20, 20, 2.0f);

		// Draw distance if a shot has been taken
		if (golf_->GetShotCount() > 0) {
			char distText[64];
			sprintf_s(distText, "DISTANCE: %.1f", golf_->GetLastShotDistance());
			DebugText::GetInstance()->Print(distText, 20, 50, 2.0f);
		}

		// Draw power meter when charging
		if (golf_->IsCharging()) {
			float power = golf_->GetChargePower();
			char powerText[64];
			sprintf_s(powerText, "POWER: %.1f", power);
			DebugText::GetInstance()->Print(powerText, 20, 80, 2.0f);

			// Draw power bar
			int barWidth = static_cast<int>(power / 2.0f * 200.0f);  // Max 200 pixels
			for (int i = 0; i < barWidth; i += 10) {
				DebugText::GetInstance()->Print("=", 20 + i, 110, 2.0f);
			}
		}

		// Draw direction indicator when near ball
		AABB playerAABB = player_->GetAABB();
		AABB ballAABB = golf_->GetAABB();
		if (IsCollision(playerAABB, ballAABB)) {
			DebugText::GetInstance()->Print("< HIT >", 20, 140, 2.0f);
			DebugText::GetInstance()->Print("Hold SPACE to charge power", 20, 170, 1.5f);
		}
	}
}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
	#pragma region
	AABB aabb1, aabb2;

	aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_) {
		aabb2 = enemy->GetAABB();

		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);

			enemy->OnCollision(player_);
		}
	}

	// Check collision between player and golf ball
	if (golf_ && !golf_->IsInHole()) {
		aabb2 = golf_->GetAABB();
		if (IsCollision(aabb1, aabb2)) {
			// Start charging when space key is pressed
			if (Input::GetInstance()->TriggerKey(DIK_SPACE) && !golf_->IsCharging()) {
				golf_->StartCharging();
			}
			// Release to hit with charged power
			else if (!Input::GetInstance()->PushKey(DIK_SPACE) && golf_->IsCharging()) {
				golf_->Hit(player_, golf_->GetChargePower());
			}
		}
	}
	#pragma endregion
}
