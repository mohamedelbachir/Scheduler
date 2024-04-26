#pragma once
#ifndef TASKSTATE_H_INCLUDED
#define TASKSTATE_H_INCLUDED
#include "../MyTinyGUI/include/AppState.h"
#include "ScheduleItem.h"
#include "../IconHeader/IconsMaterialDesign.h"
#include <vector>
#include "../MainApp.h"

#if !EMSCRIPTEN
#include "../FileDialog/tinyfiledialogs.h"
#else
#include "../FileDialog/emscripten_browser_file.h"
#endif

enum answer
{
    YES,
    NO,
    CANCEL
};

class TabBarSchedule
{
private:
    static ImVector<int> tabs;
    std::string tabName = "MyTabBar";
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton;
    TabBarSchedule() {}
    int i_active = -1;
    int close_tab = -1;
    static TabBarSchedule *instance;
    std::vector<ScheduleTaskTab> tabItems;

public:
    static int n_instace;
    static TabBarSchedule *Instance()
    {
        if (instance == NULL)
        {
            instance = new TabBarSchedule();
        }
        return instance;
    }

    ~TabBarSchedule()
    {
        tabs.clear();
        tabItems.clear();
    }

    int tabNumber()
    {
        return tabItems.size();
    }

    void addTab()
    {
        ScheduleTaskTab t;
        tabItems.push_back(t);
        n_instace++;
    }

    int saveWarningPopup(std::string &desc);

    void addTab(char *filePath)
    {
        ScheduleTaskTab t(filePath);
        tabItems.push_back(t);
        if (t.getPath() == NULL)
        {
            ScheduleTaskTab::ID--;
            tabItems.pop_back();
        }
        else
        {
            n_instace++;
        }
    }

    void saveActiveFile()
    {
        if (i_active != -1)
        {
            if (tabItems[i_active].isCanSaved())
            {
                tabItems[i_active].isSaved = tabItems[i_active].saveFile();
            }
        }
    }

    void CloseTabs()
    {
        for (size_t i = 0; i < tabItems.size(); i++)
        {
            removeTab(i);
        }
        tabItems.clear();
    }

    void remove(int id)
    {
        std::vector<ScheduleTaskTab>::iterator it = tabItems.begin();
        tabItems.erase(it + id);
    }
    void removeTab(int id)
    {

        if (tabItems[id].isCanSaved())
        {
            int buttonid = -1;
            // tinyfd_messageBox("error",errorDesc.c_str(),"okcancel","error",0);
            std::string btnYes = App::LangData("idbtnYes");
            std::string btnNo = App::LangData("idbtnNo");
            std::string btnCancel = App::LangData("idbtnCancel");
            SDL_MessageBoxButtonData buttons[] = {
                // .flags, .buttonid, .text
                {2, CANCEL, btnCancel.c_str()},
                {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, NO, btnNo.c_str()},
                {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, YES, btnYes.c_str()}};

            std::string errorDesc = App::LangData("idUserUnsaveDesc");
            errorDesc += "\" ";
            errorDesc += tabItems[id].getTabName() + "  \" ";
            errorDesc += App::LangData("idUserUnsaveDescEnd");
            errorDesc += "\n\n";
            errorDesc += App::LangData("idUserUnsaveInfo");
            SDL_MessageBoxData messageboxdata = {
                SDL_MESSAGEBOX_WARNING,          /* .flags */
                theApp::Instance()->getWindow(), /* .window */
                App::LangData("idInfo"),         /* .title */
                errorDesc.c_str(),               /* .message */
                SDL_arraysize(buttons),          /* .numbuttons */
                buttons,                         /* .buttons */
                NULL};
#if !EMSCRIPTEN
            SDL_ShowMessageBox(&messageboxdata, &buttonid);
#else
            tabItems[id].isOpen = true;
            ImGui::OpenPopup(App::LangData("idInfo"));
            std::cout << "open the Warning popUp";
            // while (buttonid == -1)
            //{
            buttonid = saveWarningPopup(errorDesc);
            //}
#endif
            if (buttonid == 0)
            {
                if (tabItems[id].saveFile())
                {
                    remove(id);
                    close_tab = -1;
                }
            }
            else
            {
                if (buttonid == 1 || !tabItems[id].isCanSaved())
                {
                    remove(id);
                    close_tab = -1;
                }
                else
                {
                    close_tab = -1;
                    tabItems[id].isOpen = true;
                    if (!theApp::Instance()->isRunning())
                        theApp::Instance()->setIsRunnig(true);
                }
            }
        }
        else
        {
            remove(id);
            close_tab = -1;
        }
    }

    void render()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(3, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 3);
        if (ImGui::BeginTabBar(tabName.c_str(), tab_bar_flags))
        {
            if (ImGui::TabItemButton(ICON_MD_ADD_CHART, ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
            {
                addTab();
            }
            i_active = -1;
            for (size_t i = 0; i < tabItems.size(); i++)
            {
                tabItems[i].render();
                if (!tabItems[i].isOpen)
                {
                    close_tab = i;
                }
                if (tabItems[i].isActive)
                {
                    i_active = i;
                }
            }
            if (close_tab != -1)
            {
#if EMSCRIPTEN
                // tabItems[close_tab].isOpen = true;
#endif
                removeTab(close_tab);
            }
            ImGui::EndTabBar();
        }
        ImGui::PopStyleVar(3);
    }
};
typedef TabBarSchedule TabShedule;
class TaskState : public AppState
{
private:
public:
    static std::string stateID;
    TaskState(Window *pWindow) : AppState(pWindow) {}

    bool onEnter() override;
    bool onExit() override;

    void update() override;
    void render() override;

    std::string getStateID() const override;
};

#endif // TASKSTATE_H_INCLUDED
