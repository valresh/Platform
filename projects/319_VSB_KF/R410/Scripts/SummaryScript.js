// SummaryScript.js
// $Revision: 219 $
// $Date: 27/05/10 3:09p $
var PROP_Version		= "Version";
var PROP_Selected		= "Selected";
var PROP_Title			= "Title";
var PROP_ViewName		= "ViewName";
var PROP_Displayed		= "DISPLAY";
var PROP_DashboardGroup	= "SystemStatusPane_Group";
// The name of the attribute to which the table state is saved
var PROP_TableState		= "TableState";
var PROP_TDOState		= "TDOState";
// Attribute added to monitor the resolution display persisted in
var PROP_Width		= "Width";
var PROP_Height		= "Height";
var VK_PUP				= 33;
var VK_PDN				= 34;
var VK_AUP				= 38;
var VK_ADN				= 40;
var VK_HOME				= 36;
var VK_END				= 35;
// Error codes
var ERR_SAVEVIEW_SUCCESS		= 266256;
var ERR_SAVEVIEW_ALREADYEXISTS	= -2147217390;
var ERR_LCS_NOSELECTION			= -2147217372;
var ERR_VIEW_DEGRADEDMODE		= 298832;
var ERR_VIEW_REFRESHNOW			= 298835;
var ERR_COMMENT_UNAVAILABLE		= 298763;
var ERR_COMMENT_REFRESH			= 298764;
var ERR_COMMENT_PRE800_DSAALARM	= 298765;
var ERR_ALARM_OVERFLOW 			= -2147217395;
var ERR_NO_ALARM_OVERFLOW		= 266254;
// Summary report types
var SUMMARY_REPORT_PRINT		= 0;
var SUMMARY_REPORT_PREVIEW		= 1;
var SUMMARY_REPORT_CSV  		= 2;
// Summary types
var SUMMARY_ALARM				= 1;
var SUMMARY_MESSAGE				= 2;
var SUMMARY_EVENT				= 3;
var SUMMARY_SOE					= 4;
var SUMMARY_SYSTEM_ALARM		= 5;
var SUMMARY_ALERT				= 6;
var SUMMARY_EVENTVIEW			= 7;
var SUMMARY_ACTIVITY			= 8;
var SUMMARY_BATCH   			= 9;
var SUMMARY_PROCEDURE			= 10;
var SUMMARY_ACTIVITY_ORPHAN		= 11;
// Table components
var TC_ERROR			= 0;
var TC_SCROLLBAR		= 1;
var TC_MENU				= 2;
var TC_THEAD			= 3;
var TC_TBODY			= 4;
var TC_TABLE			= 5;
var NAME_SCROLLBAR		= "Scrollbar";
var NAME_MENU			= "Menu";
var NAME_THEAD			= "Table header";
var NAME_TBODY			= "Table body";
// callout
var CALLOUT_TIMEOUT     = 10;
var CALLOUT_LEVEL_ERROR = 2;
// detail pane tabs
var DETAIL_GENERAL_TAB        = 0;
var DETAIL_COMMENT_TAB        = 1;
var DETAIL_SUPPRESSION_TAB    = 2;
var DETAIL_ALARMHELP_TAB      = 3;
var DETAIL_BLOCKCOMMENT_TAB   = 4;
var DETAIL_INSTRUCTION_TAB    = 5;
// The panes which require persistence
var g_arrPanesToPersist = new Array(
                                "PaneContainer",
                                "AreaPane",
                                "VerticalResizePad",
                                "InnerPaneContainer",
                                "ViewConfigPane",
                                "GridPane",
                                "HorizontalResizePad_BOTTOM",
                                "DetailsPane",
                                "AlarmTrendPane",
                                "StatusPane",
                                "HPadAlarmTrend",
                                "SystemStatusPane",
                                "HorizontalResizePad_TOP"
                            );
// The buttons which require persistence. Make sure these buttons are named using the
// notation below - that is, 'btn' followed by the pane identifier.
var g_arrButtonsToPersist = new Array(
                                    "btnAreaPane",
                                    "btnViewConfigPane",
                                    "btnDetailsPane",
                                    "btnAlarmTrendPane",
                                    "btnStatusPane",
                                    "btnSystemStatusPane"
                                );
var g_TDO				= null;
var g_strViewCacheName		= "";
var g_strLayoutCacheName		= "";
var g_oEventSink		= null;
var g_lDisplayWidth = 0;
var g_lDisplayHeight = 0;
var g_lDefaultDashboardWidth = 1276;
var g_lDefaultDashboardHeight = 425;
var g_lPanePadding		= 4;		// If you change this value, remember to change the appropriate
                                    // style property of the Resizer pads
