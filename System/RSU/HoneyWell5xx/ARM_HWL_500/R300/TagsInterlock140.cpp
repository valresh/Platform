#include "StdAfx.h"
#include "TagsInterlock140.h"
#include "../AplicFrm.h"

CTag* fnCM_FB_CXXINVERT(int n) { return new CTagCmFbCxxInvert(n); }
CTag* fnCM_FB_GXNUMINPUTS(int n) { return new CTagCmFbGxNumInputs(n); }
CTag* fnCM_FB_GXFIRSTCOND(int n) { return new CTagCmFbGxFistCond(n); }
CTag* fnCM_FB_GXSO(int n) { return new CTagCmFbGxSO(n); }
CTag* fnCM_FB_UPDETECTED(int n) { return new CTagCmFbUpDetected(n); }
CTag* fnCM_FB_FIRSTUP(int n) { return new CTagCmFbFirstUp(n); }
CTag* fnCM_FB_CXXSO(int n) { return new CTagCmFbCXXSo(n); }
CTag* fnCM_FB_CXXFL(int n) { return new CTagCmFbCXXFl(n); }
CTag* fnCM_FB_CXXByPreq(int n) { return new CTagCmFbCXXByPreq(n); }
CTag* fnCM_FB_GXAlgId(int n) { return new CTagCmFbGXAlgId(n); }
CTag* fnNotBindedCombo(int n) { return new CNotBindedCombo(n); }
CTag* fnCM_FB_CXXByPerm(int n) { return new CTagCmFbCXXByPerm(n); }

CTag* _get_object(CHTML* html, CTag* tag, char * id)
{
	char * _id = tag->Style(*html).NameID(*html);
	if (!strcmp(_id, id)) return tag;
	else
	{
		INT_PTR nCount = tag->GetSize();
		CTag** children = tag->GetData();
		for ( INT_PTR n = 0; n < nCount; n++ )
		{
			CTag* rslt = _get_object(html, children[n], id);
			if (rslt) return rslt;
		}
	}
	return NULL;
}
CTag* get_object(CHTML* html, char * id)
{
	return _get_object(html, html->mBody, id);
}


//int g_IECGates = 0;
//int g_CM_ArrayPointer = 0;
//int g_Gate1 = 99;
//int g_Gate2 = 99;
//int g_Gate3 = 99;
//int g_Gate4 = 99;
//bool g_GateError = false;
//bool g_SetComboboxValues = true;
//int g_DisplayErrorMessages = 0;
//bool g_UpdateAllowed = true;

void vbsSetNegator(CHTML* html, char snValue, int snNegator, char * snMsg)
{
/*
'called by:
'CM_FB_CxINVERT [event: onupdate]
function SetNegator(snValue, snNegator, snMsg)
	on error resume next
	if snValue = "1" then
		if g_IECGates = 0 then
			page.all("Negator" & snNegator).style.visibility = "visible"
			page.all("C" & snNegator & "FNeg").style.visibility = "hidden"
		else
			page.all("Negator" & snNegator).style.visibility = "hidden"
			page.all("C" & snNegator & "FNeg").style.visibility = "visible"
		end if
	elseif snValue = "0" then
		page.all("Negator" & snNegator).style.visibility = "hidden"
		page.all("C" & snNegator & "FNeg").style.visibility = "hidden"
	else
		page.all("Negator" & snNegator).style.visibility = "hidden"
		page.all("C" & snNegator & "FNeg").style.visibility = "hidden"
		if g_DisplayErrorMessages = 1 then
			call BadSignal(snMsg)
			g_MessageTimer = window.setInterval("ClearMessageBox()", g_GivenMessageTime)
		end if
	end if
end function
*/
	if (snValue == 1) 
	{
		//g_IECGates=0
		char tmp[64]; sprintf(tmp, "Negator%d", snNegator);
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		sprintf(tmp, "C%dFNeg", snNegator);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
	} 
	else if (snValue == 0) 
	{
		char tmp[64]; sprintf(tmp, "Negator%d", snNegator);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "C%dFNeg", snNegator);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
	} 
	else 
	{
		char tmp[64]; sprintf(tmp, "Negator%d", snNegator);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "C%dFNeg", snNegator);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
	}
}

void vbsNumInputsFirstcond(CHTML* html, int nifValue, char * nifMsg)
{
/*
'called by:
'CM_FB_GxNUMINPUTS [event: onupdate]
'CM_FB_GxFIRSTCOND [event: onupdate]
function NumInputsFirstcond(nifValue, nifMsg)
	on error resume next
	if CInt(nifValue) >= 0 and CInt(nifValue) <= 12 then
		if g_UpdateAllowed = true then
			call DrawAllObjects
		elseif g_UpdateAllowed = false and g_UpStreamClicked = true then
			g_UpStreamClicked = false
			g_UpdateTimer = window.setInterval ("clearUpdateTimer()", g_GivenUpdateTime)
		end if
	else
		g_GateError = true
		loadingText.style.visibility = "visible"
		if g_DisplayErrorMessages = 1 then
			call BadSignal(nifMsg)
			g_MessageTimer = window.setInterval("ClearMessageBox()", g_GivenMessageTime)
		end if
	end if
end function
*/
	if (nifValue >= 0 && nifValue <= 12)
	{
		vbsDrawAllObjects(html);
	}
}

