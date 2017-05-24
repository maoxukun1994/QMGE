#ifndef MASSIVEGROUNDWINDOW_H
#define MASSIVEGROUNDWINDOW_H

#include <QMainWindow>
#include "src/qmge_app.h"

namespace Ui
{
class MassiveGroundWindow;
}

class MassiveGroundWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MassiveGroundWindow(QWidget *parent = 0);

    ~MassiveGroundWindow();

protected:

private slots:
    void on_pushButton_clicked();

private:

    QScopedPointer<QMGE_Core::QMGE_App> m_app;

    Ui::MassiveGroundWindow * ui;
};

#endif // MASSIVEGROUNDWINDOW_H
