#include"MainApp.h"
#include"SplashScreen.h"
#include<fstream>
std::string MainSplashState::stateID="MainSplashState";
bool MainSplashState::taskThreadDone=false;

int MainSplashState::loadInitialisation(void *ptr){

    //get Main App Directory
    char characterDirectory;
    App::m_path[MAINAPP_PATH]=SDL_GetBasePath();

    //std::cout<<App::m_path[MAINAPP_PATH];
    characterDirectory=App::m_path[MAINAPP_PATH].back();

    char LangPath[6];
    sprintf(LangPath,"%s%c","lang",characterDirectory);
    App::m_path[LANG_PATH]=LangPath;


    //get Preference App directory to storage
    std::string PrefPath=SDL_GetPrefPath("Scheduler","");
    PrefPath.pop_back();

    char SettingPath[PrefPath.size()];
    strcpy(SettingPath,PrefPath.c_str());


    //get Setting App directory
    char recentFilesPath[strlen(SettingPath)];
    strcpy(recentFilesPath,SettingPath);

    //saving setting path file
    App::m_path[SETTING_PATH]=SettingPath+std::string("setting.ini");

    //fetching recent items file
    App::m_path[RECENT_PATH]=recentFilesPath+std::string("recent.ini");


    //read recent file
    std::ifstream recent(App::m_path[RECENT_PATH]);
    std::string currentFile;
    while(std::getline(recent,currentFile)){

        //check if the file still exist
        std::ifstream fic(currentFile);
        if(fic){
            App::m_recentItems.push_back(currentFile);
        }
        fic.close();
    }

    initSettingFile();
    initLangFile();

    //delete allocated string memory for path
    delete []recentFilesPath;
    delete []SettingPath;
    delete []LangPath;
    SDL_Delay(300);
    taskThreadDone=true;
    return 0;
}
bool MainSplashState::onEnter()
{
    theTextureManager::Instance()->load("splash/SchedulerSplashScreen.jpg","splash",referenceWindow->getRenderer());
    thefontManager::Instance()->load("fonts/candara.ttf",30,"splashTextFont");
    thefontManager::Instance()->load("fonts/candara.ttf",20,"splashTextVerSion");
    thefontManager::Instance()->load("fonts/candara.ttf",10,"splashText");

    listWidget.push_back(new UI_Image("splash",0,0));
    UI_Text *appName=new UI_Text(referenceWindow->getRenderer(),APPNAME,20,100,"splashTextFont");
    std::string appVersionLabel="Version : "+APPVERSION;
    UI_Text *appVersion=new UI_Text(referenceWindow->getRenderer(),appVersionLabel.c_str()
                                                                  ,appName->getVector()->getX()
                                                                  ,appName->getVector()->getY()+appName->getHeight()+5
                                                                  ,"splashTextVerSion");
    listWidget.push_back(appName);
    listWidget.push_back(appVersion);
    return true;
}
bool MainSplashState::onExit()
{
    SDL_DetachThread(fetch);
    for(auto widget:listWidget){
        widget->clean();
    }
    listWidget.clear();
    return true;
}

void MainSplashState::update()
{
    for(auto widget:listWidget){
        widget->update(referenceWindow->getEvent());
    }
}

void MainSplashState::render()
{
    for(auto widget:listWidget){
        widget->draw(referenceWindow->getRenderer());
    }
    if(taskThreadDone){
        referenceWindow->quit();
    }
}

std::string MainSplashState::getStateID()const
{
    return stateID;
}
