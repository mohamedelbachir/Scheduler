#pragma once
#ifndef SPLASHSCREEN_H_INCLUDED
#define SPLASHSCREEN_H_INCLUDED
#include "../MyTinyGUI/include/AppStateMachine.h"
#include "../MyTinyGUI/include/AppState.h"

#include "../MyTinyGUI/include/FontManager.h"
#include "../MyTinyGUI/include/Widgets.h"


class MainSplashState : public AppState
{
private:
    std::vector<Widget *> listWidget;
    SDL_Thread *fetch;
    static bool taskThreadDone;

public:
    static std::string stateID;
    MainSplashState(Window *pWindow) : AppState(pWindow)
    {
        fetch = SDL_CreateThread(loadInitialisation, "loadInitialisation", (void *)nullptr);
    }

    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    static int loadInitialisation(void *ptr);
    static bool initSettingFile();
    static bool initLangFile();
    std::string getStateID() const override;
};

class SplashScreen : public Window
{
public:
    AppStateMachine *states;

    SplashScreen(std::string pTitle, int pX, int pY, int pW, int pH, Uint32 pFlagsWindow = SDL_WINDOW_SHOWN) : Window(pTitle, pX, pY, pW, pH, pFlagsWindow)
    {
        states = new AppStateMachine();
        states->pushState(new MainSplashState(this));
    }

    void update() override
    {
        states->update();
    }

    void draw() override
    {
        // color of background renderer
        SDL_SetRenderDrawColor(this->getRenderer(), 255, 255, 255, 255);
        // clear window renderer
        SDL_RenderClear(this->getRenderer());

        states->render();

        // show the window
        SDL_RenderPresent(this->getRenderer());
    }

    void clean() override
    {
        theTextureManager::Instance()->cleanUp();
        thefontManager::Instance()->cleanUp();
        states->clear();
        Window::clean();
    }
};
typedef SplashScreen theSplashScreen;
#endif // SPLASHSCREEN_H_INCLUDED
