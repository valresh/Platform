#ifndef PARAMSTREE_H
#define PARAMSTREE_H
#include <QTreeView>
#include "Param.h"
#include <QStandardItemModel>

struct ParamsTree : public QTreeView
{
    CParams * pParams;
    int kParams;
    QStandardItemModel * pRoot;
    class ShowParams * pParamsList;
    ParamsTree(QWidget * Parent ) : QTreeView ( Parent )
    {
        pParamsList = NULL;
    }
    void Init ( int kParams, CParams * pParams, char * ObjName );
};

#endif // PARAMSTREE_H
