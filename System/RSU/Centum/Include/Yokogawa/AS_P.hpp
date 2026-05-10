//
YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
//
YPARM_Dref(OPHI,W->SH,"----")
YPARM_Dref(OPLO,W->SL,"----")
YPARM_Dref(MH,W->MSH,"----")
YPARM_Dref(ML,W->MSL,"----")

YPARM_DDS(MSH,MSL,cMesMV,W->SH,W->SL,8,"AORA","%D:%D:%S")
//
YPARM_I(SWH,4,"----")
YPARM_I(SWL,1,"----")
//
YPARM_DB(Out_VL,MAN_bypass,100.,false,"OVLM", "%D:%B")
YPARM_B(Limit_On_CLP,false,"CLPO")
YPARM_L(Positional,1,"CCOU","%{VELOCITY;POSITION}")
YPARM_B(PV_Overshoot,false,"OVPV")
YPARM_B(PV_Limit,false,"PVRL")
//
YPARM_I(ENCCS1,0,"----")
YPARM_I(ENCCS2,0,"----")
YPARM_I(ENCCS3,0,"----")
YPARM_D(RV1_const,0,"----")
YPARM_D(RV2_const,0,"----")
YPARM_D(RV3_const,0,"----")

YPARM_I(PSW,0,"----")

