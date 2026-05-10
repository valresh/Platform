// /************************************************************************/
// /*****     COPYRIGHT 2008 - 2009    HONEYWELL INTERNATIONAL SARL     ****/
// /************************************************************************/
// PntBrowserScrplt.js
// $Revision: 26 $
var EVENT_ONREADY               = "event_onready";
var EVENT_ONPOINTSELECT         = "event_onpointselect";
var EVENT_ONSERVERSTATUSCHANGE  = "event_onserverstatuschange";
var MIN_TAGLIST_HEIGHT          = 20;
var MAX_TAGLIST_HEIGHT          = 15 * 50; // TagTable.rowHeight * MAX_VIEWSIZE
var MIN_TREE_HEIGHT             = 20;
var MAX_TREE_HEIGHT             = 500;
var BASE_DECIMAL                = 10;
var POINT_TYPE_CLH              = 7;
var POINT_TYPE_SYS              = 8;
var POINT_SUBTYPE_SYS_ALMGRP    = 1002;
var POINT_SUBTYPE_SERVER        = 1002;
var MIN_LCNNODE_SUBTYPE         = 1044;
var MAX_LCNNODE_SUBTYPE         = 1100;
var MIN_UCNNODE_SUBTYPE         = 1101;
var MAX_UCNNODE_SUBTYPE         = 1143;
var MIN_IOPNODE_SUBTYPE         = 1150;
var MAX_IOPNODE_SUBTYPE         = 1213;
var BRIDGE_SUBTYPE				= 67;
var LCN_SUBTYPE					= 1144;
var UCN_SUBTYPE					= 1145;
var g_strBckgndColor            = 0xF0F0F0;
var g_strDisabledBckgndColor    = "inactivecaption";
var g_nDisabledTimeout          = 1000;
var g_nTypingTimeout            = 800;
var g_nTickTimeout              = 200;
var g_nCurrentTick              = 0;
var g_nMaxTick                  = 1000000;
var g_nRetryWindow              = 1000;
var g_strLastFindStr            = "";
var g_bChangedFind              = false;
var g_nLastChangeTick           = 0;
var g_strServerName             = "";
var g_strServerAlias            = "";
var g_bPageInitialized          = false;
var g_bHsccliInitialized        = false;
var g_bTreeDOInitialized        = false;
var g_strTdoStartLocation       = "";
var g_bTagDOInitialized         = false;
var g_strTagDOStartLocation     = "";
var g_DES000_F                  = 35;
var g_SHIFTWORD                 = 65536;
var g_TAGFL_POINT               = g_DES000_F * g_SHIFTWORD + 1;
var g_bUIEnabled                = true;
var g_iDisableGUITimerID        = 0;
var g_ShowFilter                = false;
var g_IncludeSubTree            = true;
var g_nEntityTreeHeight         = 150;
var g_nEmHeight                   = 300;
var g_nEmWidth                    = 300;
var g_TagSelectionChangeXML     = "";
var g_EntitySelectionChangeXML  = "";
var g_LastSelectedTagname       = "";
var g_LastSelectedFullname      = "";
var g_LastSelectedDescription   = "";
var g_LastSelectedEntityname    = "";
var g_LastSelectedEntityDescription = "";
var g_LastSelectedPointTypeAndSubType  = "";
var g_LastSelectedPointOwner    = "";
var g_ClipboardFormat           = "CF_HWEMDATA";
var g_textRange                 = null;
var g_sBookMark                 = "";
var g_objTextRange              = null;
var g_SystemToggled                =false;
var g_FLAG_USE_DESCENDENTS         = 2;
var g_FLAG_SYSTEM_MODE             = 4;
var g_ModeButtonColorOn         = 0x707070;
var g_ModeButtonColorOff        = 0xF0F0F0;
var g_Release                   = 0;
var g_DashboardLinkName         = "DashboardLinkShape";
var g_DashboardLcnShape		= "LCNStatusShape";
var g_DashboardUcnShape		= "UCNStatusShape";
var g_DashboardIopShape		= "IOPStatusShape";
var g_GenericStatusShape	= "GenericStatusShape";
// --------------------------------------------------------------------------------------
// OnPageLoad()
//
//  Called when the page loads
// --------------------------------------------------------------------------------------
function OnPageLoad()
{
    try
    {
        OnPersist_LOAD();
        g_textRange = document.body.createTextRange();
        g_sBookMark = g_textRange.getBookmark();  
        attachEvent("onresize", fnPositionAll);
        if (window.external.frozen == false) {
            // raised for container html page
            window.external.raiseEvent(EVENT_ONREADY, "");
        }
        window.setInterval("fnHandleOnEntryTimeout()", g_nTickTimeout);
        fnDisableUI();
        g_bPageInitialized = true;
    }
    catch( err ) { }
}
// --------------------------------------------------------------------------------------
// OnPersist_LOAD()
//
//  Responsible for load the current page layout and view from persisted state. Called 
//  when the page is loaded.
// --------------------------------------------------------------------------------------
function OnPersist_LOAD()
{
    try
    {
        OnPersist_EntityState_LOAD();       // EntityDataObject
        OnPersist_TagState_LOAD();          // TagDataObject
    }
    catch( err ) { }
}
// --------------------------------------------------------------------------------------
// OnPersist_EntityState_LOAD()
//
//  Checks to make sure the table is fully loaded before loading the persisted view.
// --------------------------------------------------------------------------------------
function OnPersist_EntityState_LOAD()
{
    var sStartLocation = "";
    var hr = -2;
    try
    {       
        try
        {
            sStartLocation = EntityDataObject.StartLocation;
        }
        catch( err ) {
            EntityDataObject.attachEvent("OnAckTransaction", OnPersist_EntityState_LOAD);
            return;
        }
        EntityDataObject.detachEvent("OnAckTransaction", OnPersist_EntityState_LOAD);
        if (g_bTreeDOInitialized == true) {return;}
        EntityDataObject.Version = 4.5;
        EntityDataObject.AutoMigrate = 0;
        EntityDataObject.WaitForSelectionList = 1;
        EntityDataObject.SummaryType = 5;
        EntityDataObject.StartLocation = g_strTdoStartLocation;
        EntityDataObject.AuxFilter = "(IsAssigned or HasAssignedDescendents)"; 
        if (xmlEntityTemplate.readyState == "complete")
        {
            EntityDataObject.TemplateXML = xmlEntityTemplate.xml;
        }
        else
        {
            EntityDataObject.attachEvent("OnAckTransaction", OnPersist_EntityState_LOAD);
            return; 
        }
        var TableState = "";
        var TDOState = "";
        var TDOWidth = g_nEmWidth - 20;
        TableState = "<state><colref id='Tree' width='";
        TableState = TableState + TDOWidth;
        TableState = TableState + "' hidden='false'></colref></state>";
        EntityDataObject.SetState(TDOState, TableState);
        g_bTreeDOInitialized = true;
    }
    catch( err ) {  }
}
function OnPersist_TagState_LOAD()
{
    var sStartLocation = "";
    try
    {       
        try
        {
            sStartLocation = TagDataObject.StartLocation;
        }
        catch( err )
        {
            TagDataObject.attachEvent("OnAckTransaction", OnPersist_TagState_LOAD);
            return;
        }
        TagDataObject.detachEvent("OnAckTransaction", OnPersist_TagState_LOAD);
        if (g_bTagDOInitialized == true) {
            return;
        }
        TagDataObject.Version = 4.5;
        TagDataObject.AutoMigrate = 0;
        TagDataObject.WaitForSelectionList = 1;
        TagDataObject.StartLocation = g_strTagDOStartLocation;
        TagDataObject.FilterOptions = 1;
        TagDataObject.AuxFilter = "Type = " + g_TAGFL_POINT;
        if (xmlTagTemplate.readyState == "complete")
        {
            TagDataObject.TemplateXML = xmlTagTemplate.xml;
        }
        else
        {
            TagDataObject.attachEvent("OnAckTransaction", OnPersist_TagState_LOAD);
            return; 
        }
        var TableState = "";
        var TagDOState = "";
        var TagDOWidth = g_nEmWidth - 20;
        TableState = "<state><colref id='Tag' width='";
        TableState = TableState + TagDOWidth;
        TableState = TableState + "' hidden='false'></colref></state>";
        TagDataObject.SetState(TagDOState, TableState);
        txtFullName.firstChild.innerText = g_LastSelectedFullname;
        txtFullName.title = g_LastSelectedFullname;
        g_bTagDOInitialized = true;
    }
    catch( err ) {  }
}
function InitializePBControl()
{
    var hr = -2;
    hr = PBControlObject.InitialiseHSCCLI();
    if (hr == -2) { return; }   // something bad happened
    g_bHsccliInitialized = true;
}
// --------------------------------------------------------------------------------------
// OnPersist_SAVE()
//
//  Responsible for persisting the current page layout and view. Called when the page is
//  unloaded.
// --------------------------------------------------------------------------------------
function OnPersist_SAVE()
{
    try
    {
        if (g_bPageInitialized)
        {
        }
    }
    catch( err ) { }
}
// --------------------------------------------------------------------------------------
// OnPageUnload()
//
//  Called when the page is unloaded
// --------------------------------------------------------------------------------------
function OnPageUnload()
{
    try
    {
        OnPersist_SAVE();
    }
    catch( err ) { }
}
//-------------------------
function mfnSetServerName(sServerName)
{
    g_strServerName = JsTrim(sServerName);
}
//-------------------------
function mfnGetServerName()
{
// returns the name of the Server to which Point Browser is currently connected.
    var sTemp = "";
    if (PBControlObject.connectionNo < 0)
        return sTemp;
    else
        return JsTrim(g_strServerName);
}
//-------------------------
function fnSetRelease(release)
{
  g_Release = release;
}
//-------------------------
function fnGetReleaseFromServerInfo(serverinfo)
{
   var checkString = "<property name=\"ServerVersion\">";
   var n = serverinfo.search(checkString);
   var strRelease = serverinfo.substr(n + checkString.length, 3);
   var intRelease= parseInt(strRelease);
   return intRelease;
}
//---------------------------
function fnGetRelease()
{
  return g_Release;
}
//---------------------------
function fnGetServerAlias()
{
    return g_strServerAlias;
}
//---------------------------
function fnGetServerAliasFromServerInfo(serverinfo)
{
    var pattern =/<property name=\"ServerAlias\">([^<]*)/i;
    var result = pattern.exec(serverinfo);
    if (result && result.length > 1)
        return result[1];
    else
        return "";
}
//---------------------------
function fnSetServerAlias(serverAlias)
{
    g_strServerAlias = serverAlias;
}
//---------------------------
    function mfnConnectServer()
    {
// attempts to connect to the Server set by a previous call to SetServerName().
// returns 0 on success; 1 otherwise.    
        if (!g_bHsccliInitialized) {
            InitializePBControl();
        }
        if (g_strServerName != "")
        {
            if (PBControlObject.connectionNo >= 0)
            {
                nReturn = -2;
                nReturn = PBControlObject.DisconnectServer(-1);
            }
            fnShowStatus("Connecting to<br><b>" + g_strServerName + "</b>");
            fnDisableUI();      
            PBControlObject.ConnectServerA(g_strServerName);
            var serverinfo;
            serverinfo = PBControlObject.GetServerInfo(g_strServerName);
            var release = fnGetReleaseFromServerInfo(serverinfo);
            fnSetRelease(release);
            var serverAlias = fnGetServerAliasFromServerInfo(serverinfo);
            fnSetServerAlias(serverAlias);
            scrpltBody.style.cursor = "wait";
        }
        return 0;
    }         
