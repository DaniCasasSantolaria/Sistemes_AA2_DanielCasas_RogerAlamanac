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
	bool canMove = true;
	positionMutex.lock();
	Vector2 lastPos = node->GetPosition();
	positionMutex.unlock();
	while (canMove) {
		int randomX = rand() % ((1 - (-1) + 1) - 1);
		int randomY = rand() % ((1 - (-1) + 1) - 1);
		Vector2 pos{ randomX, randomY };
		Vector2 nextPos = node->GetPosition() + pos;
		currentMap->SafePickNode(nextPos, [this, nextPos, &canMove](Node* auxNode) {
			if (auxNode->GetINodeContent()->GetContent() == NodeContent::NOTHING) {
				positionMutex.lock();
				node->SetPosition(Vector2(nextPos.x, nextPos.y));
				positionMutex.unlock();
				auxNode->SetContent(NodeContent::ENEMY);
				auxNode->DrawContent();
				canMove = true;
			}
			else {
				canMove = false;
			}
		});
	}
	currentMap->SafePickNode(lastPos, [this, lastPos](Node* auxNode) {
		if (auxNode->GetINodeContent()->GetContent() == NodeContent::ENEMY) {
			positionMutex.lock();
			node->SetPosition(Vector2(lastPos.x, lastPos.y));
			positionMutex.unlock();
			auxNode->SetContent(NodeContent::NOTHING);
			auxNode->DrawContent();
		}
	});
}

void Enemy::Draw() {
	CC::Lock();
	CC::SetPosition(node->GetPosition().x, node->GetPosition().y);
	node->DrawContent();
	CC::Unlock();
}

Object* Enemy::DropObject() {
	return object;
}