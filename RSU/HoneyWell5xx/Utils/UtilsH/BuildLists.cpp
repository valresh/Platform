#include <HoneywellStruct.h>
#include <ShData.h>
#include "ListValue.h"

#include "Sh_Sheme.h"
#include <HoneywellEnums.h>
#pragma warning(push)
#pragma warning(disable : 4101)

void W_xC300::BuildList(void)
{
//#define shareObj W_xC300
//#define netObj CShxC300
  typedef W_xC300 shareObj;
  typedef CShxC300 netObj;
  #include "Modules/xC300.h" 
  #include "Modules/xC300_P.h"
  ACK_ALARM
}

void W_Dx_24y::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_Dx_24y shareObj;
  typedef CShDx_24y netObj;
  #include "Modules/Dx_24y.h" 
  #include "Modules/Dx_24y_P.h"
  ACK_ALARM
}

void W_xHART_HL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_xHART_HL shareObj;
  typedef CShxHART_HL netObj;
  #include "Modules/xHART_HL.h"
  #include "Modules/xHART_HL_P.h"
  ACK_ALARM
}

void W_CONTROLMODULE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_CONTROLMODULE shareObj;
  typedef CShCONTROLMODULE netObj;
  #include "Modules/CONTROLMODULE.h" 
  #include "Modules/CONTROLMODULE_P.h"
  ACK_ALARM
}

void W_IOLINK::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_IOLINK shareObj;
  typedef CShIOLINK netObj;
  #include "Modules/IOLINK.h" 
  #include "Modules/IOLINK_P.h"
  ACK_ALARM
}

void W_CEEC300::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_CEEC300 shareObj;
    typedef CShCEEC300 netObj;
  #include "Modules/CEEC300.h" 
  #include "Modules/CEEC300_P.h"
}

void W_PCDI_MASTER::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_PCDI_MASTER shareObj;
  typedef CShPCDI_MASTER netObj;
  #include "Modules/PCDI_MASTER.h" 
  #include "Modules/PCDI_MASTER_P.h"
  ACK_ALARM
}

void W_DIV::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DIV shareObj;
  typedef CShDIV netObj;
  #include "Blocks/DIV.h" 
  #include "Blocks/DIV_P.h"
  ACK_ALARM
}

void W_MUL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_MUL shareObj;
  typedef CShMUL netObj;
  #include "Blocks/MUL.h" 
  #include "Blocks/MUL_P.h"
  ACK_ALARM
}

void W_ADD::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ADD shareObj;
  typedef CShADD netObj;
  #include "Blocks/ADD.h" 
  #include "Blocks/ADD_P.h"
  ACK_ALARM
}

void W_SELREAL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SELREAL shareObj;
  typedef CShSELREAL netObj;
  #include "Blocks/SELREAL.h" 
  #include "Blocks/SELREAL_P.h"
  ACK_ALARM
}

void W_EQ::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_EQ shareObj;
  typedef CShEQ netObj;
  #include "Blocks/EQ.h" 
  #include "Blocks/EQ_P.h"
  ACK_ALARM
}

void W_PULSE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_PULSE shareObj;
  typedef CShPULSE netObj;
  #include "Blocks/PULSE.h" 
  #include "Blocks/PULSE_P.h"
  ACK_ALARM
}

void W_AND::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AND shareObj;
  typedef CShAND netObj;
  #include "Blocks/AND.h" 
  #include "Blocks/AND_P.h"
  ACK_ALARM
}

void W_OR::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_OR shareObj;
  typedef CShOR netObj;
  #include "Blocks/OR.h" 
  #include "Blocks/OR_P.h"
  ACK_ALARM
}

void W_NOT::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_NOT shareObj;
  typedef CShNOT netObj;
  #include "Blocks/NOT.h" 
  #include "Blocks/NOT_P.h"
  ACK_ALARM
}

void W_DIGACQ::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DIGACQ shareObj;
  typedef CShDIGACQ netObj;
  #include "Blocks/DIGACQ.h" 
  #include "Blocks/DIGACQ_P.h"
  ACK_ALARM
}

