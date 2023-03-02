#include"ScheduleItem.h"
#include"Icons/IconsMaterialDesign.h"
#include "MainApp.h"
int ScheduleTaskTab::ID=0;

ScheduleTaskTab::ScheduleTaskTab(char *pathXMLFile):ScheduleTaskTab(){
    //parse from xml file
    parseFromXML(pathXMLFile);
}

void ScheduleTaskTab::update(){
    if(m_generated)m_numberOfProcess=m_processes.size();
    if(m_numberOfProcess==0){
        m_generated=false;
        n_process_generated=0;
        isPidChecked=false;
    }

    /*if(isCanSaved()){
        if(ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S)){
            isSaved=saveFile();
        }
    }*/

    if(isSaved)
        m_nameTab[0]=' ';
    else
        m_nameTab[0]='*';

    if(m_groupSchedule==PREEMPTIF){
        m_stypeSchedule=m_typeSchedule+SCH_SRTF;
    }else{
        m_stypeSchedule=m_typeSchedule;
    }
}

void ScheduleTaskTab::mainState(){
    std::string algorithname[]={App::LangData("idFCFS"),App::LangData("idSJF"),App::LangData("idSRTF"),App::LangData("idRR")};
    //left-side
    {
        ImGui::BeginChild("left pane", ImVec2(250, 0));
            ImGui::Spacing();
            ImGui::Text(App::LangData("idchooseTypeAlg"));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,1);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(4,4));
                    ImGui::RadioButton(App::LangData("idNoPremptive"),(int*)&m_groupSchedule,NOPREEMPTIF);ImGui::SameLine();
                    ImGui::RadioButton(App::LangData("idPremptive"),(int *)&m_groupSchedule,PREEMPTIF);
                ImGui::PopStyleVar();
                ImGui::Spacing();
                ImGui::Text(App::LangData("idLabelAlgoSchedule"));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(4,6));
                    //char algoNP[255];
                    std::string algoNP=App::LangData("idFCFS");
                    algoNP+='\0';
                    algoNP+=App::LangData("idSJF");
                    algoNP+='\0';

                    std::string algoP=App::LangData("idSRTF");
                    algoP+='\0';
                    algoP+=App::LangData("idRR");
                    algoP+='\0';

                    switch(m_groupSchedule){
                        case NOPREEMPTIF:
                            ImGui::Combo("##type algo",(int*)&m_typeSchedule,algoNP.c_str());
                            break;
                        case PREEMPTIF:
                            ImGui::Combo("##type algo",(int*)&m_typeSchedule,algoP.c_str());
                        break;
                    }
                    if (ImGui::IsItemHovered()){
                        ImGui::SetNextWindowSize(ImVec2(500,100));
                        ImGui::SetNextWindowBgAlpha(0.9);
                        ImGui::BeginTooltip();
                        std::string titleToolTip="",descToolTips="";
                        switch(m_stypeSchedule){
                            case SCH_FCFS:
                                titleToolTip=App::LangData("idFCFSMeaning");
                                descToolTips=App::LangData("idFCFSDesc");
                            break;
                            case SCH_SJF:
                                titleToolTip=App::LangData("idSJFMeaning");
                                descToolTips=App::LangData("idSJFDesc");
                            break;
                            case SCH_SRTF:
                                titleToolTip=App::LangData("idSRTFMeaning");
                                descToolTips=App::LangData("idSRTFDesc");
                            break;
                            case SCH_RR:
                                titleToolTip=App::LangData("idRRMeaning");
                                descToolTips=App::LangData("idRRDesc");
                            break;
                        }
                        if(m_stypeSchedule!=SCH_RR)
                            ImGui::Text("%s (%s)",algorithname[m_stypeSchedule].c_str(),titleToolTip.c_str());
                        else
                            ImGui::Text("%s",algorithname[m_stypeSchedule].c_str());
                        ImGui::Separator();
                        ImGui::TextWrapped(descToolTips.c_str());
                        ImGui::EndTooltip();
                    }

                    if(m_groupSchedule==PREEMPTIF){
                        if(m_stypeSchedule==SCH_RR){
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("Quantum : ");
                            ImGui::SameLine();
                            ImGui::BeginGroup();
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                                    ImGui::InputScalar("##Quantum : ",ImGuiDataType_U8,&m_quantum,&u8_one, NULL, "%u");
                                    if(m_quantum<1){
                                        m_quantum=1;
                                    }
                                ImGui::PopItemWidth();
                            ImGui::EndGroup();
                            ImGui::Spacing();
                        }
                    }

                    ImGui::BeginDisabled(m_generated);
                    ImGui::Text(App::LangData("idLabelAlgoGenMode"));
                    std::string algoGen[]={
                        App::LangData("idDefault")
                        ,App::LangData("idAlphabet")
                        ,App::LangData("idAlphaNum")
                        ,App::LangData("idNum")
                    };
                    ImGui::Combo("##gen mode",(int*)&m_algoGenType,(char **)algoGen,IM_ARRAYSIZE(algoGen));
                    if(m_algoGenType==ALG_ALPHA_NUM){
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text(App::LangData("idLabelLetter"));
                        ImGui::SameLine();
                        ImGui::BeginGroup();
                            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                                ImGui::Combo("##alphabet",&alphabet,lettre,IM_ARRAYSIZE(lettre));
                            ImGui::PopItemWidth();
                        ImGui::EndGroup();
                    }
                    ImGui::EndDisabled();
                    ImGui::Spacing();

                    ImGui::BeginDisabled(m_generated);
                        ImGui::Text(App::LangData("idLabelNumberProcess"));
                        ImGui::InputScalar("##inputProcessValue",     ImGuiDataType_U16,    &m_numberOfProcess,&u16_one, NULL, "%u");
                    ImGui::EndDisabled();
                    ImGui::Spacing();
                    ImGui::BeginDisabled((int)m_numberOfProcess==0||m_generated||(m_typeSchedule+SCH_SJF+1==SCH_RR&&m_quantum==0));
                        if(ImGui::Button(App::LangData("idbtnGenerate"),ImVec2(ImGui::GetContentRegionAvail().x,30))){
                            m_generated=true;
                            AddProcess(m_numberOfProcess);
                        }
                    ImGui::EndDisabled();

                    ImGui::Spacing();
                    ImGui::PushStyleColor(ImGuiCol_Button,ImColor(255,0,0,255).Value);
                    ImGui::PushStyleColor(ImGuiCol_Text,ImColor(255,255,255,255).Value);
                    ImGui::BeginDisabled(!m_generated);
                        if(ImGui::Button(App::LangData("idbtnRenitialize"),ImVec2(ImGui::GetContentRegionAvail().x,30))){
                            deleteAllprocess();
                            m_generated=false;
                        }
                    ImGui::EndDisabled();
                    ImGui::PopStyleColor(2);
                ImGui::PopStyleVar();
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();
        ImGui::EndChild();
    }
    ImGui::SameLine();
    // Right
    {
        ImGui::BeginGroup();
            ImGui::Spacing();
            ImGui::Text("%s : %s ",App::LangData("idLabelAlgoSchedule"),algorithname[m_stypeSchedule].c_str());
            ImGui::Separator();
            ImGui::BeginChild("parent view", ImVec2(0,ImGui::GetContentRegionAvail().y));
                ImVec2 tableZone(ImGui::GetContentRegionAvail().x-200,ImGui::GetContentRegionAvail().y-50);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::BeginChild("##table",ImVec2(tableZone),!m_generated);
                    if(m_generated){
                        showTableOfProcess(true);
                    }else{
                        std::string EmptyProcessesTest="[ "+std::string(App::LangData("idEmptyTop"))+" ]";
                        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x/2-ImGui::CalcTextSize(EmptyProcessesTest.c_str()).x/2);
                        ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y/2-ImGui::CalcTextSize(EmptyProcessesTest.c_str()).y/2);
                        ImGui::Text(EmptyProcessesTest.c_str());
                    }
                ImGui::EndChild();
                ImGui::SameLine();
                ImGui::BeginGroup();
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,2);
                    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign,ImVec2(0,0.5f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(3,4));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,1);
                    ImGui::BeginDisabled(!m_generated);

                    if(ImGui::Button(App::LangData("idbtnAdd",ICON_MD_ADD),ImVec2(ImGui::GetContentRegionAvail().x,30))){
                        AddProcess();
                    }
                    ImGui::BeginDisabled(m_process_checked.size()==0);
                        ImGui::PushStyleColor(ImGuiCol_Button,ImColor(255,0,0,255).Value);
                        ImGui::PushStyleColor(ImGuiCol_Text,ImColor(255,255,255,255).Value);
                        if(ImGui::Button(App::LangData("idbtnDel",ICON_MD_DELETE),ImVec2(ImGui::GetContentRegionAvail().x,30))){
                            deleteSelectedProcess();
                        }
                        ImGui::PopStyleColor(2);
                    ImGui::EndDisabled();
                    if(ImGui::Button(App::LangData("idbtnDisplayGRANTT",ICON_MD_SETTINGS_DISPLAY),ImVec2(ImGui::GetContentRegionAvail().x,30))){
                        if(VerifiedUserData()==USERINPUTERROR){
                            m_showPopus=true;
                        }else{
                            StartScheduling();
                        }
                    }
                    ImGui::EndDisabled();
                    ImGui::PopStyleVar(4);
                ImGui::EndGroup();
            ImGui::EndChild();
        ImGui::EndGroup();
    }
    if(m_showPopus)
    {
        m_showPopus=false;
        std::string btnYes=App::LangData("idbtnOk");
        std::string btnCancel=App::LangData("idbtnCancel");

        const SDL_MessageBoxButtonData buttons[] = {
            // .flags, .buttonid, .text
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, btnYes.c_str() },
            { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, btnCancel.c_str() },
        };
        std::string msg=App::LangData("idUserWrongInputContent");
        const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            theApp::Instance()->getWindow(), /* .window */
            App::LangData("idWarning"), /* .title */
            msg.c_str(), /* .message */
            SDL_arraysize(buttons), /* .numbuttons */
            buttons, /* .buttons */
            NULL
        };
        int buttonid;
        SDL_ShowMessageBox(&messageboxdata, &buttonid);

        /*SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,App::LangData("idWarning")
                                 ,(char*)App::LangData("idUserWrongInputContent"),theApp::Instance()->getWindow());*/
    }
}
void ScheduleTaskTab::showTableOfProcess(bool isEditable){
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    const float minRowHeight=25;

    static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY
                                |ImGuiTableFlags_RowBg
                                |ImGuiTableFlags_BordersOuter
                                |ImGuiTableFlags_BordersInnerH
                                |ImGuiTableFlags_BordersV
                                |ImGuiTableFlags_Resizable;
    if(isEditable)
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding,ImVec2(2,2));
    else
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding,ImVec2(5,6));
    if(ImGui::BeginTable("table_scrolly", 3, flags))
    {

        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn(App::LangData("idTopPID"), ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn(App::LangData("idTopArrive"), ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn(App::LangData("idTopExec"), ImGuiTableColumnFlags_None);

        if(!isEditable){
            ImGui::TableHeadersRow();
        }else{
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers,minRowHeight);
            ImGui::TableSetColumnIndex(0);

            const char* column_name = ImGui::TableGetColumnName(0); // Retrieve name passed to TableSetupColumn()
            ImGui::PushID(0);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX()+5);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY()+minRowHeight/2-TEXT_BASE_HEIGHT/2+5);
            if(!isPidChecked){
                if(m_process_checked.size()==m_processes.size()){
                    isPidChecked=true;
                }
            }
            ImGui::Checkbox("##checkall", &isPidChecked);
            if(!isPidChecked){
                if(m_process_checked.size()==m_processes.size()){
                    for(size_t i=0;i<m_process_checked.size();i++){
                        m_processes[m_process_checked[i]].selected=false;
                    }
                }
            }
            ImGui::PopStyleVar(2);
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
            ImGui::TableHeader(column_name);
            ImGui::PopID();

            ImGui::TableSetColumnIndex(1);
            column_name = ImGui::TableGetColumnName(1); // Retrieve name passed to TableSetupColumn()
            ImGui::SetCursorPosX(ImGui::GetCursorPosX()+5);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY()+minRowHeight/2-TEXT_BASE_HEIGHT/2+5);
            ImGui::TableHeader(column_name);

            ImGui::TableSetColumnIndex(2);
            column_name = ImGui::TableGetColumnName(2); // Retrieve name passed to TableSetupColumn()
            ImGui::SetCursorPosX(ImGui::GetCursorPosX()+5);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY()+minRowHeight/2-TEXT_BASE_HEIGHT/2+5);
            ImGui::TableHeader(column_name);
        }

        for (size_t row = 0; row < m_processes.size(); row++)
        {
            ImGui::TableNextRow();
            char id[255]="";

            //PID Columns
            ImGui::TableNextColumn();
            ImGui::TableSetColumnIndex(0);
            if(isEditable){
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,1);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX()+5);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY()+minRowHeight/2-TEXT_BASE_HEIGHT/2+2);
                sprintf(id,"##checkPID %d",ID+row);
                if(isPidChecked&&!m_processes[row].selected){
                    m_processes[row].selected=true;
                }
                ImGui::Checkbox(id, &m_processes[row].selected);
                if(!m_processes[row].selected){
                    isPidChecked=false;
                }
                std::vector<int>::iterator it;
                it=std::find(m_process_checked.begin(),m_process_checked.end(),row);
                if(m_processes[row].selected){
                    if(it==m_process_checked.end())m_process_checked.push_back(row);
                }else{
                    if(it!=m_process_checked.end()){
                        m_process_checked.erase(it);
                    }
                }
                ImGui::PopStyleVar(2);
                ImGui::SameLine();
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(0,5));
                ImGui::PushStyleColor(ImGuiCol_FrameBg,0);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX()-5);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY()-3);
                sprintf(id,"##pid %d",ID+row);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputText(id,m_processes[row].Sid,IM_ARRAYSIZE(m_processes[row].Sid),m_algoGenType!=ALG_DEFAULT?ImGuiInputTextFlags_ReadOnly:ImGuiInputTextFlags_None|ImGuiInputTextFlags_CharsNoBlank);
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
            }else{
                ImGui::Text("%s",m_processes[row].Sid);
            }

            //Time to Arrive
            ImGui::TableNextColumn();
            ImGui::TableSetColumnIndex(1);
            if(isEditable){

                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY()+1);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,1);

                sprintf(id,"##inputProcessArriveValue %d",ID+row);
                ImGui::InputScalar(id,ImGuiDataType_U8,&m_processes[row].t_arrive,&u8_one, NULL, "%u");
                ImGui::PopStyleVar(3);
                ImGui::PopItemWidth();

            }else{
                ImGui::Text("%d",m_processes[row].t_arrive);
            }

            //Time to execute
            ImGui::TableNextColumn();
            ImGui::TableSetColumnIndex(2);
            if(isEditable){
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY()+1);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,1);

                sprintf(id,"##inputProcessExecValue %d",ID+row);
                ImGui::InputScalar(id,ImGuiDataType_U8,&m_processes[row].t_exec,&u8_one, NULL, "%u");
                if(m_processes[row].t_exec<1){
                    m_processes[row].t_exec=1;
                }
                ImGui::PopStyleVar(3);
                ImGui::PopItemWidth();
            }else{
                ImGui::Text("%d",m_processes[row].t_exec);
            }
        }
        //ImGui::PopStyleVar();
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}
