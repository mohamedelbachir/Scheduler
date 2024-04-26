#pragma once
#ifndef SCHEDULEITEM_H_INCLUDED
#define SCHEDULEITEM_H_INCLUDED

#include "../imgui/imgui.h"
#include "../Utils.h"

#define USERINPUTERROR -1
#define CORRECTUSERINPUT 0

#include "../tinyxml/tinystr.h"
#include "../tinyxml/tinyxml.h"

// initialisation minimum variable
const ImU16 u16_one = 1;
const ImU8 u8_one = 1;

static const char *lettre[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};

enum tabState
{
    MAIN_STATE,
    DISPLAY_STATE
};

struct DataField
{
};

class ScheduleTaskTab
{
private:
    bool m_showPopus = false;

    bool m_showLegend = true, m_showSynopsis = true, m_showTOP = true, m_resetView = true;

    std::vector<int> m_process_checked;

    int m_tabID;

    bool isPidChecked = false;

    int n_process_generated = 0;

    bool m_generated = false;

    tabState m_current_state;

    std::string m_nameTab = "";

    std::string m_name;

    ImU16 m_numberOfProcess;

    int m_stypeSchedule = 0;

    ScheduleType m_typeSchedule;

    ScheduleGroup m_groupSchedule;

    AlgoGenerationType m_algoGenType;

    ImU8 m_quantum = 1;

    bool withPriority = false;

    std::vector<Process> m_processes;

    std::vector<ProcessChunck> m_chunck_process;

    ImGuiTabItemFlags flagsTab = ImGuiTabItemFlags_UnsavedDocument;

    void mainState();

    void displayState();

    void update();

    int alphabet = 0;

    DrawItem m_drawItems;

    void StartScheduling();

    char *pathSavedFile = NULL;

    void parseFromXML(char *pathXMLFile);

    void parsingScheduleElement(TiXmlElement *elmt);

public:
    static int ID;
    bool isSaved = false;

    bool isActive = true;

    bool saveFile();

    void showTableOfProcess(bool isEditable);

    void badInputPopup();

    bool isOpen;

    ScheduleTaskTab()
    {
        isOpen = true;
        m_current_state = MAIN_STATE;
        m_tabID = ID++;
        m_typeSchedule = SCH_FCFS;
        m_groupSchedule = NOPREEMPTIF;
        m_numberOfProcess = (ImU16)1;
        m_algoGenType = ALG_DEFAULT;

        char name[255];
        sprintf(name, " Scheduler %d", m_tabID);
        m_nameTab += name;
        m_name = m_nameTab;
    }

    ScheduleTaskTab(char *pathXMLFile);

    ~ScheduleTaskTab()
    {
        deleteAllprocess();
        m_chunck_process.clear();
        m_processes.clear();
        m_process_checked.clear();
        // m_drawItems.clean();
    }

    std::string getTabName()
    {
        return m_name;
    }

    char *getPath()
    {
        return pathSavedFile;
    }

    bool isCanSaved()
    {
        return m_generated && !isSaved;
    }

    void setTabName(std::string str)
    {
        char name[500];
        m_name = str;
        sprintf(name, " %s ##%d", str.c_str(), m_tabID);
        m_nameTab = name;
    }

    void deleteSelectedProcess()
    {
        int k = 0;
        std::sort(m_process_checked.begin(), m_process_checked.end());
        for (auto i : m_process_checked)
        {
            std::vector<Process>::iterator it = m_processes.begin();
            m_processes.erase(it + i - k);
            k++;
        }
        m_process_checked.clear();
    }

    int VerifiedUserData()
    {
        if (m_algoGenType == ALG_DEFAULT)
        {
            for (size_t i = 0; i < m_processes.size(); i++)
            {
                std::string pId = m_processes[i].Sid;
                for (size_t row = 0; row < m_processes.size(); row++)
                {
                    if ((row != i && m_processes[row].Sid == pId) || strlen(m_processes[row].Sid) < 1)
                    {
                        return USERINPUTERROR;
                    }
                }
            }
        }
        return CORRECTUSERINPUT;
    }

    void AddProcess()
    {
        Process p;
        p.Id = n_process_generated;
        switch (m_algoGenType)
        {
        case ALG_ALPHA_NUM:
            sprintf(p.Sid, "%c%d", (char)(alphabet + 65), n_process_generated + 1);
            break;
        case ALG_NUM:
            sprintf(p.Sid, "%d", n_process_generated + 1);
            break;
        case ALG_DEFAULT:
            sprintf(p.Sid, "%s", "");
            break;
        case ALG_ALPHA:
            int n_car = n_process_generated;
            char ouput[255] = "";
            while (n_car >= 26)
            {
                strcat(ouput, lettre[n_car % 26]);
                n_car /= 26;
            }
            strcat(ouput, lettre[n_process_generated < 26 ? n_car : n_car - 1]);
            sprintf(p.Sid, "%s", ouput);
            // delete []ouput;
            break;
        }
        m_processes.push_back(p);
        n_process_generated++;
        if (isPidChecked)
            isPidChecked = false;
    }

    void deleteProcess(int id)
    {
        std::vector<Process>::iterator it = m_processes.begin() + id;
        m_processes.erase(it);
    }

    void deleteAllprocess()
    {
        m_process_checked.clear();
        for (size_t i = 0; i < m_processes.size(); i++)
        {
            m_process_checked.push_back(i);
        }
        deleteSelectedProcess();
        m_processes.clear();
        m_numberOfProcess = 1;
        n_process_generated = 0;
        isPidChecked = false;
    }

    void AddProcess(int n)
    {
        for (int i = 0; i < n; i++)
        {
            AddProcess();
        }
    }

    void render()
    {
        update();
        if (ImGui::BeginTabItem(m_nameTab.c_str(), &isOpen, flagsTab))
        {
            isActive = true;
            switch (m_current_state)
            {
            case MAIN_STATE:
                mainState();
                break;

            case DISPLAY_STATE:
                displayState();
                break;
            }
            ImGui::EndTabItem();
        }
        else
        {
            isActive = false;
        }
    }
};
#endif // SCHEDULEITEM_H_INCLUDED
