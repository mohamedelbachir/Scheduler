#include "MainApp.h"
#include"tinyfiledialogs.h"
#include "TaskState.h"
#include"AboutState.h"
//===== static variable initialisation =====

App* App::m_app=NULL;
std::map<Path,std::string>App::m_path;
std::map<std::string,std::map<Lang,std::string>> App::LangMap;

ImVec4 App::color_exec = ImVec4(0, 0, 1, 1);
ImVec4 App::color_wait = ImVec4{1,0,0,1};

SDL_Rect App::m_windowPos={0};

Theme App::m_appTheme=LIGHT_THEME;
Lang App::m_appLanguage=EN_en;

ImFont *App::cft=nullptr;

std::vector<std::string> App::m_recentItems;

//=============

bool isEqual(ImVec4 a,ImVec4 b){
    return a.x==b.x&&a.y==b.y&&a.w==b.w&&a.z==b.z;
}

int App::fetchingData(){
    SplashScreen splash("splash",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,655,285,SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS|SDL_WINDOW_SKIP_TASKBAR|SDL_WINDOW_ALWAYS_ON_TOP);
    return splash.run();
}

int App::aboutWindow(void *ptr){
    return theAboutWindow::Instance()->run();
}


void App::update()
{
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    if(m_appTheme==DARK_THEME){
        setDarkTheme();
    }else{
        setLightTheme();
    }

    ImGui::NewFrame();
    mainMenu();
    states->update();
    ImGui::EndFrame();


    /*if(!ImGui::IsKeyReleased(ImGuiKey_O)&&ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_O)){
        openFile();
    }*/
}

void App::draw()
{
    ImGui::Render();
    ImGuiStyle* style=&ImGui::GetStyle();
    //color of background renderer
    SDL_SetRenderDrawColor(this->getRenderer(),style->Colors[ImGuiCol_WindowBg].x*255
                                              ,style->Colors[ImGuiCol_WindowBg].y*255
                                              ,style->Colors[ImGuiCol_WindowBg].z*255
                                              ,style->Colors[ImGuiCol_WindowBg].w*255);
    SDL_RenderClear(getRenderer());
    states->render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(getRenderer());
}

void closeTabs(){
    TabShedule::Instance()->CloseTabs();
}

void App::checkEvent()
{
    Window::checkEvent();

    switch(e.drop.type)
    {
        case SDL_DROPBEGIN:
        case SDL_DROPCOMPLETE:
            isDroppedFile=true;
            break;
        case SDL_DROPFILE:
            TabShedule::Instance()->addTab(e.drop.file);
            theApp::Instance()->states->changeState(new TaskState(theApp::Instance()));
            break;
        default:
            isDroppedFile=false;
        break;
    }

    if(!m_running){
        closeTabs();
    }
}

void App::clean()
{
    saveSettingFile();
    clearData();
    states->clear();
    Window::clean();
}

void App::setDarkTheme(){
    ImGui::StyleColorsDark();
}

void App::setLightTheme(){
    ImGui::StyleColorsLight();
}

void openFile(){
    char const * lFilterPatterns[1]={"*.shd"};
    char *files=tinyfd_openFileDialog(App::LangData("idOpen"),NULL,1,lFilterPatterns,NULL,1);
    if(files!=NULL){
        TabShedule::Instance()->addTab(files);
        theApp::Instance()->states->changeState(new TaskState(theApp::Instance()));
    }
    return;
}

void App::loadParam(char *path){
    std::cout<<path<<std::endl;
    TabShedule::Instance()->addTab(path);
    theApp::Instance()->states->changeState(new TaskState(theApp::Instance()));
}

void App::OpenAboutWindow(){
    SDL_ShowWindow(theAboutWindow::Instance()->getWindow());
}

