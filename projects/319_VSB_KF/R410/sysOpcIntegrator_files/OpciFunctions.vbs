'----------------------------------------------------------------------
'
'  Copyright 2002 Honeywell PacIfic.  All Rights Reserved.
'
'----------------------------------------------------------------------

Dim m_bValidating 
Dim m_bSettingDirty
Dim m_ServerName
Dim m_objStation
Dim m_objApplication
Dim g_GroupProcessing
Dim g_objWShell
Dim g_sServerType
Dim g_sServerTypeShort
Dim g_sApplicationName
Dim g_itemData
Dim g_sItemSortOrder

Set m_objApplication = Window.External
Set m_objStation = Window.External.Parent
Set WShell = m_objStation.Dictionary.Item("WScriptShellObject").Value 'CreateObject("WScript.Shell")

g_sServerType = WShell.RegRead(REG_OPCI_SERVER_TYPE)
If g_sServerType = HON_SERVER_TYPE_EBI Then
    g_sApplicationName = APPLICATION_NAME_EBI
    g_sServerTypeShort = HON_SERVER_TYPE_EBI_SHORT
Else
    g_sApplicationName = APPLICATION_NAME_EPKS
    g_sServerTypeShort = HON_SERVER_TYPE_EPKS_SHORT
End If

m_ServerName = GetStationServerName()

m_bSettingDirty = false
m_bValidating = false

' Reset member variables for title page "refresh" - no need to reset member objects
Sub ResetMemberVariables
    ' Reset flags
    m_bSettingDirty = false
    m_bValidating = false

    ' Reset server name
    m_ServerName = GetStationServerName()
End Sub

Function GetStationServerName
    Dim CStrServerName

    If m_objStation.RuntimeStatus.IsConsoleStation Then
        CStrServerName = m_objStation.RuntimeStatus.ClusterServerName
    Else
        CStrServerName = m_objStation.RuntimeStatus.ServerName
    End If

    GetStationServerName = CStrServerName
End Function

Function GetConnectionString
    On Error Resume Next
    Dim CStrConnectionString
    
    'get the client data path from the registry
    CStrConnectionString = WShell.RegRead(REG_OPCI_CLIENT_CONNECT_STRING_PATH)
    If CStrConnectionString = "" Then
        CStrConnectionString = OPCI_CONNECT_STRING
        ' clear the error from the registry access
        Err.Clear
    End If
    
    GetConnectionString = CStrConnectionString
End Function

Sub SetLinkText
    cmdConfiguration.innerHTML = g_sApplicationName & " Configuration"
    cmdSysStatus.innerHTML = g_sApplicationName & " System Status"
End Sub

Sub UpdateItemCount
    Dim itemCount
    Err.Clear
    On Error Resume Next

    itemCount = parent.title.OpciDashboard.NumItemsInGroup

    If itemCount = 1 Then
        lblItemCount.innerText = "1 item"
    Else
        lblItemCount.innerText = CStr(itemCount) & " items"
    End If
    
    If Err.number Then
        ' Assume grid is empty
        lblItemCount.innertext = "0 items"
    End If
End Sub

Function populate_lstGroups
    Dim e,iCurrentGroupId,i
    On Error Resume Next
        
    ' Clear it out
    parent.title.lstGroups.options.length = 0
    
    Err.Clear
    
    If parent.title.OpciDashboard.NumberOfGroups = 0 Then 
        populate_lstGroups = false
        Exit Function
    End If
    
    Dim arrGroupData
    Dim groupId
    Dim groupName

    If (parent.title.OpciDashboard.GetGroupListData(arrGroupData)) Then
        ' Populate lstGroups Combobox
        For i = 0 To UBound(arrGroupData, 2)
            groupId = arrGroupData(0, i)
            groupName = arrGroupData(1, i)
            Set e = Document.createElement("OPTION")
            e.Text = groupName
            e.Value = groupId
            parent.title.lstGroups.Options.Add e 
            Set e = Nothing
        Next
    End If
    
    ' Set the current record
    iCurrentGroupId = parent.title.OpciDashboard.CurrentGroup
    For Each e in parent.title.lstGroups.options
        If CLng(e.Value) = CLng(iCurrentGroupId) Then 
            parent.title.lstGroups.selectedIndex = e.index
            parent.title.txtGroupId.value = iCurrentGroupId
        End If
    Next
    
    populate_lstGroups = true
