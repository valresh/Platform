Dim sSecondaryRef(10)
Dim sFormat
Dim ObjtxtSD

Public Sub GetStringsObject(ObjGrptxtSD)

    'This Function has been used only for getting string value from displays
    'Function will be called for each faceplate when (document.readyState = "complete")
     Set ObjtxtSD = ObjGrptxtSD

End Sub
'**********************************************************
'Purpose
'
'This function will be used by the faceplates whose centre panel should to be stretched in group detain display
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'
'**********************************************************

Public Sub ApplyLayout_Stretch(objIndPV)
On Error Resume Next
	GrpLowerPanel.Style.Top = vbNullString
	GrpLowerPanel.Style.bottom = "2px"
	GrpCenterPanel.Style.Top = GrpUpperPanel.Style.pixelTop + GrpUpperPanel.Style.pixelHeight + 2
	GrpCenterPanel.Style.bottom = (GrpLowerPanel.Style.pixelHeight + 15) & "px"
	GrpCenterPanel.Style.pixelHeight = Page.Style.pixelHeight - GrpLowerPanel.Style.pixelHeight - 	GrpUpperPanel.Style.pixelHeight - 10
	If Window.external.Application.CurrentDisplayPageType = 10 Then
		grpEU.style.PosTop = objIndPV.style.posTop - 2
		shaOUTIND.style.pixeltop = shaOUTIND.style.pixeltop + 7
		shaOUTIND.style.pixelheight = objIndPV.style.pixelheight + 20
	end if
End Sub
'**********************************************************
'Purpose
'
'This function will be used by the faceplates whose centre panel should not to be streched in group detain display
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub ApplyLayout_NoStretch(strBlockName)
On Error Resume Next
	GrpLowerPanel.Style.Top = vbNullString
	GrpLowerPanel.Style.bottom = "2px"
	If StrComp(ucase(strBlockName), "DIGITAL") = 0 Then
		GrpCenterPanel.Style.Top = vbNullString
		GrpCenterPanel.Style.bottom = (GrpLowerPanel.Style.pixelHeight + 15) & "px"
	ElseIf StrComp(ucase(strBlockName), "SWITCHA") = 0 Then
		GrpCenterPanel.style.top= vbNullString 
		GrpCenterPanel.style.bottom = (GrpLowerPanel.style.pixelHeight - 2) & "px"
	Else
		GrpCenterPanel.Style.Top = GrpUpperPanel.Style.pixelTop + GrpUpperPanel.Style.pixelHeight + 2
		GrpCenterPanel.Style.bottom = (GrpLowerPanel.Style.pixelHeight + 15) & "px"
	End If
End Sub

