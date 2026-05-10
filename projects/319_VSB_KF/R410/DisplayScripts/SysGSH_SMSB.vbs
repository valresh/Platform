'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' Included the contents of standard SysGSH.vbs
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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



Public Sub ApplyLayout_Stretch(intIndHeight)

On Error Resume Next

	GrpLowerPanel.Style.Top = vbNullString

	GrpLowerPanel.Style.bottom = "2px"

	GrpCenterPanel.Style.Top = GrpUpperPanel.Style.pixelTop + GrpUpperPanel.Style.pixelHeight + 2

	GrpCenterPanel.Style.bottom = (GrpLowerPanel.Style.pixelHeight + 15) & "px"

	GrpCenterPanel.Style.pixelHeight = Page.Style.pixelHeight - GrpLowerPanel.Style.pixelHeight - 	GrpUpperPanel.Style.pixelHeight - 10

	If Window.external.Application.CurrentDisplayPageType = 10 Then

		grpEU.Style.pixelTop = grpEU.Style.pixelTop + 8

		shaOUTIND.style.pixeltop = shaOUTIND.style.pixeltop + 7

		shaOUTIND.style.pixelheight = intIndHeight + 20

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

		sFormat = "%." & PVFormat-1 & "f"

	else

		sFormat = "%.2f"

		

	End If

		

	ConAlpPV_GSH.numericdisplayformat = sFormat
	ConAlpPV_GSH002.numericdisplayformat = sFormat
	ConAlpPV_GSH003.numericdisplayformat = sFormat

	ConAlpSP.numericdisplayformat = sFormat

	ConAlpPSP_GSH.numericdisplayformat = sFormat

	ConAlpPVEUHi_GSH.numericdisplayformat = sFormat

	ConAlpPVEULo_GSH.numericdisplayformat = sFormat

	ConAlpPVMan_GSH.numericdisplayformat = sFormat

	ConAlpSPHi_GSH.numericdisplayformat=sFormat

	ConAlpSPLo_GSH.numericdisplayformat=sFormat
	
	ConAlpTP.numericdisplayformat=sFormat

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
		'SM FGS Standard Builds FP
		ConAlpEUDescRAW_GSH.Style.pixelWidth = ConAlpEUDesc_GSH.Style.pixelWidth 
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

            objPVMAN.Style.visibility = "visible"

            objPV.Style.visibility = "hidden"

        Else

            objPVMAN.Style.visibility = "hidden"

            objPV.Style.visibility = "visible"

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
'Jul 22, 2015 - Vladimir Kasabov - modified for SM CDA integration

'**********************************************************

Public Sub RecToolTip(objSD, objRecTT, strDacaBlockName)

