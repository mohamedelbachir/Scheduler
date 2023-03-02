#pragma once
#ifndef MAINAPP_H_INCLUDED
#define MAINAPP_H_INCLUDED
#include"MyTinyGUI/include/Window.h"
#include"DearImgui/misc/freetype/imgui_freetype.h"
#include"MyTinyGUI/include/AppStateMachine.h"

#include"Icons/IconsMaterialDesign.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 680

#define MIN_WINDOW_WIDTH 750
#define MAX_WINDOW_HEIGHT 522

#include"MainAppState.h"

#include "SplashScreen.h"

#define APPNAME (std::string)"Scheduler"
#define APPVERSION (std::string)"1.0.0"

#include "ParsingFile.h"
enum Theme{LIGHT_THEME,DARK_THEME};
enum Lang{EN_en,FR_fr};
enum Path{MAINAPP_PATH,SETTING_PATH,RECENT_PATH,LANG_PATH};
class App:public Window{
private:

    bool isDroppedFile=false;

    SDL_Thread *aboutWindowThread=NULL;

    std::string fileDroped;

    Timer tOpenAbout;

    /**
        @brief mainMenu
        function to display main menu bar
    */
    void mainMenu();

    App():Window(){}

    ~App(){
        clearData();
    }

    App(std::string pTitle,int pX,int pY,int pW,int pH,Uint32 pFlagsWindow=SDL_WINDOW_SHOWN):Window(pTitle,pX,pY,pW,pH,pFlagsWindow){
        states=new AppStateMachine();
        states->pushState(new MainState(this));
    }

    App(int argc,char *argv[]):App("Scheduler",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT){
        //init(argc,argv);
    }

    /**
        @brief fetchchingDatat
        function to load saved data and lang when the application is initialize
    */
    int fetchingData();

public:
    ///path file storage
    static std::map<Path,std::string> m_path;

    ///recent path folder
    static std::vector<std::string> m_recentItems;

    ///lang pathFile
    static std::map<std::string,std::map<Lang,std::string>> LangMap;

    friend class MainSplashState;

    static App* m_app;
    AppStateMachine *states;

    static ImVec4 color_exec;
    static ImVec4 color_wait;
    static SDL_Rect m_windowPos;

    static Theme m_appTheme;
    static Lang m_appLanguage;

    static  ImFont *cft;

    void clearData(){
        delete cft;
        if(aboutWindowThread!=NULL)SDL_DetachThread(aboutWindowThread);
        LangMap.clear();
        m_recentItems.clear();
        m_path.clear();
    }

    static int aboutWindow(void *ptr);

    /**
        @brief init
        function to initialize the application
        @param argc : argument number
        @param argv : argument list
    */
    void init(int argc,char **argv){

        m_windowPos.x=SDL_WINDOWPOS_CENTERED;
        m_windowPos.y=SDL_WINDOWPOS_CENTERED;
        m_windowPos.w=WINDOW_WIDTH;
        m_windowPos.h=WINDOW_HEIGHT;

        m_path[MAINAPP_PATH]=argv[0];

        fetchingData();

        aboutWindowThread=SDL_CreateThread(aboutWindow,"aboutWindow",(void*)nullptr);

        Window::init("Scheduler",m_windowPos.x,m_windowPos.y,m_windowPos.w,m_windowPos.h,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE,true);
        SDL_SetWindowMinimumSize(m_window,MIN_WINDOW_WIDTH,MAX_WINDOW_HEIGHT);

        states=new AppStateMachine();

        ImFontConfig font_config;
        font_config.FontBuilderFlags|=ImGuiFreeTypeBuilderFlags_ForceAutoHint;
        io->Fonts->AddFontFromFileTTF("fonts\\verdana.ttf",16.0f,&font_config);

        //loading icon fonts ...
        static ImWchar *icons_ranges=new ImWchar[3];
        icons_ranges[0]=ICON_MIN_MD;
        icons_ranges[1]=ICON_MAX_MD;
        icons_ranges[2]=0;

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphOffset=ImVec2(0,5);
        icons_config.FontBuilderFlags|=ImGuiFreeTypeBuilderFlags_ForceAutoHint;
        io->Fonts->AddFontFromFileTTF( "Icons/MaterialIcons-Regular.ttf", 20.0f, &icons_config, icons_ranges);

        ImFontConfig cfg;
        cfg.FontBuilderFlags|=ImGuiFreeTypeBuilderFlags_Bold|ImGuiFreeTypeBuilderFlags_ForceAutoHint;
        cft=io->Fonts->AddFontFromFileTTF("fonts\\verdana.ttf",16.0f,&cfg);
        //io->Fonts->Build();

        states->pushState(new MainState(this));

        if(argc>1){
            loadParam(argv[1]);
        }
    }

    void loadParam(char *path);

    void OpenAboutWindow();

    static char* LangData(std::string s,std::string icon=""){
        return (char*)std::string(icon+LangMap[s][m_appLanguage]).c_str();
    }

    static App* Instance(){
        if(m_app==NULL){
            m_app=new App();
        }
        return m_app;
    }

    void setDarkTheme();

    void setLightTheme();

    void update()override;

    void draw()override;

    void checkEvent()override;

    void clean()override;

    void saveSettingFile(){
        initParam(true);
    }


};
typedef App theApp;
void openFile();
#endif // MAINAPP_H_INCLUDED
