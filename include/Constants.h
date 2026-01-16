#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace constants
{
    // Constants
	constexpr int gScreenWidth     { 1080 };
   	constexpr int gScreenHeight    { 720 };
   	// PATH TO YOUR RESOURCE FOLDER 'resources', 'gResPath'
   	const std::string gResPath { "./resources/" };
	// PATH'S TO ALL YOUR EXTERNAL RESOURCES using 'gResPath'
	const std::string grass {gResPath + "images/grass_bg.png"};
	const std::string wall_1 {gResPath + "images/wall.png"};
	const std::string wall_2 {gResPath + "images/wall_02.png"};
	const std::string wall_3 {gResPath + "images/wall_03.png"};
	const std::string wall_4 {gResPath + "images/wall_04.png"};
	const std::string wall_5 {gResPath + "images/wall_05.png"};
	const std::string green_enemy {gResPath + "images/GreenEnemy.png"};
	const std::string bullet {gResPath + "images/bullet.png"};
	const std::string player {gResPath + "images/Player.png"};
	const std::string start_game {gResPath + "sounds/start_game.wav"};
	const std::string gunshot {gResPath + "sounds/gunshot.wav"};
	const std::string monster_death {gResPath + "sounds/monster_death.wav"};
	const std::string player_death {gResPath + "sounds/player_death.wav"};
	const std::string wall_hit {gResPath + "sounds/wall_hit.wav"};
	const std::string wall_break {gResPath + "sounds/wall_break.wav"};
	const std::string win_sound {gResPath + "sounds/win.wav"};
}

#endif