void W_RBPERMISSION::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_RBPERMISSION shareObj;
  typedef CShRBPERMISSION netObj;
  #include "Blocks/RBPERMISSION.h" 
  #include "Blocks/RBPERMISSION_P.h"
  ACK_ALARM
}

void W_AUTOMAN::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AUTOMAN shareObj;
  typedef CShAUTOMAN netObj;
  #include "Blocks/AUTOMAN.h" 
  #include "Blocks/AUTOMAN_P.h"
  ACK_ALARM
}

void W_FANOUT::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_FANOUT shareObj;
  typedef CShFANOUT netObj;
  #include "Blocks/FANOUT.h" 
  #include "Blocks/FANOUT_P.h"
  ACK_ALARM
}

void W_TEXTCOMMENT::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TEXTCOMMENT shareObj;
  typedef CShTEXTCOMMENT netObj;
  #include "Blocks/TEXTCOMMENT.h" 
  #include "Blocks/TEXTCOMMENT_P.h"
  ACK_ALARM
}

void W_TOTALIZER::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TOTALIZER shareObj;
  typedef CShTOTALIZER netObj;
  #include "Blocks/TOTALIZER.h" 
  #include "Blocks/TOTALIZER_P.h"
  ACK_ALARM
}

void W_PID::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_PID shareObj;
  typedef CShPID netObj;
  #include "Blocks/PID.h" 
  #include "Blocks/PID_P.h"
  ACK_ALARM
}

void W_NUMERIC::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_NUMERIC shareObj;
  typedef CShNUMERIC netObj;
  #include "Blocks/NUMERIC.h" 
  #include "Blocks/NUMERIC_P.h"
  ACK_ALARM
}

void W_TYPECONVERT::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TYPECONVERT shareObj;
  typedef CShTYPECONVERT netObj;
  #include "Blocks/TYPECONVERT.h" 
  #include "Blocks/TYPECONVERT_P.h"
  ACK_ALARM
}

void W_FLAG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_FLAG shareObj;
  typedef CShFLAG netObj;
  #include "Blocks/FLAG.h" 
  #include "Blocks/FLAG_P.h"
  ACK_ALARM
}

void W_DEVCTL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DEVCTL shareObj;
  typedef CShDEVCTL netObj;
  #include "Blocks/DEVCTL.h" 
  #include "Blocks/DEVCTL_P.h"
  ACK_ALARM
}

void W_DATAACQ::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DATAACQ shareObj;
  typedef CShDATAACQ netObj;
  #include "Blocks/DATAACQ.h" 
  #include "Blocks/DATAACQ_P.h"
  ACK_ALARM
}

void W_AUXCALC::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AUXCALC shareObj;
  typedef CShAUXCALC netObj;
  #include "Blocks/AUXCALC.h" 
  #include "Blocks/AUXCALC_P.h"
  ACK_ALARM
}

void W_DOCHANNEL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DOCHANNEL shareObj;
  typedef CShDOCHANNEL netObj;
#include "Blocks/DOCHANNEL.h" 
#include "Blocks/DOCHANNEL_P.h"
  ACK_ALARM
}

void W_SPARE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SPARE shareObj;
  typedef CShSPARE netObj;
#include "Blocks/SPARE.h" 
#include "Blocks/SPARE_P.h"
  ACK_ALARM
}

void W_DICHANNEL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DICHANNEL shareObj;
  typedef CShDICHANNEL netObj;
#include "Blocks/DICHANNEL.h" 
#include "Blocks/DICHANNEL_P.h"
  ACK_ALARM
}

void W_AOCHANNEL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AOCHANNEL shareObj;
  typedef CShAOCHANNEL netObj;
#include "Blocks/AOCHANNEL.h" 
#include "Blocks/AOCHANNEL_P.h"
  ACK_ALARM
}

void W_AICHANNEL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AICHANNEL shareObj;
  typedef CShAICHANNEL netObj;
#include "Blocks/AICHANNEL.h" 
#include "Blocks/AICHANNEL_P.h"
  ACK_ALARM
}

void W_PCDINUMARRCH::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_PCDINUMARRCH shareObj;
  typedef CShPCDINUMARRCH netObj;
