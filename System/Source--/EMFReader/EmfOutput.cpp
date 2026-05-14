/*
  Copyright 2008 Brad Hards  <bradh@frogmouth.net>
  Copyright 2009 Inge Wallin <inge@lysator.liu.se>
  Copyright 2015 Ion Vasilief <ion_vasilief@yahoo.fr>
*/

#include "EmfOutput.h"
#include "EmfParser.h"

namespace QEmf
{

bool AbstractOutput::load(struct SS * pSS, const QByteArray &contents)
{
	EmfParser parser;
	parser.setOutput(this);
    return parser.load( pSS, contents);
}

bool AbstractOutput::load( struct SS * pSS, const QString& fileName)
{
	EmfParser parser;
	parser.setOutput(this);
  return parser.load( pSS, QString(fileName));
}

} // xnamespace...
