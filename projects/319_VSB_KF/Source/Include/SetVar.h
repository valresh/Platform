#pragma once


#define IO_2(pFlow_in,pFlow_out) \
		if ( Var[F_0] >= 0. )\
			{\
			pFlow_in = (CFlow*)pVar[0];\
			pFlow_out = (CFlow*)pVar[1];\
			}\
		else\
			{\
			pFlow_in = (CFlow*)pVar[1];\
			pFlow_out = (CFlow*)pVar[0];\
			}

#define IO_2M(pFlow_in,pFlow_out) \
		pFlow_in = (CFlow*)pVar[0];\
		pFlow_out = (CFlow*)pVar[1];

#define IO_1(pFlow) \
		pFlow = (CFlow*)pVar[0];
