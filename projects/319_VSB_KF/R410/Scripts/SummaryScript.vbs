' SummaryScript.vbs
' $Revision$
' $Date$
Dim g_DefaultDetailsPaneHTML
Dim g_ExistCommentsHTML
Dim g_DetailsPaneXML
Dim g_DefaultXslDoc
Dim g_AccessEventXslDoc
Dim g_ExistCommentsXslDoc
Dim g_EventCategory
Dim g_statuspane_SelectedEntity
Dim g_statuspane_FaceplateName
Dim g_blank_Faceplate
Dim g_SuppressionXslDoc
Dim g_AlarmsSuppressingSelectedAlarmHTML
Dim g_AlarmsSuppressedBySelectedAlarmHTML
g_DefaultDetailsPaneHTML = ""
g_ExistCommentsHTML = ""
g_DetailsPaneXML = ""
g_DefaultXslDoc = NULL
g_AccessEventXslDoc = NULL
g_ExistCommentsXslDoc = NULL
g_EventCategory = 0
g_statuspane_SelectedEntity = ""
g_statuspane_FaceplateName = ""
g_blank_Faceplate = "sysStsBlank_fp.htm"
g_SuppressionXslDoc = NULL
g_AlarmsSuppressingSelectedAlarmHTML = ""
g_AlarmsSuppressedBySelectedAlarmHTML = ""
'for storing the last command map
Dim LastCommandMap
LastCommandMap = 0
'MASKS for interpretting Activity Commmand Map
Dim ACT_COMMAND_GO
Dim ACT_COMMAND_HOLD
Dim ACT_COMMAND_STOP
Dim ACT_COMMAND_ABORT
Dim ACT_COMMAND_REMOVE
ACT_COMMAND_GO = 1
ACT_COMMAND_HOLD = 2
ACT_COMMAND_STOP = 4
ACT_COMMAND_ABORT = 8
ACT_COMMAND_REMOVE = 16
'Constant for Activity Commmands
Dim ACT_COMMAND_GO_ENUM
Dim ACT_COMMAND_HOLD_ENUM
Dim ACT_COMMAND_STOP_ENUM
Dim ACT_COMMAND_ABORT_ENUM
Dim ACT_COMMAND_REMOVE_ENUM
ACT_COMMAND_GO_ENUM = 1
ACT_COMMAND_HOLD_ENUM = 2
ACT_COMMAND_STOP_ENUM = 3
ACT_COMMAND_ABORT_ENUM = 4
ACT_COMMAND_REMOVE_ENUM = 5
Sub Localise(strName, xmlDoc)
    Dim xmlConvertNode
    strName = "fields/field[@name='" + strName + "']"
    Set xmlConvertNode = xmlDoc.selectSingleNode(strName)
    If Not xmlConvertNode Is Nothing And (VarType(DTFormat) = 9) Then
        xmlConvertNode.text = DTFormat.Localize( xmlConvertNode.text, g_dateTimeFormat )
    End If
    Set xmlConvertNode = Nothing
End Sub
Sub LocaliseLiveValue(xmlDoc)
    Dim xmlConvertNode
    Dim xmlPVFormatNode
    strLiveValueName = "fields/field[@name='LiveValue']"
	strPVFormatName = "fields/field[@name='PVFormat']"
    Set xmlConvertNode = xmlDoc.selectSingleNode(strLiveValueName)
    Set xmlPVFormatNode = xmlDoc.selectSingleNode(strPVFormatName)
    If Not xmlConvertNode Is Nothing And (VarType(DTFormat) = 9) Then
        If Not xmlPVFormatNode Is Nothing And IsNumeric(xmlPVFormatNode.text) And IsNumeric(xmlConvertNode.text) Then
            xmlConvertNode.text = LiveValueLocalize(CDbl(xmlConvertNode.text), CInt(xmlPVFormatNode.text), 0, g_dateTimeFormat)
        Else
            xmlConvertNode.text = DTFormat.Localize( xmlConvertNode.text, g_dateTimeFormat )
        End If
    End If
    Set xmlPVFormatNode = Nothing
    Set xmlConvertNode = Nothing
