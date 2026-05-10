Boolean(BADPVFL,"Bad PV Flag ",false)
Enumeration(PTEXECST,"Point Execution State",Inactive,Active,Def=Inactive)
Boolean(PV,"PV",false)
Integer(BADCODE,"Reason for Bad States",0)
Boolean(COS,"Change of State",false)
Boolean(HWFAULT,"Hardware fault detected",false)
Integer(IOCSTATE,"IOC State",0)
Boolean(PVFL,"Actual State Flag",false)// - Indicates the current state of the Flag function block as a Boolean
Enumeration(PVSTS,"Process Variable (PV) Status",BAD,UNCERTAIN,NORMAL,MANUAL,Def=BAD)
StructBE(PVVALSTS,"",PVVAL,PVSTS)
Boolean(WIREOFF,"Indicates the absence",false)// of a field wire (sensor / actuator) or the break in field wiring.
Enumeration(IOMCONN,"Currently Assigned Channels",INCONN,OUTCONN,PCONN,Def=INCONN)
