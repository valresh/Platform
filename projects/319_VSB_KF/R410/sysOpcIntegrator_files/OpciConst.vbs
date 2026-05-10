'----------------------------------------------------------------------
'
'  Copyright 2002 Honeywell Pacific.  All Rights Reserved.
'
'----------------------------------------------------------------------

Const FAILED			    = -1	' function failed return value
Const SUCCEEDED             = 0  ' function succeeded return value

Const GROUP_NOT_SELECTED    = -1

Const ERROR_MODE            = -1	' not in config or online
Const ONLINE_MODE           = 1	' Show running groups and perform online operations
Const CONFIG_MODE           = 2	' Show configured groups and perform appropriate operations

Const SRCPROGID	            = 1	' Constants so that ProgID browser populates the right textbox
Const SRCRPROGID            = 2
Const DSTPROGID	            = 3
Const DSTRPROGID            = 4

Const BROWSER_ALL           = 0	' Modes in which the OPC Browser is shown
Const BROWSER_SRC           = 1
Const BROWSER_DST           = 2

'EPKS cluster server states
Const NOT_CONSOLE_STN		= -1
Const SERVER_OK				= 0
Const SERVER_UNAVAILABLE 	= 1  
Const SERVER_SYNCHRONISING 	= 2
Const SERVER_STATUS_UNKNOWN	= 3

' group status
Const GROUP_DISABLED	    = "Disabled"
Const GROUP_STARTING	    = "Starting"
Const GROUP_OK				= "OK"
Const GROUP_MARGINAL	    = "Marginal"
Const GROUP_OVERLOAD	    = "Overload"
Const GROUP_FAILED		    = "Failed"

' key codes
Const ENTER_KEY		        = 13
Const DELETE_KEY 	        = 46

' error codes
Const USER_CANCEL           = 32755

' flex grid Constants
Const flexcpForeColor		= 7
Const flexDMBound			= 1
Const flexDMBoundBatch		= 2
Const flexExSortShowAndMove = 7
Const flexEDNone			= 0
Const flexEDKbdMouse		= 2
Const flexSelectionFree		= 0
Const flexSelectionByRow	= 1
Const flexSelectionListbox	= 3
Const flexTabControls		= 0
Const flexTabCells			= 1
Const flexResizeColumns		= 1

' ADO constants
Const adAffectAll 			= 3
Const adAffectCurrent 	    = 1

' station security levels
Const SECURITY_LEVEL_MNGR   = 5

' colours
Const COLOUR_READONLY	    = &h808080	'grey
Const COLOUR_NORMAL	        = &h00		'black
Const COLOUR_ERROR	        = &hFF		'red

' database field names
Const DB_ITEM_ID			= "ItemId"
Const DB_SOURCE_ITEM		= "SourceItem"
Const DB_DESTINATION_ITEM	= "DestinationItem"
Const DB_SOURCE_TYPE		= "SourceDataType"
Const DB_DESTINATION_TYPE	= "DestinationDataType"
Const DB_GROUP_ID			= "GroupId"
Const DB_ERROR_STRING		= "ErrorString"

Const LOCAL_IP_ADDRESS 		= "127.0.0.1"
Const LOCAL_SERVER 			= "LOCALHOST"
Const HON_OPC_SERVER_PROGID = "HWHSC.OPCSERVER"

' messages
Const MSG_ITEM_DELETE_CONFIRM			    = "Delete selected rows?"
Const MSG_GROUP_DELETE_CONFIRM			    = "Delete group "
Const MSG_ONLINE_GROUP_DELETE_WARNING		= "This will also delete the online group."
Const MSG_ONLINE_GROUP_OVERWRITE_WARNING    = "This will overwrite the online group."
Const MSG_COMMIT_CONFIRM			        = "Commit changes to group "
Const MSG_CANCEL_CONFIRM			        = "Cancel changes to group "
Const MSG_WORKING_GROUP_OVERWRITE_WARNING	= "This will overwrite this working group with the online group."
Const MSG_UNSAVED_CHANGES_WARNING		    = "The current group contains changes that have not been committed."
Const MSG_NAVIGATE_CONFIRM			        = "Are you sure you want to leave this page before committing your changes?"
Const MSG_VALIDATE_COMPLETE			        = "Validation Complete."
Const MSG_GROUP_VALIDATION_FAILED		    = "An error was found validating group details."
Const MSG_CHECK_GROUP_DETAILS			    = "Check the group details tab for more information."
Const MSG_NO_ERRORS_FOUND			        = "No errors were found."
Const MSG_SINGLE_ERROR_FOUND			    = "1 error was found."
Const MSG_MULTIPLE_ERRORS_FOUND			    = " errors were found."
Const MSG_CHECK_ITEM_LIST			        = "Check the item list for more information."
Const MSG_ONLINE_GROUP_DIFFERS			    = "The working group differs from the online group."
Const MSG_UPLOAD_ONLINE_CONFIRM			    = "Upload online group configuration into working group?"
Const MSG_INSUFFICIENT_PRIVELAGES		    = "Higher security level required"
Const MSG_SRC_SERVER_CONFIG_ERROR		    = "You have configured the source OPC server as the local OPC server."
Const MSG_DST_SERVER_CONFIG_ERROR		    = "You have configured the destination OPC server as the local OPC server."
Const MSG_CORRECT_SERVER_CONFIG			    = "Please select the 'This Experion OPC server' option."
Const MSG_SELECTED_FILE				        = "'"
Const MSG_LOCAL_MAPPED_DRIVE			    = "' is on a local drive."
Const MSG_CONFIRM_SERVER_WRITE			    = "Are you sure the server can write to this file?"
Const MSG_CONFIRM_SERVER_READ			    = "Are you sure the server can read from this file?"
Const MSG_CONFIRM_GROUP_EXIT			    = "Group has been modified.  Commit changes? (No to cancel changes)"
Const MSG_CONFIRM_DISABLE_DIAGNOSTIC 		= "Are you sure you want to disable diagnostics on this group?"
Const MSG_SERVER_UNAVAILABLE			    = "OPC Integrator is currently unavailable. Reason: Server is unavailable"
Const MSG_SERVER_SYNCHRONISING			    = "OPC Integrator is currently unavailable. Reason: Server is synchronizing"
Const MSG_SOURCE_SERVER_NOT_CONFIGURED		= "Source server details not configured"
Const MSG_DEST_SERVER_NOT_CONFIGURED		= "Destination server details not configured"
Const MSG_IMPORTEXPORT_NOT_SUPPORTED		= "Import/Export items not support from remote stations"
Const MSG_REDUN_SERVER_INVALID			    = "Primary and redundant servers must be unique"
Const MSG_REDUN_PROGID_INVALID			    = "Primary and redundant servers must have the same class ID"
Const MSG_CHANGES_CANCELLED					= "Changes Cancelled"
Const MSG_COMMIT_REMINDER					= "Configuration changes have been made. Remember to commit or cancel them."
Const MSG_NUMERIC_VALUE_NOT_PROVIDED	    = "Substitute value for numerics has not been provided"
Const MSG_NONNUMERIC_VALUE_NOT_PROVIDED	    = "Substitute value for non-numerics has not been provided"
Const MSG_NUMERIC_VALUE_INVALID			    = "Substitute value for numerics is invalid"
Const MSG_NONNUMERIC_VALUE_INVALID		    = "Substitute value for non-numerics is invalid"
Const MSG_INVALID_STATUS_DATA               = "Status data for group is invalid"

