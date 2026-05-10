String_K(NAME,"",32*4)
String_K(DESC,"Description",132*4)
Enumeration(BOOLVALUEOFF,"Value OFF mapped to Enum",Enum0,Enum1,Enum2,Enum3,Enum4,Enum5,Enum6,Enum7,Enum8,Enum9,Enum10\
            ,Enum11,Enum12,Enum13,Enum14,Enum15,Enum16,Enum17,Enum18,Enum19,Enum20\
            ,Enum21,Enum22,Enum23,Enum24,Enum25,Enum26,Enum27,Enum28,Enum29,Enum30\
            ,Enum31,Enum32,Enum33,Enum34,Enum35,Enum36,Enum37,Enum38,Enum39,Enum40\
            ,Enum41,Enum42,Enum43,Enum44,Enum45,Enum46,Enum47,Enum48,Enum49,Enum50\
            ,Enum51,Enum52,Enum53,Enum54,Enum55,Enum56,Enum57,Enum58,Enum59,Enum60\
            ,Enum61,Enum62,Enum63,Def=Enum0)
Enumeration(BOOLVALUEON,"Value ON mapped to Enum",Enum0,Enum1,Enum2,Enum3,Enum4,Enum5,Enum6,Enum7,Enum8,Enum9,Enum10\
            ,Enum11,Enum12,Enum13,Enum14,Enum15,Enum16,Enum17,Enum18,Enum19,Enum20\
            ,Enum21,Enum22,Enum23,Enum24,Enum25,Enum26,Enum27,Enum28,Enum29,Enum30\
            ,Enum31,Enum32,Enum33,Enum34,Enum35,Enum36,Enum37,Enum38,Enum39,Enum40\
            ,Enum41,Enum42,Enum43,Enum44,Enum45,Enum46,Enum47,Enum48,Enum49,Enum50\
            ,Enum51,Enum52,Enum53,Enum54,Enum55,Enum56,Enum57,Enum58,Enum59,Enum60\
            ,Enum61,Enum62,Enum63,Def=Enum0)
Boolean_A(ENUMBOOLMAP,"Enum-Bool Map",64,false)
Real(INPUTCHGDB,"Input change deadband",0)
Integer(ORDERINCM,"Execution Order in CM",10)
//STRUCT(OUT)
//  FLAG_S(OUT,BOOLEAN,"Boolean Output Value")
//  ENUM_S(OUT,ENUM,"Enumerated Output",ENUM0,ENUM1,ENUM2,ENUM3,ENUM4,ENUM5,ENUM6,ENUM7,ENUM8,ENUM9,ENUM10,ENUM11,ENUM12,ENUM13,ENUM14,ENUM15,ENUM16,ENUM17,ENUM18,ENUM19,ENUM20,ENUM21,ENUM22,ENUM23,ENUM24,ENUM25,ENUM26,ENUM27,ENUM28,ENUM29,ENUM30,ENUM31,ENUM32,ENUM33,ENUM34,ENUM35,ENUM36,ENUM37,ENUM38,ENUM39,ENUM40,ENUM41,ENUM42,ENUM43,ENUM44,ENUM45,ENUM46,ENUM47,ENUM48,ENUM49,ENUM50,ENUM51,ENUM52,ENUM53,ENUM54,ENUM55,ENUM56,ENUM57,ENUM58,ENUM59,ENUM60,ENUM61,ENUM62,ENUM63,Def=)
//  DOUBLE_S(OUT,FLOAT32,"32-bit floating point Output")
//  DOUBLE_S(OUT,FLOAT64,"64-bit floating point Output")
//  INTEGER_S(OUT,INT16,"Signed 16-bit Integer Output")
//  INTEGER_S(OUT,INT32,"Signed 32-bit Integer Output")
//  STRING_S(OUT,SDENUM,64,"Self-Defining Enum Output")
//  INTEGER_S(OUT,UINT32,"Unsigned 32-bit Integer Output")
//STRUCT_END(OUT)
String_A(SDENUMTEXT,"Enum Text (12 char max)",12 * 4,64)
Boolean(STOREENB,"Output Store Enable",true)
Boolean(STORONCHGENB,"Store on change Enable",false)
Real(THRESHOLD,"Threshold Value",0)
Enumeration(TRUNCATEOPT,"Truncate Option",Truncate,Round,Def=Truncate)
