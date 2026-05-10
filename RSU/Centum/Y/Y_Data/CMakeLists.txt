cmake_minimum_required (VERSION 3.10) 
set(CMAKE_FOLDER ".RSUs/Centum/Y")

set(PROJECT_NAME Y_Data)
include(../../../../global.cmake)

set(Model
	"./ReadParms.cpp"
	"./ReadParms.h"
	"./Signal.cpp"
	"./StdDef.h"
	"./Utils.cpp"
	"../../Include/Y_Class.h"
	"./Y_Data.cpp"
	"../../Include/Y_Data.h"
	"./Y_DR.cpp"
	"../../Include/Y_export.h"
	"../../Include/Y_Signal.h"
)
source_group("Model" FILES ${Model})	

set(Structs
	"./ALM-R.cpp"
	"./ANN_FUP.cpp"
	"./AREAIN.cpp"
	"./ARITH.cpp"
	"./AS_HML.cpp"
	"./ASTM1.cpp"
	"./ASTM2.cpp"
	"./AVE.cpp"
	"./AVE_C.cpp"
	"./AVE_M.cpp"
	"./BAND.cpp"
	"./BDA-C.cpp"
	"./BDA-L.cpp"
	"./BDSET_1C.cpp"
	"./BDSET_1L.cpp"
	"./BDSET_2C.cpp"
	"./BDSET_2L.cpp"
	"./BNOT.cpp"
	"./BSETU-2.cpp"
	"./BSI.cpp"
	"./CI.cpp"
	"./CO.cpp"
	"./CTS.cpp"
	"./DI.cpp"
	"./Discrete.cpp"
	"./DLAY.cpp"
	"./DLAY_C.cpp"
	"./DSET.cpp"
	"./DSET_PVI.cpp"
	"./DSW_16.cpp"
	"./DSW_16C.cpp"
	"./FUNC-VAR.cpp"
	"./HAS3C.cpp"
	"./INDST2.cpp"
	"./INDST3.cpp"
	"./INTEG.cpp"
	"./IO.cpp"
	"./LAG.cpp"
	"./LD.cpp"
	"./LDLAG.cpp"
	"./MC_NE.cpp"
	"./MC_NE_def.h"
	"./MLD.cpp"
	"./MLD_PVI.cpp"
	"./MLD_SW.cpp"
	"./OFFD.cpp"
	"./OND.cpp"
	"./ONOFF-G.cpp"
	"./ONOFF-GE.cpp"
	"./ONOFF.cpp"
	"./PBS10C.cpp"
	"./PBS5C.cpp"
	"./PG_L13.cpp"
	"./PTC.cpp"
	"./PVI.cpp"
	"./PVI_DV.cpp"
	"./RAMP.cpp"
	"./RATIO.cpp"
	"./RL.cpp"
	"./RS.cpp"
	"./SIO_X.cpp"
	"./SPLIT.cpp"
	"./SRSX_X.cpp"
	"./SS.cpp"
	"./SS_DUAL.cpp"
	"./SW_33.cpp"
	"./SW_91.cpp"
	"./TM.cpp"
	"./TOFF.cpp"
	"./TON.cpp"
	"./TPCFL.cpp"
	"./Var_Parm.h"
	"./VELLIM.cpp"
	"./WOUT.cpp"
	"./XCPL.cpp"
)
source_group("Model\\Структуры" FILES ${Structs})	

set(PID
	"./Control.cpp"
	"./Deadband.cpp"
	"./IMAN.cpp"
	"./Non_linear.cpp"
	"./PID.cpp"
	"./PIDWork.cpp"
	"./PV_trk.cpp"
	"./Rset_limit.cpp"
	"./SP_pushback.cpp"
)
source_group("Model\\Структуры\\PID" FILES ${PID})	

set(CALCU
	"./AnyRef.cpp"
	"./CALCU.cpp"
	"./CALCU_C.cpp"
	"./CALCU_trans.cpp"
	"./Functions.cpp"
	"./GET_DATA.h"
)
source_group("Model\\Структуры\\CALCU" FILES ${CALCU})	

