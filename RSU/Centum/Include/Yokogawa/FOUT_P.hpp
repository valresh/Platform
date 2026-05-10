YPARM_S(Comment,64,"ETCM")
YPARM_DD(SSH,SSL,DBL_MAX,DBL_MAX,"SVRG","%D:%D") 
//
YPARM_I(Mes,-1,"----")
YPARM_S(cMes,8,"EUNT")
//
YPARM_Dref(MSL1,W->SSL,"----")
YPARM_Dref(MSL2,W->SSL,"----")
YPARM_Dref(MSL3,W->SSL,"----")
YPARM_Dref(MSL4,W->SSL,"----")
YPARM_Dref(MSL5,W->SSL,"----")
YPARM_Dref(MSL6,W->SSL,"----")
YPARM_Dref(MSL7,W->SSL,"----")
YPARM_Dref(MSL8,W->SSL,"----")
//
YPARM_Dref(MSH1,W->SSH,"----")
YPARM_Dref(MSH2,W->SSH,"----")
YPARM_Dref(MSH3,W->SSH,"----")
YPARM_Dref(MSH4,W->SSH,"----")
YPARM_Dref(MSH5,W->SSH,"----")
YPARM_Dref(MSH6,W->SSH,"----")
YPARM_Dref(MSH7,W->SSH,"----")
YPARM_Dref(MSH8,W->SSH,"----")
//
YPARM_L(Display_style_MV,0,"MV99","%{AUTO;SELF}")//Display style for MV data"
//
SKIP("MVRG") //Диапазон MV при MV99=SELF(Used defined)