#include <iostream>
#if EMSCRIPTEN
#include <emscripten.h>
#endif
#include "InputManager.h"
#include "include/Window.h"

SDL_Event Window::m_event;

int Window::create(std::string pTitle, int pX, int pY, int pW, int pH, Uint32 pFlagsWindow, Uint32 pFlagsRenderer)
{
    init(pTitle, pX, pY, pW, pH, pFlagsWindow, pFlagsRenderer);
}

void Window::mainloop(void *arg)
{
    Window *window = static_cast<Window *>(arg);
    while (SDL_PollEvent(&Window::m_event))
    {
        window->checkEvent();
    }
    window->update();
    window->draw();
}

void Window::checkEvent()
{
    switch (Window::m_event.type)
    {
    case SDL_WINDOWEVENT:
        if (Window::m_event.window.event == SDL_WINDOWEVENT_CLOSE && Window::m_event.window.windowID == m_windowID)
        {
            std::cout << "cliquer !!" << std::endl;
            quit();
        }
        if (Window::m_event.window.event == SDL_WINDOWEVENT_FOCUS_LOST && Window::m_event.window.windowID == m_windowID)
        {
            m_focus = false;
        }
        if (Window::m_event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED && Window::m_event.window.windowID == m_windowID)
        {
            m_focus = true;
        }
        break;
    }
    if (m_focus)
    {
        handleEvent();
    }
}

int Window::run()
{
#if EMSCRIPTEN
    emscripten_set_main_loop_arg(mainloop, (Window *)this, -1, 1);
#else
    while (isRunning())
    {
        while (SDL_PollEvent(&Window::m_event))
        {
            checkEvent();
        }
        update();
        draw();
        // mainloop(nullptr);
    }
#endif

    clean();
    return 0;
}

void Window::toogleWindowVisibility(bool isVisible)
{
    if (!isVisible)
    {
        SDL_HideWindow(m_window);
    }
    else
    {
        SDL_ShowWindow(m_window);
    }
    this->isVisible = isVisible;
}

bool Window::isRunning()
{
    return m_running;
}

void Window::quit()
{
    m_running = false;
}

Uint32 Window::getIDWindow()
{
    return m_windowID;
}

SDL_Window *Window::getWindow()
{
    return this->m_window;
}

SDL_Renderer *Window::getRenderer()
{
    return m_renderer;
}

void Window::setIsRunnig(bool b_running)
{
    m_running = b_running;
}

void Window::clean()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
}
