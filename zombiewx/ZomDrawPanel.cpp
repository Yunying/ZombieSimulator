//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/listimpl.cpp>
#include "World.h"
#include "machine.h"
#include <sstream>

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
	EVT_PAINT(ZomDrawPanel::paintEvent)
	EVT_MOTION(ZomDrawPanel::MouseMoved)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	//curr_state = nullptr;
	memory_updated = true;
	
}

void ZomDrawPanel::paintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	render(dc);
}

void ZomDrawPanel::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void ZomDrawPanel::render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();

	//Draw Memory output
	//if (!memory_updated){
		
		//memory_updated = true;
		//return;
	//}
	
	// Draw the grid
	drawGrid(dc);
	drawZombieAndHuman(dc);
	drawMemory(dc);
	drawInfo(dc);
	/*if (curr_state != NULL){
		dc.DrawText("Mem1: " + curr_state->GetMemory(0), wxPoint(630, 40));
		dc.DrawText("Mem2: " + curr_state->GetMemory(1), wxPoint(630, 60));
	}*/
}

void ZomDrawPanel::drawMemory(wxDC& dc){
	if (curr_state != nullptr && !curr_state->GetInfect()){
		try{
			std::cout << "Draw Memory" << std::endl;
			std::stringstream ss1;
			ss1 << "Mem1 :"; ss1 << curr_state->GetMemory(0);
			std::stringstream ss2;
			ss2 << "Mem2 :"; ss2 << curr_state->GetMemory(1);
			std::string str1 = ss1.str();
			std::string str2 = ss2.str();
			dc.DrawText(str1, wxPoint(630, 40));
			dc.DrawText(str2, wxPoint(630, 60));
			std::cout << curr_state->GetMemory(0) << " " << curr_state->GetMemory(1) << std::endl;
		}
		catch (AccessViolation ex){
			std::cout << ex.what() << std::endl;
			dc.DrawText("Mem1: ", wxPoint(630, 40));
			dc.DrawText("Mem2: ", wxPoint(630, 60));
		}
		catch (std::exception e){
			dc.DrawText("Mem1: ", wxPoint(630, 40));
			dc.DrawText("Mem2: ", wxPoint(630, 60));
			std::cout << e.what() << std::endl;
		}
	}
	else{
		dc.DrawText("Mem1: ", wxPoint(630, 40));
		dc.DrawText("Mem2: ", wxPoint(630, 60));
	}
}

void ZomDrawPanel::drawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);

	for (int i = 1; i < 20; i++){
		dc.DrawLine(wxPoint(10 + i * 30, 10), wxPoint(10 + i * 30, 610));
		dc.DrawLine(wxPoint(10, 10 + i * 30), wxPoint(610, 10 + i * 30));
	}
}

void ZomDrawPanel::MouseMoved(wxMouseEvent& event){
	if (event.m_x > 10 && event.m_y > 10 && event.m_y < 600 && event.m_x < 600){
		for (unsigned int i = 0; i < World::get().humans.size(); i++){
			MachineState state = *World::get().humans.at(i);
			int left = state.x * 30 + 10;
			int right = state.x * 30 + 40;
			int up = state.y * 30 + 10;
			int down = state.y * 30 + 40;
			/*switch (state.m_Facing)
			{
			case (MachineState::UP) :
				up += 60;
				down += 60;
				break;
			case (MachineState::RIGHT) :
				left -= 60;
				right -= 60;
				break;
			case (MachineState::DOWN) :
				up -= 60;
				down -= 60;
				break;
			default:
			case (MachineState::LEFT) :
				left += 60;
				right += 60;
				break;
			}*/
			//std::cout << state.x << " " << state.y << " " << left << " " << right << " " << up << " " << down << std::endl;
			//std::cout << event.m_x << " " << event.m_y << std::endl;
			if (event.m_x > left && event.m_x < right && event.m_y > up && event.m_y < down){
				//std::cout << "Hereeee" << std::endl;
				//if (curr_state.x != state.x || curr_state.y != state.y){
					curr_state = &state;
					//memory_updated = false;
				//}
				
				
				paintNow();
			}
		}
	}
	//std::cout << event.m_x << " " << event.m_y << std::endl;
}

