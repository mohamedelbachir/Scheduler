#include "../MainApp.h"
#include "MainAppState.h"
#include "TaskState.h"
#include <iostream>
std::string MainState::stateID = "MAINSTATE";
bool MainState::onEnter()
{
    return true;
}
bool MainState::onExit()
{
    return true;
}

void MainState::update()
{
    App::startWindow();
    {
        // Left
        {
            ImGui::BeginChild("left pane", ImVec2(200, 0), false, ImGuiWindowFlags_NoSavedSettings);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f));
            if (ImGui::Button(App::LangData("idBtnNew", ICON_MD_FILE_OPEN), ImVec2(200, 30)))
            {
                TabShedule::Instance()->addTab();
                theApp::Instance()->states->changeState(new TaskState(referenceWindow));
            }
            ImGui::Spacing();
            if (ImGui::Button(App::LangData("idBtnOpen", ICON_MD_FOLDER_OPEN), ImVec2(200, 30)))
            {
                openFile();
            }
            ImGui::Spacing();
            ImGui::PopStyleVar(2);

            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 150);
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text(App::LangData("idTutorial"));
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                if (App::m_appLanguage == FR_fr)
                {
                    SDL_OpenURL(std::string(App::m_path[MAINAPP_PATH] + "manual/help_fr.html").c_str());
                }
                else
                {
                    SDL_OpenURL(std::string(App::m_path[MAINAPP_PATH] + "manual/help_en.html").c_str());
                }
            }

            ImGui::Spacing();
            ImGui::Text(App::LangData("idContrib"));
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                SDL_OpenURL("https://github.com/mohamedelbachir/scheduler/pulls");
            }

            ImGui::Spacing();
            ImGui::Text(App::LangData("idreport"));
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                SDL_OpenURL("https://github.com/mohamedelbachir/scheduler/issues");
            }

            ImGui::Spacing();
            ImGui::Text(App::LangData("idAbout"));
            if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
// theApp::Instance()->OpenAboutWindow();
#if !EMSCRIPTEN
                theApp::Instance()->OpenAboutWindow();
#else
                App::isAboutPopupWindowOpen = true;
#endif
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();
        // Right
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("parent view", ImVec2(0, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_NoSavedSettings);
            ImVec2 dropFileZone(ImGui::GetContentRegionAvail().x - 100, ImGui::GetContentRegionAvail().y - 100);
            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2 - dropFileZone.x / 2);
            ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 2 - dropFileZone.y / 2);
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 15);
            ImGui::BeginChild("item view", dropFileZone, true, ImGuiWindowFlags_NoSavedSettings);
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() / 2 - ImGui::CalcTextSize(ICON_MD_FILE_OPEN).x / 2 - 25);
            ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y / 2 - ImGui::CalcTextSize(ICON_MD_FILE_OPEN).y / 2 - 20);

            // ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
            // if (ImGui::DragFloat("window scale", &window_scale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp)) // Scale only this window
            ImGui::SetWindowFontScale(5.0f);
            ImGui::Text(ICON_MD_FILE_OPEN);
            ImGui::SetWindowFontScale(1.5f);
            std::string fileDrop = App::LangData("idTextDrag");
            ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() / 2 - (ImGui::CalcTextSize(fileDrop.c_str()).x) / 2 + 10);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
            ImGui::Text(fileDrop.c_str());

            // ImGui::PopItemWidth();
            ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::EndChild();
            ImGui::EndGroup();
        }
    }
    App::endWindow();
    // ImPlot::ShowDemoWindow(&sg);
}

void MainState::render()
{
}

std::string MainState::getStateID() const
{
    return stateID;
}
