#include "trendvarprop.h"
#include "ui_trendvarprop.h"
#include "Err.h"
#include "showtrends.h"

TrendVarProp::TrendVarProp(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::TrendVarProp)
{
  ui->setupUi(this);
}

TrendVarProp::~TrendVarProp()
{
  delete ui;
}

void OutDI( double V, char Txt[256]);

bool TrendVarProp::Dial( TrendVar * pVar )
  {
  char V[256];
  strcpy ( V, pVar->Name );
  char * P = strchr ( V, '.' );
  if ( P )
    {
    *P = 0;
    ui->Object->setText( V );
    ui->Param->setText( P + 1 );
    }
  else
    {
    ui->Object->setText( V );
    ui->Param->setText( "" );
    }
  char Txt[256];
  OutDI( pVar->Max, Txt );
  ui->Max->setText( Txt );
  OutDI( pVar->Min, Txt );
  ui->Min->setText( Txt );
  OutDI( pVar->Nom, Txt );
  ui->Nom->setText( Txt );
//  ui->color->color = pVar->color;
  exec();
  int Res = result();
  if ( Res == 0 )
    return false;
//  pVar->color = ui->color->color;
  KKK();
  return true;
  }
