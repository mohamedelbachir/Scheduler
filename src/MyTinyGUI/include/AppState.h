#ifndef APPSTATE_H_INCLUDED
#define APPSTATE_H_INCLUDED
#include"Window.h"
#include<cstring>
class AppState{
protected:
    Window *referenceWindow;
public:
    AppState(Window *pWindow){
        referenceWindow=pWindow;
    }
    virtual ~AppState(){}

    virtual bool onEnter()=0;
    virtual bool onExit()=0;

    virtual void update()=0;
    virtual void render()=0;

    virtual std::string getStateID()const=0;
};


#endif // APPSTATE_H_INCLUDED
