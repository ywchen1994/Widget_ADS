#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<ads1115.h>
#include<qcustomplot.h>
#include<valarray>
#define DataLength (2048)
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     ads1115 *ADC;
private slots:
    void on_Btn_Start_clicked();
    void OnGetValue(double val,double tim);
private:
    Ui::MainWindow *ui;

    valarray<double> QIn;
    valarray<double> TimeRaw;
    uint ReadDataIndex=0;
};

#endif // MAINWINDOW_H
