#include <Windows.h>
#include"KamataEngine.h"
#include"GameScene.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	KamataEngine::Initialize(L"LE2D_23_ムラタ_カイラ_AL3");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	GameScene* gameScene = new GameScene();

	gameScene->Initialize();


	while(true) {

		if (KamataEngine::Update()) {
		break;
		}

		gameScene->Update();

		dxCommon->PreDraw();


		gameScene->Draw();

		dxCommon->PostDraw();

	}


	KamataEngine::Finalize();


	delete gameScene;

	gameScene = nullptr;


	return 0;
}
