#pragma once
#include "PlayerAttack.h"
#include "PlayerHealing.h"
#include "../Vector2/Vector2.h"
#include <vector>
#include "Weapons/Weapon.h"
#include <mutex>
#include "../Nodes/Node.h"
#include "../InputManager/InputManager.h"
#include "../Objects/Object.h"
#include "../Nodes/NodeMap.h"

class Player : public PlayerAttackable, public PlayerDamageable, public PlayerHealing {
public:
	enum class PlayerState { RIGHT, LEFT, UP, DOWN, IDLE };
private:
	PlayerState movementState = PlayerState::IDLE;
	Node* position;
	std::mutex positionMutex;
	int coinCounter;
	std::mutex coinsMutex;
	int lifes;
	std::mutex lifeMutex;
	int maxLife = 100;
	int potionsCounter;
	std::mutex potionsMutex;
	float cooldown = 1.0f;
	std::vector<Weapon*> weapons;
	Weapon* equipedWeapon;

public:
	Player();
	~Player();
	void Attack(EnemyDamageable* enemy) override;
	inline void ActivatePlayer() {IS.StartListen();}
	inline void DesactivatePlayer() {IS.StopListen();}
	void ReceiveMoreCoins(int amount);
	int GetLifes();
	int GetCoins();
	int GetAmountPotions();
	inline void RecievePotion() { potionsCounter++; }
	inline int GetCooldownMovement() const { return cooldown; }
	inline PlayerState GetPlayerState() const { return movementState; }
	inline void SetMovementState(PlayerState m) { movementState = m; }
	Vector2 GetPosition();
	void UpdatePosition();
	void ReceiveDamage(int damage) override;
	void TakeObject(Object* object);
	void Heal(int lifeToHeal) override;
	void Draw();
};