// #include "StdAfx.h"
// #include "Scripts127.h"
// #include <algorithm>

// SParamValueH shape_get_data_value(CHTML& html, CTag* tag, char * tagname, char * prop)
// {
// 	std::string a = Alias(html, tag, tagname);
// 	std::string b = Alias(html, tag, prop);

// 	std::string c = a + b;

// 	if (tag->vals.find(c) == tag->vals.end())
// 	{
// 		tag->vals[c] = FindExperion( html, a.c_str(), b.c_str() );
// 	}
// 	return tag->vals[c];
// }
// long shape_get_data_value_integer(CHTML& html, CTag* tag, char * tagname, char * prop)
// {
// 	SParamValueH v = shape_get_data_value(html, tag, tagname, prop);
// 	void* val = html.pipe.ParamValue_H(v);
// 	if (val && v.def)
// 	{
// 		switch(v.def->eVal)
// 		{
// 		case enumValueInt: return *(int*)val;
// 		case enumValueChr: return *(char*)val;
// 		case enumValueBol: return *(bool*)val;
// 		case enumValueI64: return *(long*)val;
// 		}
// 	}
// 	return (long)0xf0000000f000f0ff;
// }
// double shape_get_data_value_double(CHTML& html, CTag* tag, char * tagname, char * prop)
// {
// 	SParamValueH v = shape_get_data_value(html, tag, tagname, prop);
// 	void* val = html.pipe.ParamValue_H(v);
// 	if (val && v.def)
// 	{
// 		switch(v.def->eVal)
// 		{
// 		case enumValueDbl: return *(double*)val;
// 		default:
// 			KKK();
// 			break;
// 		}
// 	}
// 	return NaN;
// }
// char * shape_get_data_value_string(CHTML& html, CTag* tag, char * tagname, char * prop)
// {
//     if(tag)
//     {
// 	SParamValueH v = shape_get_data_value(html, tag, tagname, prop);
// 	void* val = html.pipe.ParamValue_H(v);
// 	if (val && v.def && (v.def->eVal == enumValueStr || v.def->eVal == enumValueChr)) return (char*)val;
//     }
// 	return 0;
// }
// long get_value_integer(CTag* obj, CHTML& html)
// {
// 	void* val = html.pipe.ParamValue_H(obj->mVal);
// 	if (val && obj->mVal.def)
// 	{
// 		switch(obj->mVal.def->eVal)
// 		{
// 		case enumValueInt: return *(int*)val;
// 		case enumValueChr: return *(char*)val;
// 		case enumValueBol: return *(bool*)val;
// 		case enumValueI64: return *(long*)val;
// 		case enumValueStr:
// 			char * str = (char*) val;
// 			for (int i = 0; i < 3; i++)
// 			{
// 				char prop[32]; sprintf_s(prop, "FLAGA.STATETEXT[%d]", i);
// 				char * text = shape_get_data_value_string( html,  obj, "tagname", prop);
// 				if (text && !lstrcmpi(str, text)) return i;
// 			}
// 			break;
// 		}
// 	}
// 	return (long)0xf0000000f000f0ff;
// }
// double get_value_double(CTag* obj, CHTML& html)
// {
// 	void* val = html.pipe.ParamValue_H(obj->mVal);
// 	if (val && obj->mVal.def && obj->mVal.def->eVal == enumValueDbl) return *(double*)val;
// 	return -1;
// }
// const char * get_alias_string(CHTML& html, CTag* obj, char * name)
// {
// 	return Alias(html, obj, name);
// }
// int get_alias_int(CHTML& html, CTag* obj, char * name)
// {
// 	return atoi(Alias(html, obj, name));
// }
// double get_alias_double(CHTML& html, CTag* obj, char * name)
// {
// 	return atof(Alias(html, obj, name));
// }
// void set_class(CHTML& html, CTag* obj, EStyleClass _class)
// {
// 	if ((char)_class == 0 || !obj) return;
// 	html.Style(obj->m_nStyle).mClass = _class;
// }

// std::map<std::string, EStyleClass> create_style_map()
// {
// 	std::map<std::string, EStyleClass> result;
// //	result["BOXLOCREM_ENUM0"]               = clsBoxLocRem_Enum0;
// //	result["BOXLOCREM_ENUM1"]               = clsBoxLocRem_Enum1;
// //	result["DISCREPANCY_ALARM"]             = clsDiscrepancy_Alarm;
// //	result["DISCREPANCY_NORMAL"]            = clsDiscrepancy_Normal;
// //	result["ENCLAV_ACTUADOR_OFF"]           = clsEnclav_Actuador_Off;
// //	result["ENCLAV_ACTUADOR_ON"]            = clsEnclav_Actuador_On;
// //	result["ENCLAV_INITANL_OFF"]            = clsEnclav_InitAnl_Off;
// //	result["ENCLAV_INITANL_ON"]             = clsEnclav_InitAnl_On;
// //	result["ENCLAV_INITDIG_OFF"]            = clsEnclav_InitDig_Off;
// //	result["ENCLAV_INITDIG_ON"]             = clsEnclav_InitDig_On;
// //	result["ENCLAV_TXTBYPASS_OFF"]          = clsEnclav_TxtBypass_Off;
// //	result["ENCLAV_TXTBYPASS_ON"]           = clsEnclav_TxtBypass_On;
// //	result["ENCLAV_TXTBYPASS_ON_AUTO"]      = clsEnclav_TxtBypass_On_Auto;
// //	result["ENCLAV_TXTDESCRIPTION"]         = clsEnclav_TxtDescription;
// //	result["ENCLAV_TXTNAME_ACTIVE"]         = clsEnclav_TxtName_Active;
// //	result["ENCLAV_TXTNAME_NORMAL"]         = clsEnclav_TxtName_Normal;
// //	result["LINEXECSTATE_OFFSCAN_INACTIVE"] = clsLinExecState_OffScan_InActive;
// //	result["LINEXECSTATE_ONSCAN_ACTIVE"]    = clsLinExecState_OnScan_Active;
// //	result["OVLBODY_MOTOR_BAD"]             = clsOvlBody_Motor_Bad;
// //	result["OVLBODY_MOTOR_RUN"]             = clsOvlBody_Motor_Run;
// //	result["OVLBODY_MOTOR_STOP"]            = clsOvlBody_Motor_Stop;
// //	result["OVLBODY_PUMP_BAD"]              = clsOvlBody_Pump_Bad;
// //	result["OVLBODY_PUMP_OTHER"]            = clsOvlBody_Pump_Other;
// //	result["OVLBODY_PUMP_RUN"]              = clsOvlBody_Pump_Run;
// 	result["OVLBODY_PUMP_STOP"]             = clsOvlBody_Pump_Stop;
// //	result["PLGBODY_MOTOR_BAD"]             = clsPlgBody_Motor_Bad;
// //	result["PLGBODY_MOTOR_RUN"]             = clsPlgBody_Motor_Run;
// //	result["PLGBODY_MOTOR_STOP"]            = clsPlgBody_Motor_Stop;
// //	result["PLGBODY_PUMP_BAD"]              = clsPlgBody_Pump_Bad;
// //	result["PLGBODY_PUMP_OTHER"]            = clsPlgBody_Pump_Other;
// //	result["PLGBODY_PUMP_RUN"]              = clsPlgBody_Pump_Run;
// //	result["PLGBODY_PUMP_STOP"]             = clsPlgBody_Pump_Stop;
// //	result["PLGIN_ONOFFVALVE_BAD"]          = clsPlgIn_OnOffValve_Bad;
// 	result["PLGIN_ONOFFVALVE_CLOSE"]        = clsPlgIn_OnOffValve_Close;
// //	result["PLGIN_ONOFFVALVE_INBET"]        = clsPlgIn_OnOffValve_InBet;
// //	result["PLGIN_ONOFFVALVE_OPEN"]         = clsPlgIn_OnOffValve_Open;
// 	result["PLGIN_REGVALVE_CLOSE"]          = clsPlgIn_RegValve_Close;
// //	result["PLGIN_REGVALVE_OPEN"]           = clsPlgIn_RegValve_Open;
// //	result["PLGIN_VALVECNTLSTATUS_ALARM"]   = clsPlgIn_ValveCntlStatus_Alarm;
// //	result["PLGIN_VALVECNTLSTATUS_CLOSE"]   = clsPlgIn_ValveCntlStatus_Close;
// //	result["PLGIN_VALVECNTLSTATUS_OPEN"]    = clsPlgIn_ValveCntlStatus_Open;
// //	result["PLGIN_VALVEONOFFSTATUS_ALARM"]  = clsPlgIn_ValveOnOffStatus_Alarm;
// //	result["PLGIN_VALVEONOFFSTATUS_CLOSE"]  = clsPlgIn_ValveOnOffStatus_Close;
// //	result["PLGIN_VALVEONOFFSTATUS_OPEN"]   = clsPlgIn_ValveOnOffStatus_Open;
// //	result["PLGINTERLOCK_INTERLOCK_OFF"]    = clsPlgInterlock_Interlock_OFF;
// //	result["PLGINTERLOCK_INTERLOCK_ON"]     = clsPlgInterlock_Interlock_ON;
// //	result["PLGOUT_ONOFFVALVE_BAD"]         = clsPlgOut_OnOffValve_Bad;
// 	result["PLGOUT_ONOFFVALVE_CLOSE"]       = clsPlgOut_OnOffValve_Close;
// //	result["PLGOUT_ONOFFVALVE_INBET"]       = clsPlgOut_OnOffValve_InBet;
// //	result["PLGOUT_ONOFFVALVE_OPEN"]        = clsPlgOut_OnOffValve_Open;
// 	result["PLGOUT_REGVALVE_CLOSE"]         = clsPlgOut_RegValve_Close;
// //	result["PLGOUT_REGVALVE_OPEN"]          = clsPlgOut_RegValve_Open;
// //	result["PLGOUT_VALVECNTLSTATUS_ALARM"]  = clsPlgOut_ValveCntlStatus_Alarm;
// //	result["PLGOUT_VALVECNTLSTATUS_CLOSE"]  = clsPlgOut_ValveCntlStatus_Close;
// //	result["PLGOUT_VALVECNTLSTATUS_OPEN"]   = clsPlgOut_ValveCntlStatus_Open;
// //	result["PLGOUT_VALVEONOFFSTATUS_ALARM"] = clsPlgOut_ValveOnOffStatus_Alarm;
// //	result["PLGOUT_VALVEONOFFSTATUS_CLOSE"] = clsPlgOut_ValveOnOffStatus_Close;
// //	result["PLGOUT_VALVEONOFFSTATUS_OPEN"]  = clsPlgOut_ValveOnOffStatus_Open;
// //	result["RECBODY_MOTOR_BAD"]             = clsRecBody_Motor_Bad;
// //	result["RECBODY_MOTOR_RUN"]             = clsRecBody_Motor_Run;
// //	result["RECBODY_MOTOR_STOP"]            = clsRecBody_Motor_Stop;
// //	result["RECBODY_PUMP_BAD"]              = clsRecBody_Pump_Bad;
// //	result["RECBODY_PUMP_OTHER"]            = clsRecBody_Pump_Other;
// //	result["RECBODY_PUMP_RUN"]              = clsRecBody_Pump_Run;
// //	result["RECBODY_PUMP_STOP"]             = clsRecBody_Pump_Stop;
// //	result["RECFRAME_DIGITALSTATE_CYAN"]    = clsRecFrame_DigitalState_CYAN;
// //	result["RECFRAME_DIGITALSTATE_GRAY"]    = clsRecFrame_DigitalState_GRAY;
// //	result["RECFRAME_DIGITALSTATE_GREEN"]   = clsRecFrame_DigitalState_GREEN;
// //	result["RECFRAME_DIGITALSTATE_RED"]     = clsRecFrame_DigitalState_RED;
// //	result["RECPV_DIGITALSTATE_GRAY"]       = clsRecPV_DigitalState_GRAY;
// //	result["RECPV_DIGITALSTATE_GREEN"]      = clsRecPV_DigitalState_GREEN;
// //	result["RECPV_DIGITALSTATE_RED"]        = clsRecPV_DigitalState_RED;
// //	result["RECPV_ENUM0"]                   = clsRecPV_Enum0;
// //	result["RECPV_ENUM1"]                   = clsRecPV_Enum1;
// //	result["RECPV_ENUM2"]                   = clsRecPV_Enum2;
// //	result["RECPV_ENUM3"]                   = clsRecPV_Enum3;
// //	result["RECPV_ENUM4"]                   = clsRecPV_Enum4;
// //	result["RECPV_ENUM5"]                   = clsRecPV_Enum5;
// //	result["RECPV_ENUM6"]                   = clsRecPV_Enum6;
// //	result["RECPV_ENUM7"]                   = clsRecPV_Enum7;
// //	result["RECPV_VLVCLOSE"]                = clsRecPV_VlvClose;
// //	result["RECPV_VLVOPEN"]                 = clsRecPV_VlvOpen;
// //	result["SCCONALPPV_ENUM0"]              = clsScConAlpPV_Enum0;
// //	result["SCCONALPPV_ENUM1"]              = clsScConAlpPV_Enum1;
// //	result["SCCONALPPV_ENUM2"]              = clsScConAlpPV_Enum2;
// //	result["SCCONALPPV_ENUM3"]              = clsScConAlpPV_Enum3;
// //	result["SCCONALPPV_ENUM4"]              = clsScConAlpPV_Enum4;
// //	result["SCCONALPPV_ENUM5"]              = clsScConAlpPV_Enum5;
// //	result["SCCONALPPV_ENUM6"]              = clsScConAlpPV_Enum6;
// //	result["SCCONALPPV_ENUM7"]              = clsScConAlpPV_Enum7;
// //	result["TXT_ABSORBERSTATUS_GREEN"]      = clsTxt_AbsorberStatus_Green;
// //	result["TXT_ABSORBERSTATUS_GREY"]       = clsTxt_AbsorberStatus_Grey;
// //	result["TXTBAD_BAD"]                    = clsTxtBad_Bad;
// //	result["TXTBAD_NOTBAD"]                 = clsTxtBad_NotBad;
// 	result["TXTERROR_NOERROR"]              = clsTxtError_NoError;
// //	result["TXTMODE_IN_NMODE"]              = clsTxtMode_in_NMode;
// //	result["TXTMODE_NOT_IN_NMODE"]          = clsTxtMode_not_in_NMode;
// //	result["WDGC_DAMPERVALVE_GRAY"]         = clsWdgC_DamperValve_GRAY;
// //	result["WDGC_DAMPERVALVE_GREEN"]        = clsWdgC_DamperValve_GREEN;
// //	result["WDGC_DAMPERVALVE_OTHER"]        = clsWdgC_DamperValve_OTHER;
// //	result["WDGC_DAMPERVALVE_RED"]          = clsWdgC_DamperValve_RED;
// //	result["WDGC_ONOFFVALVE_BAD"]           = clsWdgC_OnOffValve_Bad;
// //	result["WDGC_ONOFFVALVE_CLOSE"]         = clsWdgC_OnOffValve_Close;
// //	result["WDGC_ONOFFVALVE_INBET"]         = clsWdgC_OnOffValve_InBet;
// //	result["WDGC_ONOFFVALVE_OPEN"]          = clsWdgC_OnOffValve_Open;
// //	result["WDGREDTAG_OFF"]                 = clsWdgRedTag_Off;
// //	result["WDGREDTAG_ON"]                  = clsWdgRedTag_On;
// //	result["WDGC_REGVALVE_CLOSE"]           = clsWdgC_RegValve_Close;
// //	result["WDGC_REGVALVE_OPEN"]            = clsWdgC_RegValve_Open;
// 	result["RECPV_LK2B_NORMAL"]             = clsRecPV_LK2B_Normal;
// 	result["TXTCTEXT_LK2B_NORMAL"]          = clsTxtCText_LK2B_Normal;
// 	result["RECPV_LK2B_FAIL"]               = clsRecPV_LK2B_Fail;
// 	result["TXTCTEXT_LK2B_FAIL"]            = clsTxtCText_LK2B_Fail;

// 	result["PLGINTERLOCK_ACTIVE"]           = clsPlgInterLock_Active;
// 	result["PLGINTERLOCK_INACTIVE"]         = clsPlgInterLock_Inactive;
// 	result["LINBYPASS_INACTIVE"]            = clsLinBypass_Inactive;
// 	result["LINBYPASS_ACTIVE"]              = clsLinBypass_Active;
// 	result["LINOUTBYPASS_INACTIVE"]         = clsLinOutBypass_Inactive;
// 	result["LINOUTBYPASS_ACTIVE"]           = clsLinOutBypass_Active;
// 	result["RECPV_CAUSE_FAIL"]              = clsRecPV_Cause_Fail;
// 	result["TXTCTEXT_CAUSE_FAIL"]           = clsTxtCText_Cause_Fail;