End Function

' disable the commAnd buttons (used while processing commAnds)
Sub DisableButtons
    On Error Resume Next
    document.body.style.cursor = CURSOR_TYPE_BUSY
    cmdCommitGroup.disabled = true
    cmdCancelGroup.disabled = true
    cmdValidateGroup.disabled = true
    cmdDeleteGroup.disabled = true
    cmdDeleteItems.disabled = true
    cmdBrowseSrcItem.disabled = true
    cmdBrowseDstItem.disabled = true
    cmDimportItems.disabled = true
    cmdExportItems.disabled = true
End Sub

Sub SetVisibilityOfButtons
    On Error Resume Next
    If CLng(parent.title.OpciDashboard.mode) = CLng(CONFIG_MODE) Then
        cmdValidateGroup.disabled = false
        cmdDeleteGroup.disabled = false
        If (parent.title.OpciDashboard.GroupDirty) Then
            cmdCommitGroup.disabled = false
            cmdCancelGroup.disabled = false
            lblDirtyText.style.visibility = "visible"
        Else
            cmdCommitGroup.disabled = true
            cmdCancelGroup.disabled = true
            lblDirtyText.style.visibility = "hidden"
        End If
        cmdBrowseSrcItem.disabled = false
        cmdBrowseDstItem.disabled = false
        cmDimportItems.disabled = false
        cmdExportItems.disabled = false
        cmdDeleteItems.disabled = false
        
        ' Remove the Status tab from the tab definitions when in config mode.
        Dim tabNode
        Set tabNode = TabsXml.selectSingleNode("//*[string(@tabid)='GroupStatus.htm']")
        If Not tabNode Is Nothing Then
            tabNode.parentNode.removeChild tabNode
        End If
    Else
        lblDirtyText.style.visibility = "hidden"
    End If
    TabSet.InitContent TabsXml
    document.body.style.cursor = CURSOR_TYPE_NORMAL
End Sub

