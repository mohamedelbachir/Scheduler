#include "tinyfiledialogs.h"
#include"ScheduleItem.h"
#include "MainApp.h"


std::string getTabNames(std::string str)
{
    str.erase(str.length()-4);
    int i=str.length()-1;
    std::string name="";
    while(str[i]!='/'&&str[i]!='\\')
    {
        name+=str[i];
        i--;
    }
    std::reverse(name.begin(),name.end());
    return name;

}

void ScheduleTaskTab::parsingScheduleElement(TiXmlElement *elmt)
{

    for(TiXmlElement *e=elmt->FirstChildElement(); e!=NULL; e=e->NextSiblingElement())
    {
        if(!e->NoChildren())
        {
            parsingScheduleElement(e);
        }
        if(strcmp(e->Value(),"SCH_GRP")==0){
            e->Attribute("value",(int*)&m_groupSchedule);
        }

        if(strcmp(e->Value(),"SCH_ALG")==0){
            e->Attribute("value",(int*)&m_typeSchedule);

        }

        if(strcmp(e->Value(),"SCH_GEN")==0){
            e->Attribute("value",(int*)&m_algoGenType);
        }

        if(strcmp(e->Value(),"Alphabet")==0){
            e->Attribute("value",&alphabet);
        }

        if(strcmp(e->Value(),"ProcessNumber")==0){
            e->Attribute("value",(int*)&m_numberOfProcess);
        }

        if(strcmp(e->Value(),"Quantum")==0){
            e->Attribute("value",(int*)&m_quantum);
        }

        if(strcmp(e->Value(),"table")==0){
            setTabName(e->Attribute("name"));
        }

        if(strcmp(e->Value(),"Process")==0){
            Process p;
            e->Attribute("upid",&p.Id);
            strcpy(p.Sid,e->Attribute("pid"));
            e->Attribute("t_arrive",(int*)&p.t_arrive);
            e->Attribute("t_exec",(int*)&p.t_exec);
            //std::cout<<e->Value()<<" : "<<"upid : "<<p.Id<<"pid : "<<p.Sid<<"t_arr : "<<p.t_arrive<<"t_Exec : "<<p.t_exec<<std::endl;
            m_processes.push_back(p);
            n_process_generated=p.Id+1;
        }
    }
}

void ScheduleTaskTab::parseFromXML(char *pathXMLFile)
{

    TiXmlDocument doc;

    if(pathXMLFile==NULL||!doc.LoadFile(pathXMLFile)){
        std::cout<<"Impossible to Load XML File .... : "<<doc.ErrorDesc()<<std::endl;
        return;
    }
    TiXmlElement *root=doc.RootElement();
    std::cout<<std::endl<<"+ start parsing"<<std::endl;
    parsingScheduleElement(root);
    std::cout<<std::endl<<"+ [done]"<<std::endl;
    m_generated=true;
    pathSavedFile=new char[strlen(pathXMLFile)];
    strcpy(pathSavedFile,pathXMLFile);
    //StartScheduling();
}

bool ScheduleTaskTab::saveFile()
{
    if(pathSavedFile==NULL){
        char const * lFilterPatterns[1]= {"*.shd"};
        pathSavedFile=tinyfd_saveFileDialog(App::LangData("idSave"),NULL,1,lFilterPatterns,"Schedule File");
    }
    if(pathSavedFile!=NULL)
    {

        TiXmlDocument doc;
        TiXmlDeclaration *decl=new TiXmlDeclaration("1.0","UTF-8","");
        doc.LinkEndChild(decl);

        TiXmlElement *scheduleFile;
        scheduleFile=new TiXmlElement("ScheduleFile");
        doc.LinkEndChild(scheduleFile);

        TiXmlElement *parameter=new TiXmlElement("Paramaters");
        scheduleFile->LinkEndChild(parameter);

        TiXmlElement *p_element=new TiXmlElement("SCH_GRP");
        p_element->SetAttribute("value",(int)m_groupSchedule);
        parameter->LinkEndChild(p_element);


        p_element=new TiXmlElement("SCH_ALG");
        p_element->SetAttribute("value",(int)m_typeSchedule);
        parameter->LinkEndChild(p_element);


        p_element=new TiXmlElement("SCH_GEN");
        p_element->SetAttribute("value",(int)m_algoGenType);
        parameter->LinkEndChild(p_element);

        p_element=new TiXmlElement("Alphabet");
        p_element->SetAttribute("value",alphabet);
        parameter->LinkEndChild(p_element);

        p_element=new TiXmlElement("ProcessNumber");
        p_element->SetAttribute("value",(int)m_numberOfProcess);
        parameter->LinkEndChild(p_element);

        p_element=new TiXmlElement("Quantum");
        p_element->SetAttribute("value",(int)m_quantum);
        parameter->LinkEndChild(p_element);


        TiXmlElement *table=new TiXmlElement("table");
        table->SetAttribute("name",getTabNames(pathSavedFile).c_str());
        scheduleFile->LinkEndChild(table);

        for(size_t i=0; i<m_processes.size(); i++)
        {
            TiXmlElement *data=new TiXmlElement("Process");
            data->SetAttribute("upid",m_processes[i].Id);
            data->SetAttribute("pid",m_processes[i].Sid);
            data->SetAttribute("t_arrive",m_processes[i].t_arrive);
            data->SetAttribute("t_exec",m_processes[i].t_exec);
            table->LinkEndChild(data);
        }
        if(!doc.SaveFile(pathSavedFile))
        {
            tinyfd_messageBox(App::LangData("idError"),doc.ErrorDesc(),"okcancel","error",0);
            std::cout<<"Error While saving .."<<doc.ErrorDesc()<<std::endl;
            return false;
        }
        setTabName(table->Attribute("name"));

        //check if it is not record then save the path
        std::vector<std::string>::iterator it=std::find(App::m_recentItems.begin(),App::m_recentItems.end(),pathSavedFile);
        if(it==App::m_recentItems.end()){
            std::ofstream recentFiles(App::m_path[RECENT_PATH].c_str(),std::ios::app);
            recentFiles<<pathSavedFile<<std::endl;
            recentFiles.close();
        }
    }else{
        return false;
    }
    return true;
}
