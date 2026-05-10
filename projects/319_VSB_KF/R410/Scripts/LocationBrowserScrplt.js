// /************************************************************************/
// /****     COPYRIGHT 2008 - 2009    HONEYWELL INTERNATIONAL SARL      ****/
// /************************************************************************/
// LocationBrowserScrplt.js
// $Revision: 12 $
var EVENT_ONREADY               = "event_onready";
var EVENT_ONPOINTSELECT         = "event_onpointselect";
var EVENT_ONSERVERSTATUSCHANGE  = "event_onserverstatuschange";
var MIN_TAGLIST_HEIGHT          = 20;
var MAX_TAGLIST_HEIGHT          = 15 * 50; // TagTable.rowHeight * MAX_VIEWSIZE
var MIN_TREE_HEIGHT             = 20;
var MAX_TREE_HEIGHT             = 500;
var g_strBckgndColor            = "";
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
var g_nEntityTreeHeight         = 80;
var g_nEmHeight                 = 300;
var g_nEmWidth                  = 100;
var g_TagSelectionChangeXML     = "";
var g_EntitySelectionChangeXML  = "";
var g_LastSelectedTagname       = "";
var g_LastSelectedFullname      = "";
var g_LastSelectedPointNumber   = "";
var g_LastSelectedDescription   = "";
var g_LastSelectedEntityname    = "";
var g_LastSelectedEntityDescription = "";
var g_LastSelectedAreaNumber    = "";
var g_ClipboardFormat           = "CF_HWEMDATA";
var g_textRange                 = null;
var g_sBookMark                 = "";
var g_objTextRange              = null;
var g_EntityType                = ""; 
var g_OrganizationNodesFound	= false;
var g_BrowserInitialised		= false;
var g_bIsAssigned               = false;
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
        catch( err ) 
        {
            EntityDataObject.attachEvent("OnAckTransaction", OnPersist_EntityState_LOAD);
            return;
        }
        EntityDataObject.detachEvent("OnAckTransaction", OnPersist_EntityState_LOAD);
        if (g_bTreeDOInitialized == true) 
        {
        return;
        }
        EntityDataObject.Version = 4.5;
        EntityDataObject.AutoMigrate = 0;
        EntityDataObject.WaitForSelectionList = 1;
        EntityDataObject.SummaryType = 5;
        EntityDataObject.StartLocation = g_strTdoStartLocation;
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
    fnHideStatus();
    fnEnableUI();
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
	// Takes this path on a first callup or refresh.
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
    fnHideStatus();
    fnEnableUI();
    EntityTable.style.pixelHeight = 10;
    fnPositionAll();         
    fnClearFilter();
    fnEvent_onserverstatuschange();
    // Initialise the g_OrganizationNodesFound so it can be reset to the correct value on the first Organization filter.
    g_OrganizationNodesFound = false;
    // Initialise g_BrowserInitialised so the filter gets reset on a first callup or refresh.
    g_BrowserInitialised = false;	
    // This is required to initiate the filter based on varaibles set from the first call to this function through the interface.  
    mfnShow(g_EntityType);
    // Now set this to true so the filter doesn't get reset 
    // if g_EntityType is the same as nEntiryType when mfnShow() is next called through the interface.
    g_BrowserInitialised = true;
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
    // returns the tagname of the currently selected location. If no location is selected, an empty string is returned.    
    if (g_bIsAssigned) {
    	return JsTrim(g_LastSelectedTagname);
    }
}
//-------------------------
function mfnGetAreaNumber()
{
    // returns the area number of the currently selected location. If no location is selected, an empty string is returned.
    if (g_bIsAssigned) {
    	return JsTrim(g_LastSelectedAreaNumber);
    }
}
//-------------------------
function mfnGetFullName()
{
    // returns the full name of the currently selected location. If no location is selected, an empty string is returned.
    if (g_bIsAssigned) {
    	return JsTrim(g_LastSelectedFullname);	
    }	
}
//-------------------------
function mfnGetPointNumber()
{
    // returns the point number of the currently selected node. If no node is selected, an empty string is returned.
    if (g_bIsAssigned) {
    	return JsTrim(g_LastSelectedPointNumber);
    }
}
//-------------------------
function mfnGetEntityDescription()
{
    // returns the description of the currently selected entity. If no entity is selected, an empty string is returned.
    if (g_bIsAssigned) {
    	return JsTrim(g_LastSelectedEntityDescription);
    }	
}
//-------------------------
function mfnRefresh()
{
	// will refresh the graphical elements of the Point Browser, using the current operator security level and Enterprise Model filter. Method should only need to be called when Point Browser is operating in Station.
    var sAlert;
    sAlert = "Refresh(): ";
    alert(sAlert);
    // refresh all objects; 
    // including obtaining new security restrictions;
    return;
}
//-------------------------
function fnDragPoint(strTagname, strFullname, strActive) {
    var strXmlPoint;
    if ( (strActive != "tagname") && 
            (strActive != "fullname") && 
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
        PBControlObject.OnDragStart(g_ClipboardFormat, strXmlPoint, strTagname);
    } else {
        PBControlObject.OnDragStart(g_ClipboardFormat, strXmlPoint, strFullname);
    }
    event.cancelBubble = true;
    return;
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
    if (g_bIsAssigned) {
        if (window.external.frozen == false) {
	    window.external.raiseEvent("event_onpointselect", g_LastSelectedTagname);
    	}
		//Set focus to Entity Table
    	EntityTable.focus();
    }
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
//------------------------
function fnPositionFilterOn()
{
    var nTemp = 0;
    var nTagTop = 0;
    var nTagHeight = 0;
    var nTagWidth = 0;
    g_nEmHeight = lblTypePoint.parentElement.parentElement.style.pixelHeight;
    g_nEmWidth = lblTypePoint.parentElement.parentElement.style.pixelWidth;
    lblTypePoint.style.width = lblTypePoint.clientWidth;
    lblTypePoint.style.pixelTop = nTagTop
    lblTypePoint.style.pixelWidth = g_nEmWidth - 0;
	if (g_bUIEnabled) {     
        EntityTable.style.pixelTop = nTagTop + 20;
    }
    nTagHeight = g_nEmHeight - lblTypePoint.style.pixelHeight - 40;;
    nTagWidth = g_nEmWidth - 0;
    if (g_bUIEnabled) {
        EntityTable.style.pixelHeight = nTagHeight;
        EntityTable.style.pixelWidth = nTagWidth;
    }	
    lblDescription.style.pixelTop = nTagTop + nTagHeight + 30;
    lblDescription.style.width = lblDescription.clientWidth;
    txtDescription.style.pixelLeft = lblDescription.style.pixelWidth + 2;
    txtDescription.style.pixelTop = nTagTop + nTagHeight + 30;
    txtDescription.style.pixelWidth =  g_nEmWidth - txtDescription.style.pixelLeft - 0;
    EntityTable.style.visibility = "visible";
}
//-------------------------
function fnPositionAll() 
{
    fnPositionFilterOn();
} 
//-------------------------
function fnEnableUI() 
{
    txtDescription.firstChild.style.backgroundColor = g_strBckgndColor;
    EntityTable.style.visibility = "visible";
    scrpltBody.style.backgroundColor = g_strBckgndColor;
    scrpltBody.style.cursor = "default";
    g_bUIEnabled = true;
}
//-------------------------
function fnDisableUI() 
{
    txtDescription.title = "";
    txtDescription.firstChild.innerText = "";
    txtDescription.firstChild.style.backgroundColor = g_strDisabledBckgndColor;
    EntityTable.style.visibility = "hidden";
    scrpltBody.style.backgroundColor = g_strDisabledBckgndColor;
	g_bUIEnabled = false;
} 
//-------------------------
function fnClearFilter()
{
    if (g_bTreeDOInitialized) {
        EntityDataObject.SelectNone(); 
        Handle_OnEntitySelectionChange();
    }
}
//-------------------------
function Handle_OnEntitySelectionChange()
{
    txtDescription.firstChild.innerText = g_LastSelectedEntityDescription;
    txtDescription.title = g_LastSelectedEntityDescription;
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
function fnSetLastSelectedPointNumber(sPointNumber)
{
    g_LastSelectedPointNumber = sPointNumber;
}
//-------------------------
function fnSetLastSelectedAreaNumber(sAreaNumber)
{
    g_LastSelectedAreaNumber = sAreaNumber;    
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
	if (PBControlObject.connectionNo < 0) {return;}
	nElapsedTick = g_nCurrentTick - g_nLastChangeTick;
    if (nElapsedTick < 0) {nElapsed += g_nMaxTick;}
    nElapsedTime = nElapsedTick * g_nTickTimeout;
    if ( g_bChangedFind && (nElapsedTime > g_nTypingTimeout) && 
            (nElapsedTime < (g_nTypingTimeout + g_nRetryWindow)) ) {
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
function fnSetLastSelectedDescription(sDescription) 
{
    g_LastSelectedEntityDescription = sDescription;
}
//-------------------------
// Called from the interface and within the scripts
function mfnShow(nEntityType)
{
// Defined in EMScripts.vbs
// ItemNumber
	// em_inum_SYS_MODEL_ASSETS			= 1
	// em_inum_SYS_MODEL_ALARMGROUPS		= 2
	// em_inum_SYS_MODEL_SYSTEM			= 3
	// em_inum_SYS_MODEL_ORGANIZATIONS     	= 5
//RemoteType
	// em_type_ORH		= 9
	// em_type_ASH		= 6
	// em_type_CLH		= 7
	// Do not reset the filter if it hasn't changed and it has already been initialised.
	if ( (g_EntityType == nEntityType) && (g_BrowserInitialised) )
	{
		return;
	}
    g_EntityType = nEntityType;    
	// Initialise the Last Selected Description because the filter is being reset.
    g_LastSelectedEntityDescription = "";
    txtDescription.firstChild.innerText = g_LastSelectedEntityDescription;
    txtDescription.title = g_LastSelectedEntityDescription;
    if (g_EntityType == 1)
    {
		// Only show the Asset Model
		// If the AuxFilter is changed, please consider changing other entity types that also include Asset Model
		lblTypePoint.innerText = "Select an Asset:"; 
		EntityDataObject.AuxFilter = "(IsAssigned or HasAssignedDescendents) and (ItemNumber = 1 or ItemNumber = 0 or RemoteType = 6)"; 
    }
    else if (g_EntityType == 2)
    {
		// Only show the Organization Model
		lblTypePoint.innerText = "Select an Organization:";        
		EntityDataObject.AuxFilter = "(IsAssigned or HasAssignedDescendents) and (ItemNumber = 5 or RemoteType = 9)"; 
    }
    else if (g_EntityType == 3)
    {
		// Prior to setting the auxiliary filter, evaluate the organization model first, in case the window is too small to show it.
		// This makes sure that the g_OrganizationNodesFound flag is set correctly so the node isn't shown on a Builing Management system.
		EntityDataObject.AuxFilter = "(ItemNumber = 5 or RemoteType = 9)";        
        if (g_OrganizationNodesFound)
		{
			// Show both the Facility Model and Organization Model
			lblTypePoint.innerText = "Select a Location or Organization:"; 	
			EntityDataObject.AuxFilter = "(IsAssigned or HasAssignedDescendents) and ( ItemNumber = 5 or RemoteType = 9 or ItemNumber = 1 or RemoteType = 6 )";       
        }
        else
		{
			// Only show the Facility Model as there was no Organization Model built
			lblTypePoint.innerText = "Select a Location:"; 			
			EntityDataObject.AuxFilter = "(IsAssigned or HasAssignedDescendents) and (ItemNumber = 1 or RemoteType = 6)";
 		}
    }
    else if (g_EntityType == 4)
    {
		// Only show the Alarm Groups
		// If the AuxFilter is changed, please consider changing other entity types that also include Alarm Groups
		lblTypePoint.innerText = "Select an Alarm Group:";
		EntityDataObject.AuxFilter = "(IsAssigned or HasAssignedDescendents) and (ItemNumber = 2 or RemoteType = 7)"; 
    }
	else if (g_EntityType == 5)
    {
		// Show Asset and System Component Model
        lblTypePoint.innerText = "Select an Asset:"; 
		EntityDataObject.AuxFilter = "(IsAssigned or HasAssignedDescendents) and (ItemNumber = 3 or RemoteType = 6 or ItemNumber = 1 or ItemNumber = 0 or RemoteType = 6)"; 
    }
    else if (g_EntityType == 6)
    {
        // Show both Asset Model and Alarm Groups
        lblTypePoint.innerText = "Select an Asset or Alarm Group:";
        EntityDataObject.AuxFilter = "(IsAssigned or HasAssignedDescendents) and ((ItemNumber = 1 or ItemNumber = 0 or RemoteType = 6) or (ItemNumber = 2 or RemoteType = 7))";
    }
}
// /************************************************************************/
// /****     COPYRIGHT 2008 - 2009    HONEYWELL INTERNATIONAL SARL      ****/
// /************************************************************************/
