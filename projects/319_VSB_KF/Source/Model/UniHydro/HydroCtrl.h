#pragma once


struct CGroupCtrlInfo
  {
  int NumbGroup;
  int MaxEQ;
  int MaxVAR;
//
  bool CreateSubgroup;
  bool CompareSubgroup;
  bool WorkSubgroup;
  bool ClearCounters;
  int MaxSubgroupVars;
//
  int kSubgroup;
  int kExtVar;
  double Accel;
  double Time_sec;
  };

#define MAX_GROUP 8
struct CHydroCtrl
  {
  CGroupCtrlInfo Group[MAX_GROUP];
  int kGroups;
  };