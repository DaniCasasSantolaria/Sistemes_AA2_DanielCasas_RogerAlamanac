#pragma once
#include "NodeMap.h"
#include "../Objects/Object.h"
#include "../Enemies/Enemy.h"
#include "../Objects/Chest.h"
#include "../Enemies/SpawnerEnemies.h"
#include "../Objects/SpawnerChests.h"

class WorldMap
{
private:
	Player* player = nullptr;
	NodeMap* _currentMap = nullptr;
	std::vector<NodeMap*> _maps;
	std::vector<Object*> _objects;
	std::vector<Enemy*> _enemies;
	std::vector<Chest*> _chests;
	int currentMapNumber = 4;

public:
	SpawnerEnemies spawnerEnemies;
	SpawnerChests spawnerChests;
	WorldMap() = default;
	void PrintCurrentMap(NodeMap* currentMap);
	void PrintNewMap(NodeMap* newMap);
	void SetMap();
	NodeMap* GetCurrentMap() const { return _currentMap; }
	std::vector<NodeMap*> GetMaps() const { return _maps; }
	std::vector<Object*> GetObjects() const {return _objects; }
	std::vector<Enemy*> GetEnemies() const { return _enemies; }
	std::vector<Chest*> GetChests() const { return _chests; }
	Player* GetPlayer() const { return player; }
	int GetCurrentMapNumber() const { return currentMapNumber; }
};

