QT += widgets

TEMPLATE = subdirs

BUILDDIR = $$PWD/../../../SYSTEM/TEMP/UniHydro

DESTDIR= "$$PWD/../../../../../EXE"
OBJECTS_DIR = $${BUILDDIR}

DEFINES += LINUX

INCLUDEPATH += $$PWD/../../Include\
$$PWD/../../../../../System/Source/Include

QMAKE_CXXFLAGS_WARN_OFF += -Wwrite-strings\
-Wattributes

CONFIG += \
Chem\
ChemSpec\
Data\
EpsVol\
VolBase_b



SUBDIRS += \
ACS\
Chem\
ChemSpec\
Data\
EpsVol\
Extensions\
IntReg\
Pipe\
Temp\
Control\
NewComp\
UniHydro\
AVO\
UniHeat\
CtrlReg\
IO\
Valve_A_b\
Valve_b\
Valve_M_b\
Valve_R\
PressDrop\
CtrlConn\
UniModel\
Driver\
VBR\
Pump_b\
VolBase_b\
Vol_b\
HeatAntiFlow\
HeatChange_b\
CoolObj\
Motor\
HeatOil\
Drainage\
Klap3x\
Sens\
Level\
Reductor\
TBP\
PPK\
BaseCol\
BaseCol_NE\
BaseCol\
BaseCol_NE\
CC_1\
Column\
Column_NE\
Fan\
Thermostatic_Valve_3\
SepLiq_b\
LS\
Collector\
MultiSens\
FurnaceEx\
Electro\
Gag\
Trends\
Script\
PC_1


# AirDry\
# BaseCol\
# BaseCol_NE\
# build\
# CC_1\
# ChemSpec\
# Collector\
# Column\
# Column_NE\
# CoolObj\
# Driver\
# Ejector\
# ElectricHeater_b\
# Electro\
# Fakel\
# Fan\
# ForLinux\
# Furnace\
# FurnaceEx\
# Gag\
# HeatAntiFlow\
# HeatChange_b\
# HeatOil\
# Klap3x\
# KO\
# Level\
# LS\
# MultiSens\
# NewComp\
# PC_1\
# Pipe\
# PPK\
# Pump_b\
# Reductor\
# Script\
# Sens\
# SepLiq_b\
# Spider\
# TBP\
# Temp\
# Thermostatic_Valve_3\
# VolBase_b\
# Vol_b\
# Zadv3x
