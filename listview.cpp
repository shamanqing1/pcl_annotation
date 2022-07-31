#include "listview.h"

ListView::ListView(QWidget *parent)
    :QListView(parent)
{

}

void ListView::keyPressEvent(QKeyEvent *event)
{
    QModelIndex oldIdx = currentIndex();
    QListView::keyPressEvent(event);
    QModelIndex newIdx = currentIndex();
    if(oldIdx.row() != newIdx.row())
    {
        emit clicked(newIdx);
    }
}
