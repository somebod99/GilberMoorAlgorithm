#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "decodewidjet.h"
#include "encodewidjet.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("icon.ico"));
    ui->centralwidget = new EncodeWidjet(this);
    ui->centralwidget->setGeometry(0, this->menuBar()->geometry().height(), this->geometry().width(), this->geometry().height());

    connect(ui->encode, SIGNAL(triggered()), this, SLOT(slotSetWidjet()));
    connect(ui->decode, SIGNAL(triggered()), this, SLOT(slotSetWidjet()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSetWidjet()
{
    delete this->ui->centralwidget;

    if(sender()->objectName() == "encode")
        this->ui->centralwidget = new EncodeWidjet(this);
    else
        this->ui->centralwidget = new DecodeWidjet(this);

    this->ui->centralwidget->setGeometry(0, this->menuBar()->geometry().height(), this->geometry().width(), this->geometry().height());
    this->ui->centralwidget->show();
}