#include "Blocks/PCDINUMARRCH.h" 
#include "Blocks/PCDINUMARRCH_P.h"
  ACK_ALARM
}

void W_PCDIFLAGARRCH::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_PCDIFLAGARRCH shareObj;
  typedef CShPCDIFLAGARRCH netObj;
#include "Blocks/PCDIFLAGARRCH.h" 
#include "Blocks/PCDIFLAGARRCH_P.h"
  ACK_ALARM
}

void W_ROOT::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ROOT shareObj;
  typedef CShROOT netObj;
#include "Modules/ROOT_P.h"
}

void W_FIM4::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_FIM4 shareObj;
  typedef CShFIM4 netObj;
#include "Modules/FIM4.h" 
#include "Modules/FIM4_P.h"
  ACK_ALARM
}

void W_FFLINK::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_FFLINK shareObj;
  typedef CShFFLINK netObj;
#include "Modules/FFLINK.h" 
#include "Modules/FFLINK_P.h"
  ACK_ALARM
}

void W_848_0601::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_848_0601 shareObj;
  typedef CSh848_0601 netObj;
#include "Blocks/848_0601.h" 
#include "Blocks/848_0601_P.h"
  ACK_ALARM
}

void W_FLOWCOMP::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_FLOWCOMP shareObj;
  typedef CShFLOWCOMP netObj;
#include "Blocks/FLOWCOMP.h" 
#include "Blocks/FLOWCOMP_P.h"
  ACK_ALARM
}

void W_OFFDELAY::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_OFFDELAY shareObj;
  typedef CShOFFDELAY netObj;
#include "Blocks/OFFDELAY.h" 
#include "Blocks/OFFDELAY_P.h"
  ACK_ALARM
}

void W_Dx_SOE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_Dx_SOE shareObj;
  typedef CShDx_SOE netObj;
#include "Modules/Dx_SOE.h" 
#include "Modules/Dx_SOE_P.h"
  ACK_ALARM
}

void W_ROLLAVG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ROLLAVG shareObj;
  typedef CShROLLAVG netObj;
#include "Blocks/ROLLAVG.h" 
#include "Blocks/ROLLAVG_P.h"
  ACK_ALARM
}

void W_ONDELAY::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ONDELAY shareObj;
  typedef CShONDELAY netObj;
#include "Blocks/ONDELAY.h" 
#include "Blocks/ONDELAY_P.h"
  ACK_ALARM
}

void W_DELAY::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DELAY shareObj;
  typedef CShDELAY netObj;
#include "Blocks/DELAY.h" 
#include "Blocks/DELAY_P.h"
  ACK_ALARM
}

void W_NOR::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_NOR shareObj;
  typedef CShNOR netObj;
#include "Blocks/NOR.h" 
#include "Blocks/NOR_P.h"
  ACK_ALARM
}

void W_MAI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_MAI shareObj;
  typedef CShMAI netObj;
#include "Blocks/MAI.h" 
#include "Blocks/MAI_P.h"
  ACK_ALARM
}

void W_MAXPULSE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_MAXPULSE shareObj;
  typedef CShMAXPULSE netObj;
#include "Blocks/MAXPULSE.h" 
#include "Blocks/MAXPULSE_P.h"
  ACK_ALARM
}

void W_LT::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_LT shareObj;
  typedef CShLT netObj;
#include "Blocks/LT.h" 
#include "Blocks/LT_P.h"
  ACK_ALARM
}

void W_ENHAUXCALC::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ENHAUXCALC shareObj;
  typedef CShENHAUXCALC netObj;
#include "Blocks/ENHAUXCALC.h" 
#include "Blocks/ENHAUXCALC_P.h"
  ACK_ALARM
}

void W_SIGNALSEL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SIGNALSEL shareObj;
  typedef CShSIGNALSEL netObj;
#include "Blocks/SIGNALSEL.h" 
#include "Blocks/SIGNALSEL_P.h"
  ACK_ALARM
}

void W_9413_DIGIT_0102::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_9413_DIGIT_0102 shareObj;
  typedef CSh9413_DIGIT_0102 netObj;
