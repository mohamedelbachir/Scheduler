#pragma one
#ifndef ABOUTSTATE_H_INCLUDED
#define ABOUTSTATE_H_INCLUDED
#include "../MyTinyGUI/include/AppStateMachine.h"
#include "../MyTinyGUI/include/AppState.h"

#include "../MyTinyGUI/include/FontManager.h"
#include "../MyTinyGUI/include/Widgets.h"

#include "../MainApp.h"

class AboutState : public AppState
{
private:
    std::vector<Widget *> listWidget;
    UI_Text *authors;
    UI_Text *utools;
    std::string author;
    std::string tools;

public:
    static std::string stateID;
    AboutState(Window *pWindow) : AppState(pWindow)
    {
        if (pWindow == NULL)
        {
            std::cout << "Unable to Initialize the state About ..." << std::endl;
        }
    }

    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getStateID() const override;
};

class AboutWindow : public Window
{

private:
    AboutWindow()
    {
        states = new AppStateMachine();
    }

public:
    AppStateMachine *states;

    static AboutWindow *m_aboutWindow;

    static AboutWindow *Instance()
    {
        if (m_aboutWindow == NULL)
        {
            m_aboutWindow = new AboutWindow();
        }
        return m_aboutWindow;
    }

    void init(std::string pTitle, int pX, int pY, int pW, int pH, Uint32 pFlagsWindow, Uint32 pFlagsRenderer) override
    {
        Window::init(pTitle, pX, pY, pW, pH, pFlagsWindow, pFlagsRenderer);
        states->pushState(new AboutState(this));
    }

    void update() override
    {
        SDL_SetWindowAlwaysOnTop(this->getWindow(), SDL_TRUE);
        SDL_SetWindowTitle(this->m_window, App::LangData("idAbout"));
        states->update();
        if (!m_running)
        {
            toogleWindowVisibility(false);
            m_running = true;
        }
    }

    void checkEvent() override
    {
        Window::checkEvent();
    }
    void draw() override
    {
        ImGuiStyle *style = &ImGui::GetStyle();
        // color of background renderer
        SDL_SetRenderDrawColor(this->getRenderer(), style->Colors[ImGuiCol_WindowBg].x * 255, style->Colors[ImGuiCol_WindowBg].y * 255, style->Colors[ImGuiCol_WindowBg].z * 255, style->Colors[ImGuiCol_WindowBg].w * 255);

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
typedef AboutWindow theAboutWindow;
#endif // ABOUTSTATE_H_INCLUDED