' Make sure configured server connections are valid
Function ValidateServerConnections
    
    Dim bChecked
    Dim sourceServer
    Dim sourceProgId
    Dim sourceRedundant
    Dim redundantSourceServer
    Dim redundantSourceProgId
    Dim destinationServer
    Dim destinationProgId
    Dim destinationRedundant
    Dim redundantDestinationServer
    Dim redundantDestinationProgId

    sourceServer = parent.title.OpciDashboard.GroupSourceServer
    sourceProgId = parent.title.OpciDashboard.GroupSourceProgId
    sourceRedundant = parent.title.OpciDashboard.GroupSourceRedundant
    redundantSourceServer = parent.title.OpciDashboard.GroupRedundantSourceServer
    redundantSourceProgId = parent.title.OpciDashboard.GroupRedundantSourceProgId
    destinationServer = parent.title.OpciDashboard.GroupDestinationServer
    destinationProgId = parent.title.OpciDashboard.GroupDestinationProgId
    destinationRedundant = parent.title.OpciDashboard.GroupDestinationRedundant
    redundantDestinationServer = parent.title.OpciDashboard.GroupRedundantDestinationServer
    redundantDestinationProgId = parent.title.OpciDashboard.GroupRedundantDestinationProgId
    
    ' assume failure
    ValidateServerConnections = False
    
    On Error Resume Next

    ' validate server data
    If (sourceServer = "" Or sourceProgId = "") Then
        strMsg = MSG_SOURCE_SERVER_NOT_CONFIGURED
        m_objStation.TemporaryMessageZoneText = strMsg
        Exit Function
    End If
        
    If (sourceRedundant) Then
        If (redundantSourceServer = "" Or redundantSourceProgId = "") Then
            strMsg = MSG_SOURCE_SERVER_NOT_CONFIGURED
            m_objStation.TemporaryMessageZoneText = strMsg	
            Exit Function
        
        ElseIf (UCase(CStr(redundantSourceProgId)) <> UCase(CStr(sourceProgId))) Then
            strMsg = MSG_REDUN_PROGID_INVALID
            m_objStation.TemporaryMessageZoneText = strMsg
            Exit Function
            
        ElseIf (UCase(CStr(redundantSourceServer)) = UCase(CStr(sourceServer)) Or _
                (UCase(CStr(redundantSourceServer)) = UCase(CStr(m_ServerName)) And _
                 UCase(CStr(sourceServer)) = LOCAL_SERVER) Or _
                (UCase(CStr(sourceServer)) = UCase(CStr(m_ServerName)) And _
                 UCase(CStr(redundantSourceServer)) = LOCAL_SERVER)) Then
            strMsg = MSG_REDUN_SERVER_INVALID
            m_objStation.TemporaryMessageZoneText = strMsg
            Exit Function
            
        End If
    End If
        
    If (destinationServer = "" Or destinationProgId = "") Then
        strMsg = MSG_DEST_SERVER_NOT_CONFIGURED
        m_objStation.TemporaryMessageZoneText = strMsg
        Exit Function
    End If
        
    If (destinationRedundant) Then
        If (redundantDestinationServer = "" Or redundantDestinationProgId = "") Then
            strMsg = MSG_DEST_SERVER_NOT_CONFIGURED
            m_objStation.TemporaryMessageZoneText = strMsg	
            Exit Function
    
        ElseIf (UCase(CStr(redundantDestinationProgId)) <> UCase(CStr(destinationProgId))) Then
            strMsg = MSG_REDUN_PROGID_INVALID
            m_objStation.TemporaryMessageZoneText = strMsg
            Exit Function
            
        ElseIf (UCase(CStr(redundantDestinationServer)) = UCase(CStr(destinationServer)) Or _
                (UCase(CStr(redundantDestinationServer)) = UCase(CStr(m_ServerName)) And _
                 UCase(CStr(destinationServer)) = LOCAL_SERVER) Or _
                (UCase(CStr(destinationServer)) = UCase(CStr(m_ServerName)) And _
                 UCase(CStr(redundantDestinationServer)) = LOCAL_SERVER)) Then
            strMsg = MSG_REDUN_SERVER_INVALID
            m_objStation.TemporaryMessageZoneText = strMsg
            Exit Function
        End If
    End If
    
    ' Check if local honeywell server option should be selected 
    ' so that we don't validate or commit servers unnecessarily
    bChecked = rdOtherSourceServer.checked
    If (err.number = SUCCEEDED And bChecked) Then
        If (UCase(CStr(sourceProgId)) = HON_OPC_SERVER_PROGID And _
            (UCase(CStr(sourceServer)) = LOCAL_SERVER Or _
             UCase(CStr(sourceServer)) = UCase(CStr(m_ServerName)))) Then
            parent.title.OpciDashboard.GroupSourceServer = LCase(LOCAL_SERVER)
            parent.title.OpciDashboard.GroupSourceProgId = "HWHsc.OPCServer"
            parent.title.OpciDashboard.GroupSourceRedundant = false
         ElseIf (sourceRedundant) Then
            If (UCase(CStr(redundantSourceProgId)) = HON_OPC_SERVER_PROGID And _
                (UCase(CStr(redundantSourceServer)) = LOCAL_SERVER Or _
                UCase(CStr(redundantSourceServer)) = UCase(CStr(m_ServerName)))) Then 
                parent.title.OpciDashboard.GroupSourceServer = LCase(LOCAL_SERVER)
                parent.title.OpciDashboard.GroupSourceProgId = "HWHsc.OPCServer"
                parent.title.OpciDashboard.GroupSourceRedundant = false	
            End If
        End if
    End If
    
    bChecked = rdOtherDestinationServer.checked
    If (err.number = 0 And bChecked) Then
        If (UCase(CStr(destinationProgId)) = HON_OPC_SERVER_PROGID And _
            (UCase(CStr(destinationServer)) = LOCAL_SERVER Or _
             UCase(CStr(destinationServer)) = UCase(CStr(m_ServerName)))) Then
            parent.title.OpciDashboard.GroupDestinationServer = LCase(LOCAL_SERVER)
            parent.title.OpciDashboard.GroupDestinationProgId = "HWHsc.OPCServer"
            parent.title.OpciDashboard.GroupDestinationRedundant = false
        ElseIf (destinationRedundant) Then
            If (UCase(CStr(redundantDestinationProgId)) = HON_OPC_SERVER_PROGID And _
                (UCase(CStr(redundantDestinationServer)) = LOCAL_SERVER Or _
                UCase(CStr(redundantDestinationServer)) = UCase(CStr(m_ServerName)))) Then 
                parent.title.OpciDashboard.GroupDestinationServer = LCase(LOCAL_SERVER)
                parent.title.OpciDashboard.GroupDestinationProgId = "HWHsc.OPCServer"
                parent.title.OpciDashboard.GroupDestinationRedundant = false
            End If
        End If
    End If
    
    UpdateSourceServer
    UpdateDestinationServer
    
    ' successful exit
    ValidateServerConnections = True
    