//-------------------------
    function mfnConnectServerGUIDisabled(nConnectionNum, name)
    {
// attempts to connect to the Server set by a previous call to SetServerName().
// returns 0 on success; 1 otherwise.    
        var nReturn = -2;
        scrpltBody.style.cursor = "default";
        if (nConnectionNum < 0) {
            g_bHsccliInitialized = false;
            fnShowStatus("Unable to connect");
            fnEvent_onserverstatuschange();
            return 1;
        } else {
            g_bHsccliInitialized = true;
        }
        EntityDataObject.PointBrowseControl = PBControlObject;
        TagDataObject.PointBrowseControl = PBControlObject;
        fnHideStatus();
        fnEnableUI();
        TagTable.style.pixelHeight = 10;
        EntityTable.style.pixelHeight = 10;
        fnPositionAll();
        fnClearFilter();
        fnEvent_onserverstatuschange();
        return 0;
    }
    //-------------------------
    function mfnStationConnectServer()
    {
// attempts to connect to the Server after previous call to fnSetStationObject().
        fnShowStatus("Please wait...");
        fnDisableUI();      
        g_iDisableGUITimerID = window.setTimeout("mfnStationConnectServerGUIDisabled()", g_nDisabledTimeout);
        return 0;
    }
    //-------------------------
    function mfnStationConnectServerGUIDisabled()
    {
        // attempts to connect to the Server after previous call to fnSetStationObject().
        // returns 0 on success; 1 otherwise.    
        window.clearInterval(g_iDisableGUITimerID);        
        if (!g_bHsccliInitialized) {
            InitializePBControl();
        }
        if (PBControlObject.connectionNo >= 0) {
            g_bHsccliInitialized = true;
        } else {
            g_bHsccliInitialized = false;
            fnShowStatus("Unable to connect");
            fnEvent_onserverstatuschange();
            return 1;
        }    
        EntityDataObject.PointBrowseControl = PBControlObject;
        TagDataObject.PointBrowseControl = PBControlObject;
        fnHideStatus();        
        fnEnableUI();
        TagTable.style.pixelHeight = 10;
        EntityTable.style.pixelHeight = 10;
        fnPositionAll();         
        fnClearFilter();
        fnEvent_onserverstatuschange();
        return 0;
    }
    //-------------------------
    function mfnReconnectServer()
    {
        return mfnConnectServer();
    }
    //-------------------------
    function mfnDisconnectServer()
    {
        var sTemp = "";
        var nReturn = -2;
        fnDisableUI();
        if (g_bHsccliInitialized && (PBControlObject.connectionNo >= 0)) {
            nReturn = PBControlObject.DisconnectServer(-1);
            if (nReturn == -2) {
                return 1;
            }
        }
        g_bHsccliInitialized = false;
        g_strServerName = "";
        fnEvent_onserverstatuschange();     
        return 0;
    }
    //-------------------------
    function mfnGetServerStatus()
    {
        // returns the current status of the Server
        // 0 = disconnected
        // 1 = connected
        if (PBControlObject.connectionNo < 0) { return 0; }
        return 1;
    }
    //-------------------------
    function mfnGetTagName()
    {
        // returns the tagname of the currently selected point. If no point is selected, an empty string is returned.    
        return JsTrim(g_LastSelectedTagname);
    }
    function mfnGetTagName2()
    {
        // returns the tagname of the currently selected point. If no point is selected, an empty string is returned.    
        if (g_SystemToggled)
        {
            window.clipboardData.setData("text",'#SYSTEMPOINTPICKERDOUBLECLICKEVENT#' + fnPrependServerName(g_LastSelectedTagname));
        }
        return JsTrim(g_LastSelectedTagname);
    }
    //-------------------------
    function mfnGetPointDescription()
    {
        // returns the description of the currently selected point. If no point is selected, an empty string is returned.
        return JsTrim(g_LastSelectedDescription);
    }
    //-------------------------
    function mfnGetEntityDescription()
    {
        // returns the description of the currently selected entity. If no entity is selected, an empty string is returned.
        return JsTrim(g_LastSelectedEntityDescription);
    }
    //-------------------------
    function mfnGetFullPointName()
    {
        // returns the full point name (fully qualified name) of the currently selected point. If no point is selected, an empty string is returned.
        return JsTrim(g_LastSelectedFullname);
    }
    //-------------------------
    function mfnRefresh()
    {
// will refresh the graphical elements of the Point Browser, using the current operator security level and Enterprise Model filter. Method should only need to be called when Point Browser is operating in Station.
        var sAlert;
        sAlert = "Refresh(): ";
        // alert(sAlert);
        // refresh all objects; 
        // including obtaining new security restrictions;
        return;
    }
    //-------------------------
    function fnPrependServerName(strTagname)
    {
        if (strTagname.indexOf(":") != -1)
        {
            // Server name already prepended (eg remote point).
            return strTagname;
        }
        if (g_LastSelectedPointOwner != "")
        {
            return g_LastSelectedPointOwner + ":" + strTagname;
        }
        return strTagname;
    }
    //-------------------------
    function fnDragPoint(strTagname, strTypeSubtypeItemnameOwner, strFullname, strActive) {
        var strXmlPoint;
        if ( (strActive != "tagname") && 
             (strActive != "fullname") && 
             (strActive != "entity") && 
             (strActive != "") )
        {
            return;
        }
        /* HwEmDataSchema.xml:
        <?xml version="1.0" encoding="UTF-16"?>
        <HwEmData xmlns="x-schema:HwEmDataSchema.xml">
            <points active="tagname">
                <point>
                    <fullname> /Assets/Precipitation/Train1/Precipitator/Cyclone1/Flow </fullname>
                    <tagname> FL111 </tagname>
                </point>
            </points>
        </HwEmData>
        */
        strXmlPoint = '<?xml version="1.0" encoding="UTF-16"?>';
        strXmlPoint = strXmlPoint + '<HwEmData xmlns="x-schema:HwEmDataSchema.xml">';
        strXmlPoint = strXmlPoint + '<points';
        if (strActive != "")
        {
        strXmlPoint = strXmlPoint + ' active="';
        strXmlPoint = strXmlPoint + JsTrim(strActive);
        }
        strXmlPoint = strXmlPoint + '">';
        strXmlPoint = strXmlPoint + '<point><fullname>';
        strXmlPoint = strXmlPoint + JsTrim(strFullname);
        strXmlPoint = strXmlPoint + '</fullname><tagname>';
        strXmlPoint = strXmlPoint + JsTrim(strTagname);
        strXmlPoint = strXmlPoint + '</tagname></point></points></HwEmData>';
        event.dataTransfer.clearData();
        if (strActive == "tagname") {
            if (g_SystemToggled)
            {
                var strType = "";
                var strSubtype = "";
                var strItemname = "";
                var strOwner = "";
                var arrTokens = strTypeSubtypeItemnameOwner.split(".");
                var nToken = arrTokens.length;
                if (nToken > 0)
                    strType = arrTokens[0];
                if (nToken > 1)
                    strSubtype = arrTokens[1];
                if (nToken > 2)
                    strItemname = arrTokens[2];
                if (nToken > 3)
                    strOwner = arrTokens[3];
                fnSetLastSelectedPointOwner(strOwner);
                strTagname = fnPrependServerName(strTagname);
                if ((strType.length + strSubtype.length + strItemname.length) > 0)
                {
                    if ((parseInt(strType, BASE_DECIMAL) == POINT_TYPE_CLH) && (parseInt(strSubtype, BASE_DECIMAL) == POINT_SUBTYPE_SYS_ALMGRP))
                        strTagname = "#DATAFROMSYSTEMPOINTPICKER#" + g_DashboardLinkName + ".sha#" + strTagname + "#" + strItemname;
                    else if((parseInt(strSubtype, BASE_DECIMAL) >= MIN_LCNNODE_SUBTYPE) && (parseInt(strSubtype, BASE_DECIMAL) <= MAX_LCNNODE_SUBTYPE))
                        strTagname = "#DATAFROMSYSTEMPOINTPICKER#" + g_DashboardLcnShape + ".sha#" + strTagname + "#" + strItemname;
                    else if((parseInt(strSubtype, BASE_DECIMAL) >= MIN_UCNNODE_SUBTYPE) && (parseInt(strSubtype, BASE_DECIMAL) <= MAX_UCNNODE_SUBTYPE))
                        strTagname = "#DATAFROMSYSTEMPOINTPICKER#" + g_DashboardUcnShape + ".sha#" + strTagname + "#" + strItemname;
                    else if((parseInt(strSubtype, BASE_DECIMAL) >= MIN_IOPNODE_SUBTYPE) && (parseInt(strSubtype, BASE_DECIMAL) <= MAX_IOPNODE_SUBTYPE))
                        strTagname = "#DATAFROMSYSTEMPOINTPICKER#" + g_DashboardIopShape + ".sha#" + strTagname + "#" + strItemname;
                    else if(parseInt(strSubtype, BASE_DECIMAL) == BRIDGE_SUBTYPE || parseInt(strSubtype, BASE_DECIMAL) == UCN_SUBTYPE || parseInt(strSubtype, BASE_DECIMAL) == LCN_SUBTYPE)
                        strTagname = "#DATAFROMSYSTEMPOINTPICKER#" + g_GenericStatusShape + ".sha#" + strTagname + "#" + strItemname;
                    else
                        strTagname = "#DATAFROMSYSTEMPOINTPICKER#StatusShapeSubType" + strSubtype + ".sha#" + strTagname + "#" + strItemname;
                }
            }
            PBControlObject.OnDragStart(g_ClipboardFormat, strXmlPoint, strTagname);
        } 
        else if(strActive=="fullname")
        {
            PBControlObject.OnDragStart(g_ClipboardFormat, strXmlPoint, strFullname);
        }
        else
        {
            PBControlObject.OnDragStart(g_ClipboardFormat, strXmlPoint, strTagname);
        }
        event.cancelBubble = true;
        return;
    }
    function fnSetLastSelectedPointTypeInfo(Type, SubType)
    {
        g_LastSelectedPointTypeAndSubType = String(Type) + "." + String(SubType);
    }
    function fnSetLastSelectedPointOwner (Owner)
    {
        g_LastSelectedPointOwner = String(Owner);
    }
    //-------------------------
    function fnSetInfo(strText) {                                                        
        event.dataTransfer.clearData();
        event.dataTransfer.setData("Text", strText); 
        event.dataTransfer.effectAllowed = "copy";                 
        event.cancelBubble = true;
        return;
    }
    //-------------------------
    function fnGetInfo() {
        event.returnValue = false;                           
        event.dataTransfer.dropEffect = "copy";              
        oTarget.value = event.dataTransfer.getData("Text");  
    }
    //-------------------------
    function fnCancelDefault() {    
        event.dataTransfer.dropEffect = "copy";
    }
    //-------------------------
    function fnOnClickSelectable(object, strText) {
        g_textRange.moveToBookmark(g_sBookMark);
        if (strText != "")
        {
            g_textRange.moveToElementText(object);
            if (g_textRange.findText(strText) == true) {
                g_textRange.select();
            }
        }
        event.returnValue = false;
    }
    //-------------------------
    function fnIsContentSelected() {
        if (g_textRange.text != "") {
            return true;
        }
        return false;
    }
    //-------------------------
    function fnOnClickCell(strText) {
        try {
            divText.innerText = strText;
            fnOnClickSelectable(divText, strText);
        }
        catch(err) {
        }
    }
    //-------------------------
    function fnOnDblClickTag() {
        if (window.external.frozen == false) {
            window.external.raiseEvent("event_onpointselect", txtTagName.value);
        }
        // Set focus to Tag Table
        TagTable.focus();
    }
    //------------------------
    function fnIsSystemMode() {
        var SystemMode;
        SystemMode = TagDataObject.FilterOptions & g_FLAG_SYSTEM_MODE;
        return SystemMode;            
    }
    //-------------------------
    function fnOnBeforeCopy() {
        event.returnValue = false;
    }
    //-------------------------
    function fnOnCopy(strText) {
        window.clipboardData.setData("text",strText);
        g_textRange.moveToBookmark(g_sBookMark);
        g_textRange.collapse();
        g_textRange.select();
        return;
    }
    //-------------------------
    function fnPositionAll() 
    {
        var nTemp = 0;
        var nTagTop = 0;
        var nTagHeight = 0;
        var nTagWidth = 0;
        var nEntityTop = 0;
        var nEntityHeight = 0;
        var nEntityWidth = 0;
        g_nEmHeight = txtTagName.parentElement.parentElement.style.pixelHeight;
        g_nEmWidth = txtTagName.parentElement.parentElement.style.pixelWidth;
        if (g_nEntityTreeHeight < MIN_TREE_HEIGHT) { g_nEntityTreeHeight = MIN_TREE_HEIGHT; }
        if (g_nEntityTreeHeight > MAX_TREE_HEIGHT) { g_nEntityTreeHeight = MAX_TREE_HEIGHT; }
        EntityTable.style.pixelHeight = g_nEntityTreeHeight;
        assBtn.style.pixelLeft = 0;
        assBtn.style.pixelWidth = g_nEmWidth/2;
        assBtn.style.pixelTop = 5;
        sysBtn.style.pixelWidth = g_nEmWidth/2;
        sysBtn.style.pixelLeft = g_nEmWidth/2;
        sysBtn.style.pixelTop = 5;
        txtTagName.style.pixelTop = 30;
        txtTagName.style.pixelWidth = g_nEmWidth - 0;
        if (g_ShowFilter)
        {
            EntityTable.style.visibility = "visible";
            chkFilter.style.visibility = "visible";
            lblFilter.style.visibility = "visible";
        } else {
            EntityTable.style.visibility = "hidden";
            chkFilter.style.visibility = "hidden";
            lblFilter.style.visibility = "hidden";
        }
        btnToggleTree.style.pixelTop = txtTagName.style.pixelTop + txtTagName.style.pixelHeight  + 1;
        btnToggleTree.style.width = btnToggleTree.clientWidth;
        btnToggleTree.style.pixelLeft = g_nEmWidth - 2 * (btnToggleTree.style.pixelWidth + 3 );
        btnClearFilter.style.width = btnClearFilter.clientWidth;        
        btnClearFilter.style.pixelLeft = btnToggleTree.style.pixelLeft + btnToggleTree.style.pixelWidth + 5;
        btnClearFilter.style.pixelTop = btnToggleTree.style.pixelTop;    
        chkFilter.style.pixelTop = btnToggleTree.style.pixelTop + btnToggleTree.style.pixelHeight  + 1;
        lblFilter.style.pixelLeft = chkFilter.style.pixelWidth + 1;
        lblFilter.style.pixelTop = chkFilter.style.pixelTop + 1;
        lblFilter.style.width = lblFilter.clientWidth;
        nEntityTop = chkFilter.style.pixelTop + chkFilter.style.pixelHeight + 2;
        nEntityWidth = g_nEmWidth - 0;
        nEntityHeight = EntityTable.style.pixelHeight;
        if (g_bUIEnabled) {
            EntityTable.style.pixelTop = nEntityTop;
            EntityTable.style.pixelWidth =  nEntityWidth;
        }
        //------------------------------------------------
        nTemp = g_nEmHeight - txtTagName.style.pixelTop - 1;
        nTemp = nTemp - txtTagName.style.pixelHeight - 2;
        nTemp = nTemp - lblFullName.style.pixelHeight - txtFullName.style.pixelHeight - 3;
        nTemp = nTemp - lblDescription.style.pixelHeight - txtDescription.style.pixelHeight - 3;
        nTemp = nTemp - btnToggleTree.style.pixelHeight;
        if (g_ShowFilter) {
            nTemp = nTemp - EntityTable.style.pixelHeight - 3;
            nTemp = nTemp - chkFilter.style.pixelHeight - 1;
            nTagTop = EntityTable.style.pixelTop + EntityTable.style.pixelHeight + 13 ;
        }
        else
        {
            nTagTop = btnToggleTree.style.pixelTop + btnToggleTree.style.pixelHeight  + 5;
        }
        if (nTemp < MIN_TAGLIST_HEIGHT) { nTemp = MIN_TAGLIST_HEIGHT;}
        if (nTemp > MAX_TAGLIST_HEIGHT) { nTemp = MAX_TAGLIST_HEIGHT;}
        nTagHeight = nTemp - (nTemp % 15);
        nTagWidth = g_nEmWidth - 0;
        if (g_bUIEnabled) {
            TagTable.style.pixelTop = nTagTop;
            TagTable.style.pixelHeight = nTagHeight;
            TagTable.style.pixelWidth = nTagWidth;
        }
        lblFullName.style.pixelTop = nTagTop + nTagHeight + 2;
        lblFullName.style.width = lblFullName.clientWidth;    
        txtFullName.style.pixelTop = lblFullName.style.pixelTop + lblFullName.style.pixelHeight + 1;
        txtFullName.style.pixelLeft =  0;
        txtFullName.style.pixelWidth =  g_nEmWidth - txtFullName.style.pixelLeft - 0;
        lblDescription.style.pixelTop = txtFullName.style.pixelTop + txtFullName.style.pixelHeight + 1;
        lblDescription.style.width = lblDescription.clientWidth;
        txtDescription.style.pixelLeft = 0;
        txtDescription.style.pixelTop = lblDescription.style.pixelTop + lblDescription.style.pixelHeight + 1;
        txtDescription.style.pixelWidth =  g_nEmWidth - txtDescription.style.pixelLeft - 0;
        if (g_Release < 440 && g_Release != 0)// If it is from station, 0 will be shown as the release call is not made.
        {
            if(g_SystemToggled)
            {
                fnToggleAsset();//Make sure we are in Asset mode before hiding
            }
            assBtn.style.visibility= "hidden";
            sysBtn.style.visibility= "hidden";
        }
        else 
        {
            assBtn.style.visibility= "visible";
            sysBtn.style.visibility= "visible";
        }
    } // fnPositionAll()
    //-------------------------
    function fnEnableUI() 
    {
        txtTagName.disabled = false;
        txtTagName.style.backgroundColor = "";
        TagTable.style.visibility = "visible";        
        txtFullName.firstChild.style.backgroundColor = g_strBckgndColor;
        txtDescription.firstChild.style.backgroundColor = g_strBckgndColor;
        EntityTable.style.visibility = "visible";
        chkFilter.disabled = false;
        btnToggleTree.disabled = false;
        btnClearFilter.disabled = false;
        scrpltBody.style.backgroundColor = g_strBckgndColor;
        scrpltBody.style.cursor = "default";
        g_bUIEnabled = true;
    } 
    //-------------------------
    function fnDisableUI() 
    {
        txtTagName.value = "";
        txtTagName.disabled = true;
        txtTagName.style.backgroundColor = g_strDisabledBckgndColor;
        TagTable.style.visibility = "hidden";      
        txtFullName.title = "";
        txtFullName.firstChild.innerText = "";
        txtFullName.firstChild.style.backgroundColor = g_strDisabledBckgndColor;
        txtDescription.title = "";
        txtDescription.firstChild.innerText = "";
        txtDescription.firstChild.style.backgroundColor = g_strDisabledBckgndColor;
        EntityTable.style.visibility = "hidden";
        chkFilter.disabled = true;
        btnToggleTree.disabled = true;
        btnClearFilter.disabled = true;
        scrpltBody.style.backgroundColor = g_strDisabledBckgndColor;
        g_bUIEnabled = false;
    } 
    //-------------------------
    function fnToggleTree() 
    {
        var sVisi;
        sVisi = EntityTable.style.visibility;
        if (sVisi == "visible") {
            g_ShowFilter = false;
        } else {
            g_ShowFilter = true;
        }
        fnPositionAll();
    }
    //-------------------------
    function fnToggleChecked()
    {
        if (!g_IncludeSubTree) {
            chkFilter.checked = true;
            g_IncludeSubTree = true;
        } else {
            chkFilter.checked = false;
            g_IncludeSubTree = false;
        }
        TagDataObject.FilterOptions = TagDataObject.FilterOptions ^ g_FLAG_USE_DESCENDENTS;
    }
    //-------------------------
    function fnToggleSystem()
    {
        if (!g_SystemToggled){
            //turn Asset button "off"
            assBtn.style.backgroundColor = g_ModeButtonColorOff;
            assBtn.style.color = g_ModeButtonColorOn;
            //turn System button "on"
            sysBtn.style.backgroundColor = g_ModeButtonColorOn;
            sysBtn.style.color = g_ModeButtonColorOff;
            g_SystemToggled =true;
            TagDataObject.FilterOptions = TagDataObject.FilterOptions ^ g_FLAG_SYSTEM_MODE;
        }
    }
    function fnToggleAsset()
    {        
        if (g_SystemToggled){
            //turn Asset button "on"
            assBtn.style.backgroundColor = g_ModeButtonColorOn;
            assBtn.style.color = g_ModeButtonColorOff;
            //turn System button "off"
            sysBtn.style.backgroundColor = g_ModeButtonColorOff;
            sysBtn.style.color = g_ModeButtonColorOn;
            g_SystemToggled = false;
            TagDataObject.FilterOptions = TagDataObject.FilterOptions ^ g_FLAG_SYSTEM_MODE;
        }
    }
    //-------------------------
    function fnClearFilter()
    {
        if (g_bTreeDOInitialized) {
            EntityDataObject.SelectNone(); 
            Handle_OnEntitySelectionChange();
            if (txtTagName.value != "") {
                try {TagDataObject.Find(txtTagName.value);} catch(err){return;}
            }
        }
    }
    //-------------------------
    function Handle_OnEntitySelectionChange()
    {
        TagDataObject.EntityFilterList = EntityDataObject.SelectionListArray;
        if (txtTagName.value != "") {
            try {TagDataObject.Find(txtTagName.value);} catch(err){return;}
        }
    }
    //-------------------------
    function Handle_OnTagSelectionChange()
    {
        txtFullName.firstChild.innerText = g_LastSelectedFullname;
        txtFullName.title = g_LastSelectedFullname;
        var sDescription = "";
        sDescription = PBControlObject.getPointDescriptionV(PBControlObject.connectionNo, g_LastSelectedTagname);
        g_LastSelectedDescription = sDescription;
        txtDescription.firstChild.innerText = sDescription;
        txtDescription.title = sDescription;
    }
    //-------------------------
    function Handle_OnTagClearSelection()
    {
        fnSetLastSelectedTagname("");
        fnSetLastSelectedFullname("");
        fnSetLastSelectedPointTypeInfo("", "");
        fnSetLastSelectedPointOwner("");
        txtFullName.firstChild.innerText = "";
        txtFullName.title = "";
        txtDescription.firstChild.innerText = "";
        txtDescription.title = "";
    }
    //-------------------------
    function fnSetLastSelectedTagname(sTagname)
    {
        g_LastSelectedTagname = sTagname;
    }
    //-------------------------
    function fnSetLastSelectedFullname(sFullname)
    {
        g_LastSelectedFullname = sFullname;
    }
    //-------------------------    
    function fnHandleOnEntryTimeout()
    {
        var nElapsedTick = 0;
        var nElapsedTime = 0;        
        g_nCurrentTick++;
        if (g_nCurrentTick > g_nMaxTick) {
            g_nCurrentTick = 1;
        }
        if (txtTagName.value != g_strLastFindStr) {
            g_strLastFindStr = txtTagName.value;
            g_nLastChangeTick = g_nCurrentTick;
            g_bChangedFind = true;
            return;
        }
        if (PBControlObject.connectionNo < 0) {return;}
        nElapsedTick = g_nCurrentTick - g_nLastChangeTick;
        if (nElapsedTick < 0) {nElapsedTick += g_nMaxTick;}
        nElapsedTime = nElapsedTick * g_nTickTimeout;
        if ( g_bChangedFind && (nElapsedTime > g_nTypingTimeout) && 
             (nElapsedTime < (g_nTypingTimeout + g_nRetryWindow)) ) {
            try {TagDataObject.Find(txtTagName.value);} catch(err){return;}
            g_bChangedFind = false;
        }
    }
    //-------------------------    
    function fnShowStatus(sStatusMessage)
    {
        var nTemp = 0;
        g_nEmHeight = tblStatus.parentElement.parentElement.style.pixelHeight;
        g_nEmWidth = tblStatus.parentElement.parentElement.style.pixelWidth;
        tblStatus.title = sStatusMessage;
        tblStatus.rows[0].cells[0].innerHTML = sStatusMessage;
        tblStatus.style.zIndex = 2;
        tblStatus.style.visibility = "visible";
        nTemp = (g_nEmHeight - tblStatus.style.pixelHeight) / 2;
        if (nTemp < 0) {nTemp = 0;}
        tblStatus.style.pixelTop = nTemp;
        nTemp = (g_nEmWidth - tblStatus.style.pixelWidth) / 2;
        if (nTemp < 0) {nTemp = 0;}
        tblStatus.style.pixelLeft = nTemp;
        return;
    }
    //-------------------------    
    function fnHideStatus()
    {
        tblStatus.title = "";
        tblStatus.rows[0].cells[0].innerHTML = "";
        tblStatus.style.zIndex = -1;
        tblStatus.style.visibility = "hidden";
        return;
    }
    //-------------------------    
    function fnEntityCellSelect(strText) 
    {
        fnOnClickCell(strText);
        // Set focus to Entity Table
        EntityTable.focus();
    }
    //-------------------------
    function fnTagCellSelect(strText)
    {
        fnOnClickCell(strText);
        // Set focus to Tag Table
        TagTable.focus();
    }
// /************************************************************************/
// /*****     COPYRIGHT 2008 - 2009    HONEYWELL INTERNATIONAL SARL     ****/
// /************************************************************************/

