#include "TaskState.h"
#include "MainAppState.h"
TabBarSchedule *TabBarSchedule::instance = NULL;
int TabBarSchedule::n_instace = 0;
std::string TaskState::stateID = "TaskState";
bool TaskState::onEnter()
{
    return true;
}
bool TaskState::onExit()
{
    return true;
}

int TabBarSchedule::saveWarningPopup(std::string &desc)
{
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    int result;
    if (ImGui::BeginPopupModal(App::LangData("idInfo"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(desc.c_str());
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button(App::LangData("idbtnYes"), ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            result = YES;
        }
        ImGui::SameLine();
        if (ImGui::Button(App::LangData("idbtnNo"), ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            result = NO;
        }
        ImGui::SameLine();
        if (ImGui::Button(App::LangData("idbtnCancel"), ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            result = CANCEL;
        }
        ImGui::EndPopup();
    }
    return result;
}

void TaskState::update()
{
    App::startWindow();
    TabShedule::Instance()->render();
    if (TabShedule::Instance()->tabNumber() == 0)
    {
        theApp::Instance()->states->changeState(new MainState(referenceWindow));
        TabShedule::Instance()->n_instace = 0;
        ScheduleTaskTab::ID = 0;
    }
    else
    {
        if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S))
        {
            TabShedule::Instance()->saveActiveFile();
        }
    }

    App::endWindow();
}

void TaskState::render()
{
}

std::string TaskState::getStateID() const
{
    return stateID;
}