End Sub
Sub TransformDetailsPaneHTML(DetailsXML)
    g_DefaultDetailsPaneHTML = ""
    If (DetailsXML <> "") Then
       Set xmlDoc = LocaliseXml(DetailsXML)
    If ( g_SummaryType = SUMMARY_ACTIVITY or g_SummaryType = SUMMARY_BATCH or g_SummaryType = SUMMARY_PROCEDURE ) Then
        g_EventCategory = 0
    Else
            If IsNumeric(xmlDoc.selectSingleNode("fields/field[@name='Category']").text) Then
                g_EventCategory = CInt(xmlDoc.selectSingleNode("fields/field[@name='Category']").text)
            Else
                g_EventCategory = 0
        End If
        End If
        If (g_EventCategory = 5 or g_EventCategory = 16) Then
            If (IsNull(g_AccessEventXslDoc)) Then
                Set g_AccessEventXslDoc = CreateObject("Msxml2.DOMDocument.6.0")
                g_AccessEventXslDoc.async = false
                g_AccessEventXslDoc.load("XSL\AccessEvents.xsl")
            End If
            g_DefaultDetailsPaneHTML = xmlDoc.transformNode(g_AccessEventXslDoc)
        Else
            If (IsNull(g_DefaultXslDoc)) Then
                Set g_DefaultXslDoc = CreateObject("Msxml2.DOMDocument.6.0")
                g_DefaultXslDoc.async = false
                g_DefaultXslDoc.load("XSL\Default.xsl")
            End If
            g_DefaultDetailsPaneHTML = xmlDoc.transformNode(g_DefaultXslDoc)
        End If
    End If
End Sub
Sub UpdateStatusPane(DetailsXML, bSelectionChange, bSelectedRowDataChange, ulCookie)
    Dim iSubtype
    Dim iRemoteType
    Dim newSelected
    If (g_SummaryType = SUMMARY_SYSTEM_ALARM and bSelectionChange and DetailsXML <> "") Then
        Dim xmlDoc
        Set xmlDoc = CreateObject("Msxml2.DOMDocument.6.0")
        xmlDoc.async = false
        xmlDoc.loadXML(DetailsXML)
        newSelected = xmlDoc.selectSingleNode("fields/field[@name='FullName']").text
        If (g_statuspane_SelectedEntity = newSelected) Then
            Exit Sub
        End If
        g_statuspane_SelectedEntity = newSelected
        DisplayDataRepository.PutValue "PointOnPage", g_statuspane_SelectedEntity
    End If
End Sub
Sub ClearEntitySelection()
    g_statuspane_FaceplateName = ""
    g_statuspane_SelectedEntity = ""
    DisplayDataRepository.PutValue "PointOnPage", ""
    document.getElementById("faceplateiframe").style.visibility = "hidden"
