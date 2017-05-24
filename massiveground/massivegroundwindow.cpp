#include "massivegroundwindow.h"
#include "ui_massivegroundwindow.h"

MassiveGroundWindow::MassiveGroundWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MassiveGroundWindow)
{
    ui->setupUi(this);

    m_app.reset(new QMGE_Core::QMGE_App());

    ui->widget->createWindowContainer(m_app->createWindow(QSize(ui->widget->width(),ui->widget->height()),false).data(),ui->widget);\

    m_app->run();
}

MassiveGroundWindow::~MassiveGroundWindow()
{
    delete ui;
}

void MassiveGroundWindow::on_pushButton_clicked()
{

}
