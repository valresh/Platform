//Integer(ORDERINCM,"Execution order",10)
Real(DEADBAND1,"",0.005)
Real(DEADBAND2,"",0.005)
Boolean(INBADOPT,"",false)// - Configurable value that defines the output fail-safe value when any input is Not a Number (NaN). This is required since it is not specified whether the comparison is ordered or unordered.
Integer(NUMOFINPUTS,"Number of configured inputs",2)
Real(TP,"Trip Point Value",0.5)//Used to compare against constant values, when only one input is configured