End Sub
Sub PopulateDetailsPane(DetailsXML, bSelectionChange, bSelectedRowDataChange, ulCookie)
    If (g_SummaryType = SUMMARY_ALARM) Then
	HighlightAlarmTracker(ulCookie)
    End if
    If (bSelectionChange or bSelectedRowDataChange) Then
        g_DefaultDetailsPaneHTML = ""
        g_DetailsPaneXML = DetailsXML
        g_EventCategory = 0
    End If
    If (bSelectionChange) Then
        g_ExistCommentsHTML = ""
        g_AlarmsSuppressingSelectedAlarmHTML = ""
        g_AlarmsSuppressedBySelectedAlarmHTML = ""
    End If
    If (DetailsPane.style.display <> "none" and (bSelectedRowDataChange or bSelectionChange)) Then
        If (g_DetailsPaneXML <> "") Then
            TransformDetailsPaneHTML(g_DetailsPaneXML)
            DefaultData.innerHTML = g_DefaultDetailsPaneHTML
            DefaultData.style.display = "inline"
            divDetailsSidePanel.style.visibility = "visible"
        End If
    End If
    If (g_SummaryType = SUMMARY_EVENT or g_SummaryType = SUMMARY_ALARM or g_SummaryType = SUMMARY_SYSTEM_ALARM or g_SummaryType = SUMMARY_ALERT or g_SummaryType = SUMMARY_EVENTVIEW) Then
        If (DetailsPane.style.display <> "none" and CommentsData.style.display <> "none" and bSelectionChange) Then
            RequestComments()
        End If
    End If
    If (g_SummaryType = SUMMARY_ALARM) Then
        If (DetailsPane.style.display <> "none" and SuppressionRelationshipsData.style.display <> "none" and bSelectionChange) Then
            RequestSuppressionRelationships()
        End If
    End If
    If (g_SummaryType = SUMMARY_ALARM or g_SummaryType = SUMMARY_SYSTEM_ALARM) Then
        If (DetailsPane.style.display <> "none" and AlarmHelpData.style.display <> "none" and bSelectionChange) Then
            RequestAlarmHelp()
        End If
    End If
    If (g_SummaryType = SUMMARY_ALARM or g_SummaryType = SUMMARY_SYSTEM_ALARM or g_SummaryType = SUMMARY_ALERT or g_SummaryType = SUMMARY_ACTIVITY or g_SummaryType = SUMMARY_BATCH or g_SummaryType = SUMMARY_PROCEDURE ) Then
        CheckLiveValueUpdates()
    End If
	If ((g_SummaryType = SUMMARY_ALARM) and bSelectionChange and g_bEnableBlockComments and DetailsXML <> "") Then
		Dim xmlBlockCommentsDoc
		Set xmlBlockCommentsDoc = CreateObject("Msxml2.DOMDocument.6.0")
		xmlBlockCommentsDoc.async = false
		xmlBlockCommentsDoc.loadXML(DetailsXML)
		BlockComments1.innerText = ""
		g_BlockComment1Set = false
		BlockComments2.innerText = ""
		g_BlockComment2Set = false
		Dim paramName
		paramName = xmlBlockCommentsDoc.selectSingleNode("fields/field[@name='Block']").text
		If paramName <> "(Null)" And paramName <> "" Then
			paramName = paramName & ".BlckComment1"
		Else
			paramName = "BlckComment1"
		End If
		DisplayDataRepository.PutValue "PointOnPage", xmlBlockCommentsDoc.selectSingleNode("fields/field[@name='Source']").text
		DisplayDataRepository.PutValue "ParamOnPage", paramName
	End If
End Sub
Sub ClearDetailsPane()
    If (g_SummaryType = SUMMARY_EVENT or g_SummaryType = SUMMARY_ALARM or g_SummaryType = SUMMARY_SYSTEM_ALARM or g_SummaryType = SUMMARY_ALERT or g_SummaryType = SUMMARY_EVENTVIEW) Then
        If (not g_bCommentUnavaiable) Then
            AddComments.value = ""
        End If
        ExistComments.innerText = ""
        AddComments.style.cursor = "default"
        AddComments.disabled = True
        btnSaveDetails.readonly = True
        btnSaveDetails.className = "saveDetailsDisabled"
    End If
    DefaultData.innerText = ""
    DefaultData.style.display = "none"
    divDetailsSidePanel.style.visibility = "hidden"
    g_DefaultDetailsPaneHTML = ""
    g_ExistCommentsHTML = ""
    g_DetailsPaneXML = ""
    g_EventCategory = 0
    g_AlarmsSuppressingSelectedAlarmHTML = ""
    g_AlarmsSuppressedBySelectedAlarmHTML = ""
    If (g_SummaryType = SUMMARY_ALARM) Then
        If (g_bEnableBlockComments) Then
            BlockComments1.innerText = ""
            BlockComments2.innerText = ""
        End If
    End If
    If (g_SummaryType = SUMMARY_ALARM or g_SummaryType = SUMMARY_SYSTEM_ALARM) Then
        AlarmHelpData.innerHTML = ""
    End If
    If (g_SummaryType = SUMMARY_MESSAGE) Then
        document.getElementById("instructioniframe").contentWindow.location.replace("about:blank")
        TabDetailsPane(0)
        ShowInstructionsTab(false)
    End If
    If ( g_SummaryType = SUMMARY_ALARM or g_SummaryType = SUMMARY_SYSTEM_ALARM or g_SummaryType = SUMMARY_ALERT or g_SummaryType = SUMMARY_ACTIVITY or g_SummaryType = SUMMARY_BATCH or g_SummaryType = SUMMARY_PROCEDURE ) Then
        CheckLiveValueUpdates()
    End If
    If ( g_SummaryType = SUMMARY_ACTIVITY or g_SummaryType = SUMMARY_BATCH or g_SummaryType = SUMMARY_PROCEDURE ) Then
        UnbindActivity()
        DisableActivityCommandButtons()
    End If