End Function

' delete an entire group
Sub cmdDeleteGroup_onclick
    Dim strMsg
    Dim Response
    Dim iRet
    Dim strGroupName
    
    ' check permissions
    If Not CheckSecurity Then Exit Sub

    ' check database
    If Not CheckDatabaseConnection(true) Then Exit Sub

    strGroupName = parent.title.OpciDashboard.GroupName
    
    On Error Resume Next
    If GroupSaved Then
        strMsg = MSG_GROUP_DELETE_CONFIRM & g_GroupId & ", '" & strGroupName & "'?" & vbNewLine & MSG_ONLINE_GROUP_WARNING
    Else
        strMsg = MSG_GROUP_DELETE_CONFIRM & g_GroupId & ", '" & strGroupName & "'?"
    End If
    
    ConfirmMsg strMsg, "DeleteGroup", "", "", "", "", ""
End Sub

' cancel group changes
sub cmdCancelGroup_onclick
    Dim iCancelledGroup
    Dim strMsg
    Dim iRet
    Dim strGroupName

    On Error Resume Next
    
    ' check permissions
    If Not CheckSecurity Then Exit Sub

    ' check database
    If Not CheckDatabaseConnection(true) Then Exit Sub

    strGroupName = parent.title.OpciDashboard.GroupName

    If Not GroupSaved Then
        ' An online copy won't exist so do a delete
        cmdDeleteGroup_onclick
    Else
        strMsg = MSG_CANCEL_CONFIRM & g_GroupId & ", '" & strGroupName & "'?"
        ConfirmMsg strMsg, "CancelGroup", "", "", "", "", ""
    End If
End Sub

sub cmdValidateGroup_onclick
    Dim iResult
    Dim strMsg
    Dim strGroupName

    On Error Resume Next
    
    ' check permissions
    If Not CheckSecurity Then Exit Sub
    
    ' check database
    If Not CheckDatabaseConnection(true) Then Exit Sub

    ' if on the group details page, save the group data before validating
    ' (if on the item details page, it should have already been saved and 
    '  calling this function would result in an error)
    If (IsGroupDetailsTab) Then
        If (GroupDirty) Then
            SaveGroupData
        End If
    End If

    ' validate server connections
    If Not ValidateServerConnections Then Exit Sub
    
    strGroupName = parent.title.OpciDashboard.GroupName

    m_bValidating = true
    g_GroupProcessing = true

    'disable all buttons while processing
    dgItems.Select 0,0

    'disable all buttons while processing
    Call DisableButtons
    iResult	= parent.title.OpciDashboard.ValidateGroup(Clng(g_GroupId), Cstr(strGroupName))
    If iResult >= SUCCEEDED Then
        Call window_onload
    Else
        If (IsItemDetailsTab) Then
            SetupGrid
        End If
        Call ShowErrorText
        Call SetVisibilityOfButtons
    End If
    m_bValidating = false
    g_GroupProcessing = false
End Sub

' commit group and item details
Sub cmdCommitGroup_onclick
    Dim strMsg
    Dim strGroupName

    On Error Resume Next
    
    ' check permissions
    If Not CheckSecurity Then Exit Sub

    ' check database
    If Not CheckDatabaseConnection(true) Then Exit Sub

    ' if on the group details page, save the group data before committing
    ' (if on the item details page, it should have already been saved and 
    '  calling this function would result in an error)
    If (IsGroupDetailsTab) Then
        If (GroupDirty) Then
            SaveGroupData
        End If
    End If

    strGroupName = parent.title.OpciDashboard.GroupName
    strMsg = MSG_COMMIT_CONFIRM & g_GroupId & ", '" & strGroupName & "'?"		
    
    ConfirmMsg strMsg, "CommitGroup", "", "", "", "", ""
End Sub