// 	result["RECPV_MO_NORMAL"]               = clsRecPV_MO_Normal;
// 	result["RECPV_MO_FAIL"]                 = clsRecPV_MO_Fail;
// 	return result;
// }
// std::map<std::string, EStyleClass> styleClasses = create_style_map();

// EStyleClass find_class(char * classname)
// {
// 	std::string cls(classname);
// 	std::transform(cls.begin(), cls.end(),cls.begin(), ::toupper);
// 	if (
// 		   cls == "RECPV_CP_STYLEFORPV-268373761"
// 		|| cls == "TXTCTEXT_CP_STYLEFORPV-268373761"
// 		|| cls == "TXTCTEXT_CP_STYLEFORPV3"
// 		|| cls == "TXTCTEXT_CP_STYLEFORPV4"
// 		|| cls == "TXTCTEXT_CP_STYLEFORPV5"
// 		|| cls == "RECPV_CP_STYLEFORPV3"
// 		|| cls == "RECPV_CP_STYLEFORPV4"
// 		|| cls == "RECPV_CP_STYLEFORPV5"
//     )
// 		return (EStyleClass)0; // ‡ÁÓ·‡Ú¸Òˇ ÔÓ˜ÂÏÛ Á‡Ô‡¯Ë‚‡˛ÚÒˇ Ú‡ÍËÂ ÒÚËÎË Ë ˜ÚÓ Ò ˝ÚËÏ ‰ÂÎ‡Ú¸
// 	if (styleClasses.find(cls.c_str()) == styleClasses.end())
// 		;//__debugbreak();
// 	else
// 		return styleClasses[cls.c_str()];
// 	return (EStyleClass)0;
// }

// bool ends_with(char* haystack, char* needle)
// {
// 	int p = (int)strlen(haystack) - (int)strlen(needle);
// 	return ((p >= 0) && (_strnicmp(haystack + p, needle, strlen(needle)) == 0));
// }
// bool shape_contains_object(CHTML& html, CTag* object, char * name)
// {
// 	INT_PTR nCount = object->GetSize();
// 	CTag** obj = object->GetData();
// 	for ( INT_PTR n = 0; n < nCount; n++ )
// 	{
// 		CStyle& s = html.Style(obj[n]->m_nStyle);
// 		char* id = s.NameID(html);
// 		if (ends_with(id, name) || (obj[n]->GetSize() > 0 && shape_contains_object(html, obj[n], name)))
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
// }
// bool shape_contains_property(CHTML& html, CTag* obj, char * name)
// {
// 	const char * get = Alias(html, obj, name);
// 	return (get != name);
// }
// bool shape_property_not_empty(CHTML& html, CTag* obj, char * name)
// {
// 	const char * get = Alias(html, obj, name);
// 	return (get != name && strlen(get) > 0);
// }
// CTag* shape_get_object(CHTML& html, CTag* object, char * name)
// {
// 	INT_PTR nCount = object->GetSize();
// 	CTag** obj = object->GetData();
// 	for ( INT_PTR n = 0; n < nCount; n++ )
// 	{
// 		CStyle& s = html.Style(obj[n]->m_nStyle);
// 		char* id = s.NameID(html);
// 		if (ends_with(id, name))
// 		{
// 			return obj[n];
// 		}
// 		else if (obj[n]->GetSize() > 0)
// 		{
// 			CTag * inn = shape_get_object(html, obj[n], name);
// 			if (inn) return inn;
// 		}
// 	}
// 	return 0;
// }

// void VBSCRIPT_CDA_ExecState_Alp_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim oShape,oExStObj
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    set oExStObj=oShape.objects("LinExecState")
//    if oSource.DataValue("tagname.cp_execstate")=1 then
//       oExStObj.styleclass="LinExecState_OnScan_Active"
//    else
// 	  oExStObj.styleclass="LinExecState_OffScan_InActive"
//    end if
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	CTag* oExtObj = shape_get_object(html, oShape, "LinExecState");
// 	if (shape_get_data_value_integer(html, oSource, "tagname", "cp_execstate") == 1)
// 		set_class(html, oExtObj, find_class("LinExecState_OnScan_Active"));
// 	else
// 		set_class(html, oExtObj, find_class("LinExecState_OffScan_InActive"));
// 	/* DONE */
// }
// void VBSCRIPT_CDA_RegCtlValves_RedTag_BadCtlFl_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim iRedTag, iBadCtlFl, oShape
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    if oShape.getcustomproperty("Parameter","cp_redtag")<>"" then
//    		iRedTag=oSource.DataValue("tagname.cp_redtag")
//    		if iRedTag=1 then
//       		oShape.objects("WdgRedTag").styleclass="WdgRedTag_On"
//    		else
//       		oShape.objects("WdgRedTag").styleclass="WdgRedTag_Off"
//    		end if
//    end if
//    if oShape.getcustomproperty("Parameter","cp_badctlfl")<>"" then
//    		iBadCtlFl=oSource.DataValue("tagname.cp_badctlfl")
//    		if iBadCtlFl=1 then
//       		oShape.objects("TxtBad").styleclass="TxtBad_Bad"
//    		else
//       		oShape.objects("TxtBad").styleclass="TxtBad_NotBad"
//    		end if
//    end if
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	if (shape_property_not_empty(html, oShape, "cp_redtag"))
// 	{
// 		long iRedTag = shape_get_data_value_integer(html, oSource, "tagname", "cp_redtag");
// 		if (iRedTag == 1)
// 			set_class(html, shape_get_object(html, oShape, "WdgRedTag"), find_class("WdgRedTag_On"));
// 		else
// 			set_class(html, shape_get_object(html, oShape, "WdgRedTag"), find_class("WdgRedTag_Off"));
// 	}
// 	if (shape_property_not_empty(html, oShape, "cp_badctlfl"))
// 	{
// 		long iBadCtlFl = shape_get_data_value_integer(html, oSource, "tagname", "cp_badctlfl");
// 		if (iBadCtlFl == 1)
// 			set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_Bad"));
// 		else
// 			set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_NotBad"));
// 	}
// 	/* DONE */
// }
// void VBSCRIPT_CDA_Graph_AlpError_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim oShape
//    On error resume next
//    set oShape=oSource.parentnode.parentnode
//    if (oSource.quality and iQualityGood)=iQualityGood then
//       oShape.objects("TxtError").styleclass="TxtError_NoError"
//    else
//       oShape.objects("TxtError").styleclass="TxtError_InError"
//    end if
// 	''' ¬Ó ‚ÒÂı ÒÎÛ˜‡ˇı Ò˜ËÚ‡ÂÏ, ◊ÚÓ quality = good
// */
// 	CTag * oShape = oSource->m_pPar->m_pPar;
// 	set_class(html, shape_get_object(html, oShape, "TxtError"), find_class("TxtError_NoError"));
// }
// void VBSCRIPT_CDA_RegCntlValve_AlpVal_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim rVal,rClosed,oShape, sRange
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    rVal=oSource.value
//    rClosed=csng(oShape.getcustomproperty("Num","ClosedValue"))
//    sRange = lcase(page.GetStyleClassProperty("UseExtended_RegValve_range","key"))
//    if sRange <>"yes" then
// 	   if rVal>100 then
// 	      rVal=100
// 	   elseif rVal<0 then
// 	      rVal=0
// 	   end if
//    end if
//    if oShape.objects("TxtOP") is nothing then
// 	  oShape.objects("IndVal").value=rVal
//    else
// 	  oShape.objects("TxtOP").innertext=round(rVal) & "%"
//    end if
//    if rVal > rClosed then
// 	  index=oShape.getCustomProperty("Style","cp_styleForOpen" )
//    else
// 	  index=oShape.getCustomProperty("Style","cp_styleForClose" )
//    end if
//    if typename(oShape.objects("PlgIn")) <> "Nothing" then oShape.objects("PlgIn").styleclass="PlgIn_" & index
//    if typename(oShape.objects("PlgOut")) <> "Nothing" then oShape.objects("PlgOut").styleclass="PlgOut_" & index
//    if err.number<>0 and window.external.environment = 0 then window.external.LogMessage " CDA_RegCntlValve_AlpVal_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	double rVal = get_value_double(oSource, html);
// 	double rClosed = get_alias_double(html, oShape, "ClosedValue");
// 	if (!shape_contains_object(html, oShape, "TxtOP"))
// 		AAA();
// 	else
// 		sprintf_s(shape_get_object(html, oShape, "TxtOP")->m_szScriptValue, "%.0f%%", rVal);
	
// 	const char * index;
// 	if (rVal > rClosed)
// 		index = get_alias_string(html, oShape, "cp_styleForOpen");
// 	else
// 		index = get_alias_string(html, oShape, "cp_styleForClose");

// 	char classname[64]; sprintf_s(classname, "PlgIn_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 	sprintf_s(classname, "PlgOut_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_CDA_Devctl_Hialm_RedTag_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim iDevctl_HialmT, iRedTag, oShape, oDevctlAlm
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    if oShape.getcustomproperty("Parameter","cp_hialm.type")<>"" then iDevctl_HialmT=oSource.DataValue("tagname.cp_hialm.type")
//    set oDevctlAlm=oShape.objects("TxtHiAlmTypeDEVCTLA")
//    select case iDevctl_HialmT
// 	  case 0 oDevctlAlm.innertext=""
// 	  case 1 oDevctlAlm.innertext="BAD"
// 	  case 2 oDevctlAlm.innertext="OFF"
// 	  case 3 oDevctlAlm.innertext="CMD"
// 	  case 4 oDevctlAlm.innertext="FL"
// 	  case 5 oDevctlAlm.innertext="CH"
// 	  case 6 oDevctlAlm.innertext="SAF"
// 	  case 7 oDevctlAlm.innertext="OV0"
// 	  case 8 oDevctlAlm.innertext="OV1"
// 	  case 9 oDevctlAlm.innertext="OV2"
//    end select
//    if oShape.getcustomproperty("Parameter","cp_redtag")<>"" then
//    		iRedTag=oSource.DataValue("tagname.cp_redtag")
//    		if iRedTag=1 then
//       		oShape.objects("WdgRedTag").styleclass="WdgRedTag_On"
//    		else
//       		oShape.objects("WdgRedTag").styleclass="WdgRedTag_Off"
//    		end if
//    end if
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;

// 	long iDevctl_HialmT;
// 	if (shape_property_not_empty(html, oSource, "cp_hialm.type"))
// 		iDevctl_HialmT = shape_get_data_value_integer(html, oSource, "tagname", "cp_hialm.type");
// 	CTag* oDevctlAlm = shape_get_object(html, oShape, "TxtHiAlmTypeDEVCTLA");
// 	switch(iDevctl_HialmT)
// 	{
// 	case 0: sprintf_s(oDevctlAlm->m_szScriptValue, " "); break;
// 	case 1: sprintf_s(oDevctlAlm->m_szScriptValue, "BAD"); break;
// 	case 2: sprintf_s(oDevctlAlm->m_szScriptValue, "OFF"); break;
// 	case 3: sprintf_s(oDevctlAlm->m_szScriptValue, "CMD"); break;
// 	case 4: sprintf_s(oDevctlAlm->m_szScriptValue, "FL"); break;
// 	case 5: sprintf_s(oDevctlAlm->m_szScriptValue, "CH"); break;
// 	case 6: sprintf_s(oDevctlAlm->m_szScriptValue, "SAF"); break;
// 	case 7: sprintf_s(oDevctlAlm->m_szScriptValue, "OV0"); break;
// 	case 8: sprintf_s(oDevctlAlm->m_szScriptValue, "OV1"); break;
// 	case 9: sprintf_s(oDevctlAlm->m_szScriptValue, "OV2"); break;
// 	}
// 	if (shape_property_not_empty(html, oSource, "cp_hialm.type"))
// 	{
// 		long iRedTag = shape_get_data_value_integer(html, oSource, "tagname", "cp_redtag");
// 		if (iRedTag == 1)
// 			set_class(html, shape_get_object(html, oShape, "WdgRedTag"), find_class("WdgRedTag_On"));
// 		else
// 			set_class(html, shape_get_object(html, oShape, "WdgRedTag"), find_class("WdgRedTag_Off"));
// 	}
// 	/* DONE */
// }
// void VBSCRIPT_CDA_Device3States_AlpGPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim igPV,oShape
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    igPV=oSource.value
//    index=oShape.getCustomProperty("Style","cp_styleForPV" & igPV)
//    if typename(oShape.objects("OvlBody")) <> "Nothing" then oShape.objects("OvlBody").styleclass="OvlBody_" & index
//    if typename(oShape.objects("PlgBody")) <> "Nothing" then oShape.objects("PlgBody").styleclass="PlgBody_" & index
//    if typename(oShape.objects("RecBody")) <> "Nothing" then oShape.objects("RecBody").styleclass="RecBody_" & index
//    if igPV=cint(oShape.getcustomproperty("Num","cp_BadPVState")) then
//       oShape.objects("TxtBad").styleclass="TxtBad_Bad"
//    else
//       oShape.objects("TxtBad").styleclass="TxtBad_NotBad"
//    end if
//    if err.number<>0 and window.external.environment = 0 then window.external.LogMessage " CDA_Device3States_AlpGPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long igPV = get_value_integer(oSource, html);
// 	if (igPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", igPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "OvlBody_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "OvlBody"), find_class(classname));
// 	sprintf_s(classname, "PlgBody_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgBody"), find_class(classname));
// 	sprintf_s(classname, "RecBody_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecBody"), find_class(classname));

// 	if (igPV == get_alias_int(html, oShape, "cp_BadPVState"))
// 		set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_Bad"));
// 	else
// 		set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_NotBad"));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_Khabarovsk_Discrepancy_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim oShape
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    iPV=oSource.value
//    index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
//    oShape.objects("RecAlarm").styleclass="Discrepancy_" & index
// 	if err.number<>0 then window.external.LogMessage "Scd_Khabarovsk_Discrepancy_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "Discrepancy_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecAlarm"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_CDA_Damper_AlpVal_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim rVal,rClosed,oShape, sRange
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    rVal=oSource.value
//    rClosed=csng(oShape.getcustomproperty("Num","ClosedValue"))
//    sRange = lcase(page.GetStyleClassProperty("UseExtended_RegValve_range","key"))
//    if sRange <>"yes" then
// 	   if rVal>100 then
// 	      rVal=100
// 	   elseif rVal<0 then
// 	      rVal=0
// 	   end if
//    end if
//    if oShape.objects("TxtOP") is nothing then
// 	  oShape.objects("IndVal").value=rVal
//    else
// 	  oShape.objects("TxtOP").innertext=round(rVal) & "%"
//    end if
//    if rVal > rClosed then
// 	  index=oShape.getCustomProperty("Style","cp_styleForOpen" )
//    else
// 	  index=oShape.getCustomProperty("Style","cp_styleForClose" )
//    end if

//    if typename(oShape.objects("WdgC")) <> "Nothing" then oShape.objects("WdgC").styleclass="WdgC_" & index
   
//    if err.number<>0 and window.external.environment = 0 then window.external.LogMessage " CDA_Damper_AlpVal_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	double rVal = get_value_double(oSource, html);
// 	double rClosed = get_alias_double(html, oShape, "ClosedValue");
// 	if (rVal > 100) rVal = 100;
// 	else if (rVal < 0) rVal = 0;
// 	if (!shape_contains_object(html, oShape, "TxtOP"))
// 		AAA();
// 	else
// 		sprintf_s(shape_get_object(html, oShape, "TxtOP")->m_szScriptValue, "%.0f%%", rVal);
	
// 	const char * index;
// 	if (rVal > rClosed)
// 		index = get_alias_string(html, oShape, "cp_styleForOpen");
// 	else
// 		index = get_alias_string(html, oShape, "cp_styleForClose");