On Error Resume Next

    Dim strHH, strHi, strLL, strLo

    pvllalm = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVLLALMTP")

    pvloalm = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVLOALMTP")

    pvhialm = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVHIALMTP")

    pvhhalm = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVHHALMTP")

    pvllalmp = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVLLALMPR")

    pvloalmp = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVLOALMPR")

    pvhialmp = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVHIALMPR")

    pvhhalmp = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVHHALMPR")

    If pvhhalmp = 1 Or pvhhalmp = 2 Then

        strHH = vbNullString

    ElseIf objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVHHALMTP") Or pvhhalm = "NaN" Then

        strHH = vbNullString

    Else

        strHH = FormatNumber(pvhhalm, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVFORMAT"))

        strHH = ObjtxtSD.children("txtsdHighHigh").value & "   " & strHH & vbCr

    End If

    If pvhialmp = 1 Or pvhialmp = 2 Then

        strHi = vbNullString

    ElseIf objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVHIALMTP") Or pvhialm = "NaN" Then

        strHi = vbNullString

    Else

        strHi = FormatNumber(pvhialm, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVFORMAT"))

        strHi = ObjtxtSD.children("txtsdHigh").value & "           " & strHi & vbCr

    End If

    If pvllalmp = 1 Or pvllalmp = 2 Then

        strLL = vbNullString

    ElseIf objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVLLALMTP") Or pvloalm = "NaN" Then

        strLL = vbNullString

    Else

        strLL = FormatNumber(pvllalm, objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVFORMAT"))

        strLL = ObjtxtSD.children("txtsdLowLow").value & "     " & strLL

    End If

    If pvloalmp = 1 Or pvloalmp = 2 Then

        strLo = vbNullString

    ElseIf objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVLOALMTP")Or pvloalm = "NaN" Then

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

        ConAlpEUDesc_GSH.Style.pixelWidth = ConAlpPVEUHi_GSH.style.pixelLeft+ConAlpPVEUHi_GSH.style.pixelWidth - ConAlpEUDesc_GSH.Style.pixelLeft 'SMSB custom

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
	'SM FGS Standard Builds FP
	ConAlpEUDescRAW_GSH.Style.pixelWidth =  ConAlpEUDesc_GSH.Style.pixelWidth
	ConAlpEUDescRAW_GSH.Style.TextAlign =  ConAlpEUDesc_GSH.Style.TextAlign

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

'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
' End standard SysGSH.vbs
'++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'**********************************************************************************
' SM Standard Builds Functions
'
' SM ESD SB Faceplates
'**********************************************************************************

dim gbPageComplete, strDacaBlockName, iQualityOffScan, iQualityBad, iQualityAccessNotPermitted
iQualityGood = 192
iQualityOffScan = 8
iQualityBad = 4
iQualityAccessNotPermitted = 1024

'strDacaBlockName = "ai"
gbPageComplete = false

Sub ApplyLayout()
Dim oInd
on error resume next 	
	if typename(document.getElementById("ConIndPV"))<>"Nothing" then
		set oInd = document.getElementById("ConIndPV")
	elseif typename(document.getElementById("ConIndPVAI1"))<>"Nothing" then
		set oInd = document.getElementById("ConIndPVAI1")
	elseif typename(document.getElementById("IndPVAI1"))<>"Nothing" then
		set oInd = document.getElementById("IndPVAI1")
	else
		document.getElementById("GrpLowerPanel").style.top = "" 
		document.getElementById("GrpLowerPanel").style.bottom = "2px"
		document.getElementById("GrpCenterPanel").style.top = "" 
		document.getElementById("GrpCenterPanel").style.bottom = (document.getElementById("GrpLowerPanel").style.pixelHeight + 15) & "px"
	end if
	call ApplyLayout_Stretch(oInd.style.pixelheight)
End Sub
'==================================================================================
' Common FP init subroutines - applicable to SM SB (ESD/FGS) faceplates
'==================================================================================
document.attachEvent "onreadystatechange", getref("smsb_ondocstatechange")
'
sub smsb_ondocstatechange 'Updated 2016-07-06
on error resume next
	tmpstr=""
	If (document.readyState = "complete") then
		'Initialize descriptions for COM block status indication
		for i=1 to 8  'up to 6 status lines can fit in FP lower pane
			sBlk = DisplayDataRepository.GetValue("Blk" & i) 
			sVal = DisplayDataRepository.GetValue("Blk" & i & "Desc") 
			if sBlk<> "" and sVal <> "" then
				'if instr(sBlk,"_")>0 and instr(sVal,"LL")=0 and instr(sVal,"HH")=0 and instr(sVal,"Low")=0 and instr(sVal,"High")=0 then
				''SM Block names for voted typicals				
				'	sNum = split(sBlk,"_")
				'	tmpstr=tmpstr & i
				'	document.getElementById("TxtSt"&i).value = sNum(1)
				'	if len(tmpstr)>1 and typename(document.getElementById("TxtSt"&tmpstr)) <> "Nothing" then
				'		document.getElementById("TxtSt"&tmpstr).value = sVal
				'		tmpstr=""
				'	end if
				'else
					document.getElementById("TxtSt"&i).value = sVal
				'end if
			end if
			'Optionally add some padding for the text
			'document.getElementById("TxtSt"&i).style.paddingLeft = "4px"
		next
	end if
	'Attach to onpagecomplete
	document.getElementById("page").attachEvent "onpagecomplete", getref("smsb_onpagecomplete")
end sub
'
sub smsb_onpagecomplete 'Updated 2016-07-06
on error resume next
		Dim arrBlkNames(6)
		Dim arrBlkDesc(6)
		Dim arrBlkQualityCfgErr(6)
		'Get Block names and descriptions (statically defined in FP display custom properties)
		for i = 1 to ubound(arrBlkNames)
			arrBlkNames(i-1) = DisplayDataRepository.GetValue("Blk" & i)
			arrBlkDesc(i-1) = DisplayDataRepository.GetValue("Blk" & i & "Desc")
			arrBlkQualityCfgErr(i-1) = ((document.getElementById("ScConAlpTag" & i).quality and iQualityBad)=iQualityBad and (document.getElementById("ScConAlpTag" & i).quality and 8)<>8)
		next
		y0 = Lin1.style.pixelTop+4 'Separator line at the top of the FP lower pane
		k=0 'SM COM block status line position offset index
		for i=1 to 6'ubound(arrBlkNames)
			if Not arrBlkQualityCfgErr(i-1) then 'SM block is avalable in FLD
				'Position blocks from the top of the FP lower pane
				document.getElementById("GrpCOM"&i).style.visibility = "inherit"
				document.getElementById("GrpCOM"&i).style.pixelTop = y0+k*(document.getElementById("GrpCOM"&i).style.pixelHeight-2)
				'Special behavior for MOS status indication
				if instr(UCase(arrBlkNames(i-1)),"MOS")>0 then
					'document.getElementById("GrpCOM"&i).attachEvent "onclick", getref("GrpCom_click")
					'document.getElementById("GrpCOM"&i).style.cursor = "hand"
					'document.getElementById("GrpCOM"&i).title = "Click to invoke MOS commands"
					if typename(document.getElementById("pbMOS")) <> "Nothing" then
						if sd.DataExists("CurrnetPoint.MOSS.PV") and sd.QualityGood("CurrnetPoint.MOSS.PV") then
							document.getElementById("pbMOS").style.visibility = "inherit"
							document.getElementById("pbMOS").style.pixelTop = document.getElementById("GrpCOM"&i).style.pixelTop - 2
							document.getElementById("pbMOS").attachEvent "onclick", getref("GrpCom_click")
							document.getElementById("pbMOS").title = "Click to invoke MOS commands"
						else
							document.getElementById("pbMOS").style.visibility = "hidden"
						end if
					end if
				end if
				
				k=k+1
			else 'SM block is not avalable in FLD
				'Hide COM status objects for blocks that are not available in the SM FLD
				document.getElementById("GrpCOM"&i).style.visibility = "hidden"
				'Special behavior for RESET status indication
				if instr(UCase(arrBlkNames(i-1)),"RTR")>0 then
					document.getElementById("GrpRST").style.visibility = "hidden"	
				end if
			end if
		next
end sub
'
sub GrpCom_click
	on error resume next
	document.getElementById("GrpMOS").style.visibility = "inherit"
end sub

'==================================================================================
' Update COM status/alarms on faceplate
'==================================================================================
sub PointStatus_OnUpdate(oSource)
on error resume next
	iQualityBad = 4
	
	'FGS Faceplates with Inhibit
	if typename(ConCboINHR)<> "Nothing" then
		if right(oSource.id,1)=5 then
			if ucase(alpSTATETEXT1.value)<>"NAN" and ucase(alpSTATETEXT2.value)<>"NAN" then
				if oSource.value = 0 then
					ConCboINHR.value = ucase(alpSTATETEXT2.value)
				elseif oSource.value = 1 then
					ConCboINHR.value = ucase(alpSTATETEXT1.value)
				end if
			end if
		end if
	end if
	document.getElementById("LinAlmPriTag"&right(oSource.id,1)).linecolor = "#808080"
	document.getElementById("TxtSt"&right(oSource.id,1)).styleClass = "TxtSt"
	'Get COM block name
	sBlkName = DisplayDataRepository.GetValue("Blk" & right(oSource.id,1))	

		iPriority = document.getElementById("ConAlpPriTag"&right(oSource.id,1)).value
		if iPriority > 1 then
			iNormal = document.getElementById("ConAlpPVNormTag"&right(oSource.id,1)).value
		else
			select case Ucase(DisplayDataRepository.Getvalue("Blk" & right(oSource.id,1)))
				case "FU","FU_1","FU_2","FU_3","MOS","MOSR","MOSS","AS","DI","OA","OS","ROS","SUO"
					iNormal = 0
				case "ALM","ALM_1","ALM_2","ALM_3","FLT","FLT_1","FLT_2","FLT_3"
					iNormal = 1
				case else
					iNormal = 0
			end select
		end if
		sVisibility = "hidden"
		sShowMTTR = "hidden"
		if iPriority > 2 then sVisibility = "inherit"

		if (oSource.quality and iQualityOffscan)=iQualityOffscan then
			document.getElementById("TxtSt"&right(oSource.id,1)).styleClass = "TxtSt_OffScan"
			sVisibility = "hidden"
		elseif (oSource.quality and iQualityBad)=iQualityBad then
			document.getElementById("TxtSt"&right(oSource.id,1)).styleClass = "TxtSt_Bad"
			sVisibility = "hidden"
		elseif oSource.value<>iNormal then 
			document.getElementById("TxtSt"&right(oSource.id,1)).styleClass = "TxtSt_Active"
			document.getElementById("LinAlmPriTag"&right(oSource.id,1)).style.visibility = sVisibility
			sShowMTTR = "inherit"

			select case iPriority
				case 0 '"UNDEFINED"
					sVisibility = "hidden"
				case 1 '"NONE"
					sVisibility = "hidden"
				case 2 '"JOURNAL"
					sVisibility = "hidden"
					'sColor= "#808080"
					'sVisibility = "inherit"
				case 3 '"LOW"
					sColor= AlarmStateIconFactory.LowColor
					sVisibility = "inherit"
				case 4 '"HIGH"
					sColor= AlarmStateIconFactory.HighColor
					sVisibility = "inherit"
				case 5 '"URGENT"
					sColor= AlarmStateIconFactory.UrgentColor
					sVisibility = "inherit"
				case else
					sColor= "#808080"
					sVisibility = "inherit"
			end select

			if (oSource.Quality and iQualityAccessNotPermitted) <> iQualityAccessNotPermitted then 
				document.getElementById("LinAlmPriTag"&right(oSource.id,1)).linecolor = sColor
			end if
		end if
		document.getElementById("LinAlmPriTag"&right(oSource.id,1)).style.visibility = sVisibility
		
		'MTTR show timer (sysdtlsmsbAIVotera_fp)
		if ucase(sBlkName) = "MTTR" then
				document.getElementById("GrpMTTR").style.visibility = sShowMTTR
		end if
end sub
'==================================================================================
' Update COM voted status/alarms on faceplate
'==================================================================================
sub PointStatusVoted_OnUpdate(idx1,idx2,idx3,sVoting)
on error resume next	

	if idx1 <> "" and idx1>0 then 
		Set oAlp1= document.getElementById("ScConAlpTag" & idx1)
		iNormal1 = document.getElementById("ConAlpPVNormTag"&idx1).value
	end if
	if idx2 <> "" and idx2>0 then 
		Set oAlp2= document.getElementById("ScConAlpTag" & idx2)
		iNormal2 = document.getElementById("ConAlpPVNormTag"&idx2).value
	end if
	if idx3 <> "" and idx3>0 then 
		Set oAlp3= document.getElementById("ScConAlpTag" & idx3)
		iNormal3 = document.getElementById("ConAlpPVNormTag"&idx3).value
	end if
	
	select case sVoting
	case "1oo2","2oo2" :
		if typename(document.getElementById("TxtSt" & idx1 & idx2))<> "Nothing" then
			if oAlp1.value<>iNormal1 or oAlp2.value<>iNormal2 then
				document.getElementById("TxtSt" & idx1 & idx2).styleClass = "TxtSt_Active"
			else
				document.getElementById("TxtSt" & idx1 & idx2).styleClass = "TxtSt"
			end if
		end if
	case "2oo3" :
		if typename(document.getElementById("TxtSt" & idx1 & idx2 & idx3))<> "Nothing" then
			if oAlp1.value<>iNormal1 or oAlp2.value<>iNormal2 or oAlp3.value<>iNormal3 then
				document.getElementById("TxtSt" & idx1 & idx2 & idx3).styleClass = "TxtSt_Active"
			else
				document.getElementById("TxtSt" & idx1 & idx2 & idx3).styleClass = "TxtSt"
			end if
		end if		
	end select
end sub

'**********************************************************************************
' SM Standard Builds Functions
'
' FGS SB Faceplates
'**********************************************************************************
const iPVRaw4 = 655
const iPVRaw20 = 3276
const iPVRaw25 = 4095
Dim kmA
kmA = 25/iPVRaw25
bShowMilliamps = false
'==================================================================================
' Inhibit Status indication on FGS SB faceplates
'==================================================================================
sub Inhibit_All()
on error resume next
	for i = 1 to 4 
		if document.getElementById("ScConAlpTag5").value=1 then
			document.getElementById("TxtSt"&i).linestyle = 1
			document.getElementById("TxtSt"&i).linecolor = "#808080"
		else
			document.getElementById("TxtSt"&i).linecolor = "transparent"
        end if
	next
end sub
'==================================================================================
' Initialize PV (Bar/Analog value) presentation on FGS SB faceplates (EU/RAW/mA)
'==================================================================================
sub InitPVPresentation()
on error resume next
	if sd.DataValue("CurrentPoint.AI.SCALING") = "OFF" then
		
		SignalType = left(sd.DataValue("CurrentPoint.AI.SIGNALTYPE"),1) 

		ConAlpPVEULo_GSH.style.visibility = "hidden"
		ConAlpPVEUHi_GSH.style.visibility = "hidden"
		ConAlpEUDesc_GSH.style.visibility = "hidden"
		ConAlpEUDescRAW_GSH.style.visibility = "inherit"
		ConAlpPVEXEULo_GSH.style.visibility = "inherit"
		ConAlpPVEXEUHi_GSH.style.visibility = "inherit"
		
		if sd.QualityGood("CurrentPoint.AIEU.PVEULO") then 'CUSTOM AIEU
			ConAlpEUDescRAW_GSH.value = sd.DataValue("CurrentPoint.AIEU.EUDESC")
			ConAlpPVEXEUHi_GSH.value =  sd.DataValue("CurrentPoint.AIEU.PVEUHI")
			ConAlpPVEXEULo_GSH.value = sd.DataValue("CurrentPoint.AIEU.PVEULO")
			dblEUHI = sd.DataValue("CurrentPoint.AIEU.PVEUHI")
			dblEULO = sd.DataValue("CurrentPoint.AIEU.PVEULO")
			IndPVEU.rangeHi=  dblEUHI
			IndPVEU.rangeLo=  dblEULO
			ConIndPV.style.visibility = "hidden"
			ConIndPVRAW.style.visibility = "hidden"
			IndPVEU.style.visibility = "inherit" 'Show PV EU based on PVRAW
			ConShpAlmPr.setCustomProperty "Text", "DACFBName", "AIEU"
		else 'RAW
			if bShowMilliamps then
				ConAlpEUDescRAW_GSH.value = "mA"
			else
				ConAlpEUDescRAW_GSH.value = "RAW"
			end if
			Select Case SignalType
				case "0"
					if bShowMilliamps then
						ConAlpPVEXEUHi_GSH.value = 20
						ConAlpPVEXEULo_GSH.value = 0
					else
						ConAlpPVEXEUHi_GSH.value = iPVRaw20
						ConAlpPVEXEULo_GSH.value = 0
					end if
				case "4"
					if bShowMilliamps then
						ConAlpPVEXEUHi_GSH.value = 20
						ConAlpPVEXEULo_GSH.value = 4
					else
						ConAlpPVEXEUHi_GSH.value = iPVRaw20
						ConAlpPVEXEULo_GSH.value = iPVRaw4
					end if
			end select
			ConIndPV.style.visibility = "hidden"
			ConIndPVRAW.style.visibility = "inherit" 'Show RAW
			IndPVEU.style.visibility = "hidden"
			ConShpAlmPr.setCustomProperty "Text", "DACFBName", "AI"
		end if
	else
		ConAlpPVEXEULo_GSH.style.visibility = "hidden"
		ConAlpPVEXEUHi_GSH.style.visibility = "hidden"
		ConAlpPVEULo_GSH.style.visibility = "inherit"
		ConAlpPVEUHi_GSH.style.visibility = "inherit"
		ConAlpEUDesc_GSH.style.visibility = "inherit"
		ConAlpEUDescRAW_GSH.style.visibility = "hidden"
		ConIndPV.style.visibility = "visible"
		ConIndPVRAW.style.visibility = "hidden"
		IndPVEU.style.visibility = "hidden"
		ConShpAlmPr.setCustomProperty "Text", "DACFBName", "AI"
	end if
end sub

'==================================================================================
' Apply PV format on all alphanumerics in SM SB faceplates
'==================================================================================
Sub SM_PVFormatUpdate(objSD, strDacaBlockName)
On Error Resume Next
	Dim PVFormat, PVScaling
	PVFormat = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".PVFORMAT")
	PVScaling = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".SCALING")

	if PVScaling<>"OFF" then
		If Not objSD.QualityBad("CurrentPoint." & strDacaBlockName & ".PVFORMAT")  Then
			sFormat = "%." & (PVFormat-1) & "f"
		else
			sFormat = "%.2f"
		End If
	else
		if sd.QualityGood ("Currentpoint.AIEU.PVFORMAT") then
			PVFormat = objSD.DataValue("Currentpoint.AIEU.PVFORMAT")
			sFormat = "%." & (PVFormat-1) & "f"
		else
			if bShowMilliamps then
				sFormat = "%.1f"
			else
				sFormat = "%.0f"
			end if
		end if
	end if

	'ConAlpPV_GSH.numericdisplayformat = sFormat
	'ConAlpSP.numericdisplayformat = sFormat
	'ConAlpPSP_GSH.numericdisplayformat = sFormat
	ConAlpPVEUHi_GSH.numericdisplayformat = sFormat
	ConAlpPVEXEUHi_GSH.numericdisplayformat = sFormat
	ConAlpPVEULo_GSH.numericdisplayformat = sFormat
	ConAlpPVEXEULo_GSH.numericdisplayformat = sFormat
	ConAlpPVEU1_GSH.numericdisplayformat = sFormat
	ConAlpPVEU2_GSH.numericdisplayformat = sFormat
	ConAlpPVEU3_GSH.numericdisplayformat = sFormat	
	AlpPV.numericdisplayformat = sFormat
	SM_ApplyPVFormat objSD, "AI"
End Sub
'==================================================================================
' Supplementary tasks related to PV format
'==================================================================================
Sub SM_ApplyPVFormat(objSD, strDacaBlockName)
On Error Resume Next
	Dim EUTop,EUBottom, PVScaling, SignalType, bRAW
	PVScaling = objSD.DataValue("CurrentPoint." & strDacaBlockName & ".SCALING")	
	bRAW = false
	if PVScaling <> "OFF" then
		EUBottom = ConAlpPVEULo_GSH.Value 
		EUTop = ConAlpPVEUHi_GSH.Value
	else
		SignalType = left(sd.DataValue("CurrentPoint." & strDacaBlockName & ".SIGNALTYPE"),1) 
		if not sd.Qualitybad ("Currentpoint.AIEU.PVEULO") then
			EUBottom =  objSD.DataValue("CurrentPoint.AIEU.PVEULO")
			EUTop = objSD.DataValue("CurrentPoint.AIEU.PVEUHI")
		else
			bRAW = true
			Select Case SignalType
				case "0"
					EUTop  = iPVRaw20
					EUBottom = 0
				case "4"
					EUTop  = iPVRaw20
					EUBottom = iPVRaw4
	 		end select
		end if
	end if

    For i = 1 To 3
        Document.getElementById("ConAlpPVEU" & i & "_GSH").numericdisplayformat = sFormat
		if bRAW then
			Document.getElementById("ConAlpPVEU" & i & "_GSH").Value = cint(EUBottom + ((EUTop - EUBottom) * i) / 4)
		else
			Document.getElementById("ConAlpPVEU" & i & "_GSH").Value =  EUBottom + ((EUTop - EUBottom) * i) / 4
		end if
    Next
    Call EUdescWidth
End Sub
'==================================================================================
' Update PV Bar/Analog indication on FGS faceplates (required for custom PV presentation)
'==================================================================================
Sub AlpPVUpdate(objSD)
on error resume next
	if objSD.DataValue("CurrentPoint.AI.SCALING") = "OFF" then
		if objSD.QualityGood("CurrentPoint.AIEU.PVEULO") then 'CUSTOM AIEU
			AlpPV.value = ConAlpPVEU.value
			IndPVEU.rangeLo = objSD.DataValue("CurrentPoint.AIEU.PVEULO")
			IndPVEU.rangeHi = objSD.DataValue("CurrentPoint.AIEU.PVEUHI")
			IndPVEU.value = ConAlpPVEU.value
		else 'RAW	
			if bShowMilliamps then
				AlpPV.value = ConIndPVRAW.value*kmA
				TxtEU.innertext = "mA"
			else
				AlpPV.value = ConIndPVRAW.value
				TxtEU.innertext = ""
			end if
		end if
	else 'AI
		AlpPV.value = ConIndPV.value
	end if
end sub 'AlpPVUpdate
'==================================================================================
' Update Digital PV radio buttons for DI/MOS/Reset faceplates
'==================================================================================
sub DigPV_onupdate(sParam,rBound)
on error resume next
	if sd.QualityGood("CurrentPoint." & sParam) then
		if sd.DataChanged("CurrentPoint." & sParam) then
			if sd.DataValue("CurrentPoint." & sParam) = 1 then
				if not rBound then shpS1.value = 2
				document.getElementById("txtS1").style.fontweight = "bold"
				document.getElementById("alpSTATETEXT1").style.fontweight = "bold"
				if not rBound then shpS0.value = 1
				document.getElementById("txtS0").style.fontweight = "normal"
				document.getElementById("alpSTATETEXT0").style.fontweight = "normal"
			elseif sd.DataValue("CurrentPoint." & sParam) = 0 then
				if not rBound then shpS1.value = 1
				document.getElementById("txtS1").style.fontweight = "normal"
				document.getElementById("alpSTATETEXT1").style.fontweight = "normal"
				if not rBound then shpS0.value = 2
				document.getElementById("txtS0").style.fontweight = "bold"
				document.getElementById("alpSTATETEXT0").style.fontweight = "bold"
			else
				if not rBound then shpS1.value = 1
				document.getElementById("txtS1").style.fontweight = "normal"
				document.getElementById("alpSTATETEXT1").style.fontweight = "normal"
				if not rBound then shpS0.value = 1
				document.getElementById("txtS0").style.fontweight = "normal"
				document.getElementById("alpSTATETEXT0").style.fontweight = "normal"
			end if
		end if
	else
		if not rBound then shpS1.value = 1
		document.getElementById("txtS1").style.fontweight = "normal"
		document.getElementById("alpSTATETEXT1").style.fontweight = "normal"
		if not rBound then shpS0.value = 1
		document.getElementById("txtS0").style.fontweight = "normal"
		document.getElementById("alpSTATETEXT0").style.fontweight = "normal"
	end if
end sub
'
'
sub DigPV2_onupdate(sParam,rBound,rbNum)
on error resume next
	if sd.QualityGood("CurrentPoint." & sParam) then
		if sd.DataChanged("CurrentPoint." & sParam) then
			if sd.DataValue("CurrentPoint." & sParam) = 1 then
				if not rBound then document.getElementById("shpS" & rbNum).value = 2
				document.getElementById("txtS" & rbNum).style.fontweight = "bold"
				document.getElementById("alpSTATETEXT" & rbNum).style.fontweight = "bold"
			elseif sd.DataValue("CurrentPoint." & sParam) = 0 then
				if not rBound then document.getElementById("shpS" & rbNum).value = 1
				document.getElementById("txtS" & rbNum).style.fontweight = "normal"
				document.getElementById("alpSTATETEXT" & rbNum).style.fontweight = "normal"
			else
				if not rBound then document.getElementById("shpS" & rbNum).value = 1
				document.getElementById("txtS" & rbNum).style.fontweight = "normal"
				document.getElementById("alpSTATETEXT" & rbNum).style.fontweight = "normal"
			end if
		end if
	else
		if not rBound then document.getElementById("shpS" & rbNum).value = 1
		document.getElementById("txtS" & rbNum).style.fontweight = "normal"
		document.getElementById("alpSTATETEXT" & rbNum).style.fontweight = "normal"
	end if
end sub
'==================================================================================
' Update Digital OP radio buttons for Req/Sts faceplate
'==================================================================================
sub OP_onupdate
on error resume next
	if sd.QualityGood("CurrentPoint.cp_Req") then
		if sd.DataChanged("CurrentPoint.cp_Req") then
			if sd.DataValue("CurrentPoint.cp_Req") = 1 then
				shpCMD1.value = 2
				shpCMD0.value = 1
			elseif sd.DataValue("CurrentPoint.cp_Req") = 0 then
				shpCMD1.value = 1
				shpCMD0.value = 2
			else
				shpCMD1.value = 1
				shpCMD0.value = 1
			end if
		end if
	else
		shpCMD1.value = 1
		shpCMD0.value = 1
	end if
end sub
'==================================================================================
' Show OFFNRML for MOS/RESET faceplates (COM points)
'==================================================================================
Sub HiAlmType_onupdate
on error resume next    
	If sd.QualityGood("CurrentPoint.cp_pvNormal") and sd.QualityGood("CurrentPoint.cp_pvOffnrmpri") Then
        If cInt(sd.DataValue("CurrentPoint.cp_req")) = cInt(sd.DataValue("CurrentPoint.cp_pvNormal")) and sd.DataValue("CurrentPoint.cp_pvOffnrmpri") > 1 Then       
			ConAlpHiAlmType.style.visibility = "visible"
			ConAlpHiAlmType.value = "OFFNORM"		
        Else
			ConAlpHiAlmType.style.visibility = "hidden"
        End If
    Else
      ConAlpHiAlmType.style.visibility = "hidden"
    End If

End Sub