#include "op.h"
#include "machine.h"
#include "world.h"
#include <iostream>
#include <cstdlib>

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	//std::cout << state.m_ProgramCounter << ":" << m_OpName << "," << m_Param << " Pos: " << state.x << " " << state.y << std::endl;
}

void OpRotate::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::RIGHT;
		}
		else
		{
			state.m_Facing = MachineState::LEFT;
		}
							break;
	case (MachineState::RIGHT) :
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::DOWN;
		}
		else
		{
			state.m_Facing = MachineState::UP;
		}
							   break;
	case (MachineState::DOWN) :
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::LEFT;
		}
		else
		{
			state.m_Facing = MachineState::RIGHT;
		}
							  break;
	default:
	case (MachineState::LEFT) :
		if (m_Param == 0)
		{
			state.m_Facing = MachineState::UP;
		}
		else
		{
			state.m_Facing = MachineState::DOWN;
		}
							  break;
	}

	/*std::cout << "Now facing ";
	switch (state.m_Facing)
	{
	case (MachineState::UP):
	std::cout << "UP";
	break;
	case (MachineState::RIGHT):
	std::cout << "RIGHT";
	break;
	case (MachineState::DOWN):
	std::cout << "DOWN";
	break;
	case (MachineState::LEFT):
	std::cout << "LEFT";
	break;
	}
	std::cout << std::endl;*/
	state.m_ProgramCounter++;
	state.m_ActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
	
	state.m_ProgramCounter = m_Param;
}

void OpAttack::Execute(MachineState& state)
{
	DebugOutput(state);

	int targetx = state.x;
	int targety = state.y;
	state.m_ProgramCounter++;
	state.m_ActionsTaken++;

	//Find the coordinates before the zombie/human
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		targety -= 1;
		break;
	case (MachineState::RIGHT) :
		targetx += 1;
		break;
	case (MachineState::DOWN) :
		targety += 1;
		break;
	default:
	case (MachineState::LEFT) :
		targetx -= 1;
		break;
	}

	//Check all zombies in the world
		for (auto it = World::get().zombies.begin(); it != World::get().zombies.end(); ++it){
			MachineState ostate = **it;
			//If there's a target
			if (ostate.x == targetx && ostate.y == targety){
				//If I'm a zombie
				if (state.GetInfect()){
					//Nothing happens
					return;
				}
				//If I'm a human
				else{
					//The zombie dies
					World::get().zombies.erase(it);
					return;
				}
				
			}
			
		}
		for (auto it = World::get().humans.begin(); it != World::get().humans.end(); ++it){
			MachineState ostate = **it;
			//If there's a target
			if (ostate.x == targetx && ostate.y == targety){
				//If I'm a zombie
				if (state.GetInfect()){
					//Zombie infect human

					//Add a zombie
					MachineState* new_state = new MachineState();
					new_state->setPosition(ostate.x, ostate.y);
					new_state->setDirection(ostate.m_Facing);
					World::get().zombieMachine.BindState(*new_state);
					World::get().zombies.push_back(new_state);
					//Remove a human
					World::get().humans.erase(it);
					
					return;
				}
				//If I'm a human
				else{
					//The other human dies
					World::get().humans.erase(it);
					return;
				}

			}
		}
	
	
}

void OpRangedAttack::Execute(MachineState& state)
{
	DebugOutput(state);
	if (state.GetInfect()){
		ZombieInvalidOperation ex;
		throw ex;
		return;
	}

	int targetx = state.x;
	int targetx2 = state.x;
	int targety = state.y;
	int targety2 = state.y;
	//std::cout << state.x << " " << state.y << " " << "Ranged Attack: " << targetx << " " << targety << " " << targetx2 << " " << targety2 << std::endl;
	bool first = false; bool second = false;

	//Find the coordinates before the zombie/human
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		targety -= 1;
		targety2 -= 2;
		break;
	case (MachineState::RIGHT) :
		targetx += 1;
		targetx2 += 2;
		break;
	case (MachineState::DOWN) :
		targety += 1;
		targety2 += 2;
		break;
	default:
	case (MachineState::LEFT) :
		targetx -= 1;
		targetx2 -= 2;
		break;
	}

	//Check all zombies in the world
	for (auto it = World::get().zombies.begin(); it != World::get().zombies.end(); ++it){
		MachineState ostate = **it;
		//If there's a target
		if ((ostate.x == targetx && ostate.y == targety) || (ostate.x == targetx2 && ostate.y == targety2)){
			it = World::get().zombies.erase(it);
			break;
		}

	}
	for (auto it = World::get().humans.begin(); it != World::get().humans.end(); ++it){
		MachineState ostate = **it;
		if ((ostate.x == targetx && ostate.y == targety) || (ostate.x == targetx2 && ostate.y == targety2)){
			it = World::get().humans.erase(it);
			break;
		}	
	}

	state.m_ProgramCounter++;
	state.m_ActionsTaken++;
}

