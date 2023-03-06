#include"tinyxml/tinystr.h"
#include"tinyxml/tinyxml.h"
#include"MainApp.h"
#include"SplashScreen.h"
Lang lg=EN_en;
void initParam(bool isOnSaving)
{

    TiXmlDocument doc;
    TiXmlDeclaration *decl=new TiXmlDeclaration("1.0","UTF-8","");
    doc.LinkEndChild(decl);

    std::cout<<"initialisation Setting File ...  "<<std::endl;
    TiXmlElement *setting,*window;
    setting=new TiXmlElement("Scheduler_setting_file");
    doc.LinkEndChild(setting);

    window=new TiXmlElement("window");
    setting->LinkEndChild(window);


    //==== Setting Window Size ====
    window->SetAttribute("w",!isOnSaving?App::m_windowPos.w
                                        :theApp::Instance()->getWindowWidth());
    window->SetAttribute("h",!isOnSaving?App::m_windowPos.h
                                        :theApp::Instance()->getWindowHeight());
    int w,h;
    if(!isOnSaving){
        window->QueryIntAttribute("w",&App::m_windowPos.w);
        window->QueryIntAttribute("h",&App::m_windowPos.h);
    }

    window->SetAttribute("x",!isOnSaving?100
                                        :theApp::Instance()->getWindowPosition().x);

    window->SetAttribute("y",!isOnSaving?100
                                        :theApp::Instance()->getWindowPosition().y);

    //== setting Language app ==
    TiXmlElement *language=new TiXmlElement("language");
    setting->LinkEndChild(language);
    if(!isOnSaving){

        SDL_Locale *lang=SDL_GetPreferredLocales();

        if(lang==NULL)
        {
            App::m_appLanguage=EN_en;
        }
        else
        {
            if(strcmp(lang->language,"fr")== 0)
            {
                App::m_appLanguage=FR_fr;
            }
            else
            {
                App::m_appLanguage=EN_en;
            }
        }
    }
    language->SetAttribute("value",(int)App::m_appLanguage);

    //== setting theme data ==
    TiXmlElement *theme=new TiXmlElement("theme");
    theme->SetAttribute("value",App::m_appTheme);
    setting->LinkEndChild(theme);

    //== setting color data ==
    TiXmlElement *colorData=new TiXmlElement("colorData");
    setting->LinkEndChild(colorData);

    //== wait color process
    TiXmlElement *color=new TiXmlElement("color");
        color->SetAttribute("name","color_wait_processes");
        color->SetDoubleAttribute("r",App::color_wait.x);
        color->SetDoubleAttribute("g",App::color_wait.y);
        color->SetDoubleAttribute("b",App::color_wait.z);
        color->SetDoubleAttribute("a",App::color_wait.w);
    colorData->LinkEndChild(color);

    color=new TiXmlElement("color");
        color->SetAttribute("name","color_exec_processes");
        color->SetDoubleAttribute("r",App::color_exec.x);
        color->SetDoubleAttribute("g",App::color_exec.y);
        color->SetDoubleAttribute("b",App::color_exec.z);
        color->SetDoubleAttribute("a",App::color_exec.w);
    colorData->LinkEndChild(color);

    if(!doc.SaveFile(App::m_path[SETTING_PATH].c_str()))
    {
        std::cout<<"Error to Saving Setting File ..... "<<doc.ErrorDesc()<<std::endl;
        return;
    }
    std::cout<<"[Done]"<<std::endl;
}

