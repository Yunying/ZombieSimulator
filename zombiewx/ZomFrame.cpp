//
//  ZomFrame.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include "World.h"
#include "singleton.h"
#include <string>

enum
{
	ID_SIM_START=1000,
	ID_TURN_TIMER,
	ID_LOAD_ZOMBIE,
	ID_LOAD_SURVIVOR,
	ID_RANDOMIZE,
	ID_RESET
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
	EVT_MENU(wxID_EXIT, ZomFrame::onExit)
	EVT_MENU(wxID_NEW, ZomFrame::onNew)
	EVT_MENU(ID_SIM_START, ZomFrame::onSimStart)
	EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::LoadZombie)
	EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::LoadSurvivor)
	EVT_MENU(ID_RANDOMIZE, ZomFrame::Randomize)
	EVT_MENU(ID_RESET, ZomFrame::ResetGame)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::onTurnTimer)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, mIsActive(false)
{
	srand(time(NULL));
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SIM_START, "Start/stop\tSpace", "Start or stop the simulation");
	mSimMenu->Append(ID_LOAD_ZOMBIE, "Load Zombie", "Load Zombie File");
	mSimMenu->Append(ID_LOAD_SURVIVOR, "Load Survivor", "Load Survivor File");
	mSimMenu->Append(ID_RANDOMIZE, "Randomize", "Add 20 zombies and 10 humans to the world");
	mSimMenu->Append(ID_RESET, "Reset", "Reset the world");

	mSimMenu->Enable(ID_SIM_START, false);
	mSimMenu->Enable(ID_RANDOMIZE, false);
	mSimMenu->Enable(ID_RESET, false);
	
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);

	// TEMP CODE: Initialize zombie test machine
	//zombieMachine.LoadMachine(std::string(""));
	//zombieMachine.BindState(zombieTestState);
	// END TEMP CODE
	month = 0;
	zombie_loaded = false;
	human_loaded = false;
}

ZomFrame::~ZomFrame()
{
	delete mTurnTimer;
}

void ZomFrame::onExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::onNew(wxCommandEvent& event)
{
	StopGame();
	zombieMachine.ClearMemory();
	humanMachine.ClearMemory();
	mPanel->zomfile = "";
	mPanel->humanfile = "";
	mPanel->paintNow();
	zombie_loaded = false;
	human_loaded = false;
	EnableMenu();
}

void ZomFrame::onSimStart(wxCommandEvent& event)
{
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		//zombieMachine.BasicMovement();
		//humanMachine.BasicMovement();
		mIsActive = true;
		mPanel->paintNow();
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
		mPanel->paintNow();
	}
}

void ZomFrame::onTurnTimer(wxTimerEvent& event)
{
	// TEMP CODE: Take turn for zombie machine
	//zombieMachine.TakeTurn(zombieTestState);
	month++;
	mPanel->month = month;
	
	
	
	try{
		for (unsigned int i = 0; i < World::get().zombies.size(); i++){
			zombieMachine.TakeTurn(*World::get().zombies.at(i));
		}
		for (unsigned int i = 0; i < World::get().humans.size(); i++){
			humanMachine.TakeTurn(*World::get().humans.at(i));
		}
		/*for (auto it = World::get().zombies.begin(); it != World::get().zombies.end(); it++){
			zombieMachine.TakeTurn(**it);
		}
	
		for (auto it = World::get().humans.begin(); it != World::get().humans.end(); it++){
			humanMachine.TakeTurn(**it);
		}*/
		
	}
	catch (ZomFileNotLoadExcept e){
		//std::cout << "Exceptions" << std::endl;
		std::cout << e.what() << std::endl;
	}
	catch (ZombieInvalidOperation e){
		std::cout << e.what() << std::endl;
		StopGame();
		wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
		return;
	}
	catch (AccessViolation e){
		std::cout << e.what() << std::endl;
	}
	catch (std::exception e){
		//std::cout << e.what() << std::endl;
		StopGame();
		wxMessageBox("Line number not valid in the ZOM file", "Error", wxOK | wxICON_ERROR);
		return;
	}

	mPanel->paintNow();
	
	if (World::get().zombies.size() == 0){
		StopGame();
		wxMessageBox("Human Wins!", "Human win");
	}
	else if (World::get().humans.size() == 0){
		StopGame();
		wxMessageBox("Zombie Wins!", "Zombie win");
	}
	
	// END TEMP CODE
}

void ZomFrame::ParseFilePath(const std::string& s, std::string& dest){
	int len = s.length();
	std::string result;
	result.reserve(20);
	for (int i = len - 1; i >= 0; i--){
		if (s.at(i) != '\\'){
			result += s.at(i);
		}
		else{
			break;
		}
	}
	std::reverse(result.begin(), result.end());
	dest = result;
}

void ZomFrame::LoadZombie(wxCommandEvent& event)
{
	wxFileDialog fileDlg(this, "Load a Zombie file...",
		"./zom", "", "ZOM Files|*.zom",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (fileDlg.ShowModal() == wxID_OK)
	{
		std::string filePath;
		filePath = fileDlg.GetPath();

		try{
			zombieMachine.LoadMachine(filePath);
			ParseFilePath(filePath, mPanel->zomfile);
			mPanel->paintNow();
			//mPanel->zomfile = filePath;
			//std::strcpy(mPanel->zomfile.c_str(), filePath.c_str());
			//zombieMachine.BindState(zombieTestState);
			//World::get().zombies.push_back(&zombieTestState);
			zombie_loaded = true;
			EnableMenu();
		}
		catch (FileLoadExcept e){
			wxMessageBox("ZOM file is invalid", "Error", wxOK | wxICON_ERROR);
		}

	}
}

void ZomFrame::LoadSurvivor(wxCommandEvent& event)
{
	wxFileDialog fileDlg(this, "Load a Human file...",
		"./zom", "", "ZOM Files|*.zom",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (fileDlg.ShowModal() == wxID_OK)
	{
		std::string filePath;
		filePath = fileDlg.GetPath();

		try{
			humanMachine.LoadMachine(filePath);
			//humanMachine.BindState(humanTestState);
			ParseFilePath(filePath, mPanel->humanfile);
			mPanel->paintNow();
			//mPanel->humanfile = filePath;
			//World::get().humans.push_back(&humanTestState);
			human_loaded = true;
			EnableMenu();
		}
		catch (FileLoadExcept e){
			wxMessageBox("ZOM file is invalid", "Error", wxOK | wxICON_ERROR);
		}

	}
}

void ZomFrame::Randomize(wxCommandEvent& event){
	World::get().Randomize(zombieMachine, humanMachine);
	mPanel->paintNow();
}

void ZomFrame::ResetGame(wxCommandEvent& event){
	StopGame();
}

void ZomFrame::StopGame(){
	//Stop Timer
	mTurnTimer->Stop();
	mIsActive = false;
	month = 0;

	//Clear GameBoard
	World::get().zombies.clear();
	World::get().humans.clear();
	mPanel->paintNow();
}

void ZomFrame::EnableMenu(){
	if (human_loaded && zombie_loaded){
		mSimMenu->Enable(ID_SIM_START, true);
		mSimMenu->Enable(ID_RANDOMIZE, true);
		mSimMenu->Enable(ID_RESET, true);
	}
	else{
		mSimMenu->Enable(ID_SIM_START, false);
		mSimMenu->Enable(ID_RANDOMIZE, false);
		mSimMenu->Enable(ID_RESET, false);
	}
}