' commit the group on the server
Function CommitGroup(GroupId, GroupName)
    Dim iRet
    Dim strMsg
    On Error Resume Next
    
    ' validate server connections
    If Not ValidateServerConnections Then 
        CommitGroup = FAILED
        Exit Function
    End If

    'disable all buttons while processing
    Call DisableButtons

    ' call Commit on server
    iRet = parent.title.OpciDashboard.CommitGroup(CLng(GroupId), CStr(GroupName))
    If iRet <> SUCCEEDED Then
        ' Commit failed
        If (IsItemDetailsTab) Then
            SetupGrid
        End If
        Call ShowErrorText
        Call SetVisibilityOfButtons
    End If
    CommitGroup = iRet
End Function

Function DeleteGroup(GroupId, GroupName)
    Dim iRet
    On Error Resume Next
    'disable all buttons while processing
    Call DisableButtons
    iRet = parent.title.opcidashboard.DeleteGroup(CLng(GroupId), CStr(GroupName))
    If iRet = SUCCEEDED Then
        m_GroupDeleted = true
        Populate_lstGroups
        If parent.title.lstGroups.children.length > 0 Then
            parent.title.lstGroups_onchange
        Else
            parent.location.href = "..\sysCfgOpcIntegrator.htm"
        End If
    Else 
        Call ShowErrorText
        Call SetVisibilityOfButtons
    End If
    DeleteGroup = iRet
End Function

Function CancelGroupChange(GroupId, GroupName)
    Dim iRet
    'disable all buttons while processing
    Call DisableButtons
    If (parent.title.opcidashboard.CancelGroupChange(CLng(GroupId), CStr(GroupName)) <> 0) Then
        Call ShowErrorText
        Call SetVisibilityOfButtons
    End If
    CancelGroupChange = iRet
End Function

Dim g_iGetResponseExecuted
Dim g_bGetResponseExitLoop
Dim g_strPrompt
Dim g_strNextFunction
Dim g_DimArg1
Dim g_DimArg2
Dim g_DimArg3
Dim g_DimArg4
Dim g_DimArg5
Dim g_bWaitingForResponse

g_iGetResponseExecuted = 0	' Number of times spent executing the timerGetResponse_Timer function
g_bGetResponseExitLoop = false
g_GroupProcessing = false
g_bWaitingForResponse = false

Sub ConfirmMsg(strPrompt, strNextFunction, DimArg1, DimArg2, DimArg3, DimArg4, DimArg5)
    ' Set some flags
    g_GroupProcessing = true
    g_strPrompt = strPrompt
    g_strNextFunction = strNextFunction
    g_DimArg1 = DimArg1
    g_DimArg2 = DimArg2
    g_DimArg3 = DimArg3
    g_DimArg4 = DimArg4
    g_DimArg5 = DimArg5
    g_iGetResponseExecuted = 0
    g_bGetResponseExitLoop = false
    g_bWaitingForResponse = true
    ' Start the timer
    window.setTimeout GetRef("timerGetResponse_Timer"), 0
End Sub

Sub timerGetResponse_Timer
    GetResponse
    If (g_iGetResponseExecuted > GET_RESPONSE_TIMEOUT\GET_RESPONSE_INTERVAL) Then
        CallNextRoutine false
    End If
    If (not g_bGetResponseExitLoop) Then
        ' repeat Call
        window.setTimeout GetRef("timerGetResponse_Timer"), GET_RESPONSE_INTERVAL
    End If
End Sub

Sub GetResponse
    Dim strAns			' Response from the operator
    Dim iStatus			' Status of the GetResponse3 Call.
    strAns = ""
    
    If (g_bGetResponseExitLoop) Then
        Exit Sub
    End If

    If (g_iGetResponseExecuted = 0) Then
        ' Acquire a response from the Station message zone
        iStatus = window.external.GetResponse3(g_strPrompt, YES_NO_TYPE, strAns)
    Else
        If (Not(g_bWaitingForResponse) Or (m_objStation.MessageZoneText <> g_strPrompt)) Then
            ' Either got our response or message zone has changed, call GetResponse3 again
            iStatus = window.external.GetResponse3(g_strPrompt, YES_NO_TYPE, strAns)
        Else
            ' Assume everything is good, wait for GotResponse or change in message zone
            iStatus = GET_RESPONSE_SUCCEEDED
        End If
    End If

    g_iGetResponseExecuted = g_iGetResponseExecuted + 1

    If iStatus = GET_RESPONSE_CANCELLED Then
        ' Response was cancelled by the operator.
        CallNextRoutine false
    Else
        If Len(strAns) > 0 Then
            ' Response acquired, check the response
            If (LCase(Mid(strAns, 1, 1)) = GET_RESPONSE_YES) Then
                ' Affirmative Operator confirmation acquired
                CallNextRoutine true
            Else
                ' Negative Operator confirmation acquired
                CallNextRoutine false
            End If
        End If
    End If
