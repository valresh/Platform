'--------------------------------------------------------------------------------
'Strings that need localization
'--------------------------------------------------------------------------------
Dim str_LOCAL_SERVER
Dim str_ACTIVE_SERVER_LOCATION
str_LOCAL_SERVER = "[local server]"
str_ACTIVE_SERVER_LOCATION = "[active]"
str_TITLE_NAME = "Name: "
str_TITLE_DESC = "Description: "
str_ASSOC_ASSET = "Associated Asset: "
'--------------------------------------------------------------------------------
'Constants section
'--------------------------------------------------------------------------------
Dim str_blank_image
Dim str_unavailable_image
str_blank_image			= "Images/blank.gif"
str_unavailable_image	= "Images/unavailable.gif"
Dim em_perm_FULL
Dim em_perm_CTRL_NO_ACK
Dim em_perm_ACK
Dim em_perm_READ
Dim em_perm_READ_NO_ALM
Dim em_perm_DENY
em_perm_FULL            = 4
em_perm_ACK             = 3
em_perm_READ            = 2
em_perm_READ_NO_ALM     = 1
em_perm_DENY            = 0
Dim em_alarms_ENABLED
Dim em_alarms_DISABLED
em_alarms_ENABLED	= 0
em_alarms_DISABLED	= 1
Dim em_organisation_DISABLED
Dim em_organisation_ENABLED
em_organisation_DISABLED = 0
em_organisation_ENABLED  = 1
Dim em_type_STA
Dim em_type_ANA
Dim em_type_ACC
Dim em_type_ACS
Dim em_type_CON
Dim em_type_ASH
Dim em_type_CLH
Dim em_type_SYS
Dim em_type_CDA
Dim em_type_RDA
Dim em_type_PSA
Dim em_type_ORH
Dim em_type_EQU
em_type_STA		= 1
em_type_ANA		= 2
em_type_ACC		= 3
em_type_ACS		= 4
em_type_CON		= 5
em_type_ASH		= 6
em_type_CLH		= 7
em_type_SYS		= 8
em_type_ORH		= 9
em_type_EQU		= 10
em_type_CDA		= 101
em_type_RDA		= 102
em_type_PSA		= 103
Dim em_stype_SYS_MODEL
Dim em_stype_SYS_SERVER
Dim em_stype_SYS_CONTROLLERS
Dim em_stype_SYS_CHANNEL
Dim em_stype_SYS_CONTROLLER
Dim em_stype_SYS_PRINTERS
Dim em_stype_SYS_PRINTER
Dim em_stype_SYS_SYSINTERFACES
Dim em_stype_SYS_SYSINTERFACE
Dim em_stype_SYS_STATIONS
Dim em_stype_SYS_FLEXSTATION
Dim em_stype_SYS_CONSSTATION
Dim em_stype_SYS_CONSOLES
Dim em_stype_SYS_CONSOLE
Dim em_stype_SYS_CSTNCLIENT
Dim em_stype_SYS_UNASSIGNEDITEMS
Dim em_stype_SYS_COMPUTERS
Dim em_stype_SYS_COMPUTER
Dim em_stype_SYS_DOMAIN
Dim em_stype_SYS_ORGUNIT
Dim em_stype_SYS_NETEQUIP
Dim em_stype_SYS_SWITCH
Dim em_stype_SYS_FTE
Dim em_stype_SYS_FTECOMMUNITY
Dim em_stype_SYS_FTEDEVICE
Dim em_stype_SYS_9PSWITCH
Dim em_stype_SYS_UNKNOWNCONTROLLERS
Dim em_stype_SYS_PNTSRV_CONTROLLER
Dim em_stype_SYS_PNTSRV_CHANNEL
Dim em_stype_SYS_PNTSRV_NETWORK
Dim em_stype_SYS_METER
Dim em_stype_SYS_NBCCS
Dim em_stype_SYS_NBCC
em_stype_SYS_MODEL			= 1001
em_stype_SYS_SERVER			= 1002
em_stype_SYS_CONTROLLERS		= 1003
em_stype_SYS_CHANNEL			= 1004
em_stype_SYS_CONTROLLER			= 1005
em_stype_SYS_PRINTERS			= 1006
em_stype_SYS_PRINTER			= 1007
em_stype_SYS_SYSINTERFACES		= 1008
em_stype_SYS_SYSINTERFACE		= 1009
em_stype_SYS_STATIONS			= 1010
em_stype_SYS_FLEXSTATIONS		= 1011
em_stype_SYS_CONSSTATIONS		= 1012
em_stype_SYS_FLEXSTATION		= 1013
em_stype_SYS_CONSSTATION		= 1014
em_stype_SYS_CONSOLES			= 1015
em_stype_SYS_CONSOLE			= 1016
em_stype_SYS_CSTNCLIENT			= 1017
em_stype_SYS_UNASSIGNEDITEMS		= 1018
em_stype_SYS_COMPUTERS          	= 1019
em_stype_SYS_COMPUTER           	= 1020
em_stype_SYS_DOMAIN             	= 1021
em_stype_SYS_ORGUNIT            	= 1022
em_stype_SYS_NETEQUIP           	= 1023
em_stype_SYS_SWITCH             	= 1024
em_stype_SYS_FTE                	= 1025
em_stype_SYS_FTECOMMUNITY       	= 1026
em_stype_SYS_FTEDEVICE          	= 1027
em_stype_SYS_9PSWITCH          		= 1028
em_stype_SYS_UNKNOWNCONTROLLERS         = 1029
em_stype_SYS_PNTSRV_CONTROLLER		= 1030
em_stype_SYS_PNTSRV_CHANNEL		= 1031
em_stype_SYS_PNTSRV_NETWORK		= 1032
em_stype_SYS_METER			= 1034
em_stype_SYS_NBCCS			= 1039
em_stype_SYS_NBCC			= 1040
Const em_stype_SYS_TPS_LOW  = 1042
Const em_stype_SYS_TPS_HIGH = 1146
Const em_stype_SYS_LCN	    = 1144
Const em_stype_SYS_GATEWAYS	= 1146
Const em_stype_PSA_TPS_IOP_LOW  = 1150
Const em_stype_PSA_TPS_IOP_HIGH = 1213
Dim em_inum_SYS_MODEL_ASSETS
Dim em_inum_SYS_MODEL_ALARMGROUPS
Dim em_inum_SYS_MODEL_SYSTEM
Dim em_inum_SYS_MODEL_CANE
Dim em_inum_SYS_MODEL_ORGANIZATIONS
Dim em_inum_SYS_MODEL_SYSTEMALARMGROUPS
Dim em_inum_SYS_MODEL_LISTS
em_inum_SYS_MODEL_ASSETS            = 1
em_inum_SYS_MODEL_ALARMGROUPS       = 2
em_inum_SYS_MODEL_SYSTEM            = 3
em_inum_SYS_MODEL_CANE              = 4
em_inum_SYS_MODEL_ORGANIZATIONS     = 5
em_inum_SYS_MODEL_SYSTEMALARMGROUPS = 8
em_inum_SYS_MODEL_LISTS             = 9
Dim em_inum_SYS_SERVER_LOCAL
em_inum_SYS_SERVER_LOCAL            = 1
'--------------------------------------------------------------------------------
'This function is called by displays using the Enterprise Model Location Pane
'e.g. alarm summary, system SOR displays
'
'ARGUMENTS
'	Arg1: Value  - array of 5 variants: Value[RemoteType, SubType, ItemNumber, Redundant, Flags]
'
'RETURN
'	Function returns the relative path to the appropriate node icon in the Images sub-directory
'
'IMPORTANT
'	Changes to the CDA sub-type values will need to be mirrored in the following Control Builder
'	code:
'
'	ClearCase path rep\CtrlLib\control.cpp		CControl::GetPointSubType()
'	ClearCase path rep\CtrlLib\control.h		enum ePointSubType
'
'   These are the ranges reserved for ECI:
'      FortW CDA            001 - 199
'      Unreserved           200 - 699
'      SM (Safety Manager)  700 - 799
'      PMD                  800 - 899
'      QCS                  900 - 999
'
'   The Experion install will install the "FortW CDA" icons.
'
'   The SM, PMD and QCS icons will not be installed by Experion and will be installed by SM, PMD or QCS install package.
'--------------------------------------------------------------------------------
Function GetNodeImagePath(Value)
	Dim Return
	If (IsEmpty(Value(0))) Then
		Return = str_blank_image
	Else
		Select Case Value(0)
		Case em_type_STA
			Return = "Images/node_sta.png"
		Case em_type_ANA
			Return = "Images/node_ana.png"
		Case em_type_ACC
			Return = "Images/node_acc.png"
		Case em_type_ACS
			Return = "Images/node_acs.png"
		Case em_type_CON
			If (Value(4) and "&H0800") Then
				If (Value(4) and "&H1000") Then
					Return = "Images/node_con_list.png"
				Else
					Return = "Images/node_con_equipment.png"
				End If
			Else
				Return = "Images/node_con.png"
			End If
		Case em_type_ASH
			Return = "Images/node_ash.png"
		Case em_type_CLH
			Return = "Images/node_clh.png"
		Case em_type_ORH
			If (Value(4) and 2) then
				Return = "Images/node_orh_company.png"
			else
				Return = "Images/node_orh.png"
			end if
		Case em_type_EQU
			Return = "Images/equipment.png"
		Case em_type_SYS
			Select Case Value(1)
			Case em_stype_SYS_MODEL
				Select Case Value(2)
				Case em_inum_SYS_MODEL_ASSETS
					Return = "Images/node_sys_mdl_assets.png"
				Case em_inum_SYS_MODEL_ALARMGROUPS
					Return = "Images/node_sys_mdl_alarmgroups.png"
				Case em_inum_SYS_MODEL_SYSTEMALARMGROUPS
					Return = "Images/node_sys_mdl_alarmgroups.png"
				Case em_inum_SYS_MODEL_SYSTEM
					Return = "Images/node_sys_mdl_system.png"
				Case em_inum_SYS_MODEL_CANE
					Return = "Images/node_sys_mdl_cane.png"
				Case em_inum_SYS_MODEL_ORGANIZATIONS
					Return = "Images/node_sys_mdl_organization.png"
				Case em_inum_SYS_MODEL_LISTS
					Return = "Images/node_sys_mdl_assets.png"
				Case Else
					Return = str_blank_image
				End Select
			Case em_stype_SYS_SERVER
				If Value(3) Then
					Return = "Images/node_sys_server_redundant.png"
				Else
					Return = "Images/node_sys_server.png"
				End If
			Case em_stype_SYS_CONTROLLERS
				Return = "Images/node_sys_controllers.png"
			Case em_stype_SYS_CHANNEL
				Return = "Images/node_sys_channel.png"
			Case em_stype_SYS_CONTROLLER
				Return = "Images/node_sys_controller.png"
			Case em_stype_SYS_METER
				Return = "Images/node_sys_meter.png"
			Case em_stype_SYS_PRINTERS
				Return = "Images/node_sys_printers.png"
			Case em_stype_SYS_PRINTER
				Return = "Images/node_sys_printer.png"
			Case em_stype_SYS_SYSINTERFACES
				Return = "Images/node_sys_sysinterfaces.png"
			Case em_stype_SYS_SYSINTERFACE
				Return = "Images/node_sys_sysinterface.png"
			Case em_stype_SYS_STATIONS
				Return = "Images/node_sys_stations.png"
			Case em_stype_SYS_FLEXSTATIONS
				Return = "Images/node_sys_flexstations.png"
			Case em_stype_SYS_CONSSTATIONS
				Return = "Images/node_sys_consstations.png"
			Case em_stype_SYS_FLEXSTATION
				Return = "Images/node_sys_flexstation.png"
			Case em_stype_SYS_CONSSTATION
				Return = "Images/node_sys_consstation.png"
			Case em_stype_SYS_CONSOLES
				Return = "Images/node_sys_consoles.png"
			Case em_stype_SYS_CONSOLE
				Return = "Images/node_sys_console.png"
			Case em_stype_SYS_CSTNCLIENT
				Return = "Images/node_sys_cstnclient.png"
			Case em_stype_SYS_COMPUTERS
				Return = "Images/node_sys_computers.png"
			Case em_stype_SYS_DOMAIN
				Return = "Images/node_sys_domain.png"
			Case em_stype_SYS_ORGUNIT
				Return = "Images/node_sys_orgunit.png"
			Case em_stype_SYS_NETEQUIP
				Return = "Images/node_sys_netequip.png"
			Case em_stype_SYS_FTE
				Return = "Images/node_sys_fte.png"
			Case em_stype_SYS_FTECOMMUNITY
				Return = "Images/node_sys_ftecommunity.png"
			Case em_stype_SYS_UNASSIGNEDITEMS
				Return = "Images/node_sys_unassigneditems.png"
			Case em_stype_SYS_UNKNOWNCONTROLLERS
				Return = "Images/node_cda_unknown.png"
			Case em_stype_SYS_NBCCS
				Return = "Images/node_sys_nbccs.png"
			Case em_stype_SYS_NBCC
				Return = "Images/node_sys_nbcc.png"
			Case em_stype_SYS_LCN
				Return = "Images/node_sys_tps_" + CStr(Value(1)) + ".png"
			Case em_stype_SYS_GATEWAYS
				Return = "Images/node_sys_tps_" + CStr(Value(1)) + ".png"
			Case Else
				Return = str_blank_image
			End Select
		Case em_type_CDA
			'Reserved range for SM (Safety Manager)
			If ((Value(1) >= 700) and (Value(1) <= 799)) Then
				Select Case Value(3)
				Case 1		'Redundant primary
					Return = "Images/SM_"+CStr(Value(1))+"_redundant.gif"
				Case 2		'Redundant backup
					Return = "Images/SM_"+CStr(Value(1))+"_redundant_backup.gif"
				Case Else	'Not redundant
					Return = "Images/SM_"+CStr(Value(1))+".gif"
				End Select
			'Reserved range for PMD
			ElseIf ((Value(1) >= 800) and (Value(1) <= 899)) Then
				Select Case Value(3)
				Case 1		'Redundant primary
					Return = "Images/PMD_"+CStr(Value(1))+"_redundant.gif"
				Case 2		'Redundant backup
					Return = "Images/PMD_"+CStr(Value(1))+"_redundant_backup.gif"
				Case Else	'Not redundant
					Return = "Images/PMD_"+CStr(Value(1))+".gif"
				End Select
			'Reserved range for QCS
			ElseIf ((Value(1) >= 900) and (Value(1) <= 999)) Then
				Select Case Value(3)
				Case 1		'Redundant primary
					Return = "Images/QCS_"+CStr(Value(1))+"_redundant.gif"
				Case 2		'Redundant backup
					Return = "Images/QCS_"+CStr(Value(1))+"_redundant_backup.gif"
				Case Else	'Not redundant
					Return = "Images/QCS_"+CStr(Value(1))+".gif"
				End Select
			Else
			    Select Case Value(1)
			    case 0
				    Return = "Images/node_ana.png"
			    Case 1		'Controller [generic 2-slot modules]
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_module_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_module_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_module.png"
				    End Select
			    Case 2		'1756 I/O [generic 1-slot module]
				    Return = "Images/node_cda_io.png"
			    Case 3		'FTEB
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_fteb_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_fteb_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_fteb.png"
				    End Select
			    Case 4		'PMIO module
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_pmio_redundant.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_pmio.png"
				    End Select
			    Case 5		'PMIO Channel Block (tagged if HART-enabled)
				    Return = "Images/node_cda_pmio_channel.png"
			    Case 6		'CEE
				    Return = "Images/node_cda_cee.png"
			    Case 7		'Fieldbus device
				    Return = "Images/node_cda_fieldbus.png"
			    Case 8		'Hiways
				    Return = "Images/node_cda_hiways.png"
			    Case 9		'Hiway
				    Return = "Images/node_cda_hiway.png"
			    Case 10		'Hiway Box
				    Return = "Images/node_cda_hiway_box.png"
			    Case 11		'SIM-C200 (formerly Simulation Control Environment or SCE)
				    Return = "Images/node_cda_simc200.png"
			    Case 12		'ACE
				    Return = "Images/node_cda_ace.png"
			    Case 13		'OPC Server
				    Return = "Images/node_cda_opcsrv.png"
			    Case 14		'Unknown HW Device
				    Return = "Images/node_cda_unknown.png"
			    Case 15		'PMIO IOLINK
				    Return = "Images/node_cda_iolink.png"
			    Case 16		' Fieldbus LINK
				    Return = "Images/node_cda_fflink.png"
			    Case 17		'Redundancy Module
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_red_module_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_red_module_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_red_module.png"
				    End Select
			    Case 18		'Fieldbus FIM
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_fim_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_fim_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_fim.png"
				    End Select
			    Case 19		'PMIO IOLIM
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_iolim_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_iolim_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_iolim.png"
				    End Select
			    Case 20		'C300
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_c300_module_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_c300_module_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_c300_module.png"
				    End Select
			    Case 21		'C300 Cee
				    Return = "Images/node_cda_c300_cee.png"
			    Case 22		'C300 IOLINK EE
				    Return = "Images/node_cda_c300_iolinkee.png"
			    Case 23		'CFIM
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_cfim_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_cfim_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_cfim.png"
				    End Select
			    Case 24		'LIOM
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_liom_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_liom_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_liom.png"
				    End Select
			    Case 25		'LIOM IOM
				    Return = "Images/node_cda_liom_iom.png"
			    Case 26		'LIOM SLINK
				    Return = "Images/node_cda_liom_slink.png"
			    Case 27
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_seriesc_io_redundant.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_seriesc_io.png"
				    End Select
			    Case 28
				    Return = "Images/node_cda_c300_iolinkee_pmio.png"
			    Case 29
				    Return = "Images/node_cda_c300_iolinkee_seriesc.png"
			    Case 30
				    Return = "Images/node_cda_simc300.png"
			    Case 31
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_mb_tcp_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_mb_tcp_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_mb_tcp.png"
				    End Select
			    Case 32
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_mb_tcp_bridge_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_mb_tcp_bridge_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_mb_tcp_bridge.png"
				    End Select
			    Case 33
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_safetyManager_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/node_cda_safetyManager_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_safetyManager.png"
				    End Select
			    Case 34
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/34_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/34_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/34.png"
				    End Select
			    Case 35
				    Return = "Images/35.png"
			    Case 36 'C200E
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/36_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/36_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/36.png"
				    End Select
			    Case 37 'PGM Module
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/37_redundant.png"
				    Case 2		'Redundant backup
					    Return = "Images/37_redundant_backup.png"
				    Case Else	'Not redundant
					    Return = "Images/37.png"
				    End Select
			    Case 38 'PBLINK
				    Return = "Images/38.png"
			    Case 39 'DSB
				    Return = "Images/39.png"
			    Case 40 'PIOMB
				    Return = "Images/40.png"
			    Case 41 'FDM MUX
				    Return = "Images/41.png"
                Case 42 'PCDI GENERIC
                    Select Case Value(3)
                    Case 1		'Redundant primary
                        Return = "Images/42_redundant.png"
                    Case 2		'Redundant backup
                        Return = "Images/42_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/42.png"
                    End Select
                Case 43 'EHPM
                    Select Case Value(3)
                    Case 1		'Redundant primary
                        Return = "Images/43_redundant.png"
                    Case 2		'Redundant backup
                        Return = "Images/43_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/43.png"
                    End Select
                Case 44 'ENIM
                        Select Case Value(3)
                    Case 1		'Redundant primary
                        Return = "Images/44_redundant.png"
                    Case 2		'Redundant backup
                        Return = "Images/44_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/44.png"
                    End Select
                Case 45 'ECG
                    Select Case Value(3)
                    Case 1		'Redundant primary
                        Return = "Images/45_redundant.png"
                    Case 2		'Redundant backup
                        Return = "Images/45_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/45.png"
                    End Select
                Case 46 'IEC61850IM
                    Select Case Value(3)
                    Case 1		'Redundant primary
                        Return = "Images/46_redundant.png"
                    Case 2		'Redundant backup
                        Return = "Images/46_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/46.png"
                    End Select
			    Case 47 'eIIMLINK
				    Return = "Images/47.png"
			    Case 48 'IEC61850 IED block
				    Return = "Images/48.png"
			    Case 49 'IEC61850 Logical Device
				    Return = "Images/49.png"
			    Case 50 'PMIO / Series C Input Channel Block
				    Return = "Images/50.png"
			    Case 51 'PMIO / Series C Output Channel Block
				    Return = "Images/51.png"
                Case 52 'Ethernet Interface Module
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/52_redundant.png"
                    Case 2      'Redundant backup
                        Return = "Images/52_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/52.png"
                    End Select
                Case 53 'IEC61850 Protocol Block
                    Return = "Images/53.png"
                Case 54 'Ethernet/IP Protocol Block
                    Return = "Images/54.png"
                Case 55 'MMS Subscriber Block
                    Return = "Images/55.png"
                Case 56 'Data Set Map Block
                    Return = "Images/56.png"
                Case 57 'Custom Cabinet
                    Return = "Images/57.png"
                Case 58 'Remote Cabinet
                    Return = "Images/58.png"
                Case 59 'UOC Controller
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/59_redundant.png"
                    Case 2      'Redundant backup
                        Return = "Images/59_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/59.png"
                    End Select
                Case 60 'UOC Controller CEE Block
                    Return = "Images/60.png"
                Case 61 'Analog Input IOPOINT
                    Return = "Images/61.png"
                Case 62 'Analog Output IOPOINT
                    Return = "Images/62.png"
                Case 63 'Digital Input IOPOINT
                    Return = "Images/63.png"
                Case 64 'Digital Output IOPOINT
                    Return = "Images/64.png"
                Case 65 'Expansion Rack
                    Return = "Images/65.png"
                Case 66 'UIO IO module
                    Return = "Images/66.png"
                Case 67 'ELCNB
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/67_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/67_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/67.png"
                    End Select
                Case 68 'UEA ENIM
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/68_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/68_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/68.png"
                    End Select
                Case 69 'Virtual ENIM
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/69_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/69_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/69.png"
                    End Select
                Case 70 'UEA AM
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/70_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/70_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/70.png"
                    End Select
                Case 71 'Virtual AM
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/71_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/71_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/71.png"
                    End Select
                Case 72 'UEA EHB
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/72_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/72_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/72.png"
                    End Select
				Case 73 'Virtual EHB
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/73_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/73_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/73.png"
                    End Select
				Case 74 'UEA EPLCG
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/74_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/74_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/74.png"
                    End Select
				Case 75 'Virtual EPLCG
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/75_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/75_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/75.png"
                    End Select
                Case 76 'Non Licensed Analog Input IOPOINT
                    Return = "Images/76.png"
                Case 77 'Non Licensed Analog Output IOPOINT
                    Return = "Images/77.png"
                Case 78 'Non Licensed Drive Digital Input IOPOINT
                    Return = "Images/78.png"
                Case 79 'Non Licensed Drive Digital Output IOPOINT
                    Return = "Images/79.png"
				Case 80 'UEA NG
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/80_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/80_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/80.png"
                    End Select
				Case 81 'Virtual NG
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/81_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/81_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/81.png"
                    End Select
				Case 82 'SIM NIM
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/82_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/82_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/82.png"
                    End Select
				Case 83 'SIM EHPM
                    Select Case Value(3)
                    Case 1      'Redundant primary
                        Return = "Images/83_redundant.png"					
                    Case 2      'Redundant backup
                        Return = "Images/83_redundant_backup.png"
                    Case Else	'Not redundant
                        Return = "Images/83.png"
                    End Select
				Case 84 'M300
					Select Case Value(3)
					Case 1      'Redundant primary
						Return = "Images/84_redundant.png"					
					Case 2      'Redundant backup
						Return = "Images/84_redundant_backup.png"
					Case Else	'Not redundant
						Return = "Images/84.png"
					End Select
				Case 85 'M300Cee
					Return = "Images/85.png"
				Case 86 'TCMI
					Return = "Images/node_cda_unknown.png"
				Case 87 'ControlHive
					Return = "Images/87.png"
                Case 600		'CEE
				    Return = "Images/node_cda_cee.png"
				Case 601		'Unknown HW Device
				    Return = "Images/node_cda_unknown.png"
				Case 603		'PMIO module
				    Select Case Value(3)
				    Case 1		'Redundant primary
					    Return = "Images/node_cda_pmio_redundant.png"
				    Case Else	'Not redundant
					    Return = "Images/node_cda_pmio.png"
				    End Select
				Case Else
                    Return = str_blank_image
                End Select
	        End If
		Case em_type_RDA
			Return = "Images/node_rda.gif"
		Case em_type_PSA
			If ((Value(1) >= em_stype_SYS_TPS_LOW) and (Value(1) <= em_stype_SYS_TPS_HIGH)) Then
				Return = "Images/node_sys_tps_" + CStr(Value(1)) + ".png"
			ElseIf ((Value(1) >= em_stype_PSA_TPS_IOP_LOW) and (Value(1) <= em_stype_PSA_TPS_IOP_HIGH)) Then
				Select Case Value(3)
				Case 1      'Redundant
					Return = "Images/node_sys_tps_iop_redun.png"					
				Case Else	'Not redundant
					Return = "Images/node_sys_tps_iop.png"
				End Select
			Else
				Select Case Value(1)
				Case em_stype_SYS_COMPUTER
					Return = "Images/node_psa_computer.png"
				Case em_stype_SYS_SWITCH
					Return = "Images/node_psa_switch.png"
				Case em_stype_SYS_FTEDEVICE
					Return = "Images/node_psa_ftedevice.png"
				Case em_stype_SYS_9PSWITCH
					Return = "Images/node_psa_9pswitch.png"
				Case em_stype_SYS_PNTSRV_CONTROLLER
					Return = "Images/node_psa_controller.png"
				Case em_stype_SYS_PNTSRV_CHANNEL
					Return = "Images/node_psa_channel.png"
				Case em_stype_SYS_PNTSRV_NETWORK
					Return = "Images/node_psa_network.png"
				Case em_stype_SYS_FTECOMMUNITY
					Return = "Images/node_sys_ftecommunity.png"
				Case Else
					Return = "Images/node_psa.png"
				End Select
			End If
		Case Else
			Return = str_blank_image
		End Select
	End If
	GetNodeImagePath = Return