#include "Modules/9413-DIGIT_0102.h" 
#include "Modules/9413-DIGIT_0102_P.h"
  ACK_ALARM
}

void W_9413_DIGIT_0102_RESOURCE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_9413_DIGIT_0102_RESOURCE shareObj;
  typedef CSh9413_DIGIT_0102_RESOURCE netObj;
#include "Blocks/9413-DIGIT_0102.RESOURCE.h" 
#include "Blocks/9413-DIGIT_0102.RESOURCE_P.h"
  ACK_ALARM
}

void W_9413_DIGIT_0102_DO1TB::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_9413_DIGIT_0102_DO1TB shareObj;
  typedef CSh9413_DIGIT_0102_DO1TB netObj;
#include "Blocks/9413-DIGIT_0102.DO1TB.h" 
#include "Blocks/9413-DIGIT_0102.DO1TB_P.h"
  ACK_ALARM
}

void W_9413_DIGIT_0102_MDO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_9413_DIGIT_0102_MDO shareObj;
  typedef CSh9413_DIGIT_0102_MDO netObj;
#include "Blocks/9413-DIGIT_0102.MDO.h" 
#include "Blocks/9413-DIGIT_0102.MDO_P.h"
  ACK_ALARM
}

void W_9413_DIGIT_0102_AI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_9413_DIGIT_0102_AI shareObj;
  typedef CSh9413_DIGIT_0102_AI netObj;
#include "Blocks/9413-DIGIT_0102.AI.h" 
#include "Blocks/9413-DIGIT_0102.AI_P.h"
  ACK_ALARM
}

void W_RS::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_RS shareObj;
  typedef CShRS netObj;
#include "Blocks/RS.h" 
#include "Blocks/RS_P.h"
  ACK_ALARM
}

void W_SR::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SR shareObj;
  typedef CShSR netObj;
#include "Blocks/SR.h" 
#include "Blocks/SR_P.h"
  ACK_ALARM
}

void W_TC_IAH161::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TC_IAH161 shareObj;
    typedef CShTC_IAH161 netObj;
#include "Modules/TC-IAH161.h" 
#include "Modules/TC-IAH161_P.h"
}

void W_PUSH::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_PUSH shareObj;
  typedef CShPUSH netObj;
#include "Blocks/PUSH.h" 
#include "Blocks/PUSH_P.h"
  ACK_ALARM
}

void W_TIMER::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TIMER shareObj;
  typedef CShTIMER netObj;
#include "Blocks/TIMER.h" 
#include "Blocks/TIMER_P.h"
  ACK_ALARM
}

void W_CHECKBAD::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_CHECKBAD shareObj;
  typedef CShCHECKBAD netObj;
#include "Blocks/CHECKBAD.h" 
#include "Blocks/CHECKBAD_P.h"
  ACK_ALARM
}

void W_GE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_GE shareObj;
  typedef CShGE netObj;
#include "Blocks/GE.h" 
#include "Blocks/GE_P.h"
  ACK_ALARM
}

void W_GT::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_GT shareObj;
  typedef CShGT netObj;
#include "Blocks/GT.h" 
#include "Blocks/GT_P.h"
  ACK_ALARM
}

void W_SWITCH::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SWITCH shareObj;
  typedef CShSWITCH netObj;
#include "Blocks/SWITCH.h" 
#include "Blocks/SWITCH_P.h"
  ACK_ALARM
}

void W_REGCALC::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_REGCALC shareObj;
  typedef CShREGCALC netObj;
#include "Blocks/REGCALC.h" 
#include "Blocks/REGCALC_P.h"
  ACK_ALARM
}

void W_OVRDSEL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_OVRDSEL shareObj;
  typedef CShOVRDSEL netObj;
#include "Blocks/OVRDSEL.h" 
#include "Blocks/OVRDSEL_P.h"
  ACK_ALARM
}

void W_RATIOCTL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_RATIOCTL shareObj;
  typedef CShRATIOCTL netObj;
#include "Blocks/RATIOCTL.h" 
#include "Blocks/RATIOCTL_P.h"
  ACK_ALARM
}

void W_ENHREGCALC::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ENHREGCALC shareObj;
  typedef CShENHREGCALC netObj;