'**********************************************************
'Purpose
'
'Sets SP indicator and SP field tool tips for Preferred SP depending on the Server Wide Settings for Preferred SP. This function is used for Analog faceplates or faceplates which have Indicator in center panel
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub SetSpToolTip_CDA(objSD, objServerBits, objSPIndicator, objPSPIndicator, objSPAlpha, strCtrlBlockName, strDacaBlockName)
On Error Resume Next
    Dim bPSPFlag, bPVFORMATBadFlag
    Dim SpHil, SpLol
    bPSPFlag = objSD.QualityBad("CurrentPoint." & strCtrlBlockName & ".PREFSP")
    SpHil = objSD.DataValue("CurrentPoint." & strCtrlBlockName & ".SPHILM")
    SpLol = objSD.DataValue("CurrentPoint." & strCtrlBlockName & ".SPLOLM")
    bPVFORMATBadFlag = objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".pvformat")
        If Not bPVFORMATBadFlag Then
           SpHil = FormatNumber(SpHil, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".pvformat"))
           SpLol = FormatNumber(SpLol, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".pvformat"))
         Else
           SpHil = FormatNumber(SpHil, 2)
           SpLol = FormatNumber(SpLol, 2)
        End If

    strPSPToolTip1 = ObjtxtSD.children("txtsdSPHighLimit").value & SpHil & ObjtxtSD.children("txtsdEU").value & vbCr & ObjtxtSD.children("txtsdSPLowLimit").value & SpLol & ObjtxtSD.children("txtsdEU").value
    If Not bPSPFlag And objServerBits.Value = 1 Then 'Check if PSP exists and serverwide settings bit is on
        PSPValue = objPSPIndicator.Value
        If objSD.IsNaN("CurrentPoint." & strCtrlBlockName & ".PREFSP") Then
            objSPIndicator.Title = ObjtxtSD.children("txtsdSavePreferredSp").value & vbCr & vbCr & strPSPToolTip1
            objSPAlpha.Title = ObjtxtSD.children("txtsdSavePreferredSp").value
            objPSPIndicator.Style.visibility = "Hidden"
        Else
            objPSPIndicator.Style.visibility = "Visible"
            If Not bPVFORMATBadFlag Then
                psp = FormatNumber(PSPValue, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".pvformat"))
                SP = FormatNumber(objSPAlpha.Value, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".pvformat"))
            Else
                psp = FormatNumber(PSPValue, 2)
                SP = FormatNumber(objSPAlpha.Value, 2)
            End If
            strPSPToolTip = ObjtxtSD.children("txtsdCurrentSP").value & SP & ObjtxtSD.children("txtsdEU").value & vbCr & ObjtxtSD.children("txtsdPreferredSP").value & psp & ObjtxtSD.children("txtsdEU").value & vbCr & ObjtxtSD.children("txtsdSPHighLimit").value & SpHil & ObjtxtSD.children("txtsdEU").value & vbCr & ObjtxtSD.children("txtsdSPLowLimit").value & SpLol & ObjtxtSD.children("txtsdEU").value
            strPSPToolTipp = ObjtxtSD.children("txtsdCurrentSP").value & SP & ObjtxtSD.children("txtsdEU").value & vbCr & ObjtxtSD.children("txtsdPreferredSP").value & psp & ObjtxtSD.children("txtsdEU").value
            objSPIndicator.Title = strPSPToolTip
            objSPAlpha.Title = strPSPToolTipp
        End If
    Else
        objSPIndicator.Title = strPSPToolTip1
        objSPAlpha.Title = vbNullString
        objPSPIndicator.Style.visibility = "Hidden"
    End If
End Sub

'**********************************************************
'Purpose
'
'Sets SP indicator and SP field tool tips for Preferred SP depending on the Server Wide Settings for Preferred SP. This function is used only for Timer block faceplates
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub SetSpToolTip_Timer(objSD, objServerBits, objSPAlpha, strBlockName)
On Error Resume Next
    bPSPFlag = objSD.QualityBad("CurrentPoint." & strBlockName & ".PREFSP")
    If Not bPSPFlag And objServerBits.value = 1 Then 'Check if PSP exists and serverwide settings bit is on
        Dim PSPValue
        PSPValue = objSD.DataValue("CurrentPoint." & strBlockName & ".PREFSP")
        If PSPValue = -1 Then
            objSPAlpha.Title = ObjtxtSD.children("txtsdSavePreferredSp").value
        Else
            objSPAlpha.Title = ObjtxtSD.children("txtsdCurrentSP").value & objSPAlpha.Value & vbCr & ObjtxtSD.children("txtsdPreferredSP").value & PSPValue
        End If
    Else
        objSPAlpha.Title = vbNullString
    End If
End Sub

'**********************************************************
'Purpose
'
'Sets SP indicator and SP field tool tips for Non CDA faceplates like EHG, FF
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub SetSpToolTip_NonCDA(objSD, objSPIndicator, strBlockName, strSPHiParameter, strSPLoParameter)
On Error Resume Next
    SpHil = objSD.DataValue("CurrentPoint." & strBlockName & "." & strSPHiParameter)
    SpLol = objSD.DataValue("CurrentPoint." & strBlockName & "." & strSPLoParameter)
    strOpToolTip = ObjtxtSD.children("txtsdSPHighLimit").value & SpHil & ObjtxtSD.children("txtsdEU").value & vbCr & ObjtxtSD.children("txtsdSPLowLimit").value & SpLol & ObjtxtSD.children("txtsdEU").value
    objSPIndicator.Title = strOpToolTip
