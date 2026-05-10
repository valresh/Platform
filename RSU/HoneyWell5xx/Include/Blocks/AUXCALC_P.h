String_A(CONFIGDESC,"Configuration Descriptor",39 * 4,8)
String_K(DESC,"Block Descriptor",132*4)
String_K(EUDESC,"Block Engineering Units Descriptor",24*4)
String_A(EXPR,"Calculator Expressions",511 * 4,8)
String_A(EXPRPCODE,"Expression Pseudo-code",220 * 4,8)
String_A(EXECDESC,"ExecCode Desc",39 * 4,10)
Enumeration(PVFORMAT,"Process Variable (PV) Display Format",D0,D1,D2,D3,Def=D1)//D0 (-XXXXXX.) D1 (-XXXXX.X) D2 (-XXXX.XX) D3 (XXX.XXX)
Enumeration(PVSRC,"Process Variable (PV) source",P1,P2,P3,P4,P5,P6,C1,C2,C3,C4,C5,C6,C7,C8,P1STS,P2STS,P3STS,P4STS,P5STS,P6STS,C1STS,C2STS,C3STS,C4STS,C5STS,C6STS,C7STS,C8STS,Def=P1)//Any of P[1..6], PVSTS, C[1..8] or CSTS[1..8]
Enumeration(PVSTSSRC,"P1STS",P1,P2,P3,P4,P5,P6,C1,C2,C3,C4,C5,C6,C7,C8,PSTS1,PSTS2,PSTS3,PSTS4,PSTS5,PSTS6,CSTS1,CSTS2,CSTS3,CSTS4,CSTS5,CSTS6,CSTS7,CSTS8,Def=PSTS1)//Any of C[1..8], CSTS[1..8], P[1..6], or PVSTS.
Enumeration_A(РасчетC,"",8,Честно,Брать_с_P0,Брать_с_P1,Брать_с_P2,Брать_с_P3,Брать_с_P4,Брать_с_P5,Брать_с_P6,Def=Честно)
Enumeration(HIALM,"",NoAlarm,Config,OPHi,OPLo,AdvDev,DevHi,DevLo,SafetyIlock,BadCtl,BadCompTerm,Def=NoAlarm)
Real_A(CONST,"Constant Value",8,NaN)
Integer_A(NUMSRCCONN,"Reference/Expression",8,0)
Integer_A(PCODESIZE,"Pcode size/expression",8,0)