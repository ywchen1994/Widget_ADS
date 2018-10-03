#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ads1115.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ADC=new ads1115(this);
    ui->setupUi(this);
    QIn.resize(DataLength);
    TimeRaw.resize(DataLength);
    connect(ADC,SIGNAL(GetSignal(double,double)),this,SLOT(OnGetValue(double,double)));
    ui->Chart_Singal_Plot->addGraph();
    ui->Chart_Singal_Plot->graph(0)->setPen(QPen(QColor(40,110,255)));
    QSharedPointer<QCPAxisTickerTime>timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->Chart_Singal_Plot->xAxis->setTicker(timeTicker);
    ui->Chart_Singal_Plot->axisRect()->setupFullAxesBox();
    ui->Chart_Singal_Plot->yAxis->setRange(-2,1);
    connect(ui->Chart_Singal_Plot->xAxis,SIGNAL(rangeChanged(QCPRange)),ui->Chart_Singal_Plot->xAxis2,SLOT(setRange(QCPRange)));
    connect(ui->Chart_Singal_Plot->yAxis,SIGNAL(rangeChanged(QCPRange)),ui->Chart_Singal_Plot->yAxis2,SLOT(setRange(QCPRange)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Btn_Start_clicked()
{
    ADC->SetupAddr(0x49);
    ADC->SetupConfig(CONFIG_MUX_DIFF_0_1,CONFIG_PGA_4_096V,CONFIG_DR_860SPS,860);
    ADC->start();
}
void MainWindow::OnGetValue(double val,double tim)
{
    //ui->label->setText(QString::number(Number));
    QIn[ReadDataIndex]=val;
    TimeRaw[ReadDataIndex]=tim;
    ReadDataIndex++;
    cout<<tim<<endl;
}
