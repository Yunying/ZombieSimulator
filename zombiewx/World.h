#pragma once
#include "singleton.h"
#include <iostream>
#include <vector>
#include "machine.h"
#include "traits.h"
#include "traits.h"

class World : public Singleton <World>
{
	DECLARE_SINGLETON(World)
public:
	std::vector<MachineState*> zombies;
	std::vector<MachineState*> humans;
	Machine<HumanTraits> humanMachine;
	Machine<ZombieTraits> zombieMachine;
	int rand_human;
	int rand_zombie;

public:
		void Print();
		void Randomize(Machine<ZombieTraits> zombie, Machine<HumanTraits> human);
};