End Sub


Sub GotResponse()
    ' We have our response, called from OnResponse event
    g_bWaitingForResponse = false
End Sub

Sub CallNextRoutine(blnResponse)
    Dim iRet
    Dim strGroupName

    On Error Resume Next
    strGroupName = parent.title.OpciDashboard.GroupName

    ' stop timer
    g_bGetResponseExitLoop = true	
    ' Clear the message zone
    m_objStation.MessageZoneText = ""
    If (blnResponse) Then
        Select Case g_strNextFunction
            Case "DeleteItems"
                g_itemData = parent.title.OpciDashboard.DeleteItems(g_DimArg1)
                BindItemGridData(true)
                SetGroupDirtyBit

            Case "DeleteGroup"
                iRet = DeleteGroup(g_GroupId, strGroupName)

            Case "CommitGroup"
                If (CommitGroup(g_GroupId, strGroupName) = SUCCEEDED) Then	
                    Call window_onload
                End If

            Case "CancelGroup"
                If (CancelGroupChange(g_GroupId, strGroupName) = SUCCEEDED) Then
                    parent.main.location.reload true
                End If

            Case "GroupExit"
                ' user chose to commit group changes
                If (CommitGroup(g_GroupId, strGroupName) = SUCCEEDED) Then
                    If g_DimArg2 Then
                        If g_DimArg3 = CONFIG_MODE Then
                            parent.window.navigate("..\sysCfgOpcIntegrator.htm")
                        ElseIf g_DimArg3 = ONLINE_MODE Then
                            parent.window.navigate("..\sysOpcIntegrator.htm")
                        End If
                    ElseIf not g_DimArg2 Then
                        If g_DimArg1 = GROUP_NOT_SELECTED Then
                            parent.window.navigate("..\sysCfgOpcIntegrator.htm")
                        Else
                            parent.title.lstGroups.SelectedIndex = g_DimArg1
                            parent.title.OpciDashboard.CurrentGroup = parent.title.lstGroups.options(g_DimArg1).value
                            parent.main.location.reload true
                        End If	
                    End If
                End If

            Case "ImportItems"
                Call DisableButtons
                If (parent.title.OpciDashboard.ImportItems(CLng(g_GroupId), dlgImportExport.FileName, CStr(strGroupName)) = 0) Then
                    ' Call afteredit as it is not triggered by the import
                    SetGroupDirtyBit
                    Window_Onload
                End If
                Call SetVisibilityOfButtons

            Case "ExportItems"
                Call DisableButtons
                parent.title.OpciDashboard.ExportItems CLng(g_GroupId), dlgImportExport.FileName, CStr(strGroupName)
                Call SetVisibilityOfButtons
            Case "DisableDiagnostic"
                SetGroupDirtyBit
            Case Else
                ' do nothing
                Call SetVisibilityOfButtons
        End Select
    Else
        Select Case g_strNextFunction
            'user chose to lose group changes
            Case "GroupExit"
                If (CancelGroupChange(g_GroupId, strGroupName) = SUCCEEDED) Then
                    If g_DimArg2 Then
                        If g_DimArg3 = CONFIG_MODE Then
                            parent.window.navigate("..\sysCfgOpcIntegrator.htm")
                        ElseIf g_DimArg3 = ONLINE_MODE Then
                            parent.window.navigate("..\sysOpcIntegrator.htm")
                        End If
                    ElseIf not g_DimArg2 Then
                        If g_DimArg1 = GROUP_NOT_SELECTED Then
                            parent.window.navigate("..\sysCfgOpcIntegrator.htm")
                        Else
                            parent.title.lstGroups.SelectedIndex = g_DimArg1
                            parent.title.OpciDashboard.CurrentGroup = parent.title.lstGroups.options(g_DimArg1).value
                            parent.main.location.reload true
                        End If
                    End If
                End If

            Case "DisableDiagnostic"
                ' cancel diagnostic change
                txtDiagnosticInterval.value = g_DimArg1
                Call SetVisibilityOfButtons

            Case Else
                ' user selected No
                'Call SetVisibilityOfButtons
        End Select
    End If
    g_GroupProcessing = false
    g_bWaitingForResponse = false
