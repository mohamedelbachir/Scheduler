#include"Utils.h"
#include"ScheduleItem.h"
bool sortProcessByArriveTimeSmallFirst(Process a,Process b){
    return a.t_arrive<b.t_arrive;
}

bool sortProcessByArriveTimeSmallFirstEqual(Process a,Process b){
    return a.t_arrive<b.t_arrive;
}

void displayProcessChunkResult(std::vector<ProcessChunck> r){
    for(int i=0;i<r.size();i++){
        std::cout<<r[i].startTime<<" ["<<r[i].refIDProcess<<"] "<<r[i].endTime<<"|";
    }
}

void displayListProcess(std::vector<Process> r){
    for(int i=0;i<r.size();i++){
        std::cout<<(int)r[i].t_arrive<<" ["<<r[i].Sid<<"] "<<(int)r[i].t_exec<<"|";
    }
    std::cout<<std::endl<<"----"<<std::endl;
}

int findSmallFirstProcess(int i,int k,std::vector<Process> l){
    int imin=i;

    for(int j=i;j<l.size();j++){
        if(l[imin].t_exec>l[j].t_exec&&k>=l[j].t_arrive&&l[imin].t_arrive>=l[j].t_arrive){
            imin=j;
        }
    }
    return imin;
}

int findSmallFirstProcessSRTF(int k,std::vector<Process> l){
    int imin=0;
    for(int j=1;j<l.size();j++){
        if(l[imin].t_exec>l[j].t_exec&&k>=l[j].t_arrive){
            imin=j;
        }
    }
    if(l[imin].t_arrive>k){
        imin=-1;
    }
    return imin;
}


int sumOfLenghtToExecute(std::vector<Process> l){
    int sum=0;
    for(size_t i=0;i<l.size();i++){
        sum+=l[i].t_exec;
    }
    return sum;
}

/**
    @brief firstComeFirstServer
    schedule algorithm for FCFS [No Preemptive]
    Principles :
        - Scheduling Algorithm simply schedules the jobs according to their arrival time
        - create a chunk of process for the sort list
        - COMPLEXITY : O(N*Log(N)+N) [!FEEL FREE TO IMPROVE IT]
    @param listProcess : process saved list
    @return Chunk of execution process
*/
std::vector<ProcessChunck> firstComeFirstServer(std::vector<Process> listProcess){
    std::vector<ProcessChunck> resultChunck;

    //sort to the time coming first
    std::stable_sort(listProcess.begin(),listProcess.end(),sortProcessByArriveTimeSmallFirst);

    //Start to create process chunk
    int k=listProcess.at(0).t_arrive;
    int u=0;
    for(size_t i=0;i<listProcess.size();i++){
        ProcessChunck t;
        if(i>0){
            if(k<=listProcess[i].t_arrive){
                k=listProcess[i].t_arrive;
            }else{
                k=u;
            }
        }
        t.startTime=k;
        if(t.startTime>listProcess[i].t_arrive){
            t.startWaitTime=listProcess[i].t_arrive;
            t.endWaitTime=t.startTime;
        }
        t.refIDProcess=listProcess[i].Id;
        t.endTime=k+listProcess[i].t_exec;
        u=k+listProcess[i].t_exec;
        k=u;
        resultChunck.push_back(t);
    }
    //displayProcessChunkResult(resultChunck);
    return resultChunck;
}

/**
    @brief shortJobFirst
    schedule algorithm for SJF [No Preemptive]
    Principles :
        - start to sort the list of process by the coming early first
        - Scheduling Algorithm which the processor executes the job first that has the smallest execution time
        - create a chunk of process for list
        - Complexity: O(N*Log(N)+N(N-1)/2) [!FEEL FREE TO IMPROVE IT]
    @param listProcess : process saved list
    @return Chunk of execution process
*/
std::vector<ProcessChunck> shortJobFirst(std::vector<Process> listProcess){
    std::vector<ProcessChunck> resultChunck;

    //sort to the time coming first
    std::stable_sort(listProcess.begin(),listProcess.end(),sortProcessByArriveTimeSmallFirst);

    //Start to create process chunk
    int imin=0,k=listProcess.at(0).t_arrive,u=0;
    for(size_t i=0;i<listProcess.size();i++){
        ProcessChunck t;
        imin=findSmallFirstProcess(i,k,listProcess);
        if(k<=listProcess.at(imin).t_arrive){
            k=listProcess.at(imin).t_arrive;
        }else{
            k=u;
        }
        t.startTime=k;
        if(t.startTime>listProcess.at(imin).t_arrive){
            t.startWaitTime=listProcess.at(imin).t_arrive;
            t.endWaitTime=t.startTime;
        }
        t.refIDProcess=listProcess.at(imin).Id;
        t.endTime=k+listProcess.at(imin).t_exec;
        u=k+listProcess.at(imin).t_exec;
        k=u;
        resultChunck.push_back(t);
        std::swap(listProcess[i],listProcess[imin]);
    }
    //displayProcessChunkResult(resultChunck);
    return resultChunck;
}

