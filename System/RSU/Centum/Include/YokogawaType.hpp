// Имя модели и идентификатор
YOKO_TYPE( 0200 , _GS     , "%GS"      )// Global switch
YOKO_TYPE( 0201 , _SW     , "%SW"      )// Common switch
YOKO_TYPE( 0202 , _AN     , "%AN"      )// Annunciator
YOKO_TYPE( 0280 , _Z      , "%Z"       )// Process I/O
YOKO_TYPE( 0290 , _Y      , "%Y"       )// Process I/O
YOKO_TYPE( 0300 , _OG     , "%OG"      )// Operator guide
//KO_TYPE( 0301 , _VM     , "%VM"      )
YOKO_TYPE( 0302 , _RQ     , "%RQ"      )
//KO_TYPE( 0303 , _CP     , "%CP"      )
YOKO_TYPE( 0304 , _PR     , "%PR"      )// Printing message
//KO_TYPE( 0305 , _EV     , "%EV"      )// Signal event
//KO_TYPE( 0306 , _UP     , "%UP"      )
//KO_TYPE( 0307 , _RE     , "%RE"      )
//KO_TYPE( 0308 , _M3     , "%M3"      )
YOKO_TYPE( 1000 , PVI     , "PVI"      )
YOKO_TYPE( 1001 , PVI_DV  , "PVI-DV"   )
YOKO_TYPE( 1010 , PID     , "PID"      )
YOKO_TYPE( 1011 , PI_HLD  , "PI-HLD"   )
//KO_TYPE( 1012 , PID_BSW , "PID-BSW"  )
YOKO_TYPE( 1013 , ONOFF   , "ONOFF"    ) // Изомеризация
YOKO_TYPE( 1014 , ONOFF_G , "ONOFF-G"  )
//KO_TYPE( 1015 , PID_TP  , "PID-TP"   )
//KO_TYPE( 1016 , PD_MR   , "PD-MR"    )
//KO_TYPE( 1017 , PI_BLEND, "PI-BLEND" )
YOKO_TYPE( 1018 , PID_STC , "PID-STC"  ) // ЛЧ-24/7
YOKO_TYPE( 1020 , MLD     , "MLD"      )
YOKO_TYPE( 1021 , MLD_PVI , "MLD-PVI"  )
YOKO_TYPE( 1022 , MLD_SW  , "MLD-SW"   )
YOKO_TYPE( 1023 , MC_2    , "MC-2"     ) // Череповец
YOKO_TYPE( 1024 , MC_3    , "MC-3"     ) // Череповец
YOKO_TYPE( 1030 , RATIO   , "RATIO"    )
YOKO_TYPE( 1031 , PG_L13  , "PG-L13"   )
YOKO_TYPE( 1032 , BSETU_2 , "BSETU-2"  )
//KO_TYPE( 1033 , BSETU_3 , "BSETU-3"  )
YOKO_TYPE( 1040 , VELLIM  , "VELLIM"   )
YOKO_TYPE( 1050 , SS_H    , "SS-H"     )
YOKO_TYPE( 1051 , SS_M    , "SS-M"     )
YOKO_TYPE( 1052 , SS_L    , "SS-L"     )
YOKO_TYPE( 1053 , AS_H    , "AS-H"     )
YOKO_TYPE( 1054 , AS_M    , "AS-M"     )
YOKO_TYPE( 1055 , AS_L    , "AS-L"     )
YOKO_TYPE( 1056 , SS_DUAL , "SS-DUAL"  )
YOKO_TYPE( 1060 , FOUT    , "FOUT"     )// Был неправильный 1012
YOKO_TYPE( 1061 , FFSUM   , "FFSUM"    )
YOKO_TYPE( 1062 , XCPL    , "XCPL"     )
YOKO_TYPE( 1063 , SPLIT   , "SPLIT"    )
YOKO_TYPE( 1070 , ALM_R   , "ALM-R"    )
YOKO_TYPE( 1400 , ST16    , "ST16"     )
YOKO_TYPE( 1401 , ST16E   , "ST16E"    )
//KO_TYPE( 1402 , STEX    , "STEX"     )
//KO_TYPE( 1410 , LC16    , "LC16"     )
YOKO_TYPE( 1411 , LC64    , "LC64"     )
YOKO_TYPE( 1420 , SI_1    , "SI-1"     ) // Череповец
YOKO_TYPE( 1421 , SI_2    , "SI-2"     )
YOKO_TYPE( 1422 , SO_1    , "SO-1"     ) // Череповец
YOKO_TYPE( 1423 , SO_2    , "SO-2"     ) // Ярославль
YOKO_TYPE( 1424 , SIO_11  , "SIO-11"   )
YOKO_TYPE( 1425 , SIO_12  , "SIO-12"   ) // Ярославль
YOKO_TYPE( 1426 , SIO_21  , "SIO-21"   ) // Ярославль
YOKO_TYPE( 1427 , SIO_22  , "SIO-22"   ) // Ярославль
YOKO_TYPE( 1428 , SIO_12P , "SIO-12P"  ) // Ярославль
YOKO_TYPE( 1429 , SIO_22P , "SIO-22P"  ) // Ярославль
//KO_TYPE( 1430 , LSW     , "LSW"      )
YOKO_TYPE( 1431 , TM      , "TM"       ) // Timer
YOKO_TYPE( 1432 , CTS     , "CTS"      ) // Ярославль
//KO_TYPE( 1433 , CTP     , "CTP"      )
YOKO_TYPE( 1434 , CI      , "CI"       ) // Code input
YOKO_TYPE( 1435 , CO      , "CO"       ) // Code output
YOKO_TYPE( 1436 , RL      , "RL"       ) // Изомеризация
YOKO_TYPE( 1437 , RS      , "RS"       )
//KO_TYPE( 1438 , VLVM    , "VLVM"     )
YOKO_TYPE( 1800 , ADD     , "ADD"      ) // Изомеризация
YOKO_TYPE( 1801 , MUL     , "MUL"      )
YOKO_TYPE( 1802 , DIV     , "DIV"      )
YOKO_TYPE( 1803 , AVE     , "AVE"      )
YOKO_TYPE( 1810 , SQRT    , "SQRT"     )
//KO_TYPE( 1811 , EXP     , "EXP"      )
YOKO_TYPE( 1812 , LAG     , "LAG"      ) //Sergej
YOKO_TYPE( 1813 , INTEG   , "INTEG"    )
YOKO_TYPE( 1814 , LD      , "LD"       )
YOKO_TYPE( 1815 , RAMP    , "RAMP"     )
YOKO_TYPE( 1816 , LDLAG   , "LDLAG"    )
YOKO_TYPE( 1817 , DLAY    , "DLAY"     ) //Sergej
YOKO_TYPE( 1818 , DLAY_C  , "DLAY-C"   )
YOKO_TYPE( 1819 , AVE_M   , "AVE-M"    )
YOKO_TYPE( 181A , AVE_C   , "AVE-C"    )
//KO_TYPE( 181B , FUNC    , "FUNC"     )
YOKO_TYPE( 181C , FUNC_VAR, "FUNC-VAR" ) // ЛЧ-24/7 (не обязятельно, это что-то АСУ-шное)
YOKO_TYPE( 181D , TPCFL   , "TPCFL"    )
YOKO_TYPE( 181E , ASTM1   , "ASTM1"    )
YOKO_TYPE( 181F , ASTM2   , "ASTM2"    )
YOKO_TYPE( 1830 , AND     , "AND"      )
YOKO_TYPE( 1831 , OR      , "OR"       )
YOKO_TYPE( 1832 , NOT     , "NOT"      )
YOKO_TYPE( 1833 , SRS1_S  , "SRS1-S"   )
YOKO_TYPE( 1834 , SRS1_R  , "SRS1-R"   )
YOKO_TYPE( 1835 , SRS2_S  , "SRS2-S"   )
YOKO_TYPE( 1836 , SRS2_R  , "SRS2-R"   )
YOKO_TYPE( 1837 , WOUT    , "WOUT"     )
YOKO_TYPE( 1838 , OND     , "OND"      ) //Sergej
YOKO_TYPE( 1839 , OFFD    , "OFFD"     ) //Sergej
YOKO_TYPE( 183A , TON     , "TON"      )
YOKO_TYPE( 183B , TOFF    , "TOFF"     )
YOKO_TYPE( 183C , GT      , "GT"       )
YOKO_TYPE( 183D , GE      , "GE"       )
YOKO_TYPE( 183E , EQ      , "EQ"       )
YOKO_TYPE( 183F , BAND    , "BAND"     )
YOKO_TYPE( 1840 , BOR     , "BOR"      )
YOKO_TYPE( 1841 , BNOT    , "BNOT"     )
YOKO_TYPE( 1850 , CALCU   , "CALCU"    )
YOKO_TYPE( 1851 , CALCU_C , "CALCU-C"  )
//KO_TYPE( 1852 , CALEX   , "CALEX"    )
//KO_TYPE( 1853 , CALEX_C , "CALEX-C"  )
//KO_TYPE( 1860 , TR_SS   , "TR-SS"    )
//KO_TYPE( 1861 , TR_SM   , "TR-SM"    )
//KO_TYPE( 1862 , TR_CP   , "TR-CP"    )
//KO_TYPE( 1863 , TR_RN   , "TR-RN"    )
YOKO_TYPE( 1870 , SW_33   , "SW-33"    )//был неправильный 1875
YOKO_TYPE( 1871 , SW_91   , "SW-91"    )
YOKO_TYPE( 1872 , DSW_16  , "DSW-16"   ) //Sergej
YOKO_TYPE( 1873 , DSW_16C , "DSW-16C"  )
YOKO_TYPE( 1874 , DSET    , "DSET"     )
YOKO_TYPE( 1875 , DSET_PVI, "DSET-PVI" )
//KO_TYPE( 1876 , BDSET_1 , "BDSET-1"  )
YOKO_TYPE( 1877 , BDSET_1C, "BDSET-1C" ) // ЛЧ-24/7
//KO_TYPE( 1878 , BDSET_2 , "BDSET-2"  )
YOKO_TYPE( 1879 , BDSET_2C, "BDSET-2C" )
//KO_TYPE( 187A , BDA     , "BDA"      )
YOKO_TYPE( 187B , BDA_C   , "BDA-C"    )
YOKO_TYPE( 1C00 , INDST2  , "INDST2"   )
YOKO_TYPE( 1C01 , INDST2S , "INDST2S"  ) // ЛЧ-24/7
YOKO_TYPE( 1C02 , INDST3  , "INDST3"   )
//KO_TYPE( 1C80 , PBST5   , "PBST5"    )
//KO_TYPE( 1C81 , PBST10  , "PBST10"   )
YOKO_TYPE( 1C82 , BSI     , "BSI"      )
//KO_TYPE( 1D00 , HAST3   , "HAST3"    )
//KO_TYPE( 1D01 , HASTPV  , "HASTPV"   )
YOKO_TYPE( 1D80 , BDSET_1L, "BDSET-1L" ) // ЛЧ-24/7
YOKO_TYPE( 1D81 , BDSET_2L, "BDSET-2L" )
YOKO_TYPE( 1D82 , BDA_L   , "BDA-L"    )
//KO_TYPE( 1E00 , SEBOL   , "SEBOL"    )
//KO_TYPE( 1E01 , SEBOLP1 , "SEBOLP1"  )
//KO_TYPE( 1E11 , SEBOLP2 , "SEBOLP2"  )
//KO_TYPE( 1E12 , SEBOLP3 , "SEBOLP3"  )
YOKO_TYPE( 1E20 , PTC     , "PTC"      )
YOKO_TYPE( 1E21 , PBS5C   , "PBS5C"    )
YOKO_TYPE( 1E22 , PBS10C  , "PBS10C"   )
YOKO_TYPE( 1E23 , HAS3C   , "HAS3C"    )
//KO_TYPE( 1E24 , HASPVC  , "HASPVC"   )
YOKO_TYPE( 2100,  OPSFC   , "OPSFC"    )//155
YOKO_TYPE( 2101,  OPSBL   , "OPSBL"    )
//KO_TYPE( 2102,  OPSFCP1 , "OPSFCP1"  )
//KO_TYPE( 2103,  OPSFCP2 , "OPSFCP2"  )
//KO_TYPE( 2104,  OPSFCP3 , "OPSFCP3"  )
//KO_TYPE( 2105,  OPSFCP4 , "OPSFCP4"  )
//KO_TYPE( 2106,  OPSFCP5 , "OPSFCP5"  )
// Настраиваемые типы (замечены min=0x2200, max=0x294 )
YOKO_TYPE( 2200,  _SFCSW  , "_SFCSW"   )
YOKO_TYPE( 2201,  _SFCPB  , "_SFCPB"   )
YOKO_TYPE( 2202,  _SFCAS  , "_SFCAS"   )
YOKO_TYPE( 223C,  _UTAS   , "_UTAS"    )
YOKO_TYPE( 22B4,  _UTSW   , "_UTSW"    )
YOKO_TYPE( 2943,  _UTPB   , "_UTPB"  )//137
YOKO_TYPE( 2944,  _UTPB_N , "_UTPB-N"  )//155
//
//KO_TYPE( 5001 , FSBSET  , "FSBSET"   )
//KO_TYPE( 5002 , BLEND   , "BLEND"    )
YOKO_TYPE( 5003 , LC64_E  , "LC64-E"   )//91
YOKO_TYPE( 5004 , SI_1ALM , "SI-1ALM" )//137
YOKO_TYPE( 5050 , _WW     , "%WW"      )// Communication I/O
YOKO_TYPE( 5051 , _WB     , "%WB"      )// Communication I/O
YOKO_TYPE( 5052 , _XW     , "%XW"      )// Communication I/O 181
//KO_TYPE( 5090 , SLCD    , "SLCD"     )
//KO_TYPE( 5091 , SLPC    , "SLPC"     )
//KO_TYPE( 5092 , SLMC    , "SLMC"     )
//KO_TYPE( 5093 , SMST_111, "SMST-111" )
//KO_TYPE( 5094 , SMST_121, "SMST-121" )
//KO_TYPE( 5095 , SMRT    , "SMRT"     )
//KO_TYPE( 5096 , SBSD    , "SBSD"     )
//KO_TYPE( 5097 , SLCC    , "SLCC"     )
//KO_TYPE( 5098 , SLBC    , "SLBC"     )
//KO_TYPE( 5099 , STLD    , "STLD"     )
YOKO_TYPE( 50C0 , FF_AI   , "FF-AI"    )
YOKO_TYPE( 50C1 , FF_DI   , "FF-DI"    )
//KO_TYPE( 50C4 , FF_CS   , "FF-CS"    )
YOKO_TYPE( 50C6 , FF_PID  , "FF-PID"   )
//KO_TYPE( 50C7 , FF_RA   , "FF-RA"    )
YOKO_TYPE( 50C8 , FF_AO   , "FF-AO"    )
YOKO_TYPE( 50C9 , FF_DO   , "FF-DO"    )
//KO_TYPE( 50CB , FF_OS   , "FF-OS"    )
//KO_TYPE( 50CC , FF_SC   , "FF-SC"    )
//KO_TYPE( 50CF , FF_IT   , "FF-IT"    )
YOKO_TYPE( 50D1 , FF_IS   , "FF-IS"    )
//KO_TYPE( 50D5 , FF_MDI  , "FF-MDI"   )
//KO_TYPE( 50D6 , FF_MDO  , "FF-MDO"   )
YOKO_TYPE( 50D7 , FF_MAI  , "FF-MAI"   )
//KO_TYPE( 50D8 , FF_MAO  , "FF-MAO"   )
//KO_TYPE( 50D9 , FF_SUNF , "FF-SUNF"  )
//KO_TYPE( 50E0 , ONOFF_E , "ONOFF_E"  )
YOKO_TYPE( 50E1 , ONOFF_GE, "ONOFF-GE" )
YOKO_TYPE( 50E2 , MC_2E   , "MC-2E"    )
YOKO_TYPE( 50E3 , MC_3E   , "MC-3E"    )
YOKO_TYPE( 50E4 , SI_1E   , "SI-1E"    )
YOKO_TYPE( 50E5 , SI_2E   , "SI-2E"    )
YOKO_TYPE( 50E6 , SO_1E   , "SO-1E"    )
YOKO_TYPE( 50E7 , SO_2E   , "SO-2E"    )
YOKO_TYPE( 50E8 , SIO_11E , "SIO-11E"  )
YOKO_TYPE( 50E9 , SIO_12E , "SIO-12E"  )
YOKO_TYPE( 50EA , SIO_21E , "SIO-21E"  )
YOKO_TYPE( 50EB , SIO_22E , "SIO-22E"  )
YOKO_TYPE( 50EC , SIO_12PE, "SIO-12PE" )
YOKO_TYPE( 50ED , SIO_22PE, "SIO-22PE" )

