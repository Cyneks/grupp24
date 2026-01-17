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
	const std::string start_game_sfx {gResPath + "sounds/start_game.wav"};
	const std::string grass_bg {gResPath + "images/grass_bg.png"};
	const std::string player {gResPath + "images/Player.png"};
	const std::string player_death_sfx {gResPath + "sounds/player_death.wav"};
	const std::string player_victory_sfx {gResPath + "sounds/win.wav"};
	const std::string bullet {gResPath + "images/bullet.png"};
	const std::string attack_sfx {gResPath + "sounds/gunshot.wav"};
	const std::string wall_full_hp {gResPath + "images/wall.png"};
	const std::string wall_4_hp {gResPath + "images/wall_02.png"};
	const std::string wall_3_hp {gResPath + "images/wall_03.png"};
	const std::string wall_2_hp {gResPath + "images/wall_04.png"};
	const std::string wall_1_hp {gResPath + "images/wall_05.png"};
	const std::string wall_hit_sfx {gResPath + "sounds/wall_hit.wav"};
	const std::string wall_break_sfx {gResPath + "sounds/wall_break.wav"};
	const std::string enemy {gResPath + "images/GreenEnemy.png"};
	const std::string enemy_death_sfx {gResPath + "sounds/monster_death.wav"};
}

#endif