End Sub

'**********************************************************
'Purpose
'
'Sets OP indicator tool tip for OP Ranges for CDA, SCADA & EHG points
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version

'**********************************************************
Public Sub setOpToolTip(objSD, objOPIndicator, strOPHiParameter, strOPLoParameter, strCtrlBlockName)
On Error Resume Next
    If strCtrlBlockName = vbNullString Then ' For SCADA points, block name should be vbNullString
        OpHil = objSD.DataValue("CurrentPoint.OPHI")
        OpLol = objSD.DataValue("CurrentPoint.OPLO")
    Else
        OpHil = objSD.DataValue("CurrentPoint." & strCtrlBlockName & "." & strOPHiParameter)
        OpLol = objSD.DataValue("CurrentPoint." & strCtrlBlockName & "." & strOPLoParameter)
    End If
    strOpToolTip = ObjtxtSD.children("txtsdOPHighLimit").value & OpHil &  ObjtxtSD.children("txtsdPrecentageUnits").value & vbCr & ObjtxtSD.children("txtsdOPLowLimit").value & OpLol & ObjtxtSD.children("txtsdPrecentageUnits").value
    objOPIndicator.Title = strOpToolTip
End Sub

Public Sub SetTags(objSD,objPicRedTag, objPicOperTag ,objPicSecRedTag,strCtrlBlockName)
On Error Resume Next
Dim bSecRedTag, bOperTagFlag
bSecRedTag = False
    If StrComp(UCase(strCtrlBlockName), "{%FFBNAME%}") = 0 Then ' For EHG Points
        If gbPageComplete Then
            If objSD.DataValue("CurrentPoint." & strCtrlBlockName & ".REDTAG") = 1 Then
                objPicRedTag.Style.visibility = "visible"
            Else
                objPicRedTag.Style.visibility = "Hidden"
            End If
        End If
    Else
        bOperTagFlag = objSD.QualityBad("CurrentPoint." & strCtrlBlockName & ".OPERTAG")
        Dim boolRedTag, boolOperTag, boolSecRedTag
        boolRedTag = objSD.DataValue("CurrentPoint." & strCtrlBlockName & ".REDTAG")
        boolOperTag = objSD.DataValue("CurrentPoint." & strCtrlBlockName & ".OPERTAG")
        
        objPicOperTag.Style.visibility = "Hidden"
        objPicRedTag.Style.visibility = "Hidden"
        objPicSecRedTag.Style.visibility = "Hidden"
        If StrComp(UCase(strCtrlBlockName), "DEVCTLA") = 0 Or StrComp(UCase(strCtrlBlockName), "DEVCTL") = 0 Or StrComp(UCase(strCtrlBlockName), "POSPROPA") = 0 Then
            For i = 1 To 3
                If objSD.DataValue("{%SECPNTNAME" & i & "%}.{%SECBLOCKNAME" & i & "%}.REDTAG") = 1 Then
                     strSecRedTagToolTip = strSecRedTagToolTip & vbCr & sSecondaryRef(i)
                     boolSecRedTag = 1
                End If
            Next
        Else
			boolSecRedTag = objSD.DataValue("{%SECPNTNAME%}.{%SECBLOCKNAME%}.REDTAG")
        End If
        If boolRedTag = 1 Then
                objPicRedTag.Style.visibility = "visible"
        ElseIf boolSecRedTag = 1 Then
                objPicSecRedTag.Style.visibility = "visible"
                objPicSecRedTag.Title = ObjtxtSD.children("txtsdRedTagSeton").value & vbCr & objSD.DataValue("CurrentPoint." & strCtrlBlockName & ".SECBLOCKID")
        ElseIf Not bOperTagFlag Then
            If boolOperTag = 1 Then
                objPicOperTag.Style.visibility = "Visible"
                objPicOperTag.Title = objSD.DataValue("CurrentPoint." & strCtrlBlockName & ".OPERTAGDESC")
            End If
        End If
    End If