set(CalcTypes
	"./CalcVar.cpp"
	"./CalcVar.h"
	"./CalcVarC.cpp"
)
source_group("Model\\Структуры\\CALCU\\CalcTypes" FILES ${CalcTypes})	

set(LC64_LC64_E
	"../../Include/LC64_AC.h"
	"./LC64_Act.cpp"
	"./LC64_Cond.cpp"
)
source_group("Model\\Структуры\\LC64_LC64_E" FILES ${LC64_LC64_E})	

set(LC64_E
	"./LC64-E.cpp"
)
source_group("Model\\Структуры\\LC64_LC64_E\\LC64-E" FILES ${LC64_E})	

set(Base
	"./LC64_Base.cpp"
)
source_group("Model\\Структуры\\LC64_LC64_E\\Base" FILES ${Base})	

set(LC64
	"./LC64.cpp"
)
source_group("Model\\Структуры\\LC64/LC64_E\\LC64" FILES ${LC64})	

set(ST16
	"./ST16.cpp"
	"../../Include/ST16_AC.h"
	"./ST16_Act.cpp"
	"./ST16_Cond.cpp"
)
source_group("Model\\Структуры\\ST16" FILES ${ST16})	

set(ST16_LC64_Common
	"./FlagsSupport.cpp"
	"./FlagsSupport.h"
	"./GetObj.cpp"
	"./GetObj.h"
	"../../Include/ST16_LC64_AC.h"
	"./ST16_LC64_Cond.cpp"
)
source_group("Model\\Структуры\\ST16_LC64_Common" FILES ${ST16_LC64_Common})	

set(Config_SFC
	"../../Include/_calcu.h"
	"./OPSBL.cpp"
	"./OPSFC.cpp"
	"./SFC_Base.cpp"
	"./SFCAS.cpp"
	"./SFCPB.cpp"
	"./SFCSW.cpp"
	"./UTAS.cpp"
	"./UTPB.cpp"
	"./UTPB_N.cpp"
	"./UTSW.cpp"
)
source_group("Model\\Структуры\\Настраиваемые_SFC" FILES ${Config_SFC})	

set(PI_HLD
	"./PI-HLD.cpp"
	"./PI_HLD_Work.cpp"
)
source_group("Model\\Структуры\\PI_HLD" FILES ${PI_HLD})

set(Prosafe
	"./ANLG_S.cpp"
	"./ANLGI.cpp"
	"./MOA.cpp"
	"./MOB_11.cpp"
	"./MOB_21.cpp"
	"./MOB_RS.cpp"
	"./OVR_B.cpp"
	"./PASSWD.cpp"
	"./S_GOV_B.cpp"
	"./VEL.cpp"
)
source_group("Model\\Структуры\\Prosafe-вовые" FILES ${Prosafe})	

set(BOPC_SIOS
	"./BOPCSYS.cpp"
	"./SR_FNCVR.cpp"
	"./SR_PID.cpp"
)
source_group("Model\\Структуры\\BOPC(SIOS)" FILES ${BOPC_SIOS})	

set(DI
	"./BOPCBOL.cpp"
	"./BOPCCHR.cpp"
	"./BOPCF32.cpp"
	"./BOPCF64.cpp"
	"./BOPCI16.cpp"
	"./BOPCI32.cpp"
	"./BOPCU16.cpp"
	"./BOPCU32.cpp"
)
source_group("Model\\Структуры\\BOPC(SIOS)\\DI" FILES ${DI})	

set(FF
	"./FF_AI.cpp"
	"./FF_AO.cpp"
	"./FF_DI.cpp"
	"./FF_DO.cpp"
	"./FF_IS.cpp"
	"./FF_MAI.cpp"
	"./FFSUM.cpp"
	"./FOUT.cpp"
)
source_group("Model\\Структуры\\FF" FILES ${FF})	

set(Processing
	"./ALARM.cpp"
	"../../Include/Alarms.h"
	"./IN.cpp"
	"./OUT.cpp"
)
source_group("Model\\Обработка" FILES ${Processing})	