#include "Blocks/ENHREGCALC.h" 
#include "Blocks/ENHREGCALC_P.h"
  ACK_ALARM
}

void W_LE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_LE shareObj;
  typedef CShLE netObj;
#include "Blocks/LE.h" 
#include "Blocks/LE_P.h"
  ACK_ALARM
}

void W_NE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_NE shareObj;
  typedef CShNE netObj;
#include "Blocks/NE.h" 
#include "Blocks/NE_P.h"
  ACK_ALARM
}

void W_FTRIG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_FTRIG shareObj;
  typedef CShFTRIG netObj;
#include "Blocks/FTRIG.h" 
#include "Blocks/FTRIG_P.h"
  ACK_ALARM
}

void W_CPM200::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_CPM200 shareObj;
  typedef CShCPM200 netObj;
#include "Modules/CPM200.h" 
#include "Modules/CPM200_P.h"
  ACK_ALARM
}

void W_CEEFB::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_CEEFB shareObj;
  typedef CShCEEFB netObj;
#include "Modules/CEEFB.h" 
#include "Modules/CEEFB_P.h"
}

void W_TC_IDD321::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TC_IDD321 shareObj;
  typedef CShTC_IDD321 netObj;
#include "Modules/TC-IDD321.h" 
#include "Modules/TC-IDD321_P.h"
}

void W_TC_ODD321::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TC_ODD321 shareObj;
  typedef CShTC_ODD321 netObj;
#include "Modules/TC-ODD321.h" 
#include "Modules/TC-ODD321_P.h"
}

void W_RTRIG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_RTRIG shareObj;
  typedef CShRTRIG netObj;
#include "Blocks/RTRIG.h" 
#include "Blocks/RTRIG_P.h"
  ACK_ALARM
}

void W_NUMERICARRAY::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_NUMERICARRAY shareObj;
  typedef CShNUMERICARRAY netObj;
#include "Blocks/NUMERICARRAY.h" 
#include "Blocks/NUMERICARRAY_P.h"
  ACK_ALARM
}

void W_SUB::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SUB shareObj;
  typedef CShSUB netObj;
#include "Blocks/SUB.h" 
#include "Blocks/SUB_P.h"
  ACK_ALARM
}

void W_REGSUMMER::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_REGSUMMER shareObj;
  typedef CShREGSUMMER netObj;
#include "Blocks/REGSUMMER.h" 
#include "Blocks/REGSUMMER_P.h"
  ACK_ALARM
}

void W_9413_DIGIT_0102_MDI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_9413_DIGIT_0102_MDI shareObj;
  typedef CSh9413_DIGIT_0102_MDI netObj;
#include "Blocks/9413-DIGIT_0102.MDI.h" 
#include "Blocks/9413-DIGIT_0102.MDI_P.h"
  ACK_ALARM
}

void W_MESSAGE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_MESSAGE shareObj;
  typedef CShMESSAGE netObj;
#include "Blocks/MESSAGE.h" 
#include "Blocks/MESSAGE_P.h"
  ACK_ALARM
}

void W_FTEB::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_FTEB shareObj;
  typedef CShFTEB netObj;
#include "Modules/FTEB.h" 
#include "Modules/FTEB_P.h"
}

void W_TEXTARRAY::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TEXTARRAY shareObj;
  typedef CShTEXTARRAY netObj;
#include "Blocks/TEXTARRAY.h" 
#include "Blocks/TEXTARRAY_P.h"
}

void W_RMFBCLASS::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_RMFBCLASS shareObj;
  typedef CShRMFBCLASS netObj;
#include "Modules/RMFBCLASS.h" 
#include "Modules/RMFBCLASS_P.h"
}

void W_TC_IXL061::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TC_IXL061 shareObj;
  typedef CShTC_IXL061 netObj;
#include "Modules/tc-ixl061.h" 
#include "Modules/TC-IXL061_P.h"
}

void W_PITOTALIZER::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_PITOTALIZER shareObj;
  typedef CShPITOTALIZER netObj;
#include "Blocks/PITOTALIZER.h" 
#include "Blocks/PITOTALIZER_P.h"
  ACK_ALARM
}

