//
YPARM_S(Comment,64,"ETCM")
YPARM_DD(SH,SL,DBL_MAX,DBL_MAX,"ESCL","%D:%D") 
//
YPARM_D(GAIN,1.,"RTGN");
//
YPARM_I( kAvereged, 1, "AVCT" )
YPARM_I( Sample_All, 0, "SPTA" )
YPARM_I( Calc_Val_Err, 0, "CABA" );
YPARM_L( PositionalOut, 0, "CCOU", "%{POSITION;VELOCITY}" );
YPARM_L(Aux,0,"SUOU","%{PV;DPV;MV;DMV}")
YPARM_D( SMPL, 1, "SMPL")//Sampling interval
YPARM_I( NUM, 1, "NUM" )//Number of samples