' mode link text
Const LINK_CONFIGURATION_MODE	            = "OPC Integrator Configuration"
Const LINK_ONLINE_MODE				        = "OPC Integrator System Status"

' column header display text
Const ITEM_ID_HEADER 			            = "| Item Id"
Const SOURCE_ITEM_HEADER 	                = "| Source Item"
Const MASTER_ITEM_HEADER 	                = "| Master Item"
Const DEST_ITEM_HEADER 		                = "| Destination Item"
Const SLAVE_ITEM_HEADER 	                = "| Slave Item"
Const SOURCE_TYPE_HEADER 	                = "| Source Type"
Const MASTER_TYPE_HEADER 	                = "| Master Type"
Const DEST_TYPE_HEADER 		                = "| Dest. Type"
Const SLAVE_TYPE_HEADER 	                = "| Slave Type"
Const CONFIG_ERROR_HEADER                   = "| Error Information"
Const ONLINE_ERROR_HEADER                   = "| Error/Trace Information"

' OPC server type
Const SERVER_TYPE_SOURCE 			        = "source"
Const SERVER_TYPE_DESTINATION               = "destination"
Const SERVER_TYPE_MASTER 			        = "master"
Const SERVER_TYPE_SLAVE 			        = "slave"

' Application names
Const APPLICATION_NAME_EPKS                 = "OPC Integrator"
Const APPLICATION_NAME_EBI 	                = "OPC Data Transfer"

' Honeywell server type
Const HON_SERVER_TYPE_EBI 					= "EBI Server"
Const HON_SERVER_TYPE_EPKS 					= "Experion PKS Server"
Const HON_SERVER_TYPE_EPKS_CONSOLE 	        = "Experion PKS Console Station"
Const HON_SERVER_TYPE_EBI_SHORT 		    = "EBI"
Const HON_SERVER_TYPE_EPKS_SHORT 		    = "Experion"


' Registry keys
Const REG_OPCI_CLIENT_DATAPATH	            = "HKEY_LOCAL_MACHINE\Software\Honeywell\OPC Integrator\ClientDataPath"
Const REG_OPCI_CLIENT_CONNECT_STRING_PATH   = "HKEY_LOCAL_MACHINE\Software\Honeywell\OPC Integrator\ClientConnectString"
Const REG_OPCI_SERVER_TYPE                  = "HKEY_LOCAL_MACHINE\Software\Honeywell\HSCServerType"

' Default OPCI client data path
Const OPCI_CLIENT_DATAPATH	                = "c:\ProgramData\Honeywell\ProductConfig\Client\OPC Integrator"

' Default OPCI connection string
Const OPCI_CONNECT_STRING					= "Provider=sqlncli11;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=OPCIntegrator;Data Source="

Const blnEnableBrowsing 	= "TRUE"
Const cstrPreferredServer   = ""
Const cstrBackupServer 		= ""

Const YES_NO_TYPE 			= 1			' Yes/No response question type.
Const GET_RESPONSE_TIMEOUT  = 30000	' 30 sec timeout
Const GET_RESPONSE_INTERVAL	= 1000	' 1 sec interval
Const GET_RESPONSE_CANCELLED = 0		' GetResponse3 call was cancelled
Const GET_RESPONSE_SUCCEEDED = 1		' GetResponse3 call succeeded

Const GET_RESPONSE_YES      = "y"
Const GET_RESPONSE_NO       = "n"

Const CURSOR_TYPE_BUSY		= "wait"
Const CURSOR_TYPE_NORMAL	= "auto"