End Sub
Sub PopulateActivityDetailsPane(bstrPointName, bstrActName, bstrPublicName, bSelectionChange, bSelectedRowDataChange, bAutoSelected)
    If (bSelectionChange) Then
        g_EventCategory = 0
        'Set the corresponding Custom properties to the newly retrieved Activity point and parameter names
        'ignore error as tell only occur this event is triggered when navigating away from the page
        On Error Resume Next
        displayDataRepository.PutValue "ActivityPnt", bstrPointName
        On Error GoTo 0
        g_SelectedActName = bstrActName
        g_SelectedActPublicName = bstrPublicName
        g_SelectionOnGrid = true
        'Set the data UI Object to correct configurations namely activityTagName of selected activity
        g_CloseCreationUIOnUpdate = Not(bAutoSelected)
        UpdateActDetailsPane()
        g_CloseCreationUIOnUpdate = false
    End If
    CheckLiveValueUpdates()
End Sub
Sub ClearActivitySelection()
    g_DefaultDetailsPaneHTML = ""
    g_DetailsPaneXML = ""
    g_EventCategory = 0
    CheckLiveValueUpdates()
    'ISR116829 (really should have been done as part of work to keep the details pane loaded with the activity even if it disappears off the grid)
    'don't unbind the Activity from the comboboxes.
    'UnbindActivity()
    'but commandButtons should be disabled as they are NOT in the details pane.
    DisableActivityCommandButtons()
    g_SelectionOnGrid = false
    UpdateActDetailsPane()
End Sub
Sub UpdateActivityCommandBtns(bstrCommandParamName, lCommandMap)
    'ignore error as tell only occur this event is triggered when navigating away from the page
    On Error Resume Next
    'Set the corresponding Custom properties to the newly retrieved Activity point and parameter names
    displayDataRepository.PutValue "CommandPrm", bstrCommandParamName
    On Error GoTo 0
    'Set Activity Command Buttons
    ToggleActivityCommandButtons(lCommandMap)
End Sub
Sub UpdateActivityElements(bstrSubCommandParamName, bstrModeParamName, bstrModeAttrParamName)
    'ignore error as tell only occur this event is triggered when navigating away from the page
    On Error Resume Next
    'Set the corresponding Custom properties to the newly retrieved Activity point and parameter names
    displayDataRepository.PutValue "SubCommandPrm", bstrSubCommandParamName
    displayDataRepository.PutValue "ModePrm", bstrModeParamName
    displayDataRepository.PutValue "ModeAttrPrm", bstrModeAttrParamName
    On Error GoTo 0
End Sub
Sub UnbindActivity()
    'ignore error as tell only occur this event is triggered when navigating away from the page
    On Error Resume Next
    'Set Custom properties to null
    displayDataRepository.PutValue "ActivityPnt", ""
    displayDataRepository.PutValue "CommandPrm", ""
    displayDataRepository.PutValue "SubCommandPrm", ""
    displayDataRepository.PutValue "ModePrm", ""
    displayDataRepository.PutValue "ModeAttrPrm", ""
    On Error GoTo 0
End Sub
Sub ToggleActivityCommandButtons(validCommands)
    If( validCommands <> LastCommandMap ) Then
        SetActivityCommandButtons(validCommands)
        LastCommandMap = validCommands
    End If
End Sub
Sub DisableActivityCommandButtons()
    'Map to disable all commands
    Dim validCommands
    validCommands = 0
    If( validCommands <> LastCommandMap ) Then
        SetActivityCommandButtons(validCommands)
        LastCommandMap = validCommands
    End If
End Sub
Sub TransformExistComments(CommentsXML)
    g_ExistCommentsHTML = ""
    If (CommentsXML <> "") Then
        Dim xmlDoc
        Set xmlDoc = CreateObject("Msxml2.DOMDocument.6.0")
        xmlDoc.async = false
        xmlDoc.loadXML(CommentsXML)
        If (IsNull(g_ExistCommentsXslDoc)) Then
            Set g_ExistCommentsXslDoc = CreateObject("Msxml2.DOMDocument.6.0")
            g_ExistCommentsXslDoc.async = false
            g_ExistCommentsXslDoc.SetProperty "AllowXsltScript", True
            g_ExistCommentsXslDoc.load("XSL\ExistComments.xsl")
        End If
        g_ExistCommentsHTML = xmlDoc.transformNode(g_ExistCommentsXslDoc)
    End If
