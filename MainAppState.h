#pragma once
#ifndef MAINAPPSTATE_H_INCLUDED
#define MAINAPPSTATE_H_INCLUDED
#include"MyTinyGUI/include/AppState.h"

class MainState:public AppState{
private:

public:
    static std::string stateID;
    MainState(Window *pWindow):AppState(pWindow){}

    bool onEnter()override;
    bool onExit()override;

    void update()override;
    void render()override;

    std::string getStateID()const override;
};

#endif // MAINAPPSTATE_H_INCLUDED
