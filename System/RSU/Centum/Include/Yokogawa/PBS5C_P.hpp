//
YPARM_S(Comment,64,"ETCM")
YPARM_I_A(SWCR,5,"")//цвета кнопок 
YPARM_I_A(SWST,5,"")//моргание ламп	(1)
YPARM_I_A(SWOP,5,"")//отключение > 0
//
YPARM_S_A(AKLB,5,16,"")//подтверждение
YPARM_S_A(SWLB,5,16,"")//метки кнопок(char[8]??)
YPARM_L( Type_Action, 0,"BTA2", "%{RADIO;MOMENT;ALTER}" )// Button Action
YPARM_I( TimeW, 2, "MBOT" )//