// 	char classname[64]; sprintf_s(classname, "WdgC_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "WdgC"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_Khabarovsk_DamperValves_4State_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim iPV,oShape,index
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	if typename(oShape.objects("WdgC")) <> "Nothing" then oShape.objects("WdgC").styleclass="WdgC_" & index
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_Khabarovsk_DamperValves_4State_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "WdgC_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "WdgC"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_Graph_AlpError_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	if (oSource.quality and iQualityOffScan)=iQualityOffScan then
//   		if typename(oShape.objects("LinExecState"))<>"Nothing" then oShape.objects("LinExecState").styleclass="LinExecState_OffScan_InActive"
//   		if typename(oShape.objects("TxtError"))<>"Nothing" then oShape.objects("TxtError").styleclass="TxtError_NoError"
// 	elseif (oSource.quality and iQualityBad)=iQualityBad then
//   		if typename(oShape.objects("TxtError"))<>"Nothing" then oShape.objects("TxtError").styleclass="TxtError_InError"
//   		if typename(oShape.objects("LinExecState"))<>"Nothing" then oShape.objects("LinExecState").styleclass="LinExecState_OnScan_Active"
// 	else
//   		if typename(oShape.objects("TxtError"))<>"Nothing" then oShape.objects("TxtError").styleclass="TxtError_NoError"
//   		if typename(oShape.objects("LinExecState"))<>"Nothing" then oShape.objects("LinExecState").styleclass="LinExecState_OnScan_Active"
// 	end if
// 	''' ¬Ó ‚ÒÂı ÒÎÛ˜‡ˇı Ò˜ËÚ‡ÂÏ, ◊ÚÓ quality = good
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	set_class(html, shape_get_object(html, oShape, "TxtError"), find_class("TxtError_NoError"));
// 	set_class(html, shape_get_object(html, oShape, "LinExecState"), find_class("LinExecState_OnScan_Active"));
// }
// void VBSCRIPT_Scd_DigStateCColor_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	oShape.objects("RecPV").styleclass="RecPV_" & index
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_DigStateCColor_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "RecPV_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecPV"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_OnOffValves_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim iPV,oShape,iOP,index,indexOP
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	if typename(oShape.objects("PlgIn")) <> "Nothing" then oShape.objects("PlgIn").styleclass="PlgIn_" & index
// 	if typename(oShape.objects("PlgOut")) <> "Nothing" then oShape.objects("PlgOut").styleclass="PlgOut_" & index
// 	if oShape.getcustomproperty("Parameter","cp_op")<>"" then
//    		iOP = oSource.datavalue("Tagname.cp_op")
// 		indexOP = oShape.getCustomProperty("Style","cp_styleForPV" & iOP)
// 		if typename(oShape.objects("WdgC")) <> "Nothing" then oShape.objects("WdgC").styleclass = "WdgC_" & indexOP
//    	end if
// 	if iPV=cint(oShape.getcustomproperty("Num","cp_BadPVState")) then
//  		oShape.objects("TxtBad").styleclass="TxtBad_Bad"
// 	else
//  		oShape.objects("TxtBad").styleclass="TxtBad_NotBad"
// 	end if
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_OnOffValves_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);

// 	char classname[64]; sprintf_s(classname, "PlgIn_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 	sprintf_s(classname, "PlgOut_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));

// 	if (shape_property_not_empty(html, oShape, "cp_op"))
// 	{
// 		long iOP = shape_get_data_value_integer(html, oShape, "tagname", "cp_op");
// 		sprintf_s(concat, "cp_styleForPV%d", iOP);
// 		const char * indexOP = get_alias_string(html, oShape, concat);
// 		sprintf_s(classname, "WdgC_%s", indexOP);
// 		set_class(html, shape_get_object(html, oShape, "WdgC"), find_class(classname));
// 	}
// 	if (iPV == get_alias_int(html, oShape, "cp_BadPVState"))
// 		set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_Bad"));
// 	else
// 		set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_NotBad"));
// 	/* DONE */
// }
// void VBSCRIPT_CDA_Numeric_AlpPVFormat_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim sFormat,oShape, nNumericFormat
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    nNumericFormat=oSource.Datavalue("tagname.cp_pvformat")
//    if IsNumeric(nNumericFormat) and instr(nNumericFormat,".")=0 then
// 	  sFormat="%." & nNumericFormat & "f"
// 	  if typename(oshape.objects("ScConAlpPV"))<>"Nothing" then oShape.objects("ScConAlpPV").numericdisplayformat=sFormat
// 	  if typename(oshape.objects("ConAlpSP"))<>"Nothing" then oShape.objects("ConAlpSP").numericdisplayformat=sFormat
//    else
//    	  window.external.LogMessage "CDA_Numeric_AlpPVFormat_OnUpdate ->" & oSource.id & " " & "Invalid PV Format: " & nNumericFormat
//    end if
//       if err.number<>0 and window.external.environment = 0 then window.external.LogMessage " CDA_Numeric_AlpPVFormat_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	int nNumericFormat = shape_get_data_value_integer(html, oShape, "tagname", "cp_pvformat");
// 	if (nNumericFormat>=0)
// 	{
// 		char sFormat[16];
// 		sprintf_s(sFormat, "%%.%df", nNumericFormat);
// 		CTag * ScConAlpPV = shape_get_object(html, oShape, "ScConAlpPV");
// 		if (ScConAlpPV) sprintf_s(ScConAlpPV->m_szFormat, "%s", sFormat);
// 		CTag * ConAlpSP = shape_get_object(html, oShape, "ConAlpSP");
// 		if (ConAlpSP) sprintf_s(ConAlpSP->m_szFormat, "%s", sFormat);
// 	}
// }
// void VBSCRIPT_Scd_Numeric_AlpPVFormat_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim sFormat,oShape, nNumericFormat
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	nNumericFormat = oshape.getcustomproperty("Text","cp_PVFormat")
// 	if isnumeric(trim(nNumericFormat)) then
// 		sFormat="%." & trim(nNumericFormat) & "f"
// 		if typename(oshape.objects("ScConAlpPV"))<>"Nothing" then oShape.objects("ScConAlpPV").numericdisplayformat=sFormat
// 		if typename(oshape.objects("ConAlpSP"))<>"Nothing" then oShape.objects("ConAlpSP").numericdisplayformat=sFormat
// 	else
// 		window.external.LogMessage "Scd_Numeric_AlpPVFormat_OnUpdate ->" & oSource.id & " " & "Invalid PV Format: " & nNumericFormat
// 	end if
//     if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_Numeric_AlpPVFormat_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	int nNumericFormat = get_alias_int(html, oShape, "cp_PVFormat");
// 	if (nNumericFormat>=0)
// 	{
// 		char sFormat[16];
// 		sprintf_s(sFormat, "%%.%df", nNumericFormat);
// 		CTag * ScConAlpPV = shape_get_object(html, oShape, "ScConAlpPV");
// 		if (ScConAlpPV) sprintf_s(ScConAlpPV->m_szFormat, "%s", sFormat);
// 		CTag * ConAlpSP = shape_get_object(html, oShape, "ConAlpSP");
// 		if (ConAlpSP) sprintf_s(ConAlpSP->m_szFormat, "%s", sFormat);
// 	}
// }
// void VBSCRIPT_Scd_RegCntl_Mode_Hialm_EUDesc_OnUpdate_me_1_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim sMode,sNMode,iHialmT, iAlmEnbState,oShape,oAlm
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	if oshape.getcustomproperty("parameter","cp_mode")<>"" then
// 		sMode=oSource.DataValue("tagname.cp_mode")
// 		oShape.objects("TxtMode").innertext=left(sMode,iLen)
//     end if
// 	if oshape.getcustomproperty("parameter","cp_nmode")<>"" then
// 		sNMode=oSource.DataValue("tagname.cp_nmode")
// 		if sMode=sNMode then
//         	oShape.objects("TxtMode").styleclass="TxtMode_in_NMode"
//     	else
//         	oShape.objects("TxtMode").styleclass="TxtMode_not_in_NMode"
// 		end if
//     end if
// 	if oshape.getcustomproperty("parameter","cp_hialm.type")<>"" then
// 		iHialmT=oSource.DataValue("tagname.cp_hialm.type")
// 		set oAlm=oShape.objects("TxtHiAlmType")
// 		select case iHialmT
// 			case 0 oAlm.innertext=""
// 			case 1 oAlm.innertext="RC"
// 			case 2 oAlm.innertext="DVL"
// 			case 3 oAlm.innertext="DVH"
// 			case 4 oAlm.innertext="XML"
// 			case 5 oAlm.innertext="XMH"
// 			case 6 oAlm.innertext="L"
// 			case 7 oAlm.innertext="H"
// 			case 8 oAlm.innertext="LL"
// 			case 9 oAlm.innertext="HH"
// 			case 10 oAlm.innertext="RSL"
// 			case 11 oAlm.innertext="RSH"
// 		end select
// 	end if
// 	if oShape.getcustomproperty("Text","cp_TagnameEU")="" then oShape.objects("TxtEUDesc").innertext = oSource.DataValue("tagname.cp_eudesc")
// 	if err.number <>0 and window.external.environment = 0 then window.external.LogMessage "Scd_RegCntl_Mode_Hialm_EUDesc_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long sMode = 0;
// 	if (shape_property_not_empty(html, oShape, "cp_mode"))
// 	{
// 		sMode = shape_get_data_value_integer(html, oShape, "tagname", "cp_mode");
// 		switch(sMode)
// 		{
// 			case 0: sprintf_s(shape_get_object(html, oShape, "TxtMode")->m_szScriptValue, "M"); break;
// 			case 1: sprintf_s(shape_get_object(html, oShape, "TxtMode")->m_szScriptValue, "A"); break;
// 			case 2: sprintf_s(shape_get_object(html, oShape, "TxtMode")->m_szScriptValue, "C"); break;
// 			case 3: sprintf_s(shape_get_object(html, oShape, "TxtMode")->m_szScriptValue, "B"); break;
// 			case 4: sprintf_s(shape_get_object(html, oShape, "TxtMode")->m_szScriptValue, "N"); break;
// 			case 5: sprintf_s(shape_get_object(html, oShape, "TxtMode")->m_szScriptValue, "N"); break;
// 			default: sprintf_s(shape_get_object(html, oShape, "TxtMode")->m_szScriptValue, "-"); break;
// 		}
// 	}
// 	if (shape_property_not_empty(html, oShape, "cp_nmode"))
// 	{
// 		long sNMode=shape_get_data_value_integer(html, oShape, "tagname", "cp_nmode");
// 		if (sNMode == sMode)
// 			set_class(html, shape_get_object(html, oShape, "TxtMode"), find_class("TxtMode_in_NMode"));
// 		else
// 			set_class(html, shape_get_object(html, oShape, "TxtMode"), find_class("TxtMode_not_in_NMode"));
// 	}
// 	if (shape_property_not_empty(html, oShape, "cp_hialm.type"))
// 	{
// 		long iHialmT=shape_get_data_value_integer(html, oShape, "tagname", "cp_hialm.type");
// 		CTag* oAlm = shape_get_object(html, oShape, "TxtHiAlmType");
// 		switch(iHialmT)
// 		{
// 			case 0: sprintf_s(oAlm->m_szScriptValue, " "); break;
// 			case 1: sprintf_s(oAlm->m_szScriptValue, "RC"); break;
// 			case 2: sprintf_s(oAlm->m_szScriptValue, "DVL"); break;
// 			case 3: sprintf_s(oAlm->m_szScriptValue, "DVH"); break;
// 			case 4: sprintf_s(oAlm->m_szScriptValue, "XML"); break;
// 			case 5: sprintf_s(oAlm->m_szScriptValue, "XMH"); break;
// 			case 6: sprintf_s(oAlm->m_szScriptValue, "L"); break;
// 			case 7: sprintf_s(oAlm->m_szScriptValue, "H"); break;
// 			case 8: sprintf_s(oAlm->m_szScriptValue, "LL"); break;
// 			case 9: sprintf_s(oAlm->m_szScriptValue, "HH"); break;
// 			case 10: sprintf_s(oAlm->m_szScriptValue, "RSL"); break;
// 			case 11: sprintf_s(oAlm->m_szScriptValue, "RSH"); break;
// 			default: sprintf_s(oAlm->m_szScriptValue, " "); break;
// 		}
// 	}
// 	if (!shape_property_not_empty(html, oShape, "cp_TagnameEU"))
// 	{
// 		char * value = shape_get_data_value_string(html, oShape, "tagname", "cp_eudesc");
// 		sprintf_s(shape_get_object(html, oShape, "TxtEUDesc")->m_szScriptValue, "%s", value);
// 	}
// 	/* ~DONE */
// }

// void VBSCRIPT_CDA_DigState_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim oShape
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    iPV=oSource.breakpointvalueenum
//    index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
//    oShape.objects("ScConAlpPV").styleclass="ScConAlpPV_" & index
//    if err.number<>0 and window.external.environment = 0 then window.external.LogMessage " CDA_DigState_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html); //iPV=oSource.breakpointvalueenum  !!!
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "ScConAlpPV_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "ScConAlpPV"), find_class(classname));
// 	char txt[512] = ""; oSource->FillText( html, txt );
// 	if (txt && *txt) sprintf_s(oSource->m_szScriptValue, "%s", txt);
// 	/* DONE */
// }
// void VBSCRIPT_CDA_DigStateCColor_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim oShape
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    iPV=oSource.value
//    index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
//    oShape.objects("RecPV").styleclass="RecPV_" & index
//    if err.number<>0 and window.external.environment = 0 then window.external.LogMessage " CDA_DigStateCColor_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "RecPV_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecPV"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_CDA_OnOffValves_AlpGPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim igPV,oShape,igOP,index,indexOP
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    igPV=oSource.value
//    index=oShape.getCustomProperty("Style","cp_styleForPV" & igPV)
//    if typename(oShape.objects("PlgIn")) <> "Nothing" then oShape.objects("PlgIn").styleclass="PlgIn_" & index
//    if typename(oShape.objects("PlgOut")) <> "Nothing" then oShape.objects("PlgOut").styleclass="PlgOut_" & index
//    if igPV=cint(oShape.getcustomproperty("Num","cp_BadPVState")) then
//       oShape.objects("TxtBad").styleclass="TxtBad_Bad"
//    else
//       oShape.objects("TxtBad").styleclass="TxtBad_NotBad"
//    end if
//    if oShape.getcustomproperty("Parameter","cp_gop")<>"" then
//    		igOP = oSource.datavalue("Tagname.cp_gop")
// 		indexOP = oShape.getCustomProperty("Style","cp_styleForPV" & igOP)
// 		if typename(oShape.objects("WdgC")) <> "Nothing" then oShape.objects("WdgC").styleclass = "WdgC_" & indexOP
//    end if
//    if err.number<>0 and window.external.environment = 0 then window.external.LogMessage " CDA_OnOffValves_AlpGPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long igPV = get_value_integer(oSource, html);
// 	if (igPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", igPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "PlgIn_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 	sprintf_s(classname, "PlgOut_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));

// 	if (igPV == get_alias_int(html, oShape, "cp_BadPVState"))
// 	{
// 		set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_Bad"));
// 	}
// 	else
// 	{
// 		set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_NotBad"));
// 	}
// 	if (shape_contains_property(html, oShape, "cp_gop"))
// 	{
// 		long igOP = shape_get_data_value_integer(html, oShape, "tagname", "cp_gop");
// 		sprintf_s(concat, "cp_styleForPV%d", igOP);
// 		const char * indexOP = get_alias_string(html, oShape, concat);
// 		sprintf_s(classname, "WdgC_%s", indexOP);
// 		set_class(html, shape_get_object(html, oShape, "WdgC"), find_class(classname));
// 	}
// 	/* DONE */
// }
// void VBSCRIPT_Scd_Device3States_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim iPV,oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
//    	if typename(oShape.objects("OvlBody")) <> "Nothing" then oShape.objects("OvlBody").styleclass="OvlBody_" & index
//     if typename(oShape.objects("PlgBody")) <> "Nothing" then oShape.objects("PlgBody").styleclass="PlgBody_" & index
//     if typename(oShape.objects("RecBody")) <> "Nothing" then oShape.objects("RecBody").styleclass="RecBody_" & index
// 	if iPV=cint(oShape.getcustomproperty("Num","cp_BadPVState")) then
//  		oShape.objects("TxtBad").styleclass="TxtBad_Bad"
// 	else
//  		oShape.objects("TxtBad").styleclass="TxtBad_NotBad"
// 	end if
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_Device3States_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "OvlBody_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "OvlBody"), find_class(classname));
// 	sprintf_s(classname, "PlgBody_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgBody"), find_class(classname));
// 	sprintf_s(classname, "RecBody_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecBody"), find_class(classname));
// 	if (iPV == get_alias_int(html, oShape, "cp_BadPVState"))
// 		set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_Bad"));
// 	else
// 		set_class(html, shape_get_object(html, oShape, "TxtBad"), find_class("TxtBad_NotBad"));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_DigState_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.breakpointvalueenum
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	oShape.objects("ScConAlpPV").styleclass="ScConAlpPV_" & index
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage " Scd_DigState_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html); // iPV=oSource.breakpointvalueenum !!!
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "ScConAlpPV_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "ScConAlpPV"), find_class(classname));

// 	char state[32]; sprintf_s(state, "STATEDES%d", iPV);
// 	char * statedes = shape_get_data_value_string(html, oSource, "tagname", state);
// 	sprintf_s(oSource->m_szScriptValue, "%s", statedes);

// 	/* DONE */
// }
// void VBSCRIPT_Scd_DigStateButton_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	iPV1=oSource.datavalue("tagname.cp_pv")

// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)

// 	if iPV = 0 then
// 			oShape.objects("RecFrame").styleclass = "RecFrame_" & index
// 			oShape.objects("TxtState").innertext = iPV1
// 		else
// 			oShape.objects("RecFrame").styleclass = "RecFrame_" & index
// 			oShape.objects("TxtState").innertext = iPV1
// 	end if

// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_DigStateButton_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	long iPV1 = shape_get_data_value_integer(html, oSource, "tagname", "cp_pv");
// 	if (iPV < 0) return;
	
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
	
