#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ads1115.h>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ADC=new ads1115(this);
    ui->setupUi(this);
    QIn.resize(DataLength);
    TimeRaw.resize(DataLength);
    mTimer=new QTimer(this);
    mTimer->setTimerType(Qt::PreciseTimer);
    mTimer->setInterval(100);
    connect(mTimer,SIGNAL(timeout()),this,SLOT(ShowData()));
    connect(ADC,SIGNAL(GetSignal(double,double)),this,SLOT(OnGetValue(double,double)));

    //Signal Plot Setting

    {

        ui->Chart_Singal_Plot->addGraph();
        ui->Chart_Singal_Plot->graph(0)->setPen(QPen(Qt::blue));
        ui->Chart_Singal_Plot->graph(0)->setBrush(QBrush(QColor(0,0,255,20)));
        ui->Chart_Singal_Plot->xAxis2->setVisible(true);
        ui->Chart_Singal_Plot->xAxis2->setTickLabels(false);
        ui->Chart_Singal_Plot->yAxis2->setVisible(true);
        ui->Chart_Singal_Plot->yAxis2->setTickLabels(false);
        connect(ui->Chart_Singal_Plot->xAxis,SIGNAL(rangeChanged(QCPRange)),ui->Chart_Singal_Plot->xAxis2,SLOT(setRange(QCPRange)));
        connect(ui->Chart_Singal_Plot->yAxis,SIGNAL(rangeChanged(QCPRange)),ui->Chart_Singal_Plot->yAxis2,SLOT(setRange(QCPRange)));
        ui->Chart_Singal_Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


    }
    //FFT Plot Setting
    {
        ui->Chart_FFTPlot->addGraph();
        ui->Chart_FFTPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->Chart_FFTPlot->graph(0)->setBrush(QBrush(QColor(0,0,255,20)));
        ui->Chart_FFTPlot->xAxis2->setVisible(true);
        ui->Chart_FFTPlot->xAxis2->setTickLabels(false);
        ui->Chart_FFTPlot->yAxis2->setVisible(true);
        ui->Chart_FFTPlot->yAxis2->setTickLabels(false);
        connect(ui->Chart_FFTPlot->xAxis,SIGNAL(rangeChanged(QCPRange)),ui->Chart_FFTPlot->xAxis2,SLOT(setRange(QCPRange)));
        connect(ui->Chart_FFTPlot->yAxis,SIGNAL(rangeChanged(QCPRange)),ui->Chart_FFTPlot->yAxis2,SLOT(setRange(QCPRange)));
        ui->Chart_FFTPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_Btn_Start_clicked()
{
   int a= ADC->SetupAddr(0x49);
   int b=ADC->SetupConfig(CONFIG_MUX_DIFF_0_1,CONFIG_PGA_4_096V,CONFIG_DR_860SPS,SR);
    ADC->start();
}
void MainWindow::OnGetValue(double val,double tim)
{
    //ui->label->setText(QString::number(Number));
    QIn[ReadDataIndex]=val;
    TimeRaw[ReadDataIndex]=tim;
    ReadDataIndex++;
    cout<<tim<<endl;
    if(ReadDataIndex==(DataLength))
    {
        if(mTimer->isActive()!=true) mTimer->start();
        ReadDataIndex=DataLength/2;
        for(uint_fast16_t i=0;i<DataLength;i++)
        {
            QIn[i]=QIn[ReadDataIndex+i];
            TimeRaw[i]=TimeRaw[ReadDataIndex+i];
        }

    }
}
void MainWindow::ZeroMean(valarray<double> &x,valarray<double> &X){
    X.resize(x.size());
   double sum=0;
    for(uint i=0;i<x.size();i++)
        sum+=x[i];
    for(uint i=0;i<x.size();i++)
        X[i]=(x[i]-sum/x.size());
}
void MainWindow::fft(valarray<double> x, valarray<Complex> &X)
{
    unsigned int N = x.size(), k = N, n;
    X.resize(N);
    double thetaT = M_PI / N;
    Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
    for(uint i=0;i<N;i++)
      X[i]=Complex(x[i],0);


    while (k > 1)
    {
        n = k;
        k >>= 1;
        phiT = phiT * phiT;
        T = 1.0L;
        for (uint l = 0; l < k; l++)
        {
            for (uint a = l; a < N; a += n)
            {
                uint b = a + k;
                Complex t = X[a] - X[b];
                X[a] += X[b];
                X[b] = t * T;
            }
            T *= phiT;
        }
    }
    // Decimate
    uint m = (uint)log2(N);
    for (uint a = 0; a < N; a++)
    {
        uint b = a;
        // Reverse bits
        b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
        b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
        b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
        b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
        b = ((b >> 16) | (b << 16)) >> (32 - m);
        if (b > a)
        {
            Complex t = X[a];
            X[a] = X[b];
            X[b] = t;
        }
    }
}
void MainWindow::ShowData()
{
    //FFT Plot
    {
        valarray<Complex> FFt_Out;
        QVector<double>Amplitude,Frequency;
        valarray<double> Zm;
        ZeroMean(QIn,Zm);
        fft(Zm,FFt_Out);
        for(uint i=0;i<FFt_Out.size()/2;i++)
        {
            double A=sqrt(pow(FFt_Out[i].real(),2)+pow(FFt_Out[i].imag(),2));
            Amplitude.push_back(A);
            Frequency.push_back((SR*i)/DataLength);
        }
        ui->Chart_FFTPlot->graph(0)->setData(Frequency,Amplitude);
        ui->Chart_FFTPlot->replot();
    }
    //Signal Plot
    {
        QVector<double>t,V;
        double mean=0;
        for(uint i=0;i<TimeRaw.size();i++)
        {
            V.push_back(QIn[i]);
            t.push_back(i);
            mean+=TimeRaw[i];
        }
        mean/=DataLength;
        ui->Chart_Singal_Plot->graph(0)->setData(t,V);
        ui->Chart_Singal_Plot->replot();

        ui->label_2->setText(QString::number(mean));
    }
}
