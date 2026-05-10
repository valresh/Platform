#include <rsuFormats.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <string.h>

void DblToStr(std::string& s, const double d)
{
	double ad = fabs(d);
	char buf[32];
	const char* fmt = NULL;
	if (d == 0) {
		s = "0";
		return;
	}
	else if (ad > 1e5 || ad < 1e-3)
	{
		fmt = "%.1e";
	}
	else {
		if (d > 100)
			fmt = "%.0f";
		else if (d > 10)
			fmt = "%.1f";
		else if (d > 1)
			fmt = "%.2f";
		else
			fmt = "%.3f";
	}
	sprintf(buf, fmt, d);
	char* c = strchr(buf, ',');
	if (c)
		*c = '.';
	s = buf;
}
