#pragma once

#include <vector>
#include <string>
#include <memory>

class Player;
struct GameState
{

	std::vector<std::unique_ptr<Player>> m_players;
};