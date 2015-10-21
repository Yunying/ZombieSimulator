//
//  ZomDrawPanel.h
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>
#include "machine.h"

class ZomDrawPanel : public wxPanel
{
 
public:
	ZomDrawPanel(wxFrame* parent);
	void paintNow();
	MachineState* curr_state;
	bool memory_updated;
	std::string zomfile;
	std::string humanfile;
	int month;
 
protected:
	void paintEvent(wxPaintEvent & evt);
	void render(wxDC& dc);
	void drawGrid(wxDC& dc);
	void drawZombieAndHuman(wxDC& dc);
	void MouseMoved(wxMouseEvent& event);
	void drawMemory(wxDC& dc);
	void drawInfo(wxDC& dc);
	DECLARE_EVENT_TABLE()
	
public:
	// Variables here
};

