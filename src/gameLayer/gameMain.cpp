#include <gameMain.h>
#include <raylib.h>
#include <raymath.h>
#include <assetManager.h>
#include <gameMap.h>
#include <worldGenerator.h>
#include <imgui.h>
#include <rlImGui.h>
#include <structure.h>
#include <saveMap.h>
#include <physics.h>
#include <player.h>
#include <iostream>
#include <zombie.h>
#include <unordered_map>
#include <map>
#include <items.h>
#include <utility>
#include <string>
using namespace std;


struct GameData {
	Camera2D camera;
	GameMap gameMap = {};

	int w = 5000;
	int h = 600;

	int selectedBlockType = Block::dirt;

	Vector2 selectionStart = { 0,0 };
	Vector2 selectionEnd = { 0,0 };

	Structure copyStructure;

	char saveName[100] = {};

	PlayerEntity player;
	Zombie zombie{ gameMap };
	vector<Item> dropedItems;
	map<int, pair<Item, int>> inventory;

}gameData;

AssetManager assetManager;
bool showimgui = false;
int selectedItemId = 0;
bool handEmpty = true;

bool initGame() {
	gameData.camera.target = { 100,120 };
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 100;
	//gameData.camera.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f + 50.f };

	assetManager.loadAll();

	gameData.gameMap.create(gameData.w, gameData.h);
	generateWorld(gameData.gameMap, 2);

	gameData.player.transform = { {100.5f,200.5f}, 0.8f, 1.8f };
	gameData.player.playerTex = assetManager.player;
	gameData.player.selectedBlock = 1;

	gameData.zombie.transform = { {105.5f,150.5f}, 0.8f, 1.8f };
	gameData.zombie.entityTex = assetManager.zombie;
	//gameData.zombie.gameMap = gameData.gameMap;
	gameData.zombie.teleport({ 120.7f, 200.5f });
	gameData.zombie.entityTex = assetManager.zombie;


	return true;
}


