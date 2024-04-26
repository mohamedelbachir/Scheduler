#pragma once
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include "imgui/imgui.h"
#if !EMSCRIPTEN
#include <bits/stdc++.h>
#else
#include <map>
#include <vector>
#include <iostream>
#endif

#define Imv4ToCol32(c) IM_COL32(c.x * 255, c.y * 255, c.z * 255, c.w * 255)

enum ScheduleGroup
{
    PREEMPTIF,
    NOPREEMPTIF
};

enum ScheduleType
{ /*No Preemptif*/ SCH_FCFS,
  SCH_SJF,
  /*Preemptif*/ SCH_SRTF,
  SCH_RR };

enum AlgoGenerationType
{
    ALG_DEFAULT,
    ALG_ALPHA,
    ALG_ALPHA_NUM,
    ALG_NUM
};

struct Process
{

    // boolean to Select processes
    bool selected = false;

    /// Integer identifier of process
    int Id;

    /// String Identifier of processes
    char Sid[255];

    /// Time to arrive
    ImU8 t_arrive = 1;

    /// Time of execution
    ImU8 t_exec = 1;

    /// Priority value
    ImU8 priority = 1;

    /// Boolean to determine if her task is done !!useful for [Preemptive Algorithm]
    bool isTaskDone = false;
};

struct ProcessChunck
{
    int startTime = 0;
    int refIDProcess;
    int endTime = 0;
    int startWaitTime = 0;
    int endWaitTime = 0;
};

struct DrawItem
{
    double *yvlabel;
    char **ylabel;
    int n_ytick = 0;
    double avgExecTime = 0;
    double avgWaitTime = 0;

    std::map<int, int> coord;
    bool isInit = false;
    void initDrawingData(std::vector<Process> p)
    {
        yvlabel = new double[p.size()];
        ylabel = new char *[p.size()];
        n_ytick = p.size();
        for (size_t i = 0; i < p.size(); i++)
        {
            yvlabel[i] = i + 1;
            ylabel[i] = new char[strlen(p[i].Sid) + 1];
            strcpy(ylabel[i], p[i].Sid);
            coord[p[i].Id] = yvlabel[i];
        }
    }

    void clean()
    {
        delete yvlabel;
        coord.clear();
        delete[] ylabel;
    }

    void initSysnopsys(std::vector<ProcessChunck> c, int n)
    {
        // calc avgExecTime,avgExecTime
        avgWaitTime = 0;
        avgExecTime = 0;
        for (size_t i = 0; i < c.size(); i++)
        {
            avgWaitTime += c[i].endWaitTime - c[i].startWaitTime;
            avgExecTime += c[i].endTime - c[i].startTime;
        }
        avgExecTime /= n;
        avgWaitTime /= n;
    }
};

#endif // UTILS_H_INCLUDED
