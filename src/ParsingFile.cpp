#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"
#include "MainApp.h"
#include "Screens/SplashScreen.h"
Lang lg = EN_en;
void initParam(bool isOnSaving)
{

    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "UTF-8", "");
    doc.LinkEndChild(decl);

    std::cout << "initialisation Setting File ...  " << std::endl;
    TiXmlElement *setting, *window;
    setting = new TiXmlElement("Scheduler_setting_file");
    doc.LinkEndChild(setting);

    window = new TiXmlElement("window");
    setting->LinkEndChild(window);

    //==== Setting Window Size ====
    window->SetAttribute("w", !isOnSaving ? App::m_windowPos.w
                                          : theApp::Instance()->getWindowWidth());
    window->SetAttribute("h", !isOnSaving ? App::m_windowPos.h
                                          : theApp::Instance()->getWindowHeight());
    int w, h;
    if (!isOnSaving)
    {
        window->QueryIntAttribute("w", &App::m_windowPos.w);
        window->QueryIntAttribute("h", &App::m_windowPos.h);
    }

    window->SetAttribute("x", !isOnSaving ? 100
                                          : theApp::Instance()->getWindowPosition().x);

    window->SetAttribute("y", !isOnSaving ? 100
                                          : theApp::Instance()->getWindowPosition().y);

    //== setting Language app ==
    TiXmlElement *language = new TiXmlElement("language");
    setting->LinkEndChild(language);
    if (!isOnSaving)
    {

        SDL_Locale *lang = SDL_GetPreferredLocales();

        if (lang == NULL)
        {
            App::m_appLanguage = EN_en;
        }
        else
        {
            if (strcmp(lang->language, "fr") == 0)
            {
                App::m_appLanguage = FR_fr;
            }
            else
            {
                App::m_appLanguage = EN_en;
            }
        }
    }
    language->SetAttribute("value", (int)App::m_appLanguage);

    //== setting theme data ==
    TiXmlElement *theme = new TiXmlElement("theme");
    theme->SetAttribute("value", App::m_appTheme);
    setting->LinkEndChild(theme);

    //== setting color data ==
    TiXmlElement *colorData = new TiXmlElement("colorData");
    setting->LinkEndChild(colorData);

    initColorSetting(colorData);

    if (!doc.SaveFile(App::m_path[SETTING_PATH].c_str()))
    {
        std::cout << "Error to Saving Setting File ..... " << doc.ErrorDesc() << std::endl;
        return;
    }
    std::cout << "[Done]" << std::endl;
    // system("PAUSE");
}

void initColorSetting(TiXmlElement *linkTo)
{

    //== wait color process
    TiXmlElement *color = new TiXmlElement("color");
    color->SetAttribute("name", "color_wait_processes");
    color->SetDoubleAttribute("r", App::color_wait.x);
    color->SetDoubleAttribute("g", App::color_wait.y);
    color->SetDoubleAttribute("b", App::color_wait.z);
    color->SetDoubleAttribute("a", App::color_wait.w);
    linkTo->LinkEndChild(color);

    //== exec color process
    color = new TiXmlElement("color");
    color->SetAttribute("name", "color_exec_processes");
    color->SetDoubleAttribute("r", App::color_exec.x);
    color->SetDoubleAttribute("g", App::color_exec.y);
    color->SetDoubleAttribute("b", App::color_exec.z);
    color->SetDoubleAttribute("a", App::color_exec.w);
    linkTo->LinkEndChild(color);
}