End Sub

Sub cmdConfiguration_onclick
    On Error Resume Next
    If g_Mode = CONFIG_MODE And GroupDirty And GroupSaved And Not GroupProcessing Then
        If Not (m_objStation.RuntimeStatus.SecurityLevel < SECURITY_LEVEL_MNGR) Then
            lstGroups_ChangeConfirm GROUP_NOT_SELECTED, true, CONFIG_MODE
        Else
            parent.window.navigate("..\sysCfgOpcIntegrator.htm")
        End If
    ElseIf g_Mode = CONFIG_MODE Then 
        parent.main.navigate("GroupTable.htm")
    ElseIf not cmdConfiguration.disabled Then
        parent.window.navigate("..\sysCfgOpcIntegrator.htm")
    End If
End Sub

Sub cmdSysStatus_onclick
    On Error Resume Next
    If g_Mode = CONFIG_MODE And GroupDirty And GroupSaved And Not GroupProcessing Then
        If Not (m_objStation.RuntimeStatus.SecurityLevel < SECURITY_LEVEL_MNGR) Then
            lstGroups_ChangeConfirm GROUP_NOT_SELECTED, true, ONLINE_MODE
        Else
            parent.window.navigate("..\sysOpcIntegrator.htm")
        End If
    ElseIf g_Mode = ONLINE_MODE Then
        parent.main.navigate("GroupTable.htm")
    ElseIf not cmdSysStatus.disabled Then	
        parent.window.navigate("..\sysOpcIntegrator.htm")
    End If
End Sub

Public Function GroupDirty
    On Error Resume Next
    If (parent.title.OpciDashboard.GroupDirty) Then
        GroupDirty = true
    Else 
        GroupDirty = false
    End If
    If Err.Number <> SUCCEEDED Then
        GroupDirty = false
    End If
End Function

Public Function GroupSaved
    On Error Resume Next
    If (parent.title.OpciDashboard.GroupSaved) Then
        GroupSaved = true
    Else
        GroupSaved = false
    End If
    If Err.Number <> SUCCEEDED Then
        GroupSaved = false
    End If
End Function

Public Function lstGroups_ChangeConfirm(iSelectedGroup, bExiting, mode)
    Dim strGroupName
    g_GroupProcessing = true
    strGroupName = parent.title.OpciDashboard.GroupName
    strMsg = MSG_CONFIRM_GROUP_EXIT
    ConfirmMsg strMsg, "GroupExit", iSelectedGroup, bExiting, mode, "", ""
End Function

Public Function GroupProcessing
    GroupProcessing = g_GroupProcessing
End Function

Sub Sleep(seconds)
    On Error Resume Next
    Dim oShell
    Set oShell = m_objStation.Dictionary.Item("WScriptShellObject").Value 'CreateObject("Wscript.Shell")
    cmd = "%COMSPEC% /c ping -n " & 1 + seconds & " 127.0.0.1>nul"
    oShell.Run cmd, 0, True
End Sub

' flag group as dirty
Sub SetGroupDirtyBit
    If Not m_bSettingDirty And Not parent.title.OpciDashboard.GroupDirty Then 
        m_bSettingDirty = true
        parent.title.OpciDashboard.GroupDirty = true
        m_bSettingDirty = false
        cmdCommitGroup.disabled = false
        cmdCancelGroup.disabled = false
        lblDirtyText.style.visibility = "visible"
    End If
End Sub