// 	char classname[64]; sprintf_s(classname, "RecFrame_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecFrame"), find_class(classname));

// 	char field[32]; sprintf(field, "STATEDES%d", iPV1);
// 	char * txt = shape_get_data_value_string(html, oSource, "tagname", field);
// 	sprintf_s(shape_get_object(html, oShape, "TxtState")->m_szScriptValue, "%s", txt);
// 	/* DONE */
// }
// void VBSCRIPT_Scd_DigStateColor_Khabarovsk_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)

// 	select case ipv
// 		case 0 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 1 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 2 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 3 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 4 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 5 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 6 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 7 oShape.objects("RecPV").styleclass="RecPV_" & index
// 	end select

// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_DigStateColor_Khabarovsk_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV >= 0 && iPV <= 7)
// 	{
// 		char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 		const char * index = get_alias_string(html, oShape, concat);
// 		char classname[64]; sprintf_s(classname, "RecPV_%s", index);
// 		set_class(html, shape_get_object(html, oShape, "RecPV"), find_class(classname));
// 	}
// 	/* DONE */
// }
// void VBSCRIPT_Scd_Khabarovsk_BoxLocRem_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim oShape
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    iPV=oSource.value
// 	if typename(oShape.objects("TxtLocRem"))<>"Nothing" then
// 		oShape.objects("TxtLocRem").innertext=oShape.getCustomProperty("Text","cp_TextForPV" & iPV)
// 		oShape.objects("RecFrame").styleclass="BoxLocRem_" & oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	end if
// 	if err.number<>0 then window.external.LogMessage "Scd_Khabarovsk_BoxLocRem_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV<0) return;
// 	char concat[32]; sprintf_s(concat, "cp_TextForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	sprintf_s(shape_get_object(html, oShape, "TxtLocRem")->m_szScriptValue, "%s", index);
// 	sprintf_s(concat, "cp_StyleForPV%d", iPV);
// 	index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "BoxLocRem_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecFrame"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_Khabarovsk_DigStateCColor_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	oShape.objects("RecPV").styleclass="RecPV_" & index
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_Khabarovsk_DigStateCColor_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "RecPV_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecPV"), find_class(classname));
// 	/* DONE */
// }

// void VBSCRIPT_CDA_DigStateButton_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	iPV1=oSource.datavalue("tagname.cp_pv")

// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)

// 	select case ipv
// 		case 0 oShape.objects("RecFrame").styleclass = "RecFrame_" & index
// 				oShape.objects("TxtState").innertext = iPV1
// 		case 1 oShape.objects("RecFrame").styleclass = "RecFrame_" & index
// 				oShape.objects("TxtState").innertext = iPV1
// 		case 4 oShape.objects("RecFrame").styleclass = "RecFrame_" & index
// 				oShape.objects("TxtState").innertext = iPV1
// 		case 5 oShape.objects("RecFrame").styleclass = "RecFrame_" & index
// 				oShape.objects("TxtState").innertext = iPV1
// 		case 6 oShape.objects("RecFrame").styleclass = "RecFrame_" & index
// 				oShape.objects("TxtState").innertext = iPV1
// 	end select

// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "CDA_DigStateButton_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	long iPV1 = shape_get_data_value_integer(html, oSource, "tagname", "cp_pv");
// 	if (iPV == 0 || iPV == 1 || iPV == 4 || iPV == 5 || iPV == 6)
// 	{
// 		char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 		const char * index = get_alias_string(html, oShape, concat);
// 		char classname[64]; sprintf_s(classname, "RecFrame_%s", index);
// 		set_class(html, shape_get_object(html, oShape, "RecFrame"), find_class(classname));
// 		switch(iPV1)
// 		{
// 		case 0: sprintf_s(shape_get_object(html, oShape, "TxtState")->m_szScriptValue, "Bad"); break;
// 		case 1: sprintf_s(shape_get_object(html, oShape, "TxtState")->m_szScriptValue, "Inbet"); break;
// 		case 2: sprintf_s(shape_get_object(html, oShape, "TxtState")->m_szScriptValue, "Active"); break;
// 		case 3: sprintf_s(shape_get_object(html, oShape, "TxtState")->m_szScriptValue, "Safe"); break;
// 		case 4: sprintf_s(shape_get_object(html, oShape, "TxtState")->m_szScriptValue, "STOP"); break;
// 		case 5: sprintf_s(shape_get_object(html, oShape, "TxtState")->m_szScriptValue, "START"); break;
// 		case 6: sprintf_s(shape_get_object(html, oShape, "TxtState")->m_szScriptValue, "Not Used"); break;
// 		}
		
// 	}
// 	/* DONE */
// }
// void VBSCRIPT_CDA_DigStateColor_Khabarovsk_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)

// 	select case ipv
// 		case 0 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 1 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 4 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 5 oShape.objects("RecPV").styleclass="RecPV_" & index
// 		case 6 oShape.objects("RecPV").styleclass="RecPV_" & index
// 	end select
		
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage " CDA_DigStateColor_Khabarovsk_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV == 0 || iPV == 1 || iPV == 4 || iPV == 5 || iPV == 6)
// 	{
// 		char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 		const char * index = get_alias_string(html, oShape, concat);
// 		char classname[64]; sprintf_s(classname, "RecPV_%s", index);
// 		set_class(html, shape_get_object(html, oShape, "RecPV"), find_class(classname));
// 	}
// 	/* DONE */
// }
// void VBSCRIPT_Scd_Khabarovsk_Bypass_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	Dim oShape,iPV
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	oShape.objects("RecPV").styleclass="Enclav_TxtBypass_" & index
// 	if err.number<>0 then window.external.LogMessage "Scd_Khabarovsk_Bypass_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "Enclav_TxtBypass_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecPV"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_KHABAROVSK_RotorkValvesLocalRemote_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	Dim iPV3,oShape,index3
// 	On Error Resume Next
// 	Set oShape=oSource.parentnode.parentnode
// 	iPV3=oSource.datavalue("tagname3.cp3_pv")
// 	index3=oShape.getCustomProperty("Style","cp3_styleForPV" & iPV3)
// 	if typename(oShape.objects("TxtLocalRemoto")) <> "Nothing" then oShape.objects("TxtLocalRemoto").styleclass="TxtLocalRemoto_" & index3
// 	If err.number<>0 then window.external.LogMessage "Scd_KHABAROVSK_RotorkValvesLocalRemote_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV3 = get_value_integer(oSource, html);
// 	if (iPV3 < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp3_styleForPV%d", iPV3);
// 	const char * index3 = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "TxtLocalRemoto_%s", index3);
// 	set_class(html, shape_get_object(html, oShape, "TxtLocalRemoto"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_libKHABAROVSK_Bypass_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	Dim igPV,oShape
// 	On Error Resume Next
// 	Set oShape=oSource.parentnode.parentnode
// 	igPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & igPV)
// 	If typename(oShape.objects("RecPV")) <> "Nothing" then oShape.objects("RecPV").styleclass="RecPV_" & index
// 	If typename(oShape.objects("OvlLlave")) <> "Nothing" then oShape.objects("OvlLlave").styleclass="RecPV_" & index
// 	If typename(oShape.objects("LinLlave")) <> "Nothing" then oShape.objects("LinLlave").styleclass="RecPV_" & index
// 	If typename(oShape.objects("LinLlave1")) <> "Nothing" then oShape.objects("LinLlave1").styleclass="RecPV_" & index
// 	If typename(oShape.objects("LinLlave2")) <> "Nothing" then oShape.objects("LinLlave2").styleclass="RecPV_" & index
// 	If typename(oShape.objects("LinLlave3")) <> "Nothing" then oShape.objects("LinLlave3").styleclass="RecPV_" & index
// 	If err.number<>0 then window.external.LogMessage " CDA_RLR_libACPDM_Bypass_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long igPV = get_value_integer(oSource, html);
// 	if (igPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", igPV);
// 	const char * index = get_alias_string(html, oShape, concat);

// 	char classname[64]; sprintf_s(classname, "RecPV_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecPV"), find_class(classname));
// 	set_class(html, shape_get_object(html, oShape, "OvlLlave"), find_class(classname));
// 	set_class(html, shape_get_object(html, oShape, "LinLlave"), find_class(classname));
// 	set_class(html, shape_get_object(html, oShape, "LinLlave1"), find_class(classname));
// 	set_class(html, shape_get_object(html, oShape, "LinLlave2"), find_class(classname));
// 	set_class(html, shape_get_object(html, oShape, "LinLlave3"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_libKHABAROVSK_Interlock_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	Dim iPV,oShape,index
// 	On Error Resume Next
// 	Set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	if typename(oShape.objects("PlgInterlock")) <> "Nothing" then oShape.objects("PlgInterlock").styleclass="PlgInterlock_" & index
// 	If err.number<>0 then window.external.LogMessage "Scd_libKHABAROVSK_Interlock_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "PlgInterlock_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgInterlock"), find_class(classname));
// 	/* DONE */
// }
// void VBSCRIPT_Scd_AbsorberStatus_Khabarovsk_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape,iPV,index
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	if typename(oShape.objects("TxtLabel")) <> "Nothing" then oShape.objects("TxtLabel").styleclass="Txt_" & index
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_AbsorberStatus_Khabarovsk_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	if (iPV < 0) return;
// 	char concat[32]; sprintf_s(concat, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, concat);
// 	char classname[64]; sprintf_s(classname, "Txt_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "TxtLabel"), find_class(classname));
// 	/* DONE */
// }

// void VBSCRIPT_Scd_Khabarovsk_Interlock_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape,n,var,tagexist
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	set oSource=oshape.objects("ScConAlpPV")
// 	if typename(oShape.objects("PlgInterLock")) <> "Nothing" then
// 		for n=1 to 15
// 			if (oShape.getcustomproperty("point","Tagname" & n )<> "")then
// 				tagexist = n
// 			end if
// 		next

// 		var=0
// 		for n=1 to tagexist
// 			if oSource.datavalue("Tagname"&n&".pv") = 1 then
// 				var = var + 1
// 			end if
// 		next

// 		if var < tagexist then
// 			oShape.objects("PlgInterLock").styleclass="PlgInterlock_Interlock_ON"
// 		else
// 			oShape.objects("PlgInterLock").styleclass="PlgInterlock_Interlock_OFF"
// 		end if
// 	end if
	
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_Khabarovsk_Interlock_OnUpdate ->" & oSource.id & " " & err.description

// */
// 	int tagexist = 0;
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	oSource = shape_get_object(html, oShape, "ScConAlpPV");