end sub

'**********************************************************
'Purpose
'
'Updates the SECPNTNAME, SECBLOCKNAME custom parameters using DisplayDataRepository object passed from display
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub GetSecData(objSD, i, strBlockName)
On Error Resume Next
	dim strSecBlkId, strSECBLOCKParam
	if i > 0 then
		if strcomp(ucase(strBlockName),"POSPROPA") = 0 then
			strSECBLOCKParam = ".SECBLOCKIDPOS["
		else
			strSECBLOCKParam = ".SECBLOCKID["
		end if

		strSecBlkId = objSD.DataValue("CurrentPoint." & strBlockName & strSECBLOCKParam & i & "]")
		If strSecBlkId <> vbNullString Then
			sSecondaryRef(i) = strSecBlkId
			sSecondarySplit = Split(sSecondaryRef(i), ".")
			If UBound(sSecondarySplit) = 1 Then
				DisplayDataRepository.putvalue "SECPNTNAME" & i, sSecondarySplit(0)
				DisplayDataRepository.putvalue "SECBLOCKNAME" & i, sSecondarySplit(1)
			End If
		End If
	else
		strSecBlkId = objSD.DataValue("CurrentPoint." & strBlockName & ".SECBLOCKID")
		If strSecBlkId <> vbNullString Then
			sSecondaryRef(i) = strSecBlkId
			sSecondarySplit = Split(sSecondaryRef(i), ".")
			If UBound(sSecondarySplit) = 1 Then
				DisplayDataRepository.putvalue "SECPNTNAME", sSecondarySplit(0)
				DisplayDataRepository.putvalue "SECBLOCKNAME", sSecondarySplit(1)
			End If
		End If
	End If
End Sub

'**********************************************************
'Purpose
'
'Splits the PVRAWREF parameter into CM Name & block name and updates the custom parameters for PointName and AICHANNELNAME using DisplayDataRepository object sent through display file
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub SetPVRAWRef(objSD,strCtlBlockName)
On Error Resume Next
	Dim sPVRAWRef, sPVRAWRefPP
    sPVRAWRef = objSD.DataValue("CurrentPoint." & strCtlBlockName & ".PVRAWREF")
    sPVRAWRefPP = Split(sPVRAWRef, ".")
    If UBound(sPVRAWRefPP) = 1 Then
        DisplayDataRepository.putvalue "PointName", sPVRAWRefPP(0)
        DisplayDataRepository.putvalue "AICHANNELNAME", sPVRAWRefPP(1)
    End If
End Sub

'**********************************************************
'Purpose
'
'Sets PVRAW tool tip for PV Indicator and PV field
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub SetPVRAW(objSD, objPVIndicator, objPVAlpha, objPVManAlpha, strCtrlBlockName)
On Error Resume Next
    Dim fPVRAW, bPVRAWFlag
    If StrComp(UCase(strCtrlBlockName), "{%FFBNAME%}") = 0 Then
		If objSD.IsNaN("CurrentPoint.{%FFBNAME%}.PVRAW") then
			fPVRAW = "NaN"
		else
			fPVRAW = objSD.DataValue("CurrentPoint.{%FFBNAME%}.PVRAW")
			strPVRAWToolTip = ObjtxtSD.children("txtsdPV").value & objPVAlpha.innerText & ObjtxtSD.children("txtsdEU").value & vbCr & ObjtxtSD.children("txtsdPVRAW").value &  fPVRAW & " " & ObjtxtSD.children("txtsdPrecentageUnits").value
		end if
		objPVIndicator.Title = strPVRAWToolTip
		objPVAlpha.Title = strPVRAWToolTip
		objPVManAlpha.Title = strPVRAWToolTip
    Else
        bPVRAWFlag = objSD.QualityBad("CurrentPoint." & strCtrlBlockName & ".PVRAWREF")
        If Not bPVRAWFlag And (objSD.DataValue("CurrentPoint." & strCtrlBlockName & ".PVRAWREF") <> vbNullString) Then
            Dim strPVRawUnits
            If objSD.IsNaN("{%PointName%}.{%AICHANNELNAME%}.PVRAW") Then
                fPVRAW = "NaN"
                strPVRawUnits = ""
            Else
                fPVRAW = objSD.DataValue("{%PointName%}.{%AICHANNELNAME%}.PVRAW")
                strPVRawUnits = SetPVRawUnits(objSD)
            End If
            strPVRAWToolTip = ObjtxtSD.children("txtsdPV").value  & objPVAlpha.innerText & ObjtxtSD.children("txtsdEU").value & vbCr & ObjtxtSD.children("txtsdPVRAW").value  & fPVRAW & " " & strPVRawUnits
            objPVIndicator.Title = strPVRAWToolTip
            objPVAlpha.Title = strPVRAWToolTip
            objPVManAlpha.Title = strPVRAWToolTip
        Else
            objPVIndicator.Title = ""
            objPVAlpha.Title = ""
            objPVManAlpha.Title = ""
        End If
    End If
