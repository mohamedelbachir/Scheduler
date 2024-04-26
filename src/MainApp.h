#pragma once
#ifndef MAINAPP_H_INCLUDED
#define MAINAPP_H_INCLUDED
#if EMSCRIPTEN
#include <emscripten.h>
#include "FileDialog/emscripten_browser_file.h"
#endif

#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"

//====== DearImGUI Library ==========/
#include "utility.h"
#include "imgui/imconfig.h"
#include "imgui/implot.h"

#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_sdlrenderer.h"

#include "MyTinyGUI/include/Window.h"
// #include"DearImgui/misc/freetype/imgui_freetype.h"
#include "MyTinyGUI/include/AppStateMachine.h"

#include "IconHeader/IconsMaterialDesign.h"
#include "Screens/MainAppState.h"
#include "Screens/SplashScreen.h"
#include "ParsingFile.h"

class App : public Window
{
private:
    bool isDroppedFile = false;

    SDL_Thread *aboutWindowThread = NULL;

    std::string fileDroped;

    Timer tOpenAbout;

    /**
        @brief mainMenu
        function to display main menu bar
    */
    void mainMenu();

    App() : Window() {}

    ~App()
    {
        clearData();
    }

    App(std::string pTitle, int pX, int pY, int pW, int pH, Uint32 pFlagsWindow = SDL_WINDOW_SHOWN) : Window(pTitle, pX, pY, pW, pH, pFlagsWindow)
    {
        states = new AppStateMachine();
        states->pushState(new MainState(this));
    }

