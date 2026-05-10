String_K(NAME,"Имя",32*4)
String_K(DESC,"Description",132*4)
Enumeration(BOOLVALUEOFF,"",Enum0,Enum1,Enum2,Enum3,Enum4,Enum5,Enum6,Enum7,Enum8,Enum9,Enum10\
            ,Enum11,Enum12,Enum13,Enum14,Enum15,Enum16,Enum17,Enum18,Enum19,Enum20\
            ,Enum21,Enum22,Enum23,Enum24,Enum25,Enum26,Enum27,Enum28,Enum29,Enum30\
            ,Enum31,Enum32,Enum33,Enum34,Enum35,Enum36,Enum37,Enum38,Enum39,Enum40\
            ,Enum41,Enum42,Enum43,Enum44,Enum45,Enum46,Enum47,Enum48,Enum49,Enum50\
            ,Enum51,Enum52,Enum53,Enum54,Enum55,Enum56,Enum57,Enum58,Enum59,Enum60\
            ,Enum61,Enum62,Enum63,Def=Enum0)//TCGENERICENUM
Enumeration(BOOLVALUEON,"",Enum0,Enum1,Enum2,Enum3,Enum4,Enum5,Enum6,Enum7,Enum8,Enum9,Enum10\
            ,Enum11,Enum12,Enum13,Enum14,Enum15,Enum16,Enum17,Enum18,Enum19,Enum20\
            ,Enum21,Enum22,Enum23,Enum24,Enum25,Enum26,Enum27,Enum28,Enum29,Enum30\
            ,Enum31,Enum32,Enum33,Enum34,Enum35,Enum36,Enum37,Enum38,Enum39,Enum40\
            ,Enum41,Enum42,Enum43,Enum44,Enum45,Enum46,Enum47,Enum48,Enum49,Enum50\
            ,Enum51,Enum52,Enum53,Enum54,Enum55,Enum56,Enum57,Enum58,Enum59,Enum60\
            ,Enum61,Enum62,Enum63,Def=Enum0)//TCGENERICENUM
String_A(SDENUMTEXT,"Enumeration Text",12 * 4,64)
Real(THRESHOLD,"Threshold for converting floating point values to boolean values",0.001)//If the input value is larger-than or equal-to THRESHOLD, then the OUT is TRUE. Otherwise it is FALSE
                                                                                        //If the THRESHOLD = NaN, for any nonzero input value, OUT is TRUE. Otherwise it is FALSE
Enumeration(TRUNCATEOPT,"Choice for truncating or rounding integers",Truncate,Round,Def=Truncate)