set(...
	"./Y_Data_dll.cpp"
	"../../Include/Yokogawa/YPARM_PARM.h"
	"../../Include/Yokogawa/YPARM_CREATE.h"
	"../../Include/Yokogawa/yparm_init.h"
)
source_group("..." FILES ${...})	

set(Draw
	"./CallDrawHandler.cpp"
	"./CallDrawHandler.h"
)
source_group("Draw" FILES ${Draw})	

set(HardwareBP
	"./hardwarebp.cpp"
	"./hardwarebp.h"
)
source_group("HardwareBP" FILES ${HardwareBP})	

set(HPP_P
	"../../Include/Yokogawa/_UTAS_P.hpp"
	"../../Include/Yokogawa/_UTPB_N_P.hpp"
	"../../Include/Yokogawa/ALM-R_P.hpp"
	"../../Include/Yokogawa/ARITH_P.hpp"
	"../../Include/Yokogawa/AS_P.hpp"
	"../../Include/Yokogawa/AVE_P.hpp"
	"../../Include/Yokogawa/BAND_P.hpp"
	"../../Include/Yokogawa/BDA-C_P.hpp"
	"../../Include/Yokogawa/BDA-L_P.hpp"
	"../../Include/Yokogawa/BDSET_1C_P.hpp"
	"../../Include/Yokogawa/BDSET_1L_P.hpp"
	"../../Include/Yokogawa/BDSET_2C_P.hpp"
	"../../Include/Yokogawa/BDSET_2L_P.hpp"
	"../../Include/Yokogawa/BNOT_P.hpp"
	"../../Include/Yokogawa/BSETU-2_P.hpp"
	"../../Include/Yokogawa/CALCU_C_P.hpp"
	"../../Include/Yokogawa/CALCU_P.hpp"
	"../../Include/Yokogawa/CI_P.hpp"
	"../../Include/Yokogawa/CO_P.hpp"
	"../../Include/Yokogawa/CTS_P.hpp"
	"../../Include/Yokogawa/DLAY_P.hpp"
	"../../Include/Yokogawa/DSET_P.hpp"
	"../../Include/Yokogawa/DSW_16_P.hpp"
	"../../Include/Yokogawa/FF_DI_P.hpp"
	"../../Include/Yokogawa/FF_DO_P.hpp"
	"../../Include/Yokogawa/FF_MAI_P.hpp"
	"../../Include/Yokogawa/FFSUM_P.hpp"
	"../../Include/Yokogawa/FOUT_P.hpp"
	"../../Include/Yokogawa/FUNC_VAR_P.hpp"
	"../../Include/Yokogawa/HAS3C_P.hpp"
	"../../Include/Yokogawa/INDST2_P.hpp"
	"../../Include/Yokogawa/INDST3_P.hpp"
	"../../Include/Yokogawa/INTEG_P.hpp"
	"../../Include/Yokogawa/LAG_P.hpp"
	"../../Include/Yokogawa/LC64_P.hpp"
	"../../Include/Yokogawa/MC_P.hpp"
	"../../Include/Yokogawa/MLD_P.hpp"
	"../../Include/Yokogawa/MLD_PVI_P.hpp"
	"../../Include/Yokogawa/mld_sw_p.hpp"
	"../../Include/Yokogawa/OFFD_P.hpp"
	"../../Include/Yokogawa/OND_P.hpp"
	"../../Include/Yokogawa/onoff_p.hpp"
	"../../Include/Yokogawa/PBS10C_P.hpp"
	"../../Include/Yokogawa/PBS5C_P.hpp"
	"../../Include/Yokogawa/pg_l13_p.hpp"
	"../../Include/Yokogawa/PI-HLD_P.hpp"
	"../../Include/Yokogawa/PID_P.hpp"
	"../../Include/Yokogawa/PTC_P.hpp"
	"../../Include/Yokogawa/PVI_P.hpp"
	"../../Include/Yokogawa/RATIO_P.hpp"
	"../../Include/Yokogawa/rl_p.hpp"
	"../../Include/Yokogawa/RS_P.hpp"
	"../../Include/Yokogawa/S_ANLGI_P.hpp"
	"../../Include/Yokogawa/SIO_P.hpp"
	"../../Include/Yokogawa/SPLIT_P.hpp"
	"../../Include/Yokogawa/SR_PID_P.hpp"
	"../../Include/Yokogawa/SRS2_R_P.hpp"
	"../../Include/Yokogawa/SS_DUAL_P.hpp"
	"../../Include/Yokogawa/SS_P.hpp"
	"../../Include/Yokogawa/ST16_P.hpp"
	"../../Include/Yokogawa/SW_33_P.hpp"
	"../../Include/Yokogawa/SW_91_P.hpp"
	"../../Include/Yokogawa/TM_P.hpp"
	"../../Include/Yokogawa/TOFF_P.hpp"
	"../../Include/Yokogawa/TON_P.hpp"
	"../../Include/Yokogawa/TPCFL_P.hpp"
	"../../Include/Yokogawa/vellim_p.hpp"
)
source_group("HPP_P" FILES ${HPP_P})	