void W_PICHANNEL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_PICHANNEL shareObj;
  typedef CShPICHANNEL netObj;
#include "Blocks/PICHANNEL.h" 
#include "Blocks/PICHANNEL_P.h"
  ACK_ALARM
}

void W_TC_MDP081::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TC_MDP081 shareObj;
  typedef CShTC_MDP081 netObj;
#include "Modules/TC-MDP081.h" 
#include "Modules/TC-MDP081_P.h"
}

void W_TC_IXL062::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TC_IXL062 shareObj;
  typedef CShTC_IXL062 netObj;
#include "Modules/TC-IXL062.h" 
#include "Modules/TC-IXL062_P.h"
}

void W_TC_OAV081::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TC_OAV081 shareObj;
  typedef CShTC_OAV081 netObj;
#include "Modules/TC-OAV081.h" 
#include "Modules/TC-OAV081_P.h"
}

void W_PIDFF::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_PIDFF shareObj;
  typedef CShPIDFF netObj;
#include "Blocks/PIDFF.h" 
#include "Blocks/PIDFF_P.h"
  ACK_ALARM
}

void W_DEADTIME::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DEADTIME shareObj;
  typedef CShDEADTIME netObj;
#include "Blocks/DEADTIME.h" 
#include "Blocks/DEADTIME_P.h"
  ACK_ALARM
}

void W_AI_LLMUX::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AI_LLMUX shareObj;
  typedef CShAI_LLMUX netObj;
#include "Modules/AI-LLMUX.h" 
#include "Modules/AI-LLMUX_P.h"
  ACK_ALARM
}

void W_TC_IXR061::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TC_IXR061 shareObj;
  typedef CShTC_IXR061 netObj;
#include "Modules/TC-IXR061.h" 
#include "Modules/TC-IXR061_P.h"
}

void W_AIREF::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AIREF shareObj;
  typedef CShAIREF netObj;
#include "Blocks/AIREF.h" 
#include "Blocks/AIREF_P.h"
  ACK_ALARM
}

void W_AOREF::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AOREF shareObj;
  typedef CShAOREF netObj;
#include "Blocks/AOREF.h" 
#include "Blocks/AOREF_P.h"
  ACK_ALARM
}

void W_DIREF::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DIREF shareObj;
  typedef CShDIREF netObj;
#include "Blocks/DIREF.h" 
#include "Blocks/DIREF_P.h"
  ACK_ALARM
}

void W_DOREF::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DOREF shareObj;
  typedef CShDOREF netObj;
#include "Blocks/DOREF.h" 
#include "Blocks/DOREF_P.h"
  ACK_ALARM
}

void W_NUM2BOOL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_NUM2BOOL shareObj;
  typedef CShNUM2BOOL netObj;
#include "Blocks/NUM2BOOL.h" 
#include "Blocks/NUM2BOOL_P.h"
  ACK_ALARM
}

void W_BOOL2NUM::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_BOOL2NUM shareObj;
  typedef CShBOOL2NUM netObj;
#include "Blocks/BOOL2NUM.h" 
#include "Blocks/BOOL2NUM_P.h"
  ACK_ALARM
}

void W_INTERLOCK::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_INTERLOCK shareObj;
  typedef CShINTERLOCK netObj;
#include "Blocks/INTERLOCK.h" 
#include "Blocks/INTERLOCK_P.h"
  ACK_ALARM
}

void W_2OO3::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_2OO3 shareObj;
  typedef CSh2OO3 netObj;
#include "Blocks/2OO3.h" 
#include "Blocks/2OO3_P.h"
  ACK_ALARM
}

void W_QOR::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_QOR shareObj;
  typedef CShQOR netObj;
#include "Blocks/QOR.h" 
#include "Blocks/QOR_P.h"
  ACK_ALARM
}

void W_FIRSTOUT::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_FIRSTOUT shareObj;
  typedef CShFIRSTOUT netObj;
#include "Blocks/FIRSTOUT.h" 
#include "Blocks/FIRSTOUT_P.h"
  ACK_ALARM
}

void W_ABS::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ABS shareObj;
  typedef CShABS netObj;
