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

	}

	void update() {

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
}