//>>
//BCVO OPC
YOKO_TYPE( 5280 , BOPCSYS,  "BOPCSYS" )
YOKO_TYPE( 5287 , BOPCCHR,  "BOPCCHR" )
YOKO_TYPE( 5289 , BOPCBOL,  "BOPCBOL" )
YOKO_TYPE( 528A , BOPCI16,  "BOPCI16" )
YOKO_TYPE( 528B , BOPCU16,  "BOPCU16" )
YOKO_TYPE( 528C , BOPCI32,  "BOPCI32" )
YOKO_TYPE( 528D , BOPCU32,  "BOPCU32" )
YOKO_TYPE( 528E , BOPCF32,  "BOPCF32" )
YOKO_TYPE( 528F , BOPCF64,  "BOPCF64" )
YOKO_TYPE( 52A0 , SR_FNCVR, "SR-FNCVR" )
YOKO_TYPE( 52AB , SR_PID,   "SR-PID" )
//<<

//prosafe
YOKO_TYPE( 5300 , S_ANLGI,  "S_ANLGI" )
YOKO_TYPE( 5301 , S_VEL,    "S_VEL" )
YOKO_TYPE( 5302 , S_OVR_B,  "S_OVR_B" )
YOKO_TYPE( 5307 , S_PASSWD, "S_PASSWD" )
YOKO_TYPE( 5308 , S_ANLG_S, "S_ANLG_S" )
YOKO_TYPE( 5309 , S_MOB_21, "S_MOB_21" )
YOKO_TYPE( 530A , S_MOB_11, "S_MOB_11" )
YOKO_TYPE( 530B , S_MOB_RS, "S_MOB_RS" )
YOKO_TYPE( 530C , S_MOA,    "S_MOA"    )
YOKO_TYPE( 530D , S_GOV_B,  "S_GOV_B"  )
YOKO_TYPE( 53CC , ANN_FUP,  "ANN_FUP" )//возможно его надо будет исключить
//KO_TYPE( 5497 , ZCTEMP  , "ZCTEMP"   )
