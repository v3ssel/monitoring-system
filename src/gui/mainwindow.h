#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private:
    QPropertyAnimation* fadeOut(QWidget* widget, int msec);
    void fadeIn(QWidget* widget, int msec);

    Ui::MainWindow *ui;
};

#endif // _MAINWINDOW_H_
