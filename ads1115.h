#ifndef ADS1115_H
#define ADS1115_H
#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<QDebug>
#include<QThread>
#include<sys/ioctl.h>
#include<linux/i2c-dev.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<inttypes.h>
#include<chrono>
#include<sys/time.h>
#include<wiringPi.h>
#include<fcntl.h>
#include<thread>
#include<QtCore>
#include <QObject>
using namespace std;
using namespace std::chrono;
using dsec=duration<double>;
const double bitResolution=32768.0;
enum ErrorType
{
    NoneErr,
    DeviceErr,
    AddressErr,
    WriteErr,ReadErr

};
#define	CONFIG_OS_MASK_HIGH		(0x80)	// Operational Status Register
#define	CONFIG_OS_MASK_LOW		(0x03)	// Operational Status Register
// The multiplexor
#define	CONFIG_MUX_MASK		(0x70)

// Differential modes

#define	CONFIG_MUX_DIFF_0_1	(0x00)	// Pos = AIN0, Neg = AIN1 (default)
#define	CONFIG_MUX_DIFF_0_3	(0x10)	// Pos = AIN0, Neg = AIN3
#define	CONFIG_MUX_DIFF_1_3	(0x20)	// Pos = AIN1, Neg = AIN3
#define	CONFIG_MUX_DIFF_2_3	(0x30)	// Pos = AIN2, Neg = AIN3 (2nd differential channel)

// Single-ended modes

#define	CONFIG_MUX_SINGLE_0	(0x40)	// AIN0
#define	CONFIG_MUX_SINGLE_1	(0x50)	// AIN1
#define	CONFIG_MUX_SINGLE_2	(0x60)	// AIN2
#define	CONFIG_MUX_SINGLE_3	(0x70)	// AIN3

// Programmable Gain Amplifier

#define	CONFIG_PGA_6_144V	(0x00)	// +/-6.144V range = Gain 2/3
#define	CONFIG_PGA_4_096V	(0x02)	// +/-4.096V range = Gain 1
#define	CONFIG_PGA_2_048V	(0x04)	// +/-2.048V range = Gain 2 (default)
#define	CONFIG_PGA_1_024V	(0x06)	// +/-1.024V range = Gain 4
#define	CONFIG_PGA_0_512V	(0x08)	// +/-0.512V range = Gain 8
#define	CONFIG_PGA_0_256V	(0x0A)	// +/-0.256V range = Gain 16
// Data Rate

#define	CONFIG_DR_8SPS		(0x00)	//   8 samples per second
#define	CONFIG_DR_16SPS		(0x20)	//  16 samples per second
#define	CONFIG_DR_32SPS		(0x40)	//  32 samples per second
#define	CONFIG_DR_64SPS		(0x80)	//  64 samples per second
#define	CONFIG_DR_128SPS	(0xA0)	// 128 samples per second (default)
#define	CONFIG_DR_475SPS	(0xC0)	// 475 samples per second
#define	CONFIG_DR_860SPS	(0xE0)	// 860 samples per second

#define	CONFIG_DEFAULT_HIGHByte		(0x84)	// From the datasheet,Modify to Continuous mode
#define	CONFIG_DEFAULT_LOWByte	(0x83)


class ads1115:public QThread
{
     Q_OBJECT
public:
    explicit ads1115(QObject *parent = nullptr);

    int SetupAddr(int address);
    int SetupConfig(int InPutPin,int PGA,int SPS,int _FPS);
    void run();
private:
    //auto tstep;//=nanoseconds(0);
    double VPS=4.096/bitResolution;
    int fd;
    int FPS;
signals:
    void GetSignal(double,double);
};

#endif // ADS1115_H