End Function
'--------------------------------------------------------------------------------
'This function is called by displays using the Enterprise Model tree to assign
'action permissions (e.g. SOR displays)
'
'ARGUMENTS
'	Arg1: Value  - variant corresponding to the Permission value
'
'RETURN
'	Function returns the relative path to the appropriate permission icon in the Images sub-directory
'--------------------------------------------------------------------------------
Function GetPermissionImagePath(Value)
	Dim Return
	If (IsEmpty(Value(0))) Then
		Return = str_blank_image
	ElseIf (Value(1) <> "#Error") Then
		Select Case Value(0)
		Case em_perm_FULL
			Return = "Images/aa_perm_full.gif"
		Case em_perm_ACK
			Return = "Images/aa_perm_ack.gif"
		Case em_perm_READ
			Return = "Images/aa_perm_read.gif"
		Case em_perm_READ_NO_ALM
			Return = "Images/aa_perm_read_no_alm.gif"
		Case em_perm_DENY
			Return = "Images/aa_perm_deny.gif"
		Case Else
			Return = str_blank_image
		End Select
	Else
		Select Case Value(0)
		Case em_perm_FULL
			Return = "Images/perm_full.gif"
		Case em_perm_ACK
			Return = "Images/perm_ack.gif"
		Case em_perm_READ
			Return = "Images/perm_read.gif"
		Case em_perm_READ_NO_ALM
			Return = "Images/perm_read_no_alm.gif"
		Case em_perm_DENY
			Return = "Images/perm_deny.gif"
		Case Else
			Return = str_blank_image
		End Select
	End If
	GetPermissionImagePath = Return