// 	if ( shape_contains_object(html, oShape, "PlgInterLock"))
// 	{
// 		for (int n = 1; n <= 15; n++)
// 		{
// 			char prop[16]; sprintf_s(prop, "Tagname%d", n);
// 			if (shape_property_not_empty(html, oShape, prop)) tagexist = n;
// 		}
// 		int var = 0;
// 		for (int n = 1; n <= tagexist; n++)
// 		{
// 			char tag[16]; sprintf_s(tag, "Tagname%d", n);
// 			if (shape_get_data_value_integer(html, oSource, tag, "pv") == 1) var++;
// 		}
// 		if (var < tagexist)
// 			set_class(html, shape_get_object(html, oShape, "PlgInterLock"), find_class("PlgInterlock_Interlock_ON"));
// 		else
// 			set_class(html, shape_get_object(html, oShape, "PlgInterLock"), find_class("PlgInterlock_Interlock_OFF"));
// 	}
// 	/* DONE */
// }
// void VBSCRIPT_CDA_RegCntl_Mode_Hialm_RedTag_EUDesc_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
//    dim sMode, oMode, iModeAttr, iNModeAttr, iPID_HialmT, iDac_HialmT, iAlmEnbState, iRedTag, oShape,oPIDAlm,oDACAlm, oInitWindup, iInitMan,iArwop
//    on error resume next
//    set oShape=oSource.parentnode.parentnode
//    if typename(oShape.objects("TxtMode"))<>"Nothing" then
// 	   set oMode=oShape.objects("TxtMode")
// 	   if oshape.getcustomproperty("parameter","cp_mode")<>"" then
// 	      sMode=oSource.DataValue("tagname.cp_mode")
// 	      oMode.innertext=left(sMode,1)
// 	   end if
// 	   if oshape.getcustomproperty("parameter","cp_nmode")<>"" then
// 	      sNMode=oSource.DataValue("tagname.cp_nmode")
// 		  if sMode=sNMode then
// 	      	oMode.styleclass="TxtMode_in_NMode"
// 	      else
// 	      	oMode.styleclass="TxtMode_not_in_NMode"
// 	      end if
// 	   end if
// 	   if oshape.getcustomproperty("parameter","cp_nmodeAttr")<>"" then
// 		  iNModeAttr=oSource.DataValue("tagname.cp_nmodeAttr")
// 		  iModeAttr=oSource.DataValue("tagname.cp_modeAttr")
// 		  if iNModeAttr<>0 then
// 		   	  if iModeAttr=iNModeAttr then
// 		      	oMode.linecolor="transparent"
// 		      else
// 		      	oMode.linecolor=oMode.getstyleproperty("mAttrProgF")
// 				oMode.linestyle=1
// 			    oMode.linewidth=oMode.getstyleproperty("lWidth")
// 		      end if
// 			  if iModeAttr=2 then oMode.textcolor = oMode.getstyleproperty("mAttrProgL")
//            end if
// 	   end if
//    end if
//    if oshape.getcustomproperty("parameter","cp_PID_hialm.type")<>"" then
//       iPID_HialmT=oSource.DataValue("tagname.cp_PID_hialm.type")
//       set oPIDAlm=oShape.objects("TxtHiAlmTypePIDA")
//       select case iPID_HialmT
// 	     case 0 oPIDAlm.innertext=""
// 	     case 1 oPIDAlm.innertext="CTL"
// 	     case 2 oPIDAlm.innertext="SI"
// 	     case 3 oPIDAlm.innertext="DVH"
// 	     case 4 oPIDAlm.innertext="DVL"
// 	     case 5 oPIDAlm.innertext="ADV"
// 	     case 6 oPIDAlm.innertext="OPH"
// 	     case 7 oPIDAlm.innertext="OPL"
//       end select
//    end if
//    if oshape.getcustomproperty("parameter","cp_DAC_hialm.type")<>"" then
//       iDAC_HialmT=oSource.DataValue("tagname.cp_DAC_hialm.type")
//       set oDACAlm=oShape.objects("TxtHiAlmTypeDACA")
//       select case iDAC_HialmT
// 	     case 0 oDACAlm.innertext=""
// 	     case 1 oDACAlm.innertext="BAD"
// 	     case 2 oDACAlm.innertext="HH"
// 	     case 3 oDACAlm.innertext="LL"
// 	     case 4 oDACAlm.innertext="H"
// 	     case 5 oDACAlm.innertext="L"
// 	     case 6 oDACAlm.innertext="PRC"
// 	     case 7 oDACAlm.innertext="NRC"
//       end select
//    end if
//    if oshape.getcustomproperty("parameter","cp_redtag")<>"" then
//       iRedTag=oSource.DataValue("tagname.cp_redtag")
//       if iRedTag=1 then
//          oShape.objects("WdgRedTag").styleclass="WdgRedTag_On"
//       else
//          oShape.objects("WdgRedTag").styleclass="WdgRedTag_Off"
//       end if
//    end if
//    if oshape.getcustomproperty("parameter","cp_initman")<>"" then
//       set oInit = oShape.objects("TxtInit")
// 	  set oWindup = oShape.objects("LinWR")
// 	  iInitMan = oSource.datavalue ("tagname.cp_initman")
// 	  oWindup.styleclass = "LinWR_No_WindupReset"
// 	  	if iInitMan = 1 then
// 			oInit.styleclass = "Init_Active"
// 			oWindup.styleclass = "LinWR_No_WindupReset"
// 		else
// 			oInit.styleclass = "Init_Inactive"
// 			if oshape.getcustomproperty("parameter","cp_sptvstate")<>"" then
// 				iTVstate=oSource.datavalue("Tagname.cp_sptvstate")
// 				iTV=oSource.datavalue("Tagname.cp_sptv")
// 				if iTVState=2 then
// 					if typename(oShape.objects("ConAlpSP"))<>"Nothing" then
// 						rSP=oShape.objects("ConAlpSP").value
//                     else
//                         if oshape.getcustomproperty("parameter","cp_dev")<>"" then
// 							rSP=oSource.value-oSource.datavalue("tagname.cp_dev")
//                         else
// 							rSP=oSource.datavalue("tagname.cp_sp")
// 						end if
// 					end if
// 				    if iTV>rSP then oWindup.styleclass="LinWR_Ramp_HI"
// 					if iTV<rSP then oWindup.styleclass="LinWR_Ramp_LO"
//                 else
// 					oWindup.styleclass="LinWR_No_WindupReset"
// 				end if
//             end if
// 			if oshape.getcustomproperty("parameter","cp_arwop")<>"" then
// 				iArwop = oSource.datavalue ("tagname.cp_arwop")
// 				select case iArwop
// 				     case 1 oWindup.styleclass="LinWR_Windup_Hi"
// 				     case 2 oWindup.styleclass="LinWR_Windup_LO"
// 				     case 3 oWindup.styleclass="LinWR_Windup_HILO"
// 				end select
//             end if
// 		end if
//    end if
//    if oShape.getcustomproperty("Text","cp_TagnameEU")="" then oShape.objects("TxtEUDesc").innertext = oSource.DataValue("tagname.cp_eudesc")
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	if (shape_contains_object(html, oShape, "TxtMode"))
// 	{
// 		long sMode = 0;
// 		CTag * oMode = shape_get_object(html, oShape, "TxtMode");
// 		if (shape_property_not_empty(html, oShape, "cp_mode"))
// 		{
// 			sMode = shape_get_data_value_integer(html, oSource, "tagname","cp_mode");
// 			switch(sMode)
// 			{
// 			case 0: sprintf_s(oMode->m_szScriptValue, "M"); break;
// 			case 1: sprintf_s(oMode->m_szScriptValue, "A"); break;
// 			case 2: sprintf_s(oMode->m_szScriptValue, "C"); break;
// 			case 3: sprintf_s(oMode->m_szScriptValue, "B"); break;
// 			case 4: sprintf_s(oMode->m_szScriptValue, "N"); break;
// 			case 5: sprintf_s(oMode->m_szScriptValue, "N"); break;
// 			default: sprintf_s(oMode->m_szScriptValue, "-"); break;
// 			}
// 		}
// 		if (shape_property_not_empty(html, oShape, "cp_nmode"))
// 		{
// 			long sNMode = shape_get_data_value_integer(html, oSource, "tagname","cp_nmode");
// 			if (sNMode == sMode)
// 			{
// 				set_class(html, oMode, find_class("TxtMode_in_NMode"));
// 			}
// 			else
// 			{
// 				set_class(html, oMode, find_class("TxtMode_not_in_NMode"));
// 			}
// 		}
// 		if (shape_property_not_empty(html, oShape, "cp_nmodeAttr"))
// 		{
// 			long iNModeAttr = shape_get_data_value_integer(html, oSource, "tagname","cp_nmodeAttr");
// 			long iModeAttr = shape_get_data_value_integer(html, oSource, "tagname","cp_modeAttr");
// 			if (iNModeAttr != 0)
// 			{
// 				CStyle& style = html.Style(oMode->m_nStyle);
// 				if (iModeAttr == iNModeAttr)
// 				{
// 					// oMode.linecolor="transparent"
// 				}
// 				else
// 				{
// 					// oMode.linecolor=oMode.getstyleproperty("mAttrProgF")
// 					// oMode.linestyle=1
// 					// oMode.linewidth=oMode.getstyleproperty("lWidth")
// 				}
// 				if (iModeAttr == 2)
// 				{
// 					// oMode.textcolor = oMode.getstyleproperty("mAttrProgL")
// 				}
// 			}
// 		}
// 	}
// 	if (shape_property_not_empty(html, oShape, "cp_PID_hialm.type"))
// 	{
// 		long iPID_HialmT = shape_get_data_value_integer(html, oSource, "tagname","cp_PID_hialm.type");
// 		CTag * oPIDAlm = shape_get_object(html, oShape, "TxtHiAlmTypePIDA");
// 		switch(iPID_HialmT)
// 		{
// 		case 0: sprintf_s(oPIDAlm->m_szScriptValue, " "); break;
// 		case 1: sprintf_s(oPIDAlm->m_szScriptValue, "CTL"); break;
// 		case 2: sprintf_s(oPIDAlm->m_szScriptValue, "SI"); break;
// 		case 3: sprintf_s(oPIDAlm->m_szScriptValue, "DVH"); break;
// 		case 4: sprintf_s(oPIDAlm->m_szScriptValue, "DVL"); break;
// 		case 5: sprintf_s(oPIDAlm->m_szScriptValue, "ADV"); break;
// 		case 6: sprintf_s(oPIDAlm->m_szScriptValue, "OPH"); break;
// 		case 7: sprintf_s(oPIDAlm->m_szScriptValue, "OPL"); break;
// 		}
// 	}
// 	if (shape_property_not_empty(html, oShape, "cp_DAC_hialm.type"))
// 	{
// 		long iDAC_HialmT = shape_get_data_value_integer(html, oSource, "tagname","cp_DAC_hialm.type");
// 		CTag * oDACAlm = shape_get_object(html, oShape, "TxtHiAlmTypeDACA");
// 		switch(8 - iDAC_HialmT)
// 		{
// 		case 8: sprintf_s(oDACAlm->m_szScriptValue, " "); break;
// 		case 1: sprintf_s(oDACAlm->m_szScriptValue, "BAD"); break;
// 		case 2: sprintf_s(oDACAlm->m_szScriptValue, "HH"); break;
// 		case 3: sprintf_s(oDACAlm->m_szScriptValue, "LL"); break;
// 		case 4: sprintf_s(oDACAlm->m_szScriptValue, "H"); break;
// 		case 5: sprintf_s(oDACAlm->m_szScriptValue, "L"); break;
// 		case 6: sprintf_s(oDACAlm->m_szScriptValue, "PRC"); break;
// 		case 7: sprintf_s(oDACAlm->m_szScriptValue, "NRC"); break;
// 		}
// 	}
// 	if (shape_property_not_empty(html, oShape, "cp_redtag"))
// 	{
// 		long iRedTag = shape_get_data_value_integer(html, oSource, "tagname","cp_redtag");
// 		if (iRedTag == 1)
// 		{
// 			set_class(html, shape_get_object(html, oShape, "WdgRedTag"), find_class("WdgRedTag_On"));
// 		}
// 		else
// 		{
// 			set_class(html, shape_get_object(html, oShape, "WdgRedTag"), find_class("WdgRedTag_Off"));
// 		}
// 	}
// 	if (shape_property_not_empty(html, oShape, "cp_initman"))
// 	{
// 		CTag * oInit = shape_get_object(html, oShape, "TxtInit");
// 		CTag * oWindup = shape_get_object(html, oShape, "LinWR");
// 		long iInitMan = shape_get_data_value_integer(html, oSource, "tagname","cp_initman");
// 		set_class(html, oWindup, find_class("LinWR_No_WindupReset"));
// 		if (iInitMan == 1)
// 		{
// 			set_class(html, oInit, find_class("Init_Active"));
// 			set_class(html, oWindup, find_class("LinWR_No_WindupReset"));
// 		}
// 		else
// 		{
// 			set_class(html, oInit, find_class("Init_Inactive"));
// 			if (shape_property_not_empty(html, oShape, "cp_sptvstate"))
// 			{
// 				long rSP = 0;
// 				long iTVstate = shape_get_data_value_integer(html, oSource, "tagname","cp_sptvstate");
// 				long iTV = shape_get_data_value_integer(html, oSource, "tagname","cp_sptv");
// 				if (iTVstate == 2)
// 				{
// 					if (shape_contains_object(html, oShape, "ConAlpSP"))
// 					{
// 						rSP = get_value_integer(shape_get_object(html, oShape, "ConAlpSP"), html);
// 					}
// 					else
// 					{
// 						if (shape_property_not_empty(html, oShape, "cp_dev"))
// 						{
// 							rSP = get_value_integer(oSource, html) - shape_get_data_value_integer(html, oSource, "tagname", "cp_dev");
// 						}
// 						else
// 						{
// 							rSP = shape_get_data_value_integer(html, oSource, "tagname", "cp_sp");
// 						}
// 					}
// 					if (iTV > rSP) set_class(html, oWindup, find_class("LinWR_Ramp_HI"));
// 					if (iTV < rSP) set_class(html, oWindup, find_class("LinWR_Ramp_LO"));
// 				}
// 				else
// 				{
// 					set_class(html, oWindup, find_class("LinWR_No_WindupReset"));
// 				}
// 			}
// 			if (shape_property_not_empty(html, oShape, "cp_arwop"))
// 			{
// 				long iArwop = shape_get_data_value_integer(html, oSource, "tagname","cp_arwop");
// 				switch(iArwop)
// 				{
// 				case 1: set_class(html, oWindup, find_class("LinWR_Windup_Hi")); break;
// 				case 2: set_class(html, oWindup, find_class("LinWR_Windup_LO")); break;
// 				case 3: set_class(html, oWindup, find_class("LinWR_Windup_HILO")); break;
// 				}
// 			}
// 		}
// 	}
// 	if (shape_property_not_empty(html, oShape, "cp_TagnameEU"))
// 	{
// 		char * eu = shape_get_data_value_string(html, oSource, "tagname", "cp_eudesc");
// 		sprintf_s(shape_get_object(html, oShape, "TxtEUDesc")->m_szScriptValue, "%s", eu);
// 	}
// 	/* DONE */
// }
// void VBSCRIPT_Scd_Khabarovsk_Enclav_onupdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	Dim igPV, igPV1, igPV2, igPV3, igPV4, igPV5, igPV6, igPV7, igPV8, igPV9, igPV10, igPV11, igPV12, igPV13, igPV14, igPV15, igPVx, index
// 	on error resume next
	
// 	set oShape=oSource.parentnode.parentnode

// 	if typename(oShape.objects("TxtDescription")) <> "Nothing" then
// 		oShape.objects("TxtDescription").innertext="  " & oSource.DataValue("Enclav_tagname.cp_Enclav_Desc")
// 	end if
	
// 	igPV=oSource.DataValue("Enclav_tagname.cp_pv")
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & igPV)
// 	if typename(oShape.objects("TxtEnclavName")) <> "Nothing" then
// 		oShape.objects("TxtEnclavName").styleclass="Enclav_TxtName_" & index
// 	end if

// 	if typename(oShape.objects("TxtBypass")) <> "Nothing" then
// 		igPV1=oSource.DataValue("Bypass_tagname.cp_pv")
// 		igPV2=oSource.DataValue("Bypass_tagname1.cp_pv")
// 		igPV3=oSource.DataValue("Bypass_tagname2.cp_pv")
// 		igPV4=oSource.DataValue("Bypass_tagname3.cp_pv")
// 		igPV5=oSource.DataValue("Bypass_tagname4.cp_pv")
// 		igPV6=oSource.DataValue("Bypass_tagname5.cp_pv")
// 		igPV7=oSource.DataValue("Bypass_tagname6.cp_pv")
// 		igPV8=oSource.DataValue("Bypass_tagname7.cp_pv")
// 		igPV9=oSource.DataValue("Bypass_tagname8.cp_pv")
// 		igPV10=oSource.DataValue("Bypass_tagname9.cp_pv")
// 		igPV11=oSource.DataValue("Bypass_tagname10.cp_pv")
// 		igPV12=oSource.DataValue("Bypass_tagname11.cp_pv")
// 		igPV13=oSource.DataValue("Bypass_tagname12.cp_pv")
// 		igPV14=oSource.DataValue("Bypass_tagname13.cp_pv")
// 		igPV15=oSource.DataValue("Bypass_tagname14.cp_pv")
// 		if igPV1 = 1 or igPV2 = 1 or igPV3 = 1 or igPV4 = 1 or igPV5 = 1 or igPV6 = 1 or igPV7 = 1 or igPV8 = 1 or igPV9 = 1 or igPV10 = 1 or igPV11 = 1 or igPV12 = 1 or igPV13 = 1 or igPV14 = 1 or igPV15 = 1 then
// 			igPVx = 1
// 			index=oShape.getCustomProperty("Style","cp_styleForBP" & igPVx)
// 			oShape.objects("TxtBypass").styleclass="Enclav_TxtBypass_" & index
// 			oShape.objects("TxtBypass").innertext= oShape.getCustomProperty("Text","cp_TxtForPV" & igPVx)
// 		else
// 			igPVx = 0
// 			index=oShape.getCustomProperty("Style","cp_styleForBP" & igPVx)
// 			oShape.objects("TxtBypass").styleclass="Enclav_TxtBypass_" & index
// 			oShape.objects("TxtBypass").innertext= oShape.getCustomProperty("Text","cp_TxtForPV" & igPVx)
// 		end if

// 	end if

// 	if err.number <>0 then window.external.LogMessage " Scd_Khabarovsk_Enclav_onupdate ->" & oSource.id & " " & err.description
// */
// 	//return;
// 	CTag* oShape = oSource->m_pPar->m_pPar;
	
// 	if (shape_contains_object(html, oShape, "TxtDescription"))
// 	{
// 		char * text = shape_get_data_value_string(html, oSource, "Enclav_tagname", "cp_Enclav_Desc");
// 		sprintf_s(shape_get_object(html, oShape, "TxtDescription")->m_szScriptValue, "  %s", text);
// 	}
// 	long igPV = shape_get_data_value_integer(html, oSource, "Enclav_tagname", "cp_pv");
// 	char prop[32]; sprintf_s(prop, "cp_styleForPV%d", igPV);
// 	const char * index = get_alias_string(html, oShape, prop);
// 	char classname[64]; sprintf_s(classname, "Enclav_TxtName_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "TxtEnclavName"), find_class(classname));
// 	if (igPV >= 0)
// 	{
// 		char name[32]; sprintf_s(name, "cp_styleForPV%d", igPV);
// 		const char * index = get_alias_string(html, oShape, name);
// 		if (shape_contains_object(html, oShape, "TxtEnclavName"))
// 		{
// 			char classname[64]; sprintf_s(classname, "Enclav_TxtName_%s", index);
// 			set_class(html, shape_get_object(html, oShape, "TxtEnclavName"), find_class(classname));
// 		}
// 	}

// 	if (shape_contains_object(html, oShape, "TxtBypass"))
// 	{
// 		long igPV1, igPV2, igPV3, igPV4, igPV5, igPV6, igPV7, igPV8, igPV9, igPV10, igPV11, igPV12, igPV13, igPV14, igPV15;
// 		igPV1 = igPV2 = igPV3 = igPV4 = igPV5 = igPV6 = igPV7 = igPV8 = igPV9 = igPV10 = igPV11 = igPV12 = igPV13 = igPV14 = igPV15 = 0;
// 		igPV1 = shape_get_data_value_integer(html, oSource, "Bypass_tagname", "cp_pv");
// 		igPV2 = shape_get_data_value_integer(html, oSource, "Bypass_tagname1", "cp_pv");
// 		igPV3 = shape_get_data_value_integer(html, oSource, "Bypass_tagname2", "cp_pv");
// 		igPV4 = shape_get_data_value_integer(html, oSource, "Bypass_tagname3", "cp_pv");
// 		igPV5 = shape_get_data_value_integer(html, oSource, "Bypass_tagname4", "cp_pv");
// 		igPV6 = shape_get_data_value_integer(html, oSource, "Bypass_tagname5", "cp_pv");
// 		igPV7 = shape_get_data_value_integer(html, oSource, "Bypass_tagname6", "cp_pv");
// 		igPV8 = shape_get_data_value_integer(html, oSource, "Bypass_tagname7", "cp_pv");
// 		igPV9 = shape_get_data_value_integer(html, oSource, "Bypass_tagname8", "cp_pv");
// 		igPV10 = shape_get_data_value_integer(html, oSource, "Bypass_tagname9", "cp_pv");
// 		igPV11 = shape_get_data_value_integer(html, oSource, "Bypass_tagname10", "cp_pv");
// 		igPV12 = shape_get_data_value_integer(html, oSource, "Bypass_tagname11", "cp_pv");
// 		igPV13 = shape_get_data_value_integer(html, oSource, "Bypass_tagname12", "cp_pv");
// 		igPV14 = shape_get_data_value_integer(html, oSource, "Bypass_tagname13", "cp_pv");
// 		igPV15 = shape_get_data_value_integer(html, oSource, "Bypass_tagname14", "cp_pv");

// 		long igPVx = 0;
// 		if (igPV1 == 1 || igPV2 == 1 || igPV3 == 1 || igPV4 == 1 || igPV5 == 1 || igPV6 == 1 || igPV7 == 1 || igPV8 == 1 || igPV9 == 1 || igPV10 == 1 || igPV11 == 1 || igPV12 == 1 || igPV13 == 1 || igPV14 == 1 || igPV15 == 1  	)
// 			igPVx = 1;
// 		char prop[32]; sprintf_s(prop, "cp_styleForBP%d", igPVx);
// 		const char * index = get_alias_string(html, oShape, prop);
// 		char classname[32]; sprintf_s(classname, "Enclav_TxtBypass_%s", index);
// 		set_class(html, shape_get_object(html, oShape, "TxtBypass"), find_class(classname));
// 		sprintf_s(prop, "cp_TxtForPV%d", igPVx);
// 		index = get_alias_string(html, oShape, prop);
// 		sprintf_s(shape_get_object(html, oShape, "TxtBypass")->m_szScriptValue, "%s", index);
// 	}
// 	/* DONE */
// }
// void VBSCRIPT_Scd_Khabarovsk_Description_onupdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	Dim iDesc
// 	on error resume next
	
