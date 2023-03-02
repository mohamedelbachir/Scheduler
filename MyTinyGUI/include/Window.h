#pragma once
#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

//====== DearImGUI Library ==========/
#include"../../DearImgui/imconfig.h"
#include"../../DearImgui/implot.h"

#include"../../DearImgui/imgui_impl_sdl.h"
#include"../../DearImgui/imgui_impl_sdlrenderer.h"

#include<SDL.h>
#include<iostream>
class Window{
protected:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    bool m_running=true;
    Uint32 m_windowID;
    SDL_Event e;
    int m_window_width,m_window_height;
    bool isInitImgui;
public:
    ImGuiIO *io;
    Window(std::string pTitle,int pX,int pY,int pW,int pH,Uint32 pFlagsWindow=SDL_WINDOW_SHOWN,bool isWithImgui=false){
        init(pTitle,pX,pY,pW,pH,pFlagsWindow,isWithImgui);
        isInitImgui=isWithImgui;
    }

    void init(std::string pTitle,int pX,int pY,int pW,int pH,Uint32 pFlagsWindow=SDL_WINDOW_SHOWN,bool isWithImgui=false){
        m_window=NULL;
        m_renderer=NULL;
        isInitImgui=isWithImgui;
        if(!SDL_WasInit(SDL_INIT_VIDEO)){
            if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS)<0)
            {
                std::cout<<"Error to Initialize SDL : "<<SDL_GetError()<<std::endl;
                exit(EXIT_FAILURE);
            }
        }
        m_window=SDL_CreateWindow(pTitle.c_str(),pX,pY,pW,pH,pFlagsWindow);
        if(m_window==NULL)
        {
            std::cout<<"\nError to Create Window "<<pTitle<<" : "<<SDL_GetError()<<std::endl;
            exit(EXIT_FAILURE);
        }

        m_renderer=SDL_CreateRenderer(m_window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

        if(m_renderer==NULL)
        {
            std::cout<<"Error to Create Renderer: "<<SDL_GetError()<<std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_GetWindowSize(m_window,&m_window_width,&m_window_height);
        m_windowID=SDL_GetWindowID(m_window);

        if(isWithImgui){
            // Setup Dear ImGui context
            ImGui::CreateContext();
            ImPlot::CreateContext();
            io=&ImGui::GetIO();
            (void)io;

            // Setup Dear ImGui style
            ImGui::StyleColorsLight();

            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForSDLRenderer(m_window,m_renderer);
            ImGui_ImplSDLRenderer_Init(m_renderer);
        }
    }

    Window(){
       m_window=NULL;
       m_renderer=NULL;
    }

    SDL_Event *getEvent(){
        return &e;
    }
    virtual ~Window(){}

    /** @brief update
    *function for Updating Window and Renderer
    */
    virtual void update(){};

    /** @brief isRunning
    * function to get if Window is Running
    * @return true if is Running and false otherwise
    */
    bool isRunning();

    /** @brief handleEvent
    *function to get Event associate to the Current Window
    */
    virtual void handleEvent();

    /** @brief draw
    * function for Drawing inside the Renderer of the Current Window
    */
    virtual void draw(){};

    /** @brief quit
    * function to exit to the current Window
    */
    void quit();

    /** @brief clean
    * function for Cleaning and free Up memory allocate to the current Window and renderer
    */
    virtual void clean();

    /** @brief clean
    * function for get ID attached to the Current Window
    * @return ID of Window
    */
    Uint32 getIDWindow();

    /**
    *@brief getWindow
    *funcion to get The Current Window
    *@return SDL_Window
    */

    SDL_Window *getWindow();

    /**/
    int getWindowWidth(){
        SDL_GetWindowSize(m_window,&m_window_width,&m_window_height);
        return m_window_width;
    }

    SDL_Point getWindowPosition(){
        SDL_Point pos;
        SDL_GetWindowPosition(m_window,&pos.x,&pos.y);
        return pos;
    }
    /**/
    int getWindowHeight(){
        SDL_GetWindowSize(m_window,&m_window_width,&m_window_height);
        return m_window_height;
    }

    virtual int run();

    virtual void checkEvent();

    /**
    *@brief getRenderer
    *funcion to get The Current Window Renderer
    *@return SDL_Renderer
    */

    SDL_Renderer *getRenderer();

    void setIsRunnig(bool b_running);
};



#endif // WINDOW_H_INCLUDED