const char * vbsGetGateLogic(CHTML * html, char State)
{
/*
'called by:
'DrawAllObjects()
'SetGateTitle()
function GetGateLogic(State)
	on error resume next
	if g_IECGates = 1 then
		select case State
			case 0		GetGateLogic = ""
						exit function
			case 1		GetGateLogic = "ON"
						exit function
			case 2		GetGateLogic = "OFF"
						exit function
			case 3		GetGateLogic = "&"
						exit function
			case 4		GetGateLogic = ChrW(8805) & "1"
						exit function
			case 5		GetGateLogic = "1"
						exit function
			case 6		GetGateLogic = "1"
						exit function
			case 7		GetGateLogic = ChrW(8805) & "1"
						exit function
			case 8		GetGateLogic = "&"
						exit function
			case 9		GetGateLogic = "=1"
						exit function
			case else	GetGateLogic = ""
						exit function
		end select
	else
		select case State
			case 0		GetGateLogic = "NO_ALG"
						exit function
			case 1		GetGateLogic = "ON"
						exit function
			case 2		GetGateLogic = "OFF"
						exit function
			case 3		GetGateLogic = "AND"
						exit function
			case 4		GetGateLogic = "OR"
						exit function
			case 5		GetGateLogic = "CONN"
						exit function
			case 6		GetGateLogic = "NOT"
						exit function
			case 7		GetGateLogic = "NOR"
						exit function
			case 8		GetGateLogic = "NAND"
						exit function
			case 9		GetGateLogic = "XOR"
						exit function
			case else	GetGateLogic = ""
						exit function
		end select
	end if
end function
*/
//'called by:
//'DrawAllObjects()
//'SetGateTitle()
//function GetGateLogic(State)
//	on error resume next
	if (html->mBody->m_strInterlockContainer.g_IECGates == 1){
//		select case State
//			case 0		GetGateLogic = ""
//						exit function
//			case 1		GetGateLogic = "ON"
//						exit function
//			case 2		GetGateLogic = "OFF"
//						exit function
//			case 3		GetGateLogic = "&"
//						exit function
//			case 4		GetGateLogic = ChrW(8805) & "1"
//						exit function
//			case 5		GetGateLogic = "1"
//						exit function
//			case 6		GetGateLogic = "1"
//						exit function
//			case 7		GetGateLogic = ChrW(8805) & "1"
//						exit function
//			case 8		GetGateLogic = "&"
//						exit function
//			case 9		GetGateLogic = "=1"
//						exit function
//			case else	GetGateLogic = ""
//						exit function
//		end select
	}else{
		switch(State){
			case 0: return "NO_ALG";
			case 1: return "ON";
			case 2: return "OFF";
			case 3: return "AND";
			case 4: return "OR";
			case 5: return "CONN";
			case 6: return "NOT";
			case 7: return "NOR";
			case 8: return "NAND";
			case 9: return "XOR";
			default: return "";
		}
	}
//end function
}
void vbsDrawAllObjects(CHTML* html)
{
/*
'called by:
'loadOnPageComplete()
'clearUpdateTimer()
'NumInputsFirstcond()
'CM_FB_CxUPSTREAM [event: onupdate, condition: g_UpdateAllowed = true]
function DrawAllObjects
	on error resume next
	CM_Description.value = CM_FB_DESC.value
	CM_Description.style.visibility = "visible"
	CM_Display.value = DisplayDataRepository.GetValue("CurrentPoint") & "." & DisplayDataRepository.GetValue("InterlockName")
	CM_Display.style.visibility = "visible"		
	g_Gate1 = CM_FB_G1NUMINPUTS.value
	g_Gate2 = CM_FB_G2NUMINPUTS.value
	g_Gate3 = CM_FB_G3NUMINPUTS.value
	g_Gate4 = g_Gate1 + g_Gate2 + g_Gate3
	if not g_CM_ArrayPointer = 0 then	
		backButton.style.visibility = "visible"
	end if
	if g_Gate4 = "0" then
		loadingText.value = "No Signal assigned to any Gate"
		loadingText.style.visibility = "visible"
		exit function
	end if
	for i = 1 To g_Gate4
		page.all("Signal" & i).style.visibility = "visible"
		page.all("Signal" & i & "_text").value = page.all("CM_FB_C" & i & "DESC").value
		page.all("Signal" & i & "_text").style.visibility = "visible"
		page.all("Line" & i).style.visibility = "hidden" 'hide all Gatelines
		page.all("C" & i & "FLine").style.visibility = "visible"
		page.all("Upstream" & i).style.visibility = "visible"
		page.all("UpstreamBypass" & i).style.visibility = "visible"
		page.all("Bypass_Checkbox" & i).style.visibility = "visible"
		page.all("Bypass_BG" & i).style.visibility = "visible"
		page.all("Bypass_Combo" & i).style.visibility = "visible"
		if not page.all("CM_FB_C" & i & "UPSTREAM") = "" then
			page.all("Upstream" & i).value = "<"
		else
			page.all("Upstream" & i).value = ""
		end if
		if page.all("CM_FB_C" & i & "UPSTREAMBYP").value = "1" then
			page.all("UpstreamBypass" & i).value = "B"
		else
			page.all("UpstreamBypass" & i).value = ""
		end if
		if g_SetComboboxValues = true then
			tmp = ""
			tmp = page.all("Bypass_Combo" & i).currentvalue
			page.all("Bypass_Combo" & i).ResetContent
			page.all("Bypass_Combo" & i).AddString "-"
			page.all("Bypass_Combo" & i).AddString "1"
			page.all("Bypass_Combo" & i).AddString "0"
			page.all("Bypass_Combo" & i).currentvalue = tmp
			if g_SetComboboxValues = true and i = 1 then
				tmp = ""
				tmp = FinalBypass_Combo.currentvalue
				FinalBypass_Combo.ResetContent
				FinalBypass_Combo.AddString "-"
				FinalBypass_Combo.AddString "1"
				FinalBypass_Combo.AddString "0"
				FinalBypass_Combo.currentvalue = tmp
			end if
			if g_SetComboboxValues = true and i = 12 then
				g_SetComboboxValues = false
			end if
		end if
	next
	for i = g_Gate4 + 1 To 12
		page.all("Signal" & i).style.visibility = "hidden"
		page.all("Signal" & i & "_text").style.visibility = "hidden"
		page.all("Signal" & i & "_updetected").style.visibility = "hidden"
		page.all("Line" & i).style.visibility = "hidden"
		page.all("C" & i & "FLine").style.visibility = "hidden"
		page.all("Upstream" & i).style.visibility = "hidden"
		page.all("UpstreamBypass" & i).style.visibility = "hidden"
		page.all("Bypass_Checkbox" & i).style.visibility = "hidden"
		page.all("Bypass_BG" & i).style.visibility = "hidden"
		page.all("Bypass_Combo" & i).style.visibility = "hidden"
		if g_SetComboboxValues = true and i = 1 then
			page.all("FinalBypass_Checkbox").style.visibility = "hidden"
			page.all("FinalBypass_BG").style.visibility = "hidden"
			page.all("FinalBypass_Combo").style.visibility = "hidden"
		end if
		if g_SetComboboxValues = true and i = 12 then
			g_SetComboboxValues = false
		end if
	next
	if not g_Gate1 = 0 then
		Gate1.style.pixelTop = 0
		Gate1.style.pixelWidth = 50
		Gate1.style.pixelHeight = g_Gate1 * 21 + (g_Gate1 - 1) * 3
		Gate1.style.visibility = "visible"
		textGate1.value = GetGateLogic(CM_FB_G1ALGID.value)
		if CM_FB_G1ALGID.value = 5 or CM_FB_G1ALGID.value = 6 then
			page.all("Line1").style.visibility = "visible"
		elseif CM_FB_G1ALGID.value = 3 or CM_FB_G1ALGID.value = 4 or CM_FB_G1ALGID.value = 7 or CM_FB_G1ALGID.value = 8 or CM_FB_G1ALGID.value = 9 then
			for i =  1 To g_Gate1
				page.all("Line" & i).style.visibility = "visible"
			next
		end if
		if g_IECGates = 1 then
			'textGate1.style.pixelTop = Gate1.style.pixelHeight / 2 - 8
			textGate1.style.pixelTop = 2
			textGate1.style.textAlign = "center"
			textGate1.style.pixelLeft = 0
			textGate1.style.pixelWidth = 50
		else
			textGate1.style.pixelTop = 2
			textGate1.style.pixelLeft = 5
			textGate1.style.pixelWidth = 45
		end if
		textGate1.style.visibility = "visible"
		Line13.style.pixelLeft = 50
		Line13.style.pixelTop = Gate1.style.pixelHeight / 2
		Line13.style.visibility = "hidden"
		if g_IECGates = 1 then
			if CM_FB_G1ALGID.value = 6 or CM_FB_G1ALGID.value = 7 or CM_FB_G1ALGID.value = 8 then
				NegGate1.style.pixelLeft = 50
				NegGate1.style.pixelWidth = 8
				NegGate1.style.pixelHeight = 8
				NegGate1.style.pixelTop = Gate1.style.pixelHeight / 2 - 4
				NegGate1.style.visibility = "visible"
			else
				NegGate1.style.visibility = "hidden"
			end if
		else
			NegGate1.style.visibility = "hidden"
		end if
	end if
	if not g_Gate2 = 0 then
		Gate2.style.Top = Gate1.style.pixelHeight + 3
		Gate2.style.pixelWidth = 50
		Gate2.style.pixelHeight = g_Gate2 * 21 + (g_Gate2 - 1) * 3
		Gate2.style.visibility = "visible"
		textGate2.value = GetGateLogic(CM_FB_G2ALGID.value)
		if CM_FB_G2ALGID.value = 5 or CM_FB_G2ALGID.value = 6 then
			page.all("Line" & g_Gate1 + 1).style.visibility = "visible"
		elseif CM_FB_G2ALGID.value = 3 or CM_FB_G2ALGID.value = 4 or CM_FB_G2ALGID.value = 7 or CM_FB_G2ALGID.value = 8 or CM_FB_G2ALGID.value = 9 then
			for i =  g_Gate1 + 1 To g_Gate1 + g_Gate2
				page.all("Line" & i).style.visibility = "visible"
			next
		end if
		if g_IECGates = 1 then
			'textGate2.style.pixelTop = Gate1.style.pixelHeight + Gate2.style.pixelHeight / 2 - 5
			textGate2.style.pixelTop = 2 + Gate1.style.pixelHeight + 3
			textGate2.style.textAlign = "center"
			textGate2.style.pixelLeft = 0
			textGate2.style.pixelWidth = 50
		else
			textGate2.style.pixelTop = 2 + Gate1.style.pixelHeight + 3
			textGate2.style.pixelLeft = 5
			textGate2.style.pixelWidth = 45
		end if
		textGate2.style.visibility = "visible"
		Line14.style.pixelLeft = 50
		Line14.style.pixelTop = Gate1.style.pixelHeight + 3 + Gate2.style.pixelHeight / 2
		Line14.style.visibility = "hidden"
		if g_IECGates = 1 then
			if CM_FB_G2ALGID.value = 6 or CM_FB_G2ALGID.value = 7 or CM_FB_G2ALGID.value = 8 then
				NegGate2.style.pixelLeft = 50
				NegGate2.style.pixelWidth = 8
				NegGate2.style.pixelHeight = 8
				NegGate2.style.pixelTop = Gate1.style.pixelHeight + 3 + Gate2.style.pixelHeight / 2 - 4
				NegGate2.style.visibility = "visible"
			else
				NegGate2.style.visibility = "hidden"
			end if
		else
			NegGate2.style.visibility = "hidden"
		end if
	end if
	if not g_Gate3 = 0 then
		Gate3.style.Top = Gate1.style.pixelHeight + Gate2.style.pixelHeight + 6
		Gate3.style.pixelWidth = 50
		Gate3.style.pixelHeight = g_Gate3 * 21 + (g_Gate3 - 1) * 3
		Gate3.style.visibility = "visible"
		textGate3.value = GetGateLogic(CM_FB_G3ALGID.value)
		if CM_FB_G3ALGID.value = 5 or CM_FB_G3ALGID.value = 6 then
			page.all("Line" & g_Gate1 + g_Gate2 + 1).style.visibility = "visible"
		elseif CM_FB_G3ALGID.value = 3 or CM_FB_G3ALGID.value = 4 or CM_FB_G3ALGID.value = 7 or CM_FB_G3ALGID.value = 8 or CM_FB_G3ALGID.value = 9 then
			for i =  g_Gate1 + g_Gate2 + 1 To g_Gate1 + g_Gate2 + g_Gate3
				page.all("Line" & i).style.visibility = "visible"
			next
		end if
		if g_IECGates = 1 then
			'textGate3.style.pixelTop = Gate1.style.pixelHeight + Gate2.style.pixelHeight + Gate3.style.pixelHeight / 2 - 2
			textGate3.style.pixelTop = 2 + Gate1.style.pixelHeight + Gate2.style.pixelHeight + 6
			textGate3.style.textAlign = "center"
			textGate3.style.pixelLeft = 0
			textGate3.style.pixelWidth = 50
		else
			textGate3.style.pixelTop = 2 + Gate1.style.pixelHeight + Gate2.style.pixelHeight + 6
			textGate3.style.pixelLeft = 5
			textGate3.style.pixelWidth = 45
		end if
		textGate3.style.visibility = "visible"
		Line15.style.pixelLeft = 50
		Line15.style.pixelTop = Gate1.style.pixelHeight + Gate2.style.pixelHeight + 6 + Gate3.style.pixelHeight / 2
		Line15.style.visibility = "hidden"
		if g_IECGates = 1 then
			if CM_FB_G3ALGID.value = 6 or CM_FB_G3ALGID.value = 7 or CM_FB_G3ALGID.value = 8 then
				NegGate3.style.pixelLeft = 50
				NegGate3.style.pixelWidth = 8
				NegGate3.style.pixelHeight = 8
				NegGate3.style.pixelTop = Gate1.style.pixelHeight + Gate2.style.pixelHeight + 6 + Gate3.style.pixelHeight / 2 - 4
				NegGate3.style.visibility = "visible"
			else
				NegGate3.style.visibility = "hidden"
			end if
		else
			NegGate3.style.visibility = "hidden"
		end if
	end if
	if not g_Gate4 = 0 then
		Gate4.style.pixelTop = 0
		Gate4.style.pixelWidth = 50
		Gate4.style.pixelLeft = 80
		Gate4.style.pixelHeight = g_Gate4 * 21 + (g_Gate4 - 1) * 3
		Gate4.style.visibility = "visible"
		textGate4.value = GetGateLogic(CM_FB_G4ALGID.value)
		if CM_FB_G4ALGID.value = 5 or CM_FB_G4ALGID.value = 6 then
			page.all("Line13").style.visibility = "visible"
		elseif CM_FB_G4ALGID.value = 3 or CM_FB_G4ALGID.value = 4 or CM_FB_G4ALGID.value = 7 or CM_FB_G4ALGID.value = 8 or CM_FB_G4ALGID.value = 9 then
			page.all("Line13").style.visibility = "visible"
			if g_Gate2 > 0 then page.all("Line14").style.visibility = "visible"
			if g_Gate3 > 0 then page.all("Line15").style.visibility = "visible"
		end if
		if g_IECGates = 1 then
			'textGate4.style.pixelTop = Gate4.style.pixelHeight / 2 - 8
			textGate4.style.pixelTop = 2
			textGate4.style.textAlign = "center"
			textGate4.style.pixelLeft = 80
			textGate4.style.pixelWidth = 50
		else
			textGate4.style.pixelTop = 2
			textGate4.style.pixelLeft = 85
			textGate4.style.pixelWidth = 45
		end if
		textGate4.style.visibility = "visible"
		Line16.style.pixelLeft = 130
		Line16.style.pixelTop = Gate4.style.pixelHeight / 2
		Line16.style.visibility = "visible"
		if g_IECGates = 1 then
			if CM_FB_G4ALGID.value = 6 or CM_FB_G4ALGID.value = 7 or CM_FB_G4ALGID.value = 8 then
				NegGate4.style.pixelLeft = 130
				NegGate4.style.pixelWidth = 8
				NegGate4.style.pixelHeight = 8
				NegGate4.style.pixelTop = Gate4.style.pixelHeight / 2 - 4
				NegGate4.style.visibility = "visible"
			else
				NegGate4.style.visibility = "hidden"
			end if
		else
			NegGate4.style.visibility = "hidden"
		end if
		FinalBypass.style.left = 110
		FinalBypass.style.top = (Gate4.style.pixelHeight / 2) + 5
		FinalBypass_Checkbox.style.visibility = "visible"
		FinalBypass_BG.style.visibility = "visible"
		FinalBypass_Combo.style.visibility = "visible"
	end if
	if g_GateError = false then
		loadingText.style.visibility = "hidden"
	else
		loadingText.value = "Loading...Please Wait"
		loadingText.style.visibility = "visible"
	end if
end function
*/

	CTag * CM_Description = get_object(html,  "CM_Description");
	sprintf(CM_Description->m_szScriptValue, "%s", html->pipe.ValueS_H(get_object(html,  "CM_FB_DESC")->mVal));
	CM_Description->Style(*html).m_eVisible = enumVisible;
	CTag* CM_Display = get_object(html,  "CM_Display");
	sprintf(CM_Display->m_szScriptValue, "%s", html->pipe.GetModelName(get_object(html,  "CM_FB_DESC")->mVal.nNumber)); 
	CM_Display->Style(*html).m_eVisible = enumVisible;	
	html->mBody->m_strInterlockContainer.g_Gate1 = html->pipe.ValueI_H(get_object(html,  "CM_FB_G1NUMINPUTS")->mVal);
	html->mBody->m_strInterlockContainer.g_Gate2 = html->pipe.ValueI_H(get_object(html,  "CM_FB_G2NUMINPUTS")->mVal);
	html->mBody->m_strInterlockContainer.g_Gate3 = html->pipe.ValueI_H(get_object(html,  "CM_FB_G3NUMINPUTS")->mVal);
	html->mBody->m_strInterlockContainer.g_Gate4 = html->mBody->m_strInterlockContainer.g_Gate1 + html->mBody->m_strInterlockContainer.g_Gate2 + html->mBody->m_strInterlockContainer.g_Gate3;
	if (html->mBody->m_strInterlockContainer.g_CM_ArrayPointer != 0) {
		__debugbreak();
//		backButton.style.visibility = "visible"
	}
	if (html->mBody->m_strInterlockContainer.g_Gate4 == 0) {
		sprintf(get_object(html,  "loadingText")->m_szScriptValue, "No Signal assigned to any Gate");
		return;
	}

    CTag* GroupSignal = get_object(html, "Signal"); 
    if(GroupSignal)
       GroupSignal->Style(*html).m_eVisible = enumVisible;
    CTag* GroupLine = get_object(html, "Line"); 
    if(GroupLine)
       GroupLine->Style(*html).m_eVisible = enumVisible;
    
	for (int i = 1; i <= html->mBody->m_strInterlockContainer.g_Gate4; i++) {
		char tmp[32], tmp1[32]; sprintf(tmp, "Signal%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		sprintf(tmp, "Signal%d_text", i); sprintf(tmp1, "CM_FB_C%dDESC", i); 
		sprintf(get_object(html, tmp)->m_szScriptValue, "%s ", html->pipe.ValueS_H(get_object(html, tmp1)->mVal));
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		sprintf(tmp, "Line%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "C%dFLine", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		sprintf(tmp, "Upstream%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		sprintf(tmp, "UpstreamBypass%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		sprintf(tmp, "Bypass_Checkbox%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		sprintf(tmp, "Bypass_BG%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		sprintf(tmp, "Bypass_Combo%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;

        sprintf(tmp, "Bypass%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;

//		if not page.all("CM_FB_C" & i & "UPSTREAM") = "" then
//			page.all("Upstream" & i).value = "<"
//		else
//			page.all("Upstream" & i).value = ""
//		end if
//		if page.all("CM_FB_C" & i & "UPSTREAMBYP").value = "1" then
//			page.all("UpstreamBypass" & i).value = "B"
//		else
//			page.all("UpstreamBypass" & i).value = ""
//		end if
		if (html->mBody->m_strInterlockContainer.g_SetComboboxValues) {
			char bpcbname[16]; sprintf(bpcbname, "Bypass_Combo%d", i);
			CHTML_NBCB * Bypass_Combo = (CHTML_NBCB*)get_object(html, bpcbname)->m_pWnd;
			const char * tmp = Bypass_Combo->CurrentValue();
			Bypass_Combo->ResetContent();
			Bypass_Combo->AddString("-");
			Bypass_Combo->AddString("1");
			Bypass_Combo->AddString("0");
			Bypass_Combo->CurrentValue(tmp);
			if (html->mBody->m_strInterlockContainer.g_SetComboboxValues && i == 1 ){
				CHTML_NBCB * FinalBypass_Combo = (CHTML_NBCB*)get_object(html, "FinalBypass_Combo")->m_pWnd;
				tmp = FinalBypass_Combo->CurrentValue();
				FinalBypass_Combo->ResetContent();
				FinalBypass_Combo->AddString("-");
				FinalBypass_Combo->AddString("1");
				FinalBypass_Combo->AddString("0");
				FinalBypass_Combo->CurrentValue(tmp);
			}
			if (html->mBody->m_strInterlockContainer.g_SetComboboxValues && i == 12){
				html->mBody->m_strInterlockContainer.g_SetComboboxValues = false;
			}
		}
	}
	for (int i = html->mBody->m_strInterlockContainer.g_Gate4 + 1; i <= 12; i++) {
		char tmp[32]; 
		sprintf(tmp, "Signal%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "Signal%d_text", i); 
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "Signal%d_updetected", i); 
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "Line%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "C%dFLine", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "Upstream%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "UpstreamBypass%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "Bypass_Checkbox%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "Bypass_BG%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		sprintf(tmp, "Bypass_Combo%d", i);
		get_object(html, tmp)->Style(*html).m_eVisible = enumHidden;
		if (html->mBody->m_strInterlockContainer.g_SetComboboxValues && i == 1){
			__debugbreak();
//			page.all("FinalBypass_Checkbox").style.visibility = "hidden"
//			page.all("FinalBypass_BG").style.visibility = "hidden"
//			page.all("FinalBypass_Combo").style.visibility = "hidden"
		}
		if (html->mBody->m_strInterlockContainer.g_SetComboboxValues = true && i == 12) {
			html->mBody->m_strInterlockContainer.g_SetComboboxValues = false;
		}
	}
	CTag* Gate = get_object(html, "Gate");
	CTag* Gate1 = get_object(html, "Gate1");
	CTag* Gate2 = get_object(html, "Gate2");
	CTag* Gate3 = get_object(html, "Gate3");
	CTag* textGate1 = get_object(html, "textGate1");
	CTag* textGate2 = get_object(html, "textGate2");
	CTag* textGate3 = get_object(html, "textGate3");
	CTag* NegGate1 = get_object(html, "NegGate1");
	CTag* NegGate2 = get_object(html, "NegGate2");
	CTag* NegGate3 = get_object(html, "NegGate3");
	if (html->mBody->m_strInterlockContainer.g_Gate1 != 0) {
		Gate1->Style(*html).mRect.Y = Gate->Style(*html).mRect.Y;
		Gate1->Style(*html).mRect.Width = 50;
		Gate1->Style(*html).mRect.Height = (html->mBody->m_strInterlockContainer.g_Gate1 * 21 + (html->mBody->m_strInterlockContainer.g_Gate1 - 1) * 3);
		Gate1->Style(*html).m_eVisible = enumVisible;
		CTag* CM_FB_G1ALGID = get_object(html, "CM_FB_G1ALGID");
		sprintf(textGate1->m_szScriptValue, "%s", vbsGetGateLogic(html, html->pipe.ValueB_H(CM_FB_G1ALGID->mVal)));
		char CM_FB_G1ALGID_value = html->pipe.ValueB_H(CM_FB_G1ALGID->mVal);
		if (CM_FB_G1ALGID_value == 5 || CM_FB_G1ALGID_value == 6){
			get_object(html, "Line1")->Style(*html).m_eVisible = enumVisible;
		}else if (CM_FB_G1ALGID_value == 3 || CM_FB_G1ALGID_value == 4 || CM_FB_G1ALGID_value == 7 || CM_FB_G1ALGID_value == 8 || CM_FB_G1ALGID_value == 9 ){
			for (int i =  1; i <= html->mBody->m_strInterlockContainer.g_Gate1; i++){
				char tmp[16]; sprintf(tmp, "Line%d", i);
                CTag* LineX = get_object(html, tmp);
                if(LineX)
                {
                CStyle& st = LineX->Style(*html);
				st.m_eVisible = enumVisible;
                }
			}
		}
		if (html->mBody->m_strInterlockContainer.g_IECGates == 1){
			__debugbreak();
//			'textGate1.style.pixelTop = Gate1.style.pixelHeight / 2 - 8
//			textGate1.style.pixelTop = 2
//			textGate1.style.textAlign = "center"
//			textGate1.style.pixelLeft = 0
//			textGate1.style.pixelWidth = 50
		}else{
			textGate1->Style(*html).mRect.Y = Gate->Style(*html).mRect.Y + 2;
			textGate1->Style(*html).mRect.X = Gate->Style(*html).mRect.X + 5;
			textGate1->Style(*html).mRect.Width = 45;
		}
		textGate1->Style(*html).m_eVisible = enumVisible;
		CTag* Line13 = get_object(html, "Line13");
		POINT * pts = Line13->Style(*html).PT(*html);
		pts[0].x = Gate->Style(*html).mRect.X + 50;
		pts[0].y = Gate->Style(*html).mRect.Y + (Gate1->Style(*html).mRect.Height / 2);
		pts[1].y = Gate->Style(*html).mRect.Y + (Gate1->Style(*html).mRect.Height / 2);
		Line13->Style(*html).m_eVisible = enumHidden;
		if (html->mBody->m_strInterlockContainer.g_IECGates == 1){
			__debugbreak();
			if (CM_FB_G1ALGID_value == 6 || CM_FB_G1ALGID_value == 7 || CM_FB_G1ALGID_value == 8 ){
				__debugbreak();
//				NegGate1.style.pixelLeft = 50
//				NegGate1.style.pixelWidth = 8
//				NegGate1.style.pixelHeight = 8
//				NegGate1.style.pixelTop = Gate1.style.pixelHeight / 2 - 4
//				NegGate1.style.visibility = "visible"
			}else{
				__debugbreak();
//				NegGate1.style.visibility = "hidden"
			}
		}else{
			NegGate1->Style(*html).m_eVisible = enumHidden;
		}
	}
    
	if (html->mBody->m_strInterlockContainer.g_Gate2 != 0) {
		CTag* CM_FB_G2ALGID = get_object(html, "CM_FB_G2ALGID");
		Gate2->Style(*html).mRect.Y = Gate->Style(*html).mRect.Y + Gate1->Style(*html).mRect.Height + 3;
		Gate2->Style(*html).mRect.Width = 50;
		Gate2->Style(*html).mRect.Height = html->mBody->m_strInterlockContainer.g_Gate2 * 21 + (html->mBody->m_strInterlockContainer.g_Gate2 - 1) * 3;
		Gate2->Style(*html).m_eVisible = enumVisible;
		char CM_FB_G2ALGID_value = html->pipe.ValueB_H(CM_FB_G2ALGID->mVal);
		sprintf(textGate2->m_szScriptValue, "%s", vbsGetGateLogic(html, CM_FB_G2ALGID_value));
		if (CM_FB_G2ALGID_value == 5 || CM_FB_G2ALGID_value == 6 ){
			char tmp[16]; sprintf(tmp, "Line%d", html->mBody->m_strInterlockContainer.g_Gate1 + 1);
			get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		}else if (CM_FB_G2ALGID_value == 3 || CM_FB_G2ALGID_value == 4 || CM_FB_G2ALGID_value == 7 || CM_FB_G2ALGID_value == 8 || CM_FB_G2ALGID_value == 9 ){
			for (int i =  html->mBody->m_strInterlockContainer.g_Gate1 + 1; i<=( html->mBody->m_strInterlockContainer.g_Gate1 + html->mBody->m_strInterlockContainer.g_Gate2); i++){
				char tmp[16]; sprintf(tmp, "Line%d", i);
				get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
			}
		}
		if (html->mBody->m_strInterlockContainer.g_IECGates == 1 ){
			__debugbreak();
//			textGate2.style.pixelTop = 2 + Gate1.style.pixelHeight + 3
//			textGate2.style.textAlign = "center"
//			textGate2.style.pixelLeft = 0
//			textGate2.style.pixelWidth = 50
		}else{
			textGate2->Style(*html).mRect.Y = Gate->Style(*html).mRect.Y + 2 + Gate1->Style(*html).mRect.Height + 3;
			textGate2->Style(*html).mRect.X = Gate->Style(*html).mRect.X + 5;
			textGate2->Style(*html).mRect.Width = 45;
		}
		textGate2->Style(*html).m_eVisible = enumVisible;
		CTag* Line14 = get_object(html, "Line14");
		POINT * pts = Line14->Style(*html).PT(*html);
		pts[0].x = Gate->Style(*html).mRect.X + 50;
		pts[0].y = Gate->Style(*html).mRect.Y + Gate1->Style(*html).mRect.Height + 2 + Gate2->Style(*html).mRect.Height / 2;
		pts[1].y = Gate->Style(*html).mRect.Y + Gate1->Style(*html).mRect.Height + 2 + Gate2->Style(*html).mRect.Height / 2;
		Line14->Style(*html).m_eVisible = enumHidden;
		if (html->mBody->m_strInterlockContainer.g_IECGates == 1 ){
			if (CM_FB_G2ALGID_value == 6 || CM_FB_G2ALGID_value == 7 || CM_FB_G2ALGID_value == 8 ){
				__debugbreak();
//				NegGate2.style.pixelLeft = 50
//				NegGate2.style.pixelWidth = 8
//				NegGate2.style.pixelHeight = 8
//				NegGate2.style.pixelTop = Gate1.style.pixelHeight + 3 + Gate2.style.pixelHeight / 2 - 4
//				NegGate2.style.visibility = "visible"
			}else{
				__debugbreak();
//				NegGate2.style.visibility = "hidden"
			}
		}else{
			NegGate1->Style(*html).m_eVisible = enumHidden;
		}
	}
	if (html->mBody->m_strInterlockContainer.g_Gate3 != 0) {
		CTag* CM_FB_G3ALGID = get_object(html, "CM_FB_G3ALGID");
		Gate3->Style(*html).mRect.Y = Gate->Style(*html).mRect.Y + Gate1->Style(*html).mRect.Height + Gate2->Style(*html).mRect.Height + 6;
		Gate3->Style(*html).mRect.Width = 50;
		Gate3->Style(*html).mRect.Height = html->mBody->m_strInterlockContainer.g_Gate3 * 21 + (html->mBody->m_strInterlockContainer.g_Gate3 - 1) * 3;
		Gate3->Style(*html).m_eVisible = enumVisible;
		char CM_FB_G3ALGID_value = html->pipe.ValueB_H(CM_FB_G3ALGID->mVal);
		sprintf(textGate2->m_szScriptValue, "%s", vbsGetGateLogic(html, CM_FB_G3ALGID_value));
		if (CM_FB_G3ALGID_value == 5 || CM_FB_G3ALGID_value == 6){
			char tmp[16]; sprintf(tmp, "Line%d", html->mBody->m_strInterlockContainer.g_Gate1 + html->mBody->m_strInterlockContainer.g_Gate2 + 1);
			get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
		}else if (CM_FB_G3ALGID_value == 3 || CM_FB_G3ALGID_value == 4 || CM_FB_G3ALGID_value == 7 || CM_FB_G3ALGID_value == 8 || CM_FB_G3ALGID_value == 9 ){
			for (int i =  html->mBody->m_strInterlockContainer.g_Gate1 + html->mBody->m_strInterlockContainer.g_Gate2 + 1; i<= html->mBody->m_strInterlockContainer.g_Gate1 + html->mBody->m_strInterlockContainer.g_Gate2 + html->mBody->m_strInterlockContainer.g_Gate3; i++){
				char tmp[16]; sprintf(tmp, "Line%d", i);
				get_object(html, tmp)->Style(*html).m_eVisible = enumVisible;
			}
		}
		if (html->mBody->m_strInterlockContainer.g_IECGates == 1 ){
			__debugbreak();
//			textGate3.style.pixelTop = 2 + Gate1.style.pixelHeight + Gate2.style.pixelHeight + 6
//			textGate3.style.textAlign = "center"
//			textGate3.style.pixelLeft = 0
//			textGate3.style.pixelWidth = 50
		}else{
			textGate3->Style(*html).mRect.Y = Gate->Style(*html).mRect.Y + 2 + Gate1->Style(*html).mRect.Height + Gate2->Style(*html).mRect.Height + 6;
			textGate3->Style(*html).mRect.X = Gate->Style(*html).mRect.X + 5;
			textGate3->Style(*html).mRect.Width = 45;
		}
		textGate3->Style(*html).m_eVisible = enumVisible;
		CTag* Line15 = get_object(html, "Line15");
		POINT * pts = Line15->Style(*html).PT(*html);
		pts[0].x = Gate->Style(*html).mRect.X + 50;
		pts[0].y = Gate->Style(*html).mRect.Y + Gate1->Style(*html).mRect.Height + Gate2->Style(*html).mRect.Height + 6 + Gate3->Style(*html).mRect.Height / 2;
		pts[1].y = Gate->Style(*html).mRect.Y + Gate1->Style(*html).mRect.Height + Gate2->Style(*html).mRect.Height + 6 + Gate3->Style(*html).mRect.Height / 2;
		Line15->Style(*html).m_eVisible = enumHidden;
		if (html->mBody->m_strInterlockContainer.g_IECGates == 1){
			if (CM_FB_G3ALGID_value == 6 || CM_FB_G3ALGID_value == 7 || CM_FB_G3ALGID_value == 8 ){
				__debugbreak();
//				NegGate3.style.pixelLeft = 50
//				NegGate3.style.pixelWidth = 8
//				NegGate3.style.pixelHeight = 8
//				NegGate3.style.pixelTop = Gate1.style.pixelHeight + Gate2.style.pixelHeight + 6 + Gate3.style.pixelHeight / 2 - 4
//				NegGate3.style.visibility = "visible"
			}else{
				__debugbreak();
//				NegGate3.style.visibility = "hidden"
			}
		}else{
			NegGate3->Style(*html).m_eVisible = enumHidden;
		}
	}
	if (html->mBody->m_strInterlockContainer.g_Gate4 != 0) {
		CTag* textGate4 = get_object(html, "textGate4");
		CTag* Gate = get_object(html, "Gate");
		CTag* Gate4 = get_object(html, "Gate4");
		Gate4->Style(*html).mRect.Y = Gate->Style(*html).mRect.Y;
		Gate4->Style(*html).mRect.Width = 50;
		Gate4->Style(*html).mRect.X = Gate->Style(*html).mRect.X + 80;
		Gate4->Style(*html).mRect.Height = html->mBody->m_strInterlockContainer.g_Gate4 * 21 + (html->mBody->m_strInterlockContainer.g_Gate4 - 1) * 3;
		Gate4->Style(*html).m_eVisible = enumVisible;
		CTag* CM_FB_G4ALGID = get_object(html, "CM_FB_G4ALGID");
		char CM_FB_G4ALGID_value = html->pipe.ValueB_H(CM_FB_G4ALGID->mVal);
		sprintf(textGate4->m_szScriptValue, "%s", vbsGetGateLogic(html, CM_FB_G4ALGID_value));
		if (CM_FB_G4ALGID_value == 5 || CM_FB_G4ALGID_value == 6) {
			get_object(html, "Line13")->Style(*html).m_eVisible = enumVisible;
		} else if (CM_FB_G4ALGID_value == 3 || CM_FB_G4ALGID_value == 4 || CM_FB_G4ALGID_value == 7 || CM_FB_G4ALGID_value == 8 || CM_FB_G4ALGID_value == 9 ) {
			get_object(html, "Line13")->Style(*html).m_eVisible = enumVisible;
			if (html->mBody->m_strInterlockContainer.g_Gate2 > 0) get_object(html, "Line14")->Style(*html).m_eVisible = enumVisible;
			if (html->mBody->m_strInterlockContainer.g_Gate3 > 0) get_object(html, "Line15")->Style(*html).m_eVisible = enumVisible;
		}
		if (html->mBody->m_strInterlockContainer.g_IECGates == 1){
			__debugbreak();
//			'textGate4.style.pixelTop = Gate4.style.pixelHeight / 2 - 8
//			textGate4.style.pixelTop = 2
//			textGate4.style.textAlign = "center"
//			textGate4.style.pixelLeft = 80
//			textGate4.style.pixelWidth = 50
		}else{
			textGate4->Style(*html).mRect.Y = Gate->Style(*html).mRect.Y + 2;
			textGate4->Style(*html).mRect.X = Gate->Style(*html).mRect.X + 85;
			textGate4->Style(*html).mRect.Width = 45;
		}
		textGate4->Style(*html).m_eVisible = enumVisible;
		CTag* Line16 = get_object(html, "Line16");
		POINT * pts = Line16->Style(*html).PT(*html);
		pts[0].x = Gate->Style(*html).mRect.X + 110;
		pts[0].y = Gate->Style(*html).mRect.Y + (Gate4->Style(*html).mRect.Height / 2);
		pts[1].y = Gate->Style(*html).mRect.Y + (Gate4->Style(*html).mRect.Height / 2);
		Line16->Style(*html).m_eVisible = enumVisible;
		CTag* NegGate4 = get_object(html, "NegGate4");
		if (html->mBody->m_strInterlockContainer.g_IECGates == 1) {
			__debugbreak();
			if (CM_FB_G4ALGID_value == 6 || CM_FB_G4ALGID_value == 7 || CM_FB_G4ALGID_value == 8){
				__debugbreak();
				NegGate4->Style(*html).mRect.X = 130;
				NegGate4->Style(*html).mRect.Width = 8;
				NegGate4->Style(*html).mRect.Height = 8;
				NegGate4->Style(*html).mRect.Y = Gate4->Style(*html).mRect.Height / 2 - 4;
				NegGate4->Style(*html).m_eVisible = enumVisible;
			}else{
				__debugbreak();
				NegGate4->Style(*html).m_eVisible = enumHidden;
			}
		}else{
			NegGate4->Style(*html).m_eVisible = enumHidden;
		}		
		CTag* FinalBypass = get_object(html, "FinalBypass");
		FinalBypass->Style(*html).mRect.X = Gate->Style(*html).mRect.X + 110;
		FinalBypass->Style(*html).mRect.Y = Gate->Style(*html).mRect.Y + (Gate4->Style(*html).mRect.Height / 2) + 5;
        FinalBypass->Style(*html).m_eVisible = enumVisible;

		CTag * FinalBypass_Checkbox = get_object(html, "FinalBypass_Checkbox")->GetData()[0];
		get_object(html, "FinalBypass_Checkbox")->Style(*html).m_eVisible = enumVisible;
		get_object(html, "FinalBypass_Checkbox")->Style(*html).mRect.X = FinalBypass->Style(*html).mRect.X;
		get_object(html, "FinalBypass_Checkbox")->Style(*html).mRect.Y = FinalBypass->Style(*html).mRect.Y + 1;
		FinalBypass_Checkbox->Style(*html).m_eVisible = enumVisible;
		FinalBypass_Checkbox->Style(*html).mRect.X = FinalBypass->Style(*html).mRect.X;
		FinalBypass_Checkbox->Style(*html).mRect.Y = FinalBypass->Style(*html).mRect.Y + 1;

		get_object(html, "FinalBypass_BG")->Style(*html).m_eVisible = enumVisible;
		get_object(html, "FinalBypass_BG")->Style(*html).mRect.X = FinalBypass->Style(*html).mRect.X;
		get_object(html, "FinalBypass_BG")->Style(*html).mRect.Y = FinalBypass->Style(*html).mRect.Y + 1;
		get_object(html, "FinalBypass_Combo")->Style(*html).m_eVisible = enumVisible;
		get_object(html, "FinalBypass_Combo")->Style(*html).mRect.X = FinalBypass->Style(*html).mRect.X + 20;
		get_object(html, "FinalBypass_Combo")->Style(*html).mRect.Y = FinalBypass->Style(*html).mRect.Y + 1;
	}
	if (!html->mBody->m_strInterlockContainer.g_GateError) {
		get_object(html, "loadingText")->Style(*html).m_eVisible = enumHidden;
	} else {
		__debugbreak();
//		loadingText.value = "Loading...Please Wait"
//		loadingText.style.visibility = "visible"
	}
}

void vbsGateSO(CHTML* html, char gsoValue, char *gsoLine, char *gsoMsg)
{
/*
'called by:
'CM_FB_GxSO [event: onupdate]
'CM_FB_G4SOFINAL [event: onupdate]
function GateSO(gsoValue, gsoLine, gsoMsg)
	on error resume next
	if gsoValue = "1" then
		page.all(gsoLine).lineColor = g_ColorSet_Array(1,13) 'ColorLineOn
	elseif gsoValue = "0" then
		page.all(gsoLine).lineColor = g_ColorSet_Array(1,14) 'ColorLineOff
	else
		call BadSignal(gsoMsg)
		page.all(gsoLine).lineColor = g_ColorSet_Array(1,15) 'ColorLineError
	end if
end function
*/

	if (gsoValue == 1) {
		get_object(html, gsoLine)->Style(*html).m_clrLine = 0x00ff00;
	}else if (gsoValue == 0){
		get_object(html, gsoLine)->Style(*html).m_clrLine = 0xaaaaaa;
	}else{
//		call BadSignal(gsoMsg)
//		page.all(gsoLine).lineColor = g_ColorSet_Array(1,15) 'ColorLineError
	}
}
void vbsUpDetection(CHTML* html, int udSignum)
{
/*
'called by:
'CM_FB_UPDETECTEDx [event: onupdate]
function UpDetection(udSignum)
	on error resume next
	if page.all("CM_FB_UPDETECTED" & udSignum) = "1" and page.all("CM_FB_FIRSTUP" & udSignum) = "0" then
		page.all("Signal" & udSignum & "_updetected").value = "*"
		page.all("Signal" & udSignum & "_updetected").style.visibility = "visible"
	elseif page.all("CM_FB_UPDETECTED" & udSignum) = "1" and page.all("CM_FB_FIRSTUP" & udSignum) = "1" then
		page.all("Signal" & udSignum & "_updetected").value = "I"
		page.all("Signal" & udSignum & "_updetected").style.visibility = "visible"
	else
		page.all("Signal" & udSignum & "_updetected").style.visibility = "hidden"
	end if
end function
*/
//'called by:
//'CM_FB_UPDETECTEDx [event: onupdate]
//function UpDetection(udSignum)
//	on error resume next
	char tmp[32]; sprintf(tmp, "CM_FB_UPDETECTED%d", udSignum);
	CTag* CM_FB_UPDETECTED = get_object(html, tmp);
	sprintf(tmp, "CM_FB_FIRSTUP%d", udSignum);
	CTag* CM_FB_FIRSTUP = get_object(html, tmp);
	sprintf(tmp, "Signal%d_updetected", udSignum);
	CTag* SignalX_updetected = get_object(html, tmp);
	char CM_FB_UPDETECTED_val = html->pipe.ValueB_H(CM_FB_UPDETECTED->mVal);
	char CM_FB_FIRSTUP_val = html->pipe.ValueB_H(CM_FB_FIRSTUP->mVal);
	if (CM_FB_UPDETECTED_val == 1 && CM_FB_FIRSTUP_val == 0){
		sprintf(SignalX_updetected->m_szScriptValue, "*");
		SignalX_updetected->Style(*html).m_eVisible = enumVisible;
	}else if (CM_FB_UPDETECTED_val == 1 && CM_FB_FIRSTUP_val == 1){
		sprintf(SignalX_updetected->m_szScriptValue, "I");
		SignalX_updetected->Style(*html).m_eVisible = enumVisible;
	}else{
		SignalX_updetected->Style(*html).m_eVisible = enumHidden;
	}
//end function
}

void vbsFirstUpDetection(CHTML* html, int fudSignum)
{
/*
'called by:
'CM_FB_FIRSTUPx [event: onupdate]
function FirstUpDetection(fudSignum)
	on error resume next
	if page.all("CM_FB_FIRSTUP" & fudSignum) = "1" then
		page.all("Signal" & fudSignum & "_updetected").value = "I"
		page.all("Signal" & fudSignum & "_updetected").style.visibility = "visible"
	elseif page.all("CM_FB_FIRSTUP" & fudSignum) = "0" and page.all("CM_FB_UPDETECTED" & fudSignum) = "1" then
		page.all("Signal" & fudSignum & "_updetected").value = "*"
		page.all("Signal" & fudSignum & "_updetected").style.visibility = "visible"
	else
		page.all("Signal" & fudSignum & "_updetected").style.visibility = "hidden"
	end if
end function
*/

//'called by:
//'CM_FB_FIRSTUPx [event: onupdate]
//function FirstUpDetection(fudSignum)
//	on error resume next
	char tmp[32]; sprintf(tmp, "CM_FB_FIRSTUP%d", fudSignum);
	CTag* CM_FB_FIRSTUP = get_object(html, tmp);
	sprintf(tmp, "CM_FB_UPDETECTED%d", fudSignum);
	CTag* CM_FB_UPDETECTED = get_object(html, tmp);
	sprintf(tmp, "Signal%d_updetected", fudSignum);
	CTag* SignalX_updetected = get_object(html, tmp);
	char CM_FB_FIRSTUP_val = html->pipe.ValueB_H(CM_FB_FIRSTUP->mVal);
	char CM_FB_UPDETECTED_val = html->pipe.ValueB_H(CM_FB_UPDETECTED->mVal);
	if (CM_FB_FIRSTUP_val == 1 ){
		sprintf(SignalX_updetected->m_szScriptValue, "I");
		SignalX_updetected->Style(*html).m_eVisible = enumVisible;
	}else if (CM_FB_FIRSTUP_val == 0 && CM_FB_UPDETECTED_val == 1 ){
		sprintf(SignalX_updetected->m_szScriptValue, "*");
		SignalX_updetected->Style(*html).m_eVisible = enumVisible;
	}else{
		SignalX_updetected->Style(*html).m_eVisible = enumHidden;
	}
//end function
}

void vbsSignalFL(CHTML* html, char sflValue, int sflSignum)
{
/*
'called by:
'CM_FB_CxFL [event: onupdate]
function SignalFL(sflValue, sflSignum)
	on error resume next
	if sflValue = "1" then
		page.all("C" & sflSignum & "FLine").lineColor = g_ColorSet_Array(1,13) 'ColorLineOn
	elseif sflValue = "0" then
		page.all("C" & sflSignum & "FLine").lineColor = g_ColorSet_Array(1,14) 'ColorLineOff
	else
		if g_DisplayErrorMessages = 1 then
			call BadSignal("C" & sflSignum & "FL")
			g_MessageTimer = window.setInterval("ClearMessageBox()", g_GivenMessageTime)
		end if
		page.all("C" & sflSignum & "FLine").lineColor = g_ColorSet_Array(1,15) 'ColorLineError
	end if
end function
*/
//'called by:
//'CM_FB_CxFL [event: onupdate]
//function SignalFL(sflValue, sflSignum)
//	on error resume next
	char tmp[16]; sprintf(tmp, "C%dFLine", sflSignum);
	CTag* CXFLine = get_object(html, tmp);
	if (sflValue == 1){
		CXFLine->Style(*html).m_clrLine = 0x00ff00;
	}else if (sflValue == 0){
		CXFLine->Style(*html).m_clrLine = 0xaaaaaa;
	}else{
		if (html->mBody->m_strInterlockContainer.g_DisplayErrorMessages == 1){
//			call BadSignal("C" & sflSignum & "FL")
//			g_MessageTimer = window.setInterval("ClearMessageBox()", g_GivenMessageTime)
		}
		CXFLine->Style(*html).m_clrLine = 0x0000ff;
	}
//end function
}
void vbsSignalSO(CHTML* html, char ssoValue, int ssoSignum)
{
/*
'called by:
'CM_FB_CxSO [event: onupdate]
function SignalSO(ssoValue, ssoSignum)
	on error resume next
	if ssoValue = "1" then
		page.all("Line" & ssoSignum).lineColor = g_ColorSet_Array(1,13) 'ColorLineOn
		page.all("Signal" & ssoSignum & "_text").textColor = g_ColorSet_Array(1,7) 'ColorTextOn
		page.all("Signal" & ssoSignum & "_updetected").textColor = g_ColorSet_Array(1,7) 'ColorTextOn
		page.all("Signal" & ssoSignum).fillColor = g_ColorSet_Array(1,3) 'ColorBackGroundOn
	elseif ssoValue = "0" then
		page.all("Line" & ssoSignum).lineColor = g_ColorSet_Array(1,14) 'ColorLineOff
		page.all("Signal" & ssoSignum & "_text").textColor = g_ColorSet_Array(1,8) 'ColorTextOff
		page.all("Signal" & ssoSignum & "_updetected").textColor = g_ColorSet_Array(1,8) 'ColorTextOff
		page.all("Signal" & ssoSignum).fillColor = g_ColorSet_Array(1,4) 'ColorBackGroundOff
	else
		if g_DisplayErrorMessages = 1 then
			call BadSignal("C" & ssoSignum & "SO")
			g_MessageTimer = window.setInterval("ClearMessageBox()", g_GivenMessageTime)
		end if
		page.all("Line" & ssoSignum).lineColor = g_ColorSet_Array(1,15) 'ColorLineError
		page.all("Signal" & ssoSignum & "_text").textColor = g_ColorSet_Array(1,9) 'ColorTextError
		page.all("Signal" & ssoSignum & "_updetected").textColor = g_ColorSet_Array(1,9) 'ColorTextError
		page.all("Signal" & ssoSignum).fillColor = g_ColorSet_Array(1,5) 'ColorBackGroundError
	end if
end function
*/
//'called by:
//'CM_FB_CxSO [event: onupdate]
//function SignalSO(ssoValue, ssoSignum)
//	on error resume next
	char tmp[32]; sprintf(tmp, "Line%d", ssoSignum); 
	CTag* LineX = get_object(html, tmp); sprintf(tmp, "Signal%d_text", ssoSignum); 
	CTag* SignalX_text = get_object(html, tmp); sprintf(tmp, "Signal%d_updetected", ssoSignum); 
	CTag* SignalX_updetected = get_object(html, tmp); sprintf(tmp, "Signal%d", ssoSignum); 
	CTag* SignalX = get_object(html, tmp);
	if (ssoValue == 1){
		LineX->Style(*html).m_clrLine = 0x00ff00;
		SignalX_text->Style(*html).m_clrText = 0x000000;
		SignalX_updetected->Style(*html).m_clrText = 0x000000;
		SignalX->Style(*html).m_clrFill = 0x00ff00;
	}else if (ssoValue == 0){
		LineX->Style(*html).m_clrLine = 0xaaaaaa;
		SignalX_text->Style(*html).m_clrText = 0x000000;
		SignalX_updetected->Style(*html).m_clrText = 0x000000;
		SignalX->Style(*html).m_clrFill = 0xffffff;
	}else{
		if (html->mBody->m_strInterlockContainer.g_DisplayErrorMessages == 1 ){
//			call BadSignal("C" & ssoSignum & "SO")
//			g_MessageTimer = window.setInterval("ClearMessageBox()", g_GivenMessageTime)
		}
		LineX->Style(*html).m_clrLine = 0x0000ff;
		SignalX_text->Style(*html).m_clrText = 0x000000;
		SignalX_updetected->Style(*html).m_clrText = 0x000000;
		SignalX->Style(*html).m_clrFill = 0x0000ff;
	}
//end function
}
void vbsSignalBypreq(CHTML* html, char sbrValue, int sbrSignum)
{
/*
'called by:
'CM_FB_CxBYPREQ [event: onupdate]
function SignalBypreq(sbrValue, sbrSignum)
	on error resume next
	select case sbrValue
		case 0		page.all("Bypass_Combo" & sbrSignum).currentvalue = "-"
		case 1		page.all("Bypass_Combo" & sbrSignum).currentvalue = "1"
		case 2		page.all("Bypass_Combo" & sbrSignum).currentvalue = "0"
	end select
end function
*/
//'called by:
//'CM_FB_CxBYPREQ [event: onupdate]
//function SignalBypreq(sbrValue, sbrSignum)
//	on error resume next
	char tmp[16]; sprintf(tmp, "Bypass_Combo%d", sbrSignum);
	CTag* Bypass_ComboX = get_object(html, tmp);
	switch(sbrValue){
		case 0: /* TODO: Select value on ComboBox Bypass_ComboX "-" */ break;
		case 1: /* TODO: Select value on ComboBox Bypass_ComboX "1" */ break;
		case 2: /* TODO: Select value on ComboBox Bypass_ComboX "0" */ break;
	}
//end function
}

void vbsSetGateTitle(CHTML* html)
{
/*
'called by:
'CM_FB_GxALGID [event: onupdate, condition: g_UpdateAllowed = true]
function SetGateTitle
	on error resume next
	if not g_Gate1 = 99 then
		textGate1.value = GetGateLogic(CM_FB_G1ALGID.value)
	end if
	if not g_Gate2 = 99 then
		textGate2.value = GetGateLogic(CM_FB_G2ALGID.value)
	end if
	if not g_Gate3 = 99 then
		textGate3.value = GetGateLogic(CM_FB_G3ALGID.value)
	end if
	if not g_Gate4 = 99 then
		textGate4.value = GetGateLogic(CM_FB_G4ALGID.value)
	end if
end function
*/
//'called by:
//'CM_FB_GxALGID [event: onupdate, condition: g_UpdateAllowed = true]
//function SetGateTitle
//	on error resume next
	if (html->mBody->m_strInterlockContainer.g_Gate1 != 99){
		sprintf(get_object(html, "textGate1")->m_szScriptValue, "%s", vbsGetGateLogic(html, html->pipe.ValueB_H(get_object(html, "CM_FB_G1ALGID")->mVal)));
	}
	if (html->mBody->m_strInterlockContainer.g_Gate2 != 99){
		sprintf(get_object(html, "textGate2")->m_szScriptValue, "%s", vbsGetGateLogic(html, html->pipe.ValueB_H(get_object(html, "CM_FB_G2ALGID")->mVal)));
	}
	if (html->mBody->m_strInterlockContainer.g_Gate3 != 99){
		sprintf(get_object(html, "textGate3")->m_szScriptValue, "%s", vbsGetGateLogic(html, html->pipe.ValueB_H(get_object(html, "CM_FB_G3ALGID")->mVal)));
	}
	if (html->mBody->m_strInterlockContainer.g_Gate4 != 99){
		sprintf(get_object(html, "textGate4")->m_szScriptValue, "%s", vbsGetGateLogic(html, html->pipe.ValueB_H(get_object(html, "CM_FB_G4ALGID")->mVal)));
	}
//end function
}
void vbsBypassCheckbox(CHTML* html, char bcValue, int bcSignum)
{
/*
'called by:
'CM_FB_CxBYPPERM [event: onupdate]
function BypassCheckbox(bcValue, bcSignum)
	on error resume next
	page.all("Bypass_Checkbox" & bcSignum).value = bcValue
'v.204:
'	if  window.external.parentDocument.parentWindow.external.SecurityLevel-2 >= CM_FB_BYPPERMLCK.value then
'		if bcValue = "0" then
'			page.all("Bypass_Combo" & bcSignum).disabled = true
'		else
'			page.all("Bypass_Combo" & bcSignum).disabled = false
'		end if
'	else
'		page.all("Bypass_Combo" & bcSignum).disabled = true
'	end if
end function
*/
	char tmp[32]; sprintf(tmp, "Bypass_Checkbox%d", bcSignum);
	sprintf(get_object(html, tmp)->m_szScriptValue, "%d", bcValue);
}

CTagCmFbCxxInvert::CTagCmFbCxxInvert(int n)
{
	mParam = n;
}
void CTagCmFbCxxInvert::Matrix( LOCALS )
{
	char value = html.pipe.ValueB_H(this->mVal, 255);
	char name[16]; sprintf(name, "C%dINVERT", mParam);
	vbsSetNegator(&html, value, mParam, name);
	CTagTextArea::Matrix(hDC, html);
}

CTagCmFbGxNumInputs::CTagCmFbGxNumInputs(int n)
{
	mParam = n;
}
void CTagCmFbGxNumInputs::Matrix( LOCALS )
{
	int value = html.pipe.ValueI_H(this->mVal, -1);
	char name[16]; sprintf(name, "G%dNUMINPUTS", mParam);
	vbsNumInputsFirstcond(&html, value, name);
	CTagTextArea::Matrix(hDC, html);
}

CTagCmFbGxFistCond::CTagCmFbGxFistCond(int n)
{
	mParam = n;
}
void CTagCmFbGxFistCond::Matrix( LOCALS )
{
	int value = html.pipe.ValueI_H(this->mVal, 255);
	char name[16]; sprintf(name, "G%dFIRSTCOND", mParam);
	vbsNumInputsFirstcond(&html, value, name);
	CTagTextArea::Matrix(hDC, html);
}

CTagCmFbGxSO::CTagCmFbGxSO(int n)
{
	mParam = n;
}
void CTagCmFbGxSO::Matrix( LOCALS )
{
	char value = html.pipe.ValueB_H(this->mVal);

	switch(mParam)
	{
	case 1:vbsGateSO(&html, value, "Line13", "G1SO"); break;
	case 2:vbsGateSO(&html, value, "Line14", "G2SO"); break;
	case 3:vbsGateSO(&html, value, "Line15", "G3SO"); break;
	case 4:vbsGateSO(&html, value, "Line16", "G4SOFINAL"); break;
	}
	CTagTextArea::Matrix(hDC, html);
}

CTagCmFbUpDetected::CTagCmFbUpDetected(int n)
{
	mParam = n;
}
void CTagCmFbUpDetected::Matrix( LOCALS )
{
	vbsUpDetection(&html, mParam);
	CTagTextArea::Matrix(hDC, html);
}

CTagCmFbFirstUp::CTagCmFbFirstUp(int n)
{
	mParam = n;
}
void CTagCmFbFirstUp::Matrix( LOCALS )
{
	vbsFirstUpDetection(&html, mParam);
	CTagTextArea::Matrix(hDC, html);
}
CTagCmFbCXXSo::CTagCmFbCXXSo(int n)
{
	mParam = n;
}
void CTagCmFbCXXSo::Matrix( LOCALS )
{
	char value = html.pipe.ValueB_H(this->mVal);
	vbsSignalSO(&html, value, mParam);
	CTagTextArea::Matrix(hDC, html);
}
CTagCmFbCXXFl::CTagCmFbCXXFl(int n)
{
	mParam = n;
}
void CTagCmFbCXXFl::Matrix( LOCALS )
{
	char value = html.pipe.ValueB_H(this->mVal);
	vbsSignalFL(&html, value, mParam);
	CTagTextArea::Matrix(hDC, html);
}
CTagCmFbCXXByPreq::CTagCmFbCXXByPreq(int n)
{
	mParam = n;
}
void CTagCmFbCXXByPreq::Matrix( LOCALS )
{
	char value = html.pipe.ValueB_H(this->mVal);
	vbsSignalBypreq(&html, value, mParam);
	CTagTextArea::Matrix(hDC, html);
}
CTagCmFbGXAlgId::CTagCmFbGXAlgId(int n)
{
	mParam = n;
}
void CTagCmFbGXAlgId::Matrix( LOCALS )
{
	if (html.mBody->m_strInterlockContainer.g_UpdateAllowed)
		vbsSetGateTitle(&html);
	CTagTextArea::Matrix(hDC, html);
}
CNotBindedCombo::CNotBindedCombo(int n)
{
	mParam = n;
}
void CNotBindedCombo::Analiz ( ANALIZ )
{
	AnalizSPAN( this, html, teg );
}
void CNotBindedCombo::Kerdyk ( KERDYK )
{
#ifdef _DEBUG
    char * id = Style(html).NameID(html);
#endif
	CTag::Kerdyk( hWnd, html );
	m_pWnd = new CHTML_NBCB( html, this, hWnd );
}
CTagCmFbCXXByPerm::CTagCmFbCXXByPerm(int n)
{
	mParam = n;
}
void CTagCmFbCXXByPerm::Matrix( LOCALS )
{
	char value = html.pipe.ValueB_H(this->mVal);
	vbsBypassCheckbox(&html, value, mParam);
	CTagTextArea::Matrix(hDC, html);
}
