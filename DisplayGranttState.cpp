#include"ScheduleItem.h"
#include"DearImgui/implot.h"
#include"Icons/IconsMaterialDesign.h"
#include"MainApp.h"
#include<iostream>
enum EXEC_TYPE_COLOR{
    WAIT_TIME_COLOR,
    EXEC_TIME_COLOR
};


void OpenPaletteColor(ImVec4 &bckColor,EXEC_TYPE_COLOR t){
    // Generate a default palette. The palette will persist and can be edited.
    static bool saved_palette_init = true;
    static ImVec4 saved_palette[32] = {};
    if (saved_palette_init)
    {
        for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
        {
            ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
                saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
            saved_palette[n].w = 1.0f; // Alpha
        }
        saved_palette_init = false;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,1);
    if (ImGui::BeginPopup("mypicker"))
    {
        ImGui::Text(App::LangData("idPalette"));
        ImGui::Separator();
        ImGui::ColorPicker4("##picker", (float*)&bckColor, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
        ImGui::SameLine();

        ImGui::BeginGroup(); // Lock X position
            ImGui::Text(App::LangData("idCurrentColor"));
            ImGui::ColorButton("##current", bckColor, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
            ImGui::Separator();

            ImGui::Text("Palette");
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                ImGui::PushID(n);
                    if ((n % 8) != 0)
                        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

                    ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
                    if (ImGui::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
                        bckColor = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, bckColor.w); // Preserve alpha!
                ImGui::PopID();
            }
            ImGui::EndGroup();
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void ScheduleTaskTab::displayState()
{
    if(!m_drawItems.isInit){
        m_drawItems.initDrawingData(m_processes);
        m_drawItems.initSysnopsys(m_chunck_process,m_processes.size());
        m_drawItems.isInit=true;
    }
    ImGui::BeginChild("left panel", ImVec2(250,0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,1);
        if(ImGui::ArrowButton("##left", ImGuiDir_Left))
        {
            m_current_state=MAIN_STATE;
            isSaved=false;
        }
        ImGui::SameLine();
        ImGui::Text(App::LangData("idBackEdit"));
        ImGui::Separator();
        if(m_stypeSchedule==SCH_RR)
        {
            ImGui::Spacing();
            ImGui::Text("Quantum:%d",(int)m_quantum);
            ImGui::Separator();
        }
        ImGui::Spacing();
        ImGui::Checkbox(App::LangData("idShowLegend"),&m_showLegend);
        ImGui::Checkbox(App::LangData("idShowSynopSis"),&m_showSynopsis);
        ImGui::Checkbox(App::LangData("idShowTop"),&m_showTOP);
        ImGui::Spacing();
        ImGui::Separator();
        if(ImGui::Button(App::LangData("idbtnResetView",ICON_MD_RESET_TV),ImVec2(ImGui::GetContentRegionAvail().x,30)))
        {
            m_resetView=true;
        }
        if(ImGui::Button(App::LangData("idbtnSave",ICON_MD_SAVE),ImVec2(ImGui::GetContentRegionAvail().x,30))){
            isSaved=saveFile();
        }
        ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginGroup();
        ImGui::BeginChild("parent view", ImVec2(ImGui::GetContentRegionAvail().x
                                                ,(m_showTOP||m_showSynopsis)?ImGui::GetContentRegionAvail().y-ImGui::GetContentRegionAvail().y/3:ImGui::GetContentRegionAvail().y));
            ImGui::BeginChild("content view", ImVec2(ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y));
                std::string algorithname[]={App::LangData("idFCFS"),App::LangData("idSJF"),App::LangData("idSRTF"),App::LangData("idRR")};
                ImGui::Text("%s : %s ",App::LangData("idLabelAlgoSchedule"),algorithname[m_stypeSchedule].c_str());
                ImGui::Separator();
                if(m_showLegend)
                {
                    ImGuiWindowFlags window_flags= ImGuiWindowFlags_NoDecoration
                                                   | ImGuiWindowFlags_AlwaysAutoResize
                                                   | ImGuiWindowFlags_NoSavedSettings
                                                   | ImGuiWindowFlags_NoScrollWithMouse
                                                   | ImGuiWindowFlags_NoNav
                                                   |ImGuiWindowFlags_NoMove;

                    ImVec2 center = ImVec2(ImGui::GetContentRegionMax().x,175);
                    if(App::m_appLanguage==EN_en){
                        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(-0.54f, 0.5f));
                    }else{
                        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(-0.36f, 0.5f));
                    }
                    ImGui::SetNextWindowBgAlpha(0.90f);// Transparent background
                    char label[255];
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,1);
                    sprintf(label,"legende%d",ID);
                    if(ImGui::Begin(label,&m_showLegend, window_flags))
                    {
                        ImGui::SetCursorPosX(ImGui::GetWindowWidth()/2-(ImGui::CalcTextSize(App::LangData("idLegend")).x)/2);
                        ImGui::Text(App::LangData("idLegend"));
                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();

                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(0,3));
                        ImGui::AlignTextToFramePadding();
                        static const ImGuiColorEditFlags colFlags=ImGuiColorEditFlags_InputRGB
                                                                 |ImGuiColorEditFlags_NoTooltip
                                                                 |ImGuiColorEditFlags_NoDragDrop;
                        static ImVec4 *color=nullptr;
                        static EXEC_TYPE_COLOR typeColor;
                        if(ImGui::ColorButton("#exec_color",theApp::Instance()->color_exec,colFlags,ImVec2(22,22))){
                            ImGui::OpenPopup("mypicker");
                            color=&theApp::Instance()->color_exec;
                            typeColor=EXEC_TIME_COLOR;
                        }
                        ImGui::SameLine();
                        ImGui::Text(App::LangData("idTextColorInExec"));

                        ImGui::AlignTextToFramePadding();
                        if(ImGui::ColorButton("#wait_color",theApp::Instance()->color_wait,colFlags,ImVec2(22,22))){
                            ImGui::OpenPopup("mypicker");
                            color=&theApp::Instance()->color_wait;
                            typeColor=WAIT_TIME_COLOR;
                        }
                        ImGui::SameLine();
                        ImGui::Text(App::LangData("idTextColorInWait"));
                        ImGui::PopStyleVar();
                        if(color!=nullptr)OpenPaletteColor(*color,typeColor);
                    }
                    ImGui::PopStyleVar();
                    ImGui::End();
                }
                //ImPlotFlags_
                char PlotTiltle[255];
                if(m_stypeSchedule!=SCH_RR)
                    sprintf(PlotTiltle,"%s | %s ##%d",App::LangData("idDisplayGranttTile"),algorithname[m_stypeSchedule].c_str(),m_tabID);
                else
                    sprintf(PlotTiltle,"%s | %s | Quantum : %d ##%d",App::LangData("idDisplayGranttTile"),algorithname[m_stypeSchedule].c_str(),(int)m_quantum,m_tabID);

                if(ImPlot::BeginPlot(PlotTiltle,ImVec2(ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y),ImPlotFlags_NoMenus))
                {

                    int n_ytick=m_drawItems.n_ytick;
                    //setup axis label
                    //setup axis limits
                    ImPlot::SetupAxesLimits(0,m_chunck_process.back().endTime+1,0,n_ytick+1);

                    //axis constraints
                    ImPlot::SetupAxisLimitsConstraints(ImAxis_X1,0,m_chunck_process.back().endTime+1);

                    ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1,0,n_ytick+1);

                    //custum label
                    ImPlot::SetupAxisTicks(ImAxis_Y1, m_drawItems.yvlabel,n_ytick, m_drawItems.ylabel, false);
                    if(m_resetView)
                    {
                        ImPlot::SetupAxisLimits(ImAxis_X1,0,m_chunck_process.back().endTime+1,ImPlotCond_Always);
                        ImPlot::SetupAxisLimits(ImAxis_Y1,0,n_ytick+1,ImPlotCond_Always);
                        m_resetView=false;
                    }
                    ImPlot::SetupAxes(App::LangData("idLabelX"),std::string(App::LangData("idLabelY")).c_str());

                    //plotting processes
                    ImGui::PushID(m_tabID);
                    ImPlot::PushPlotClipRect();
                    for(size_t i=0;i<m_chunck_process.size();i++){
                        if(m_chunck_process[i].endWaitTime-m_chunck_process[i].startWaitTime>0){
                            ImVec2 rminw= ImPlot::PlotToPixels(ImPlotPoint(m_chunck_process[i].startWaitTime,m_drawItems.coord[m_chunck_process[i].refIDProcess]));
                            ImVec2 rmaxw= ImPlot::PlotToPixels(ImPlotPoint(m_chunck_process[i].endWaitTime,m_drawItems.coord[m_chunck_process[i].refIDProcess]-0.2f));
                            ImPlot::GetPlotDrawList()->AddRectFilled(rminw, rmaxw,Imv4ToCol32(theApp::Instance()->color_wait));
                        }
                        ImPlot::GetPlotDrawList()->AddRectFilled(ImPlot::PlotToPixels(ImPlotPoint(m_chunck_process[i].startTime,m_drawItems.coord[m_chunck_process[i].refIDProcess])),
                                                                 ImPlot::PlotToPixels(ImPlotPoint(m_chunck_process[i].endTime,m_drawItems.coord[m_chunck_process[i].refIDProcess]-0.2f))
                                                                 ,Imv4ToCol32(theApp::Instance()->color_exec));
                    }
                    //std::cout<<std::endl;
                    ImPlot::PopPlotClipRect();
                    ImGui::PopID();
                    ImPlot::EndPlot();
                }
            ImGui::EndChild();
        ImGui::EndChild();
        if(m_showTOP)
        {
            ImGui::BeginChild("##TOP",ImVec2(m_showSynopsis?ImGui::GetContentRegionAvail().x/2:ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y),true);
                ImGui::Text(App::LangData("idTop"));
                showTableOfProcess(false);
            ImGui::EndChild();
            ImGui::SameLine();
        }
        if(m_showSynopsis)
        {
            ImGui::BeginChild("##Synopsis",ImVec2(ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y),true);
                char data[255];

                ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x/2-(ImGui::CalcTextSize(App::LangData("idSynopsisTile")).x)/2);
                ImGui::Text(App::LangData("idSynopsisTile"));
                ImGui::Separator();

                ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x/2-(ImGui::CalcTextSize(App::LangData("idSynopsisTOTAL_EXEC")).x)/2);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY()+20);
                ImGui::Text("%s : ",App::LangData("idSynopsisTOTAL_EXEC"));
                ImGui::Spacing();
                    sprintf(data,"%d UT",m_chunck_process.back().endTime);
                    ImGui::PushFont(App::cft);
                        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x/2-(ImGui::CalcTextSize(data).x/2));
                        ImGui::TextColored(App::m_appTheme==DARK_THEME?ImVec4(1,1,0,1):ImVec4(1,0,0,1),"%s",data);
                    ImGui::PopFont();
                ImGui::Spacing();

                ImGui::Separator();
                ImGui::Spacing();
                ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x/2-(ImGui::CalcTextSize(App::LangData("idSynopsisAVG_Wait")).x)/2);
                ImGui::Text("%s : ",App::LangData("idSynopsisAVG_Wait"));
                ImGui::Spacing();
                    sprintf(data,"%0.2f UT",m_drawItems.avgWaitTime);
                    ImGui::PushFont(App::cft);
                        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x/2-(ImGui::CalcTextSize(data).x/2));
                        ImGui::TextColored(App::color_wait,"%s",data);
                    ImGui::PopFont();
                ImGui::Spacing();

                ImGui::Separator();
                ImGui::Spacing();
                ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x/2-(ImGui::CalcTextSize(App::LangData("idSynopsisAVG_EXEC")).x)/2);
                ImGui::Text("%s : ",App::LangData("idSynopsisAVG_EXEC"));
                ImGui::Spacing();
                    sprintf(data,"%0.2f UT",m_drawItems.avgExecTime);
                    ImGui::PushFont(App::cft);
                        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x/2-(ImGui::CalcTextSize(data).x/2));
                        ImGui::TextColored(App::color_exec,"%s",data);
                    ImGui::PopFont();
                ImGui::Spacing();

                //ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y-ImGui::CalcTextSize(App::LangData("idSynopsisUNIT")).y-5);
                ImGui::Text("* UT : ( %s )",App::LangData("idSynopsisUNIT"));

            ImGui::EndChild();
        }
    ImGui::EndGroup();
    //static bool isOpen=true;
    //ImPlot::ShowDemoWindow(&isO);
}
