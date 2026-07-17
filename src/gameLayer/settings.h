#pragma once
#include <raylib.h>
#include <raymath.h>


struct Settings
{

	float musicVolume = 0.75;
	float masterVolume = 0.75;
	float soundVolume = 0.75;


	void sanitize() {
		soundVolume = Clamp(soundVolume, 0, 1);
		masterVolume = Clamp(masterVolume, 0, 1);
		musicVolume = Clamp(musicVolume, 0, 1);
	}


};

Settings& getSettings();