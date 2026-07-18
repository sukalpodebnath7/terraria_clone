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
#include <helper.h>
#include <recipeBook.h>
#include <randomFuncs.h>
#include <audio.h>
#include <score.h>
#include <ui.h>
#include <ctime>
using namespace std;

std::ranlux24_base rng(69);


#include <gameData.h>

GameData gameData;

Zombie* zombie;
vector<Zombie*> zombies;

AssetManager assetManager;
Score score;
bool showimgui = false;
int selectedItemId = 0;
bool handEmpty = true;
float transition = 0.f;
bool changing = false;
bool showRecipe = false;
vector<int> recipeItems = { 70,71,72,73,74,75,76,78,79,80,81,82,83,84, 89,90,91,92,93,100,103,106,107,108 };
//matrix size 4x6


bool initGame() {

	Audio::init();
	//zombie = new Zombie{ gameData.gameMap, gameData.player };

	gameData.camera.target = { 100,120 };
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 100;
	//gameData.camera.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f + 50.f };

	assetManager.loadAll();

	rng.seed(time(NULL));

	gameData.gameMap.create(gameData.w, gameData.h);
	generateWorld(gameData.gameMap, getRandomInt(rng, 1, 1000000));

	gameData.player.transform = { {100.5f,200.5f}, 0.8f, 1.8f };
	gameData.player.playerTex = assetManager.player;
	gameData.player.selectedBlock = 1;
	gameData.player.lastDownTouch = gameData.player.transform.pos.y;

	//zombie->transform = { {105.5f,150.5f}, 0.8f, 1.8f };
	//zombie->entityTex = assetManager.zombie;
	//gameData.zombie.gameMap = gameData.gameMap;
	//zombie->teleport({ 120.7f, 200.5f });
	//zombie->entityTex = assetManager.zombie;

	gameData.bg.resize(4); // 0-> forest, 1 -> mountain, 2-> desert, 3 -> ice
	gameData.bg = { 0,0,0,0 };
	gameData.prevBg = assetManager.forestBg;

	return true;
}

void resetGame() {
	for (auto& z : zombies) {
		if (z != nullptr) delete z;
	}
	zombies.clear();

	gameData.dropedItems.clear();
	gameData.inventory.clear();

	score.reset();

	gameData.gameMap.create(gameData.w, gameData.h);
	generateWorld(gameData.gameMap, getRandomInt(rng, 1, 1000000));

	gameData.player.transform.pos = {100.5f, 200.5f};
	gameData.player.velocity = {0, 0};
	for (int i = 0; i < 5; i++) {
		gameData.player.health[i] = 20.f;
	}
	gameData.player.takenDamage = 0.f;
	gameData.player.selectedBlock = 1;
	gameData.player.lastDownTouch = gameData.player.transform.pos.y;
	gameData.player.state = PlayerEntity::standing;
	
	handEmpty = true;
}

void spawnZombie(vector<Zombie*>& zombies, PlayerEntity& player, GameMap& gameMap, Texture2D tex) {
	if (zombies.size() == 10) return;

	if (getRandomFloat(rng, 0, 1) < 0.0004f) {
		Zombie* z = new Zombie{ gameMap, player };
		int texDetect = getRandomInt(rng, -1, 1); // -1 -> zombie, 0 -> mummy , 1-> iceZombie
		switch (texDetect) {
		case -1:
			tex = assetManager.zombie;
			break;
		case 0:
			tex = assetManager.mummy;
			break;
		case 1:
			tex = assetManager.iceZombie;
			break;
		}
		z->entityTex = tex;
		z->transform.w = 0.8f;
		z->transform.h = 1.8f;
		float d = getRandomInt(rng, -1, 1);
		if (d == 0) d = -1;
		int y = 0;
		int zw = getRandomInt(rng, 30, 50);
		z->transform.pos.x = player.transform.pos.x + d * zw;

		for (; y < gameData.gameMap.h; y++) {
			if (gameData.gameMap.getBlock(z->transform.pos.x, y).type != Block::air) {
				break;
			}
		}

		z->transform.pos.y = y-2;
		z->teleport(z->transform.pos);

		zombies.push_back(z);
	}
}

void despawnZombie(vector<Zombie*>& zombies, PlayerEntity& player) {
	for (auto& it : zombies) {
		if (it != nullptr) {
			if (abs(it->transform.pos.x - player.transform.pos.x) >= 50.f) {
				it = nullptr;
			}
		}
	}
	for (int i = 0; i < zombies.size(); i++) {
		if (zombies[i] == nullptr) {
			zombies.erase(zombies.begin() + i);
			i--;
		}
	}
}

