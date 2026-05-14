#include "qtrends.h"
#include "ui_trends.h"

qtrends::qtrends(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::trends)
{
  ui->setupUi(this);
}

qtrends::~qtrends()
{
  delete ui;
}
