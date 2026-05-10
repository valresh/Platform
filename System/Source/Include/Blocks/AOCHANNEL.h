Boolean(ACCEPTDEV,"Accept Device ID",false)
Boolean(COMMFAILFL,"Communications Failure",false)//. Notification whether or not an error occurred while reading OP data from the channel - PWM channel for DOCHANNEL. 
/*
бефера устройства
Byte_A(HCMD00,"",17,0)
Byte_A(HCMD12,"",24,0)
Byte_A(HCMD13,"",21,0)
Byte_A(HCMD14,"",16,0)
Byte_A(HCMD16,"",3,0)
Boolean_A(HCMD48BT,"",200,false)
Enumeration_A(HCMD48NOTIFY,"",200,ViewOnly,Event,Dummy1,Alarm,Def=Event)
...
*/
Real_A(HSLOTVAL,"",4,0)
Boolean(HVARALERT,"Device Variable Alert",false)
Boolean(INITREQ,"Initialization Request Flags",false)
Real(OP,"Output",-6.9)//Defines the output value from this point in percent

/*
Boolean(BADCAL,"Bad Calibration Indicator ",false)// - On a per channel basis, this parameter indicates if calibration is bad or missing.
Integer(BADCODE,"Reason for Bad States",0)
Boolean(COMMFAILFL,"Communications Failure",false)//. Notification whether or not an error occurred while reading OP data from the channel - PWM channel for DOCHANNEL. 
Boolean(INITREQ,"Initialization Request Flags",false)
Integer(IOCSTATE,"IOC State",0)
Real(OP,"Output",-6.9)//Defines the output value from this point in percent. 
Real(OPFINAL,"Final Percent Output",-6.9)//(Sent to Control Element)
Real(PV,"Process Variable",NaN)
*/

Real_A(HDYNVAL,"Value",4,NaN)
Boolean(HENABLE,"Enable HART",false)

Real(BACKCALCOUT,"",NaN)