// 	set oShape=oSource.parentnode.parentnode
// 	iDesc=oSource.DataValue("Tagname.cp_Desc")
// 	oShape.objects("TxtDescription").styleclass="Enclav_TxtDescription"
// 	oShape.objects("TxtDescription").innertext="   " & iDesc

// 	if err.number<>0 then window.external.LogMessage "Scd_Khabarovsk_Description_onupdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	char * iDesc = shape_get_data_value_string(html, oSource, "Tagname", "cp_Desc");
// 	set_class(html, oShape, find_class("Enclav_TxtDescription"));
// 	sprintf_s(shape_get_object(html, oShape, "TxtDescription")->m_szScriptValue, "   %s", iDesc);
// 	/* DONE */
// }
// void VBSCRIPT_Butt_Page_OnShapeLoad(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// on error resume next
// set txtName = oSource.objects("TxtName")
// set txtEUDesc = oSource.objects("TxtEUDesc")
// if typename(oSource.objects("ConAlarmState"))<>"Nothing" then
// 	sTgNm=oSource.objects("ConAlarmState").datavalue("tagname.name")
// elseif oSource.objects("ScConAlpPV").dataexists("tagname.name") then
// 	sTgNm=oSource.objects("ScConAlpPV").datavalue("tagname.name")
// else
// 	sTgNm=oSource.getcustomproperty("Point","Tagname")
// end if


// if sShow="Name" then
//   txtName.style.visibility="inherit"
//   txtEUDesc.style.visibility="hidden"
// elseif sShow="EngU" then
//   txtName.style.visibility="hidden"
//   txtEUDesc.style.visibility="inherit"
// else
//   txtName.style.visibility="hidden"
//   txtEUDesc.style.visibility="hidden"
// end if

// sTagEU = oSource.getcustomproperty("Text","cp_TagnameEU")


// if sTagEU <> "-" then
// 	if sTagEU<>"" then
// 		if instr(sTagEU,",")<>0 then
// 			sTag = left(sTagEU,instr(sTagEU,",")-1)
// 			sEU	= ltrim(mid(sTagEU,instr(sTagEU,",")+1))
// 			if sTag = "-" and sEU = "" then iSw=2
// 			if sTag = "-" and (sEU <> "" and sEU <> "-") then iSw=3
// 			if sTag = "" and sEU = "-" then iSw=4
// 			if sTag = "" and (sEU <> "" and sEU <> "-") then iSw=6
// 			if (sTag <> "" and sTag <> "-") and sEU = "-" then iSw=7
// 			if (sTag <> "" and sTag <> "-") and sEU = "" then iSw=8
// 			if (sTag <> "" and sTag <> "-") and (sEU <> "" and sEU <> "-") then iSw=9
// 		else
// 			iSw=7
// 			sTag=sTagEU
// 		end if
// 	else
// 		iSw=5
// 	end if
// else
// 	iSw=1
// end if
			
// select case iSw
// 	case 1:
// 		txtName.style.visibility="hidden"
//   		txtEUDesc.style.visibility="hidden"
// 	case 2:
// 		txtName.innertext=""
// 		txtEUDesc.innertext=oSource.Objects("ScConAlpPV").DataValue("tagname.cp_eudesc")
//     case 3:
// 		txtName.innertext=""
// 		txtEUDesc.innertext=sEU
// 	case 4:
// 		txtName.innertext=sTgNm
// 		txtEUDesc.innertext=""
// 	case 5:
// 		txtName.innertext=sTgNm
// 		txtEUDesc.innertext=oSource.Objects("ScConAlpPV").DataValue("tagname.cp_eudesc")
// 	case 6:
// 		txtName.innertext=sTgNm
// 		txtEUDesc.innertext=sEU
// 	case 7:
// 		txtName.innertext= sTag
// 		txtEUDesc.innertext=""
// 	case 8:
// 		txtName.innertext=sTag
// 		txtEUDesc.innertext=oSource.Objects("ScConAlpPV").DataValue("tagname.cp_eudesc")
// 	case 9:
// 		txtName.innertext=sTag
// 		txtEUDesc.innertext=sEU
// end select
// */
// 	//return;
// 	//if (!IsKhabarovsk()) return;
// 	if (!oSource->m_bIsShape) return;

// 	EShowType95 sShow = html.m_eShow95;

// 	CTag* txtName = shape_get_object(html, oSource, "TxtName");
// 	CTag* txtEUDesc = shape_get_object(html, oSource, "TxtEUDesc");

// 	SParamValueH s = shape_get_data_value(html, oSource, "tagname", "name");
// 	const char * sTgNm = 0;

// 	if (shape_contains_object(html, oSource, "ConAlarmState"))
// 	{
// 		sTgNm = shape_get_data_value_string(html, shape_get_object(html, oSource, "ConAlarmState"), "tagname", "name");
// 		if (!sTgNm) sTgNm = Alias(html, oSource, "Tagname");
// 	}
// 	else if (shape_get_data_value(html, oSource, "tagname", "name").nNumber >= 0)
// 	{
// 		sTgNm = shape_get_data_value_string(html, shape_get_object(html, oSource, "ScConAlpPV"), "tagname", "name");
// 	}
// 	else
// 	{
// 		sTgNm = Alias(html, oSource, "Tagname");
// 	}
// 	//
//     if(sTgNm == NULL || strcmp(sTgNm, "Tagname") == 0)
//        return;
// 	//
// 	if (sShow == est95Name)
// 	{
// 		if (txtName) txtName->Style(html).m_eVisible = enumInherit;
// 		if (txtEUDesc) txtEUDesc->Style(html).m_eVisible = enumHidden;
// 	}
// 	else if (sShow == est95EngEU)
// 	{
// 		if (txtName) txtName->Style(html).m_eVisible = enumHidden;
// 		if (txtEUDesc) txtEUDesc->Style(html).m_eVisible = enumInherit;
// 	}
// 	else
// 	{
// 		//if (txtName) txtName->Style(html).m_eVisible = enumHidden;
// 		//if (txtEUDesc) txtEUDesc->Style(html).m_eVisible = enumHidden;
// 	}
// 	//
// 	int iSw = 0;
// 	char sTag[128];
// 	char sEU[128];
// 	const char* sTagEU = Alias(html, oSource, "cp_TagnameEU");
//     if(strcmp(sTagEU, "cp_TagnameEU") == 0)
//        return;

// 	if (strcmp("-", sTagEU))
// 	{
// 		if (*sTagEU != 0 && strcmp("cp_TagnameEU", sTagEU))
// 		{
// 			int p = (int)(strchr(sTagEU, ',') - sTagEU);
// 			if (p >= 0)
// 			{
// 				strncpy_s(sTag, sTagEU, p); // sTag = left(sTagEU,instr(sTagEU,",")-1)
// 				lstrcpy(sEU, sTagEU+p+1); // sEU	= ltrim(mid(sTagEU,instr(sTagEU,",")+1))
//                 iSw=9;
// 				// if sTag = "-" and sEU = "" then iSw=2
// 				// if sTag = "-" and (sEU <> "" and sEU <> "-") then iSw=3
// 				// if sTag = "" and sEU = "-" then iSw=4
// 				// if sTag = "" and (sEU <> "" and sEU <> "-") then iSw=6
// 				// if (sTag <> "" and sTag <> "-") and sEU = "-" then iSw=7
// 				// if (sTag <> "" and sTag <> "-") and sEU = "" then iSw=8
// 				// if (sTag <> "" and sTag <> "-") and (sEU <> "" and sEU <> "-") then iSw=9
// 			}
// 			else
// 			{
// 				iSw = 7;
// 				lstrcpy(sTag, sTagEU);
// 			}
// 		}
// 		else iSw = 5;
// 	}
// 	else iSw = 1;

// 	//
// 	switch(iSw)
// 	{
// 	case 1:
// 		if (txtName) txtName->Style(html).m_eVisible = enumHidden;
// 		if (txtEUDesc) txtEUDesc->Style(html).m_eVisible = enumHidden;
// 		break;
// 	case 2:
// 		if (txtName) sprintf_s(txtName->m_szScriptValue, "");
// 		if (txtEUDesc)
// 		{
// 			char* value = shape_get_data_value_string(html, shape_get_object(html, oSource, "ScConAlpPV"), "tagname", "cp_eudesc");
// 			sprintf_s(txtEUDesc->m_szScriptValue, "%s", value);
// 		}
// 		break;
// 	case 3:
// 		if (txtName) sprintf_s(txtName->m_szScriptValue, "");
// 		if (txtEUDesc) sprintf_s(txtEUDesc->m_szScriptValue, "%s", sEU);
// 		break;
// 	case 4:
// 		if (txtName) sprintf_s(txtName->m_szScriptValue, "%s", sTgNm);
// 		if (txtEUDesc) sprintf_s(txtEUDesc->m_szScriptValue, "");
// 		break;
// 	case 5:
// 		if (txtName)
//         {
//             sprintf_s(txtName->m_szScriptValue, "%s", sTgNm);
//             CStyle& style = txtName->Style(html);
// #ifdef _DEBUG
//             const char *id = style.NameID( html );
// #endif
//             //style.align &= ~DT_RIGHT;
//         }
// 		if (txtEUDesc)
// 		{
// 			char* value = shape_get_data_value_string(html, shape_get_object(html, oSource, "ScConAlpPV"), "tagname", "cp_eudesc");
// 			if (value) sprintf_s(txtEUDesc->m_szScriptValue, "%s", value);
// 			else txtEUDesc->m_szScriptValue[0] = 0;
// 		}
// 		break;
// 	case 6:
// 		if (txtName) sprintf_s(txtName->m_szScriptValue, "%s", sTgNm);
// 		if (txtEUDesc) sprintf_s(txtEUDesc->m_szScriptValue, "%s", sEU);
// 		break;
// 	case 7:
// 		if (txtName) sprintf_s(txtName->m_szScriptValue, "%s", sTag);
// 		if (txtEUDesc) sprintf_s(txtEUDesc->m_szScriptValue, "");
// 		break;
// 	case 8:
// 		if (txtName) sprintf_s(txtName->m_szScriptValue, "%s", sTag);
// 		if (txtEUDesc)
// 		{
// 			char* value = shape_get_data_value_string(html, shape_get_object(html, oSource, "ScConAlpPV"), "tagname", "cp_eudesc");
// 			sprintf_s(txtEUDesc->m_szScriptValue, "%s", value);
// 		}
// 		break;
// 	case 9:
// 		if (txtName) sprintf_s(txtName->m_szScriptValue, "%s", sTag);
// 		if (txtEUDesc) sprintf_s(txtEUDesc->m_szScriptValue, "%s", sEU);
// 		break;
// 	}
// }
// void VBSCRIPT_RP1516CB_TextBox_UpdateScript(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*	if shape015_textbox001.QualityGood("ZSL.PV") = True and shape015_textbox001.QualityGood("ZSH.PV") = True then
// 		if shape015_textbox001.DataValue("ZSL.PV") = 1 and shape015_textbox001.DataValue("ZSH.PV") = 0 then
// 			shape015_textbox001.textColor = vbGreen
// 			shape015_textbox001.value = "¬ Àﬁ◊≈Õ"
// 		else
// 			if shape015_textbox001.DataValue("ZSL.PV") = 0 and shape015_textbox001.DataValue("ZSH.PV") = 1 then
// 				shape015_textbox001.textColor = vbRed
// 				shape015_textbox001.value = "¬€ Àﬁ◊≈Õ"
// 			end if
// 		end if
// 	else
// 		shape015_textbox001.textColor = vbYellow
// 		shape015_textbox001.value = "Œÿ»¡ ¿"
// 	end if
// */
// 	long zsl = shape_get_data_value_integer(html, oSource, "ZSL", "PV");
// 	long zsh = shape_get_data_value_integer(html, oSource, "ZSH", "PV");

// 	if (zsl == 1 && zsh == 0)
// 	{
// 		sprintf_s(shape_get_object(html, oSource, "textbox001")->m_szScriptValue, "¬ Àﬁ◊≈Õ");
// 		html.Style(shape_get_object(html, oSource, "textbox001")->m_nStyle).m_clrText = 0xff00;
// 	}
// 	else if (zsl == 0 && zsh == 1)
// 	{
// 		sprintf_s(shape_get_object(html, oSource, "textbox001")->m_szScriptValue, "¬€ Àﬁ◊≈Õ");
// 		html.Style(shape_get_object(html, oSource, "textbox001")->m_nStyle).m_clrText = 0xff;
// 	}
// 	else
// 	{
// 		sprintf_s(shape_get_object(html, oSource, "textbox001")->m_szScriptValue, "Œÿ»¡ ¿");
// 		html.Style(shape_get_object(html, oSource, "textbox001")->m_nStyle).m_clrText = 0xffff;
// 	}/**/
// }
// void VBSCRIPT_Scd_Khabarovsk_InitAnalog_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// Sub Scd_Khabarovsk_InitAnalog_AlpPV_OnUpdate(oSource)
// 	Dim oShape
// 	on error resume next
	
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.DataValue("AlarmTagname.cp_pv")
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	oShape.objects("RecFrame").styleclass="Enclav_InitAnl_" & index

// 	if err.number<>0 then window.external.LogMessage "Scd_Khabarovsk_InitAnalog_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// end sub
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = shape_get_data_value_integer(html, oShape, "AlarmTagname", "cp_pv");
// 	if (iPV < 0) return;
// 	char prop[64]; sprintf_s(prop, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, prop);
// 	char classname[128]; sprintf_s(classname, "Enclav_InitAnl_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecFrame"), find_class(classname));
// }
// // Scd_Khabarovsk_Actuador_AlpPV_OnUpdate_me_
// void VBSCRIPT_Scd_Khabarovsk_Actuador_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	Dim oShape, txt, txt_system
// 	on error resume next
	
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.value
// 	txt=oShape.getCustomProperty("text","cp_Desc")
// 	txt_system =oSource.Datavalue("tagname.description")
// 	If txt = "-" Then
// 		oShape.objects("TxtDescription").innertext = txt_system
// 	Else
// 		oShape.objects("TxtDescription").innertext = txt
// 	End If
		
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	oShape.objects("RecFrame1").styleclass="Enclav_Actuador_" & index
// 	oShape.objects("RecFrame2").styleclass="Enclav_Actuador_" & index

