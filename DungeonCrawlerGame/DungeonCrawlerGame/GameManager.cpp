#include "GameManager.h"
#include "../InputManager/InputManager.h"
#include "Timer/Timer.h"
#include "ConsoleControl/ConsoleControl.h"
#include "Enemies/SpawnerEnemies.h"
#include "Objects/SpawnerChests.h"
#include "Objects/SpawnerObjects.h"
#include <fstream>
#include <iostream>

GameManager::GameManager() {
    srand(time(NULL));
    int positionX = rand() % (9 - 1 + 1) + 1;
    int positionY = rand() % (9 - 1 + 1) + 1;
    player = new Player();
    for (int i = -10; i < 30; i += 10) {
        for (int j = -10; j < 30; j += 10)
            maps.push_back(new NodeMap(Vector2(11, 11), Vector2(j, i)));
    }
    currentMapNumber = 4;
    currentMap = maps[currentMapNumber];
    enemies.push_back(SpawnerEnemies::SpawnEnemy(currentMap));
    enemies.push_back(SpawnerEnemies::SpawnEnemy(currentMap));

    objects.push_back(SpawnerObjects::SpawnObject(currentMap));
    objects.push_back(SpawnerObjects::SpawnObject(currentMap));

    chests.push_back(SpawnerChests::SpawnChest(currentMap));
    chests.push_back(SpawnerChests::SpawnChest(currentMap));
}

Json::Value GameManager::CodeEnemies() {
    Json::Value json = Json::Value();
    Json::Value enemiesArray(Json::arrayValue);
    for (Enemy* enemy : enemies)
        enemiesArray.append(enemy->Code());
    json["enemies"] = enemiesArray;
    return json;
}

Json::Value GameManager::CodeMaps() {
    Json::Value json = Json::Value();
    Json::Value mapsArray(Json::arrayValue);
    for (NodeMap* map : maps)
        mapsArray.append(map->Code());
    json["grids"] = mapsArray;
    return json;
}

void GameManager::Code() {
    Json::Value jsonArray = Json::Value(Json::arrayValue);
    jsonArray.append(player->Code());
    jsonArray.append(CodeEnemies());
    jsonArray.append(CodeMaps());
    jsonArray.append(currentMap->Code());

    try {
        std::ofstream jsonWriteFile = std::ofstream("DungeonCrawlerGame.json", std::ifstream::binary);

        if (!jsonWriteFile.fail()) {
            jsonWriteFile << jsonArray;
            jsonWriteFile.close();
        }
        else {
            throw std::exception("ERROR AL ABRIR EL ARCHIVO JSON");
        }
    }
    catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
}

void GameManager::Decode() {
    std::ifstream jsonReadFile = std::ifstream("DungeonCrawlerGame.json", std::ifstream::binary);

    if (!jsonReadFile.fail()) {
        Json::Value readedJson;
        jsonReadFile >> readedJson;

        if (readedJson[0].isMember("player")) {
            player->Decode(readedJson[0]["player"]);
        }

        if (readedJson[1].isMember("enemies")) {
            for (Json::Value value : readedJson[1]["enemies"]) {
                Enemy* e = new Enemy();
                e->Decode(value["enemy"]);
                enemies.push_back(e);
            }
        }

        if (readedJson[2].isMember("grids")) {
            for (Json::Value value : readedJson[2]["grids"]) {
                if (value.isMember("grid") && value["grid"].isArray()) {
                    NodeMap* m = new NodeMap(Vector2(11, 11), Vector2(0, 0));
                    m->Decode(value);
                    maps.push_back(m);
                }
            }
        }

        if (readedJson[3].isMember("currentMap")) {
            currentMap->Decode(readedJson["currentMap"]);
        }

        jsonReadFile.close();
    }
}

void GameManager::PrintNewMap() {
    system("cls");
    currentMap->Draw();
    player->Draw();
    for (Object* object : objects) {
        object->Draw();
    }
    for (Chest* c : chests) {
        c->Draw();
    }
    for (Enemy* e : enemies) {
        e->Draw();
    }
    Print();
}

void GameManager::CheckPortals() {
    Vector2 pos = player->GetPosition();
    int nextMap = 0;
    switch (player->GetPlayerState()) {
    case Player::PlayerState::DOWN:
        nextMap = 3;
        pos += Vector2(0, 1);
        break;
    case Player::PlayerState::LEFT:
        nextMap = -1;
        pos += Vector2(-1, 0);
        break;
    case Player::PlayerState::RIGHT:
        nextMap = 1;
        pos += Vector2(1, 0);
        break;
    case Player::PlayerState::UP:
        nextMap = -3;
        pos += Vector2(0, -1);
        break;
    }
    currentMap->SafePickNode(pos, [this, nextMap](Node* node) {
        if (node->GetINodeContent()->GetContent() == NodeContent::PORTAL) {
            if(currentMapNumber + nextMap >= 0 && currentMapNumber + nextMap < 9)
                currentMap = maps[currentMapNumber + nextMap];
            //Falta asignar els portals correctament als mapas
        }
        });
}

void GameManager::Print() {

    CC::Lock();
    CC::SetPosition(currentMap->GetSize().x + 10, 0);
    std::cout << "Monedas: " << player->GetCoins();
    CC::SetPosition(currentMap->GetSize().x + 10, 1);
    std::cout << "Vidas: " << player->GetLifes();
    CC::SetPosition(currentMap->GetSize().x + 10, 2);
    std::cout << "Pociones: " << player->GetAmountPotions();
    CC::SetPosition(currentMap->GetSize().x + 10, 3);
    /*std::cout << "Pociones: " << player->GetWeapon();*/ //Falta hacer un enum per poder fer el cout
    CC::Unlock();
    CC::Lock();
    CC::SetPosition(currentMap->GetSize().x, currentMap->GetSize().y);
    CC::Unlock();
}
void GameManager::Start() {
    player->ActivatePlayer();
    Print();
}

void GameManager::Update() {
    /*std::thread* move = new std::thread(Timer::DelayExecute, player->GetCooldownMovement(), [this]() {*/
        player->UpdatePosition();
        /*});
    move->detach();*/
    /*for (Enemy* e : enemies)
        e->Update();*/
}

void GameManager::End() {
    player->DesactivatePlayer();
}

GameManager::~GameManager() {
    delete player;
    for (Enemy* e : enemies)
        delete e;
    for (NodeMap* map : maps)
        delete map;
    delete currentMap;
}