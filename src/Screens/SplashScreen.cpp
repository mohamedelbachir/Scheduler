#include "../MainApp.h"
#include "SplashScreen.h"
#include <fstream>
std::string MainSplashState::stateID = "MainSplashState";
bool MainSplashState::taskThreadDone = false;

int MainSplashState::loadInitialisation(void *ptr)
{
#if !EMSCRIPTEN
    // get Main App Directory
    char characterDirectory;
    App::m_path[MAINAPP_PATH] = SDL_GetBasePath();

    // std::cout<<App::m_path[MAINAPP_PATH];
    characterDirectory = App::m_path[MAINAPP_PATH].back();
#else
    char characterDirectory = '/';
#endif

    App::m_path[LANG_PATH] = App::m_path[MAINAPP_PATH] + "lang" + characterDirectory;
    std::cout << "lang : " << App::m_path[LANG_PATH];

#if !EMSCRIPTEN
    // get Preference App directory to storage
    std::string PrefPath = SDL_GetPrefPath("Scheduler", "");
    PrefPath.pop_back();

    App::m_path[SAVE_PATH] = PrefPath;

    // saving setting path file
    App::m_path[SETTING_PATH] = PrefPath + std::string("setting.ini");

    // fetching recent items file
    App::m_path[RECENT_PATH] = PrefPath + std::string("recent.ini");

    // read recent file
    std::ifstream recent(App::m_path[RECENT_PATH]);
    std::string currentFile;
    while (std::getline(recent, currentFile))
    {

        // check if the file still exist
        std::ifstream fic(currentFile);
        if (fic)
        {
            App::m_recentItems.push_back(currentFile);
        }
        fic.close();
    }
#endif
    initSettingFile();
    initLangFile();
    SDL_Delay(300);
    taskThreadDone = true;
    return 0;
}

bool MainSplashState::onEnter()
{
    SDL_Delay(100);
    theTextureManager::Instance()->load(App::m_path[MAINAPP_PATH] + "splash/SchedulerSplashScreen.jpg", "splash", referenceWindow->getRenderer());
    thefontManager::Instance()->load(App::m_path[MAINAPP_PATH] + "fonts/candara.ttf", 30, "splashTextFont");
    thefontManager::Instance()->load(App::m_path[MAINAPP_PATH] + "fonts/candara.ttf", 20, "splashTextVerSion");
    thefontManager::Instance()->load(App::m_path[MAINAPP_PATH] + "fonts/candara.ttf", 10, "splashText");

    listWidget.push_back(new UI_Image("splash", 0, 0));
    UI_Text *appName = new UI_Text(referenceWindow->getRenderer(), APPNAME, 20, 100, "splashTextFont");
    std::string appVersionLabel = "Version : " + APPVERSION;
    UI_Text *appVersion = new UI_Text(referenceWindow->getRenderer(), appVersionLabel.c_str(), appName->getVector()->getX(), appName->getVector()->getY() + appName->getHeight() + 5, "splashTextVerSion");
    listWidget.push_back(appName);
    listWidget.push_back(appVersion);

    return true;
}
bool MainSplashState::onExit()
{
    SDL_DetachThread(fetch);
    for (auto widget : listWidget)
    {
        widget->clean();
    }
    listWidget.clear();
    return true;
}

void MainSplashState::update()
{
    for (auto widget : listWidget)
    {
        widget->update(&this->referenceWindow->m_event);
    }
}

void MainSplashState::render()
{
    for (auto widget : listWidget)
    {
        widget->draw(referenceWindow->getRenderer());
    }
    if (taskThreadDone)
    {
        referenceWindow->quit();
    }
}

std::string MainSplashState::getStateID() const
{
    return stateID;
}