void App::mainMenu()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,0.0f);
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,1);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(8,8));
        if (ImGui::BeginMenu(LangData("idfile")))
        {
            if (ImGui::MenuItem(LangData("idNew"))) {
                TabShedule::Instance()->addTab();
                states->changeState(new TaskState(this));
            }
            if (ImGui::MenuItem(LangData("idOpen"), "Ctrl+O")) {
                openFile();
            }

            ImGui::BeginDisabled(TabShedule::Instance()->tabNumber()==0);
                if (ImGui::MenuItem(LangData("idSave"),"Ctrl+S")){
                    TabShedule::Instance()->saveActiveFile();
                }
            ImGui::EndDisabled();

            ImGui::Separator();
            if (ImGui::BeginMenu(LangData("idRecent"))) {
                for(size_t i=0;i<m_recentItems.size();i++){
                    char p[255];
                    sprintf(p,"%d %s",i+1,m_recentItems[i].c_str());
                    if(ImGui::MenuItem(p)){
                        TabShedule::Instance()->addTab((char*)m_recentItems[i].c_str());
                        theApp::Instance()->states->changeState(new TaskState(theApp::Instance()));
                    }
                }
                ImGui::BeginDisabled(m_recentItems.size()==0);
                    ImGui::Separator();
                    if(ImGui::Button(App::LangData("idBtnClearRecent",ICON_MD_CLEAR_ALL),ImVec2(ImGui::GetContentRegionAvail().x,30))){
                        m_recentItems.clear();
                        FILE *recent=fopen(m_path[RECENT_PATH].c_str(),"w+");
                        fclose(recent);
                    }
                ImGui::EndDisabled();
                ImGui::EndMenu();
            }

            ImGui::Separator();
            if (ImGui::MenuItem(LangData("idExit"),"Alt+F4")) {
                m_running=false;
                closeTabs();
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu(LangData("idOption"))){
            if(ImGui::BeginMenu(LangData("idlang",ICON_MD_LANGUAGE))){
                ImGui::RadioButton(LangData("idFR"),(int*)&m_appLanguage,FR_fr);
                ImGui::RadioButton(LangData("idEN"),(int*)&m_appLanguage,EN_en);
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu(LangData("idConfigParam",ICON_MD_SETTINGS))){

                ImGui::Text(LangData("idTextColorInExec"));
                ImGui::ColorEdit3("##clear color", (float*)&color_exec);

                ImGui::Separator();
                    ImGui::Text(LangData("idTextColorInWait"));
                    ImGui::ColorEdit3("##couleur_wait",(float*)&color_wait);

                ImGui::Separator();
                    ImGui::PushStyleColor(ImGuiCol_Button,ImColor(255,0,0,255).Value);
                    ImGui::PushStyleColor(ImGuiCol_Text,ImColor(255,255,255,255).Value);
                    ImGui::BeginDisabled(isEqual(color_exec,ImVec4(0,0,1,1))&&isEqual(color_wait,ImVec4(1,0,0,1)));
                    if(ImGui::Button(App::LangData("idbtnRenitialize"),ImVec2(ImGui::GetContentRegionAvail().x,30))){
                        color_exec=ImVec4(0,0,1,1);
                        color_wait=ImVec4(1,0,0,1);
                    }
                    ImGui::EndDisabled();
                    ImGui::PopStyleColor(2);
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu(LangData("idTheme",ICON_MD_COLORIZE))){
                ImGui::RadioButton(LangData("idLightTheme"),(int*)&m_appTheme, LIGHT_THEME);
                ImGui::Separator();
                ImGui::RadioButton(LangData("idDarkTheme"),(int*)&m_appTheme,DARK_THEME);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu(LangData("idHelp"))){
            if(ImGui::MenuItem(LangData("idTutorial"))){
                if(m_appLanguage==FR_fr){
                    SDL_OpenURL(std::string(App::m_path[MAINAPP_PATH]+"manual/help_fr.html").c_str());
                }else{
                    SDL_OpenURL(std::string(App::m_path[MAINAPP_PATH]+"manual/help_en.html").c_str());
                }
            }

            if(ImGui::MenuItem(LangData("idContrib"))){
                SDL_OpenURL("https://github.com/mohamedelbachir/scheduler/pulls");
            }

            if(ImGui::MenuItem(LangData("idUpdate",ICON_MD_UPDATE))){
                SDL_OpenURL("https://mohamedelbachir.github.com/scheduler");
            }

            if(ImGui::MenuItem(LangData("idreport",ICON_MD_BUG_REPORT))){
                SDL_OpenURL("https://github.com/mohamedelbachir/scheduler/issues");
            }

            ImGui::Separator();
            if(ImGui::MenuItem(LangData("idAbout"))){
                OpenAboutWindow();
            }
            ImGui::EndMenu();
        }

        ImGui::PopStyleVar(2);
        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleVar();
}
