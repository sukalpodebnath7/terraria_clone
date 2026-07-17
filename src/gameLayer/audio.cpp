#include <audio.h>
#include <settings.h>
#include <raymath.h>
#include <vector>

namespace Audio {

	void init() {
		InitAudioDevice();
		SetMasterVolume(0.9);

		loadAllMusicAndSounds();
	}
	std::vector<Music> allMusic;
	int currentMusic = -1;

	std::vector<Sound> allSounds;

	void loadAllMusicAndSounds() {

		auto loadSound = [&](const char* path) {

			Sound s;
			s = LoadSound(path);

			if (s.stream.buffer) {
				allSounds.push_back(s);
			}
			else {
				allSounds.push_back({});
			}
		};

		allSounds.push_back({});
		loadSound(RESOURCES_PATH "sounds/place.ogg");
		loadSound(RESOURCES_PATH "sounds/break.ogg");
		loadSound(RESOURCES_PATH "sounds/HImpact3.ogg");
		loadSound(RESOURCES_PATH "sounds/ButtonPress.ogg");
		loadSound(RESOURCES_PATH "sounds/BackButton.ogg");
		loadSound(RESOURCES_PATH "sounds/eating.ogg");
		loadSound(RESOURCES_PATH "sounds/zombie.ogg");
		loadSound(RESOURCES_PATH "sounds/FinalHit3.ogg");
		loadSound(RESOURCES_PATH "sounds/crafting.ogg");
		loadSound(RESOURCES_PATH "sounds/attacking.ogg");

		allMusic.resize(MUSIC_COUNT);

		allMusic[forestMusic] = LoadMusicStream(RESOURCES_PATH "music/forestBG.mp3");
		allMusic[desertMusic] = LoadMusicStream(RESOURCES_PATH "music/desertBG.mp3");
		allMusic[mountainMusic] = LoadMusicStream(RESOURCES_PATH "music/mountainBG.mp3");
		allMusic[snowMusic] = LoadMusicStream(RESOURCES_PATH "music/snowBG.mp3");
		allMusic[bossMusic] = LoadMusicStream(RESOURCES_PATH "music/boss_fightBG.mp3");

	}

	void update() {
			if (currentMusic != -1)
				UpdateMusicStream(allMusic[currentMusic]);
		

	}


	void playSound(int sound, float volume) {
		if (sound <= noneSound || sound >= SOUNDS_COUNT) {
			return;
		}

		volume = Clamp(volume, 0, 1);

		volume *= getSettings().masterVolume * getSettings().masterVolume;
		volume *= getSettings().soundVolume * getSettings().soundVolume;

		SetSoundVolume(allSounds[sound], volume);
		PlaySound(allSounds[sound]);
	}
	void playMusic(int music)
	{
		if (music == currentMusic)
			return;

		if (currentMusic != -1)
			StopMusicStream(allMusic[currentMusic]);

		currentMusic = music;

		PlayMusicStream(allMusic[currentMusic]);
	}
	void stopMusic()
	{
		if (currentMusic != -1)
		{
			StopMusicStream(allMusic[currentMusic]);
		}

		for (auto& m : allMusic)
			UnloadMusicStream(m);

		CloseAudioDevice();
	}
}