Sub ShowGroupStatus
    ' set the group status
    Dim strGroupStatusLabel
    Dim strGroupStatusValue
    Dim strGroupStatusImage

    strGroupStatusLabel = ""
    strGroupStatusValue = ""
    strGroupStatusImage = ""
    
    If (parent.title.lstGroups.style.display = "") Then
        strGroupStatusLabel = "Group Status" & ": &nbsp;"
        parent.title.OpciDashboard.CurrentGroup = parent.title.OpciDashboard.CurrentGroup
        strGroupStatusValue = parent.title.OpciDashboard.GroupStatus
        Select Case strGroupStatusValue
            Case GROUP_DISABLED
                strGroupStatusImage = "<IMG align='absmiddle' src='GroupDisabled.gif'>"
            Case GROUP_OK
                strGroupStatusImage = "<IMG align='absmiddle' src='GroupOK.gif'>"
            Case GROUP_MARGINAL
                strGroupStatusImage = "<IMG align='absmiddle' src='GroupMarginal.gif'>"
            Case GROUP_FAILED
                strGroupStatusImage = "<IMG align='absmiddle' src='GroupFailed.gif'>"
            Case GROUP_OVERLOAD
                strGroupStatusImage = "<IMG align='absmiddle' src='GroupOverload.gif'>"
            Case GROUP_STARTING
                strGroupStatusImage = "<IMG align='absmiddle' src='GroupStarting.gif'>"
            Case Else
                strGroupStatusValue = "Not Configured"
                strGroupStatusImage = "<IMG align='absmiddle' src='GroupDisabled.gif'>"
        End Select
    End If

    parent.title.GroupStatusLabel.innerHTML = strGroupStatusLabel
    parent.title.GroupStatusImage.innerHTML = strGroupStatusImage
    parent.title.GroupStatusValue.innerHTML = strGroupStatusValue
End Sub

Function CheckSecurity
    ' check permissions
    CheckSecurity = true
    If m_objStation.RuntimeStatus.SecurityLevel < SECURITY_LEVEL_MNGR Then
        m_objStation.TemporaryMessageZoneText = MSG_INSUFFICIENT_PRIVELAGES
        CheckSecurity = false
    End If
End Function

Function CheckDatabaseConnection(bInteractive)
    If bInteractive Then
        document.body.style.cursor = CURSOR_TYPE_BUSY
    End If
    
    ' check database
    CheckDatabaseConnection = parent.title.OpciDashboard.IsConnectionLive()
    
    If bInteractive Then
        If Not CheckDatabaseConnection Then
            m_objStation.TemporaryMessageZoneText = MSG_SERVER_UNAVAILABLE
        End If

        document.body.style.cursor = CURSOR_TYPE_NORMAL
    End If
End Function

' Binds the item grid to the global item array
' Adds a new row for inserting records if required
Function BindItemGridData(bAddNew)
    If UBound(g_itemData, 1) = -1 And bAddNew Then
        ' no records, add a new one
        dgItems_AddNewRow
    End If

    If UBound(g_itemData, 1) = -1 Then
        ' still no records, make grid empty
        ' this will be the case when viewing the item status of a group with no items
        dgItems.Rows = 1
        dgItems.Cols = 8
    Else
        ' bind the array
        Call dgItems.BindToArray(g_itemData)

        ' if there is no edit row, add it now
        If g_Mode = CONFIG_MODE Then
            Dim lastRow, sourceItem, destItem
            lastRow = UBound(g_itemData, 2)
            sourceItem = 1
            destItem = 2
            ' if source and destination item (col 1 and 2 in 0-based array) are empty,
            ' add a new record
            If g_itemData(sourceItem, lastRow) <> "" Or g_itemData(destItem, lastRow) <> "" Then
                dgItems_AddNewRow
            Else
                dgItems.TextMatrix(dgItems.Rows-1, 0) = "*"	
            End If
            
        End If
        SetColumnColours
    End If
End Function

' is current tab groupdetails
Function IsGroupDetailsTab
    If InStr(1, parent.main.location.href, "groupdetails", 1) > 0  Then
        IsGroupDetailsTab = True
    Else
        IsGroupDetailsTab = False
    End If
End Function

' is current tab itemdetails
Function IsItemDetailsTab
    If InStr(1, parent.main.location.href, "itemdetails", 1) > 0  Then
        IsItemDetailsTab = True
    Else
        IsItemDetailsTab = False
    End If
End Function

' Show error text if applicable
Sub ShowErrorText
    If (parent.title.OpciDashboard.GroupErrorString <> "") Then
        lblErrorString.innerHTML = parent.title.OpciDashboard.GroupErrorString
        lblErrorString.style.visibility = "visible"
    Else 
        lblErrorString.innerHTML = ""
        lblErrorString.style.visibility = ""
    End If
End Sub

Sub LogMessage(message)
    Window.External.Parent.LogMessage message
End Sub
