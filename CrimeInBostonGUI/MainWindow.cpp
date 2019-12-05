#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openSlot()));
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

void MainWindow::openSlot()
{
    int accepted;
    QFileDialog* fd = new QFileDialog(this);

    fd->setWindowTitle(QString("Load a Crime CSV file"));
    fd->setNameFilter(QString("CSV files (*.csv)"));
    fd->setAcceptMode(QFileDialog::AcceptOpen);
    fd->setFileMode(QFileDialog::ExistingFile);
    accepted = fd->exec();
    if (accepted == QDialog::Accepted)
    {
        QStringList list = fd->selectedFiles();
        if (list.size() > 0 )
        {
            const QString filepath = list.first();
            int error_code;

            //open the file here
            error_code = ui->bv_view->loadCrimeCSV(filepath);
            if (error_code)
            {
                QString title, message;
                title = QString("Failed to open file");
                message = QString("Failed to open file \"");
                message.append(filepath);
                message.append("\" for reading");
                QMessageBox::warning(this, title, message); //display an file open error
            }
        }
    }

    delete fd;
}
