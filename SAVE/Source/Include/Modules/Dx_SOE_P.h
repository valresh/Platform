String_K(NAME,"Имя",31*4)
String_K(DESC,"Block Descriptor",132*4)
String_A(CHNLNAME,"Channel Name",16 * 4,32)//Identifies the configured name of the channels associated with this IOM block.
Integer( IOMCARDA, "Card A(1-15)", 1 )//Enter the card or slot number of the IOM connected to FTA connector A.
                                      //For AO8 IOM type, IOM A will be the preferred primary
Integer( IOMCARDB, "Card B(1-15)", 1 )//Enter the card or slot number of the IOM connected to FTA connector B.
                                      //For AO8 IOM type, IOM A will be the preferred primary
Integer( IOMFILEA, "IOM File A(1-8)", 1)//Enter the file number of the IOM connected to FTA connector A.
                                        //For AO8 IOM type, IOM A will be the preferred primary.
Integer( IOMFILEB, "IOM File B(1-8)", 1)//Enter the file number of the IOM connected to FTA connector B.
                                        //For AO8 IOM type, IOM A will be the preferred primary.
String_K( IOMFWREVA, "Input/Output Module (IOM) Firmware Revision", 12 )//This is the IOM card firmware revision status in the form V.R.B, where V = Version and R = Revision and B is Build ID.
String_K( IOMFWREVB, "Input/Output Module (IOM) Firmware Revision", 12 )//This is the IOM card firmware revision status in the form V.R.B, where V = Version and R = Revision and B is Build ID.
String_K( IOMHWREVA, "Input/Output Module (IOM) Hardware Revision", 12 )//This is the IOM card hardware revision status.
String_K( IOMHWREVB, "Input/Output Module (IOM) Hardware Revision", 12 )//This is the IOM card hardware revision status.
Integer( IOMNUM,"I/O Module Number (0-40)", 0 )//This parameter specifies the address of the IOM on the I/O Link. The value must be unique on a specific I/O Link, and its user-entered value must be between 1 and 40. The value must also match the IOLINK address tile setting on the IOTA related to this IOM.
                                              //Note that the default value of 0 is assigned when the IOM is created. However, valid values for IOMs that communicate on the IOLINK are from 1 to 40.
Boolean( IOREDOPT, "This IOM is redundant ", 0 )//Specifies if IOM is redundant.
                                                //0 - Off: The IOM is not redundant
                                                //1 - On: The IOM is redundant
String_K( IOMTYPE, "Module Type", 64 )//Note that there is a general format for IOMTYPE descriptor strings: Module type, number of channels, electrical characteristics, auxiliary descriptor.
Integer( NUMCHANS, "Number of Channels", 0 )
Enumeration( DIMODE,"DI Mode",Normal,Soe,LowLatency,Def=Soe)
String_K(IOPLOCATION,"IOM Location",64*4)

String_K(SCANASSOCDSP,"SCAN Associated Display",255*4)
Integer( SCANCTRLLVL, "Server Security Level", 200)
String_K(SCANGRPDTL,"SCAN Group Detail Page",255*4)
String_K(SCANPNTDTL,"SCAN Point Detail Page",255*4)
/*SCANRATE
SELECTCABLEACMD
SELECTCABLEBCMD
STMCHLASTOVERRUNTIMEA (Partner A)
STMCHLASTOVERRUNTIMEB (Partner B)
STMCHMAXOVRRUNTIMEA (Partner A)
STMCHMAXOVRRUNTIMEA (Partner B)
STMCHOVRRUNSA (Partner A)
STMCHOVRRUNSB (Partner B)
SWAPPRIMARYCMD
TYPEINVALIDA
TYPEINVALIDB
WARMSTRTA
WARMSTRTB
*/