#include "Blocks/ABS.h" 
#include "Blocks/ABS_P.h"
  ACK_ALARM
}

void W_GENLIN::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_GENLIN shareObj;
  typedef CShGENLIN netObj;
#include "Blocks/GENLIN.h" 
#include "Blocks/GENLIN_P.h"
  ACK_ALARM
}
void W_SCM::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SCM shareObj;
  typedef CShSCM netObj;
#include "Modules/SCM.h"
#include "Modules/SCM_P.h"
}

void W_HANDLER::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_HANDLER shareObj;
  typedef CShHANDLER netObj;
#include "Blocks/HANDLER.h"
#include "Blocks/HANDLER_P.h"
}

void W_TRANSITION::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TRANSITION shareObj;
  typedef CShTRANSITION netObj;
#include "Blocks/TRANSITION.h"
#include "Blocks/TRANSITION_P.h"
}

void W_STEP::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_STEP shareObj;
  typedef CShSTEP netObj;
#include "Blocks/STEP.h"
#include "Blocks/STEP_P.h"
}

void W_MUXREAL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_MUXREAL shareObj;
  typedef CShMUXREAL netObj;
#include "Blocks/MUXREAL.h"
#include "Blocks/MUXREAL_P.h"
}

void W_MAX::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_MAX shareObj;
  typedef CShMAX netObj;
#include "Blocks/MAX.h"
#include "Blocks/MAX_P.h"
}

void W_CTUD::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_CTUD shareObj;
  typedef CShCTUD netObj;
#include "Blocks/CTUD.h"
#include "Blocks/CTUD_P.h"
}

void W_HLAIHART::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_HLAIHART shareObj;
  typedef CShHLAIHART netObj;
#include "Modules/HLAIHART.h"
#include "Modules/HLAIHART_P.h"
}

void W_HLAI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_HLAI shareObj;
  typedef CShHLAI netObj;
#include "Modules/HLAI.h"
#include "Modules/HLAI_P.h"
}

void W_AO16::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AO16 shareObj;
  typedef CShAO16 netObj;
#include "Modules/AO16.h"
#include "Modules/AO16_P.h"
}

void W_DO32::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DO32 shareObj;
  typedef CShDO32 netObj;
#include "Modules/DO32.h"
#include "Modules/DO32_P.h"
}

void W_DI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DI shareObj;
  typedef CShDI netObj;
#include "Modules/DI.h"
#include "Modules/DI_P.h"
}

void W_AUXSUMMER::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_AUXSUMMER shareObj;
  typedef CShAUXSUMMER netObj;
#include "Blocks/AUXSUMMER.h"
#include "Blocks/AUXSUMMER_P.h"
}

void W_LIMIT::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_LIMIT shareObj;
  typedef CShLIMIT netObj;
#include "Blocks/LIMIT.h"
#include "Blocks/LIMIT_P.h"
}

void W_MINPULSE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_MINPULSE shareObj;
  typedef CShMINPULSE netObj;
#include "Blocks/MINPULSE.h"
#include "Blocks/MINPULSE_P.h"
}

void W_DISOE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DISOE shareObj;
  typedef CShDISOE netObj;
#include "Modules/DISOE.h"
#include "Modules/DISOE_P.h"
}

void W_HAICHANNEL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_HAICHANNEL shareObj;
  typedef CShHAICHANNEL netObj;
#include "Blocks/HAICHANNEL.h"
#include "Blocks/HAICHANNEL_P.h"
}

void W_DI24V::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_DI24V shareObj;
  typedef CShDI24V netObj;
#include "Modules/DI24V.h"
#include "Modules/DI24V_P.h"
}

void W_LLMUX::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_LLMUX shareObj;
  typedef CShLLMUX netObj;
#include "Modules/LLMUX.h"
#include "Modules/LLMUX_P.h"
}

void W_XOR::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_XOR shareObj;
  typedef CShXOR netObj;
#include "Blocks/XOR.h" 
#include "Blocks/XOR_P.h"
  ACK_ALARM
}

void W_WATCHDOG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_WATCHDOG shareObj;
  typedef CShWATCHDOG netObj;