End Sub
Sub PopulateExistComments()
    If (g_SummaryType = SUMMARY_EVENT or g_SummaryType = SUMMARY_ALARM or g_SummaryType = SUMMARY_SYSTEM_ALARM or g_SummaryType = SUMMARY_ALERT or g_SummaryType = SUMMARY_EVENTVIEW) Then
        If (g_ExistCommentsHTML <> "") Then
            On Error Resume Next
            ExistComments.innerHTML = g_ExistCommentsHTML
        End If
    End If
End Sub
Sub TransformSuppressionRelationships(sAlarmsSuppressingSelectedAlarm, sAlarmsSuppressedBySelectedAlarm)
    g_AlarmsSuppressingSelectedAlarmHTML = ""
    g_AlarmsSuppressedBySelectedAlarmHTML = ""
    If (sAlarmsSuppressingSelectedAlarm <> "" or g_AlarmsSuppressedBySelectedAlarmHTML <> "") Then
        Dim xmlDoc
        Set xmlDoc = CreateObject("Msxml2.DOMDocument.6.0")
        xmlDoc.async = false
        If (IsNull(g_SuppressionXslDoc)) Then
            Set g_SuppressionXslDoc = CreateObject("Msxml2.DOMDocument.6.0")
            g_SuppressionXslDoc.async = false
            g_SuppressionXslDoc.SetProperty "AllowXsltScript", True
            g_SuppressionXslDoc.load("XSL\Suppression.xsl")
        End If
        If (sAlarmsSuppressingSelectedAlarm <> "" ) Then
            xmlDoc.loadXML(sAlarmsSuppressingSelectedAlarm)
            g_AlarmsSuppressingSelectedAlarmHTML = xmlDoc.transformNode(g_SuppressionXslDoc)
        End if
        If (sAlarmsSuppressedBySelectedAlarm <> "" ) Then
            xmlDoc.loadXML(sAlarmsSuppressedBySelectedAlarm)
            g_AlarmsSuppressedBySelectedAlarmHTML = xmlDoc.transformNode(g_SuppressionXslDoc)
        End if
    End If
End Sub
Sub PopulateSuppressionRelationships()
    If (g_SummaryType = SUMMARY_ALARM) Then
        If (g_AlarmsSuppressingSelectedAlarmHTML <> "") Then
            On Error Resume Next
            AlarmsSuppressingSelectedAlarm.innerHTML = g_AlarmsSuppressingSelectedAlarmHTML
        End If
        If (g_AlarmsSuppressedBySelectedAlarmHTML <> "") Then
            On Error Resume Next
            AlarmsSuppressedBySelectedAlarm.innerHTML = g_AlarmsSuppressedBySelectedAlarmHTML
        End If
    End If
End Sub
Sub OnUpdateAlarmHelp(strAlarmHelpXML)
    ' Don't worry about updating suppression relationships if not displaying them
    if (AlarmHelpData.style.display <> "none") then
        Dim xmlDoc, AlarmHelpXslDoc, transformResult, almHlpHTML
        Set AlarmHelpXslDoc = CreateObject("Msxml2.DOMDocument.6.0")
        AlarmHelpXslDoc.async = false
        AlarmHelpXslDoc.SetProperty "AllowXsltScript", True
        ' The InStr check is a hack to manage system alarm help on the alarm summary
        ' because of the option to show system alarms.  Ideally the two XSL files could
        ' be combined into a single file that correctly styles based off the nodes in
        ' the XML but that will require re-verification of the styling.
        If (g_SummaryType = SUMMARY_SYSTEM_ALARM Or InStr(strAlarmHelpXML, "<Cause>") > 0) Then
            AlarmHelpXslDoc.load("XSL\SystemAlarmHelp.xsl") 'system summary
        Else
            AlarmHelpXslDoc.load("XSL\AlarmHelp.xsl") 'process summary
        End If
        Set xmlDoc = CreateObject("Msxml2.DOMDocument.6.0")
        xmlDoc.async = false
            xmlDoc.loadXML(strAlarmHelpXML)
        ' Convert the xml strings to html and then populate the display
        transformResult = xmlDoc.transformNode(AlarmHelpXslDoc)
        almHlpHTML = Replace(transformResult,vbCrLf,"</br>") 'IE quirksmode does not whitepsace
        AlarmHelpData.innerHTML = almHlpHTML
        ResizeAlarmHelp()
    end if