const char *initColorSetting()
{
    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "UTF-8", "");
    doc.LinkEndChild(decl);

    TiXmlElement *setting;
    setting = new TiXmlElement("Scheduler_setting_file");
    doc.LinkEndChild(setting);

    initColorSetting(setting);

    TiXmlPrinter printer;
    doc.Accept(&printer);
    const char *xmlcstr = printer.CStr();
    return xmlcstr;
}
void parsingChildElement(TiXmlElement *elmt)
{
    for (TiXmlElement *e = elmt->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        // std::cout << e->Value() << std::endl;
        // std::cout << e->Attribute("name") << std::endl;
        if (!e->NoChildren())
        {
            parsingChildElement(e);
        }
        if (e->Attribute("id") != NULL)
        {
            if (e->Attribute("value") != NULL)
            {
                // std::cout << e->Attribute("id") << " : " << e->Attribute("value") << std::endl;
                App::LangMap[e->Attribute("id")][lg] = e->Attribute("value");
            }
        }
        if (strcmp(e->Value(), "color") == 0)
        {
            char name[strlen(e->Attribute("name")) + 1];
            strcpy(name, e->Attribute("name"));

            if (strcmp(name, "color_wait_processes") == 0)
            {
                double col;
                e->Attribute("r", &col);
                App::color_wait.x = col;
                std::cout << "r : " << col << std::endl;

                e->Attribute("g", &col);
                App::color_wait.y = col;
                std::cout << "g : " << col << std::endl;

                e->Attribute("b", &col);
                App::color_wait.z = col;
                std::cout << "b : " << col << std::endl;

                e->Attribute("a", &col);
                App::color_wait.w = col;
                std::cout << "a : " << col << std::endl;
            }

            if (strcmp(name, "color_exec_processes") == 0)
            {
                double col;
                e->Attribute("r", &col);
                App::color_exec.x = col;
                std::cout << "r: " << col << std::endl;

                e->Attribute("g", &col);
                App::color_exec.y = col;
                std::cout << "g: " << col << std::endl;

                e->Attribute("b", &col);
                App::color_exec.z = col;
                std::cout << "b: " << col << std::endl;

                e->Attribute("a", &col);
                App::color_exec.w = col;
                std::cout << "a: " << col << std::endl;
            }
        }
    }
}

bool parseSettingFile()
{
    std::cout << "+ Starting to parsing Setting File .... " << std::endl;
    TiXmlDocument doc;
    if (!doc.LoadFile(App::m_path[SETTING_PATH].c_str()))
    {
        std::cout << "Impossible to Load Setting File Setting File .... " << std::endl;
        return false;
    }

    TiXmlElement *root = doc.RootElement();
    parseSetting(root);

    std::cout << "- [Done]" << std::endl;
    return true;
}

void initLang(Lang l, TiXmlDocument doc)
{
    lg = l;
    TiXmlElement *root = doc.RootElement();
    parsingChildElement(root);
}

void parseSetting(TiXmlElement *root)
{
    for (TiXmlElement *e = root->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        if (strcmp(e->Value(), "window") == 0)
        {
            e->Attribute("x", &App::m_windowPos.x);
            e->Attribute("y", &App::m_windowPos.y);
            e->Attribute("w", &App::m_windowPos.w);
            e->Attribute("h", &App::m_windowPos.h);
        }

        if (strcmp(e->Value(), "language") == 0)
        {
            int vlang = 0;
            e->Attribute("value", &vlang);
            App::m_appLanguage = (Lang)vlang;
        }

        if (strcmp(e->Value(), "theme") == 0)
        {
            int vTheme;
            e->Attribute("value", &vTheme);
            App::m_appTheme = (Theme)vTheme;
        }
        // std::cout << e->Value() << std::endl;

        if (strcmp(e->Value(), "colorData") == 0)
        {
            parsingChildElement(e);
        }
    }
}

bool parseLangFile()
{
    std::cout << "+ Starting to parsing Language File .... " << std::endl;
    TiXmlDocument doc;
    std::string langFile = App::m_path[LANG_PATH] + "lang_en.xml";
    if (!doc.LoadFile(langFile.c_str()))
    {
        std::cout << "Impossible to Load Language File .... :  " << doc.ErrorDesc() << std::endl;
        return false;
    }

    // load english file ...
    std::cout << "    + parsing English file!" << std::endl;
    initLang(EN_en, doc);
    std::cout << "    - [Done]" << std::endl;

    // load french file ...
    langFile = App::m_path[LANG_PATH] + "lang_fr.xml";
    if (!doc.LoadFile(langFile.c_str()))
    {
        std::cout << "Impossible to Load Language File .... :  " << doc.ErrorDesc() << std::endl;
        return false;
    }
    std::cout << "    + parsing French file!" << std::endl;
    initLang(FR_fr, doc);
    std::cout << "    - [Done]" << std::endl;

    std::cout << "- [Done]" << std::endl;
    return true;
}