bool updateGame() {

	static float ballX = 100, ballY = 30;
	static float cameraZoom = 50;
	static int cameraSpeed = 10.f;
	static float jump = 10.5;
	static float up = 4.f;

	float deltaTime = GetFrameTime();
	deltaTime = Clamp(deltaTime, 0.f, 1 / 5.f);
	gameData.zombie.deltaTime = deltaTime;
	gameData.camera.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };
	BeginMode2D(gameData.camera);

	gameData.player.selectedBlock -= GetMouseWheelMove();
	gameData.player.selectedBlock = Clamp(gameData.player.selectedBlock, 1, 10);

	handEmpty = gameData.player.selectedBlock > (int)gameData.inventory.size();

	if (IsKeyPressed(KEY_F10)) showimgui = !showimgui;

	//if (IsKeyDown(KEY_W)) gameData.player.transform.pos.y -= cameraSpeed * deltaTime;
	//if (IsKeyDown(KEY_S)) gameData.player.transform.pos.y += cameraSpeed * deltaTime;


	//----zombie check -----
	gameData.zombie.entityBehaviour(gameData.player.transform);



	//------------player movement updates---------------

	bool downTouch = gameData.player.downTouch;
	if (IsKeyDown(KEY_A)) {
		if (downTouch) {
			if (handEmpty) {
				gameData.player.state = PlayerEntity::moving;
			}
			else {
				gameData.player.state = PlayerEntity::movingWithBlock;
			}
			
		}
		gameData.player.direction = 0;
		gameData.player.velocity.x = -cameraSpeed;
	}

	if (IsKeyDown(KEY_D)) {
		if (downTouch) {
			if (handEmpty) {
				gameData.player.state = PlayerEntity::moving;
			}
			else {
				gameData.player.state = PlayerEntity::movingWithBlock;
			}

		}
		gameData.player.direction = 1;
		gameData.player.velocity.x = cameraSpeed;
	}

	if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
		gameData.player.velocity.x = 0;
	}

	if (IsKeyPressed(KEY_SPACE)) {
		gameData.player.state = PlayerEntity::jumping;
		if (downTouch) gameData.player.velocity.y = -jump;
	}

	if (gameData.player.downTouch && !IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyPressed(KEY_SPACE)) {
		gameData.player.state = handEmpty == true ? PlayerEntity::standing : PlayerEntity::standingWithBlock;
	}

	//-----------------------------------------------------------------------------------------



	if (IsKeyPressed(KEY_Z)) cameraZoom += 5;
	if (IsKeyPressed(KEY_X)) cameraZoom -= 5;
	if (IsKeyPressed(KEY_UP)) cameraSpeed += 5;
	if (IsKeyPressed(KEY_DOWN)) cameraSpeed -= 5;

	cameraSpeed = Clamp(cameraSpeed, 1, 100);
	cameraZoom = Clamp(cameraZoom, 1, 100);


	gameData.camera.zoom = cameraZoom;

	//updating player pos and camera pos ->
	gameData.player.updateForces(deltaTime);
	gameData.player.checkCollisionOnce(gameData.gameMap, gameData.player.transform.pos);
	gameData.camera.target = { gameData.player.transform.pos.x ,gameData.player.transform.pos.y - up };
	gameData.player.updateFinal();


	Vector2 topLeftView = GetScreenToWorld2D({ 0,0 }, gameData.camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, gameData.camera);

	int startViewX = floorf(topLeftView.x - 1);
	int endViewX = ceilf(bottomRightView.x - 1);
	int startViewY = floorf(topLeftView.y - 1);
	int endViewY = ceilf(bottomRightView.y - 1);

	startViewX = Clamp(startViewX, 0, gameData.w);
	endViewX = Clamp(endViewX, 0, gameData.w);
	startViewY = Clamp(startViewY, 0, gameData.h);
	endViewY = Clamp(endViewY, 0, gameData.h);

	Vector2 cursorPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = cursorPos.x;
	int blockY = cursorPos.y;

	//std::cout << blockX << " " << blockY << std::endl;

	if (!showimgui) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			int type = gameData.gameMap.getBlock(blockX, blockY).type;
			if (type != Block::air) {
				Item item{ gameData.gameMap };
				item.type = type;
				item.transform.pos = vec{ (float)blockX + 0.5f, (float)blockY + 0.5f };
				item.transform.w = 0.7f; item.transform.h = 0.8f;
				gameData.dropedItems.push_back(item);
			}
			gameData.gameMap.getBlock(blockX, blockY).type = Block::air;
		}
		Transform2D block = { {blockX + 0.5, blockY + 0.5}, 1, 1 };
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !gameData.player.transform.intersectTransform(block) && gameData.gameMap.getBlock(blockX,blockY).type == Block::air && !handEmpty) {
			gameData.gameMap.getBlock(blockX, blockY).type = gameData.selectedBlockType;
			gameData.inventory[gameData.selectedBlockType].second--;
			if (gameData.inventory[gameData.selectedBlockType].second == 0) {
				gameData.inventory.erase(gameData.selectedBlockType);
				handEmpty = true;
			}
		}
	}

	if (showimgui) {
		if (IsKeyPressed(KEY_ONE)) {
			gameData.selectionStart = Vector2{ (float)blockX, (float)blockY };
		}
		if (IsKeyPressed(KEY_TWO)) {
			gameData.selectionEnd = Vector2{ (float)blockX, (float)blockY };
		}

		if (IsKeyPressed(KEY_THREE)) {
			gameData.copyStructure.pasteIntoMap(gameData.gameMap, Vector2{ (float)blockX, (float)blockY });
		}


		if (gameData.selectionStart.x > gameData.selectionEnd.x) {
			std::swap(gameData.selectionStart.x, gameData.selectionEnd.x);
		}

		if (gameData.selectionStart.y > gameData.selectionEnd.y) {
			std::swap(gameData.selectionStart.y, gameData.selectionEnd.y);
		}


	}




	ClearBackground(WHITE);

	//background loading 

	// After loading the texture
	SetTextureWrap(assetManager.forestBg, TEXTURE_WRAP_REPEAT);
	float parallaxFactor = 0.5f;
	static float down = 18;
	static float zoom = 4.5f; // > 1.0 = zoom out, < 1.0 = zoom in

	float bgW = GetScreenWidth() * zoom;
	float bgH = GetScreenHeight() * zoom;

	float bgX = gameData.camera.target.x * parallaxFactor - bgW * 0.5f;
	float bgY = gameData.camera.target.y * parallaxFactor - bgH * 0.5f - down;

	DrawTexturePro(
		assetManager.forestBg,
		{
			bgX,
			bgY,
			bgW,
			bgH
		},
	{
		gameData.camera.target.x - GetScreenWidth() * 0.5f,
		gameData.camera.target.y - GetScreenHeight() * 0.5f,
		(float)GetScreenWidth(),
		(float)GetScreenHeight()
	},
		{ 0, 0 },
		0.0f,
		WHITE
	);



	for (int x = startViewX; x <= endViewX; x++) {
		for (int y = startViewY; y <= endViewY; y++) {

			auto& b = gameData.gameMap.getBlock(x, y);

			if (b.type == Block::air) {
				continue;
			}

			Rectangle rec = { (float)b.type * 32.0f, 0.0f, 32.0f,32.0f };

			DrawTexturePro(
				assetManager.textures,
				rec,
				{ (float)x, (float)y, 1, 1 },
				{ 0.f,0.f },
				0.f,
				WHITE
			);
		}
	}

	if (showimgui) {
		Rectangle rect;
		rect.x = gameData.selectionStart.x;
		rect.y = gameData.selectionStart.y;
		rect.width = gameData.selectionEnd.x - gameData.selectionStart.x + 1;
		rect.height = gameData.selectionEnd.y - gameData.selectionStart.y + 1;

		DrawRectangleLinesEx(rect, 0.1, { 20,101,250,255 });
	}

	DrawTexturePro(
		assetManager.frame,
		{ 0,0,(float)assetManager.frame.width, (float)assetManager.frame.height },
		{ (float)blockX, (float)blockY, 1,1 },
		{ 0,0 },
		0.f,
		WHITE
	);

	for (auto it = gameData.dropedItems.begin(); it != gameData.dropedItems.end(); ) {
		it->dropPrint(deltaTime, assetManager.texturesbg);
		float dx = it->transform.pos.x - gameData.player.transform.pos.x;
		float dy = it->transform.pos.y - gameData.player.transform.pos.y;
		float distSq = dx * dx + dy * dy;

		

		if (gameData.player.transform.intersectTransform(it->transform)) {
			if (gameData.inventory.find(it->type) != gameData.inventory.end()) {
				gameData.inventory[it->type].second++;
				it = gameData.dropedItems.erase(it);
			}
			else if (gameData.inventory.size() < 10) {
				gameData.inventory[it->type].first = *it;
				gameData.inventory[it->type].second++;
				it = gameData.dropedItems.erase(it);
			}
			else {
				++it;
			}
		}
		else if (distSq > 25 * 25) { 
			it = gameData.dropedItems.erase(it);
		}
		else {
			++it;
		}
	}

	if (IsKeyPressed(KEY_TAB)) {
		for (auto& [k, v] : gameData.inventory) {
			cout << k << " " << v.first.type << " " << v.second << endl;
		}
	}


	gameData.zombie.entityAnimation();
	gameData.player.entityAnimation();
	DrawRectangleLinesEx(gameData.player.transform.getAABB(), 0.1, BLANK); //player aabb


	if (!handEmpty) {

		auto it = gameData.inventory.begin(); 
		for (int i = 0; i < gameData.player.selectedBlock-1; i++) {
			it++;
		}
		gameData.selectedBlockType = it->second.first.type;

		Rectangle rec;
		if (gameData.player.state != PlayerEntity::jumping) {
			if (gameData.player.direction == 1) {
				rec = Rectangle{ gameData.player.transform.getRight().x, gameData.player.transform.getRight().y, 0.3f, 0.3f };
			}
			else {
				rec = Rectangle{ gameData.player.transform.getLeft().x - 0.20f, gameData.player.transform.getLeft().y, 0.3f, 0.3f };
			}
		}

		else {
			if (gameData.player.direction == 1) {
				rec = Rectangle{ gameData.player.transform.getTopLeft().x, gameData.player.transform.getTopLeft().y + 0.10f, 0.3f, 0.3f };
			}
			else {
				rec = Rectangle{ gameData.player.transform.getTopRight().x - 0.20f, gameData.player.transform.getTopRight().y + 0.10f, 0.3f, 0.3f };
			}
		}


		DrawTexturePro(
			assetManager.texturesbg,
			{it->second.first.type*32.f, 0, 32.f, 32.f},
			rec,
			{0.f,0.f},
			0.f,
			WHITE
		);
	}

	


	EndMode2D();


	//inventoryDrawing:
	float inventoryWidth = 0.40f*GetScreenWidth()/10.f;
	float inventoryHeight = inventoryWidth;
	auto itemIt = gameData.inventory.begin();
	for (int i = 0; i < 10; i++) {
		DrawTexturePro(
			assetManager.frame,
			{ 0.f, 0.f, (float)assetManager.frame.width, (float)assetManager.frame.height },
			{0.30f*GetScreenWidth() + i*inventoryWidth, GetScreenHeight() - inventoryHeight, inventoryHeight, inventoryWidth},
			{ 0.f, 0.f },
			0.f,
			WHITE
		);

		if (itemIt != gameData.inventory.end()) {
			DrawTexturePro(
				assetManager.texturesbg,
				{ itemIt->second.first.type * 32.f, 0.f , 32.f, 32.f },
				{ 0.30f * GetScreenWidth() + i * inventoryWidth + 10.f, GetScreenHeight() - inventoryHeight + 10.f, inventoryHeight - 20.f, inventoryWidth -20.f },
				{ 0.f, 0.f },
				0.f,
				WHITE
			);
			DrawTextPro(
				GetFontDefault(),
				to_string(itemIt->second.second).c_str(),
				{ 0.30f * GetScreenWidth() + (i+1) * inventoryWidth +  -30.f, GetScreenHeight() - 30.f},
				{ 0, 0 },
				0.0f,
				20.0f,
				2.0f,
				WHITE
			);

			++itemIt;
		}
	}
	


	DrawRectangleLinesEx(
		{ 0.30f * GetScreenWidth() + (gameData.player.selectedBlock-1) * inventoryWidth , GetScreenHeight() - inventoryHeight , inventoryHeight , inventoryWidth },
		5,
		BLACK
	);
	


	if (showimgui) {

		ImGui::Begin("test");
		ImGui::SliderFloat("up", &up, 0, 10);
		ImGui::SliderFloat("jump", &jump, 10, 50);
		ImGui::SliderFloat("up/down", &down, 10.f, 100.f);
		ImGui::SliderFloat("zoom in/out", &zoom, 0, 5);
		if (ImGui::Button("Copy")) {
			gameData.copyStructure.copyFromMap(gameData.gameMap, gameData.selectionStart, gameData.selectionEnd);
		}
		ImGui::InputText("File name", gameData.saveName, sizeof(gameData.saveName));

		if (ImGui::Button("Save to File")) {
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";

			saveBlockDataToFile(gameData.copyStructure.mapData, gameData.copyStructure.w, gameData.copyStructure.h, path.c_str());
		}

		if (ImGui::Button("Load From File")) {
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";

			loadBlockDataFromFile(gameData.copyStructure.mapData, gameData.copyStructure.w, gameData.copyStructure.h, path.c_str());
		}

		ImGui::Separator();

		for (int i = 1; i < Block::BLOCKS_COUNT; i++) {
			Rectangle atlas{ (float)i * 32.f, 0.f, 32.f, 32.f };
			atlas.x /= assetManager.texturesbg.width;
			atlas.width /= assetManager.texturesbg.width;
			atlas.y /= assetManager.texturesbg.height;
			atlas.height /= assetManager.texturesbg.height;

			ImGui::PushID(i);
			ImTextureID tex = (ImTextureID)(intptr_t)assetManager.texturesbg.id;

			if (ImGui::ImageButton(tex, { 35,35 }, { atlas.x, atlas.y }, { atlas.x + atlas.width, atlas.y + atlas.height })) {
				gameData.selectedBlockType = i;
			}

			ImGui::PopID();

			if (i % 10 != 0)
				ImGui::SameLine();
		}
		ImGui::End();

	}



	DrawFPS(10, 10);
	return true;
}

bool closeGame() {
	return true;
}