End Function
'--------------------------------------------------------------------------------
'This function is called by the alarm enable/disable display (sys137.htm)
'
'ARGUMENTS
'	Arg1: Value  - variant corresponding to the disabled value
'
'RETURN
'	Function returns the relative path to the appropriate disabled icon in the Images sub-directory
'--------------------------------------------------------------------------------
Function GetDisabledImagePath(Value)
	Dim Return
	If (IsEmpty(Value)) Then
		Return = str_blank_image
	Else
		Select Case Value
		Case em_alarms_DISABLED
			Return = "Images/perm_deny.gif"
		Case em_alarms_ENABLED
			Return = "Images/perm_full.gif"
		Case Else
			Return = str_blank_image
		End Select
	End If
	GetDisabledImagePath = Return
End Function
'--------------------------------------------------------------------------------
'This function is called by the organisation enable/disable display (sysCfgOrgEntitlement.htm)
'
'ARGUMENTS
'	Arg1: Value  - variant corresponding to the enabled value
'
'RETURN
'	Function returns the relative path to the appropriate enabled icon in the Images sub-directory
'--------------------------------------------------------------------------------
Function GetEnabledImagePath(Value)
	Dim Return
	If (IsEmpty(Value)) Then
		Return = str_blank_image
	Else
		Select Case Value
		Case em_organisation_DISABLED
			Return = "Images/perm_deny.gif"
		Case em_organisation_ENABLED
			Return = "Images/perm_full.gif"
		Case Else
			Return = str_blank_image
		End Select
	End If
	GetEnabledImagePath = Return