var g_oCurrentPad		= null;
var g_oOrganizer		= null;
var g_bDisplayToolTips		= true;
var g_bViewPersistenceLoaded	= false;
var g_bLayoutPersistenceLoaded	= false;
var g_bUsingConfiguredDefaultView = false;
var g_bFiredOnce                = false;
var g_bTableIsActive		= false;
var g_bAreaTableIsActive	= false;
var g_bSaveViewsEnabled		= false;
var g_bWebCaptureMode		= false;
var g_strNotifyErrorMsg			= "";
var g_strEventInDegradedMode	= "The event summary is not viewable on this Station when the Server is unavailable";
var g_strSOEInDegradedMode		= "The SOE summary is not viewable on this Station when the Server is unavailable";
var g_bInDegradedMode			= false;
var g_bCommentUnavaiable		= false;
var g_bPageInitialized			= false;
var g_bPageReady			= false;
var g_bEnableLiveValueUpdates	= false;
var g_bEnableBlockComments      = false;
var g_LastFocusedObject = null;
var g_FocusedObject = null;
var g_bFlush = false;
var g_bPageNotFocused = false;
var g_bTable_DblClk = false;
var g_bGenerateEvent_onclick = false;	// set to true if user just clicked on the generate event button
var g_bAlreadyPersistedViewOnQuit = false;
var g_bAlreadyPersistedLayoutOnQuit = false;
var g_bLocationPaneInitialised = false;
var g_bAlarmTrendLabelsVisible = false;
var g_nAlarmTrendTimePeriod = 0;
var g_nDecimalPaddingWidth = 4; 
var g_ZoomTimer;
var g_bRefreshing = false;
var g_bFirstCommentsCallup = true;
var g_PersistanceSaveDisabled = false;
var g_EquipmentFilter = "";
var g_SourceFilter = "";
var g_DashboardUrlArgument = "";
var g_AssetUrlArgument = "";
var g_FilterURLParams = new Array("Source","Block","LocationFullName","SourceTagName");
var g_FilterCols = [];
var g_FilterVals = [];
var g_SelectFirstRow = false;
var g_ClearPersistedFilter = false;
var g_sOverviewDashboardTitle = "System Overview Dashboard";
var g_sOverviewDashboardFilename = "overview_dashboard.htm";
var g_sOverviewDashboardGroup = "/System Alarm Groups";
var g_Environment = window.external.application.Environment;
var SHOW_MS				= 0x03;
var SHOW_US				= 0x10;
var SHOW_TIME			= 0x04;
var SHOW_DATE			= 0x08;
var SUPPORT_US			= 0x40;
var SUPPORT_AM_PM		= 0x100;
var SHOW_DATE_TIME		= SHOW_DATE | SHOW_TIME;
var SHOW_TIME_MS		= SHOW_TIME | SHOW_MS | SUPPORT_US;
var SHOW_DATE_TIME_MS	= SHOW_DATE | SHOW_TIME_MS | SUPPORT_US;
var SHOW_DATE_TIME_US	= SHOW_DATE | SHOW_TIME_MS | SHOW_US | SUPPORT_US;
var STATION_ENV 		= 0;
//EBM summary display related
//variables used to render the appearance of the details/creation pane properly
var g_DataUIIsLoaded = false;
var g_BeforeCreationUIDetailsPaneShowing = false;
var g_CreationUIOn = false;
var ALL_ACTIVITIES = 0;
var BATCH_ONLY = 1;
var PROCEDURE_ONLY = 2;
var g_CreationFilter = ALL_ACTIVITIES;
var g_LastActDetailsPaneSize = 0;
var g_SelectedActName = "";
var g_SelectedActPublicName = "";
var CREATIONUI_CREATED = 0;
var g_SelectionOnGrid = false;
var g_CloseCreationUIOnUpdate = false;
var g_DataUIInErrorMode = false;
//constanst used for setting the hosting environment for the Creation/DataUI
var EBMCREATIONDATAUI_ACTIVITYSUMMARY = 0;
var EBMCREATIONDATAUI_BATCHSUMMARY = 1;
var EBMCREATIONDATAUI_PROCEDURESUMMARY = 2;
var EBMCREATIONDATAUI_POPUP = 3;
var g_OnTouchPanel = false;
var g_LargeFontRowHeight = 30;
var g_MediumFontRowHeight = 25;
var g_SmallFontRowHeight = 22;
//total width of the close button in the details pane
var g_CloseButtonWidth = 30;
//used for system alarm help
var g_HlpContentSize = 0;
var SYSALMHLP_CELL_ROWS = 2;
var SYSALMHLP_MIN_LINES_SHOWN = 1;
var g_DashboardHistoryArray = new Array();
var MAX_DASHHISTORY_SIZE = 11;
var g_MessageSummaryCookie = 0;
var NO_VISIBLE_ROW_SELECTED = -2;
var MAX_RESIZER_SIZE = 5;
// --------------------------------------------------------------------------------------
// OnPageLoad()
//
//	Called when the page loads
// --------------------------------------------------------------------------------------
function OnPageLoad()
{
    InitializeLogger();
    var themeCSS = document.getElementById('theme');
    if (themeCSS)
    {
        try
        {
            var colorTheme = window.external.application.ColorTheme;
            switch ( colorTheme )
			{
                case 2:
                    themeCSS.href = "Styles/EPKS_Summary_Light.css";
                    break;
			    case 1: 	
                    themeCSS.href = "Styles/EPKS_Summary_Dark.css";
                    break;
                default:
					var summaryDisplaysTheme = Window.external.Dictionary.Item("SummaryDisplaysTheme").Value;
					// check if we need to force light theme for summary displays
					if (summaryDisplaysTheme === 1) {
						themeCSS.href = "Styles/EPKS_Summary_Light.css";
					} else {
						//default to classic theme
						themeCSS.href = "Styles/EPKS_Summary_Classic.css";
					}
            }
        }
        catch (e)
        {
        }
    }
    // Adjust the size of the Pane Container
    SetPaneContainerSize();
    RetrieveFiltersFromURL();
    // Are we running on an E-server machine?
    var IsWebCaptureStation = window.external.application.status.IsWebCaptureStation;
    if (typeof IsWebCaptureStation != 'unknown')
    {
        g_bWebCaptureMode = IsWebCaptureStation;
    }
    // Initialize the focused object to be the Table
    if (Table != null)
    {
        g_FocusedObject = Table;
    }
    if ( g_SummaryType == SUMMARY_ALARM && AlarmTrendPane.style.display != "none")
    {
        AlarmTrendPane.style.pixelHeight = 300;
        AlarmTrendPane.style.display = "none";
    }
    if ( g_SummaryType == SUMMARY_ALARM && g_bEnableBlockComments )
    {
        btnLocaleTab.style.visibility = "visible";
        BlockCommentsAlpha.style.visibility = "visible";
    }
    try
    {
        LoadPersistenceCaches();
        LoadPersistedLayout();
        LoadPersistedView();
	SetFiltersFromURL();
        if( g_oEventSink == null )
        {
            g_oEventSink = new ActiveXObject("HDXElements.EventSink");
            g_oEventSink.Advise(window.external.Application);
            g_oEventSink.AttachEvent("OnOperatorChange", OnOperatorChange);
        }
        Table.style.visibility = "visible";
        if(Page.HasFocus)
        {
            Table.focus();
        }
        if( g_SummaryType == SUMMARY_SYSTEM_ALARM )
        {
            CheckSystemLinks();
        }
        if( g_SummaryType == SUMMARY_MESSAGE )
        {
            ShowInstructionsTab(false);
        }
        try
        {
            g_TDO.DVMDatabaseServerName = window.external.Dictionary.Item('DVMObjectModel').Value.DatabaseServer.Name;
            g_TDO.DVMWebServerPath = window.external.Dictionary.Item('DVMObjectModel').Value.WebServerPath;
        }
        catch ( DVMerr)
        {
        }
        g_bPageInitialized = true;
    }
    catch( err )
    {
        HANDLE_ERROR("OnPageLoad", err.number, err.description);
    }
	//Check if we are on the touch panel or not
	try
	{
        if(window.external.IsOnTouchPanel)
        {
            g_OnTouchPanel = true;
            g_LargeFontRowHeight = 35;
            g_MediumFontRowHeight = 35;
            g_SmallFontRowHeight = 35;
            var masterCSS = document.getElementById('masterTheme');
            masterCSS.href = "Styles/EPKS_SummaryStyle_Touch.css";
            AreaTable.rowheight = 35;
            Table.HeaderHeight = 35;
            Table.rowheight = 35;
            if(g_SummaryType == SUMMARY_EVENTVIEW)
            {
                divPaused.style.pixelBottom=5;
                lblFilterApplied.style.pixelRight=160;
                lblFilterApplied.style.pixelTop += 7;
            }
            else
            {
                AreaPane.style.pixelTop=91;
                ViewConfigPane.style.pixelHeight = 155;
                lblFilterApplied.style.pixelTop = 60;
            }
            if(g_SummaryType == SUMMARY_ALARM)
            {
                toolbar.style.pixelWidth=650;
            }
            else if(g_SummaryType == SUMMARY_EVENTVIEW)
            {
                toolbar.style.pixelWidth = 140;
            }
            else
            {
                toolbar.style.pixelWidth = 610;
            }
            SetTableSize();
        }
    }
    catch(err)
    {
        window.external.logmessage("Unable to query for touch panel status: " + err.message);
    }
}
// --------------------------------------------------------------------------------------
// LoadPersistenceCaches()
//
//	Responsible for loading the view and layout persistence from cache.
//	Called when the page is first loaded and when the operator changes.
// --------------------------------------------------------------------------------------
function LoadPersistenceCaches()
{
    try
    {
        // Don't modify the DOM if we're in print mode
        if( window.document.media == "print" )
            return;
        if (!g_bInDegradedMode && !g_bWebCaptureMode)
        {
            var view_bag = document.getElementById(PROP_Cache_View);
            var layout_bag = document.getElementById(PROP_Cache_Layout);
            var strAppendName = "";
            if (g_SummaryType == SUMMARY_EVENTVIEW)
            {
                strAppendName += "_tnd" + Table.TrendNumber;
            }
            var app = window.external.application;
            if( typeof(app) == "object" && app != null )
            {
                strAppendName += "_" + app.OperatorID;
                var strServerName = app.ServerName;
                // Strip off redundant suffix for server name
                var strServerSuffix = strServerName.charAt(strServerName.length - 1);
                if (strServerSuffix.toUpperCase() == "A" || strServerSuffix.toUpperCase() == "B")
                {
                    var numberOfCharsToTrim = 1;
                    if (strServerName.length > 2)
                    {
                        strServerSuffix = strServerName.charAt(strServerName.length - 2);
                        if (!isNaN(parseFloat(strServerSuffix)) && isFinite(strServerSuffix))
                        {
                            numberOfCharsToTrim = 2;
                        }
                    }
                    strServerName = strServerName.substring(0, strServerName.length - numberOfCharsToTrim);
                }
                strAppendName += "_" + strServerName;
            }
            // Replace '.' and '\' characters that appear in the Operator ID
            // when Windows based operator security is used.
            strAppendName = strAppendName.replace(".", "_");
            strAppendName = strAppendName.replace("\\", "_");
            g_strViewCacheName = PROP_Cache_View + strAppendName;
            g_strLayoutCacheName = PROP_Cache_Layout + strAppendName;
            view_bag.load(g_strViewCacheName);
            layout_bag.load(g_strLayoutCacheName);
            g_bAlreadyPersistedViewOnQuit = false;
            g_bAlreadyPersistedLayoutOnQuit = false;
        }
    }
    catch( err )
    {
        HANDLE_ERROR("LoadPersistenceCaches: error loading cache", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// LoadPersistedLayout()
//
//	Responsible for load the current page layout from persisted state. Called
//	when the page is loaded.
// --------------------------------------------------------------------------------------
function LoadPersistedLayout()
{
    try
    {
        // Don't modify the DOM if we're in print mode
        if( window.document.media == "print" )
            return;
        if (!g_bInDegradedMode && !g_bWebCaptureMode)
        {
            var layout_bag = document.getElementById(PROP_Cache_Layout);
            var v = layout_bag.getAttribute(PROP_Version);
            ZoomSummary();
            if( null != v && DISPLAY_VERSION == v )
            {
                var pane;
                var i = 0;
                // Compare current and old resolution, and do not load cached settings if different
                if ((layout_bag.getAttribute(PROP_Width) == g_lDisplayWidth) &&
                    (layout_bag.getAttribute(PROP_Height) == g_lDisplayHeight))
                {
                    var p;
                    for( i=0; i<g_arrPanesToPersist.length; i++ )
                    {
                        pane = document.getElementById(g_arrPanesToPersist[i]);
                        if (pane == null)
                            continue;
                        var visible = pane.style.display;
                        p = new Pane( pane );
                        p.Load(layout_bag);
                        if (p.name == "StatusPane" && g_SummaryType == SUMMARY_SYSTEM_ALARM)
                        {
                            g_statuspane_SelectedEntity = layout_bag.getAttribute(p.name + "_SelectedEntity");
                            g_statuspane_FaceplateName  = layout_bag.getAttribute(p.name + "_FaceplateName");
                        }
                        else if (p.name == "AlarmTrendPane" && g_SummaryType == SUMMARY_ALARM)
                        {
                            if (layout_bag.getAttribute(p.name + "_DISPLAY") != "none" && visible == "none")
                                ShiftToolbar(AlarmTrendPane.style.pixelHeight + HPadAlarmTrend.style.pixelHeight);
                            if (layout_bag.getAttribute(p.name + "_DISPLAY") == "none" && GetURLParam("HscFile") != "0")
                                ToggleAlarmTrendPane();
                            g_bAlarmTrendLabelsVisible = layout_bag.getAttribute(p.name + "_LabelsVisible");
                            g_nAlarmTrendTimePeriod = layout_bag.getAttribute(p.name + "_TimePeriod");
                        }
                    }
                    // ISR70075 - If cached value of a data table's height is too large, limit it to avoid Station bugging out
                    if (AreaPane.style.pixelHeight > (50 * Table.rowHeight + Table.headerHeight))
                        AreaPane.style.height = PaneContainer.style.pixelHeight;
                    CheckGridSize();
                    if( AreaPane.style.display == "inline" )
                    {
                        btnDropAreaPane.style.display = "none";
                        spnDropAreaPane.style.display = "inline";
                    }
                    else if( AreaPane.style.display == "none" )
                    {
                        btnDropAreaPane.style.display = "inline";
                        spnDropAreaPane.style.display = "none";
                    }
                    for( i=0; i<g_arrButtonsToPersist.length; i++ )
                    {
                        var btn = document.getElementById(g_arrButtonsToPersist[i]);
                        if (btn == null)
                            continue;
                        var selected = layout_bag.getAttribute(g_arrButtonsToPersist[i] + "_" + PROP_Selected);
                        var title = layout_bag.getAttribute(g_arrButtonsToPersist[i] + "_" + PROP_Title);
                        // If we don't have a 'selected' attribute persisted, use the pane visibility as
                        // a guide. This assumes the buttons are named according to a convention (see
                        // declaration of g_arrButtonsToPersist)
                        if ((selected == null ) || (selected == ""))
                        {
                            p = document.getElementById( g_arrButtonsToPersist[i].substr(3) );
                            btn.selected = p.style.display == "inline";
                        }
                        else
                            btn.selected = selected;
                        if( title != null )
                            btn.title = title;
                    }
                }
                else
                {
                    for( i=0; i<g_arrPanesToPersist.length; i++ )
                    {
                        pane = document.getElementById(g_arrPanesToPersist[i]);
                        if (pane == null)
                            continue;
                        var display = layout_bag.getAttribute(g_arrPanesToPersist[i] + "_" + PROP_Displayed);
                        if ( display == "inline" )
                        {
                            if (pane.id == "AreaPane" && AreaPane.style.display == "none") ToggleAreaPane();
                            if (pane.id == "StatusPane" && StatusPane.style.display == "none") ToggleStatusPane();
                            if (pane.id == "DetailsPane" && DetailsPane.style.display == "none") ToggleDetailsPane();
                            if (pane.id == "ViewConfigPane" && ViewConfigPane.style.display == "none") ToggleViewConfigPane();
                            if (g_SummaryType == SUMMARY_ALARM && pane.id == "AlarmTrendPane" && AlarmTrendPane.style.display == "none") ToggleAlarmTrendPane();
                            if (g_SummaryType == SUMMARY_SYSTEM_ALARM && pane.id == "SystemStatusPane" && SystemStatusPane.style.display == "none") ToggleSystemStatusPane(false, false);
                        }
                        else if ( g_SummaryType == SUMMARY_ALARM && pane.id == "AlarmTrendPane" && GetURLParam("HscFile") != "0" )
                        {
                            ToggleAlarmTrendPane();
                        }
                        if (g_SummaryType == SUMMARY_ALARM && pane.id == "AlarmTrendPane")
                        {
                            g_bAlarmTrendLabelsVisible = layout_bag.getAttribute(pane.id + "_LabelsVisible");
                            g_nAlarmTrendTimePeriod = layout_bag.getAttribute(pane.id + "_TimePeriod");
                        }
                    }
                    layout_bag.setAttribute(PROP_Width, g_lDisplayWidth);
                    layout_bag.setAttribute(PROP_Height, g_lDisplayHeight);
                }
                g_bLayoutPersistenceLoaded = true;
            }
            else
            {
                g_bLayoutPersistenceLoaded = false;
                if( g_SummaryType == SUMMARY_SYSTEM_ALARM && AreaPane.style.display == "none" )
                {
                    ToggleAreaPane();
                }
                if ( g_SummaryType == SUMMARY_ALARM && AlarmTrendPane.style.display == "none" && GetURLParam("HscFile") != "0" )
                {
                    ToggleAlarmTrendPane();
                }
            }
        }
        //make sure that the details pane is never shown on a orphan activity display
        if ( g_SummaryType == SUMMARY_ACTIVITY_ORPHAN ) {
            if (DetailsPane.style.display == "inline")
                ToggleDetailsPane();
        }
        //ISR119942 for EBM summary pags if the details pane is shown show the BOTTOM extended resize pad, otherwise hide
        if((g_SummaryType == SUMMARY_ACTIVITY) || (g_SummaryType == SUMMARY_BATCH) || (g_SummaryType == SUMMARY_PROCEDURE) ) {
            if (DetailsPane.style.display == "none") 
            {
                divExtendedResizepadBOTTOM.style.display="none";
            } 
            else 
            {
                divExtendedResizepadBOTTOM.style.display="inline";
            }
        }
    }
    catch( err )
    {
        HANDLE_ERROR("LoadPersistedLayout", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// LoadPersistedView()
//
//	Checks to make sure the table is fully loaded before loading the persisted view.
// --------------------------------------------------------------------------------------
function LoadPersistedView()
{
    try
    {
        try
        {
            g_TDO = Table.Binding.DataObjects.Item(0);
        }
        catch( err )
        {
            // If we couldn't access the table data object,
            // then we must wait until the page connector is ready.
            BindingBehavior.attachEvent("onpageconnectorready", LoadPersistedView);
            return;
        }
        BindingBehavior.detachEvent("onpageconnectorready", LoadPersistedView);
        // Don't modify the DOM if we're in print mode
        if( window.document.media == "print" )
            return;
        var TableState = "";
        var TDOState = "";
        if (!g_bInDegradedMode && !g_bWebCaptureMode)
        {
            // The cache should have already been loaded
            var view_bag = document.getElementById(PROP_Cache_View);
            var v = view_bag.getAttribute(PROP_Version);
            if( null != v && DISPLAY_VERSION == v )
            {
                TableState = view_bag.getAttribute(PROP_TableState);
                TDOState = view_bag.getAttribute(PROP_TDOState);
                if( TableState == null )
                    TableState = "";
                if( TDOState == null )
                    TDOState = "";
                // load system status dashboard persistence if pane is already open from loadPersistedLayout() and there is no dashboard url argument
                if ((g_SummaryType == SUMMARY_SYSTEM_ALARM) && 
                    (SystemStatusPane.style.display == "inline") &&
                    ((!g_DashboardUrlArgument) || (g_DashboardUrlArgument.length == 0)))
                {
                    var dashboardGroup = view_bag.getAttribute(PROP_DashboardGroup);
                    // if persistence is empty, open the overview dashboard
                    if (!dashboardGroup || dashboardGroup.length == 0)
                        dashboardGroup = g_sOverviewDashboardGroup;
                    var dropdownTDO = document.getElementById("DashboardDropdown").Binding.DataObjects.Item(0);
                    dropdownTDO.SelectionListString = dashboardGroup;
                }
            }
        }
        if (!g_bInDegradedMode)
        {
            if ( g_SummaryType != SUMMARY_EVENTVIEW )
                g_TDO.SetState(TDOState, TableState);
            else
                Table.SetEventViewState(TDOState, TableState);
        }
        g_bViewPersistenceLoaded = true;
    }
    catch( err )
    {
        g_bViewPersistenceLoaded = false;
        HANDLE_ERROR("LoadPersistedView", err.number, err.description);
        g_TDO.SetState("", "");
    }
}
// --------------------------------------------------------------------------------------
// LoadDefaultView()
//
//	Responsible for loading the default table state . Called
//	when the persistence setting is retrieved or when operator changes.
// --------------------------------------------------------------------------------------
function LoadDefaultView()
{
    try
    {
        // Don't modify the DOM if we're in print mode
        if( window.document.media == "print" )
        {
            return;
        }
        ZoomSummary();
        try
        {
            g_TDO = Table.Binding.DataObjects.Item(0);
        }
        catch( err )
        {
            // If we couldn't access the table data object,
            // then we must wait until the page connector is ready.
            BindingBehavior.attachEvent("onpageconnectorready", LoadDefaultView);
            return;
        }
        BindingBehavior.detachEvent("onpageconnectorready", LoadDefaultView);
        if (!g_bInDegradedMode)
        {
            g_TDO.SetState("", "");
            // Open default dashboard on system summary and there is no dashboard url argument
            if ((g_SummaryType == SUMMARY_SYSTEM_ALARM) && ((!g_DashboardUrlArgument) || (g_DashboardUrlArgument.length == 0)))
            {
                if ((SystemStatusPane.style.display == "inline"))
                {
                    SetDefaultDashboard();
                }
                else if (g_statuspane_SelectedEntity != "")
                {
                    // update the faceplate binding only if on system alarm summary and not loading default dashboard
                    DisplayDataRepository.PutValue("PointOnPage", g_statuspane_SelectedEntity);
                }
            }
        }
        g_bViewPersistenceLoaded = false;
    }
    catch( err )
    {
        HANDLE_ERROR("LoadDefaultView", err.number, err.description);
        g_TDO.SetState("", "");
    }
}
// --------------------------------------------------------------------------------------
// LoadDefaultLayout()
//
//	Resets the page layout to default.
//  Called when persistence setting is retrieved or when operator changes.
// --------------------------------------------------------------------------------------
function LoadDefaultLayout()
{
    var bAreaPaneVisible = AreaPaneVisible();
    //Ensure the area pane is closed for all summary displays except for the
    //system summary display where we ensure it is open
    if ((bAreaPaneVisible && g_SummaryType != SUMMARY_SYSTEM_ALARM) ||
        (!bAreaPaneVisible && g_SummaryType == SUMMARY_SYSTEM_ALARM))
        ToggleAreaPane();
    if (StatusPane.style.display == "inline")
        ToggleStatusPane();
    if (ViewConfigPane.style.display == "inline")
        ToggleViewConfigPane();
    if (DetailsPane.style.display == "inline")
        ToggleDetailsPane();
    if (g_SummaryType == SUMMARY_ALARM && AlarmTrendPane.style.display != "none")
        ToggleAlarmTrendPane();
    if (g_SummaryType == SUMMARY_SYSTEM_ALARM && SystemStatusPane.style.display != "none")
        ToggleSystemStatusPane(true, false);
    g_bLayoutPersistenceLoaded = false;
}
// --------------------------------------------------------------------------------------
// OnSavePersistance()
//
//	Bound to the OnSavePersistance event from the TDO, allowing us to persist page
//  layout and views under multiwindow.
// --------------------------------------------------------------------------------------
function OnSavePersistance(TDOState, bOnQuit)
{
    var bSaveViewPersistence = !g_bAlreadyPersistedViewOnQuit;
    var bSaveLayoutPersistence = !g_bAlreadyPersistedLayoutOnQuit;
    if (bOnQuit === undefined || bOnQuit == null)
    {
        bOnQuit = true;
    }
    if (g_bAlreadyPersistedViewOnQuit && g_bAlreadyPersistedLayoutOnQuit && bOnQuit)
    {
        return;
    }
    if(g_TDO.ViewPersistenceEnabled == false && g_SummaryType != SUMMARY_EVENTVIEW)
    {
        DeleteViewPersistence();
        bSaveViewPersistence = false;
    }
    if(g_TDO.LayoutPersistenceEnabled == false && g_SummaryType != SUMMARY_EVENTVIEW)
    {
        DeleteLayoutPersistence();
        bSaveLayoutPersistence = false;
    }
    try
    {
        if (g_bPageInitialized && !g_bInDegradedMode && !g_bWebCaptureMode)
        {
            // Don't modify the DOM if we're in print mode
            if( window.document.media == "print" )
                return;
            if (bSaveLayoutPersistence)
            {
                if( String(btnDropAreaPane.selected) == "true" )
                {
                    btnDropAreaPane.click();
                }
                var layout_bag = document.getElementById(PROP_Cache_Layout);
                // Save the layout of the panes
                var i = 0;
                for( i=0; i<g_arrPanesToPersist.length; i++ )
                {
                    var currPane = document.getElementById(g_arrPanesToPersist[i]);
                    if (currPane ==  null)
                        continue;
                    var p = new Pane( currPane );
                    p.Save(layout_bag);
                    if (p.name == "StatusPane" && g_SummaryType == SUMMARY_SYSTEM_ALARM)
                    {
                        layout_bag.setAttribute(p.name + "_SelectedEntity", g_statuspane_SelectedEntity);
                        layout_bag.setAttribute(p.name + "_FaceplateName",  g_statuspane_FaceplateName);
                    }
                    if (p.name == "AlarmTrendPane" && g_SummaryType == SUMMARY_ALARM)
                    {
                        var AlarmTrend = document.getElementById('AlarmTrend');
                        if (AlarmTrend)
                        {
                            layout_bag.setAttribute(p.name + "_LabelsVisible", AlarmTrend.Content.EATView.LabelsVisible);
                            layout_bag.setAttribute(p.name + "_TimePeriod", AlarmTrend.Content.EATView.TotalDuration);
                        }
                    }
                }
                for( i=0; i<g_arrButtonsToPersist.length; i++ )
                {
                    var btn = document.getElementById(g_arrButtonsToPersist[i]);
                    if (btn ==  null)
                        continue;
                    layout_bag.setAttribute(g_arrButtonsToPersist[i] + "_" + PROP_Selected, btn.selected);
                    layout_bag.setAttribute(g_arrButtonsToPersist[i] + "_" + PROP_Title, btn.title);
                }
                layout_bag.setAttribute(PROP_Width, g_lDisplayWidth);
                layout_bag.setAttribute(PROP_Height, g_lDisplayHeight);
                layout_bag.setAttribute(PROP_Version, DISPLAY_VERSION);
                layout_bag.save(g_strLayoutCacheName);
                if (bOnQuit)
                    g_bAlreadyPersistedLayoutOnQuit = true;
            }
            if (bSaveViewPersistence)
            {
                var view_bag = document.getElementById(PROP_Cache_View);
                if (TDOState === undefined || TDOState == null)
                {
                    TDOState = g_TDO.GetState();
                }
                if( typeof(Table.TableReadyState) == "string" && Table.TableReadyState == "ready" )
                {
                    view_bag.setAttribute(PROP_TableState, Table.GetState());
                    view_bag.setAttribute(PROP_TDOState, TDOState);
                    view_bag.setAttribute(PROP_Version, DISPLAY_VERSION);
                }
                view_bag.save(g_strViewCacheName);
                 if (bOnQuit)
                    g_bAlreadyPersistedViewOnQuit = true;
            }
        }
    }
    catch( err )
    {
        g_bAlreadyPersistedViewOnQuit = false;
        g_bAlreadyPersistedLayoutOnQuit = false;
        HANDLE_ERROR("OnSavePersistance", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// OnPersist_SAVE()
//
//	Responsible for persisting the current page layout and view. Called when the page is
//	unloaded.
// --------------------------------------------------------------------------------------
function OnPersist_SAVE(bOnQuit)
{
	OnSavePersistance( null, bOnQuit );
}
// --------------------------------------------------------------------------------------
// OnPageUnload()
//
//	Called when the page is unloaded
// --------------------------------------------------------------------------------------
function OnPageUnload()
{
    try
    {
        if (g_SummaryType == SUMMARY_SYSTEM_ALARM)
            ClearFaceplate();
        if(!g_PersistanceSaveDisabled)
        {
            OnPersist_SAVE(true);
        }
        if( g_oEventSink != null )
        {
            // Don't _need_ to call this, but it's here for good measure
            g_oEventSink.Unadvise();
        }
        if (g_bInDegradedMode && window != null)
        {
            // Clear message zone
            window.external.application.MessageZoneText = "";
        }
    }
    catch( err )
    {
        HANDLE_ERROR("OnPageUnload", err.number, err.description);
    }
	g_oEventSink = null;
    TerminateLogger();
}
// --------------------------------------------------------------------------------------
// DeleteViewPersistence()
//
//	Deletes the view persistence data by setting the expiry date/time to now
// --------------------------------------------------------------------------------------
function DeleteViewPersistence()
{
    try
    {
        var view_bag = document.getElementById(PROP_Cache_View);
        if(view_bag != null)
        {
            var dtNow = new Date(); // Current date and time
            var szExpirationDateTime = dtNow.toUTCString();
            view_bag.expires = szExpirationDateTime;
            view_bag.save(g_strViewCacheName);
        }
    }
    catch( err )
    {
        HANDLE_ERROR("DeleteViewPersistence", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// DeleteLayoutPersistence()
//
//	Deletes the layout persistence data by setting the expiry date/time to now
// --------------------------------------------------------------------------------------
function DeleteLayoutPersistence()
{
    try
    {
        var layout_bag = document.getElementById(PROP_Cache_Layout);
        if(layout_bag != null)
        {
            var dtNow = new Date(); // Current date and time
            var szExpirationDateTime = dtNow.toUTCString();
            layout_bag.expires = szExpirationDateTime;
            layout_bag.save(g_strLayoutCacheName);
        }
    }
    catch( err )
    {
        HANDLE_ERROR("DeleteLayoutPersistence", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// OnOperatorChange()
//
//	Informs the page that the operator has changed. This enables us to update the
//	page configuration.
// --------------------------------------------------------------------------------------
function OnOperatorChange()
{
    try
    {
        if (g_bInDegradedMode)
        {
            RegenerateErrorMsg();
        }
        if ((g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_EVENTVIEW)) {
            g_TDO.OperatorChange();
        }
        if (g_SummaryType == SUMMARY_SYSTEM_ALARM)
        {
            // clear dashboard navigation history
            g_DashboardHistoryArray = new Array();
        }
        if (!g_PersistanceSaveDisabled )
        {
            OnPersist_SAVE(false);
        }
        //Only need to reload if persistence is enabled as the table's state will have to
        //be changed for the new user
        LoadPersistenceCaches();
        if (g_SummaryType == SUMMARY_EVENTVIEW || g_TDO.LayoutPersistenceEnabled)
            LoadPersistedLayout();
        if (g_SummaryType == SUMMARY_EVENTVIEW || g_TDO.ViewPersistenceEnabled)
            LoadPersistedView();
        else if ((g_SummaryType == SUMMARY_SYSTEM_ALARM) && (SystemStatusPane.style.display == "inline"))
        {
            SetDefaultDashboard();
        }
        // Enable/disable the tracker configuration link based off security level
        var AlarmTrend = document.getElementById('AlarmTrend');
        if (g_SummaryType == SUMMARY_ALARM && AlarmTrend)
        {
            if (window.external.Parent.RuntimeStatus.SecurityLevel >= 4 &&
                AlarmTrend.Content.EATView.ConfigID != 0)
            {
                lnkTrackerConfig.disabled = false;
                lnkTrackerConfig.onclick = null;
            }
            else
            {
                lnkTrackerConfig.disabled = true;
                lnkTrackerConfig.onclick = function() { return false; };
            }
        }
    }
    catch( err )
    {
        HANDLE_ERROR("OnOperatorChange", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// OnPersistenceSettingRetrieved()
//
//	Fired by the TDO when it first retrieves the persistence setting from the server.
//	Used to check if persistence has been incorrectly loaded.
// --------------------------------------------------------------------------------------
function OnPersistenceSettingRetrieved(bEnableViewPersistence, bEnableLayoutPersistence, bUsingConfiguredDefaultView)
{
    g_bUsingConfiguredDefaultView = bUsingConfiguredDefaultView;
    if(bEnableLayoutPersistence == false && g_bLayoutPersistenceLoaded == true)
    {
        LoadDefaultLayout();
        DeleteLayoutPersistence();
    }
    if(bEnableViewPersistence == false && g_bViewPersistenceLoaded == true)
    {
        LoadDefaultView();
        DeleteViewPersistence();
    }
    else
    {
        if( g_SummaryType == SUMMARY_SYSTEM_ALARM )
        {
            // update the faceplate binding only if on system alarm summary and view persistence is enabled.
            if (g_statuspane_SelectedEntity != "")
            {
                DisplayDataRepository.PutValue("PointOnPage", g_statuspane_SelectedEntity);
            }
        }
    }
    // now select the dashboard if specified in url
    try
    {
        if( (g_SummaryType == SUMMARY_SYSTEM_ALARM) && g_DashboardUrlArgument )
        {
            if (SystemStatusPane.style.display == "none" )
                ToggleSystemStatusPane(true, true);
            var dropdownTDO = document.getElementById("DashboardDropdown").Binding.DataObjects.Item(0);
            dropdownTDO.SelectionListString = g_DashboardUrlArgument;
        }
    }
    catch( err )
    {
        HANDLE_ERROR("OnPersistenceSettingRetrieved", err.number, "Loading url defined dashboard: " + err.description);
    }
    // Page is considered fully loaded once persistence setting is retrieved. Ok for faceplate to start updating.
    setTimeout(function(){
        g_bPageReady = true;
        if(g_SummaryType == SUMMARY_SYSTEM_ALARM)
            UpdateFaceplate();
    }, 500);
}
// --------------------------------------------------------------------------------------
// OnTimeLocaleSettingChange()
//
//	Fired by the TDO when it first retrieves the time locale setting from the server.
// --------------------------------------------------------------------------------------
function OnTimeLocaleSettingChange(bEnableTimeLocale)
{
    if(bEnableTimeLocale)
    {
        g_dateTimeFormat |= SUPPORT_AM_PM;
        g_dateTimeFormatTip |= SUPPORT_AM_PM;
    }
    else
    {
        g_dateTimeFormat &= ~SUPPORT_AM_PM;
        g_dateTimeFormatTip &= ~SUPPORT_AM_PM;
    }
}
// --------------------------------------------------------------------------------------
// OnToolTipSettingChange()
//
//	Fired by the TDO when it first retrieves the tool tip setting from the server.
// --------------------------------------------------------------------------------------
function OnToolTipSettingChange(bEnableToolTip)
{
    if(bEnableToolTip)
    {
        g_bDisplayToolTips = true;
    }
    else
    {
        g_bDisplayToolTips = false;
    }
}
// --------------------------------------------------------------------------------------
// OnFontSizeChanged()
//
//	Fired by the TDO when it first retrieves the large font setting from the server.
// --------------------------------------------------------------------------------------
function OnFontSizeChanged(lFontSizeType)
{
    if (g_SummaryType != SUMMARY_EVENTVIEW)
    {
        if (lFontSizeType == 2) // large font size
        {
            Table.rowHeight=g_LargeFontRowHeight;
            Table.style.fontSize=18+"pt";
			Table.className = ClassListRemove(Table.className, "medium-font-size");
			Table.className = ClassListRemove(Table.className, "small-font-size");
			Table.className = ClassListAdd(Table.className, "large-font-size");
		}
        else if (lFontSizeType == 1) // medium font size
        {
            Table.rowHeight=g_MediumFontRowHeight;
            Table.style.fontSize=14+"pt";
			Table.className = ClassListRemove(Table.className, "large-font-size");
			Table.className = ClassListRemove(Table.className, "small-font-size");
			Table.className = ClassListAdd(Table.className, "medium-font-size");
        }
        else //default (or lFontSizeType == 0) // small font size
        {
            Table.rowHeight=g_SmallFontRowHeight;
            Table.style.fontSize=10+"pt";
			Table.className = ClassListRemove(Table.className, "medium-font-size");
			Table.className = ClassListRemove(Table.className, "large-font-size");
			Table.className = ClassListAdd(Table.className, "small-font-size");
        }
    }
}
// --------------------------------------------------------------------------------------
// ClassListAdd()
// 
// Helper function to append a CSS class onto a classList.
// --------------------------------------------------------------------------------------
function ClassListAdd(OriginalClassList, newClass)
{
    if (OriginalClassList == "")
    {
        return newClass;
    }
	var retValue = OriginalClassList.concat(" ");
	retValue = retValue.concat(newClass);
	return retValue;
}
// --------------------------------------------------------------------------------------
// ClassListRemove()
// 
// Helper function to remove a CSS class from a classList. Returns the original list
// if the class to remove is not found in it.
// --------------------------------------------------------------------------------------
function ClassListRemove(OriginalClassList, RemoveClass)
{
	if (OriginalClassList.indexOf(RemoveClass) != -1)
	{
		var newClassList = OriginalClassList.replace(RemoveClass, '');
		return newClassList;
	}
	return OriginalClassList;
}
// --------------------------------------------------------------------------------------
// OnAlarmTrendDisabledChange()
//
//	Fired by the TDO when it first retrieves the time locale setting from the server.
// --------------------------------------------------------------------------------------
function OnAlarmTrendDisabledChange(bAlarmTrendDisabled)
{
    if (bAlarmTrendDisabled)
    {
        if (AlarmTrendPane.style.display != "none")
            btnAlarmTrendPane.click();
        btnAlarmTrendPane.className = "toolBarBtnDisabled alarmTrendDisabled";
        btnAlarmTrendPane.disabled = true;
        btnAlarmTrendPane.selected = false;
    }
    else
    {
        btnAlarmTrendPane.className = "toolBarBtn alarmTrendEnabled";
        btnAlarmTrendPane.disabled = false;
        if (AlarmTrendPane.style.display != "none")
            btnAlarmTrendPane.selected = true;
    }
}
// --------------------------------------------------------------------------------------
// OnNotifyErrorStatus()
//
//	Bound to the OnNotifyErrorStatus event from the TDO, allowing us to handle any errors
//  as appropriate.
// --------------------------------------------------------------------------------------
function OnNotifyErrorStatus(code, msg)
{
    if( code == ERR_SAVEVIEW_SUCCESS )
    {
        // When a view has been saved successfully, we need to close the view configuration pane
        ToggleViewConfigPane();
    }
    else if( code == ERR_SAVEVIEW_ALREADYEXISTS )
    {
        // This error occurs when the user tries to save a view with the same
        // name as an existing view. We need to prompt the user to see if
        // they want to overwrite the existing view.
        var strMessage = "A view with the same name already exists. Do you want to overwrite it?";
        ConfirmMsg(strMessage, "SaveView", txtViewName.value, chkAssignArea.checked ? txtAreaName.value : "", Table.GetState(), chkIncludeArea.checked, true);
    }
    else if( code == ERR_LCS_NOSELECTION )
    {
        // This error occurs when the user tries to apply a (like currently selected)
        // filter when there is nothing selected.
        window.external.application.TemporaryMessageZoneText = "The filter cannot be applied because there is nothing selected";
    }
    else if( code == ERR_VIEW_DEGRADEDMODE )
    {
        // This error applies only to the event/soe summary. This error occurs when
        // the summary page is hosted in the Direct Station and the Station goes into
        // degraded mode (Cluster Server is unavaible to the Direct Station
        // e.g. connection lost).
        if (!g_bInDegradedMode && ((g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_SOE) || (g_SummaryType == SUMMARY_EVENTVIEW)))
        {
            g_bInDegradedMode = true;
            if ((g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_EVENTVIEW))
            {
                g_strNotifyErrorMsg = g_strEventInDegradedMode;
            }
            else
            {
                g_strNotifyErrorMsg = g_strSOEInDegradedMode;
            }
            window.external.application.MessageZoneText = g_strNotifyErrorMsg;
            // Use the transparent layer to cover the whole table
            PageDisabledCover.style.display = "inline";
            // Use the black layer to cover the grid
            tableDisabledCover.style.display = "inline";
            // Disable all buttons
            btnClearFilter.disabled = true;
            btnClearFilter.className='toolBarBtnDisabled FilterDisabled';
            btnResetView.disabled = true;
            btnResetView.className='toolBarBtnDisabled resetViewDisabled';
            btnViewConfigPane.selected = false;
            btnViewConfigPane.disabled = true;
            btnViewConfigPane.className='toolBarBtnDisabled viewConfigPaneDisabled';
            btnViewConfigPane.title = "Show view configuration pane ";
            btnAreaPane.selected = false;
            btnAreaPane.disabled = true;
            btnAreaPane.className='toolBarBtnDisabled areaPaneDisabled';
            btnAreaPane.title = "Show location pane";
            btnDetailsPane.selected = false;
            btnDetailsPane.disabled = true;
            btnDetailsPane.className='toolBarBtnDisabled detailsPaneDisabled';
            btnDetailsPane.title = "Show details pane";
            btnColumnOrganizer.disabled = true;
            btnColumnOrganizer.className='toolBarBtnDisabled columnOrganizerDisabled';
            btnPrintAsReport.disabled = true;
            btnPrintAsReport.className='toolBarBtnDisabled printAsReportDisabled';
            btnPrintPreview.disabled = true;
            btnPrintPreview.className='toolBarBtnDisabled printPreviewDisabled';
            btnCsvExport.disabled = true;
            btnCsvExport.className='toolBarBtnDisabled csvExportDisabled';
            // Clear area and view drop down list text
            btnDropAreaPane.style.display = "none";
            spnDropAreaPane.style.display = "none";
            btnDropViewList.style.display = "none";
            // Clear the 'Filter Applied' text indicator
            lblFilterApplied.style.display = "none";
            // Set matching events count to zero
            divEventCount.innerText = 0;
            if ((g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_EVENTVIEW))
            {
                // For event summary only
                // Disable all buttons
                btnPause.readonly = true;
                btnPause.className='pauseDisabled';
                btnResume.readonly = true;
                btnResume.className='resumeDisabled';
                btnGenerateEvent.disabled = true;
                btnGenerateEvent.className='generateEventDisabled';
                // Clear the 'Paused' text indicator
                divPaused.style.display = 'none';
                // Hide progress bar
                spnProgress.style.display = "none";
            }
        }
    }
    else if( code == ERR_VIEW_REFRESHNOW )
    {
        // This error applies only to the event/soe summary. This error occurs when
        // the summary page is hosted in the Direct Station and the Station gets out of
        // degraded mode (Cluster Server is once again avaible to the Direct Station
        // e.g. connection re-establish).
        if (!g_bRefreshing && ((g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_SOE) || (g_SummaryType == SUMMARY_EVENTVIEW)))
        {
            g_bRefreshing = true;
            window.external.application.refresh();
        }
    }
    else if( code == ERR_COMMENT_UNAVAILABLE )
    {
        // This error applies only to the alarm/system alarm/alert summary. This error occurs
        // when the summary page is hosted in the Direct Station and the Station goes into
        // degraded mode (Cluster Server is unavaible to the Direct Station
        // e.g. connection lost). This error represents the comment functionality is not available.
        if ((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT))
        {
            g_bCommentUnavaiable = true;
            ExistComments.innerText = "";
            AddComments.disabled = false;
            AddComments.contentEditable = false;
            AddComments.value = "(Comments can not be added when the server is unavailable)";
            AddComments.style.cursor = "default";
            btnSaveDetails.readonly = true;
            btnSaveDetails.className = 'saveDetailsDisabled';
        }
    }
    else if( code == ERR_COMMENT_REFRESH )
    {
        // This error applies only to the alarm/system alarm/alert summary. This error occurs
        // when the summary page is hosted in the Direct Station and the Station gets out of
        // degraded mode (Cluster Server is once again avaible to the Direct Station
        // e.g. connection re-establish). This error will force the refresh of the comment pane.
        if ((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT))
        {
            g_bCommentUnavaiable = false;
            RequestComments();
        }
    }
    else if( code == ERR_COMMENT_PRE800_DSAALARM )
    {
        // This error represents the comment functionality is not available for DSA alarm/event
        // from pre 800 system.
        ExistComments.innerText = "";
        AddComments.disabled = false;
        AddComments.contentEditable = false;
        AddComments.style.cursor = "default";
        if ((g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_EVENTVIEW))
            AddComments.value = "(Comments can not be added for this event)";
        else
            AddComments.value = "(Comments can not be added for this alarm)";
        btnSaveDetails.readonly = true;
        btnSaveDetails.className = 'saveDetailsDisabled';
    }
    else if ( code == ERR_ALARM_OVERFLOW )
    {
        if (btnAlarmOverflowAck.style.visibility != "visible")
        {
            btnAlarmOverflowAck.disabled = false;
            btnAlarmOverflowAck.style.visibility = "visible";
        }
    }
    else if ( code == ERR_NO_ALARM_OVERFLOW )
    {
        btnAlarmOverflowAck.disabled = false;
        btnAlarmOverflowAck.style.visibility = "hidden";
    }
    else
    {
        // For all other errors, simply display the error message in the message zone
        if( msg.length > 0 )
            window.external.application.TemporaryMessageZoneText = msg;
    }
}
function RegenerateErrorMsg()
{
    window.external.application.MessageZoneText = g_strNotifyErrorMsg;
}
// --------------------------------------------------------------------------------------
// OnSecuritySettingChange()
//
//	Bound to the OnSecuritySettingChange event from the TDO, allowing us enable/disable
//  the appropriate functionality on the page. Note that some of the functionality
//  (eg the view config pane, page ack, etc) is enabled/disabled via property bindings.
// --------------------------------------------------------------------------------------
function OnSecuritySettingChange(
    bULEnabled,
    bSortingAndFilteringEnabled,
    bColumnOrganizingEnabled,
    bSavingViewsEnabled,
    bPauseResumeEnabled,
    bPageAcknowledgeEnabled)
{
    Table.ColumnResizing = bColumnOrganizingEnabled;
    Table.ColumnMoving = bColumnOrganizingEnabled;
    if ((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_MESSAGE) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT))
    {
        if (bULEnabled)
        {
            // If UL licensed, then the Page Acknowledge button will be hidden.
            btnAckPage.style.visibility = "hidden";
            if (g_SummaryType == SUMMARY_MESSAGE)
                btnClearPage.style.visibility = "hidden";
        }
        else
        {
            // If UL is not licensed, then the Page Acknowledge button is always visible
            // but it will get grey out if bPageAcknowledgeEnabled is set to false.
            btnAckPage.style.visibility = "visible";
            if (bPageAcknowledgeEnabled)
            {
                btnAckPage.readonly = false;
                btnAckPage.className='AcknowledgePageEnabled';
                if (g_SummaryType == SUMMARY_MESSAGE)
                {
                    btnClearPage.disabled = false;
                    btnClearPage.className = 'clearPageEnabled';
                }
            }
            else
            {
                btnAckPage.readonly = true;
                btnAckPage.className='AcknowledgePageDisabled';
                if (g_SummaryType == SUMMARY_MESSAGE)
                {
                    btnClearPage.disabled = true;
                    btnClearPage.className = 'clearPageDisabled';
                }
            }
            var AlarmTrend = document.getElementById('AlarmTrend');
            if (AlarmTrend)
            {
                // try...catch around this because it can happen before EAT loads
                try
                {
                    AlarmTrend.Content.EATView.AckEnabled = bPageAcknowledgeEnabled;
                }
                catch (err)
                {
                }
            }
        }
    }
    if (bColumnOrganizingEnabled)
    {
        btnColumnOrganizer.disabled = false;
        btnColumnOrganizer.className='toolBarBtn columnOrganizerEnabled';
    }
    else
    {
        btnColumnOrganizer.disabled = true;
        btnColumnOrganizer.className='toolBarBtnDisabled columnOrganizerDisabled';
    }
    if (bSortingAndFilteringEnabled && g_SummaryType != SUMMARY_EVENTVIEW)
    {
        btnClearFilter.disabled = false;
        btnClearFilter.className='toolBarBtn ClearFilter';
        btnResetView.disabled = false;
        btnResetView.className='toolBarBtn resetViewClass';
    }
    else
    {
        btnClearFilter.disabled = true;
        btnClearFilter.className='toolBarBtnDisabled FilterDisabled';
        btnResetView.disabled = true;
        btnResetView.className='toolBarBtnDisabled resetViewDisabled';
    }
    if (bSavingViewsEnabled)
    {
        g_bSaveViewsEnabled = true;
        ViewConfigBody.disabled = false;
        btnSaveView.readonly = false;
        btnSaveView.className='saveViewBtnEnabled';
        CheckViewDelete(false);
    }
    else
    {
        g_bSaveViewsEnabled = false;
        ViewConfigBody.disabled = true;
        btnSaveView.readonly = true;
        btnDeleteView.readonly = true;
        btnSaveView.className='saveViewBtnDisabled';
        btnDeleteView.className='deleteViewDisabled';
    }
}
// --------------------------------------------------------------------------------------
function CheckViewDelete(disabled)
{
    // Surround the operation with try/catch block because this function is part of
    // OnSecuritySettingChange callback which could cause timer entrant problem in
    // a localized system using a foreign language input method (ISR113468).
    //     "It's illegal to call out while inside message filter"
    //     http://support.microsoft.com/kb/176399
    try
    {
        if ( String(disabled) == "true" || spnCurrentView.innerText == g_TDO.UntitledViewName || spnCurrentView.innerText == g_TDO.UntitledViewName + " *" )
        {
            btnDeleteView.readonly = true;
            btnDeleteView.className='deleteViewDisabled';
        }
        else
        {
            btnDeleteView.readonly = false;
            btnDeleteView.className='deleteViewEnabled';
        }
    }
    catch (err) {}
}
// --------------------------------------------------------------------------------------
function SetPaneContainerSize()
{
    var PaneContainerHeight = 0;
    var PaneContainerWidth = 0;
    // Get the Pane Container height and width
    PaneContainerHeight = Page.style.pixelHeight - PaneContainer.paddingTop - PaneContainer.paddingBottom;
    PaneContainerWidth = Page.style.pixelWidth - PaneContainer.paddingLeft - PaneContainer.paddingRight;
    // Adjust the height of the Pane Container so that the last row will not be obscured
    PaneContainerHeight = PaneContainerHeight - ((PaneContainerHeight - Table.headerHeight - 2) % Table.rowHeight);
    // Set the Pane Container's height, width, top and left properties
    if ( PaneContainer.style.pixelHeight != PaneContainerHeight )
        PaneContainer.style.pixelHeight = PaneContainerHeight;
    if ( PaneContainer.style.pixelWidth != PaneContainerWidth )
        PaneContainer.style.pixelWidth = PaneContainerWidth;
    if ( PaneContainer.style.pixelTop != PaneContainer.paddingTop )
        PaneContainer.style.pixelTop = PaneContainer.paddingTop;
    if ( PaneContainer.style.pixelLeft != PaneContainer.paddingLeft )
        PaneContainer.style.pixelLeft = PaneContainer.paddingLeft;
    // Use Pane Container's height to set the height of the area pane
    var alarmTrendHeight = 0;
    if (g_SummaryType == SUMMARY_ALARM && AlarmTrendPane.style.display != "none")
    {
        alarmTrendHeight = AlarmTrendPane.style.pixelHeight + HPadAlarmTrend.style.pixelHeight;
    }
    AreaPane.style.pixelHeight = PaneContainerHeight - alarmTrendHeight;
    if (g_SummaryType == SUMMARY_SYSTEM_ALARM)
    {
        StatusPane.style.pixelHeight = PaneContainerHeight;
        StatusPane.style.pixelWidth = 165;
    }
    if (g_SummaryType == SUMMARY_ALARM)
    {
        AdjustAlarmTrendPane();
    }
    AreaPaneDropBorder.style.pixelHeight = PaneContainerHeight;
    VerticalResizePad.style.pixelHeight = PaneContainerHeight;
    // Since we're now setting InnerPaneContainer manually too, make sure it's set right
    SetVerticalPanePositions(AreaPaneVisible(), StatusPaneVisible());
}
// --------------------------------------------------------------------------------------
function CheckGridSize()
{
    var systemStatusPaneHeight = 0;
    if ( (g_SummaryType == SUMMARY_SYSTEM_ALARM) && (SystemStatusPane.style.visibility == "visible") )
        systemStatusPaneHeight = parseInt(SystemStatusPane.currentStyle.height);
    if (GridPane.style.pixelHeight > (50 * Table.rowHeight + Table.headerHeight))
        GridPane.style.pixelHeight = InnerPaneContainer.style.pixelHeight
            - (DetailsPane.style.visibility == "visible" ? DetailsPane.style.pixelHeight : 0)
            - (ViewConfigPane.style.visibility == "visible" ? ViewConfigPane.style.pixelHeight : 0)
            - systemStatusPaneHeight;
}
// --------------------------------------------------------------------------------------
function GetNumberOfRows()
{
    return 2 * Table.rowHeight + Table.headerHeight;
}
// --------------------------------------------------------------------------------------
function SetTableSize()
{
    // Don't call resizing if not necessary
    if (Table.style.pixelHeight == GridPane.style.pixelHeight - 2 &&
            Table.style.pixelWidth == GridPane.style.pixelWidth - 2 &&
            Table.style.pixelTop == GridPane.style.pixelTop + PaneContainer.style.pixelTop + 1)
        return;
    // Make sure the outer containers are set to the right size...
    SetPaneContainerSize();
    // ...we haven't over done our bounds...
    var lGridPaneBottom = GridPane.style.pixelTop + GridPane.style.pixelHeight;
    var lInnerPaneBottom = InnerPaneContainer.style.pixelTop + InnerPaneContainer.style.pixelHeight;
    if ( lGridPaneBottom > lInnerPaneBottom )
        GridPane.style.pixelHeight -= lGridPaneBottom - lInnerPaneBottom;
    else if (lGridPaneBottom < lInnerPaneBottom  && DetailsPane.style.display == "none")
        GridPane.style.pixelHeight += lInnerPaneBottom - lGridPaneBottom;
    var headerHeight = Table.headerHeight;
    var rowHeight = Table.rowHeight;
    // ...and then make sure that we're not too small or not too big, and that the details pane is alright
    var lRemainder = (GridPane.style.pixelHeight - headerHeight) % rowHeight;
    if (GridPane.style.pixelHeight < GetNumberOfRows())
    {
        GridPane.style.pixelHeight = GetNumberOfRows();
    }
    else if (lRemainder != 0)
    {
        if (lRemainder > rowHeight / 2)
            GridPane.style.pixelHeight += rowHeight - lRemainder;
        else
            GridPane.style.pixelHeight -= lRemainder;
    }
    if (DetailsPane.style.display == "inline")
        Handle_PaneResize(HorizontalResizePad_BOTTOM, GridPane.style.pixelTop + GridPane.style.pixelHeight + g_lPanePadding + g_lGridHeightAdjustment);
    // Now do our stuff
    CheckGridSize();
    //to make room for activity command buttons
    Table.style.pixelHeight = GridPane.style.pixelHeight - 2;
    if (g_SummaryType == SUMMARY_ACTIVITY || g_SummaryType == SUMMARY_BATCH || g_SummaryType == SUMMARY_PROCEDURE) {
        Table.style.pixelHeight -= divActivityCommands.clientHeight;
    }
    if(g_OnTouchPanel)
    { 
        Table.style.pixelHeight -= 10;
    }
    Table.style.pixelWidth = GridPane.style.pixelWidth - 2;
    Table.style.pixelTop = GridPane.style.pixelTop + PaneContainer.style.pixelTop + 1;
    var heightOffset = (g_SummaryType == SUMMARY_MESSAGE) ? 650 : 540;
    var tempHeight = g_lDisplayHeight - heightOffset;
    if ( tempHeight > GridPane.style.pixelTop + GetNumberOfRows() )
        HorizontalResizePad_BOTTOM.min = g_lDisplayHeight - heightOffset;
    else
        HorizontalResizePad_BOTTOM.min = GridPane.style.pixelTop + GetNumberOfRows();
    if (g_SummaryType == SUMMARY_ACTIVITY || g_SummaryType == SUMMARY_BATCH || g_SummaryType == SUMMARY_PROCEDURE) {
        HorizontalResizePad_BOTTOM.min = headerHeight + rowHeight + divActivityCommands.clientHeight;
    }
    if (HorizontalResizePad_BOTTOM.max < HorizontalResizePad_BOTTOM.min)
        HorizontalResizePad_BOTTOM.max = HorizontalResizePad_BOTTOM.min;
}
// --------------------------------------------------------------------------------------
function SetToolbar()
{
    if( window.event.propertyName == "btnPrintAsReportDisabled" )
    {
        if( toolbar.btnPrintAsReportDisabled )
        {
            btnPrintAsReport.disabled = true;
            btnPrintAsReport.className ='toolBarBtnDisabled printAsReportDisabled';
        }
        else
        {
            btnPrintAsReport.disabled = false;
            btnPrintAsReport.className ='toolBarBtn printAsReportEnabled';
        }
    }
    if( window.event.propertyName == "btnPrintPreviewDisabled" )
    {
        if( toolbar.btnPrintPreviewDisabled )
        {
            btnPrintPreview.disabled = true;
            btnPrintPreview.className ='toolBarBtnDisabled printPreviewDisabled';
        }
        else
        {
            btnPrintPreview.disabled = false;
            btnPrintPreview.className ='toolBarBtn printPreviewEnabled';
        }
    }
    if( window.event.propertyName == "btnCsvExportDisabled" )
    {
        if( toolbar.btnCsvExportDisabled )
        {
            btnCsvExport.disabled = true;
            btnCsvExport.className ='toolBarBtnDisabled csvExportDisabled';
        }
        else
        {
            btnCsvExport.disabled = false;
            btnCsvExport.className ='toolBarBtn csvExportEnabled';
        }
    }
}
// --------------------------------------------------------------------------------------
function SetPauseResumeDisable()
{
    if( window.event.propertyName == "btnPauseResumeDisabled" )
    {
        if ((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_MESSAGE) || (g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT) || (g_SummaryType == SUMMARY_EVENTVIEW))
        {
            if ( btnPauseResume.btnPauseResumeDisabled )
            {
                if (btnPause.readonly && !btnResume.readonly)
                {
                    // If the pause button is disabled and the resume button is enabled,
                    // then the summary must be paused. We need to resume it before disabling pause.
                    btnResume.click();
                }
                btnPause.readonly = true;
                btnResume.readonly = true;
                btnPause.className='pauseDisabled';
                btnResume.className='resumeDisabled';
            }
            else
            {
                // reset to enable pause/resume only if pause/resume is disabled before.
                if (btnPause.readonly && btnResume.readonly)
                {
                    btnPause.readonly = false;
                    btnResume.readonly = true;
                    btnPause.className='pauseEnabled';
                    btnResume.className='resumeDisabled';
                }
            }
        }
    }
}
// --------------------------------------------------------------------------------------
function SetClearPageState() {
    if (window.event.propertyName == "pageClearDisabled")
    {
        if (g_SummaryType == SUMMARY_MESSAGE)
        {
            if (btnClearPage.pageClearDisabled)
            {
                btnClearPage.disabled = true;
                btnClearPage.className = 'clearPageDisabled';
            }
            else
            {
                btnClearPage.disabled = false;
                btnClearPage.className = 'clearPageEnabled';
            }
        }
    }
}
// --------------------------------------------------------------------------------------
function OnResetPauseResume()
{
    if ((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_MESSAGE) || (g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT) || (g_SummaryType == SUMMARY_EVENTVIEW))
    {
        if ( !(btnPause.readonly && btnResume.readonly) )
        {
            divPaused.style.display = 'none';
            btnPause.readonly = false;
            btnResume.readonly = true;
            btnPause.className='pauseEnabled';
            btnResume.className='resumeDisabled';
        }
    }
}
// --------------------------------------------------------------------------------------
function TableComponentFromElement(tbl, elem)
{
    var b = window.document.body;
    var top = null;
    var e = null;
    if( tbl == null || elem == null )
        return TC_ERROR;
    top = tbl.parentElement;
    e   = elem;
    if( top.contains(e) )
    {
        while( e != top )
        {
            if( e == tbl )
                return TC_TABLE;
            if( e.name == NAME_SCROLLBAR )
                return TC_SCROLLBAR;
            if( e.name == NAME_THEAD )
                return TC_THEAD;
            if( e.name == NAME_TBODY )
                return TC_TBODY;
            if( e.name == NAME_MENU )
                return TC_MENU;
            e = e.parentElement;
        }
    }
    return TC_ERROR;
}
// --------------------------------------------------------------------------------------
// Pane manipulation routines
// --------------------------------------------------------------------------------------
function ToggleViewConfigPane()
{
    try
    {
        if( ViewConfigPane.style.display == "none" )
        {
            var lAdjust = 0;
            GridPane.style.pixelTop += ViewConfigPane.style.pixelHeight + g_lPanePadding;
            if(g_OnTouchPanel)
            {
                GridPane.style.pixelTop += 15;
            }
            GridPane.style.pixelHeight += lAdjust - ViewConfigPane.style.pixelHeight - g_lPanePadding;
            if ( ( DetailsPane.style.display == "inline" ) && ( DetailsPane.style.pixelTop < GridPane.style.pixelTop + g_lPanePadding + GetNumberOfRows() ) )
                Handle_PaneResize( HorizontalResizePad_BOTTOM, GridPane.style.pixelTop + g_lPanePadding + GetNumberOfRows() );
            ViewConfigPane.style.display = "inline";
            btnViewConfigPane.selected = true;
            btnViewConfigPane.title = "Hide view configuration pane";
            txtViewName.select();
            g_FocusedObject = txtViewName;
            btnViewConfigPane.className = "toolBarBtn viewConfigPaneSelected";
        }
        else
        {
            ViewConfigPane.style.display = "none";
            GridPane.style.pixelTop -= ViewConfigPane.style.pixelHeight + g_lPanePadding;
            if(g_OnTouchPanel)
            {
                GridPane.style.pixelTop -= 15;
            }
            GridPane.style.pixelHeight += ViewConfigPane.style.pixelHeight + g_lPanePadding;
            btnViewConfigPane.selected = false;
            btnViewConfigPane.title = "Show view configuration pane";
            g_FocusedObject = Table;
            btnViewConfigPane.className = "toolBarBtn viewConfigPaneEnabled";
        }
    }
    catch( err )
    {
        HANDLE_ERROR("ToggleViewConfigPane", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function ToggleSystemStatusPane(bDoNotChangePersistence, bDoNotLoadDashboard)
{
    if (g_SummaryType != SUMMARY_SYSTEM_ALARM)
        return;
    try
    {
        var lAdjust = parseInt(SystemStatusPane.currentStyle.height) + HorizontalResizePad_TOP.style.pixelHeight;
        if( SystemStatusPane.style.display == "none" )  // open
        {
            ViewConfigPane.style.pixelTop += lAdjust + g_lPanePadding;    
            GridPane.style.pixelTop += lAdjust + g_lPanePadding;
            GridPane.style.pixelHeight += lAdjust + g_lPanePadding;
            if ( DetailsPane.style.display == "inline" )
                ToggleDetailsPane();
            SystemStatusPane.style.display = "inline";
            HorizontalResizePad_TOP.style.display = "inline";
            btnSystemStatusPane.selected = true;
            btnSystemStatusPane.title = "Hide dashboard pane";
            AdjustSystemStatusPane();
            if (bDoNotLoadDashboard !== true)
            {
                if (g_TDO.ViewPersistenceEnabled == false && !(bDoNotChangePersistence == true))
                {
                    // load the default dashboard
                    SetDefaultDashboard();
                }
                else
                {
                    var view_bag = document.getElementById(PROP_Cache_View);
                    var dashboardGroup = view_bag.getAttribute(PROP_DashboardGroup);
                    // if persistence is empty, open the overview dashbaord
                    if (!dashboardGroup || dashboardGroup.length == 0)
                        dashboardGroup = g_sOverviewDashboardGroup;
                    var dropdownTDO = document.getElementById("DashboardDropdown").Binding.DataObjects.Item(0);
                    dropdownTDO.SelectionListString = dashboardGroup;
                }
            }
        }
        else        // close
        {    
            // unload the dashboard display
            document.getElementById("dashboardiframe").contentWindow.location.replace("about:blank");
            // clear the dropdown selection
            var dropdownTDO = document.getElementById("DashboardDropdown").Binding.DataObjects.Item(0);
            dropdownTDO.SelectionListString = "";
            SystemStatusPane.style.display = "none";
            HorizontalResizePad_TOP.style.display = "none";
            ViewConfigPane.style.pixelTop -= lAdjust + g_lPanePadding;
            GridPane.style.pixelTop -= lAdjust + g_lPanePadding;
            GridPane.style.pixelHeight += lAdjust + g_lPanePadding;
            btnSystemStatusPane.selected = false;
            btnSystemStatusPane.title = "Show dashboard pane";
            g_FocusedObject = Table;
        }
    }
    catch( err )
    {
        HANDLE_ERROR("ToggleSystemStatusPane", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function AdjustSystemStatusPane()
{
    if ( (g_SummaryType != SUMMARY_SYSTEM_ALARM) || (SystemStatusPane.style.display == "none") )
        return;
    setTimeout(DelayedAdjustSystemStatusPane, 0);
}
function DelayedAdjustSystemStatusPane()
{
    try
    {
        var headerHeight = document.getElementById("DashboardTitlebar").clientHeight;
        var maxWidth = SystemStatusPane.clientWidth;
        var maxHeight = SystemStatusPane.clientHeight - headerHeight;
        var width = maxWidth - g_lPanePadding;
        var height = maxHeight - g_lPanePadding;
        // maintain 3:1 aspect ratio of the iframe
		var ratio = 3;
		if(divDashboardIframeWrapper.resized != "true")
		{
			// maintain the 3:1 aspect ratio of the iframe
			if (width > (height * ratio))
				width = height * ratio;
			else
				height = Math.floor(width / ratio);
		}
        divDashboardIframeWrapper.style.pixelHeight = height;
        divDashboardIframeWrapper.style.pixelWidth = width;
        divDashboardIframeWrapper.style.pixelTop = headerHeight + g_lPanePadding / 2;
        divDashboardIframeWrapper.style.pixelLeft = g_lPanePadding / 2;
        var iframe = document.getElementById("dashboardiframe");
        iframe.style.zoom = width / g_lDefaultDashboardWidth; 
    }
    catch ( err )
    {
        HANDLE_ERROR("DelayedAdjustSystemStatusPane", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function ToggleAlarmTrendPane()
{
    try
    {
        if (AlarmTrendPane.ignore == true)
        {
            AlarmTrendPane.ignore = false;
            g_FocusedObject = AlarmTrendPane;
            return;
        }
        if (AlarmTrendPane.style.display == "none")
        {
            AdjustAlarmTrendPane();
            // adjust all related component
            var heightChange = AlarmTrendPane.style.pixelHeight + HPadAlarmTrend.style.pixelHeight;
            HPadAlarmTrend.style.pixelTop = AlarmTrendPane.style.pixelTop + AlarmTrendPane.style.pixelHeight;
            ShiftToolbar(heightChange);
            PaneContainer.style.pixelTop += heightChange;
            PaneContainer.style.pixelHeight -= heightChange;
            GridPane.style.pixelTop += heightChange;
            GridPane.style.pixelHeight -= heightChange;
            // no height change for ViewConfigPane
            ViewConfigPane.style.pixelTop += heightChange;
            StatusPane.style.pixelTop += heightChange;
            StatusPane.style.pixelHeight -= heightChange;
            AreaPane.style.pixelTop += heightChange;
            AreaPane.style.pixelHeight -= heightChange;
            AreaPaneDropBorder.style.pixelTop += heightChange;
            AreaPaneDropBorder.style.pixelHeight -= heightChange;
            VerticalResizePad.style.pixelTop += heightChange;
            VerticalResizePad.style.pixelHeight -= heightChange;
            if ((DetailsPane.style.display == "inline") && (DetailsPane.style.pixelTop < GridPane.style.pixelTop + g_lPanePadding + GetNumberOfRows()))
                Handle_PaneResize(HorizontalResizePad_BOTTOM, GridPane.style.pixelTop + g_lPanePadding + GetNumberOfRows());
            SetTableSize();
            AreaPane.style.pixelHeight -= heightChange;
            AreaPaneDropBorder.style.pixelHeight -= heightChange;
            // adjust button state
            AlarmTrendPane.style.display = "inline";
            HPadAlarmTrend.style.display = "inline";
            btnAlarmTrendPane.selected = true;
            btnAlarmTrendPane.title = "Hide alarm tracker";
            btnAlarmTrendPane.className = "toolBarBtn alarmTrendSelected";
            g_FocusedObject = AlarmTrendPane;
            btnAreaPane.className = "toolBarBtn areaPaneEnabled";
        }
        else
        {
            // adjust button state
            AlarmTrendPane.style.display = "none";
            HPadAlarmTrend.style.display = "none";
            // adjust all related component
            var heightChange = AlarmTrendPane.style.pixelHeight + HPadAlarmTrend.style.pixelHeight;
            ShiftToolbar(0 - heightChange);
            PaneContainer.style.pixelTop -= heightChange;
            PaneContainer.style.pixelHeight += heightChange;
            ViewConfigPane.style.pixelTop -= heightChange;
            GridPane.style.pixelTop -= heightChange;
            GridPane.style.pixelHeight += heightChange;
            AreaPane.style.pixelTop -= heightChange;
            AreaPane.style.pixelHeight += heightChange;
            AreaPaneDropBorder.style.pixelTop -= heightChange;
            AreaPaneDropBorder.style.pixelHeight += heightChange;
            VerticalResizePad.style.pixelTop -= heightChange;
            VerticalResizePad.style.pixelHeight += heightChange;
            StatusPane.style.pixelTop -= heightChange;
            StatusPane.style.pixelHeight += heightChange;
            SetTableSize();
            btnAlarmTrendPane.selected = false;
            btnAlarmTrendPane.title = "Show alarm tracker";
            btnAlarmTrendPane.className = "toolBarBtn alarmTrendEnabled";
            // Put focus back to the Table
            g_FocusedObject = Table;
            g_bFlush = true;
            g_FocusedObject.setActive();
            window.event.cancelBubble = 'true';
        }
    }
    catch (err)
    {
        HANDLE_ERROR("ToggleAlarmTrendPane", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function AdjustAlarmTrendPane()
{
    // Use the PaneContainer to adjust Alarm Trend pane width
    AlarmTrendPaneWidth = Page.style.pixelWidth - PaneContainer.paddingLeft - PaneContainer.paddingRight;
    if (AlarmTrendPane.style.pixelWidth != AlarmTrendPaneWidth)
    {
        AlarmTrendPane.style.pixelWidth = AlarmTrendPaneWidth;
    }
}
// --------------------------------------------------------------------------------------
function ShiftToolbar(heightChange)
{
    AreaAndViewToolbar.style.pixelTop += heightChange;
    toolbar.style.pixelTop += heightChange;
    lblFilterApplied.style.pixelTop += heightChange;
    spnViewList.style.pixelTop += heightChange;
}
// --------------------------------------------------------------------------------------
function ToggleAreaPane()
{
    try
    {
        if( AreaPane.ignore == true )
        {
            AreaPane.ignore = false;
            g_FocusedObject = AreaTable;
            return;
        }
        if( AreaPane.style.display == "none" )
        {
            PinAreaPane();
            btnAreaPane.title = "Hide location pane";
            g_FocusedObject = AreaTable;
        }
        else
        {
            VerticalResizePad.style.display = "none";
            AreaPane.style.display = "none";
            SetVerticalPanePositions(AreaPaneVisible(), StatusPaneVisible());
            btnAreaPane.selected = false;
            btnAreaPane.title = "Show location pane";
            btnAreaPane.className = "toolBarBtn areaPaneEnabled";
            btnDropAreaPane.style.display = "inline";
            spnDropAreaPane.style.display = "none";
            btnDropAreaPane.title = "Show location pane";
            // Put focus back to the Table
            g_FocusedObject = Table;
            g_bFlush=true;
            g_FocusedObject.setActive();
            window.event.cancelBubble='true';
        }
    }
    catch( err )
    {
        HANDLE_ERROR("ToggleAreaPane", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function FloatAreaPane()
{
    if( String(btnDropAreaPane.selected) == "true" )
    {
        AreaPane.style.display = "inline";
        AreaPane.setActive();
        AreaPane.ondeactivate = HideAreaPane;
        btnSelectNone.onclick = ClickSelectNone;
        AreaTable.onclick = ClickAreaTable;
        btnAreaPaneClose.title = "Pin location pane";
        btnAreaPaneClose.OnHDXClick = PinAreaPane;
        btnAreaPaneClose.className = 'areaPaneOpen';
        btnAreaPaneClose.onmouseover = function() { overEffect('btnAreaPaneClose', 'areaPaneOpen areaPaneOpen'); };
        btnAreaPaneClose.onmouseout = function() { outEffect('btnAreaPaneClose', 'areaPaneOpen areaPaneOpen', ''); };
        btnDropAreaPane.title = "Hide location pane";
        // save last focused object
        g_LastFocusedObject = g_FocusedObject;
    }
    else
    {
        AreaPane.ondeactivate = null;
        AreaTable.onclick = null;
        btnSelectNone.onclick = null;
        AreaPane.style.display = "none";
        btnAreaPaneClose.className = 'areaPaneClose';
        btnAreaPaneClose.onmouseover = function() { overEffect('btnAreaPaneClose', 'areaPaneClose areaPaneClose'); };
        btnAreaPaneClose.onmouseout = function() { outEffect('btnAreaPaneClose', 'areaPaneClose areaPaneClose', ''); };
        btnAreaPaneClose.title = "Hide location pane";
        btnAreaPaneClose.OnHDXClick = ToggleAreaPane;
        AreaPaneDropBorder.style.display = "none";
        btnDropAreaPane.title = "Show location pane";
        // activate the last focused object
        g_FocusedObject = g_LastFocusedObject;
        g_FocusedObject.setActive();
    }
}
// --------------------------------------------------------------------------------------
function PinAreaPane()
{
    VerticalResizePad.style.pixelLeft = AreaPane.style.pixelWidth + PaneContainer.style.pixelLeft;
    InnerPaneContainer.style.pixelLeft = AreaPane.style.pixelWidth + g_lPanePadding;
    InnerPaneContainer.style.pixelWidth = PaneContainer.style.pixelWidth - AreaPane.style.pixelWidth - g_lPanePadding;
    AreaPane.style.display = "inline";
    VerticalResizePad.style.display = "inline";
    btnAreaPane.selected = true;
    btnAreaPane.title = "Hide location pane";
    btnDropAreaPane.style.display = "none";
    btnDropAreaPane.title = "Show location pane";
    spnDropAreaPane.style.display = "inline";
    btnAreaPaneClose.OnHDXClick = ToggleAreaPane;
    btnAreaPaneClose.className = 'areaPaneClose';
    btnAreaPaneClose.onmouseover = function() { overEffect('btnAreaPaneClose', 'areaPaneClose areaPaneClose'); };
    btnAreaPaneClose.onmouseout = function() { outEffect('btnAreaPaneClose', 'areaPaneClose areaPaneClose', ''); };
    btnAreaPaneClose.title = "Hide location pane";
    AreaPane.ondeactivate = null;
    AreaTable.onclick = null;
    btnSelectNone.onclick = null;
    btnDropAreaPane.selected = false;
    AreaPaneDropBorder.style.display = "none";
    SetVerticalPanePositions(AreaPaneVisible(), StatusPaneVisible());
}
// --------------------------------------------------------------------------------------
// Only called when the area table is floating
function ClickAreaTable()
{
    switch( TableComponentFromElement(AreaTable, window.event.srcElement) )
    {
    case TC_SCROLLBAR:
        // If we've clicked on a scrollbar of the table, don't want to close it....
        break;
    case TC_TBODY:
        // otherwise, we close the area pane by simulating another click on the toggle button
        btnDropAreaPane.click();
        break;
    default:
        // shouldn't happen
        break;
    }
}
// --------------------------------------------------------------------------------------
// Only called when the area table is floating
function ClickSelectNone()
{
    // we close the area pane by simulating another click on the toggle button
    btnDropAreaPane.click();
}
// --------------------------------------------------------------------------------------
function HideAreaPane()
{
    var toElement = window.event.toElement;
    if (!toElement)
        return;
    // If clicking on a scroll bar, do not hide
    if( toElement.tagName == "BODY" )
    {
        var pxRight = AreaPaneDropBorder.style.pixelLeft + AreaPaneDropBorder.style.pixelWidth + 100; // constant added to account for natural left-right mouse movement
        var pxBottom = AreaPaneDropBorder.style.pixelTop + AreaPaneDropBorder.style.pixelHeight;
        // if clicking on scroll bar, not message zone et al
        if ((window.event.clientX >= AreaPaneDropBorder.style.pixelLeft) &&
                (window.event.clientX <= pxRight) &&
                (window.event.clientY >= AreaPaneDropBorder.style.pixelTop) &&
                (window.event.clientY <= pxBottom))
            return;
    }
    if( AreaPane == toElement || AreaPane.contains(toElement) )
        return;
    if( btnDropAreaPane == toElement || btnDropAreaPane.contains(toElement) )
    {
        AreaPane.setActive();
        return;
    }
    else
    {
        btnDropAreaPane.selected = false;
    }
    if( btnAreaPane == toElement || btnAreaPane.contains(toElement) )
    {
        VerticalResizePad.style.pixelLeft = AreaPane.style.pixelWidth + PaneContainer.style.pixelLeft;
        InnerPaneContainer.style.pixelLeft = AreaPane.style.pixelWidth + g_lPanePadding;
        InnerPaneContainer.style.pixelWidth = PaneContainer.style.pixelWidth - AreaPane.style.pixelWidth - g_lPanePadding;
        VerticalResizePad.style.display = "inline";
        btnDropAreaPane.style.display = "none";
        spnDropAreaPane.style.display = "inline";
        AreaPane.ignore = true;
        btnAreaPane.title = "Hide location pane";
    }
    else
    {
        AreaPane.style.display = "none";
        btnAreaPane.title = "Show location pane";
    }
    AreaPaneDropBorder.style.display = "none";
    btnAreaPaneClose.title = "Hide location pane";
    btnAreaPaneClose.className = 'areaPaneClose';
    btnAreaPaneClose.onmouseover = function() { overEffect('btnAreaPaneClose', 'areaPaneClose areaPaneClose'); };
    btnAreaPaneClose.onmouseout = function() { outEffect('btnAreaPaneClose', 'areaPaneClose areaPaneClose', ''); };
    btnAreaPaneClose.OnHDXClick = ToggleAreaPane;
    AreaPane.ondeactivate = null;
    AreaTable.onclick = null;
    btnSelectNone.onclick = null;
}
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
function ToggleStatusPane()
{
    try
    {
        if( StatusPaneVisible() ) // if status pane is currently shown, Hide it
        {
            StatusPane.style.display = "none";
            SetVerticalPanePositions(AreaPaneVisible(), StatusPaneVisible());
            btnStatusPane.selected = false;
            btnStatusPane.title = "Show status pane";
            g_FocusedObject = Table;
            ClearFaceplate();
        }
        else	// show it
        {
            StatusPane.style.display = "inline";
            SetVerticalPanePositions(AreaPaneVisible(), StatusPaneVisible());
            btnStatusPane.selected = true;
            btnStatusPane.title = "Hide status pane";
            g_FocusedObject = Table;
            g_FocusedObject.setActive();
            RefreshFaceplate();
        }
    }
    catch( err )
    {
        HANDLE_ERROR("ToggleStatusPane", err.number, err.description);
    }
}
function SetVerticalPanePositions(bAreaPaneVisible, bStatusPaneVisible)
{
    if (bAreaPaneVisible && bStatusPaneVisible)
    {
        StatusPane.style.pixelLeft = PaneContainer.style.pixelLeft + AreaPane.style.pixelWidth + g_lPanePadding;
        InnerPaneContainer.style.pixelLeft = AreaPane.style.pixelWidth + g_lPanePadding + StatusPane.style.pixelWidth + g_lPanePadding;
    }
    else if (bAreaPaneVisible)
    {
        InnerPaneContainer.style.pixelLeft  = AreaPane.style.pixelWidth + g_lPanePadding;
    }
    else if (bStatusPaneVisible)
    {
        StatusPane.style.pixelLeft = PaneContainer.style.pixelLeft;
        InnerPaneContainer.style.pixelLeft  = StatusPane.style.pixelWidth + g_lPanePadding;
    }
    else
    {
        InnerPaneContainer.style.pixelLeft  = 0;
    }
    InnerPaneContainer.style.pixelWidth = PaneContainer.style.pixelWidth - InnerPaneContainer.style.pixelLeft;
}
function StatusPaneVisible()
{
    return (StatusPane.style.display != "none");
}
function AreaPaneVisible()
{
    return (btnAreaPaneClose.title != "Pin location pane" && AreaPane.style.display != "none");
}
// --------------------------------------------------------------------------------------
function ClearFaceplate()
{
    document.getElementById("faceplateiframe").contentWindow.location.replace("about:blank");
}
function RefreshFaceplate()
{
    var ptrIframe = document.getElementById("faceplateiframe");
    if(g_statuspane_FaceplateName == "")
    {
        ptrIframe.style.visibility = "hidden";
    }
    else
    {
        //Replace space characters with the URL encoding reference
        var g_statuspane_SelectedEntity_mod = g_statuspane_SelectedEntity.replace(/ /g,"%20");
        ptrIframe.contentWindow.location.replace(g_statuspane_FaceplateName + "?currentpoint=" + g_statuspane_SelectedEntity_mod +"&stretch=true&historystackignore=true");
        if(ptrIframe.style.visibility == "hidden")
            ptrIframe.style.visibility = "";
    }
}
function UpdateFaceplate()
{
    try
    {
        if (!g_bPageReady)
            return;
        if(g_bFiredOnce)
            return;
        if (selectednode.currentvalue == null || typeof(selectednode.currentvalue) == "undefined")
            return;
        var lfaceplatename;
        var strTagname = selectednode.currentvalue.toString();
        if (strTagname == "NaN")
            return;
        // Don't call for default value to avoid timing problems.
        //   $STATIONS doesn't have a faceplate anyway.
        if (strTagname != "$STATIONS")
        {
            g_bFiredOnce = true;
            lfaceplatename = HDXPopupBehavior.GetFaceplateURL(selectednode);
            // use settimeout to throttle the frequency if updatefaceplate() calls
            window.setTimeout(UnbindCustomProp, 200);
        }
        // Empty strings are easier to use and less error prone than nulls
        if (typeof(lfaceplatename) == "undefined")
            lfaceplatename = "";
        if (g_statuspane_FaceplateName != lfaceplatename)
        {
            g_statuspane_FaceplateName = lfaceplatename;
        }
        if (StatusPaneVisible())
        {
            RefreshFaceplate();
        }
    }
    catch (err)
    {
        window.external.logMessage("SummaryScript.js UpdateFaceplate(): Error - " + err.message);
    }
}
function UnbindCustomProp()
{
    DisplayDataRepository.PutValue("PointOnPage", "");
    g_bFiredOnce = false;
}
// --------------------------------------------------------------------------------------
function TabDetailsPane(tabID)
{
    var bToggleGeneralTab = false,
        bToggleCommentsTab = false,
        bToggleSuppressionTab = false,
        bToggleAlarmHelpTab = false,
        bToggleBlockCommentsTab = false,
        bToggleInstructionsTab = false;
    switch(tabID)
    {
        // General tab
        case DETAIL_GENERAL_TAB:
            bToggleGeneralTab = true;
            break;
        // Comments tab
        case DETAIL_COMMENT_TAB:
            bToggleCommentsTab = true;
            break;
        // Suppression tab
        case DETAIL_SUPPRESSION_TAB:
            bToggleSuppressionTab = true;
            break;
        // Alarm Help tab
        case DETAIL_ALARMHELP_TAB:
            bToggleAlarmHelpTab = true;
            break;
        // Block Comments tab
        case DETAIL_BLOCKCOMMENT_TAB:
            bToggleBlockCommentsTab = true;
            break;
        // Instructions tab
        case DETAIL_INSTRUCTION_TAB:
            bToggleInstructionsTab = true;
            break;
        // Unknown tab
        default:
            HANDLE_ERROR("TabDetailsPane", tabID, "Unknown tab selected");
            break;
    }
    ToggleGeneralTab(bToggleGeneralTab);
    ToggleCommentsTab(bToggleCommentsTab);
    ToggleSuppressionTab(bToggleSuppressionTab);
    ToggleAlarmHelpTab(bToggleAlarmHelpTab);
    ToggleBlockCommentsTab(bToggleBlockCommentsTab);
    ToggleInstructionsTab(bToggleInstructionsTab);
    if((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT) || (g_SummaryType == SUMMARY_ACTIVITY) || (g_SummaryType == SUMMARY_BATCH) || (g_SummaryType == SUMMARY_PROCEDURE) )
    {
        CheckLiveValueUpdates();
    }
}
// --------------------------------------------------------------------------------------
function CheckLiveValueUpdates()
{
    var bEnableLiveValueUpdates = false;
    if( (DetailsPane.style.display != "none") && (SelectedRowData.style.display != "none") && (DefaultData.style.display != "none") )
    {
        bEnableLiveValueUpdates = true;
    }
    else
    {
        bEnableLiveValueUpdates = false;
    }
    if (g_bEnableLiveValueUpdates != bEnableLiveValueUpdates)
    {
        g_bEnableLiveValueUpdates = bEnableLiveValueUpdates;
        g_TDO.LiveValueUpdates(g_bEnableLiveValueUpdates);
    }
}
// --------------------------------------------------------------------------------------
function RequestComments()
{
    if( !g_bCommentUnavaiable )
        g_TDO.RequestComments();
}
// --------------------------------------------------------------------------------------
function SaveComments()
{
    if(btnSaveDetails.readonly)
    {
        return;
    }
    var rgExp = /'/g;
    var strComments = AddComments.value;
    strComments = strComments.replace(rgExp,"''");
    if( strComments.length > 200)
    {
        var strMessage = "Comments cannot contain more than 200 characters.";
        window.external.application.TemporaryMessageZoneText = strMessage;
    }
    else
    {
        btnSaveDetails.readonly = true;
        btnSaveDetails.className = 'saveDetailsDisabled';
        g_TDO.AddComment(strComments);
        AddComments.value = "";
    }
}
// --------------------------------------------------------------------------------------
function OnCommentsChanged()
{
    if( AddComments.isContentEditable == true && AddComments.value != "" )
    {
        if( btnSaveDetails.readonly )
        {
            btnSaveDetails.readonly = false;
            btnSaveDetails.className = 'saveDetailsEnabled';
        }
    }
    else
    {
        if( !btnSaveDetails.readonly )
        {
            btnSaveDetails.readonly = true;
            btnSaveDetails.className = 'saveDetailsDisabled';
        }
    }
}
// --------------------------------------------------------------------------------------
function OnUpdateComments(dtRestoreDate, vbCanAddComments, bstrCommentsXML)
{
    // Don't worry about updating comments if not displaying them
    if (CommentsData.style.display == "none")
        return;
    // A null dtRestoreDate is returned if all comments are available - various definitions of the null
    // have been seen including 1753-01-01 in the emsevents function utcfiletodatetime and 1600-01-01
    // Pretty safe to to assume any date before 1800-01-01 is a null date.
    var dtNullDate = new Date("1 Jan 1800");
    if( !g_bCommentUnavaiable )
    {
        var sRestoreDate = "";
        if( vbCanAddComments )
        {
            AddComments.disabled = false;
            AddComments.contentEditable = true;
            AddComments.value = "";
            AddComments.style.cursor = "text";
            if (g_bFirstCommentsCallup)
            {
                AddComments.focus();
                g_bFirstCommentsCallup = false;
            }
        }
        else
        {
            AddComments.disabled = false;
            AddComments.contentEditable = false;
            AddComments.value = "(No more comments can be added)";
            AddComments.style.cursor = "default";
        }
        btnSaveDetails.readonly = true;
        btnSaveDetails.className = 'saveDetailsDisabled';
        TransformExistComments(bstrCommentsXML);
        if( dtRestoreDate > dtNullDate )
        {
            sRestoreDate = "<DIV style=\"font-family: 'Helvetica'; font-size: 9pt; font-weight: bold; position: relative; padding-top: 8px;\">"+
                           "<SPAN style=\"color: red; overflow: hidden; text-overflow: ellipsis; position: relative; white-space: nowrap;\">(" +
                           "Not all comments are displayed" + ")" +
                           "</SPAN>" +
                           "</DIV>" +
                           "<DIV style=\"color: red; font-family: 'Helvetica'; font-size: 9pt;\">(" +
                           "To show all comments, restore the event archives dated up to " + dtRestoreDate + ")" +
                           "</DIV>";
        }
        g_ExistCommentsHTML = sRestoreDate + g_ExistCommentsHTML;
        PopulateExistComments();
    }
}
// --------------------------------------------------------------------------------------
function OnUpdateSuppressionRelationships(bstrAlarmsSuppressingSelectedAlarmXML, bstrAlarmsSuppressedBySelectedAlarmXML)
{
    // Don't worry about updating suppression relationships if not displaying them
    if (SuppressionRelationshipsData.style.display == "none")
        return;
    // Convert the xml strings to html and then populate the display
    TransformSuppressionRelationships(bstrAlarmsSuppressingSelectedAlarmXML, bstrAlarmsSuppressedBySelectedAlarmXML);
    PopulateSuppressionRelationships();
}
// --------------------------------------------------------------------------------------
function RequestSuppressionRelationships()
{
    g_TDO.RequestSuppressionRelationships();//TODO: Activate this call
}
// --------------------------------------------------------------------------------------
function ToggleSuppressionTab(bSelected)
{
    if(g_SummaryType == SUMMARY_ALARM)
    {
        if(bSelected == true)
        {
            btnSuppressionRelationshipsTab.className='summaryBtnTabSelected';
            btnSuppressionRelationshipsTab.style.pixelHeight = 25;
            SuppressionRelationshipsData.style.display = "inline";
            if( g_AlarmsSuppressedBySelectedAlarmHTML != "" && g_AlarmsSuppressingSelectedAlarmHTML != "")
            {
                PopulateSuppressionRelationships();
            }
            else
            {
                RequestSuppressionRelationships();
            }
        }
        else
        {
            btnSuppressionRelationshipsTab.className='summaryBtnTab';
            btnSuppressionRelationshipsTab.style.pixelHeight = 23;
            SuppressionRelationshipsData.style.display = "none";
        }
    }
}
// --------------------------------------------------------------------------------------
function RequestAlarmHelp()
{
    // if side panel is visible, it means an alarm is selected so show alarm help text
    if (divDetailsSidePanel.style.visibility == "visible")
        AlarmHelpData.innerText = "Loading alarm help...";
    g_TDO.RequestAlarmHelp();
}
// --------------------------------------------------------------------------------------
function ToggleAlarmHelpTab(bSelected)
{
    if (g_SummaryType == SUMMARY_ALARM || g_SummaryType == SUMMARY_SYSTEM_ALARM)
    {
        if (bSelected == true)
        {
            btnAlarmHelpTab.className='summaryBtnTabSelected';
            btnAlarmHelpTab.style.pixelHeight = 25;
            AlarmHelpData.style.display = "inline";
            RequestAlarmHelp();
        }
        else
        {
            btnAlarmHelpTab.className='summaryBtnTab';
            btnAlarmHelpTab.style.pixelHeight = 23;
            AlarmHelpData.style.display = "none";
        }
    }
}
// --------------------------------------------------------------------------------------
// ResizeAlarmHelp()
//
// Sizes alarm help height into even quadrants given panel height
// Adds collapse buttons if content overflows qadrant
// --------------------------------------------------------------------------------------
function ResizeAlarmHelp()
{
    try 
    {
        if(g_SummaryType == SUMMARY_SYSTEM_ALARM && AlarmHelpData.style.display == "inline")
        {
            var causediv = document.getElementById('divSysAlmHlpCauseContent');
            if(!causediv)
            {
                return;
            }
            var conseqdiv = document.getElementById('divSysAlmHlpConseqContent');
            if(!conseqdiv)
            {
                return;
            }
            var actdiv = document.getElementById('divSysAlmHlpActionContent');
            if(!actdiv)
            {
                return;
            }
            var timediv = document.getElementById('divSysAlmHlpTimeContent');
            if(!timediv)
            {
                return;
            }
            g_HlpContentSize = CalcSysAlmHlpContentDivHeight();
            CollapseSysAlmHlp(causediv);
            CollapseSysAlmHlp(conseqdiv);
            CollapseSysAlmHlp(actdiv);
            CollapseSysAlmHlp(timediv);
        }
    }
    catch(err)
    {
        HANDLE_ERROR("ResizeAlarmHelp", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// CalculateContentDivSize()
//
// Calcs content div size to fit 4 alarm help quadrants evenly into lower panel.
// Height calculated to fit even number of lines of text (i.e. not cut text in half)
// --------------------------------------------------------------------------------------
function CalcSysAlmHlpContentDivHeight()
{
    try
    {
        // -------------------------------
        // | AlmHlpCell ... padding      |
        // |            ... cell head    |
        // |            ... content div  |
        // -------------------------------
        // | AlmHlpCell ... padding      |
        // |            ... cell head    |
        // |            ... content div  |
        // -------------------------------
        var cellPad = parseInt(divSysAlmHlpCauseCell.currentStyle.paddingTop);
        var contentDivPad = parseInt(divSysAlmHlpCauseContent.currentStyle.paddingTop);
        var contentLineHeight =  parseInt(divSysAlmHlpCauseContent.currentStyle.lineHeight);
        //heightPerCell = available height / 2
        var heightPerCell = ((AlarmHelpData.clientHeight) /SYSALMHLP_CELL_ROWS);
        var contentDivHeight = heightPerCell - divSysAlmHlpCauseHead.offsetHeight - contentDivPad - cellPad;
        //round down to number of lines that will fit
        var linesText = Math.floor((contentDivHeight-contentDivPad)/ contentLineHeight); 
        //enforce a minimum number of lines shown
        if((linesText < 0) || (linesText < SYSALMHLP_MIN_LINES_SHOWN))
        {
            linesText = SYSALMHLP_MIN_LINES_SHOWN;
        }
        return (linesText * contentLineHeight) + contentDivPad; //height of text + padding
    }
    catch(err)
    {
        HANDLE_ERROR("CalculateContentDivSize", err.number, err.description);
    }  
}
// --------------------------------------------------------------------------------------
// CollapseSysAlmHlp()
//
// Collapses an alarm help cell
// --------------------------------------------------------------------------------------
function CollapseSysAlmHlp(contentdiv)
{
    try
    {
        RemoveAlmHlpButton(contentdiv);
        if(g_HlpContentSize > 0)
        {
            contentdiv.style.height = g_HlpContentSize;
        }
        //if height exceeds than container height
        if(contentdiv.scrollHeight > g_HlpContentSize)
        {
            AddAlmHelpButton(contentdiv,'sysAlmHlpExpand');
        }
    }
    catch(err)
    {
        HANDLE_ERROR("CollapseSysAlmHlp", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// AddAlmHelpButton()
//
// Adds an alarm help expand/collapse button with style to contentdiv 
// --------------------------------------------------------------------------------------
function AddAlmHelpButton(contentdiv,style)
{
    try
    {
        var button = document.createElement('div');
        button.className = style;
        button.onclick = ToggleAlarmHelpCollapse;
        contentdiv.appendChild(button);
    } 
    catch(err)
    {
        HANDLE_ERROR("AddAlmHelpButton", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// RemoveAlmHlpButton()
//
// Removes expand/collapse buttons from contentdiv 
// -----------------------------------------------
function RemoveAlmHlpButton(contentdiv)
{
    try
    {
        var children = contentdiv.children;
        for(var i = 0; i < children.length; i++)
        {
            var cname = children[i].className; 
            if(cname == "sysAlmHlpCollapse" || cname == "sysAlmHlpExpand")
            {
                contentdiv.removeChild(children[i]); //remove button (if exists)
            }
        }
    }
    catch(err)
    {
        HANDLE_ERROR("RemoveButton", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// ExpandHelp()
//
// Expands an alarm help cell to show full text
// --------------------------------------------------------------------------------------
function ExpandHelp(contentdiv)
{
    try 
    {
        RemoveAlmHlpButton(contentdiv);
    	contentdiv.style.height = 'auto';
        AddAlmHelpButton(contentdiv,'sysAlmHlpCollapse');
    }
    catch(err)
    {
        HANDLE_ERROR("ExpandHelp", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// ToggleAlarmHelpCollapse()
//
// Toggles alarm help cell between expand/collapsed/
// Called as onclick for expand/collapse button
// --------------------------------------------------------------------------------------
function ToggleAlarmHelpCollapse()
{
    try
    { 
        var container = this.parentNode;
        if(container.style.height == "auto")
        {
            //collapse an expanded content cell
            CollapseSysAlmHlp(container);
        }
        else
        {
            //expand a collapsed cell
            ExpandHelp(container);
        }
    }
    catch (err)
    {
        HANDLE_ERROR("ToggleAlarmHelpCollapse", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function ToggleBlockCommentsTab(bSelected)
{
    if(g_SummaryType == SUMMARY_ALARM)
    {
        if(bSelected == true)
        {
            btnLocaleTab.className='summaryBtnTabSelected';
            btnLocaleTab.style.pixelHeight = 25;
            BlockCommentsData.style.display = "inline";
        }
        else
        {
            btnLocaleTab.className='summaryBtnTab';
            btnLocaleTab.style.pixelHeight = 23;
            BlockCommentsData.style.display = "none";
        }
    }
}
// --------------------------------------------------------------------------------------
function ToggleGeneralTab(bSelected)
{
    if(bSelected == true)
    {
        btnGeneralTab.className='summaryBtnTabSelected';
        btnGeneralTab.style.pixelHeight = 25;
        SelectedRowData.style.display = "inline";
    }
    else
    {
        btnGeneralTab.className='summaryBtnTab';
        btnGeneralTab.style.pixelHeight = 23;
        SelectedRowData.style.display = "none";
    }
}
// --------------------------------------------------------------------------------------
function ToggleCommentsTab(bSelected)
{
    if((g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT) || (g_SummaryType == SUMMARY_EVENTVIEW))
    {
        if(bSelected == true)
        {
            btnCommentsTab.className='summaryBtnTabSelected';
            btnCommentsTab.style.pixelHeight = 25;
            CommentsData.style.display = "inline";
            if ( AddComments.isContentEditable == true && AddComments.value != "" )
            {
                btnSaveDetails.readonly = false;
                btnSaveDetails.className = 'saveDetailsEnabled';
                if (AddComments.disabled)
                    AddComments.value = "";
            }
            if( g_ExistCommentsHTML != "" )
            {
                PopulateExistComments();
            }
            else
            {
                RequestComments();
            }
        }
        else
        {
            btnCommentsTab.className='summaryBtnTab';
            btnCommentsTab.style.pixelHeight = 23;
            CommentsData.style.display = "none";
            btnSaveDetails.readonly = true;
            btnSaveDetails.className = 'saveDetailsDisabled';
        }
    }
}
// --------------------------------------------------------------------------------------
function ToggleDetailsPane()
{
    try
    {
        if( DetailsPane.style.display == "none" )
        {
            ShowDetailsPane();
        }
        else
        {
            HorizontalResizePad_BOTTOM.style.display = "none";
            if((g_SummaryType == SUMMARY_ACTIVITY) || (g_SummaryType == SUMMARY_BATCH) || (g_SummaryType == SUMMARY_PROCEDURE) ) {
                //ISR11994 hide the BOTTOM extended resize pad
                divExtendedResizepadBOTTOM.style.display="none";
            }
            DetailsPane.style.display = "none";
            if(g_OnTouchPanel)
            {
                GridPane.style.pixelHeight += DetailsPane.style.pixelHeight;
            }
            else
            {
                GridPane.style.pixelHeight += DetailsPane.style.pixelHeight + g_lPanePadding;
            }
            btnDetailsPane.selected = false;
            btnDetailsPane.className='toolbarbtn detailsPaneEnabled';
            btnDetailsPane.title = "Show details pane";
            g_FocusedObject = Table;
        }
        if((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT) || (g_SummaryType == SUMMARY_ACTIVITY) || (g_SummaryType == SUMMARY_BATCH) || (g_SummaryType == SUMMARY_PROCEDURE) )
        {
            CheckLiveValueUpdates();
        }
        if((g_SummaryType == SUMMARY_ACTIVITY) || (g_SummaryType == SUMMARY_BATCH) || (g_SummaryType == SUMMARY_PROCEDURE) )
        {
            SetActCreateUIConfig();
        }
    }
    catch( err )
    {
        HANDLE_ERROR("ToggleDetailsPane", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function ShowDetailsPane() {
    try {
        if (DetailsPane.style.display == "none") {
            GridPane.style.pixelHeight -= DetailsPane.style.pixelHeight + g_lPanePadding;
            if (GridPane.style.pixelHeight < GetNumberOfRows()) {
                DetailsPane.style.pixelHeight = DetailsPane.style.pixelHeight + GridPane.style.pixelHeight - GetNumberOfRows();
                GridPane.style.pixelHeight = GetNumberOfRows();
            }
            DetailsPane.style.pixelTop = GridPane.style.pixelTop + GridPane.style.pixelHeight + g_lPanePadding;
            HorizontalResizePad_BOTTOM.style.pixelTop = DetailsPane.style.pixelTop - g_lPanePadding;
            DetailsPane.style.display = "inline";
            HorizontalResizePad_BOTTOM.style.display = "inline";
            btnDetailsPane.selected = true;
            btnDetailsPane.title = "Hide details pane";
            if ((ViewConfigPane.style.display == "inline") && (DetailsPane.style.pixelTop < GridPane.style.pixelTop + g_lPanePadding + GetNumberOfRows()))
                Handle_PaneResize(HorizontalResizePad_BOTTOM, GridPane.style.pixelTop + g_lPanePadding + GetNumberOfRows());
            g_FocusedObject = DetailsPane;
            //update the details pane
            if ((g_SummaryType == SUMMARY_ACTIVITY) || (g_SummaryType == SUMMARY_BATCH) || (g_SummaryType == SUMMARY_PROCEDURE)) {
                //clear details pane binding if no selection on grid
                if (!g_SelectionOnGrid) {
                    UnbindActivity();
                }
                UpdateActDetailsPane();
                //ISR119942 show the BOTTOM extended resize pad
                divExtendedResizepadBOTTOM.style.display = "inline";
            } else {
                PopulateDetailsPane(g_DetailsPaneXML, true, false, 0);
            }
            if ((g_SummaryType == SUMMARY_EVENT) || (g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT) || (g_SummaryType == SUMMARY_EVENTVIEW)) {
                if (CommentsData.style.display != "none") {
                    if (g_ExistCommentsHTML != "") {
                        PopulateExistComments();
                    }
                    else {
                        RequestComments();
                    }
                }
            }
            if ((g_SummaryType == SUMMARY_ALARM)) {
                if (SuppressionRelationshipsData.style.display != "none") {
                    if (g_AlarmsSuppressingSelectedAlarmHTML != "" && g_AlarmsSuppressedBySelectedAlarmHTML != "") {
                        PopulateSuppressionRelationships();
                    }
                    else {
                        RequestSuppressionRelationships();
                    }
                }
                else if (AlarmHelpData.style.display != "none") {
                    RequestAlarmHelp();
                }
            }
            if ((g_SummaryType == SUMMARY_SYSTEM_ALARM) && (SystemStatusPane.style.display == "inline"))
                ToggleSystemStatusPane(false, false);
        }
    }
    catch (err) {
        HANDLE_ERROR("ShowDetailsPane", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function ToggleInstructionsTab(bSelected) {
    if (g_SummaryType == SUMMARY_MESSAGE) {
        if (bSelected == true) {
            btnInstructionsTab.className = 'summaryBtnTabSelected';
            btnInstructionsTab.style.pixelHeight = 25;
        }
        else {
            btnInstructionsTab.className = 'summaryBtnTab';
            btnInstructionsTab.style.pixelHeight = 23;
        }
    }
}
// --------------------------------------------------------------------------------------
function HandleDetailsPaneFocus()
{
    if(DetailsPane.style.display!='none')
    {
        g_FocusedObject=window.event.srcElement;
        window.event.cancelBubble='true';
    }
}
// --------------------------------------------------------------------------------------
function LinksAppliedUpdate(elem)
{
    try {
        if( window.event.propertyName == "linkDisabled" )
        {
            elem.className = elem.className.replace(/\linkTextVisible\b/,'');
            elem.className = elem.className.replace(/\linkTextHidden\b/,'');
            if( elem.linkDisabled )
            {
                elem.className += " linkTextHidden";
            }
            else
            {
                elem.className += " linkTextVisible";
            }
        }
    }
    //ISR116436 invalid elements passed into this function will be ignored
    catch(err) {
        //do nothing, ignore invalid elements
    }
}
// --------------------------------------------------------------------------------------
function Handle_PaneResize(pad, newposn)
{
    try
    {
        if( pad == HorizontalResizePad_BOTTOM )
        {
            GridPane.style.pixelHeight = newposn - GridPane.style.pixelTop;
            DetailsPane.style.pixelTop = newposn + g_lPanePadding;
            DetailsPane.style.pixelHeight = InnerPaneContainer.style.pixelHeight - DetailsPane.style.pixelTop;
            HorizontalResizePad_BOTTOM.style.pixelTop = newposn;
	    ResizeAlarmHelp();
        }
        else if(g_SummaryType == SUMMARY_SYSTEM_ALARM && pad == HorizontalResizePad_TOP )
        {
            SystemStatusPane.style.pixelHeight = newposn - parseInt(SystemStatusPane.currentStyle.top);
            ViewConfigPane.style.pixelTop = newposn + g_lPanePadding;
            GridPane.style.pixelHeight -= (newposn + g_lPanePadding - GridPane.style.pixelTop);
            GridPane.style.pixelTop = newposn + g_lPanePadding;
            HorizontalResizePad_TOP.style.pixelTop = newposn;
        }
        else if (g_SummaryType == SUMMARY_ALARM && pad == HPadAlarmTrend)
        {
            var heightChange = newposn - HPadAlarmTrend.style.pixelTop;
            HPadAlarmTrend.style.pixelTop = newposn;
            AlarmTrendPane.style.pixelHeight += heightChange;
            ShiftToolbar(heightChange);
            AreaPane.style.pixelTop += heightChange;
            AreaPaneDropBorder.style.pixelTop += heightChange;
            VerticalResizePad.style.pixelTop += heightChange;
            StatusPane.style.pixelTop += heightChange;
            PaneContainer.style.pixelTop += heightChange;
            AreaPane.style.pixelHeight -= heightChange;
            AreaPaneDropBorder.style.pixelHeight -= heightChange;
            VerticalResizePad.style.pixelHeight -= heightChange;
            StatusPane.style.pixelHeight -= heightChange;
            PaneContainer.style.pixelHeight -= heightChange;
            ViewConfigPane.style.pixelTop += heightChange;
            GridPane.style.pixelTop += heightChange;
            GridPane.style.pixelHeight -= heightChange;
        }
        else
        {
            AreaPane.style.pixelWidth = newposn;
            VerticalResizePad.style.pixelLeft = AreaPane.style.pixelWidth + PaneContainer.style.pixelLeft;
            SetVerticalPanePositions(true, StatusPaneVisible());
        }
    }
    catch( err )
    {
        HANDLE_ERROR("Handle_PaneResize", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
// Pane resizing routines
// --------------------------------------------------------------------------------------
var g_iDetailPaneHeightMin = 170;
var g_iDetailPaneHeightMin = 170;
// Assumes only the Page element's zoom style attribute is ever set
function Pane_OnStartResize(pad)
{
    try
    {
        var zoom = ParseZoom(Page.style.zoom);
        g_oCurrentPad = pad;
        var tempY = g_SummaryType != SUMMARY_EVENTVIEW ? window.event.clientY : window.event.clientY - g_lPanePadding + 37;
        PaneResizer.style.cursor = pad.style.cursor;
        PaneResizer.vertical = pad.direction == "v";
        if (PaneResizer.vertical)
        {
            PaneResizer.style.pixelHeight = pad.style.pixelHeight - 1;
            PaneResizer.style.pixelWidth = Math.min(pad.style.pixelWidth - 1, MAX_RESIZER_SIZE);
        }
        else
        {
            PaneResizer.style.pixelHeight = Math.min(pad.style.pixelHeight - 1, MAX_RESIZER_SIZE);
            PaneResizer.style.pixelWidth = pad.style.pixelWidth - 1;
        }
        PaneResizer.style.pixelTop = tempY / zoom - window.event.offsetY - 2 ;
        PaneResizer.style.pixelLeft = window.event.clientX / zoom - window.event.offsetX - 2;
        PaneResizer.min = parseInt(g_oCurrentPad.min, 10);
        PaneResizer.max = parseInt(g_oCurrentPad.max, 10);
        PaneResizer.startposn = PaneResizer.vertical ? window.event.clientX : tempY;
        PaneResizer.newposn = null;
        PaneResizer.onmousemove = Pane_OnResize;
        PaneResizer.onlosecapture = Pane_OnCancelResize;
        PaneResizer.onmouseup = Pane_OnEndResize;
        PaneResizer.style.display = "inline";
        PaneResizer.setActive();		// To hide any visible menu
        PaneResizer.setCapture(true);
    }
    catch( err )
    {
        HANDLE_ERROR("Pane_OnStartResize", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function Pane_OnResize()
{
    try
    {
        var zoom = ParseZoom(Page.style.zoom);
        var diff = 0;
        if( PaneResizer.vertical )
        {
            diff = (window.event.clientX - PaneResizer.startposn) / zoom;
            var newLeft = g_oCurrentPad.style.pixelLeft + diff;
            if( newLeft < PaneResizer.min )
                newLeft = PaneResizer.min;
            else if( newLeft > PaneResizer.max )
                newLeft = PaneResizer.max;
            diff = newLeft - g_oCurrentPad.style.pixelLeft;
            PaneResizer.newposn = newLeft;
            PaneResizer.runtimeStyle.pixelLeft = PaneResizer.style.pixelLeft + diff ;
        }
        else
        {
            var rowHeight = Table.rowHeight;
            // Calculate the min and max height of the pane so that the last row is not partially obscured
            var PaneMinHeight = PaneResizer.min - ((PaneResizer.min - g_oCurrentPad.style.pixelTop) % rowHeight);
            var PaneMaxHeight = PaneResizer.max - ((PaneResizer.max - g_oCurrentPad.style.pixelTop) % rowHeight);
            diff = (window.event.clientY - PaneResizer.startposn) / zoom;
            var newTop = g_oCurrentPad.style.pixelTop + diff;
            if( (diff % rowHeight) > (rowHeight / 2))
                newTop = newTop + rowHeight;
            newTop = newTop - (diff % rowHeight);
            if( newTop < PaneMinHeight )
                newTop = PaneMinHeight;
            else if( newTop > PaneMaxHeight )
                newTop = PaneMaxHeight;
            diff = newTop - g_oCurrentPad.style.pixelTop;
            PaneResizer.newposn = newTop;
            PaneResizer.runtimeStyle.pixelTop = PaneResizer.style.pixelTop + diff;
        }
    }
    catch( err )
    {
        HANDLE_ERROR("Pane_OnResize", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function Pane_OnEndResize()
{
    try
    {
        if( PaneResizer.newposn != null )
            Handle_PaneResize(g_oCurrentPad, PaneResizer.newposn);
        PaneResizer.releaseCapture();
    }
    catch( err )
    {
        HANDLE_ERROR("Pane_OnEndResize", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function Pane_OnCancelResize()
{
    try
    {
        g_oCurrentPad = null;
        PaneResizer.runtimeStyle.left = "";
        PaneResizer.runtimeStyle.top = "";
        PaneResizer.startposn = 0;
        PaneResizer.newposn = null;
        PaneResizer.style.display = "none";
        PaneResizer.onmouseup = null;
        PaneResizer.onmousemove = null;
        PaneResizer.onlosecapture = null;
    }
    catch( err )
    {
        HANDLE_ERROR("Pane_OnCancelResize", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function CheckViewName()
{
    var prop = "";
    if( arguments.length > 0 )
        prop = arguments[0];
    else
        prop = window.event.propertyName;
    if( prop == "value" )
    {
        if( txtViewName.value.length == 0 )
        {
            btnSaveView.readonly = true;
            btnDeleteView.readonly = true;
            btnSaveView.className='saveViewBtnDisabled';
            btnDeleteView.className='deleteViewDisabled';
        }
        else
        {
            if( g_bSaveViewsEnabled )
            {
                btnSaveView.readonly = false;
                btnSaveView.className='saveViewBtnEnabled';
                CheckViewDelete(false);
            }
        }
    }
}
// --------------------------------------------------------------------------------------
function SaveCurrentView()
{
    if(btnSaveView.readonly)
    {
        return;
    }
    g_TDO.SaveView(txtViewName.value, chkAssignArea.checked ? txtAreaName.value : "", Table.GetState(), chkIncludeArea.checked, false);
}
// --------------------------------------------------------------------------------------
function DeleteCurrentView()
{
    if(btnDeleteView.readonly)
    {
        return;
    }
    var strMessage = "Are you sure you want to delete the current view?";
    ConfirmMsg(strMessage, "DeleteCurrentView", "", "", "", "", "");
}
// --------------------------------------------------------------------------------------
function AreaAssignChange()
{
    if( window.event.propertyName == "checked" )
    {
        if( chkAssignArea.checked )
        {
            txtAreaName.disabled = false;
            txtAreaName.className='viewTxtAreaName';
        }
        else
        {
            txtAreaName.disabled = true;
            txtAreaName.className='viewTxtAreaNameDisabled';
        }
    }
}
// --------------------------------------------------------------------------------------
// Bound to the TDO's OnRefreshViewList event. The TDO fires this event in response to
// a request from the view drop-down (when the user attempts to display the view menu).
function RefreshViewList(list)
{
    var rgList = new VBArray(list).toArray();
    var item = null;
    spnViewList.innerHTML = "";
    spnViewList.style.height = "auto";
    spnViewList.style.overflowY = "auto";
    for( var i=0; i<rgList.length; i++ )
    {
        spnViewList.insertAdjacentHTML("beforeEnd", xmlViewListItemTemplate.text);
        item = spnViewList.lastChild;
        item.innerText = rgList[i];
        item.title = rgList[i];
    }
    if( spnViewList.offsetHeight >= 150 )
    {
        spnViewList.style.height = "150px";
        spnViewList.style.overflowY = "scroll";
    }
    spnViewList.scrollTop = 0;
    spnViewList.style.visibility = "visible";
    spnViewList.setActive();
    spnViewList.ondeactivate = HideViewList;
}
// --------------------------------------------------------------------------------------
function FilterAppliedUpdate()
{
    if( window.event.propertyName == 'filterApplied' )
    {
        if( String(lblFilterApplied.filterApplied) == "true" )
        {
            lblFilterApplied.style.display = "inline";
            if ((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_MESSAGE) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT))
            {
                if (g_SummaryType != SUMMARY_MESSAGE)
                {
                    divShelvedFilteredTotal.className = 'filter_text';
                    if (g_SummaryType == SUMMARY_ALARM)
                    {
                        divSuppressedFilteredTotal.className = 'filter_text';
                    }
                }
                divUnackFilteredTotal.className = 'filter_text';
                divAckFilteredTotal.className = 'filter_text';
                lblOfAckTotal.style.display = "inline";
                divAckUnfilteredTotal.style.display = "inline";
                lblOfUnackTotal.style.display = "inline";
                divUnackUnfilteredTotal.style.display = "inline";
            }
        }
        else
        {
            lblFilterApplied.style.display = "none";
            if ((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_MESSAGE) || (g_SummaryType == SUMMARY_SYSTEM_ALARM) || (g_SummaryType == SUMMARY_ALERT))
            {
                if (g_SummaryType != SUMMARY_MESSAGE)
                {
                    divShelvedFilteredTotal.className = 'unfilter_text';
                    if (g_SummaryType == SUMMARY_ALARM)
                    {
                        divSuppressedFilteredTotal.className = 'unfilter_text';
                    }
                }
                divUnackFilteredTotal.className = 'unfilter_text';
                divAckFilteredTotal.className = 'unfilter_text';
                lblOfAckTotal.style.display = "none";
                divAckUnfilteredTotal.style.display = "none";
                lblOfUnackTotal.style.display = "none";
                divUnackUnfilteredTotal.style.display = "none";
            }
            if ((g_SummaryType == SUMMARY_ALERT) && ((divUnackFilteredTotal.innerText != divUnackUnfilteredTotal.innerText) || (divAckFilteredTotal.innerText != divAckUnfilteredTotal.innerText)))
            {
                divShelvedFilteredTotal.className = 'filter_text';
                divUnackFilteredTotal.className = 'filter_text';
                divAckFilteredTotal.className = 'filter_text';
                lblOfAckTotal.style.display = "inline";
                divAckUnfilteredTotal.style.display = "inline";
                lblOfUnackTotal.style.display = "inline";
                divUnackUnfilteredTotal.style.display = "inline";
            }
        }
    }
}
// --------------------------------------------------------------------------------------
function DropViewList()
{
    if( String(btnDropViewList.selected) != "true" )
    {
        spnViewList.ondeactivate = null;
        spnViewList.style.visibility = "hidden";
        // activate the last focused object
        g_FocusedObject.setActive();
    }
}
// --------------------------------------------------------------------------------------
function HideViewList()
{
    var activeElement = window.event.toElement;
    if( spnViewList == activeElement || spnViewList.contains(activeElement) )
        return;
    if( btnDropViewList == activeElement || btnDropViewList.contains(activeElement) )
        return;
    btnDropViewList.click();
}
// --------------------------------------------------------------------------------------
function ToggleDashboardList()
{
    try
    {
        var dropdownWrapper = document.getElementById("divDashboardDropdownWrapper")
        if (dropdownWrapper.style.display == "inline")
        {
            // activate the last focused object
            g_FocusedObject.setActive();
            dropdownWrapper.style.display = "none";
        } 
        else 
        {
            dropdownWrapper.style.display = "inline";
            dropdownWrapper.setActive();
        }
    }
    catch(err)
    {
        HANDLE_ERROR("ToggleDashboardList", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function HideDashboardList()
{
    var activeElement = window.event.toElement;
    if( divDashboardDropdownWrapper == activeElement || divDashboardDropdownWrapper.contains(activeElement) )
        return;
    if( btnDropDashboardList == activeElement || btnDropDashboardList.contains(activeElement) )
        return;
    //btnDropDashboardList.click();
    divDashboardDropdownWrapper.ondeactivate = null;
    divDashboardDropdownWrapper.style.display = "none";
    // activate the last focused object
    g_FocusedObject.setActive();
}
// --------------------------------------------------------------------------------------
function OnViewSelect(elem)
{
    spnCurrentView.innerText = elem.innerText;
    btnDropViewList.click();
    var AlarmTrend = document.getElementById('AlarmTrend');
    if (AlarmTrend)
        AlarmTrend.Content.EATView.ClearFilters();
}
// Activity Creation
// --------------------------------------------------------------------------------------
function DropNewActList()
{
    if( btnNewActDropList.selected )
    {
        g_FocusedObject = btnNewActDropList;
        g_FocusedObject.setActive();
        g_FocusedObject.ondeactivate = HideNewActDropList;
        NewActOptionsList.style.visibility = "visible";
    }
    else
    {
        NewActOptionsList.style.visibility = "hidden";
    }
}
function HideNewActDropList()
{
    var activeElement = window.event.toElement;
    if( btnNewActDropList == activeElement || btnNewActDropList.contains(activeElement) )
        return;
    if( NewActOptionsList == activeElement || NewActOptionsList.contains(activeElement) )
        return;
    btnNewActDropList.selected = false;
    NewActOptionsList.style.visibility = "hidden";
}
// --------------------------------------------------------------------------------------
function HandleActivate(t)
{
    var ae = window.event.fromElement;
    if( t == ae || t.contains(ae) )
        return;
    if( t.id == "Table" )
        g_bTableIsActive = true;
    else if (t.id == "AreaTable")
        g_bAreaTableIsActive = true;
    // Do not flush if the user click anywhere outside the Pane Container
    // (except buttons for column organizer, area pane, details pane and
    //  view configuration pane)
    if( !g_bFlush && !g_bPageNotFocused && !(PaneContainer == ae || PaneContainer.contains(ae) || AreaPane == ae || StatusPane == ae
        || AreaPane.contains(ae) || AreaPaneDropBorder == ae || VerticalResizePad == ae || Table == ae || Table.contains(ae)
        || btnAreaPane == ae || btnDetailsPane == ae || btnColumnOrganizer == ae || btnViewConfigPane == ae || btnStatusPane == ae) )
    {
        return;
    }
    if (g_SummaryType == SUMMARY_ALARM)
    {
        if(!g_bFlush && !g_bPageNotFocused && !(AlarmTrendPane == ae || AlarmTrendPane.contains(ae)))
        {
            return;
        }
    }
    g_bFlush = false;
    g_bPageNotFocused = false;
    // when the floating area pane is pinned, flush the table as well
    if( t.id == "AreaTable" && btnAreaPaneClose == ae)
        Table.Flush();
    t.Flush();
}
// --------------------------------------------------------------------------------------
function HandleDeactivate(t)
{
    var ae = window.event.toElement;
    if( t == ae || t.contains(ae) )
        return;
    if( t.id == "Table" )
        g_bTableIsActive = false;
    else if (t.id == "AreaTable")
        g_bAreaTableIsActive = false;
    // Do not flush if the user click anywhere outside the Pane Container
    // (except buttons for column organizer, area pane, details pane and
    //  view configuration pane)
    if( !(PaneContainer == ae || PaneContainer.contains(ae) || btnAreaPane == ae || !Page.contains(ae) || AreaPane == ae || StatusPane == ae
        || AreaPane.contains(ae) || AreaPaneDropBorder == ae || VerticalResizePad == ae || Table == ae || Table.contains(ae)
        || btnDetailsPane == ae || btnColumnOrganizer == ae || btnViewConfigPane == ae || btnStatusPane == ae) )
    {
        return;
    }
    if (g_SummaryType == SUMMARY_ALARM)
    {
        if(!(AlarmTrendPane == ae || AlarmTrendPane.contains(ae)))
        {
            return;
        }
    }
    t.Flush();
}
// --------------------------------------------------------------------------------------
function OnEnter_MenuItem(object)
{
    if (object.parentElement.disabled == true) return;
    object.className = "MENUITEM_HOVER";
}
// --------------------------------------------------------------------------------------
function OnLeave_MenuItem(object)
{
    if (object.parentElement.disabled == true) return;
    object.className = "";
}
// --------------------------------------------------------------------------------------
function CheckDrag()
{
    return window.event.srcElement.isContentEditable;
}
// --------------------------------------------------------------------------------------
function CheckSelect()
{
    return window.event.srcElement.isContentEditable;
}
// --------------------------------------------------------------------------------------
// Make sure a double-click on the table only sends you to a point-details page
// if it originated on the table body.
// TODO: Make sure double-click on an empty row doesn't do anything
function CheckDoubleClick()
{
    var src = window.event.srcElement;
    var valid = false;
    while( src != Table )
    {
        if( src.name == NAME_TBODY )
        {
            // If we actually went through a column element (ie, a visible column)
            // then continue
            valid = src != window.event.srcElement;
            break;
        }
        src = src.parentElement;
    }
    window.event.cancelBubble = !valid;
    g_bTable_DblClk = valid;
    return valid;
}
// --------------------------------------------------------------------------------------
function ShowColumnOrganizer(tbl)
{
    if( g_oOrganizer != null && !g_oOrganizer.closed )
    {
        g_oOrganizer.focus();
    }
    else
    {
		// Dynamically center column organizer window relative to summary window.
		// OR statement used for compatibility with both IE8 and IE9
		var windowWidth = window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth;		
		var windowHeight = window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight;
		var dialogWidth = 300;
		var dialogHeight = 330;
		var dialogX = window.screenLeft + (windowWidth - dialogWidth) / 2;
		var dialogY = window.screenTop + (windowHeight - dialogHeight) / 2;
		var dialogArg = "dialogHeight: " + dialogHeight + "px; dialogWidth: " + dialogWidth + "px; dialogLeft: " + dialogX 
					  + "px; dialogTop: " + dialogY + "px; edge: raised; help: no; scroll: no; status: no; " ;
        g_oOrganizer = window.showModelessDialog("Dialogs/ColumnOrganizer.htm",
                                                tbl,
                                                dialogArg);
        g_oOrganizer.attachEvent("onbeforeunload", OrganizerUnload);
    }
}
// --------------------------------------------------------------------------------------
function OrganizerUnload()
{
    if( g_oOrganizer != null )
    {
        g_oOrganizer.detachEvent("onbeforeunload", OrganizerUnload);
        g_oOrganizer = null;
    }
}
// --------------------------------------------------------------------------------------
function HandleKeyDown()
{
    var k = window.event.keyCode;
    switch(k)
    {
    case VK_PUP:
    case VK_PDN:
    case VK_AUP:
    case VK_ADN:
    case VK_HOME:
    case VK_END:
        window.event.cancelBubble = true;
        return false;
        break;
    default:
        break;
    }
    return true;
}
// --------------------------------------------------------------------------------------
// Print/Print Preview routines
// --------------------------------------------------------------------------------------
function HandlePrintAsReport()
{
    try
    {
        // get the windows default printer
        var defaultPrinter = TableSourceHelper.DefaultPrinter;
        if (!defaultPrinter)
        {
            var strNoPrinter = "Before you can print you need to install a printer.";
            window.external.application.TemporaryMessageZoneText = strNoPrinter;
            return;
        }
        // get the default printer's paper orientation
        var printerOrientation = TableSourceHelper.PrinterOrientation(defaultPrinter);
        // get the number of alarms in the table
        var tablesize = g_TDO.TableSize;
        // calculate the number of page required
        var pageNumber = 0;
        if( printerOrientation == 1 )
        {
            pageNumber = tablesize / 50;
        }
        else
        {
            pageNumber = tablesize / 35;
        }
        // prompt user for confirmation of printing a report contains of more than 10 pages
        if( pageNumber > 10 )
        {
            var strMessage = "The print request contains more than 10 pages, do you want to proceed with printing?";
            ConfirmMsg(strMessage, "GenerateReport", Table.GetState(), g_dateTimeFormat, SUMMARY_REPORT_PRINT, "", "");
            return;
        }
        // send request to data source
        g_TDO.GenerateReport(Table.GetState(), g_dateTimeFormat, SUMMARY_REPORT_PRINT);
    }
    catch( err )
    {
        HANDLE_ERROR("HandlePrintAsReport", err.number, err.description);
    }
}
function HandlePrintPreview()
{
    // send request to data source
    g_TDO.GenerateReport(Table.GetState(), g_dateTimeFormat, SUMMARY_REPORT_PREVIEW);
}
function HandleCsvExport()
{
    // send request to data source
    g_TDO.GenerateReport(Table.GetState(), g_dateTimeFormat, SUMMARY_REPORT_CSV);
}
function PrinterTooltip()
{
    try
    {
        // get the windows default printer
        var defaultPrinter = TableSourceHelper.DefaultPrinter;
        if (!defaultPrinter)
        {
            defaultPrinter = "No Printer";
        }
        btnPrintAsReport.title = "Print" + " (" + defaultPrinter + ")";
    }
    catch( err )
    {
        HANDLE_ERROR("PrinterTooltip", err.number, err.description);
    }
}
function SetWindowSize()
{
    var iBrowser = 1;
    //If we are being viewed in HMIWebBroswer adjust the size differently due
    //to the differences between the available realestate in Station and Internet Explorer
    if (g_Environment == iBrowser)
    {
        g_lDisplayWidth = document.body.offsetWidth - 25;
        g_lDisplayHeight = document.body.offsetHeight - 20;
    }
    else
    {
        g_lDisplayWidth = document.body.offsetWidth - 20; // Arbitrary constant to make sure we zoome (ISR65282)
        g_lDisplayHeight = document.body.offsetHeight;
    }
    if (g_lDisplayWidth == 0)
    {
        g_lDisplayWidth = screen.availWidth;
        g_lDisplayHeight = screen.availHeight;
    }
    if (g_SummaryType == SUMMARY_EVENTVIEW)
    {
        g_lDisplayHeight = document.body.offsetHeight + 40; // to offset height shift
        g_lDisplayWidth = document.body.offsetWidth - 20;
    }
    if ( g_SummaryType == SUMMARY_ACTIVITY || g_SummaryType == SUMMARY_BATCH || g_SummaryType == SUMMARY_PROCEDURE )
    {
        //setTableSize must be delayed a little such that automatic sizing and positioning doesn't interfere
        setTimeout("SetTableSize();", 500);
    }
}
function BackgroundPropChange()
{
    if ( event.propertyName == "style.zoom")
    {
        DelayedZoomSummary();
    }
}
// ISR68710 - This function is used for the OnResize event and when the zoom level
//			changes.  It delays the calling of the CPU-intensive ZoomSummary() method
//			for 50ms.  If it gets called again in those 50ms, it starts counting the
//			50ms again.  This prevents multiple calls on ZoomSummary() in rapid succession.
function DelayedZoomSummary()
{
    clearTimeout(g_ZoomTimer);
    g_ZoomTimer = window.setTimeout(ZoomSummary, 50);
}
function ZoomSummary()
{
    // If not zoom to fit and running in Station, just display at 1280x1024 (default) res
    // HMIWeb Browser doesn't have a zoom to fit concept so we always zoom to fit
    if (!window.external.application.zoomtofit && g_Environment == STATION_ENV)
    {
        if (g_SummaryType == SUMMARY_EVENTVIEW)
        {
            if (g_lDisplayWidth != 1278 || g_lDisplayHeight != 425)
            {
                g_lDisplayWidth = 1278;
                g_lDisplayHeight = 425;
            }
            else
                return;
        }
        else if (g_lDisplayWidth != 1278 || g_lDisplayHeight != 870)
        {
            g_lDisplayWidth = 1278;
            g_lDisplayHeight = 870;
        }
        else
            return;
    }
    else
    {
        // And don't resize if not necessary
        var oldDisplayWidth = g_lDisplayWidth;
        var oldDisplayHeight = g_lDisplayHeight;
        SetWindowSize();
        if (g_lDisplayWidth == oldDisplayWidth && g_lDisplayHeight == oldDisplayHeight )
            return;
        else if (g_lDisplayWidth < 20 || g_lDisplayHeight < 20)
        {
            g_lDisplayWidth = oldDisplayWidth;
            g_lDisplayHeight = oldDisplayHeight;
            return;
        }
        Page.style.zoom = 1.01;
    }
    // Size everything up appropriately
    var xDiff = g_lDisplayWidth - 1024;
    Background.style.pixelWidth = g_lDisplayWidth - 2;
    Background.style.pixelHeight = g_lDisplayHeight - 25;
    Page.style.pixelWidth = g_lDisplayWidth - 2;
    Page.style.pixelHeight = g_lDisplayHeight - 25;
    SetPaneContainerSize();
    SetTableSize();
    VerticalResizePad.max = (270 + xDiff > 270 ? 270 + xDiff : 270);
    if (g_SummaryType == SUMMARY_MESSAGE)
        HorizontalResizePad_BOTTOM.max = g_lDisplayHeight - 450;
    else
        HorizontalResizePad_BOTTOM.max = g_lDisplayHeight - 350;
    if (g_SummaryType == SUMMARY_SYSTEM_ALARM)
    	HorizontalResizePad_TOP.max = g_lDisplayHeight - 300;
    if (g_SummaryType == SUMMARY_ALARM)
        HPadAlarmTrend.max = g_lDisplayHeight - 510;
    // Details pane height check function is called on a delay otherwise it may not be picked up all the time
    window.setTimeout(CheckDetailsPaneSize, 50);
}
function CheckDetailsPaneSize()
{
    // Maintain minimum height of details pane on window resize to avoid buttons overlapping.
	if (DetailsPane.style.pixelHeight < g_iDetailPaneHeightMin)
	{
		var newposn = (HorizontalResizePad_BOTTOM.style.pixelTop) + (DetailsPane.style.pixelHeight) - (g_iDetailPaneHeightMin);
		Handle_PaneResize(HorizontalResizePad_BOTTOM, newposn);
	}
	//Resize Details Pane Drag header to it doesn't overlap with close button
	HorizontalResizePad_BOTTOM.style.pixelWidth = DetailsPane.style.pixelWidth - g_CloseButtonWidth;
	if (document.getElementById("divExtendedResizepadBOTTOM")) {
		divExtendedResizepadBOTTOM.style.pixelWidth = DetailsPane.style.pixelWidth - g_CloseButtonWidth;
	}
}
// --------------------------------------------------------------------------------------
// Prompt for user response routines (Station's Message Zone)
// --------------------------------------------------------------------------------------
var g_iGetResponseTimeout = 30000;	// 30 sec timeout
var g_iGetResponseInterval = 1000;	// 1 sec interval
var g_iGetResponseExecuted = 0;		// Number of times spent executing the timerGetResponse_Timer function
var g_bGetResponseExitLoop = false;
var g_strPrompt;
var g_strNextFunction;
var g_varArg1;
var g_varArg2;
var g_varArg3;
var g_varArg4;
var g_varArg5;
function ConfirmMsg(strPrompt, strNextFunction, varArg1, varArg2, varArg3, varArg4, varArg5)
{
    // Set some flags
    g_strPrompt = strPrompt;
    g_strNextFunction = strNextFunction;
    g_varArg1 = varArg1;
    g_varArg2 = varArg2;
    g_varArg3 = varArg3;
    g_varArg4 = varArg4;
    g_varArg5 = varArg5;
    g_iGetResponseExecuted = 0;
    g_bGetResponseExitLoop = false;
    // Start the timer
    window.setTimeout(timerGetResponse_Timer, 0);
}
function timerGetResponse_Timer()
{
    GetResponse();
    if (g_iGetResponseExecuted > g_iGetResponseTimeout/g_iGetResponseInterval)
    {
        CallNextRoutine(false);
    }
    if (!g_bGetResponseExitLoop)
    {
        // repeat call
        window.setTimeout(timerGetResponse_Timer, g_iGetResponseInterval);
    }
}
function CallNextRoutine(blnResponse)
{
    // stop timer
    g_bGetResponseExitLoop = true;
    // Clear the message zone
    window.external.application.MessageZoneText = "";
    if (blnResponse)
    {
        switch (g_strNextFunction)
        {
            case "SaveView":
                g_TDO.SaveView(g_varArg1, g_varArg2, g_varArg3, g_varArg4, g_varArg5);
                break;
            case "DeleteCurrentView":
                g_TDO.DeleteCurrentView();
                break;
            case "GenerateReport":
                g_TDO.GenerateReport(g_varArg1, g_varArg2, g_varArg3);
                break;
            case "RecoverAlarms":
                window.setTimeout(
                    function () {
                        btnAlarmOverflowAck.disabled = false;
                    }, 10000);
                window.external.application.RequestTask(180,7000,0,0,0,"");
                break;
            case "ResetView":
                var AlarmTrend = document.getElementById('AlarmTrend');
                if (AlarmTrend)
                    AlarmTrend.Content.EATView.ClearFilters();
                if (g_SummaryType == SUMMARY_SYSTEM_ALARM)
                    ClearSystemDashboardFilter();
                g_TDO.ResetFilters();
                break;
            default:
                break;
        }
    }
    else
    {
        switch (g_strNextFunction)
        {
            case "RecoverAlarms":
                btnAlarmOverflowAck.disabled = false;
                break;
            default:
                break;
        }
    }
}
// --------------------------------------------------------------------------------------
// Summary's focusing routines
// --------------------------------------------------------------------------------------
function FocusObjectOnClick()
{
    // If nothing selected... (while loading)
    if (!g_FocusedObject)
        return;
    if(!g_bGenerateEvent_onclick)
    {
        g_bFlush=false;
        if (g_bPageNotFocused)
            g_bFlush=true;
        g_FocusedObject.setActive();
    }
    else
    {
        g_bGenerateEvent_onclick=false;
    }
}
function FocusObjectOnDblClick()
{
    window.event.cancelBubble = !g_bTable_DblClk;
    g_bTable_DblClk=false;
}
function FocusObjectOnFocusOut()
{
    if (!g_bPageNotFocused)
    {
        g_bPageNotFocused=true;
        if ( g_bPageInitialized )
        {
            if ( Page.contains(window.event.toElement) )
                g_bPageNotFocused=false;
        }
    }
}
// --------------------------------------------------------------------------------------
// Routines used for the system status display
// --------------------------------------------------------------------------------------
var g_strFTEPage  = "ftestatusdisplay.htm";
function SMLinkClick()
{
    try
    {
        TableSourceHelper.SMLinkClick;
    }
    catch(e)
    {
        // log an exception 
        window.external.logmessage("sysSystemSummary.htm | SMLinkClick | Exception occurred, " + e.message);
    }
}
function FTELinkClick()
{
    try
    {
        if( String(btnDropAreaPane.selected) == "true" )
            ToggleAreaPane();
        external.CurrentPage = g_strFTEPage;
    }
    catch(e)
    {
        HANDLE_ERROR("FTELinkClick", err.number, err.description);
    }
}
function CheckSystemLinks()
{
    var pad = 0;
    var strAddedHTML = "";
    if( TableSourceHelper.SMLinkDefined )
    {
        pad += 22;
        strAddedHTML += '<div onclick="SMLinkClick()" class="SYSTEM_LINK">System Management</div>';
    }
    if( TableSourceHelper.FTELinkDefined  )
    {
        pad += 22;
        strAddedHTML += '<div onclick="FTELinkClick()" class="SYSTEM_LINK">FTE Status</div>';
    }
    if ( strAddedHTML != "" )
    {
        SystemLinksWrapper.innerHTML += strAddedHTML;
    }
    if( pad > 0 )
    {
        AreaTable.style.borderBottom = "2px groove";
        AreaTableWrapper.style.paddingBottom = pad + "px";
        SystemLinksWrapper.style.height = pad + "px";
        SystemLinksWrapper.style.display = "inline";
        AreaPane.style.pixelHeight = PaneContainer.style.pixelHeight;
    }
}
function OnFilterChange()
{
    try
    {
        if( window.event.propertyName == 'filter' )
        {
            var areaBinding = AreaTable.Binding.DataObjects.Item(0);
            var entFilter = divEntityFilter.filter;
            if (g_TDO.EntityFilterList != entFilter)
                g_TDO.EntityFilterList = entFilter;
            if (areaBinding.SelectionListArray != entFilter || !g_bLocationPaneInitialised)
            {
                areaBinding.SelectionListArray = entFilter;
                g_bLocationPaneInitialised = true;
            }
            var AlarmTrend = document.getElementById('AlarmTrend');
            if (AlarmTrend)
            {
                var strAlarmTrendFilter = AlarmTrend.Content.EATView.EntityFilter;
                var strLocationFilter = g_TDO.EntityFilterString;
                if (strAlarmTrendFilter.charAt(strAlarmTrendFilter.length - 1) == ';')
                    strAlarmTrendFilter = strAlarmTrendFilter.substr(0, strAlarmTrendFilter.length - 1);
                if (strLocationFilter.charAt(strLocationFilter - 1) == ';')
                    strLocationFilter = strLocationFilter.substr(0, strLocationFilter - 1);
                if (strLocationFilter != strAlarmTrendFilter)
                    AlarmTrend.Content.EATView.EntityFilter = strLocationFilter;
            }
            // check if system dashboard selection needs to be cleared
            var dashboardIframe = document.getElementById("dashboardiframe");
            if (dashboardIframe)
            {
                // check if location tree selection and dashboard selection are the same
                var isSelectionSame = true;
                var areaSelectionArray = areaBinding.SelectionListString.split(";");
                areaSelectionArray.sort();
                var dashSelectionArray = divEntityFilter.dashboardFilterString.split(";");
                dashSelectionArray.sort();
                if (areaSelectionArray.length != dashSelectionArray.length)
                {
                    isSelectionSame = false;
                }
                else
                {
                    var i = 0;
                    for (i = 0; isSelectionSame && (i < areaSelectionArray.length); ++i) 
                    {
                        if (areaSelectionArray[i] != dashSelectionArray[i])
                        {
                            isSelectionSame = false;
                            break;
                        }
                    }
                }
                if (!isSelectionSame && dashboardIframe.contentWindow.ClearCurrSelection)
                {
                    dashboardIframe.contentWindow.ClearCurrSelection(false);
                }
            }
        }
    }
    catch( err )
    {
        HANDLE_ERROR("OnFilterChange", err.number, err.description);
    }
}
function ChangeDashboard(strDashboardFullItemName, strDashboardDisplayFile)
{
    try
    {
        var bSelectingSystemOverviewDashboard = false;
        // strip out parent points to get entity name only
        var strDashboardEntityName = strDashboardFullItemName;
        var pos = strDashboardEntityName.lastIndexOf("/");
        if (pos == 0)
        {
            // last occurrence of slash is the first character, so it must be the sysalarm group model point
            bSelectingSystemOverviewDashboard = true;
            strDashboardEntityName = g_sOverviewDashboardTitle;
        }
        else if (pos < (strDashboardEntityName.length - 1))
        {
            strDashboardEntityName = strDashboardEntityName.substring(pos+1);
        }
        spnCurrentDashboard.innerHTML = strDashboardEntityName;
        // Set the display file name of the current selected dashboard
        if (bSelectingSystemOverviewDashboard)
            strDashboardDisplayFile = g_sOverviewDashboardFilename;
        window.external.logMessage("ChangeDashboard() current dashboard display is " + strDashboardDisplayFile);
        spnCurrentDashboard.title = strDashboardDisplayFile;    
        if ( (spnCurrentDashboard.title == "about:blank") || (spnCurrentDashboard.title == "") )
            return;
        var fullDisplayPath = window.external.parent.FindFile(spnCurrentDashboard.title);
        if ((fullDisplayPath == null) || typeof(fullDisplayPath) != "string" || (fullDisplayPath.length == 0))
        {
            document.getElementById("dashboardiframe").contentWindow.location.replace("about:blank");
            HANDLE_ERROR("ChangeDashboard", "-", "Dashboard display path resolution failed for " + spnCurrentDashboard.title);
            window.external.showCallout(document.getElementById("btnDropDashboardWrapper"), spnCurrentDashboard.title + " cannot be found.", CALLOUT_LEVEL_ERROR, CALLOUT_TIMEOUT);
        }
        else
        {
            var iframe = document.getElementById("dashboardiframe");
            if (iframe.contentWindow.location == fullDisplayPath)
                return;
            iframe.contentWindow.location.replace(fullDisplayPath);
            // filter location tree to selected dashboard system alarm group, 
            // skip for overview dashboard as the dashboard itself will handle the filtering
            if (!bSelectingSystemOverviewDashboard)
            {
                divEntityFilter.dashboardFilterString = strDashboardFullItemName;
            }
            g_DashboardHistoryArray.push(strDashboardFullItemName);
            if (g_DashboardHistoryArray.length > MAX_DASHHISTORY_SIZE)
            {
                g_DashboardHistoryArray.shift();
            }
            UpdateDashboardBackButtonState();
            // save new dashboard to persistence if needed
            if (g_TDO.ViewPersistenceEnabled == true)
            {
                var view_bag = document.getElementById(PROP_Cache_View);
                view_bag.setAttribute(PROP_DashboardGroup, strDashboardFullItemName);
            }
        }
    }
    catch( err )
    {
        HANDLE_ERROR("ChangeDashboard", err.number, err.description);
    }
}
function UpdateDashboardBackButtonState()
{
    var backButtonElem = document.getElementById("btnDashboardBack");
    if (backButtonElem)
    {
        if ((g_DashboardHistoryArray.length >= 2) && (backButtonElem.className != "dashboardBackEnabled"))
        {
            backButtonElem.className = "dashboardBackEnabled";
            backButtonElem.onmouseover = function() { overEffect('btnDashboardBack', 'dashboardBackEnabled'); };
            backButtonElem.onmouseout = function() { outEffect('btnDashboardBack', 'dashboardBackEnabled', ''); };
        }
        else if ((g_DashboardHistoryArray.length < 2) && (backButtonElem.className != "dashboardBackDisabled"))
        {
            backButtonElem.className = "dashboardBackDisabled";
            backButtonElem.onmouseover = function() {};
            backButtonElem.onmouseout = function() {};
        }
    }
}
function OnDashboardBack()
{
    if (g_DashboardHistoryArray.length < 2)
    {
        UpdateDashboardBackButtonState();
        return;
    }
    try
    {
        var dashboardDropdownElem = document.getElementById("DashboardDropdown");
        if (dashboardDropdownElem)
        {
            // Pop once to remove the current dashboard(top of stack), then pop a second time to retrieve the previous dashboard.
            // The subsequent dashboard navigation will push the previous dashboard back onto the top of the stack as the new current dashboard.
            g_DashboardHistoryArray.pop();
            var previousDashboardItemName = g_DashboardHistoryArray.pop();
            UpdateDashboardBackButtonState();
            var dropdownTDO = dashboardDropdownElem.Binding.DataObjects.Item(0);
            dropdownTDO.SelectionListString = previousDashboardItemName;
        }
    }
    catch (err)
    {
        HANDLE_ERROR("OnDashboardBack", err.number, err.description);
    }
}
function RecoverAlarms()
{
    g_FocusedObject = btnAlarmOverflowAck;
    btnAlarmOverflowAck.disabled = true;
    outEffect('btnAlarmOverflowAck', 'alarmOverFlowAck alarmOverFlowAck', '');
    var strMessage = "Warning: This will recover all alarms, alerts and system alarms, causing load on the system.  Continue?";
    ConfirmMsg(strMessage, "RecoverAlarms", "", "", "", "", "");
}
function ResetViewWarning()
{
    g_FocusedObject = btnResetView;
    var strMessage = "Warning: This will clear all filters and remove any visible columns that are not present in the default view.  Continue?";
    ConfirmMsg(strMessage, "ResetView", "", "", "", "", "");
}
function SetFilter(column, filter)
{
    g_TDO.SetFilter(true, column, filter);
}
function ModifyFilter(column, filter)
{
    g_TDO.SetFilter(false, column, filter);
}
function GetEntityFilter()
{
    if (g_TDO != null)
        return g_TDO.EntityFilterString;
    else
        return "";
}
function GetAlarmTrendPersistedSetting(setting)
{
    if (setting == "LabelsVisible")
        return g_bAlarmTrendLabelsVisible;
    if (setting == "TimePeriod")
        return g_nAlarmTrendTimePeriod;
}
function GetAckPageDisabled()
{
    return btnAckPage.readonly;
}
function HighlightAlarmTracker(cookie)
{
    var AlarmTrend = document.getElementById('AlarmTrend');
    if (AlarmTrend && AlarmTrend.Content && AlarmTrend.Content.EATView)
    {
        if (cookie != 0)
        {
            AlarmTrend.Content.EATView.HighlightAlarm(cookie);
        }
        else
        {
            AlarmTrend.Content.EATView.ClearAlarmHighlight();
        }
    }
}
function OnAlarmTrackerFocusOut()
{
    var AlarmTrend = document.getElementById('AlarmTrend');
    if (AlarmTrend)
        AlarmTrend.Content.EATViewCodeBehind.CloseContextMenu();
}
function ClearFilters()
{
    g_TDO.ClearFilters();
    var AlarmTrend = document.getElementById('AlarmTrend');
    if (AlarmTrend)
        AlarmTrend.Content.EATView.ClearFilters();
    if (g_SummaryType == SUMMARY_SYSTEM_ALARM)
        ClearSystemDashboardFilter();
}
function ClearSystemDashboardFilter()
{
    try
    {
        if (g_SummaryType == SUMMARY_SYSTEM_ALARM)
        {
            var iframe = document.getElementById("dashboardiframe");
            var dashboardWindow = iframe.contentWindow;
            dashboardWindow.ClearCurrSelection(false);
        }
    }
    catch ( err )
    {
        HANDLE_ERROR("ClearSystemDashboardFilter", err.number, err.description);
    }
}
function GetURLParam(paramName)
{
    paramName = paramName.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
    var regexS = "[\\?&]" + paramName + "=([^&#]*)";
    var regex = new RegExp(regexS, 'i');  // do case insensitive regex search
    var results = regex.exec(window.location.href);
    if (results == null)
        return "";
    else
        return results[1];
}
function SetSelectedAlarmTracker(title, id)
{
    if (title == "")
        title = "Default alarm tracker";
    divTrackerName.innerText = title + " ";
    lnkTrackerConfig.href = "page://sysalmtrackerconfig?hscrecord=" + id;
    if (id != 0 && window.external.Parent.RuntimeStatus.SecurityLevel >= 4)
    {
        lnkTrackerConfig.disabled = false;
        lnkTrackerConfig.onclick = null;
    }
    else
    {
        lnkTrackerConfig.disabled = true;
        lnkTrackerConfig.onclick = function() { return false; };
    }
}
function overEffect(id , className)
{
    var divId = document.getElementById(id);
    var classOver = className + "_ovr";
    divId.className = classOver;
}
function outEffect(id, className, containerId)
{
    var divId = document.getElementById(id);
    var conDivId = document.getElementById(containerId);
    var classNormal;
    if (containerId != '')
    {
        if (conDivId.style.display == 'none')
        {
            classNormal  = className;
            divId.className = classNormal;
        }
    }
    else
    {
        classNormal = className;
        divId.className = classNormal;
    }
}
function HighlightPaneButton(buttonName, className, paneName)
{
    if (window.event.propertyName == "selected")
    {
        var button = document.getElementById(buttonName);
        if (String(button.selected) == "true")
        {
            overEffect(buttonName, className);
        }
        else
        {
            outEffect(buttonName, className, paneName);
        }
    }
}
function DTLocalize(value, dateTimeFormat)
{
    try 
    {
        return DTFormat.Localize(value, dateTimeFormat);
    } 
    catch(e) 
    {
        return String(value);
    }
}
function LiveValueLocalize(value, pvformat, decimalWidth, dateTimeFormat)
{
    try 
    {
        if (!isNaN(value) && (pvformat == -1))
            return "";
        return DTFormat.LocalizeLiveValue(value, pvformat, decimalWidth, dateTimeFormat);
    } 
    catch(e) 
    {
        return String(value);
    }
}
function SetActivityCommandButtons(validCommands)
{
    if ( (validCommands & ACT_COMMAND_GO) != 0) {
        btnGo.readonly = false;
        btnGo.className="btnGoEnabled";
    } else {
        btnGo.readonly = true;
        btnGo.className="btnGoDisabled";
    }
    if ( (validCommands & ACT_COMMAND_HOLD) != 0) {
        btnHold.readonly = false;
        btnHold.className="btnHoldEnabled";
    } else {
        btnHold.readonly = true;
        btnHold.className="btnHoldDisabled";
    }
    if ( (validCommands & ACT_COMMAND_STOP) != 0) {
        btnStop.readonly = false;
        btnStop.className="btnStopEnabled";
    } else {
        btnStop.readonly = true;
    btnStop.className="btnStopDisabled";
    }
    if ( (validCommands & ACT_COMMAND_ABORT) != 0) {
        btnAbort.readonly = false;
        btnAbort.className="btnAbortEnabled";
    } else {
        btnAbort.readonly = true;
        btnAbort.className="btnAbortDisabled";
    }
    if ( (validCommands & ACT_COMMAND_REMOVE) != 0) {
        btnRemove.readonly = false;
        btnRemove.className="btnRemoveEnabled";
    } else {
        btnRemove.readonly = true;
        btnRemove.className="btnRemoveDisabled";
    }
}
function UpdateActDetailsPane()
{
    //check if the creation UI should be closed
    if (g_CloseCreationUIOnUpdate && g_CreationUIOn)
    {
        CloseCreationPane();
        return;
    }
    //do nothing if the details pane is not up or creation UI is invoked
    if (DetailsPane.style.display == "none" || g_CreationUIOn)
    {
        return;
    }
    try {
        //otherwise check whether there is a selected activity and upddate visibility
        bstrActivityTagname = DisplayDataRepository.GetValue("ActivityPnt");
        var paneTitleObject = document.getElementById("DetailsPaneHeader_details");
        if ( bstrActivityTagname != "" )
        {    
            ActCreateAndDataUI.style.visibility = "visible";
            if ( g_DataUIInErrorMode ){
                divDetailsSidePanel.style.visibility = "hidden";
            } else {
                divDetailsSidePanel.style.visibility = "visible";
            }
            paneTitleObject.innerHTML = "Batch ID: ";
            if ( g_SelectedActName != "" )
            {
                paneTitleObject.innerHTML += g_SelectedActName;
            }
            paneTitleObject.innerHTML += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Public Name: ";
            if ( g_SelectedActPublicName != "" )
            {
                paneTitleObject.innerHTML += g_SelectedActPublicName;
            }
            SetActCreateUIConfig();
        }
        //if no activity point to bind to then clear the details pane
        else
        {
            paneTitleObject.innerHTML = "";
            g_SelectedActName = "";
            g_SelectedActPublicName = "";
            ActCreateAndDataUI.style.visibility = "hidden";
            divDetailsSidePanel.style.visibility = "hidden";
        }
    } catch( err )
    {
        HANDLE_ERROR("UpdateActDetailsPane", err.number, err.description);
    }
}
function ClearActDetailsPane()
{
    UnbindActivity();
    g_SelectedActName = "";
    g_SelectedActPublicName = "";
    UpdateActDetailsPane();
}
//translate the code summaryscripts uses to denote summary type to the code used by EBM Creation Data UI
//to set the host environment
function SummaryType_ScriptToCreationUICoding(SummaryType_ScriptCoding)
{
    switch (SummaryType_ScriptCoding)
    {
        case SUMMARY_ACTIVITY:
            return EBMCREATIONDATAUI_ACTIVITYSUMMARY;
        case SUMMARY_BATCH:
            return EBMCREATIONDATAUI_BATCHSUMMARY;
        case SUMMARY_PROCEDURE:
            return EBMCREATIONDATAUI_PROCEDURESUMMARY;
        default:
            return EBMCREATIONDATAUI_POPUP;
    }
}
function ActCloseBottomPane()
{
    if (g_CreationUIOn) {
        CloseCreationPane();
    } else {
        ToggleDetailsPane();
    }
}
function OpenCreationUI()
{
    g_CreationUIOn = true;
    //deselect activity on grid
    g_TDO.SelectActivity("");
    //Reset the DataUIErrorMode state
    g_DataUIInErrorMode = false;
    //store details pane current visibility state
    if (DetailsPane.style.display != "none") { 
        g_BeforeCreationUIDetailsPaneShowing = true;
    } else {
        g_BeforeCreationUIDetailsPaneShowing = false;
    }
    //disable all creation related buttons on the display
    if ( g_SummaryType == SUMMARY_ACTIVITY )
    {
        btnNewActDropList.selected = false;
        NewActOptionsList.style.visibility = "hidden";
        btnNewActDropList.readonly = true;
        imgNewActDropList.style.display = "none";
        btnNewActDropList.className = "btnNewActDisabledList";
    }
    else if ( g_SummaryType == SUMMARY_BATCH )
    {
        btnNewAct.readonly = true;
        btnNewAct.className = "btnNewBatchDisabled";
    }
    else if ( g_SummaryType == SUMMARY_PROCEDURE )
    {
        btnNewAct.readonly = true;
        btnNewAct.className = "btnNewProcedureDisabled";
    }
    //disable details pane show/hide button
    btnDetailsPane.disabled = true;
    btnDetailsPane.className='toolBarBtnDisabled detailsPaneDisabled';
    btnDetailsPane.title = "Details pane disabled";
    //update the details pane size, title and visibility
    if (HorizontalResizePad_BOTTOM.style.pixelTop < HorizontalResizePad_BOTTOM.min)
    {
        g_LastActDetailsPaneSize = GridPane.style.pixelTop + GridPane.style.pixelHeight - DetailsPane.style.pixelHeight - g_lPanePadding;
    }
    else
    {
        g_LastActDetailsPaneSize = HorizontalResizePad_BOTTOM.style.pixelTop;
    }
    DetailsPaneHeader_details.style.visibility = "hidden";
    if (g_CreationFilter == BATCH_ONLY) {
        DetailsPaneHeader_BatchCreation.style.visibility = "visible";
    } else if (g_CreationFilter == PROCEDURE_ONLY) {
        DetailsPaneHeader_ProcedureCreation.style.visibility = "visible";
    }
    btnCloseDetailsPane.title = "Close";
    DetailsPane.style.display = "inline";
    HorizontalResizePad_BOTTOM.style.display = "inline";
    divExtendedResizepadBOTTOM.style.display = "inline";
    Handle_PaneResize(HorizontalResizePad_BOTTOM, (HorizontalResizePad_BOTTOM.max + HorizontalResizePad_BOTTOM.min)/2);
    ActCreateAndDataUI.style.visibility = "visible";
    divDetailsSidePanel.style.visibility = "hidden";
    tdDetailsSidePanel.style.width = "0%";
    //set various config parameters on the UI object
    SetActCreateUIConfig();
}
function NewBatch()
{
    g_CreationFilter = BATCH_ONLY;
    OpenCreationUI();
}
function NewProcedure()
{
    g_CreationFilter = PROCEDURE_ONLY;
    OpenCreationUI();
}
//closing the creation UI
function CloseCreationPane()
{
    //navigate away from the creation UI to force clean up of creation UI resources
    var slObject = document.getElementById("ActCreateAndDataUI");
    if ( slObject != null ) 
    {
        slObject.Content.DataPage.RequestNavigate("DefaultView");
    }
    //Enable Creation button
    if ( g_SummaryType == SUMMARY_ACTIVITY )
    {
        btnNewActDropList.readonly = false;
        imgNewActDropList.style.display = "inline";
        btnNewActDropList.className = "btnNewActDList";
    }
    else if ( g_SummaryType == SUMMARY_BATCH )
    {
        btnNewAct.readonly = false;
        btnNewAct.className = "btnNewBatch";
    }
    else if ( g_SummaryType == SUMMARY_PROCEDURE )
    {
        btnNewAct.readonly = false;
        btnNewAct.className = "btnNewProcedure";
    }
    //Reenable details pane hiding and showing
    btnDetailsPane.disabled = false;
    btnDetailsPane.className='toolBarBtn detailsPaneEnabled';
    //revert the details pane size, title and visibility
    DetailsPaneHeader_details.style.visibility = "visible";
    DetailsPaneHeader_BatchCreation.style.visibility = "hidden";
    DetailsPaneHeader_ProcedureCreation.style.visibility = "hidden";
    btnCloseDetailsPane.title = "Hide details pane";
    tdDetailsSidePanel.style.width = "250px";
    //check if there is a selection on the grid or if details pane WAS shown before
    if (g_SelectionOnGrid || g_BeforeCreationUIDetailsPaneShowing)
    {
        btnDetailsPane.selected = true;        
        btnDetailsPane.title = "Hide details pane";
        Handle_PaneResize(HorizontalResizePad_BOTTOM, g_LastActDetailsPaneSize);
    }
    else
    {
    	btnDetailsPane.selected = false;
        btnDetailsPane.title = "Show details pane";
        HorizontalResizePad_BOTTOM.style.display = "none";
        divExtendedResizepadBOTTOM.style.display = "none";
        DetailsPane.style.display = "none";
        GridPane.style.pixelHeight += DetailsPane.style.pixelHeight + g_lPanePadding;
        g_FocusedObject = Table;
    }
    g_CreationUIOn = false;
    //clear details pane binding if no selection on grid
    if (!g_SelectionOnGrid) {
        UnbindActivity();
    }
    //update the details pane
    UpdateActDetailsPane();
}
//event handler for the ActivityCreated event fired from the Creation UI
function SelectActivityOnGrid(sender, args)
{
    //get the result of the creationUI
    var activityName = args.ActivityPointName;
    //request the selection of tagname is not null or empty
    if ( (activityName != null) && (activityName != "") )
    {
        g_TDO.SelectActivity(activityName);
    }    
}
//To hndle the error mode event from the DataUI
function ActDataUIErrorMode(sender, args)
{
    //get the result of the creationUI
    var result = args.InErrorMode;
    //Hide the Ride hand bar
    if (result == 0) {
        g_DataUIInErrorMode = false;
    } else {
        g_DataUIInErrorMode = true;
    }
    //force the dataUI to rerender
    UpdateActDetailsPane();
}
//used to set SilverLight Object configurations
function SetActCreateUIConfig()
{
    try
    {
        var slObject = document.getElementById("ActCreateAndDataUI");
        //set summary type and event handlers on the silverlight object
        slObject.Content.DataPage.SetHostingEnvironment(SummaryType_ScriptToCreationUICoding(g_SummaryType));
        //also set an event handler
        slObject.Content.DataPage.DataUIError = ActDataUIErrorMode;
        if ( g_CreationUIOn )
        {
            slObject.Content.DataPage.RequestNavigate("CreationUIView");
            g_DataUIIsLoaded = false;
            //set an event handlers
            slObject.Content.DataPage.CreationUIClosed = CloseCreationPane;
            slObject.Content.DataPage.ActivityCreated = SelectActivityOnGrid;
            //set other batch/procedure filtering
            slObject.Content.DataPage.SetCreationUIBaselineFilter(g_CreationFilter, "", "", "");
        }
        else
        {
            //make sure the SL page navigate is only done once first time
            if (!g_DataUIIsLoaded) {
                slObject.Content.DataPage.RequestNavigate("DataUIView");
                g_DataUIIsLoaded = true;
            }
            //grab activitytagname
            bstrActivityTagname = DisplayDataRepository.GetValue("ActivityPnt");
            slObject.Content.DataPage.SetActivityName(bstrActivityTagname);
        }
    }
    catch (err){} //do nothing
}
// Returns the currently selected row relative to the top visible row,
//  -1 if no row is selected, -2 if no visible row is selected
function GetSelectedRow()
{
	var nSelectedRow = Table.SelectedRowIndex;
	var nTableSize = Table.ViewSize;
	var nTablePosn = Table.ViewPosn;
	// Calculate last clicked row's position in the currently displayed window and get fields
	nSelectedRow = nSelectedRow - nTablePosn;
	// See if it's correct
	if (nSelectedRow >= 0 && nSelectedRow < nTableSize)
	{
		if (Table.Fields.Item("Selected").GetValue(nSelectedRow))
			return nSelectedRow;
	}
	// Check all rows for selection then
	nSelectedRow = 0;
	for (var i = 0; i < nTableSize; i++)
	{
		if (Table.Fields.Item("Selected").GetValue(i))
		{
			return i;
		}
	}
	return NO_VISIBLE_ROW_SELECTED;
}
function GetLocationSelectedRow()
{
	var nSelectedRow = AreaTable.SelectedRowIndex;
	var nTableSize = AreaTable.ViewSize;
	var nTablePosn = AreaTable.ViewPosn;
	// Calculate last clicked row's position in the currently displayed window and get fields
	nSelectedRow = nSelectedRow - nTablePosn;
	// See if it's correct
	if (nSelectedRow >= 0 && nSelectedRow < nTableSize)
	{
		if (AreaTable.Fields.Item("Selected").GetValue(nSelectedRow))
			return nSelectedRow;
	}
	// Check all rows for selection then
	nSelectedRow = 0;
	for (var i = 0; i < nTableSize; i++)
	{
		if (AreaTable.Fields.Item("Selected").GetValue(i))
		{
			return i;
		}
	}
	return NO_VISIBLE_ROW_SELECTED;
}
//Used by the batch summary to select the currently active activity point
function getActivityPointFromActivitySummary()
{
	return DisplayDataRepository.GetValue("ActivityPnt");
}
//Used by the message summary to select the currently active activity point
function getActivityPointFromNotificationSummary()
{
    return (Table.Fields.Item("SourceTagName").GetValue(GetSelectedRow()));
}
// Invoke the Procedure Explorer using the currently passed in source tag name
function InvokeProcedureExplorer(sourceTagName)
{
    try
    {
        window.external.application.InvokeProcedureExplorer(sourceTagName, 0);
    }
    catch(err)
    {
        window.external.logMessage("InvokeProcedureExplorer(): Error - " + err.message);
    }
}
function InvokeMessageDisplayForBatch() {
    try {
        var filterCols = ['BatchID'];
        var filterVals = [g_SelectedActName];
        window.external.application.InvokeMessages(true, filterCols, filterVals, true, 0); 
    }
    catch (err) {
        window.external.logMessage("InvokeMessageDisplay(): Error - " + err.message);
    }
}
//
function RetrieveFiltersFromURL()
{
    try
    {
		// Dashboard
		if( g_SummaryType == SUMMARY_SYSTEM_ALARM )
		{
			var rawArgument = GetURLParam("Dashboard");
			g_DashboardUrlArgument = decodeURI(rawArgument);
		}
		// Equipment Filter
		var rawEquipmentArgument = GetURLParam("Equipment");
		g_EquipmentFilter = decodeURI(rawEquipmentArgument);
		if (g_EquipmentFilter)
		{
			g_PersistanceSaveDisabled = true;
		}
		// Asset Filter
		if ((g_SummaryType == SUMMARY_ALARM) || (g_SummaryType == SUMMARY_MESSAGE) )
		{
			var rawAssetArgument = GetURLParam("Asset");
			g_AssetUrlArgument = decodeURI(rawAssetArgument);
			if (g_AssetUrlArgument)
			{
				g_PersistanceSaveDisabled = true;
			}
		}
		// Multi Filter	
		if(g_SummaryType == SUMMARY_MESSAGE)
		{
			var filterJson = RetrieveMessageFilter();
			if(filterJson)
			{
				g_PersistanceSaveDisabled = true; //don't persist filter
				var filterObj;
				try {
					filterObj = jQuery.parseJSON(filterJson);
				}
				catch (err)
				{
					window.external.logMessage("error parsing filter - " + err.message);
					return;
				}
				if(!filterObj)
				{
					window.external.logMessage("could not retrieve filter obj");
					return;
				}
				var msgFilters = filterObj['filters'];
				if(!msgFilters)
				{
					window.external.logMessage("could not retrieve filters");
					return;
				}
				for(var i = 0; i < msgFilters.length; i++)
				{
					var filterName = msgFilters[i].filter;
					var filterVal = msgFilters[i].value;
					if(filterName && filterVal)
					{
						g_FilterCols.push(filterName);
						g_FilterVals.push(filterVal);
					}
				}
				var selectFirst = filterObj.selectFirst;
				if(selectFirst)
				{
					g_SelectFirstRow = selectFirst;
				}
				var clearPersisted = filterObj.clearPersistedFilters;
				if(clearPersisted)
				{
					g_ClearPersistedFilter = clearPersisted;
				}
            }
		}
	}
	catch( err )
	{
	}
}
//
function SetFiltersFromURL()
{
    try
    {
	    if (g_EquipmentFilter)
		{
			g_TDO.EquipmentFilterList = g_EquipmentFilter;
		}
		if (g_AssetUrlArgument)
		{
            var areaBinding = AreaTable.Binding.DataObjects.Item(0);
            areaBinding.SelectionListString = g_AssetUrlArgument;
		}
		if ((g_FilterVals.length > 0) || g_ClearPersistedFilter)
		{
			g_TDO.SetMultiFilter(g_FilterCols, g_FilterVals, g_FilterVals.length, g_SelectFirstRow, g_ClearPersistedFilter);
		}
    }
    catch( err )
    {
        // If we couldn't access the table data object,
        // then we must wait until the page connector is ready.
        BindingBehavior.attachEvent("onpageconnectorready", SetFiltersFromURL);
        return;
    }
    BindingBehavior.detachEvent("onpageconnectorready", SetFiltersFromURL);
}
function SetDefaultDashboard()
{
    try
    {
        var dropdownTDO = document.getElementById("DashboardDropdown").Binding.DataObjects.Item(0);
        var strDefaultDashboard = g_TDO.DefaultDashboardName;
        window.external.logMessage("SetDefaultDashboard(): default dashboard full item name is <" + strDefaultDashboard + ">. Selecting...");
        dropdownTDO.SelectionListString = strDefaultDashboard;
    }
    catch (err)
    {
        window.external.logMessage("SetDefaultDashboard(): Error - " + err.message);
    }
}
function OnDashboardSelectionChange(detailsXML, bSelectionChange, bSelectedRowDataChange)
{
    try
    {
        var dropdownTDO = document.getElementById("DashboardDropdown").Binding.DataObjects.Item(0);
        // Close the dashboard dropdown list if it is open
        if (divDashboardDropdownWrapper.style.display == "inline")
            ToggleDashboardList();
        var dashboardItemName = dropdownTDO.SelectionListString;
        var currentDashboardDisplay = dropdownTDO.CurrentDashboardDisplay;
        ChangeDashboard(dashboardItemName, currentDashboardDisplay);
    }
    catch (err)
    {
        window.external.logMessage("OnDashboardSelectionChange(): Error - " + err.message);
    }
}
function OnPauseClicked()
{
    try
    {
        if (btnPause.readonly == false)
        {
            divPaused.style.display = 'inline'; 
            btnResume.readonly = false; 
            btnResume.className='resumeEnabled'; 
            btnPause.className='pauseDisabled'; 
            btnPause.readonly=true; 
            g_TDO.PauseUpdates();
        }
    }
    catch( err )
    {
        HANDLE_ERROR("OnPauseClicked", err.number, err.description);
    }
}
function onResumeClicked()
{
    try
    {
        if (btnResume.readonly == false)
        {
            divPaused.style.display = 'none'; 
            btnPause.className='pauseEnabled'; 
            btnResume.className='resumeDisabled'; 
            btnPause.readonly=false; 
            btnResume.readonly=true; 
            g_TDO.ResumeUpdates();
        }
    }
    catch( err )
    {
        HANDLE_ERROR("onResumeClicked", err.number, err.description);
    }
}
function OnAckPageStateChange()
{
    if( window.event.propertyName == "readonly" )
    {
        if (btnAckPage.readonly == true)
        {
            btnAckPage.className = "AcknowledgePageDisabled";
        }
        else if (btnAckPage.readonly == false)
        {
             btnAckPage.className = "AcknowledgePageEnabled";
        }
    }
}
function onAckPageClicked()
{
    try
    {
        if (btnAckPage.readonly == false)
        {
            g_TDO.AckPage();
        }
    }
    catch( err )
    {
        HANDLE_ERROR("onAckPageClicked", err.number, err.description);
    }
}
// --------------------------------------------------------------------------------------
function ShowInstructionsTab(bShow) {
    if (g_SummaryType == SUMMARY_MESSAGE) {
        if (bShow == true) {
            btnInstructionsTab.style.visibility = "visible";
        }
        else {
            btnInstructionsTab.style.visibility = "hidden";
        }
    }
}
// --------------------------------------------------------------------------------------
function InstructionMessageUpdate() {
    if (g_SummaryType == SUMMARY_MESSAGE) {
        if (typeof document.getElementById("instructioniframe").contentWindow.RequestUpdate != "undefined") {
            document.getElementById("instructioniframe").contentWindow.RequestUpdate();
        }
    }
}
// --------------------------------------------------------------------------------------
function ShowInstructionForMessage(msgPoint, msgBlock) {
    if (g_SummaryType == SUMMARY_MESSAGE) {
        ShowDetailsPane();
        var url = window.external.parent.FindFile("sysInstructionResponseInterface.htm") + "?msgpoint=" + msgPoint + "&msgblock=" + msgBlock;
        document.getElementById("instructioniframe").contentWindow.location.replace(url);
        ShowInstructionsTab(true)
        TabDetailsPane(DETAIL_INSTRUCTION_TAB);
    }
}
// --------------------------------------------------------------------------------------
function UpdatedMessageSummaryCookie(messageSummaryCookie) {
    if (g_MessageSummaryCookie != messageSummaryCookie) {
        g_MessageSummaryCookie = messageSummaryCookie;
        return true;
    }
    return false;
}
// --------------------------------------------------------------------------------------
function RetrieveMessageFilter()
{
    var filterJSON = "";
    if(g_SummaryType == SUMMARY_MESSAGE)
    {
        try {
            filterJSON = window.external.Application.Dictionary.Item("sysmessagesummary.htm#filter").Value;
            window.external.Application.Dictionary.Remove("sysmessagesummary.htm#filter");
        }
        catch(err)
        {
            //Filter not provided. Message summary may not have been called using InvokeMessages() that sets a filter.
            //E.g. message summary called via station menu
            filterJSON = "";
        }
    }
    return filterJSON;
}
// --------------------------------------------------------------------------------------
function OnSetStaleAlarmState(Enabled) {
    if (Enabled) {
        lblStaleLabel.style.visibility = "visible";
        divStaleFilteredTotal.style.visibility = "visible";
        lblOfStaleTotal.style.visibility = "visible";
        divStaleUnfilteredTotal.style.visibility = "visible";
    } else {
        lblStaleLabel.style.visibility = "hidden";
        divStaleFilteredTotal.style.visibility = "hidden";
        lblOfStaleTotal.style.visibility = "hidden";
        divStaleUnfilteredTotal.style.visibility = "hidden";
    }
}

