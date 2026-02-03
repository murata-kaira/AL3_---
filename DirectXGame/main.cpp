#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "ClearScene.h"
#include <Windows.h>

using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
GameOverScene* gameOverScene = nullptr;
ClearScene* clearScene = nullptr;


enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kGameOver,
	kClear,
};

Scene scene = Scene::kUnknown;

void ChangeScene() {
	switch (scene) { 
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
		
			scene = Scene::kGame;

			delete titleScene;
			titleScene = nullptr;

			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFnished()) {
			// ゲームクリアかゲームオーバーかを判定
			if (gameScene->IsGameClear()) {
				scene = Scene::kClear;

				delete gameScene;
				gameScene = nullptr;

				clearScene = new ClearScene;
				clearScene->Initialize();
			} else {
				scene = Scene::kGameOver;

				delete gameScene;
				gameScene = nullptr;

				gameOverScene = new GameOverScene;
				gameOverScene->Initialize();
			}
		}
		break;
	case Scene::kGameOver:
		if (gameOverScene->IsFinished()) {
			scene = Scene::kTitle;

			delete gameOverScene;
			gameOverScene = nullptr;

			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	case Scene::kClear:
		if (clearScene->IsFinished()) {
			scene = Scene::kTitle;

			delete clearScene;
			clearScene = nullptr;

			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	}

}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kGameOver:
		gameOverScene->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameOver:
		gameOverScene->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	}
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	KamataEngine::Initialize(L"LE2D_23_ムラタ_カイラ_AL3");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	scene = Scene::kTitle;
	titleScene =  new TitleScene;
	titleScene->Initialize();


	while (true) {

		if (KamataEngine::Update()) {
			break;
		}

		ChangeScene();

		UpdateScene();

		dxCommon->PreDraw();

		DrawScene();

		dxCommon->PostDraw();
	}

	delete gameScene;
	delete titleScene;
	delete gameOverScene;
	delete clearScene;

	KamataEngine::Finalize();

	return 0;
}
