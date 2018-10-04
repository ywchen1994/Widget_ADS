#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<ads1115.h>
#include<qcustomplot.h>
#include<QTimer>
#include<QTime>
#include<valarray>
#define DataLength (2048)
#include<complex>
#include<math.h>
using namespace std;
typedef std::complex<double> Complex;
typedef unsigned int uint;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_Btn_Start_clicked();
    void OnGetValue(double val,double tim);
    void ShowData();
private:
    void ZeroMean(valarray<double> &x,valarray<double> &X);
    uint SR=860;
    Ui::MainWindow *ui;
    ads1115 *ADC;
    QTimer *mTimer;
    valarray<double> QIn;
    valarray<double> TimeRaw;
    uint ReadDataIndex=0;
    void fft(valarray<double> x, valarray<Complex> &X);
};

#endif // MAINWINDOW_H
