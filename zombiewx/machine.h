#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "op.h"
#include "exceptions.h"
#include <fstream>
#include <sstream>

// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;

	enum Facing { UP, RIGHT, DOWN, LEFT };
	MachineState()
		: m_ProgramCounter(1)
		, m_ActionsTaken(0)
		, m_Facing(UP)
		, m_Test(false)
		, m_Memory(nullptr)
	{ 
		x = 1;
		y = 0;
		m_currMemory = -1;
		//m_isHuman = false;
	}

	~MachineState()
	{
		//delete[] m_Memory;
	}

	void setPosition(int xpos, int ypos){
		x = xpos;
		y = ypos;
	}

	void setDirection(int i){
		switch (i){
		case 0:
			m_Facing = Facing::UP;
			break;
		case 1:
			m_Facing = Facing::LEFT;
			break;
		case 2:
			m_Facing = Facing::RIGHT;
			break;
		default:
		case 3:
			m_Facing = Facing::DOWN;
			break;

		}
	}

	// Active line number in behavior program
	int m_ProgramCounter;
	// Number of actions taken this turn
	int m_ActionsTaken;
	// Current facing of this character
	Facing m_Facing;
	// Test flag for branches
	bool m_Test;
	//Current Memory Slot
	int m_currMemory;

	//bool isHuman;

	int GetActionsPerTurn() const NOEXCEPT { return m_ActionsPerTurn; }
	int GetMaxMemory() const NOEXCEPT { return m_MaxMemory; }
	bool GetInfect() const NOEXCEPT { return m_InfectOnAttack; }
	int GetMemory(int location)
	{
		// FIXME: Throw exception if out of bounds or memory is not allocated
		if (!m_Memory){
			std::cout << "Not Allocated" << std::endl;
			AccessViolation ex;
			throw ex;
		}
		else if (GetInfect()){
			std::cout << "Not a human" << std::endl;
			AccessViolation ex;
			throw ex;
		}
		else if (location >= m_MaxMemory || location < 0){
			std::cout << "Location not valid: "  << location <<" " << m_MaxMemory << std::endl;
			AccessViolation ex;
			throw ex;
		}
		
		else{
			//std::cout << location << std::endl;
			return m_Memory[location];
		}
	}
	void SetMemory(int location, int value)
	{
		m_Memory[location] = value;
	}

	

	//Additional Variables
	int x;
	int y;

private:
	// Data which is set by the traits
	int m_ActionsPerTurn;
	int m_MaxMemory;
	bool m_InfectOnAttack;
	int* m_Memory;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

	void BasicMovement();

	void ClearMemory();

	// Destructor
	~Machine();
private:
	std::vector<Op*> m_Ops;
};

template <typename MachineTraits>
void Machine<MachineTraits>::ClearMemory()
{
	m_Ops.clear();
}

