#include"AboutState.h"
std::string AboutState::stateID="AboutState";
AboutWindow* AboutWindow::m_aboutWindow=NULL;
bool AboutState::onEnter()
{
    //[LOAD RESSOURCE]
    thefontManager::Instance()->load(App::m_path[MAINAPP_PATH]+"fonts/verdana.ttf",20,"textFont");
    thefontManager::Instance()->load(App::m_path[MAINAPP_PATH]+"fonts/verdana.ttf",13,"textInfo");
    theTextureManager::Instance()->load(App::m_path[MAINAPP_PATH]+"icons/Scheduler.png","logoIcon",referenceWindow->getRenderer());

    UI_Image *icon=new UI_Image("logoIcon",20,20);
    icon->resize(200,200);

    UI_Text *appName=new UI_Text(referenceWindow->getRenderer(),APPNAME,icon->getVector()->getX()+icon->getWidth()+10,icon->getVector()->getY()+10,"textFont");
    std::string version="Version : "+APPVERSION;
    UI_Text *appVersion=new UI_Text(referenceWindow->getRenderer(),version.c_str()
                                                                  ,appName->getVector()->getX()
                                                                  ,appName->getVector()->getY()+appName->getHeight()+10
                                                                  ,"textInfo");

    UI_Text *appRelease=new UI_Text(referenceWindow->getRenderer(),"Copyright 2023"
                                                                  ,appVersion->getVector()->getX()
                                                                  ,appVersion->getVector()->getY()+appVersion->getHeight()+10
                                                                  ,"textInfo");


    author=App::LangData("idAuthor");
    author+=" : Mohamed El Bachir";
    authors=new UI_Text(referenceWindow->getRenderer(),author.c_str()
                                                                  ,appRelease->getVector()->getX()
                                                                  ,appRelease->getVector()->getY()+appRelease->getHeight()+10
                                                                  ,"textInfo");

    tools=App::LangData("idDevTools");
    tools+=" : DearImGUI ( UI ) , SDL , Freetype";
    utools=new UI_Text(referenceWindow->getRenderer(),tools.c_str()
                                                                  ,authors->getVector()->getX()
                                                                  ,authors->getVector()->getY()+authors->getHeight()+10
                                                                  ,"textInfo");
    authors->onUpdate([&]{
                        author=App::LangData("idAuthor");
                        author+=" : Mohamed El Bachir";
                        authors->changeTexte(author);

                        tools=App::LangData("idDevTools");
                        tools+=" : DearImGUI ( UI ) , SDL , Freetype";
                        utools->changeTexte(tools);
                      },2000);

    listWidget.push_back(icon);
    listWidget.push_back(appName);
    listWidget.push_back(appVersion);
    listWidget.push_back(appRelease);
    listWidget.push_back(authors);
    listWidget.push_back(utools);
    return true;
}

bool AboutState::onExit()
{
    for(auto widget:listWidget){
        widget->clean();
    }
    listWidget.clear();
    return true;
}

void AboutState::update()
{
    SDL_SetWindowTitle(referenceWindow->getWindow(),App::LangData("idAbout"));
    for(auto widget:listWidget){
        if(theApp::m_appTheme==DARK_THEME){
            widget->setAllStateColor(255,255,255,255);
        }else{
            widget->setAllStateColor(0,0,0,255);
        }
        widget->update(referenceWindow->getEvent());
    }
}

void AboutState::render()
{
    for(auto widget:listWidget){
        widget->draw(referenceWindow->getRenderer());
    }
}

std::string AboutState::getStateID()const
{
    return stateID;
}