// 	if err.number<>0 then window.external.LogMessage "Scd_Khabarovsk_Actuador_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
	
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	long iPV = get_value_integer(oSource, html);
// 	const char * txt = Alias(html, oShape, "cp_Desc");
// 	char * txt_system = shape_get_data_value_string(html, oSource, "tagname", "desc");
// 	if (strcmp(txt, "-") == 0 )
// 	{
// 		if (txt_system && *txt_system) sprintf_s(shape_get_object(html, oShape, "TxtDescription")->m_szScriptValue, "%s", txt_system);
// 	}
// 	else
// 	{
// 		if (txt && *txt) sprintf_s(shape_get_object(html, oShape, "TxtDescription")->m_szScriptValue, "%s", txt);
// 	}
// 	if (iPV < 0) return;
// 	char prop[64]; sprintf_s(prop, "cp_styleForPV%d", iPV);
// 	const char * index = Alias(html, oShape, prop);
// 	char classname[64]; sprintf_s(classname, "Enclav_Actuador_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecFrame1"), find_class(classname));
// 	set_class(html, shape_get_object(html, oShape, "RecFrame2"), find_class(classname));
// }
// void VBSCRIPT_Scd_Khabarovsk_InitDigital_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	Dim oShape
// 	on error resume next
	
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.DataValue("Tagname.cp_pv")
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	oShape.objects("RecFrame").styleclass="Enclav_InitDig_" & index
// */
// 	//set oShape=oSource.parentnode.parentnode
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	//iPV=oSource.DataValue("Tagname.cp_pv")
// 	long iPV = shape_get_data_value_integer(html, oShape, "Tagname", "cp_pv");
// 	if (iPV < 0) return;
// 	if (oSource->mVal.def->dwLog > 40000)
// 	{
// 		char prop[16]; sprintf_s(prop, "STATEDES%d", iPV);
// 		char * val = shape_get_data_value_string(html, oShape, "Tagname", prop);
// 		sprintf_s(oSource->m_szScriptValue, "%s", val);
// 	}
// 	//index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	char prop[64]; sprintf_s(prop, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, prop);
// 	//oShape.objects("RecFrame").styleclass="Enclav_InitDig_" & index
// 	char classname[128]; sprintf_s(classname, "Enclav_InitDig_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecFrame"), find_class(classname));
// }
// void VBSCRIPT__RecPV1_0140213(CTag* oSource, HDC hDC, CHTML& html)
// {
// 	/*
// dim iPV1,iPV2,iPV3,iPV4,iPV5,iPV6
//   iPV1 = me.DataValue("014AA0005A.FLAGA.PVFL")
//   iPV2 = me.DataValue("014AA0005B.FLAGA.PVFL")
//   iPV3 = me.DataValue("014AA0005C.FLAGA.PVFL")
//   iPV4 = me.DataValue("014AA0005D.FLAGA.PVFL")
//   iPV5 = me.DataValue("014AA0006A.FLAGA.PVFL")
//   iPV6 = me.DataValue("014AA0006C.FLAGA.PVFL")
// if (iPV1 = 0)  or (iPV2 = 0) or (iPV3 = 0) or (iPV4 = 0) or (iPV5 = 0) or (iPV6 = 0) then
//  RecPV1.fillColor = "#FF0000"
//  else RecPV1.fillColor = "#C0C0C0"
//  end if
// 	*/
// 	int iPV1 = shape_get_data_value_integer(html, oSource, "014AA0005A.FLAGA", "PVFL");
// 	int iPV2 = shape_get_data_value_integer(html, oSource, "014AA0005B.FLAGA", "PVFL");
// 	int iPV3 = shape_get_data_value_integer(html, oSource, "014AA0005C.FLAGA", "PVFL");
// 	int iPV4 = shape_get_data_value_integer(html, oSource, "014AA0005D.FLAGA", "PVFL");
// 	int iPV5 = shape_get_data_value_integer(html, oSource, "014AA0006A.FLAGA", "PVFL");
// 	int iPV6 = shape_get_data_value_integer(html, oSource, "014AA0006C.FLAGA", "PVFL");
// 	if (iPV1 == 0 || iPV2 == 0 || iPV3 == 0 || iPV4 == 0 || iPV5 == 0 || iPV6 == 0)
// 		shape_get_object(html, oSource->m_pPar, "RecPV1")->Style(html).m_clrFill = 0xFF;
// 	else
// 		shape_get_object(html, oSource->m_pPar, "RecPV1")->Style(html).m_clrFill = 0xC0C0C0;
// }
// void VBSCRIPT__RecPV2_0140213(CTag* oSource, HDC hDC, CHTML& html)
// {
// 	/*
// dim iPV1,iPV2,iPV3,iPV4,iPV5,iPV6
//   iPV1 = me.DataValue("014AA0007A.FLAGA.PVFL")
//   iPV2 = me.DataValue("014AA0007B.FLAGA.PVFL")
//   iPV3 = me.DataValue("014AA0007C.FLAGA.PVFL")
//   iPV4 = me.DataValue("014AA0007D.FLAGA.PVFL")
//   iPV5 = me.DataValue("014AA0008A.FLAGA.PVFL")
//   iPV6 = me.DataValue("014AA0008C.FLAGA.PVFL")
// if (iPV1 = 0)  or (iPV2 = 0) or (iPV3 = 0) or (iPV4 = 0) or (iPV5 = 0) or (iPV6 = 0) then
//  RecPV2.fillColor = "#FF0000"
//  else RecPV2.fillColor = "#C0C0C0"
//  end if
// 	*/
// 	int iPV1 = shape_get_data_value_integer(html, oSource, "014AA0007A.FLAGA", "PVFL");
// 	int iPV2 = shape_get_data_value_integer(html, oSource, "014AA0007B.FLAGA", "PVFL");
// 	int iPV3 = shape_get_data_value_integer(html, oSource, "014AA0007C.FLAGA", "PVFL");
// 	int iPV4 = shape_get_data_value_integer(html, oSource, "014AA0007D.FLAGA", "PVFL");
// 	int iPV5 = shape_get_data_value_integer(html, oSource, "014AA0008A.FLAGA", "PVFL");
// 	int iPV6 = shape_get_data_value_integer(html, oSource, "014AA0008C.FLAGA", "PVFL");
// 	if (iPV1 == 0 || iPV2 == 0 || iPV3 == 0 || iPV4 == 0 || iPV5 == 0 || iPV6 == 0)
// 		shape_get_object(html, oSource->m_pPar, "RecPV2")->Style(html).m_clrFill = 0xFF;
// 	else
// 		shape_get_object(html, oSource->m_pPar, "RecPV2")->Style(html).m_clrFill = 0xC0C0C0;
// }
// void VBSCRIPT__RecPV3_0140213(CTag* oSource, HDC hDC, CHTML& html)
// {
// 	/*
// dim iPV1,iPV2,iPV3,iPV4,iPV5,iPV6
//   iPV1 = me.DataValue("014AA0009A.FLAGA.PVFL")
//   iPV2 = me.DataValue("014AA0009B.FLAGA.PVFL")
//   iPV3 = me.DataValue("014AA0009C.FLAGA.PVFL")
//   iPV4 = me.DataValue("014AA0009D.FLAGA.PVFL")
//   iPV5 = me.DataValue("014AA0010A.FLAGA.PVFL")
//   iPV6 = me.DataValue("014AA0010C.FLAGA.PVFL")
// if (iPV1 = 0)  or (iPV2 = 0) or (iPV3 = 0) or (iPV4 = 0) or (iPV5 = 0) or (iPV6 = 0) then
//  RecPV3.fillColor = "#FF0000"
//  else RecPV3.fillColor = "#C0C0C0"
//  end if
// 	*/
// 	int iPV1 = shape_get_data_value_integer(html, oSource, "014AA0009A.FLAGA", "PVFL");
// 	int iPV2 = shape_get_data_value_integer(html, oSource, "014AA0009B.FLAGA", "PVFL");
// 	int iPV3 = shape_get_data_value_integer(html, oSource, "014AA0009C.FLAGA", "PVFL");
// 	int iPV4 = shape_get_data_value_integer(html, oSource, "014AA0009D.FLAGA", "PVFL");
// 	int iPV5 = shape_get_data_value_integer(html, oSource, "014AA0010A.FLAGA", "PVFL");
// 	int iPV6 = shape_get_data_value_integer(html, oSource, "014AA0010C.FLAGA", "PVFL");
// 	if (iPV1 == 0 || iPV2 == 0 || iPV3 == 0 || iPV4 == 0 || iPV5 == 0 || iPV6 == 0)
// 		shape_get_object(html, oSource->m_pPar, "RecPV3")->Style(html).m_clrFill = 0xFF;
// 	else
// 		shape_get_object(html, oSource->m_pPar, "RecPV3")->Style(html).m_clrFill = 0xC0C0C0;
// }
// void VBSCRIPT__RecPV1_0140220(CTag* oSource, HDC hDC, CHTML& html)
// {
// 	/*
// dim iPV1,iPV2,iPV3,iPV4,iPV5,iPV6
//   iPV1 = me.DataValue("014AA0011A.FLAGA.PVFL")
//   iPV2 = me.DataValue("014AA0011B.FLAGA.PVFL")
//   iPV3 = me.DataValue("014AA0011C.FLAGA.PVFL")
//   iPV4 = me.DataValue("014AA0011D.FLAGA.PVFL")

// if (iPV1 = 0)  or (iPV2 = 0) or (iPV3 = 0) or (iPV4 = 0) then
//  RecPV1.fillColor = "#FF0000"
//  else RecPV1.fillColor = "#DDDDDD"
//  end if
// 	*/
// 	int iPV1 = shape_get_data_value_integer(html, oSource, "014AA0011A.FLAGA", "PVFL");
// 	int iPV2 = shape_get_data_value_integer(html, oSource, "014AA0011B.FLAGA", "PVFL");
// 	int iPV3 = shape_get_data_value_integer(html, oSource, "014AA0011C.FLAGA", "PVFL");
// 	int iPV4 = shape_get_data_value_integer(html, oSource, "014AA0011D.FLAGA", "PVFL");
// 	if (iPV1 == 0 || iPV2 == 0 || iPV3 == 0 || iPV4 == 0)
// 		shape_get_object(html, oSource->m_pPar, "RecPV1")->Style(html).m_clrFill = 0xFF;
// 	else
// 		shape_get_object(html, oSource->m_pPar, "RecPV1")->Style(html).m_clrFill = 0xDDDDDD;
// }
// void VBSCRIPT__TxtMode001(CTag* oSource, HDC hDC, CHTML& html)
// {
// 	/*
// dim iPV
//  iPV= TxtMode001.DataValue("014HU0039.numerica.pv")
// select case iPV
// 	case 1 me.innertext=" «»ÃÕ»… ƒ»«≈À‹"
// 	case 2 me.innertext=" À≈“Õ»… ƒ»«≈À‹"
// 	case 3 me.innertext=" «»ÃÕ»… + À≈“Õ»… ƒ»«≈À‹"
// 	case 4 me.innertext="  ≈–Œ—»Õ + «»ÃÕ»… ƒ»«≈À‹"
// 	end select
// 	*/
// 	int iPV = shape_get_data_value_integer(html, oSource, "014HU0039.numerica", "PV");
// 	oSource->Style(html).align |= DT_WORDBREAK;
// 	switch(iPV)
// 	{
// 	case 1: oSource->m_constValue = " «»ÃÕ»… ƒ»«≈À‹"; break;
// 	case 2: oSource->m_constValue = " À≈“Õ»… ƒ»«≈À‹"; break;
// 	case 3: oSource->m_constValue = " «»ÃÕ»… + À≈“Õ»… ƒ»«≈À‹"; break;
// 	case 4: oSource->m_constValue = "  ≈–Œ—»Õ + «»ÃÕ»… ƒ»«≈À‹"; break;
// 	}
// }

// void VBSCRIPT_Scd_OnOffValves_Khabarovsk_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim oShape,iPV1,iPV2,indexClose,indexOpen,indexAlarm
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV1=oSource.datavalue("tagname1.cp1_pv")
// 	iPV2=oSource.datavalue("tagname2.cp2_pv")
// 	indexClose=oShape.getCustomProperty("Style","cp_StyleForClose")
// 	indexOpen=oShape.getCustomProperty("Style","cp_StyleForOpen")
// 	indexAlarm=oShape.getCustomProperty("Style","cp_StyleForAlarm")

// 	if iPV2 = 1 then
// 		oShape.objects("PlgIn").styleclass="PlgIn_" & indexAlarm
// 		oShape.objects("PlgOut").styleclass="PlgOut_" & indexAlarm
// 	elseif iPV1 = 0 then
// 		oShape.objects("PlgIn").styleclass="PlgIn_" & indexClose
// 		oShape.objects("PlgOut").styleclass="PlgOut_" & indexClose
// 	elseif iPV1 = 1 then
// 		oShape.objects("PlgIn").styleclass="PlgIn_" & indexOpen
// 		oShape.objects("PlgOut").styleclass="PlgOut_" & indexOpen
// 	end if

// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_OnOffValves_Khabarovsk_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	int iPV1 = shape_get_data_value_integer(html, oSource, "tagname1", "cp1_pv");
// 	int iPV2 = shape_get_data_value_integer(html, oSource, "tagname2", "cp2_pv");

// 	const char * indexClose = Alias(html, oShape, "cp_StyleForClose");
// 	const char * indexOpen = Alias(html, oShape, "cp_StyleForOpen");
// 	const char * indexAlarm = Alias(html, oShape, "cp_StyleForAlarm");

// 	if (iPV2 == 1)
// 	{
// 		char classname[64]; sprintf_s(classname, "PlgIn_%s", indexAlarm);
// 		set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 		sprintf_s(classname, "PlgOut_%s", indexAlarm);
// 		set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));
// 	}
// 	else if (iPV1 == 0)
// 	{
// 		char classname[64]; sprintf_s(classname, "PlgIn_%s", indexClose);
// 		set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 		sprintf_s(classname, "PlgOut_%s", indexClose);
// 		set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));
// 	}
// 	else if (iPV1 == 1)
// 	{
// 		char classname[64]; sprintf_s(classname, "PlgIn_%s", indexOpen);
// 		set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 		sprintf_s(classname, "PlgOut_%s", indexOpen);
// 		set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));
// 	}
// }
// void VBSCRIPT_Scd_RegCntlValve_Khabarovsk_AlpVal_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim rVal,oShape,iPV1,iPV2,iPV3,indexClose,indexOpen,indexAlarm
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV1=oSource.datavalue("tagname1.cp1_pv")
// 	iPV2=oSource.datavalue("tagname2.cp2_pv")
// 	iPV3=oSource.datavalue("tagname3.cp3_pv")
// 	indexClose=oShape.getCustomProperty("Style","cp_StyleForClose")
// 	indexOpen=oShape.getCustomProperty("Style","cp_StyleForOpen")
// 	indexAlarm=oShape.getCustomProperty("Style","cp_StyleForAlarm")

// 	rVal=round(iPV3,0)
// 	if rVal>100 then
//   		rVal=100
// 	elseif rVal<0 then
//   		rVal=0
// 	end if

// 	if iPV2 = 1 then
// 		oShape.objects("PlgIn").styleclass="PlgIn_" & indexAlarm
// 		oShape.objects("PlgOut").styleclass="PlgOut_" & indexAlarm
// 	elseif iPV1 = 0 then
// 		oShape.objects("PlgIn").styleclass="PlgIn_" & indexClose
// 		oShape.objects("PlgOut").styleclass="PlgOut_" & indexClose
// 	elseif iPV1 = 1 then
// 		oShape.objects("PlgIn").styleclass="PlgIn_" & indexOpen
// 		oShape.objects("PlgOut").styleclass="PlgOut_" & indexOpen
// 	end if

// 	oShape.objects("TxtOP").innertext=rVal & "%"
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	int iPV1 = shape_get_data_value_integer(html, oSource, "tagname1", "cp1_pv");
// 	int iPV2 = shape_get_data_value_integer(html, oSource, "tagname2", "cp2_pv");
// 	double iPV3 = shape_get_data_value_double(html, oSource, "tagname3", "cp3_pv");

// 	const char * indexClose = Alias(html, oShape, "cp_StyleForClose");
// 	const char * indexOpen = Alias(html, oShape, "cp_StyleForOpen");
// 	const char * indexAlarm = Alias(html, oShape, "cp_StyleForAlarm");

// 	int rVal = iPV3;
// 	if (rVal > 100) rVal = 100;
// 	else if (rVal < 0) rVal = 0;
	
// 	if (iPV2 == 1)
// 	{
// 		char classname[64]; sprintf_s(classname, "PlgIn_%s", indexAlarm);
// 		set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 		sprintf_s(classname, "PlgOut_%s", indexAlarm);
// 		set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));
// 	}
// 	else if (iPV1 == 0)
// 	{
// 		char classname[64]; sprintf_s(classname, "PlgIn_%s", indexClose);
// 		set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 		sprintf_s(classname, "PlgOut_%s", indexClose);
// 		set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));
// 	}
// 	else if (iPV1 == 1)
// 	{
// 		char classname[64]; sprintf_s(classname, "PlgIn_%s", indexOpen);
// 		set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 		sprintf_s(classname, "PlgOut_%s", indexOpen);
// 		set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));
// 	}
// 	CTag * txtOP = shape_get_object(html, oShape, "TxtOP");
// 	sprintf_s(txtOP->m_szScriptValue, "%d%%", rVal);
// }
// void __HWLineHeaterXX(CTag* oSource, HDC hDC, CHTML& html, const char * _point)
// {
// 	char point[64]; lstrcpy(point, _point);
// 	char * tag = point;
// 	char * param = tag + 11;
// 	tag[10] = 0;
// 	int iPV = shape_get_data_value_integer(html, oSource, tag, param);
// 	if (iPV == 1) oSource->Style(html).m_clrLine = 0xff;
// 	else oSource->Style(html).m_clrLine = 0;
// }
// void VBSCRIPT__HWLineHeater14_2500109(CTag* oSource, HDC hDC, CHTML& html) { __HWLineHeaterXX(oSource, hDC, html, "250XL0512B.PV"); }
// void VBSCRIPT__HWLineHeater13_2500109(CTag* oSource, HDC hDC, CHTML& html) { __HWLineHeaterXX(oSource, hDC, html, "250XL0511B.PV"); }
// void VBSCRIPT__HWLineHeater23_2500115(CTag* oSource, HDC hDC, CHTML& html) { __HWLineHeaterXX(oSource, hDC, html, "250XL0521B.PV"); }
// void VBSCRIPT__HWLineHeater24_2500115(CTag* oSource, HDC hDC, CHTML& html) { __HWLineHeaterXX(oSource, hDC, html, "250XL0522B.PV"); }
// void VBSCRIPT__HWLineHeater33_2500201(CTag* oSource, HDC hDC, CHTML& html) { __HWLineHeaterXX(oSource, hDC, html, "250XL0332B.PV"); }
// void VBSCRIPT_Scd_RegCntlValve_AlpVal_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// 	dim rVal,rClosed,oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	rVal=round(oSource.value,0)
// 	rClosed=csng(oShape.getcustomproperty("Num","cp_ClosedValue"))
// 	sRange = lcase(page.GetStyleClassProperty("UseExtended_RegValve_range","key"))
//     if sRange <>"yes" then
// 		if rVal>100 then
// 	  		rVal=100
// 		elseif rVal<0 then
// 	  		rVal=0
// 		end if
// 	end if
// 	if rVal > rClosed then
// 		index=oShape.getCustomProperty("Style","cp_styleForOpen" )
// 	else
// 		index=oShape.getCustomProperty("Style","cp_styleForClose" )
// 	end if
// 	if typename(oShape.objects("PlgIn")) <> "Nothing" then oShape.objects("PlgIn").styleclass="PlgIn_" & index
//     if typename(oShape.objects("PlgOut")) <> "Nothing" then oShape.objects("PlgOut").styleclass="PlgOut_" & index
// 	if oShape.objects("TxtOP") is nothing then
// 		oShape.objects("IndVAl").value=rVal
// 	else
// 		oShape.objects("TxtOP").innertext=rVal & "%"
// 	end if
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "Scd_RegCntlValve_AlpVal_OnUpdate ->" & oSource.id & " " & err.description
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
//     double rVal = get_value_double(oSource, html);
// 	double rClosed = get_alias_double(html, oSource, "cp_ClosedValue");
	