bool updateGame() {

	Audio::update();

	if (currentGameState == GameState::MainMenu) {
		DrawMainMenu();
		return true;
	}

	if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
		if (currentGameState == GameState::Playing) {
			currentGameState = GameState::Paused;
		} else if (currentGameState == GameState::Paused) {
			currentGameState = GameState::Playing;
		}
	}

	if (currentGameState == GameState::Paused) {
		DrawPauseMenu();
		return true;
	}

	//-------------------------zombie spawning ---------------------------
	spawnZombie(zombies, gameData.player, gameData.gameMap, assetManager.zombie);
	despawnZombie(zombies, gameData.player);

	if (handEmpty) gameData.selectedBlockType = 0;
	gameData.player.inHandBlock = gameData.selectedBlockType;
	gameData.player.updateHealth();
	updateInventory(gameData.inventory);
	static float ballX = 100, ballY = 30;
	static float cameraZoom = 50;
	static int cameraSpeed = 10.f;
	static float jump = 10.5;
	static float up = 4.f;

	float deltaTime = GetFrameTime();
	deltaTime = Clamp(deltaTime, 0.f, 1 / 5.f);
	if(zombie!= nullptr) zombie->deltaTime = deltaTime;
	gameData.camera.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };
	BeginMode2D(gameData.camera);

	gameData.player.selectedBlock -= GetMouseWheelMove();
	gameData.player.selectedBlock = Clamp(gameData.player.selectedBlock, 1, 10);

	handEmpty = gameData.player.selectedBlock > (int)gameData.inventory.size();

	if (IsKeyPressed(KEY_F10)) showimgui = !showimgui;

	//if (IsKeyDown(KEY_W)) gameData.player.transform.pos.y -= cameraSpeed * deltaTime;
	//if (IsKeyDown(KEY_S)) gameData.player.transform.pos.y += cameraSpeed * deltaTime;


	//----zombie check -----
	

	for (int i = 0; i < zombies.size(); i++) {
		auto& zombie = zombies[i];
		if (zombie != nullptr) {
			zombie->deltaTime = deltaTime;
			zombie->entityBehaviour(gameData.player.transform);
			zombie->entityAnimation();
			zombie->entityAttacked();

			if (zombie->entityHealth <= 0.f) {
				Audio::playSound(Audio::entityDeath);
				score.onEnemyKill();
				delete zombie;
				zombie = nullptr;
				zombies.erase(zombies.begin() + i);
				i--;
			}
		}
	}
	



	//------------ player movement updates ---------------

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

	if (gameData.player.downTouch && !IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
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



	//--------------Player Health Update ------------------------

	if (gameData.player.downTouch == 1) {
		float delta =  -gameData.player.lastDownTouch + gameData.player.transform.pos.y;
		if(delta >= 5.f)
			gameData.player.takenDamage = (delta / 5) * 7;

		gameData.player.lastDownTouch = gameData.player.transform.pos.y;
		//if(delta != 0.f) std::cout << delta << std::endl;
	}

	
	//std::cout << gameData.player.health[0] << std::endl;







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

	if (!showimgui && !showRecipe) {

// --------------------- Block Breaking and Item Creation Logic ------------------------------
		Block blk = gameData.gameMap.getBlock(blockX, blockY);
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

			if (gameData.selectedBlockType >= 70) {
				gameData.player.state = PlayerEntity::attacking;
			}
			else{
				int type = blk.type;
				if (type != Block::air) {
					Audio::playSound(Audio::breakBlock);
					Item item{ gameData.gameMap };
					item.type = type;
					if (type == Block::leaves) {
						std::ranlux24_base rng(69);
						if (getRandomFloat(rng, 0, 1) <= 1) {
							item.type = Tool::Apple;
						}
					}
					item.transform.pos = vec{ (float)blockX + 0.5f, (float)blockY + 0.5f };
					item.transform.w = 0.7f; item.transform.h = 0.8f;
					gameData.dropedItems.push_back(item);
				}
				gameData.gameMap.getBlock(blockX, blockY).type = Block::air;
			}
			
		}
		Transform2D block = { {blockX + 0.5, blockY + 0.5}, 1, 1 };
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			if (gameData.selectedBlockType >= 70) {
				toolFunction(gameData.selectedBlockType, gameData.player, gameData.inventory, false);
			}

			else if(!gameData.player.transform.intersectTransform(block) && (gameData.gameMap.getBlock(blockX, blockY).type == Block::air || !blk.isCollidable()) && !handEmpty){
				Audio::playSound(Audio::placeBlock);
				gameData.gameMap.getBlock(blockX, blockY).type = gameData.selectedBlockType;
				gameData.inventory[gameData.selectedBlockType].second--;
				if (gameData.inventory[gameData.selectedBlockType].second == 0) {
					gameData.inventory.erase(gameData.selectedBlockType);
					handEmpty = true;
				}
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
	static float down = -347.625;
	static float zoom = 30.750f; // > 1.0 = zoom out, < 1.0 = zoom in

	float bgW = GetScreenWidth() * zoom;
	float bgH = GetScreenHeight() * zoom;

	float bgX = gameData.camera.target.x * parallaxFactor - bgW * 0.5f;
	float bgY = gameData.camera.target.y * parallaxFactor - bgH * 0.5f - down;




	//----------- Drawing the Background --------------

	gameData.bg = { 0,0,0,0 };
	int val;
	int count = setBg(gameData.bg, gameData.gameMap, startViewX, startViewY, endViewX, endViewY, val);
	Texture2D bgTex;

	if (val == 0) {
		bgTex = gameData.prevBg;
	}

	else {
		//0->forest, 1->mountain, 2->desert, 3->ice

		switch (count) {
		case 0:
			bgTex = assetManager.forestBg;
			break;
		case 1:
			bgTex = assetManager.mountainBg;
			break;
		case 2:
			bgTex = assetManager.desertBg;
			break;
		case 3:
			bgTex = assetManager.snowBg;
			break;
		}

		gameData.prevBg = bgTex;
	}
	switch (count)
	{
	case 0:
		Audio::playMusic(Audio::forestMusic);
		break;

	case 1:
		Audio::playMusic(Audio::mountainMusic);
		break;

	case 2:
		Audio::playMusic(Audio::desertMusic);
		break;

	case 3:
		Audio::playMusic(Audio::snowMusic);
		break;
	}



	DrawTexturePro(
		bgTex,
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





	// -------------- Selection Logic --------------------------
	if (showimgui) {
		Rectangle rect;
		rect.x = gameData.selectionStart.x;
		rect.y = gameData.selectionStart.y;
		rect.width = gameData.selectionEnd.x - gameData.selectionStart.x + 1;
		rect.height = gameData.selectionEnd.y - gameData.selectionStart.y + 1;

		DrawRectangleLinesEx(rect, 0.1, { 20,101,250,255 });
	} 

	// -----------------------------------------------------------

	//------------------------- Frame Drawing -----------------------------

	DrawTexturePro(
		assetManager.frame,
		{ 0,0,(float)assetManager.frame.width, (float)assetManager.frame.height },
		{ (float)blockX, (float)blockY, 1,1 },
		{ 0,0 },
		0.f,
		WHITE
	);
	//---------------------------------------------------------------------

	// ----------------------- Dropped Block to Inventory Logic ---------------------------
	for (auto it = gameData.dropedItems.begin(); it != gameData.dropedItems.end(); ) {
		Texture2D tex = assetManager.texturesbg;
		if (it->type >= 70) tex = assetManager.tools;
		it->dropPrint(deltaTime, tex);
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


	

	// --------------------------- Block In Hand Drawing ---------------------------------
	static float toolSize = 0.f;
	static float toolY = 0;
	static float toolX = 0;
	float ex = 0;
	float ej = 0.f;
	float exx = 0.20f;
	if (!handEmpty) {

		auto it = gameData.inventory.begin(); 
		for (int i = 0; i < gameData.player.selectedBlock-1; i++) {
			it++;
		}
		gameData.selectedBlockType = it->second.first.type;
		
		Texture2D inTex = assetManager.texturesbg;
		int selectType = gameData.selectedBlockType;

		if (gameData.selectedBlockType >= 70) {
			
			selectType -= 70;
			inTex = assetManager.tools;
			toolSize = 0.294f;
			toolY = -0.418;
			toolX = -0.100f;
			ex = 0.30f;
			ej = 0.50f;
			exx = 0.20f;
		}
		else {
			toolSize = 0;
			toolY = 0;
			toolX = 0;
			ex = 0.20f;
			ej = 0.20f;
			exx = 0;
		}

		Rectangle rec;
		if (gameData.player.state != PlayerEntity::jumping && gameData.player.state != PlayerEntity::attacking) {
			if (gameData.player.direction == 1) {
				rec = Rectangle{ gameData.player.transform.getRight().x + toolX, gameData.player.transform.getRight().y + toolY, 0.3f + toolSize, 0.3f  + toolSize};
			}
			else {
				rec = Rectangle{ gameData.player.transform.getLeft().x - ex + toolX, gameData.player.transform.getLeft().y + toolY, 0.3f + toolSize, 0.3f + toolSize};
			}
		}
		else if (gameData.player.state == PlayerEntity::attacking) {
			if (gameData.player.tempState != PlayerEntity::jumping) {
				if (gameData.player.direction == 1) {
					rec = Rectangle{ gameData.player.transform.getRight().x + toolX, gameData.player.transform.getRight().y + toolY, 0.3f + toolSize, 0.3f + toolSize };
				}
				else {
					rec = Rectangle{ gameData.player.transform.getLeft().x - ex + toolX, gameData.player.transform.getLeft().y + toolY, 0.3f + toolSize, 0.3f + toolSize };

				}
			}
			else {
				Audio::playSound(Audio::attcking);
				if (gameData.player.direction == 1) {
					rec = Rectangle{ gameData.player.transform.getTopLeft().x + toolX + exx , gameData.player.transform.getTopLeft().y + 0.10f + toolY, 0.3f + toolSize, 0.3f + toolSize };
				}
				else {
					rec = Rectangle{ gameData.player.transform.getTopRight().x - ej + toolX , gameData.player.transform.getTopRight().y + 0.10f + toolY, 0.3f + toolSize, 0.3f + toolSize };
				}
			}
			
		}
		else {
			if (gameData.player.direction == 1) {
				rec = Rectangle{ gameData.player.transform.getTopLeft().x + toolX +  exx , gameData.player.transform.getTopLeft().y + 0.10f + toolY, 0.3f + toolSize, 0.3f + toolSize };
			}
			else {
				rec = Rectangle{ gameData.player.transform.getTopRight().x - ej + toolX , gameData.player.transform.getTopRight().y + 0.10f + toolY, 0.3f + toolSize, 0.3f + toolSize};
			}
		}

		Rectangle src;
		if (gameData.player.direction == 1) {
			src = { selectType * 32.f, 0.f, 32.f, 32.f };
		}
		else {
			src = { (selectType) * 32.f, 0.f, -32.f, 32.f };
		}

		DrawTexturePro(
			inTex,
			src,
			rec,
			{0.f,0.f},
			0.f,
			WHITE
		);
	}


	// ---------------------------------- Player Drawing ----------------------------

	for (auto& zombie : zombies) {
		if (zombie != nullptr) zombie->entityAnimation();
	}
	gameData.player.entityAnimation();
	DrawRectangleLinesEx(gameData.player.transform.getAABB(), 0.1, BLANK); //player aabb

	


	EndMode2D();

	
	if (IsKeyPressed(KEY_C)) showRecipe = !showRecipe;


	//---------------------------Recipe Book Drawing -------------------------------
	if (showRecipe) {
		DrawTexturePro(
			assetManager.recipeBook,
			{ 0.f,0.f, (float)assetManager.recipeBook.width, (float)assetManager.recipeBook.height },
			{ 10.f, 10.f, GetScreenWidth() - 20.f, GetScreenHeight() - 20.f },
			{ 0.f,0.f },
			0.f,
			WHITE
		);

		float startX = 0.12f * GetScreenWidth();
		float diffX = 0.01 * GetScreenWidth();
		float itemWidth = (GetScreenWidth() - 2*startX - diffX) / 6.f - (15 * (GetScreenWidth()/1920.f));

		float startY = 0.20f * GetScreenHeight();
		float diffY = 0.05 * GetScreenHeight();
		float itemHeight = (GetScreenHeight() - 2*startY - diffY) / 4.f;

		DrawTextPro(
			GetFontDefault(),
			"RECIPE BOOK",
			Vector2{0.38f * GetScreenWidth(), 0.1f * GetScreenHeight()},
			{ 0, 0 },
			0.0f,
			50.0f,
			20.0f,
			BLACK

		);

		//std::cout << GetScreenWidth() << std::endl;
		int toolCount = 0;
		for (int y = 0; y < 4; y++) {
			float posY = startY + diffY * y + itemHeight * y;
			for (int x = 0; x < 6; x++) {	
				float posX = startX + diffX * x + itemWidth * x;
				Color boundary = { 63, 21, 0, 128 };
				Vector2 mousePos = GetMousePosition();

				if (mousePos.x >= posX + 5 && mousePos.x <= posX + itemWidth - 5 && mousePos.y >= posY + 5 && mousePos.y <= posY + itemHeight - 5) {
					if (isBuilable(gameData.inventory, recipeItems[toolCount])) {
						boundary = GREEN;
						if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
							buildTool(gameData.inventory, recipeItems[toolCount], gameData.gameMap);
						}
					}
					else {
						boundary = RED;
					}
				}

				DrawRectangleRoundedLines(
					{ posX, posY, itemWidth, itemHeight },
					0.2f,
					16,
					3,
					 boundary 
				);


				float posItemX = posX;
				float posItemY = posY;
				DrawTexturePro(
					assetManager.tools,
					{(float)(recipeItems[toolCount] - 70)*32.f, 0.f, 32.f, 32.f},
					{posItemX + 20, posItemY + 5, itemWidth/3.f, itemHeight/2.f},
					{0.f,0.f},
					0.f,
					WHITE
				);
				float d = 1;
				for (int i = 0; i < recipeBook[recipeItems[toolCount]].size(); i++) {
					pair<int, int> block = recipeBook[recipeItems[toolCount]][i];
					DrawTexturePro(
						assetManager.textures,
						{block.first*32.f, 0.f, 32.f, 32.f},
						{(float)(posItemX + i* (itemWidth/3.f) + 1) + 5, (float)(posItemY + itemHeight/2.f + 10 ), itemWidth / 6.f, itemHeight / 4.f },
						{0.f,0.f},
						0.f,
						WHITE
					);

					DrawTextPro(
						GetFontDefault(),
						to_string(block.second).c_str(),
						Vector2{ (float)(posItemX + i * (itemWidth / 3.f) + 1) + itemWidth/6.f, (float)(posItemY + itemHeight / 2.f + 10) + itemHeight/4.f},
						{ 0, 0 },
						0.0f,
						20.0f,
						5.0f,
						BLACK

					);
				}

				


				toolCount++;


			}
		}
	}







	//------------------------inventoryDrawing---------------------------------------
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
			Texture2D inTex = assetManager.texturesbg;
			int itemType = itemIt->second.first.type;
			if (itemIt->second.first.type >= 70) {
				itemType -= 70;
				inTex = assetManager.tools;
			}
			DrawTexturePro(
				inTex,
				{ itemType * 32.f, 0.f , 32.f, 32.f },
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
		WHITE
	);


	//---------- Heart Drawing ---------------

	float heartWidth = 0.40 * GetScreenWidth() / 15.f;
	float heartHeight = heartWidth;
	float startHeart = GetScreenWidth() - 5 * heartWidth;
	int chooseHeart = 0;


	for (int i = 0; i < 5; i++) {
		if (gameData.player.health[i] > 10.f) chooseHeart = 0;
		else if (gameData.player.health[i] > 0.f) chooseHeart = 1;
		else chooseHeart = 2;

		DrawTexturePro(
			assetManager.hearts,
			{chooseHeart*32.f, 0.f, 32.f, 32.f },
			{startHeart+i*heartWidth, 10.f, heartWidth, heartHeight},
			{0.f, 0.f},
			0.f,
			WHITE
		);
	}
	
	int scoreTextWidth = MeasureText(TextFormat("Score: %d", score.totalScore), 20);
	DrawText(TextFormat("Score: %d", score.totalScore), startHeart + (5 * heartWidth) / 2.0f - scoreTextWidth / 2.0f, 10.f + heartHeight + 10.f, 20, WHITE);
	


	if (showimgui) {

		ImGui::Begin("test");
		ImGui::SliderFloat("toolSize", &toolSize, 0.f, 1.f);
		ImGui::SliderFloat("toolY", &toolY, -1.f, 1.f);
		ImGui::SliderFloat("toolX", &toolX, -1.f, 1.f);
		ImGui::SliderFloat("up", &up, 0, 50);
		ImGui::SliderFloat("jump", &jump, 10, 50);
		ImGui::SliderFloat("up/down", &down, -500.f, 500.f);
		ImGui::SliderFloat("zoom in/out", &zoom, -30, 50);
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

	Audio::stopMusic();
	return true;
}
