#include "GameManager.h"
#include "ctime"

int main() {
	srand(time(NULL));
	GameManager* gameManager = new GameManager();
	gameManager->Code();
	gameManager->Decode();
	gameManager->PrintNewMap();
	gameManager->Start();
	while (true) {
		gameManager->Update();
	}
	return 0;
} 