End Function
'--------------------------------------------------------------------------------
'This function is called by displays using the Enterprise Model Tree to determine
'what string to show as the node name
'
'ARGUMENTS
'	Arg1: Value  - array of at least 7 variants:
'		Value[EntityName, RemoteType, SubType, ItemNumber, TotalAlarms(%StationNumber%), TotalDisabledAlarms(%StationNumber%), DirectConnect, ActiveServerLocation]
'
'RETURN
'	Function returns the name to display
'--------------------------------------------------------------------------------
Function GetNodeDisplayName(Value)
	Dim Return
	If (IsEmpty(Value(0))) Then
		Return = ""
	Else
		Return = Value(0)
		If( (Value(1) = em_type_SYS) And (Value(2) = em_stype_SYS_SERVER) And (Value(3) = em_inum_SYS_SERVER_LOCAL) ) Then
			Return = Return & " " & str_LOCAL_SERVER
		ElseIf(UBound(Value) >= 7) Then
			If (( Value(1) = em_type_SYS) And (Value(2) = em_stype_SYS_NBCC) And (Value(7) = 1) ) Then
				Return = Return & " " & str_ACTIVE_SERVER_LOCATION
			End If
		End If
	End If
	GetNodeDisplayName = Return
End Function
'--------------------------------------------------------------------------------
'This function is called by displays using the Enterprise Model Tree to determine
'what string to show as its title
'
'ARGUMENTS
'	Arg1: Value  - array of at least 2 variants: Value[FullName,Description]
'
'RETURN
'	Function returns the title to display
'--------------------------------------------------------------------------------
Function GetItemTitle(Value)
	Dim Return
	Return = "" ' Default in case we can't extract a better title from the value-array.
	If IsArray(Value) Then
	    If (UBound(Value) >= 0) Then
	        If Not IsEmpty(Value(0)) Then
	            ' We have a name for this item available. Use it in the title.
	            Return = Return & str_TITLE_NAME & Value(0)
	        End If
	    End If
        If (UBound(Value) > 0) Then
            If Not IsEmpty(Value(1)) And (Value(1) <> "#Error") Then
                ' Use this non-invalid description in the title.
	            Return = Return & vbCrLf & str_TITLE_DESC & Value(1)
	        End If
		End If
        If (UBound(Value) > 1) Then
            If Not IsEmpty(Value(2)) And (Value(2) <> "#Error") Then
                ' Use this non-invalid associated asset in the title.
                Return = Return & vbCrLf & str_ASSOC_ASSET & Value(2)
            End If
        End If
	End If
	GetItemTitle = Return
