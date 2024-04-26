#pragma once
#ifndef INPUTMANAGER_H_INCLUDED
#define INPUTMANAGER_H_INCLUDED
#include<SDL.h>
class InputManager{
private:
    static InputManager *instanceInput;

    InputManager();

    ~InputManager(){}
public:
    static SDL_Event event;
    static InputManager* instance(){
        if(instanceInput==NULL){
            instanceInput=new InputManager();
        }
        return instanceInput;
    }
};
typedef InputManager theInputManager;
#endif // INPUTMANAGER_H_INCLUDED