template <typename MachineTraits>
void Machine<MachineTraits>::BasicMovement()
{
	if (m_Ops.empty()){
		//std::cout << "Empty Op" << std::endl;
		//m_Ops.clear();
		m_Ops.push_back(new OpTestWall());
		m_Ops.push_back(new OpJe(5));
		m_Ops.push_back(new OpForward());
		m_Ops.push_back(new OpGoto(1));
		m_Ops.push_back(new OpTestRandom());
		m_Ops.push_back(new OpJe(9));
		m_Ops.push_back(new OpRotate(0));
		m_Ops.push_back(new OpGoto(1));
		m_Ops.push_back(new OpRotate(1));
		m_Ops.push_back(new OpGoto(1));
	}
}

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
	// TEMP CODE: Add your parsing code here!
	/*m_Ops.clear();
	m_Ops.push_back(new OpRotate(0));
	m_Ops.push_back(new OpRotate(0));
	m_Ops.push_back(new OpRotate(1));
	m_Ops.push_back(new OpGoto(1));*/
	// END TEMP CODE
	m_Ops.clear();
	std::ifstream file(filename);

	if (file.is_open()){
		std::string operation;
		while (std::getline(file, operation)){
			//Parse the line to get an op and a param if existing
			std::string op;
			int param = -1;
			op.reserve(20);
			for (unsigned int i = 0; i < operation.length(); i++){
				if (operation.at(i) != ',' && operation.at(i) != '\n' && operation.at(i) != ' '){
					op += operation.at(i);
				}
				else if (operation.at(i) == ','){
					/*try{
						if (operation.at(i + 3) != ';' && operation.at(i+2) != '\t'){
							param = (operation.at(i + 1) - 48) * 10 + operation.at(i + 2) - 48;
						}
						else{
							param = operation.at(i + 1) - 48;
						}
					}
					catch (std::exception e){
						try{
							param = (operation.at(i + 1) - 48) * 10 + operation.at(i + 2) - 48;
						}
						catch (std::exception e){
							param = operation.at(i + 1) - 48;
						}
					}*/
					++i;
					std::stringstream ss;
					while (operation.at(i) >47 && operation.at(i) < 58 && i < operation.length()){
						ss << operation.at(i);
						++i;
						if (i == operation.length()){
							break;
						}
					}
					param = std::stoi(ss.str(), nullptr);
					
					break;
				}
				else{
					break;
				}
			}
			//std::cout << op << " " << param << std::endl;
			if (op.substr(0,6) == "attack"){
				m_Ops.push_back(new OpAttack());
			}
			else if (op.substr(0,13) == "ranged_attack"){
				m_Ops.push_back(new OpRangedAttack());
			}
			else if (op == "rotate"){
				m_Ops.push_back(new OpRotate(param));
			}
			else if (op.substr(0,7) == "forward"){
				m_Ops.push_back(new OpForward());
			}
			else if (op.substr(0,7) == "endturn"){
				m_Ops.push_back(new OpEndturn());
			}
			else if (op == "test_human"){
				m_Ops.push_back(new OpTestHuman(param));
			}
			else if (op.substr(0,9) == "test_wall"){
				m_Ops.push_back(new OpTestWall());
			}
			else if (op == "test_zombie"){
				m_Ops.push_back(new OpTestZombie(param));
			}
			else if (op.substr(0,11) == "test_random"){
				m_Ops.push_back(new OpTestRandom());
			}
			else if (op.substr(0,13) == "test_passable"){
				m_Ops.push_back(new OpTestPassable());
			}
			else if (op == "je"){
				m_Ops.push_back(new OpJe(param));
			}
			else if (op == "jne"){
				m_Ops.push_back(new OpJne(param));
			}
			else if (op == "goto"){
				m_Ops.push_back(new OpGoto(param));
			}
			else if (op == "mem"){
				m_Ops.push_back(new OpMem(param));
			}
			else if (op == "set"){
				m_Ops.push_back(new OpSet(param));
			}
			else if (op.substr(0,3) == "inc"){
				m_Ops.push_back(new OpInc());
			}
			else if (op.substr(0,3) == "dec"){
				m_Ops.push_back(new OpDec());
			}
			else if (op == "test_mem"){
				m_Ops.push_back(new OpTestMem(param));
			}
			else if (op.substr(0,8) == "save_loc"){
				m_Ops.push_back(new OpSaveLoc());
			}

			
		}
	}
	else{
		FileLoadExcept ex;
		throw ex;
	}


}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.m_ActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.m_MaxMemory = MachineTraits::MEMORY_LOCATIONS;
	state.m_InfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
	delete state.m_Memory;
	state.m_Memory = new int[state.m_MaxMemory];
	
	for (int i = 0; i < state.m_MaxMemory; i++)
	{
		state.m_Memory[i] = 0;
	}
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	std::cout << "TAKING TURN" << std::endl;
	if (m_Ops.empty()){
		std::cout << "It is empty" << std::endl;
		BasicMovement();
	}
	state.m_ActionsTaken = 0;
	while (state.m_ActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		m_Ops.at(state.m_ProgramCounter - 1)->Execute(state);
	}
}

template <typename MachineTraits>
Machine<MachineTraits>::~Machine()
{
	/*for (auto op : m_Ops)
	{
		delete op;
	}

	m_Ops.clear();*/
}