    App(int argc, char *argv[]) : App("Scheduler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT)
    {
        // init(argc,argv);
    }

    /**
        @brief fetchchingData
        function to load saved data and lang when the application is initialize
    */
    int fetchingData();

public:
    /// path file storage
    static std::map<Path, std::string> m_path;

    /// recent path folder
    static std::vector<std::string> m_recentItems;

    /// lang pathFile
    static std::map<std::string, std::map<Lang, std::string>> LangMap;

    friend class MainSplashState;

    static App *m_app;
    AppStateMachine *states;

    static bool isAboutPopupWindowOpen;

    static ImVec4 color_exec;
    static ImVec4 color_wait;
    static SDL_Rect m_windowPos;

    static Theme m_appTheme;
    static Lang m_appLanguage;

    static ImFont *cft;

    ImGuiIO *io;

    void clearData()
    {
        delete cft;
        if (aboutWindowThread != NULL)
            SDL_DetachThread(aboutWindowThread);
        LangMap.clear();
        m_recentItems.clear();
        m_path.clear();
    }

    ImGuiIO *getIO()
    {
        return io;
    }

    static void startWindow()
    {
        static bool isOpen = true;
        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing;
        ImGui::Begin("mainWindow", &isOpen, flags);
    }

    static void endWindow()
    {
        ImGui::End();
        ImGui::PopStyleVar();
    }

    static int aboutWindow(void *ptr);

    /**
        @brief init
        function to initialize the application
        @param argc : argument number
        @param argv : argument list
    */
    void init(int argc, char **argv)
    {

        /*TiXmlDocument doc;
        doc.Parse("<?xml version='1.0' encoding='UTF-8'?> <project version='4'><component name='JavaScriptSettings'><option name='languageLevel' value='ES6' /></component></project>");
        TiXmlElement *root = doc.RootElement();
        if (!root->NoChildren())
        {
            std::cout << root->FirstChildElement()->FirstAttribute()->Name() << std::endl;
        }*/
        // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

        m_windowPos.x = SDL_WINDOWPOS_CENTERED;
        m_windowPos.y = SDL_WINDOWPOS_CENTERED;
        m_windowPos.w = WINDOW_WIDTH;
        m_windowPos.h = WINDOW_HEIGHT;

        fetchingData();

        Uint32 pWindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
        Uint32 pRenderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
        // SDL_DisplayMode d;
        // SDL_GetDesktopDisplayMode(0, &d);
        SDL_Rect p = {0};

        /*if (m_windowPos.w == d.w && m_windowPos.h == d.h)
        {
            pWindowFlags |= SDL_WINDOW_MAXIMIZED;
        }*/
        std::cout << "(after fetching) height : " << m_windowPos.h << std::endl;
        Window::init("Scheduler", m_windowPos.x, m_windowPos.y, m_windowPos.w, m_windowPos.h, pWindowFlags, pRenderFlags);
        SDL_GetWindowBordersSize(m_window, &p.x, &p.y, &p.h, &p.h);
        std::cout << "top : " << p.x << " left : " << p.y << " bottom : " << p.w << " right : " << p.h << std::endl;
        if (m_windowPos.x == 0 || m_windowPos.y == 0 || m_windowPos.x < 0)
        {
            m_windowPos.x = 0;
            m_windowPos.y = p.x;
            SDL_SetWindowPosition(m_window, m_windowPos.x, m_windowPos.y);
        }
        SDL_SetWindowMinimumSize(m_window, MIN_WINDOW_WIDTH, MAX_WINDOW_HEIGHT);

        SDL_SetWindowIcon(this->getWindow(), IMG_Load(std::string(App::m_path[MAINAPP_PATH] + "Icons/SchedulerIcon.png").c_str()));

        // Setup Dear ImGui context
        ImGui::CreateContext();
        ImPlot::CreateContext();
        io = &ImGui::GetIO();
        (void)io;

        // init about windows
        aboutWindowThread = SDL_CreateThread(aboutWindow, "aboutWindow", (void *)nullptr);

        // Setup Dear ImGui style
        ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
        ImGui_ImplSDLRenderer_Init(m_renderer);

        states = new AppStateMachine();
        ImFontConfig font_config;
        // font_config.FontBuilderFlags|=ImGuiFreeTypeBuilderFlags_ForceAutoHint;
        io->Fonts->AddFontFromFileTTF(std::string(App::m_path[MAINAPP_PATH] + "fonts/verdana.ttf").c_str(), 16.0f, &font_config);

        // loading icon fonts ...
        static ImWchar *icons_ranges = new ImWchar[3];
        icons_ranges[0] = ICON_MIN_MD;
        icons_ranges[1] = ICON_MAX_MD;
        icons_ranges[2] = 0;

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphOffset = ImVec2(0, 5);
        // icons_config.FontBuilderFlags|=ImGuiFreeTypeBuilderFlags_ForceAutoHint;
        io->Fonts->AddFontFromFileTTF(std::string(App::m_path[MAINAPP_PATH] + "Icons/MaterialIcons-Regular.ttf").c_str(), 20.0f, &icons_config, icons_ranges);

        ImFontConfig cfg;
        // cfg.FontBuilderFlags|=ImGuiFreeTypeBuilderFlags_Bold|ImGuiFreeTypeBuilderFlags_ForceAutoHint;
        cft = io->Fonts->AddFontFromFileTTF(std::string(App::m_path[MAINAPP_PATH] + "fonts//verdana.ttf").c_str(), 16.0f, &cfg);
        // io->Fonts->Build();

        states->pushState(new MainState(this));

        if (argc > 1)
        {
            for (int i = 1; i < argc; i++)
            {
                std::cout << argv[i] << std::endl;
                loadParam(argv[i]);
            }
        }
        std::cout << "(done to display) height : " << m_windowPos.h << std::endl;
    }

    void loadParam(char *path);

    void OpenAboutWindow();

    static const char *LangData(std::string s, std::string icon = "")
    {
        std::string *text = new std::string(icon + LangMap[s][m_appLanguage]);
        return text->c_str();
    }

    static App *Instance()
    {
        if (m_app == NULL)
        {
            m_app = new App();
        }
        return m_app;
    }

    void setDarkTheme();

    void setLightTheme();

    void update() override;

    void draw() override;

    void checkEvent() override;

    void clean() override;

    void AboutWindowPopup();

    void handleEvent() override;

    void saveSettingFile();
};
typedef App theApp;
void openFile();
#endif // MAINAPP_H_INCLUDED
