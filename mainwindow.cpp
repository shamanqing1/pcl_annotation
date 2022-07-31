#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>
#include <QList>
#include <QStringListModel>
#include <QListView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionPointCloud_Directory, &QAction::triggered, this, &MainWindow::openPointCloudDirectory);
    connect(ui->actionAnnotation_File, &QAction::triggered, this, &MainWindow::loadAnnotationFile);
    connect(ui->listView, &QListView::clicked, this, &MainWindow::loadAnnotations);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openPointCloudDirectory()
{
    pcl_dir_ = QFileDialog::getExistingDirectory(this, tr("Open pointcloud directory ..."), QDir::home().absolutePath());
    qDebug() << "pointcloud dir: " << pcl_dir_;
}

void MainWindow::loadAnnotationFile()
{
    anno_file_ = QFileDialog::getOpenFileName(this, tr("Load annotation file ..."), QDir::home().absolutePath(),
                                              tr("(*.txt *.json *.xml)"));
    qDebug() << "annotation file: " << anno_file_;

    if(!anno_file_.isEmpty())
    {
        parseAnnotationFile();
    }
}

void MainWindow::parseAnnotationFile()
{
    QFile file(anno_file_);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open file:" << anno_file_;
        return;
    }

    QTextStream stream(&file);

    QString line;
    while (stream.readLineInto(&line))
    {
        QStringList items = line.split(' ');
        annos_map_[items[0]].append(items[1]);
    }

    QStringListModel *model = new QStringListModel(this);

    model->setStringList(sortStringList(annos_map_.keys()));

    ui->listView->setModel(model);
    ui->listView->setCurrentIndex(model->index(0, 0));

    emit ui->listView->clicked(ui->listView->currentIndex());

}

QList<QString> MainWindow::sortStringList(const QStringList &strlist)
{
    QStringList retlist = strlist;
    std::sort(retlist.begin(), retlist.end(), [](const QString x, const QString y){

        QString new_x;
        QString new_y;
        if (x[0] == '-' &&  y[0] != '-')
        {
            return false;
        }
        else if (x[0] != '-' &&  y[0] == '-')
        {
            return true;
        }
        else if (x[0] == '-' &&  y[0] == '-')
        {
            new_x = x.right(1);
            new_y = y.right(1);
        }
        else
        {
            new_x = x;
            new_y = y;
        }

        if(new_x.length() != new_y.length())
        {
            return new_x.length() < new_y.length();
        }
        else {
            return new_x < new_y;
        }
    });

    return retlist;
}

void MainWindow::loadAnnotations(const QModelIndex &index)
{
    QString item = ui->listView->model()->data(index).toString();
    qDebug() << annos_map_[item];

    QStringListModel *model = new QStringListModel(this);

    current_annos_ = sortStringList(annos_map_[item]);

    model->setStringList(current_annos_);

    ui->listView1->setModel(model);
    ui->listView1->setCurrentIndex(model->index(0, 0));

    connect(model, &QAbstractItemModel::dataChanged, this, &MainWindow::modifyAnnotation);
}



//void MainWindow::loadAnnotations()
//{
//    static int i;
//    qDebug() << "Entered :" << i++;
//}
//void MainWindow::modifyAnnotation(const QModelIndex &current, const QModelIndex &previous)
//{
//    qDebug() << current <<": " << previous;
//}


//[signal] void QAbstractItemModel::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>())


void MainWindow::modifyAnnotation(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    QString old_anno = current_annos_.at(topLeft.row());
    QString new_anno = ui->listView1->model()->data(topLeft).toString();
    qDebug() << old_anno << new_anno;
}
