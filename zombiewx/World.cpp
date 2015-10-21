#include "World.h"

void World::Print(){
	std::cout << "Here" << std::endl;
}

void World::Randomize(Machine<ZombieTraits> zombie, Machine<HumanTraits> human){
	rand_human = 10; rand_zombie = 20;
	bool pos[20][20];
	for (int i = 0; i < 20; i++){
		for (int j = 0; j < 20; j++){
			pos[i][j] = true;
		}
	}
	zombieMachine = zombie;
	humanMachine = human;
	for (int i = 0; i < rand_zombie; i++){
		MachineState* state = new MachineState();
		int x = rand() % 20;
		int y = rand() % 20;
		while (pos[x][y] == false){
			x = rand() % 20;
			y = rand() % 20;
		}
		int z = rand() % 4;
		state->setPosition(x, y);
		pos[x][y] = false;
		state->setDirection(z);
		//state->m_isHuman = false;
		zombie.BindState(*state);
		zombies.push_back(state);
	}
	for (int i = 0; i < rand_human; i++){
		MachineState* state = new MachineState();
		int x = rand() % 20;
		int y = rand() % 20;
		while (pos[x][y] == false){
			x = rand() % 20;
			y = rand() % 20;
		}
		int z = rand() % 4;
		state->setPosition(x, y);
		pos[x][y] = false;
		state->setDirection(z);
		//state->m_isHuman = true;
		human.BindState(*state);
		humans.push_back(state);
	}
}