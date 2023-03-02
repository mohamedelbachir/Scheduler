#include "TaskState.h"
#include"MainAppState.h"
TabBarSchedule *TabBarSchedule::instance=NULL;
int TabBarSchedule::n_instace=0;
std::string TaskState::stateID="TaskState";
bool TaskState::onEnter()
{
    return true;
}
bool TaskState::onExit()
{
    return true;
}

void TaskState::update()
{
    static bool isOpen=true;
    ImGui::SetNextWindowPos(ImVec2(0,ImGui::GetFrameHeight()),ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x,ImGui::GetIO().DisplaySize.y-ImGui::GetFrameHeight()));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,0.0f);
    ImGuiWindowFlags flags=ImGuiWindowFlags_NoCollapse
                          |ImGuiWindowFlags_NoTitleBar
                          |ImGuiWindowFlags_NoSavedSettings
                          |ImGuiWindowFlags_NoMove
                          |ImGuiWindowFlags_NoDecoration
                          |ImGuiWindowFlags_NoResize
                          |ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("TaskState",&isOpen,flags);
        TabShedule::Instance()->render();
        if(TabShedule::Instance()->tabNumber()==0){
            theApp::Instance()->states->changeState(new MainState(referenceWindow));
            TabShedule::Instance()->n_instace=0;
            ScheduleTaskTab::ID=0;
        }else{
            if(ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S)){
                TabShedule::Instance()->saveActiveFile();
            }
        }

    ImGui::End();
    ImGui::PopStyleVar();
}

void TaskState::render()
{

}

std::string TaskState::getStateID()const
{
    return stateID;
}