End Sub

'**********************************************************
'Purpose
'
'Returns the units for PVRAW depending on the SENSOR type of using the AICHANNEL reference. This is getting called from SetPVRAW function
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Function SetPVRawUnits(objSD)
On Error Resume Next
Select Case objSD.DataValue("{%PointName%}.{%AICHANNELNAME%}.SENSRTYP")
    Case 0
        '1_5_V
        SetPVRawUnits = ObjtxtSD.children("txtsdPrecentageUnits").value
    Case 1
        '0_5_V
        SetPVRawUnits = ObjtxtSD.children("txtsdPrecentageUnits").value
    Case 5
        'P4_2_V
        SetPVRawUnits = ObjtxtSD.children("txtsdPrecentageUnits").value
    Case 2
        '0_100_MV
        SetPVRawUnits = ObjtxtSD.children("txtsdMilliVolts").value
    Case 3
        'THERMCPL
        SetPVRawUnits = ObjtxtSD.children("txtsdMicroVolts").value
    Case 4
        'RTD
        SetPVRawUnits = ObjtxtSD.children("txtsdMilliOhms").value
    Case 6
        'SLIDEWIRE
        SetPVRawUnits = ObjtxtSD.children("txtsdSLIDWIRE").value
    Case 8
        'SPT_DP
        SetPVRawUnits = ObjtxtSD.children("txtsdpsi").value
    Case 9
        'SPT_GP
        SetPVRawUnits = ObjtxtSD.children("txtsdpsig").value
    Case 10
        'SPT_AP
        SetPVRawUnits = ObjtxtSD.children("txtsdpsia").value
    Case 11
        'STT
        SetPVRawUnits = ObjtxtSD.children("txtsddegC").value
    Case 12
        'SFM
        SetPVRawUnits = ObjtxtSD.children("txtsdMagneticFlow").value
    default
		SetPVRawUnits = ObjtxtSD.children("txtsdNoEU").value
End Select
End Function

'**********************************************************
'Applies PVFORMAT only for SP, PV, OP, PVEUHi, PVEULo and DACA PV
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Sub PVFormatUpdate(objSD, strDacaBlockName)
On Error Resume Next

	Dim PVFormat
	PVFormat = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVFORMAT")
	
	If Not objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVFORMAT")  Then
		sFormat = "%." & PVFormat & "f"
	else
		sFormat = "%.2f"
		
	End If
		
	ConAlpPV_GSH.numericdisplayformat = sFormat
	ConAlpSP.numericdisplayformat = sFormat
	ConAlpPSP_GSH.numericdisplayformat = sFormat
	ConAlpPVEUHi_GSH.numericdisplayformat = sFormat
	ConAlpPVEULo_GSH.numericdisplayformat = sFormat
	ConAlpPVMan_GSH.numericdisplayformat = sFormat
	ConAlpSPHi_GSH.numericdisplayformat=sFormat
	ConAlpSPLo_GSH.numericdisplayformat=sFormat
	ApplyPVFormat objSD, strDacaBlockName