void OpForward::Execute(MachineState& state)
{
	DebugOutput(state);
	//std::cout << state.m_ProgramCounter << ":" << m_OpName << "," << m_Param << " Pos: " << state.x << " " << state.y << std::endl;
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		if (state.y != 0){
			state.y = state.y - 1;
		}
		
		break;
	case (MachineState::RIGHT) :
		if (state.x != 19){
			state.x = state.x + 1;
		}
		break;
	case (MachineState::DOWN) :
		if (state.y != 19){
			state.y = state.y + 1;
		}
		break;
	default:
	case (MachineState::LEFT) :
		if (state.x != 0){
			state.x = state.x - 1;
		}
		break;
	}
	state.m_ProgramCounter++;
	state.m_ActionsTaken++;
}

void OpEndturn::Execute(MachineState& state)
{
	DebugOutput(state);
	state.m_ProgramCounter++;
	if (state.GetInfect()){
		state.m_ActionsTaken = 1;
	}
	else{
		state.m_ActionsTaken = 2;
	}
	
}

void OpTestHuman::Execute(MachineState& state)
{
	DebugOutput(state);
	int targetx = state.x;
	int targetx2 = state.x;
	int targety = state.y;
	int targety2 = state.y;
	state.m_Test = false;
	state.m_ProgramCounter++;

	//Find the coordinates before the zombie/human
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		targety -= 1;
		targety2 -= 2;
		break;
	case (MachineState::RIGHT) :
		targetx += 1;
		targetx2 += 2;
		break;
	case (MachineState::DOWN) :
		targety += 1;
		targety2 += 2;
		break;
	default:
	case (MachineState::LEFT) :
		targetx -= 1;
		targetx2 -= 2;
		break;
	}


	for (auto it = World::get().humans.begin(); it != World::get().humans.end(); ++it){
		MachineState ostate = **it;
		if (m_Param == 1){
			if (ostate.x == targetx && ostate.y == targety){
				state.m_Test = true;
				return;
			}
		}
		else{
			if (ostate.x == targetx2 && ostate.y == targety2){
				state.m_Test = true;
				return;
			}
		}
	}
	
}

void OpTestWall::Execute(MachineState& state)
{
	DebugOutput(state);
	
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		if (state.y == 0){
			state.m_Test = true;
		}
		else{
			state.m_Test = false;
		}
		break;
	case (MachineState::RIGHT) :
		if (state.x == 19){
			state.m_Test = true;
		}
		else{
			state.m_Test = false;
		}
		break;
	case (MachineState::DOWN) :
		if (state.y == 19){
			state.m_Test = true;
		}
		else{
			state.m_Test = false;
		}
		break;
	default:
	case (MachineState::LEFT) :
		if (state.x == 0){
			state.m_Test = true;
		}
		else{
			state.m_Test = false;
		}
		break;
	}
	//std::cout << "Test Result: " << state.m_Test << std::endl;
	state.m_ProgramCounter++;
}

void OpTestZombie::Execute(MachineState& state)
{
	DebugOutput(state);
	int targetx = state.x;
	int targetx2 = state.x;
	int targety = state.y;
	int targety2 = state.y;
	state.m_Test = false;
	state.m_ProgramCounter++;

	//Find the coordinates before the zombie/human
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		targety -= 1;
		targety2 -= 2;
		break;
	case (MachineState::RIGHT) :
		targetx += 1;
		targetx2 += 2;
		break;
	case (MachineState::DOWN) :
		targety += 1;
		targety2 += 2;
		break;
	default:
	case (MachineState::LEFT) :
		targetx -= 1;
		targetx2 -= 2;
		break;
	}


	for (auto it = World::get().zombies.begin(); it != World::get().zombies.end(); ++it){
		MachineState ostate = **it;
		if (m_Param == 1){
			if (ostate.x == targetx && ostate.y == targety){
				state.m_Test = true;
				return;
			}
		}
		else if (m_Param == 2){
			if (ostate.x == targetx2 && ostate.y == targety2){
				state.m_Test = true;
				return;
			}
		}
	}
	
}

