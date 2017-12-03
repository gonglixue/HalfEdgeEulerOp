#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLoad_Brep_File, SIGNAL(triggered(bool)), this, SLOT(LoadBrepFile()));
}

void MainWindow::LoadBrepFile()
{
    qDebug() << "load file action slot in mainwindow";
    QString fn = QFileDialog::getOpenFileName(
                this,
                tr("Open A Brep File"),
                QString(),
                tr("Brep File(*.brep)")
                );
    if(!fn.isEmpty()){
        ui->glwidget->ConstructBrep(fn);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