End Sub

'**********************************************************
'This applies PVFORMAT for intemediate EU Scales. This will be called from PVFormatUpdate
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub ApplyPVFormat(objSD, strDacaBlockName)
On Error Resume Next

    For i = 1 To 3
        Document.getElementById("ConAlpPVEU" & i & "_GSH").numericdisplayformat = sFormat
        Document.getElementById("ConAlpPVEU" & i & "_GSH").Value = ConAlpPVEULo_GSH.Value + ((ConAlpPVEUHi_GSH.Value - ConAlpPVEULo_GSH.Value) * i) / 4
    Next
    
    Call EUdescWidth
End Sub

'**********************************************************
'Purpose
'
'Adjusts the EUDESC width depending on the Serverwide settings and PVFORMAT. This will be called from ApplyPVFORMAT and EU_ServerBitsUpdate
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub EUdescWidth()
On Error Resume Next
    If ConAlpEUTickMarksServerBits_GSH.Value = 1 Then
        If Len(ConAlpPVEU2_GSH.Value) > 8 Then  'compensate for big/small numbers
           ConAlpEUDesc_GSH.Style.pixelWidth = 45 - (Len(ConAlpPVEU2_GSH.Value) - 8) * 10
                If ConAlpEUDesc_GSH.Style.pixelWidth < 20 Then
                    ConAlpEUDesc_GSH.Style.pixelWidth = 20
                End If
        ElseIf Len(ConAlpPVEU2_GSH.Value) < 9 Then
            ConAlpEUDesc_GSH.Style.pixelWidth = 45 + (7 - Len(ConAlpPVEU2_GSH.Value)) * 10
            If ConAlpEUDesc_GSH.Style.pixelWidth > 62 Then
                ConAlpEUDesc_GSH.Style.pixelWidth = 62
            End If
        End If
    End If
End Sub

'**********************************************************
'Purpose
'
'For blocks using DACA block in CM, the visibility of PV field will be chanded depending on the PVSOURCE parameter.
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub PVSOURCEUpdate(objSD, objPV, objPVMAN, strDacaBlockName)
On Error Resume Next
    If Not objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVSOURCE") Then
        if sd.DataValue("CurrentPoint." & strDacaBlockName & ".PVSOURCE") = 1 Then
            'PVSOURCE is MAN
            objPVMAN.Style.visibility = "inherit"
            objPV.Style.visibility = "hidden"
        Else
            objPVMAN.Style.visibility = "hidden"
            objPV.Style.visibility = "inherit"
        End If
   End if
End Sub