void OpTestRandom::Execute(MachineState& state)
{
	DebugOutput(state);
	int i = rand() % 2;
	if (i == 0){
		state.m_Test = true;
	}
	else{
		state.m_Test = false;
	}
	//std::cout << "Test Result: " << state.m_Test << std::endl;
	state.m_ProgramCounter++;
}

void OpTestPassable::Execute(MachineState& state)
{
	DebugOutput(state);
	int targetx = state.x;
	int targety = state.y;
	state.m_Test = true;
	state.m_ProgramCounter++;

	//Find the coordinates before the zombie/human
	switch (state.m_Facing)
	{
	case (MachineState::UP) :
		if (state.y == 0){
			state.m_Test = false;
			return;
		}
		targety -= 1;
		break;
	case (MachineState::RIGHT) :
		if (state.x == 19){
			state.m_Test = false;
			return;
		}
		targetx += 1;
		break;
	case (MachineState::DOWN) :
		if (state.y == 19){
			state.m_Test = false;
			return;
		}
		targety += 1;
		break;
	default:
	case (MachineState::LEFT) :
		if (state.x == 0){
			state.m_Test = false;
			return;
		}
		targetx -= 1;
		break;
	}


	for (auto it = World::get().zombies.begin(); it != World::get().zombies.end(); ++it){
		MachineState ostate = **it;
		if (ostate.x == targetx && ostate.y == targety){
			state.m_Test = false;
			return;
		}
	}
	for (auto it = World::get().humans.begin(); it != World::get().humans.end(); ++it){
		MachineState ostate = **it;
		if (ostate.x == targetx && ostate.y == targety){
			state.m_Test = false;
			return;
		}
	}

	
}

void OpJe::Execute(MachineState& state)
{
	DebugOutput(state);
	if (state.m_Test){
		state.m_ProgramCounter = m_Param;
	}
	else{
		state.m_ProgramCounter++;
	}
}

void OpJne::Execute(MachineState& state)
{
	DebugOutput(state);
	if (!state.m_Test){
		state.m_ProgramCounter = m_Param;
	}
	else{
		state.m_ProgramCounter++;
	}
}

void OpMem::Execute(MachineState& state)
{
	DebugOutput(state);
	if (state.GetInfect()){
		ZombieInvalidOperation ex;
		throw ex;
	}
	else{
		if (m_Param >= state.GetMaxMemory() || m_Param < 0){
			AccessViolation ex;
			throw ex;
		}
		else{
			state.m_currMemory = m_Param;
		}
	}
	state.m_ProgramCounter++;
}

void OpSet::Execute(MachineState& state)
{
	DebugOutput(state);
	ZombieInvalidOperation ex;
	if (state.GetInfect()){
		throw ex;
	}
	else{
		if (state.m_currMemory == -1){
			AccessViolation e;
			throw e;
		}
		else{
			state.SetMemory(state.m_currMemory, m_Param);
		}
	}
	state.m_ProgramCounter++;
}

void OpInc::Execute(MachineState& state)
{
	DebugOutput(state);
	ZombieInvalidOperation ex;
	if (state.GetInfect()){
		throw ex;
	}
	else{
		if (state.m_currMemory == -1){
			AccessViolation e;
			throw e;
		}
		else{
			int i = state.GetMemory(state.m_currMemory) + 1;
			state.SetMemory(state.m_currMemory, i);
		}
	}
	state.m_ProgramCounter++;
}

void OpDec::Execute(MachineState& state)
{
	DebugOutput(state);
	AccessViolation ex;
	if (state.GetInfect()){
		ZombieInvalidOperation e;
		throw e;
	}
	else{
		if (state.m_currMemory == -1){
			throw ex;
		}
		else{
			int i = state.GetMemory(state.m_currMemory) - 1;
			state.SetMemory(state.m_currMemory, i);
		}
	}
	state.m_ProgramCounter++;
}

void OpTestMem::Execute(MachineState& state)
{
	DebugOutput(state);
	AccessViolation ex;
	ZombieInvalidOperation e;
	if (state.GetInfect()){
		throw e;
	}
	else{
		if (state.m_currMemory == -1){
			throw ex;
		}
		else{
			int i = state.GetMemory(state.m_currMemory);
			if (i == m_Param){
				state.m_Test = true;
			}
			else{
				state.m_Test = false;
			}
		}
	}
	state.m_ProgramCounter++;
}

void OpSaveLoc::Execute(MachineState& state)
{
	DebugOutput(state);
	if (state.GetInfect()){
		ZombieInvalidOperation ex;
		throw ex;
	}
	else{
		state.SetMemory(0, state.x);
		state.SetMemory(1, state.y);
	}
	state.m_ProgramCounter++;
}