#include "Blocks/WATCHDOG.h" 
#include "Blocks/WATCHDOG_P.h"
  ACK_ALARM
}

void W_UIO_2::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_UIO_2 shareObj;
  typedef CShUIO_2 netObj;
#include "Modules/UIO-2.h" 
#include "Modules/UIO-2_P.h"
  ACK_ALARM
}

void W_SP::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SP shareObj;
  typedef CShSP netObj;
#include "Modules/SP.h" 
#include "Modules/SP_P.h"
  ACK_ALARM
}

void W_SEL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SEL shareObj;
  typedef CShSEL netObj;
#include "Blocks/SEL.h" 
#include "Blocks/SEL_P.h"
  ACK_ALARM
}

void W_ROR::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_ROR shareObj;
  typedef CShROR netObj;
#include "Blocks/ROR.h" 
#include "Blocks/ROR_P.h"
  ACK_ALARM
}

void W_STARTSIGNAL::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_STARTSIGNAL shareObj;
  typedef CShSTARTSIGNAL netObj;
#include "Blocks/STARTSIGNAL.h" 
#include "Blocks/STARTSIGNAL_P.h"
  ACK_ALARM
}

void W_SP_SPDVOTE::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SP_SPDVOTE shareObj;
  typedef CShSP_SPDVOTE netObj;
#include "Blocks/SP_SPDVOTE.h" 
#include "Blocks/SP_SPDVOTE_P.h"
  ACK_ALARM
}

void W_SP_AI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SP_AI shareObj;
  typedef CShSP_AI netObj;
#include "Blocks/SP_AI.h" 
#include "Blocks/SP_AI_P.h"
  ACK_ALARM
}

void W_SP_AO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SP_AO shareObj;
  typedef CShSP_AO netObj;
#include "Blocks/SP_AO.h" 
#include "Blocks/SP_AO_P.h"
  ACK_ALARM
}

void W_SP_DI::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SP_DI shareObj;
  typedef CShSP_DI netObj;
#include "Blocks/SP_DI.h" 
#include "Blocks/SP_DI_P.h"
  ACK_ALARM
}

void W_SP_DO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SP_DO shareObj;
  typedef CShSP_DO netObj;
#include "Blocks/SP_DO.h" 
#include "Blocks/SP_DO_P.h"
  ACK_ALARM
}

void W_SP_SPEED::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_SP_SPEED shareObj;
  typedef CShSP_SPEED netObj;
#include "Blocks/SP_SPEED.h" 
#include "Blocks/SP_SPEED_P.h"
  ACK_ALARM
}

void W_NAND::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_NAND shareObj;
  typedef CShNAND netObj;
#include "Blocks/NAND.h" 
#include "Blocks/NAND_P.h"
  ACK_ALARM
}

void W_UIO::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_UIO shareObj;
  typedef CShUIO netObj;
#include "Modules/UIO.h" 
#include "Modules/UIO_P.h"
  ACK_ALARM
}

void W_FLAGARRAY::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_FLAGARRAY shareObj;
  typedef CShFLAGARRAY netObj;
#include "Blocks/FLAGARRAY.h" 
#include "Blocks/FLAGARRAY_P.h"
  ACK_ALARM
}

void W_TRIG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_TRIG shareObj;
  typedef CShTRIG netObj;
#include "Blocks/TRIG.h" 
#include "Blocks/TRIG_P.h"
  ACK_ALARM
}

void W_MIN::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_MIN shareObj;
  typedef CShMIN netObj;
#include "Blocks/MIN.h" 
#include "Blocks/MIN_P.h"
  ACK_ALARM
}

void W_NEG::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_NEG shareObj;
  typedef CShNEG netObj;
#include "Blocks/NEG.h" 
#include "Blocks/NEG_P.h"
  ACK_ALARM
}

void W_RATIOBIAS::BuildList(void)
{
#undef shareObj
#undef netObj
  typedef W_RATIOBIAS shareObj;
  typedef CShRATIOBIAS netObj;
#include "Blocks/RATIOBIAS.h" 
#include "Blocks/RATIOBIAS_P.h"
  ACK_ALARM
}