set(HPP
	"../../Include/Yokogawa/_AN.hpp"
	"../../Include/Yokogawa/_GS.hpp"
	"../../Include/Yokogawa/_OG.hpp"
	"../../Include/Yokogawa/_SFCAS.hpp"
	"../../Include/Yokogawa/_SFCPB.hpp"
	"../../Include/Yokogawa/_SFCSW.hpp"
	"../../Include/Yokogawa/_SW.hpp"
	"../../Include/Yokogawa/_UTAS.hpp"
	"../../Include/Yokogawa/_UTPB_N.hpp"
	"../../Include/Yokogawa/_WB.hpp"
	"../../Include/Yokogawa/_WW.hpp"
	"../../Include/Yokogawa/_Z.hpp"
	#"../../Include/Yokogawa/ADD.hpp"
	"../../Include/Yokogawa/ALM-R.hpp"
	#"../../Include/Yokogawa/AND.hpp"
	"../../Include/Yokogawa/ANN_FUP.hpp"
	"../../Include/Yokogawa/ARITH.hpp"
	"../../Include/Yokogawa/AS.hpp"
	"../../Include/Yokogawa/AVE.hpp"
	"../../Include/Yokogawa/AVE_C.hpp"
	"../../Include/Yokogawa/AVE_M.hpp"
	"../../Include/Yokogawa/BAND.hpp"
	"../../Include/Yokogawa/BDA-C.hpp"
	"../../Include/Yokogawa/BDA-L.hpp"
	"../../Include/Yokogawa/BDSET_1C.hpp"
	"../../Include/Yokogawa/BDSET_1L.hpp"
	"../../Include/Yokogawa/BDSET_2C.hpp"
	"../../Include/Yokogawa/BDSET_2L.hpp"
	"../../Include/Yokogawa/BNOT.hpp"
	"../../Include/Yokogawa/BSETU-2.hpp"
	"../../Include/Yokogawa/CALCU.hpp"
	"../../Include/Yokogawa/calcu_c.hpp"
	"../../Include/Yokogawa/CI.hpp"
	"../../Include/Yokogawa/CO.hpp"
	"../../Include/Yokogawa/CTS.hpp"
	#"../../Include/Yokogawa/DIV.hpp"
	"../../Include/Yokogawa/DLAY.hpp"
	"../../Include/Yokogawa/dset.hpp"
	"../../Include/Yokogawa/DSET_PVI.hpp"
	"../../Include/Yokogawa/DSW_16.hpp"
	"../../Include/Yokogawa/FF_DI.hpp"
	"../../Include/Yokogawa/FF_DO.hpp"
	"../../Include/Yokogawa/FF_MAI.hpp"
	"../../Include/Yokogawa/FFSUM.hpp"
	"../../Include/Yokogawa/FOUT.hpp"
	"../../Include/Yokogawa/func-var.hpp"
	"../../Include/Yokogawa/FUNC_VAR.hpp"
	#"../../Include/Yokogawa/GT.hpp"
	"../../Include/Yokogawa/HAS3C.hpp"
	"../../Include/Yokogawa/INDST2.hpp"
	"../../Include/Yokogawa/INDST2S.hpp"
	"../../Include/Yokogawa/INDST3.hpp"
	"../../Include/Yokogawa/INTEG.hpp"
	"../../Include/Yokogawa/LAG.hpp"
	"../../Include/Yokogawa/lc64.hpp"
	"../../Include/Yokogawa/LD.hpp"
	"../../Include/Yokogawa/MC.hpp"
	"../../Include/Yokogawa/MLD.hpp"
	"../../Include/Yokogawa/MLD_PVI.hpp"
	"../../Include/Yokogawa/MLD_SW.hpp"
	#"../../Include/Yokogawa/NOT.hpp"
	"../../Include/Yokogawa/OFFD.hpp"
	"../../Include/Yokogawa/OND.hpp"
	"../../Include/Yokogawa/ONOFF.hpp"
	#"../../Include/Yokogawa/OR.hpp"
	"../../Include/Yokogawa/PBS10C.hpp"
	"../../Include/Yokogawa/PBS5C.hpp"
	"../../Include/Yokogawa/PG_L13.hpp"
	"../../Include/Yokogawa/pg_l13.hpp"
	"../../Include/Yokogawa/PI-HLD.hpp"
	"../../Include/Yokogawa/PID-STC.hpp"
	"../../Include/Yokogawa/PID.hpp"
	"../../Include/Yokogawa/PTC.hpp"
	"../../Include/Yokogawa/PVI.hpp"
	"../../Include/Yokogawa/RATIO.hpp"
	"../../Include/Yokogawa/rl.hpp"
	"../../Include/Yokogawa/RS.hpp"
	"../../Include/Yokogawa/SIO.hpp"
	"../../Include/Yokogawa/SPLIT.hpp"
	"../../Include/Yokogawa/SR_PID.hpp"
	"../../Include/Yokogawa/SRS2-R.hpp"
	"../../Include/Yokogawa/SRS2_R.hpp"
	"../../Include/Yokogawa/SS-DUAL.hpp"
	"../../Include/Yokogawa/SS.hpp"
	"../../Include/Yokogawa/SS_DUAL.hpp"
	"../../Include/Yokogawa/ST16.hpp"
	"../../Include/Yokogawa/SW.hpp"
	"../../Include/Yokogawa/SW_33.hpp"
	"../../Include/Yokogawa/SW_91.hpp"
	"../../Include/Yokogawa/TM.hpp"
	"../../Include/Yokogawa/TOFF.hpp"
	"../../Include/Yokogawa/TON.hpp"
	"../../Include/Yokogawa/TPCFL.hpp"
	"../../Include/Yokogawa/VELLIM.hpp"
	"../../Include/Yokogawa/WOUT.hpp"
	#"../../Include/Yokogawa/YPARM_CREATE.hpp"
)
source_group("HPP" FILES ${HPP})	

