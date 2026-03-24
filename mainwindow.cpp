#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include "fractalwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    auto *layout = new QVBoxLayout(ui->centralwidget);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *fractal = new FractalWidget(ui->centralwidget);
    layout->addWidget(fractal);
}

MainWindow::~MainWindow()
{
    delete ui;
}