void ZomDrawPanel::drawZombieAndHuman(wxDC& dc){
	dc.SetPen(*wxBLACK_PEN);
	dc.SetBrush(*wxRED_BRUSH);

	for (auto it = World::get().zombies.begin(); it != World::get().zombies.end(); ++it){
		//std::cout << "Here" << std::endl;
		MachineState state = *(*it);
		wxPoint points[3];
		switch (state.m_Facing)
		{
		case (MachineState::UP) :
			points[0] = wxPoint(10 + state.x * 30, 40 + state.y * 30);
			points[1] = wxPoint(40 + state.x * 30, 40 + state.y * 30);
			points[2] = wxPoint(25 + state.x * 30, 10 + state.y * 30);
			dc.DrawPolygon(3,points,0,0,wxODDEVEN_RULE);
			break;
		case (MachineState::RIGHT) :
			points[0] = wxPoint(10 + state.x * 30, 10 + state.y * 30);
			points[1] = wxPoint(40 + state.x * 30, 25 + state.y * 30);
			points[2] = wxPoint(10 + state.x * 30, 40 + state.y * 30);
			dc.DrawPolygon(3, points, 0, 0, wxODDEVEN_RULE);
			break;
		case (MachineState::DOWN) :
			points[0] = wxPoint(10 + state.x * 30, 10 + state.y * 30);
			points[1] = wxPoint(40 + state.x * 30, 10 + state.y * 30);
			points[2] = wxPoint(25 + state.x * 30, 40 + state.y * 30);
			dc.DrawPolygon(3, points, 0, 0, wxODDEVEN_RULE);
			break;
		default:
		case (MachineState::LEFT) :
			points[0] = wxPoint(40 + state.x * 30, 10 + state.y * 30);
			points[1] = wxPoint(40 + state.x * 30, 40 + state.y * 30);
			points[2] = wxPoint(10 + state.x * 30, 25 + state.y * 30);
			dc.DrawPolygon(3, points, 0, 0, wxODDEVEN_RULE);
			break;
		}
		
	}

	dc.SetBrush(*wxGREEN_BRUSH);
	for (auto it = World::get().humans.begin(); it != World::get().humans.end(); ++it){
		//std::cout << "Here" << std::endl;
		MachineState state = *(*it);
		wxPoint points[3];
		switch (state.m_Facing)
		{
		case (MachineState::UP) :
			points[0] = wxPoint(10 + state.x * 30, 40 + state.y * 30);
			points[1] = wxPoint(40 + state.x * 30, 40 + state.y * 30);
			points[2] = wxPoint(25 + state.x * 30, 10 + state.y * 30);
			dc.DrawPolygon(3, points, 0, 0, wxODDEVEN_RULE);
			break;
		case (MachineState::RIGHT) :
			points[0] = wxPoint(10 + state.x * 30, 10 + state.y * 30);
			points[1] = wxPoint(40 + state.x * 30, 25 + state.y * 30);
			points[2] = wxPoint(10 + state.x * 30, 40 + state.y * 30);
			dc.DrawPolygon(3, points, 0, 0, wxODDEVEN_RULE);
			break;
		case (MachineState::DOWN) :
			points[0] = wxPoint(10 + state.x * 30, 10 + state.y * 30);
			points[1] = wxPoint(40 + state.x * 30, 10 + state.y * 30);
			points[2] = wxPoint(25 + state.x * 30, 40 + state.y * 30);
			dc.DrawPolygon(3, points, 0, 0, wxODDEVEN_RULE);
			break;
		default:
		case (MachineState::LEFT) :
			points[0] = wxPoint(40 + state.x * 30, 10 + state.y * 30);
			points[1] = wxPoint(40 + state.x * 30, 40 + state.y * 30);
			points[2] = wxPoint(10 + state.x * 30, 25 + state.y * 30);
			dc.DrawPolygon(3, points, 0, 0, wxODDEVEN_RULE);
			break;
		}

	}

}

void ZomDrawPanel::drawInfo(wxDC& dc){
	std::stringstream ss;
	ss << "# of Zombies: "; ss << World::get().zombies.size();
	dc.DrawText(ss.str(), wxPoint(630, 200));
	dc.DrawText("Zombie file: ", wxPoint(630, 220));
	dc.DrawText(zomfile, wxPoint(630, 240));
	std::stringstream ss1;
	ss1 << "# of Humans: "; ss1 << World::get().humans.size();
	dc.DrawText(ss1.str(), wxPoint(630, 300));
	dc.DrawText("Human file: ", wxPoint(630, 320));
	dc.DrawText(humanfile, wxPoint(630, 340));
	std::stringstream ss2;
	ss2 << "Month: "; ss2 << month;
	dc.DrawText(ss2.str(), wxPoint(630, 400));
}