#if EMSCRIPTEN

void saveStringLocalStorage(const std::string &key, const std::string &value)
{
    EM_ASM(
        {
            localStorage.setItem(UTF8ToString($0), UTF8ToString($1));
        },
        key.c_str(), value.c_str());
}

void saveColorSetting()
{
    saveStringLocalStorage("setting", initColorSetting());
}

const char *getStringLocalStorage(const std::string &key)
{
    int ptr = EM_ASM_INT(
        {
            var value = localStorage.getItem(UTF8ToString($0));
            if (value == null)
                return 0;
            var length = lengthBytesUTF8(value) + 1;
            var buffer = _malloc(length);
            stringToUTF8(value, buffer, length);
            return buffer;
        },
        key.c_str());
    if (ptr == 0)
    {
        return "";
    }
    return (char *)ptr;
}
#endif
bool MainSplashState::initSettingFile()
{
#if !EMSCRIPTEN
    /*TiXmlDocument doc;
        doc.Parse("<?xml version='1.0' encoding='UTF-8'?> <project version='4'><component name='JavaScriptSettings'><option name='languageLevel' value='ES6' /></component></project>");
        TiXmlElement *root = doc.RootElement();
        if (!root->NoChildren())
        {
            std::cout << root->FirstChildElement()->FirstAttribute()->Name() << std::endl;
        }*/
    FILE *fileSetting = fopen(App::m_path[SETTING_PATH].c_str(), "r");
    if (fileSetting == NULL)
    {
        std::cout << "file setting not found !!!" << std::endl;
        initParam();
        return false;
    }
    parseSettingFile();
    fclose(fileSetting);

#else
    std::string settingValue = getStringLocalStorage("setting");
    if (settingValue == "")
    {
        saveStringLocalStorage("setting", initColorSetting());
        settingValue = initColorSetting();
    }
    TiXmlDocument doc;
    doc.Parse(settingValue.c_str(), 0, TIXML_ENCODING_UTF8);
    TiXmlElement *root = doc.RootElement();
    std::cout << "parsing " << settingValue.c_str() << std::endl;
    parsingChildElement(root);

    App::m_appLanguage = (Lang)EM_ASM_INT(
        {
            var lang = navigator.language || navigator.userLanguage;
            var codeLang = $0;
            if (lang.startsWith('fr'))
            {
                codeLang = $1;
            }

            return localStorage.getItem('lang') || codeLang;
        },
        EN_en, FR_fr);
    App::m_appTheme = (Theme)EM_ASM_INT(
        {
            return localStorage.getItem('theme') || $0;
        },
        LIGHT_THEME);
    EM_ASM({
        Module.windowWidth = document.body.clientWidth;
        Module.windowHeight = document.documentElement.offsetHeight;
        var canvas = document.getElementById('canvas');
        canvas.style.width = Module.windowWidth + 'px';
        // canvas.style.height=Module.windowHeight +'px';
        window.addEventListener(
            'resize', function() {
                var canvas = document.getElementById('canvas');
                Module.windowWidth = document.body.clientWidth;
                Module.windowHeight = document.documentElement.offsetHeight;
                canvas.style.width = Module.windowWidth + 'px';
            });
    });
    App::m_windowPos.w = EM_ASM_INT({ return Module.windowWidth; });
    App::m_windowPos.h = EM_ASM_INT({ return Module.windowHeight; });
    std::cout << "(fetching) height : " << App::m_windowPos.h << std::endl;

#endif
    return true;
}

bool MainSplashState::initLangFile()
{
    return parseLangFile();
}
