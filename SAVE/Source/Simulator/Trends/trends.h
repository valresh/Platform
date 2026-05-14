#ifndef TRENDS_H
#define TRENDS_H

#include <QMainWindow>

namespace Ui {
class trends;
}

class trends : public QMainWindow
{
    Q_OBJECT

  public:
    explicit trends(QWidget *parent = nullptr);
    ~trends();

  private:
    Ui::trends *ui;
};

#endif // TRENDS_H
