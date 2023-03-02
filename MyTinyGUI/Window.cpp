#include<iostream>
#include "include/Window.h"
void Window::handleEvent()
{
    while(SDL_PollEvent(&e)){
        if(isInitImgui){
            ImGui_ImplSDL2_ProcessEvent(&e);
        }
        checkEvent();
    }
}

void Window::checkEvent(){
    switch(e.type){
        case SDL_WINDOWEVENT:
            if(e.window.event==SDL_WINDOWEVENT_CLOSE&&e.window.windowID==m_windowID){
                quit();
            }
        break;
    }
}

int Window::run(){
    while(isRunning()){
        handleEvent();
        update();
        draw();
    }
    clean();
    return 0;
}

bool Window::isRunning()
{
    return m_running;
}

void Window::quit()
{
    m_running=false;
}

Uint32 Window::getIDWindow(){
    return m_windowID;
}

SDL_Window *Window::getWindow(){
    return m_window;
}

SDL_Renderer *Window::getRenderer(){
    return m_renderer;
}

void Window::setIsRunnig(bool b_running){
    m_running=b_running;
}

void Window::clean()
{
    if(isInitImgui){
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        //ImGui::DestroyContext(ImGui::GetCurrentContext());
    }
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
}
