#pragma once
#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

enum Theme
{
    LIGHT_THEME,
    DARK_THEME
};
enum Lang
{
    EN_en,
    FR_fr
};
enum Path
{
    MAINAPP_PATH,
    SETTING_PATH,
    RECENT_PATH,
    SAVE_PATH,
    LANG_PATH
};

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 680

#define MIN_WINDOW_WIDTH 750
#define MAX_WINDOW_HEIGHT 522

#define MAJORVERSION 1
#define MINORVERSION 0
#define PATCHVERSION 0

#define APPNAME "Scheduler"
#define APPVERSION (std::string) "1.0.1"

#define URL_PULL_REQUEST "https://github.com/mohamedelbachir/scheduler/pulls"
#define URL_WEBSITE "https://mohamedelbachir.github.io/Scheduler/"
#define URL_REPORT "https://github.com/mohamedelbachir/scheduler/issues"

#define APP_EXTENSION "*.shd"

#define APP_EXTENSION_FILE ".shd"

#endif // UTILITY_H_INCLUDED