End Function
'--------------------------------------------------------------------------------
' Function called to determine if a particular node in the system tree is available
' during server-unavailable mode on a console station
'
'ARGUMENTS
'	Arg1: Value  - array of 4 variants: Value[EntityName, Type, SubType, DirectConnect]
'
'RETURN
'	Function returns TRUE if the node is available
'
'DEPENDENCY
'	Function depends on the CheckServerAvailable() function from EMScripts.js
'--------------------------------------------------------------------------------
Function GetSystemNodeAvailability(Value)
	Dim Return
	Dim Available
	Dim Direct
	Dim RemoteType
	Dim SubType
	If (IsEmpty(Value(0))) Then
		Return = False
	Else
		Available	= CheckServerAvailable(false)
		Name		= LCase(Value(0))
		LocalType	= Value(1)
		SubType		= Value(2)
		Direct		= Value(3)
		On Error Resume Next
		CSName		= LCase(external.Parent.RuntimeStatus.ConsoleStationName)
		If Err.number <> 0 Then
			CSName  = ""
			Err.Clear
		End If
		Select Case LocalType
		Case em_type_SYS
			Select Case SubType
			Case em_stype_SYS_CHANNEL, em_stype_SYS_CONTROLLER, em_stype_SYS_FLEXSTATION
				Return = Available
			Case em_stype_SYS_PRINTER					' Available if it is a network printer
				Return = Available Or (InStr(Name, "\\") = 1)
			Case em_stype_SYS_SYSINTERFACE				' Available for direct connections
				Return = Available Or Direct
			Case em_stype_SYS_CONSSTATION				' Available when local
				Return = Available Or (Name = CSName)
			Case em_stype_SYS_CSTNCLIENT				' Available when local
				Return = Available Or (InStr(Name, CSName) = 1)
			Case Else									' All other SYS points are available by default
				Return = True
			End Select
		Case em_type_CDA					' CDA points should always be directly connected
			Return = True
		Case em_type_RDA					' RDA points are not available if the cluster server is gone
			Return = Available
		Case Else							' Everything else by default does not get shown in the system tree
			Return = True
		End Select
	End If
	GetSystemNodeAvailability = Return
