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
	const std::string red_wall_1 {gResPath + "images/RedWall.png"};
	const std::string red_wall_2 {gResPath + "images/RedWall_02.png"};
	const std::string red_wall_3 {gResPath + "images/RedWall_03.png"};
	const std::string red_wall_4 {gResPath + "images/RedWall_04.png"};
	const std::string red_wall_5 {gResPath + "images/RedWall_05.png"};
	const std::string green_enemy {gResPath + "images/GreenEnemy.png"};
	const std::string bullet {gResPath + "images/bullet.png"};
	const std::string player {gResPath + "images/football.png"};
}

#endif