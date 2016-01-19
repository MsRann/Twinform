#pragma once

// This will just be some inlined helper functions for printing stuff
#include <SFML/System/Vector2.hpp>
#include <iostream>

inline void DebugPrint(const sf::Vector2f& v)
{
	std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
}

inline void DebugPrint(const sf::Vector2i& v)
{
	std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
}

inline void DebugPrint(const sf::FloatRect& r)
{
	std::cout << "(" << r.left << ", " << r.top << " - w: "<< r.width << " h: " << r.height << ")" << std::endl;
}