void parsingChildElement(TiXmlElement *elmt){
   for(TiXmlElement *e=elmt->FirstChildElement();e!=NULL;e=e->NextSiblingElement()){
        if(!e->NoChildren()){
            parsingChildElement(e);
        }
        if(e->Attribute("id")!=NULL){
            if(e->Attribute("value")!=NULL){
                App::LangMap[e->Attribute("id")][lg]=e->Attribute("value");
            }
        }
        if(strcmp(e->Value(),"color")==0){
            char name[255];
            strcpy(name,e->Attribute("name"));

            if(strcmp(name,"color_wait_processes")==0){
                double col;
                e->Attribute("r",&col);
                App::color_wait.x=col;
                e->Attribute("g",&col);
                App::color_wait.y=col;
                e->Attribute("b",&col);
                App::color_wait.z=col;
                e->Attribute("a",&col);
                App::color_wait.w=col;
            }

            if(strcmp(name,"color_exec_processes")==0){
                double col;
                e->Attribute("r",&col);
                App::color_exec.x=col;
                e->Attribute("g",&col);
                App::color_exec.y=col;
                e->Attribute("b",&col);
                App::color_exec.z=col;
                e->Attribute("a",&col);
                App::color_exec.w=col;
            }
        }
   }
}

bool parseSettingFile()
{
    std::cout<<"+ Starting to parsing Setting File .... "<<std::endl;
    TiXmlDocument doc;
    if(!doc.LoadFile(App::m_path[SETTING_PATH].c_str())){
        std::cout<<"Impossible to Load Setting File Setting File .... "<<std::endl;
        return false;
    }

    TiXmlElement *root=doc.RootElement();
    for(TiXmlElement *e=root->FirstChildElement();e!=NULL;e=e->NextSiblingElement()){
        if(strcmp(e->Value(),"window")==0){
            e->Attribute("x",&App::m_windowPos.x);
            e->Attribute("y",&App::m_windowPos.y);
            e->Attribute("w",&App::m_windowPos.w);
            e->Attribute("h",&App::m_windowPos.h);
        }

        if(strcmp(e->Value(),"language")==0){
            int vlang=0;
            e->Attribute("value",&vlang);
            App::m_appLanguage=(Lang)vlang;
        }

        if(strcmp(e->Value(),"theme")==0){
            int vTheme;
            e->Attribute("value",&vTheme);
            App::m_appTheme=(Theme)vTheme;
        }

        if(strcmp(e->Value(),"colorData")==0){
            parsingChildElement(e);
        }
    }
    std::cout<<"- [Done]"<<std::endl;
    return true;
}

void initLang(Lang l,TiXmlDocument doc){
    lg=l;
    TiXmlElement *root=doc.RootElement();
    parsingChildElement(root);
}

bool parseLangFile(){
    std::cout<<"+ Starting to parsing Language File .... "<<std::endl;
    TiXmlDocument doc;
    std::string langFile=App::m_path[LANG_PATH]+"lang_en.xml";
    if(!doc.LoadFile(langFile.c_str())){
        std::cout<<"Impossible to Load Language File .... :  "<<doc.ErrorDesc()<<std::endl;
        return false;
    }

    //load english file ...
    std::cout<<"    + parsing English file!"<<std::endl;
    initLang(EN_en,doc);
    std::cout<<"    - [Done]"<<std::endl;

    //load french file ...
    langFile=App::m_path[LANG_PATH]+"lang_fr.xml";
    if(!doc.LoadFile(langFile.c_str())){
        std::cout<<"Impossible to Load Language File .... :  "<<doc.ErrorDesc()<<std::endl;
        return false;
    }
    std::cout<<"    + parsing French file!"<<std::endl;
    initLang(FR_fr,doc);
    std::cout<<"    - [Done]"<<std::endl;

    std::cout<<"- [Done]"<<std::endl;
    return true;
}

bool MainSplashState::initSettingFile(){
    FILE *fileSetting=fopen(App::m_path[SETTING_PATH].c_str(),"r");
    if(fileSetting==NULL){
        std::cout<<"file setting not found !!!"<<std::endl;
        initParam();
        return false;
    }
    parseSettingFile();
    fclose(fileSetting);
    return true;
}

bool MainSplashState::initLangFile(){
    return parseLangFile();
}