'**********************************************************
'Purpose
'
'Sets tool tip for Alarm bar
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub RecToolTip(objSD, objRecTT, strDacaBlockName)
On Error Resume Next
    Dim strHH, strHi, strLL, strLo
    pvllalm = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVLLALM.TP")
    pvloalm = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVLOALM.TP")
    pvhialm = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVHIALM.TP")
    pvhhalm = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVHHALM.TP")
    pvllalmp = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVLLALM.PR")
    pvloalmp = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVLOALM.PR")
    pvhialmp = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVHIALM.PR")
    pvhhalmp = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVHHALM.PR")
    If pvhhalmp = 0 Or pvhhalmp = 1 Then
        strHH = vbNullString
    ElseIf objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVHHALM.TP") Or Ucase(pvhhalm) = Ucase("1.#QNaN") Then
        strHH = vbNullString
    Else
        strHH = FormatNumber(pvhhalm, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVFORMAT"))
        strHH = ObjtxtSD.children("txtsdHighHigh").value & "   " & strHH & vbCr
    End If
    If pvhialmp = 0 Or pvhialmp = 1 Then
        strHi = vbNullString
    ElseIf objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVHIALM.TP") Or Ucase(pvhialm) = Ucase("1.#QNaN") Then
        strHi = vbNullString
    Else
        strHi = FormatNumber(pvhialm, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVFORMAT"))
        strHi = ObjtxtSD.children("txtsdHigh").value & "           " & strHi & vbCr
    End If
    If pvllalmp = 0 Or pvllalmp = 1 Then
        strLL = vbNullString
    ElseIf objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVLLALM.TP") Or Ucase(pvllalm) = Ucase("1.#QNaN") Then
        strLL = vbNullString
    Else
        strLL = FormatNumber(pvllalm, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVFORMAT"))
        strLL = ObjtxtSD.children("txtsdLowLow").value & "     " & strLL
    End If
    If pvloalmp = 0 Or pvloalmp = 1 Then
        strLo = vbNullString
    ElseIf objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVLOALM.TP")Or Ucase(pvloalm) = Ucase("1.#QNaN") Then
        strLo = vbNullString
    Else
        strLo = FormatNumber(pvloalm, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVFORMAT"))
        strLo = ObjtxtSD.children("txtsdLow").value & "            " & strLo & vbCr
    End If
    If strHH = vbNullString And strHi = vbNullString And strLL = vbNullString And strLo = vbNullString Then
        objRecTT.Title = vbNullString
    Else
        objRecTT.Title = ObjtxtSD.children("txtsdPVAlarmLimits").value & "    " & vbCr & strHH & strHi & strLo & strLL
    End If
End Sub

'**********************************************************
'Purpose
'
'Sets tool tip for OUTIND shape
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub SetOUTINDToolTip(objshaOUTIND, iDirect, iReverse)
On Error Resume Next
    If objshaOUTIND.Value = iDirect Then
        objshaOUTIND.Title = ObjtxtSD.children("txtsdDirectControl").value
    ElseIf objshaOUTIND.Value = iReverse Then
        objshaOUTIND.Title = ObjtxtSD.children("txtsdReverseControl").value
    Else
        objshaOUTIND.Title = vbNullString
    End If
End Sub

'**********************************************************
'Purpose
'
'Updates the Simulation text depending on the SIMRUN parameter
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub SimStateUpdate(objsd,objEnumSimulationState, objTxtSIM)
On Error Resume Next
    
    objTxtSIM.Style.visibility = "hidden"
    objEnumSimulationState.Style.visibility = "Hidden"
    
    if objsd.DataValue("currentpoint.SIMSTATE") = 2 then
        'SIMSTATE is SIMFREEZE
        objTxtSIM.Style.visibility = "Hidden": objEnumSimulationState.Style.visibility = "visible"
    Elseif objsd.DataValue("currentpoint.SIMSTATE") = 1 then
        'SIMSTATE is SIMRUN
        objTxtSIM.Style.visibility = "visible": objEnumSimulationState.Style.visibility = "Hidden"
    End If
End Sub

'**********************************************************
'Purpose
'
'Changes the visibility of the Intermediate scale ranges depending on the server wide settings
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub EU_ServerBitsUpdate()
On Error Resume Next
    If ConAlpEUTickMarksServerBits_GSH.Value = 0 Then
        ConAlpPVEU1_GSH.Style.visibility = "Hidden"
        ConAlpPVEU2_GSH.Style.visibility = "Hidden"
        ConAlpPVEU3_GSH.Style.visibility = "Hidden"
        ConAlpEUDesc_GSH.Style.pixelWidth = 95
        rectEUValueProtect_GSH.Style.visibility = "Hidden"
        ConAlpEUDesc_GSH.Style.TextAlign = "right"
    Else
        ConAlpPVEU1_GSH.Style.visibility = "Visible"
        ConAlpPVEU2_GSH.Style.visibility = "Visible"
        ConAlpPVEU3_GSH.Style.visibility = "Visible"
        ConAlpEUDesc_GSH.Style.pixelWidth = 45
        rectEUValueProtect_GSH.Style.visibility = "Visible"
        ConAlpEUDesc_GSH.Style.TextAlign = "left"
    End If
    Call EUdescWidth
End Sub

'**********************************************************
'Purpose
'
'If the value of the object is NONE, the object will be hidden otherwise make it visible
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub Alarm_None(ObjAlarmObjName)
On Error Resume Next
If StrComp(ObjAlarmObjName.Value, ObjtxtSD.children("txtsdNONE").value, 1 ) = 0 Or StrComp(ObjAlarmObjName.Value, ObjtxtSD.children("txtsdNOALARM").value, 1) = 0 Then
    ObjAlarmObjName.Style.visibility = "hidden"
Else
    ObjAlarmObjName.Style.visibility = "visible"	
End If
End Sub

'**********************************************************
'Purpose
'
'Changes text visibility for Mode if the Mode is not same as Normal Mode
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub OffNormalModeState(objSD, objCboMode, objTxtMode, strBlockName, strNormModeParamName)
On Error Resume Next
    Dim strMode, strNormMode
    strMode = objCboMode.Value
    strNormMode = objSD.DataValue("CurrentPoint." & strBlockName & "." & strNormModeParamName)
    If strcomp(ucase(strNormMode), ucase(ObjtxtSD.children("txtsdNONE").value)) <> 0 and  StrComp(ucase(strMode), ucase(strNormMode)) <> 0 Then
        objTxtMode.styleclass = "Txt_Label_Off_Normal"
        objTxtMode.Title = ObjtxtSD.children("TxtsdOffNormMode").value & strNormMode & ObjtxtSD.children("TxtsdOffNormModebracket").value
    Else
        objTxtMode.styleclass = "Txt_Label_Normal"
        objTxtMode.Title = vbNullString 
    End If
End Sub

'**********************************************************
'Purpose
'
'Changes text visibility for Mode attribute if the Mode attribure is not same as Normal Mode
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub OffNormalModeAttrState(objSD, objCboModeAttr, objTxtModeAttr1, objTxtModeAttr2, strBlockName, strNormModeAttrParamName)
On Error Resume Next
    Dim strModeAttr, strNormModeAttr
    strModeAttr = objCboModeAttr.Value
    strNormModeAttr = objSD.DataValue("CurrentPoint." & strBlockName & "." & strNormModeAttrParamName)
    If strcomp(ucase(strNormModeAttr), ucase(ObjtxtSD.children("txtsdNONE").value)) <> 0 and StrComp(ucase(strModeAttr), ucase(strNormModeAttr)) <> 0 Then
        objTxtModeAttr1.styleclass = "Txt_Label_Off_Normal"
        objTxtModeAttr2.styleclass = "Txt_Label_Off_Normal"
        objTxtModeAttr1.Title = ObjtxtSD.children("TxtsdOffNormModeAttr").value & strNormModeAttr & ObjtxtSD.children("TxtsdOffNormModebracket").value
        objTxtModeAttr2.Title = ObjtxtSD.children("TxtsdOffNormModeAttr").value & strNormModeAttr & ObjtxtSD.children("TxtsdOffNormModebracket").value
    Else
        objTxtModeAttr1.styleclass = "Txt_Label_Normal"
        objTxtModeAttr2.styleclass = "Txt_Label_Normal"
        objTxtModeAttr1.Title = vbNullString
        objTxtModeAttr2.Title = vbNullString
    End If
End Sub

'**********************************************************
'Purpose
'
'Sets the tool tip for EUDESC tool tip
'
'History
'**********************************************************
Public Sub EUDescToolTip(objEUDesc)
On Error Resume Next
    If Trim(objEUDesc.Value) <> vbNullString Then
        objEUDesc.Title = ObjtxtSD.children("txtsdEUDESC").value & objEUDesc.Value
    End If
End Sub

'**********************************************************
'Purpose
'
'Sets the Height of for EUDESC to accomodate only two lines.
'
'History
'Feb 01, 2008 - Vidyasagar Anumolu - Created the Initial Version
'**********************************************************
Public Sub SetEUDescHeight(objEUDesc)
On Error Resume Next
    objEUDesc.Style.pixelHeight = 34
End Sub
