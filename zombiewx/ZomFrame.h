//
//  ZomFrame.h
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include "machine.h"
#include "traits.h"

class ZomFrame : public wxFrame
{
public:
	ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~ZomFrame();
private:
	void onExit(wxCommandEvent& event);
	void onNew(wxCommandEvent& event);
	void onSimStart(wxCommandEvent& event);
	void onTurnTimer(wxTimerEvent& event);
	void LoadZombie(wxCommandEvent& event);
	void LoadSurvivor(wxCommandEvent& event);
	void Randomize(wxCommandEvent& event);
	void ResetGame(wxCommandEvent& event);
	void StopGame();
	int month;
	bool zombie_loaded;
	bool human_loaded;
	void EnableMenu();
	void ParseFilePath(const std::string& s, std::string& dest);
	
	wxDECLARE_EVENT_TABLE();
private:
	// Panel for drawing
	class ZomDrawPanel* mPanel;
	
	// Simulation menu
	wxMenu* mSimMenu;
	
	// Turn timer
	wxTimer* mTurnTimer;
	
	// TEMP CODE TEST MACHINES
	MachineState zombieTestState;
	Machine<ZombieTraits> zombieMachine;
	MachineState humanTestState;
	Machine<HumanTraits> humanMachine;
	// END TEMP CODE
	
	bool mIsActive;
};