/**
    @brief shortestRemaningFirstTime :
    schedule algorithm for SRTF [Preemptive]
    Principles :
        - start to sort the list of process by the coming early first
        - Scheduling Algorithm which the process that requires the least amount of time to complete its execution is the one that is executed first
        - create a chunk of process for the list
        - Complexity: O(N*Log(N)+K(N^l-1)) #with k number Of Total Execution and l number of execution for single process [!FEEL FREE TO IMPROVE IT]
    @param listProcess : process saved list
    @return Chunk of execution process
*/
std::vector<ProcessChunck> shortestRemaningFirstTime(std::vector<Process> listProcess){
    std::vector<ProcessChunck> resultChunck;
    std::vector<Process>::iterator it;
    //sort to the time coming first
    std::stable_sort(listProcess.begin(),listProcess.end(),sortProcessByArriveTimeSmallFirst);

    int maxExecutionTime=sumOfLenghtToExecute(listProcess);

    int imin=0,k=listProcess.at(0).t_arrive,pass=0;
    //Start to create process chunk
    while(pass<maxExecutionTime){
        imin=findSmallFirstProcessSRTF(k,listProcess);
        if(imin!=-1){
            ProcessChunck t;
            t.startTime=k;
            if(t.startTime>listProcess.at(imin).t_arrive){
                t.startWaitTime=listProcess.at(imin).t_arrive;
                t.endWaitTime=t.startTime;
            }
            t.refIDProcess=listProcess.at(imin).Id;
            t.endTime=k+1;
            resultChunck.push_back(t);

            listProcess.at(imin).t_exec--;
            if(listProcess.at(imin).t_exec==0){
                it=listProcess.begin();
                listProcess.erase(it+imin);
            }else{
                listProcess.at(imin).t_arrive=t.endTime;
            }
            pass++;
        }
        k++;
    }
    //displayProcessChunkResult(resultChunck);
    return resultChunck;
}


/**
    @brief roundRobin :
    schedule algorithm for RR [Preemptive]
    Principles :
        - start to sort the list of process by the coming early first
        - Scheduling Algorithm where each process in the ready state gets the CPU for a fixed time in a cycle way
        - create a chunk of process for the list
    @param listProcess : process saved list
    @return Chunk of execution process
*/
std::vector<ProcessChunck> roundRobin(std::vector<Process> listProcess,int quantum){

    std::vector<ProcessChunck> resultChunck;

    //sort to the time coming first
    std::stable_sort(listProcess.begin(),listProcess.end(),sortProcessByArriveTimeSmallFirst);

    int maxExecutionTime=sumOfLenghtToExecute(listProcess);

    std::vector<Process>::iterator it;
    int k=listProcess.at(0).t_arrive;

    bool firstInit=true;
    while(!listProcess.empty()){

        //saving the first process inside a queue list
        ProcessChunck t;
        if(k<=listProcess.at(0).t_arrive){
            k=listProcess.at(0).t_arrive;
        }

        t.startTime=k;

        if(t.startTime>listProcess.at(0).t_arrive){
            t.startWaitTime=listProcess.at(0).t_arrive;
            t.endWaitTime=t.startTime;
        }
        t.refIDProcess=listProcess.at(0).Id;

        t.endTime=k;
        if(listProcess.at(0).t_exec>quantum)
            t.endTime+=quantum;
        else
            t.endTime+=listProcess.at(0).t_exec;

        resultChunck.push_back(t);

        //new set position of process
        listProcess.at(0).t_arrive=t.endTime;
        if(listProcess.at(0).t_exec>quantum){
            listProcess.at(0).t_exec-=quantum;
            listProcess.at(0).isTaskDone=true;
        }else{
            listProcess.at(0).t_exec=0;
        }
        k=listProcess.at(0).t_arrive;

        //if it finish his task delete it
        if(listProcess.at(0).t_exec==0){
            it=listProcess.begin();
            listProcess.erase(it);
        }else{
            std::rotate(listProcess.begin(),listProcess.begin()+1,listProcess.end());
        }
        std::stable_sort(listProcess.begin(),listProcess.end(),sortProcessByArriveTimeSmallFirst);
    }

    displayProcessChunkResult(resultChunck);
    return resultChunck;
}

void ScheduleTaskTab::StartScheduling(){
    m_current_state=DISPLAY_STATE;
    m_drawItems.isInit=false;
    m_resetView=true;

    //------- FUNCTION FOR SCHEDULING -------
    switch(m_stypeSchedule){
        case SCH_FCFS:
            m_chunck_process=firstComeFirstServer(m_processes);
        break;
        case SCH_SJF:
            m_chunck_process=shortJobFirst(m_processes);
        break;
        case SCH_SRTF:
            m_chunck_process=shortestRemaningFirstTime(m_processes);
        break;
        case SCH_RR:
            m_chunck_process=roundRobin(m_processes,(int)m_quantum);
        break;
    }
}