// 	if (rVal>100) rVal=100;
// 	else if (rVal<0) rVal=0;

// 	const char * index;
// 	if (rVal > rClosed)
// 		index = get_alias_string(html, oShape, "cp_styleForOpen");
// 	else
// 		index = get_alias_string(html, oShape, "cp_styleForClose");

// 	char classname[64]; sprintf_s(classname, "PlgIn_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgIn"), find_class(classname));
// 	sprintf_s(classname, "PlgOut_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "PlgOut"), find_class(classname));
	
// 	if (!shape_contains_object(html, oShape, "TxtOP"))
// 	{
// 		if (!_isnan(rVal))
// 			sprintf_s(shape_get_object(html, oShape, "IndVAl")->m_szScriptValue, "%.0f%%", rVal);
// 		else
// 			sprintf_s(shape_get_object(html, oShape, "IndVAl")->m_szScriptValue, "NaN");
// 	}
// 	else
// 	{
// 		if (!_isnan(rVal))
// 			sprintf_s(shape_get_object(html, oShape, "TxtOP")->m_szScriptValue, "%.0f%%", rVal);
// 		else
// 			sprintf_s(shape_get_object(html, oShape, "TxtOP")->m_szScriptValue, "NaN");
// 	}
	
// }

// void VBSCRIPT_Cda_CEXXX_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// Sub Cda_CEXXX_AlpPV_OnUpdate(oSource)
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.datavalue("Tagname.cp_pv")
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	oShape.objects("RecPV").styleclass="RecPV_" & index
// 	oShape.objects("TxtCText").styleclass="TxtCText_" & index
// 	if typename(oShape.objects("TxtPName"))<>"Nothing" then oShape.objects("TxtPName").styleclass="TxtCText_" & index
// 	iBlockAlarm = cint(oShape.getcustomproperty("Num","cp_BlockAlarm"))
// 	if iBlockAlarm = 1 then oShape.objects("RecPV").alarm=oSource.alarm'oSource.dataalarm("Tagname.cp_pv")
// 	if err.number<>0 and window.external.Application.environment = 0 then window.external.Application.LogMessage "Cda_CEXXX_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// end sub
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;

// 	int iPV = shape_get_data_value_integer(html, oSource, "Tagname", "cp_pv");

// 	char _temp[128];
// 	sprintf(_temp, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, _temp);

// 	char _style1[128]; sprintf(_style1, "RecPV_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecPV"), find_class(_style1));

// 	char _style2[128]; sprintf(_style2, "TxtCText_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "TxtCText"), find_class(_style2));

// 	if (shape_contains_object(html, oShape, "TxtPName"))
// 	{
// 		//set_class(html, shape_get_object(html, oShape, "TxtPName"), find_class(_style2));
// 	}

// 	//int iBlockAlarm = get_alias_int(html, oShape, "cp_BlockAlarm");
// }
// void VBSCRIPT_LK2B_Cda_CEXXX_AlpPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// Sub LK2B_Cda_CEXXX_AlpPV_OnUpdate(oSource)
// 	dim oShape
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	iPV=oSource.datavalue("Tagname.cp_pv")
// 	index=oShape.getCustomProperty("Style","cp_styleForPV" & iPV)
// 	oShape.objects("RecPV").styleclass="RecPV_" & index
// 	oShape.objects("TxtCText").styleclass="TxtCText_" & index
// 	if typename(oShape.objects("TxtPName"))<>"Nothing" then oShape.objects("TxtPName").styleclass="TxtCText_" & index
// 	'iBlockAlarm = cint(oShape.getcustomproperty("Num","cp_BlockAlarm"))
// 	'if iBlockAlarm = 1 then oShape.objects("RecPV").alarm=oSource.alarm '("Tagname.cp_pv")
// 	if err.number<>0 and window.external.Application.environment = 0 then window.external.Application.LogMessage "Cda_CEXXX_AlpPV_OnUpdate ->" & oSource.id & " " & err.description
// end sub
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;

// 	int iPV = shape_get_data_value_integer(html, oSource, "Tagname", "cp_pv");

// 	char _temp[128];
// 	sprintf(_temp, "cp_styleForPV%d", iPV);
// 	const char * index = get_alias_string(html, oShape, _temp);

// 	char _style1[128]; sprintf(_style1, "RecPV_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "RecPV"), find_class(_style1));

// 	char _style2[128]; sprintf(_style2, "TxtCText_%s", index);
// 	set_class(html, shape_get_object(html, oShape, "TxtCText"), find_class(_style2));

// 	if (shape_contains_object(html, oShape, "TxtPName"))
// 	{
// 		//set_class(html, shape_get_object(html, oShape, "TxtPName"), find_class(_style2));
// 	}
// }
// void VBSCRIPT_LK2B_Cda_CEXXX_ConAlpTP_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// sub LK2B_Cda_CEXXX_ConAlpTP_OnUpdate(oSource)

// 	dim stAlmType
	
// 	set oTxtAlarmType = oSource.parentnode.parentnode.objects("TxtAlarmType")
// 	stAlmType = oSource.parentElement.parentElement.getcustomproperty("Parameter","cp_pvtp")
// 	if stAlmType="DACA.PVHHALM.TP" then
// 		oTxtAlarmType.innerText="HH"
// 	elseif stAlmType="DACA.PVLLALM.TP" then
// 		oTxtAlarmType.innerText="LL"
// 	elseif stAlmType="DACA.PVLOALM.TP" then
// 		oTxtAlarmType.innerText="L"
// 	elseif stAlmType="DACA.PVHIALM.TP" then
// 		oTxtAlarmType.innerText="H"

// 	else
// 	oTxtAlarmType.innerText="--"
// 	end if

// end sub
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;

// 	CTag* oTxtAlarmType = shape_get_object(html, oShape, "TxtAlarmType");
// 	const char* stAlmType = get_alias_string(html, oTxtAlarmType, "cp_pvtp");

// 	if (!strcmp(stAlmType, "DACA.PVHHALM.TP"))
// 	{
// 		sprintf_s(oTxtAlarmType->m_szScriptValue, "HH");
// 	}
// 	else if (!strcmp(stAlmType, "DACA.PVLLALM.TP"))
// 	{
// 		sprintf_s(oTxtAlarmType->m_szScriptValue, "LL");
// 	}
// 	else if (!strcmp(stAlmType, "DACA.PVLOALM.TP"))
// 	{
// 		sprintf_s(oTxtAlarmType->m_szScriptValue, "L");
// 	}
// 	else if (!strcmp(stAlmType, "DACA.PVHIALM.TP"))
// 	{
// 		sprintf_s(oTxtAlarmType->m_szScriptValue, "H");
// 	}
// 	else
// 	{
// 		sprintf_s(oTxtAlarmType->m_szScriptValue, "--");
// 	}

// }
// void VBSCRIPT_LK2B_Cda_CEXXX_RecPV_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// sub LK2B_Cda_CEXXX_RecPV_OnUpdate(oSource)
    
// dim iAlmType, stAlmType

// 	iAlmType=oSource.datavalue("Tagname.DACA.HIALM.TYPE")
// 	stAlmType = oSource.parentElement.parentElement.getcustomproperty("Parameter","cp_pvtp")

// 	if stAlmType="DACA.PVHHALM.TP"  then
// 	select case iAlmType
// 	    case 2 oSource.fillColor=vbred			' HH
// 		case 0, 1, 3, 4, 5, 6 	oSource.fillColor="transparent"
//  	end select
// 	end if

// 	if stAlmType="DACA.PVLOALM.TP"  then
// 	select case iAlmType
// 	    case 3, 5 oSource.fillColor=vbyellow			' L
// 		case 0, 1, 2, 4, 6 	oSource.fillColor="transparent"
//  	end select
// 	end if

// 	if stAlmType="DACA.PVLLALM.TP" then
// 	select case iAlmType
// 	    case 3 oSource.fillColor=vbred			' LL
// 		case 0, 1, 2, 4, 5, 6 	oSource.fillColor="transparent"
//  	end select
// 	end if

// 	if stAlmType="DACA.PVHIALM.TP" then
// 	select case iAlmType
// 	    case 2, 4 oSource.fillColor=vbyellow			' H
// 		case 0, 1, 3, 5, 6 	oSource.fillColor="transparent"
//  	end select
// 	end if

	  
// end sub
// */
// 	int iAlmType = shape_get_data_value_integer(html, oSource, "tagname", "DACA.HIALM.TYPE");
// 	const char* stAlmType = get_alias_string(html, oSource->m_pPar->m_pPar, "cp_pvtp");

// 	if (!strcmp(stAlmType, "DACA.PVHHALM.TP"))
// 	{
// 		if (iAlmType == 2)
// 		{
// 			oSource->Style(html).m_clrFill = 0x0000ff;
// 		}
// 	}
// 	else if (!strcmp(stAlmType, "DACA.PVLOALM.TP"))
// 	{
// 		if (iAlmType == 3 && iAlmType == 5)
// 		{
// 			oSource->Style(html).m_clrFill = 0x00ffff;
// 		}
// 	}
// 	else if (!strcmp(stAlmType, "DACA.PVLLALM.TP"))
// 	{
// 		if (iAlmType == 3)
// 		{
// 			oSource->Style(html).m_clrFill = 0x0000ff;
// 		}
// 	}
// 	else if (!strcmp(stAlmType, "DACA.PVHIALM.TP"))
// 	{
// 		if (iAlmType == 2 && iAlmType == 4)
// 		{
// 			oSource->Style(html).m_clrFill = 0x00ffff;
// 		}
// 	}
// }
// void VBSCRIPT_UZ_Cda_Graph_AlpError_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// sub UZ_CDA_Graph_AlpError_OnUpdate(oSource)
// 	dim oShape, sOffScan, sBad, j
// 	on error resume next
// 	set oShape=oSource.parentnode.parentnode
// 	if oShape.getcustomproperty("Point","Tagname") <> "" then
// 		if instr(lcase(oSource.classname),"scriptdata")<>0 then
// 			for j=1 to oShape.customproperties.count-1
// 				Quality = 4
// 				if oShape.customproperties.Item(j).Type = "Parameter" and oSource.DataExists("Tagname." & oShape.customproperties.Item(j).Name) then
// 					Quality = oSource.DataQuality("Tagname." & oShape.customproperties.Item(j).Name)
// 					if (Quality and 8)=8 or(Quality and 4)=4 then
// 			      		sBad="Bad"
// 			    	end if
// 				end if
// 			next
// 		end if
// 		if (oSource.Quality and 4)=4 then
// 			sBad="Bad"
// 		end if
// 	else
// 		sBad="Bad"
// 	end if
// 	if 	sBad="Bad" then
//   		if typename(oShape.objects("TxtError"))<>"Nothing" then oShape.objects("TxtError").styleclass="TxtError_InError"
// 	else
//   		if typename(oShape.objects("TxtError"))<>"Nothing" then oShape.objects("TxtError").styleclass="TxtError_NoError"
// 	end if
// 	if err.number<>0 and window.external.Application.environment = 0 then window.external.Application.LogMessage "UZ_CDA_Graph_AlpError_OnUpdate ->" & oSource.id & " " & err.description
// end sub
// */
// 	char * sBad;
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	set_class(html, shape_get_object(html, oShape, "TxtError"), find_class("TxtError_NoError"));
// }

// void VBSCRIPT_All_ILBPGraph_AlpError_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// sub All_ILBPGraph_AlpError_OnUpdate(oSource)
// 	dim oShape
// 	on error resume next
//     set oShape=oSource.parentnode.parentnode
// 	if oShape.getcustomproperty("Point","Tag2_Inhibited")<>"" then
// 		iQuality2 =(oSource.dataquality("Tag2_Inhibited.cp_Tag2_pv") and iQualityGood)
// 	else
// 		iQuality2 = iQualityGood
// 	end if
//     if ((oSource.quality and iQualityGood)=iQualityGood) and (iQuality2=iQualityGood) then
//        oShape.objects("TxtError").styleclass="TxtError_NoError"
//     else
//        oShape.objects("TxtError").styleclass="TxtError_InError"
//     end if
//     if err.number<>0 and window.external.Application.environment = 0 then window.external.Application.LogMessage " Scd_ILBPGraph_AlpError_OnUpdate ->" & oSource.id & " " & err.description
// end sub
// */
// }

// void VBSCRIPT_CDA_Interlock_OnUpdate_me_(CTag* oSource, HDC hDC, CHTML& html)
// {
// /*
// sub CDA_Interlock_OnUpdate(oSource)
// 	dim oShape,IL,ILBypass,ILOUTBypass,ExtraCond,sReverse

// 	set oShape=oSource.parentnode.parentnode
// 	IL=oSource.value
// 	if (not isnumeric(IL)) or instr(IL,"N")<>0 then exit sub
// 	if IL=1 then
// 		oShape.objects("PlgInterlock").styleclass="PlgInterLock_Active"
// 	else
// 		oShape.objects("PlgInterlock").styleclass="PlgInterLock_Inactive"
// 	end if
// 	if oShape.getcustomProperty("Parameter","cp_bypass")<>"" then
// 		ILBypass=oSource.datavalue("tagname.cp_bypass")
// 		if (oSource.datavalue("tagname.cp_finalbypassperm") = 1) and (oSource.datavalue("tagname.cp_finalbypreq") <> "No_CMD") then
// 			ILOUTBypass=1
// 		else
// 			ILOUTBypass=0
// 		end if
// 		if ILBypass=1 then
// 			oShape.objects("LinBypass").styleclass="LinBypass_Active"
// 		else
// 			oShape.objects("LinBypass").styleclass="LinBypass_Inactive"
// 		end if
// 		if ILOutBypass=1 then
// 			oShape.objects("LinOutBypass").styleclass="LinOutBypass_Active"
// 		else
// 			oShape.objects("LinOutBypass").styleclass="LinOutBypass_Inactive"
// 		end if
// 	end if
// 	if err.number<>0 and window.external.environment = 0 then window.external.LogMessage "CDA_Interlock_OnUpdate ->" & oSource.id & " " & err.description
// end sub
// */
// 	CTag* oShape = oSource->m_pPar->m_pPar;
// 	if (oSource->mVal.def)
// 	{
// 		/* TODO */ if (oSource->mVal.def->eVal != enumValueChr) ;//__debugbreak();

// 		void *_IL = html.pipe.ParamValue_H(oSource->mVal);
// 		if (_IL)
// 		{
// 			char IL = *(char*)_IL;
// 			if ( IL == 1)
// 				set_class(html, shape_get_object(html, oShape, "PlgInterlock"), find_class("PlgInterLock_Active"));
// 			else
// 				set_class(html, shape_get_object(html, oShape, "PlgInterlock"), find_class("PlgInterLock_Inactive"));
// 		}
// 		else
// 		{
// 			set_class(html, shape_get_object(html, oShape, "PlgInterlock"), find_class("PlgInterLock_Inactive"));
// 		}

// 		if (shape_property_not_empty(html, oShape, "cp_bypass"))
// 		{
// 			int ILOUTBypass;
// 			int ILBypass = shape_get_data_value_integer(html, oSource, "tagname", "cp_bypass");
// 			if (shape_get_data_value_integer(html, oSource, "tagname", "cp_finalbypassperm") == 1
// 				&& strcmp(shape_get_data_value_string(html, oSource, "tagname", "cp_finalbypassperm"), "No_CMD"))
// 				ILOUTBypass = 1;
// 			else
// 				ILOUTBypass = 0;
// 			if (ILBypass == 1)
// 				set_class(html, shape_get_object(html, oShape, "LinBypass"), find_class("LinBypass_Active"));
// 			else
// 				set_class(html, shape_get_object(html, oShape, "LinBypass"), find_class("LinBypass_Inactive"));
// 			if (ILOUTBypass == 1)
// 				set_class(html, shape_get_object(html, oShape, "LinOutBypass"), find_class("LinOutBypass_Active"));
// 			else
// 				set_class(html, shape_get_object(html, oShape, "LinOutBypass"), find_class("LinOutBypass_Inactive"));
// 		}
// 	}
// }