End Sub
Sub GetResponse()
    Const ciYesNoType = 1		' Yes/No response question type.
    Dim strAns					' Response from the operator
    Dim iStatus					' Status of the GetResponse3 call.
    strAns = ""
    ' Acquire a response from the Station message zone
    iStatus = window.external.application.GetResponse3(g_strPrompt, ciYesNoType, strAns)
    g_iGetResponseExecuted = g_iGetResponseExecuted + 1
    if iStatus = 0 Then
        ' Response was cancelled by the operator.
        CallNextRoutine(false)
    Else
        If Len(strAns) > 0 Then
            ' Response acquired, check the response
            If (LCase(Mid(strAns, 1, 1)) = "y") Then
                ' Affirmative Operator confirmation acquired
                CallNextRoutine(true)
            Else
                ' Negative Operator confirmation acquired
                CallNextRoutine(false)
            End If
        End If
    End If
End Sub
Sub OnMessageSummarySelectionChange(DetailsXML, bSelectionChange, bSelectedRowDataChange, ulCookie)
    If (g_SummaryType = SUMMARY_MESSAGE) Then
        If (bSelectionChange or bSelectedRowDataChange) Then
            g_DefaultDetailsPaneHTML = ""
            g_DetailsPaneXML = DetailsXML
            g_EventCategory = 0
        End If
        If (bSelectionChange) Then
            g_ExistCommentsHTML = ""
            g_AlarmsSuppressingSelectedAlarmHTML = ""
            g_AlarmsSuppressedBySelectedAlarmHTML = ""
        End If
        Set localisedXml = LocaliseXml(DetailsXML)
        Dim executionId
        executionId = localisedXml.selectSingleNode("fields/field[@name='ExecutionID']").text
        If executionId <> "(Null)" And executionId <> "" Then
            If (bSelectionChange) Then
                If (UpdatedMessageSummaryCookie(ulCookie)) Then
                    Dim msgPoint
                    Dim msgBlock
                    msgPoint = localisedXml.selectSingleNode("fields/field[@name='SourceTagName']").text
                    msgBlock = localisedXml.selectSingleNode("fields/field[@name='Block']").text
                    ShowInstructionForMessage msgPoint, msgBlock
                End If
            ElseIf (bSelectedRowDataChange) Then
                InstructionMessageUpdate
            End If
        Else
            If (UpdatedMessageSummaryCookie(ulCookie)) Then
                TabDetailsPane(0)
                ShowInstructionsTab(false)
            End If
        End If
        If (g_DetailsPaneXML <> "") Then
            TransformDetailsPaneHTML(g_DetailsPaneXML)
            DefaultData.innerHTML = g_DefaultDetailsPaneHTML
            DefaultData.style.display = "inline"
            divDetailsSidePanel.style.visibility = "visible"
        End If
    End If
End Sub
Function LocaliseXml(DetailsXML)
 If (DetailsXML <> "") Then
        Set LocaliseXml = CreateObject("Msxml2.DOMDocument.6.0")
        LocaliseXml.async = false
        LocaliseXml.loadXML(DetailsXML)
        LocaliseXml.setProperty "SelectionLanguage", "XPath"
        Localise "ChangedTime", LocaliseXml
        Localise "FieldTime", LocaliseXml
        Localise "Time", LocaliseXml
        Localise "UnsuppressedTime", LocaliseXml
        Localise "OldestTime", LocaliseXml
        Localise "ExpiryTime", LocaliseXml
        Localise "UnshelvedTime", LocaliseXml
        Localise "Value", LocaliseXml
        Localise "PrevValue", LocaliseXml
        LocaliseLiveValue LocaliseXml
        Localise "AlarmLimit", LocaliseXml
    End If
End Function
