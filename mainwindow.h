#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //private Q_SLOTS:
private slots:
    void openPointCloudDirectory();
    void loadAnnotationFile();


private:
    void parseAnnotationFile();
    QList<QString> sortStringList(const QStringList &strlist);
    void loadAnnotations(const QModelIndex &index);
//    void modifyAnnotation(const QModelIndex &index);
//    void modifyAnnotation(const QModelIndex &current, const QModelIndex &previous);
    void modifyAnnotation(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());

    Ui::MainWindow *ui;

    QString pcl_dir_;
    QString anno_file_;
    QMap<QString, QStringList> annos_map_;
    QStringList current_annos_;
};
#endif // MAINWINDOW_H
