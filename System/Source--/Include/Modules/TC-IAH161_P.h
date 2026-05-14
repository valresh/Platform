String_K(NAME,"Имя",31*4)
String_K(DESC,"Block Descriptor",131*4)
Boolean(ALMENBSTATE,"Alarming Enabled",0)
Integer(CATNUMBER,"Catalog Number",0)
/*TC-IAH061: 1 TC- MUX021: 0
TC-IAH161: 6 TC-OAH061: 4
TC-IDA161: 5 TC-OAV061: 3
TC-IDD321: 12 TC-OAV081: 9
TC-IDJ161: 6 TC-ODA161: 7
TC-IDK161: 5 TC-ODD321: 17
TC-IDW161: 6 TC-ODJ161: 7
TC-IDX081: 1 TC-ODK161: 7
TC-IDX161: 2 TC-ODX081: 4
TC-IXL061: 2 TC-ODX161: 4
TC-IXR061: 5 TC-MDP081
(Pulse Input IOM): 8*/
Integer(DLCNBSLOT,"Downlink CNI Slot Number",0)
Real(ESTWEIGHT,"Estimated Weight",NaN)
String_K(EUDESC,"Engineering Units",24*4)
Integer(IOMSLOT,"IOM Slot Number in Chassis",0)
String_K(IOMTYPE,"IO Module Type",63*4)
Integer(NUMCHANS,"Number of Channels",0)
Integer(NUMCONN,"Number of Connections",0)
Integer(ORDERINCEE,"Execution Order in CEE",0)
//Enumeration(PERIOD,"Execution Period",DEFAULT,5mS,10mS,20mS,50mS,100mS,200mS,500mS,1sec,2sec,5sec,10sec,20sec,25mS,NONE,30sec,1min,2min,5min,10min,20min,30min,1hr,2hr,4hr,8hr,12hr,24hr,40mS,400mS,80mS,800mS,Def=DEFAULT)
Integer(PRODTYPE,"Product Type",0)
Integer(SCANCTRLLVL,"SCAN Control Level",200)
String_K(SCANEUHI,"SCAN EUHI Parameter",254*4)
String_K(SCANEULO,"SCAN EULO Parameter",254*4)
Integer(ULCNBMAC,"Uplink CNI Address in IO Rack",0)
Enumeration(IOSCHEDOPT,"I/O Schedule Option",NotIOM,OutputModule,MixedModule,InputModule,Def=NotIOM)
Enumeration(SECSIGSECLVL,"Sec Signer Security Level",VIEWONLY,ACKONLY,OPER,SUPV,ENGR,MNGR,Def=VIEWONLY)
Enumeration(TEMPMODE,"Temperature Mode",Celsius,Fahrenheit,Def=Fahrenheit)