set(ConfigFromProp
	"./GlobalSettings.cpp"
	"./GlobalSettings.h"
)
source_group("НастройкиИзProp-ов" FILES ${ConfigFromProp})	

set(ALL_FILESS
	${Model}
	${Structs}
	${PID}
	${CalcTypes}
	${LC64_LC64_E}
	${LC64_E}
	${Base}
	${LC64}
	${ST16}
	${ST16_LC64_Common}
	${Config_SFC}
	${PI_HLD}
	${Prosafe}
	${BOPC_SIOS}
	${DI}
	${FF}
	${Processing}
	${...}
	${Draw}
	${HardwareBP}
	${HPP_P}
	${HPP}
	${ConfigFromProp}
)


set(ALL_FILES
	${...}
)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_compile_definitions(Y_DATA_EXPORTS)

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
#add_dependencies (${PROJECT_NAME} )

target_link_libraries(${PROJECT_NAME} cutf crosslib ConnectY CommonY MemoryServiceY YBridge2SysParam)

set(PROJECT_INCLUDES
    #"${CMAKE_CURRENT_SOURCE_DIR}/"        
	"${CMAKE_CURRENT_SOURCE_DIR}/../../Include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../../../system/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../common/include"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_INCLUDES}
    ${COMMON_INCLUDES}
)

