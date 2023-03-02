#pragma one
#ifndef ABOUTSTATE_H_INCLUDED
#define ABOUTSTATE_H_INCLUDED
#include"MyTinyGUI/include/AppStateMachine.h"
#include"MyTinyGUI/include/AppState.h"

#include"MyTinyGUI/include/FontManager.h"
#include"MyTinyGUI/include/Widgets.h"

#include "MainApp.h"

class AboutState:public AppState{
private:
    std::vector<Widget*> listWidget;
    UI_Text *authors;
    UI_Text *utools;
    std::string author;
    std::string tools;
public:

    static std::string stateID;
    AboutState(Window *pWindow):AppState(pWindow){

    }

    bool onEnter()override;
    bool onExit()override;

    void update()override;
    void render()override;

    std::string getStateID()const override;
};

class AboutWindow:public Window{

private:
    AboutWindow(std::string pTitle,int pX,int pY,int pW,int pH,Uint32 pFlagsWindow=SDL_WINDOW_SHOWN):Window(pTitle,pX,pY,pW,pH,pFlagsWindow){
        states=new AppStateMachine();
        states->pushState(new AboutState(this));

    }

public:

    AppStateMachine *states;

    static AboutWindow* m_aboutWindow;

    static AboutWindow* Instance(){
        if(m_aboutWindow==NULL){
            m_aboutWindow=new AboutWindow(App::LangData("idAbout"),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,655,285
                         ,SDL_WINDOW_HIDDEN|SDL_WINDOW_SKIP_TASKBAR|SDL_WINDOW_ALWAYS_ON_TOP);
        }
        return m_aboutWindow;
    }

    void update()override{
        SDL_SetWindowAlwaysOnTop(this->getWindow(),SDL_TRUE);
        states->update();
        if(!m_running){
            SDL_HideWindow(this->m_window);
            m_running=true;
        }
    }

    void checkEvent() override{
        Window::checkEvent();
    }

    void draw()override{
        ImGuiStyle* style=&ImGui::GetStyle();
        //color of background renderer
        SDL_SetRenderDrawColor(this->getRenderer(),style->Colors[ImGuiCol_WindowBg].x*255
                                                  ,style->Colors[ImGuiCol_WindowBg].y*255
                                                  ,style->Colors[ImGuiCol_WindowBg].z*255
                                                  ,style->Colors[ImGuiCol_WindowBg].w*255);
        //clear window renderer
        SDL_RenderClear(this->getRenderer());

            states->render();

        //show the window
        SDL_RenderPresent(this->getRenderer());
    }

    void clean()override{
        theTextureManager::Instance()->cleanUp();
        thefontManager::Instance()->cleanUp();
        states->clear();
        Window::clean();
    }
};
typedef AboutWindow theAboutWindow;
#endif // ABOUTSTATE_H_INCLUDED