End Function
'--------------------------------------------------------------------------------
' Function called by the summary displays to obtain the image to overlay on top
' of the node image indicating node availability when cluster server is down
'
'ARGUMENTS
'	Arg1: Value  - array of 4 variants: Value[EntityName, Type, SubType, DirectConnect]
'
'RETURN
'	Function returns the relative path to the appropriate disabled icon in the Images sub-directory
'--------------------------------------------------------------------------------
Function GetUnavailableImagePath(Value)
	Dim Return
	If (IsEmpty(Value(0)) Or GetSystemNodeAvailability(Value)) Then
		Return = str_blank_image
	Else
		Return = str_unavailable_image
	End If
	GetUnavailableImagePath = Return
End Function
'--------------------------------------------------------------------------------
' Function called by the operator assignment display to handle EBI's belief that
' the Asset Model, Organizational Model and Unassigned Items are unassignable.
'
'ARGUMENTS
'	Arg1: Server type.  5 to 7 are EBI server types
'	Arg2: Value - tag name
'
'RETURN
'	Function returns true or false depending on whether we're EBI and whether it's a special tag
'--------------------------------------------------------------------------------
Function EBINotAssignable(ServerType, Value)
	If (ServerType >= 5 And ServerType <= 7) Then
		If (StrComp(Value, "$ASSETMODEL") = 0 Or _
		    StrComp(Value, "$ORGANIZATIONMODEL") = 0 Or _
		    StrComp(Value, "$EQUIPMENTMODEL") = 0 Or _
		    StrComp(Value, "$UNASSIGNEDITEMS") = 0) Then
			EBINotAssignable = True
		Else
			EBINotAssignable = False
		End If
	Else
		' Always assignable for EPKS
		EBINotAssignable = False
	End If
End Function

