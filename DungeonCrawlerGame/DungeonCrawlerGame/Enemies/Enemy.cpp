#include "Enemy.h"
#include "../ConsoleControl/ConsoleControl.h"
#include <thread>
#include <ctime>
#include "../Timer/Timer.h"
Json::Value Enemy::Code() {
	Json::Value json = Json::Value();
	Json::Value jsonEnemy = Json::Value();
	jsonEnemy["life"] = life;
	jsonEnemy["attack"] = attack;
	jsonEnemy["movementCooldown"] = movementCooldown;
	jsonEnemy["lastTimeMove"] = lastTimeMove;
	jsonEnemy["isDead"] = isDead;
	json["enemy"] = jsonEnemy;
	return json;
}

void Enemy::Decode(Json::Value json) {
	life = json["life"].asInt();
	attack = json["attack"].asUInt();
	movementCooldown = json["movementCooldown"].asFloat();
	lastTimeMove = json["lastTimeMove"].asFloat();
	isDead = json["isDead"].asBool();
}

void Enemy::Move(NodeMap* currentMap) {
	bool canMove = false;
	positionMutex.lock();
	Vector2 lastPos = node->GetPosition();
	positionMutex.unlock();
	while (!canMove) {
		int randomX = rand() % ((1 - (-1) + 1) + (-1));
		int randomY = rand() % ((1 - (-1) + 1) + (-1));

		Vector2 pos{ randomX, randomY };
		Vector2 nextPos = lastPos + pos;
	
		currentMap->SafePickNode(nextPos, [this, nextPos, &canMove](Node* auxNode) {
			if (auxNode->GetINodeContent()->GetContent() == NodeContent::NOTHING) {
				positionMutex.lock();
				node->SetPosition(nextPos);
				positionMutex.unlock();
				auxNode->SetContent(NodeContent::ENEMY);
				auxNode->DrawContent(nextPos);
				canMove = true;
			}
		});
		if (canMove) {
			currentMap->SafePickNode(lastPos, [this, lastPos](Node* auxNode) {
				if (auxNode->GetINodeContent()->GetContent() == NodeContent::ENEMY) {
					auxNode->SetContent(NodeContent::NOTHING);
					auxNode->DrawContent(lastPos);
				}
			});
		}
	;}
	CC::Lock();
	CC::SetPosition(0, currentMap->GetSize().y);
	CC::Unlock();
}

void Enemy::Draw() {
	node->DrawContent(node->GetPosition());
}

Object* Enemy::DropObject() {
	return object;
}