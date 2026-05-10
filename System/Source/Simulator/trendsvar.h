#ifndef TRENDSVAR_H
#define TRENDSVAR_H

#include <QMainWindow>

namespace Ui {
class TrendsVar;
}

#include <QTableView>
#include <QTimer>

class TrendsVarModel : public QAbstractTableModel
{
    Q_OBJECT
  public:
    class ShowTrends * pTrends;
    struct TrendVar * pVars;
    int kVars;
    int kVars_old;
    bool IsData;
    static bool NoRefresh;
    QTimer * timer;
    explicit TrendsVarModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void timerHit();
  };


class TrendsVar : public QMainWindow
{
    Q_OBJECT

  public:
    explicit TrendsVar(QWidget *parent = nullptr);
    ~TrendsVar();
    TrendsVarModel Model;
    ShowTrends * pDraw;
    void Recv ( ShowTrends * pDraw );
    void show( TrendVar * _pVars, int _kVars);
  private slots:
    void on_Wnd_clicked(const QModelIndex &index);

    void on_Wnd_doubleClicked(const QModelIndex &index);

    void on_Wnd_viewportEntered();

  private:
    Ui::TrendsVar *ui;
};

#endif // TRENDSVAR_H
