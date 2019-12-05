#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//slots
void MainWindow::aboutSlot()
{
    QString title = QString("About ") + this->windowTitle();
    QString text = QString("This user inferface has been created for viewing crime data in Boston with both predicted data and actual Bostom crime data.");

    QMessageBox::about(this, title, text);
}
