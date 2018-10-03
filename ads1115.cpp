#include "ads1115.h"

ads1115::ads1115(QObject *parent) : QThread(parent)
{

}
int ads1115::SetupAddr(int address)
{

   if((fd=open("/dev/i2c-1",O_RDWR))<0)
       return DeviceErr;
   if(ioctl(fd,I2C_SLAVE,address)<0)
       return AddressErr;
   return NoneErr;
}
int ads1115::SetupConfig(int InPutPin,int PGA,int SPS,int _FPS)
{
    uint8_t writeBuf[3]={1,CONFIG_OS_MASK_HIGH,CONFIG_OS_MASK_LOW};
    //SetPin
    switch (InPutPin) {
    case CONFIG_MUX_DIFF_0_1:
        writeBuf[1] |=CONFIG_MUX_DIFF_0_1;
        break;
    case CONFIG_MUX_DIFF_0_3:
         writeBuf[1] |=CONFIG_MUX_DIFF_0_3;
        break;
    case CONFIG_MUX_DIFF_1_3:
         writeBuf[1] |=CONFIG_MUX_DIFF_1_3;
        break;
    case CONFIG_MUX_DIFF_2_3:
         writeBuf[1] |=CONFIG_MUX_DIFF_2_3;
        break;
    case CONFIG_MUX_SINGLE_0:
         writeBuf[1] |=CONFIG_MUX_SINGLE_0;
        break;
    case CONFIG_MUX_SINGLE_1:
         writeBuf[1] |=CONFIG_MUX_SINGLE_1;
        break;
    case CONFIG_MUX_SINGLE_2:
         writeBuf[1] |=CONFIG_MUX_SINGLE_2;
        break;
    case CONFIG_MUX_SINGLE_3:
         writeBuf[1] |=CONFIG_MUX_SINGLE_3;
        break;
    default:
        break;
    }
    //SetPGA
    switch (PGA) {
    case CONFIG_PGA_6_144V:
           writeBuf[1]|= CONFIG_PGA_6_144V;
           VPS=6.144;
        break;
    case CONFIG_PGA_4_096V:
        writeBuf[1]|= CONFIG_PGA_4_096V;
        VPS=4.096;
        break;
    case CONFIG_PGA_2_048V:
        writeBuf[1]|= CONFIG_PGA_2_048V;
        VPS=2.048;
        break;
    case CONFIG_PGA_1_024V:
        writeBuf[1]|= CONFIG_PGA_1_024V;
        VPS=1.024;
        break;
    case CONFIG_PGA_0_512V:
        writeBuf[1]|= CONFIG_PGA_0_512V;
        VPS=0.512;
        break;
    case CONFIG_PGA_0_256V:
        writeBuf[1]|= CONFIG_PGA_0_256V;
        VPS=0.256;
        break;

    default:
        break;
    }

    //SetSPS
    switch (SPS) {
    case CONFIG_DR_8SPS:
         writeBuf[2] |=CONFIG_DR_8SPS;
        break;
    case CONFIG_DR_16SPS:
         writeBuf[2] |=CONFIG_DR_16SPS;
        break;
    case CONFIG_DR_32SPS:
         writeBuf[2] |=CONFIG_DR_32SPS;
        break;
    case CONFIG_DR_64SPS:
         writeBuf[2] |=CONFIG_DR_64SPS;
        break;
    case CONFIG_DR_128SPS:
         writeBuf[2] |=CONFIG_DR_128SPS;
        break;
    case CONFIG_DR_475SPS:
         writeBuf[2] |=CONFIG_DR_475SPS;
        break;
    case CONFIG_DR_860SPS:
         writeBuf[2] |=CONFIG_DR_860SPS;
        break;

    default:
        break;
    }
    if(write(fd,writeBuf,3)!=3)
        return WriteErr;
    FPS=_FPS;
    return NoneErr;
}
void ads1115::run()
{
    auto tstep=nanoseconds(1000000000/FPS);
    auto t0=steady_clock::now()+tstep;
    auto t1=steady_clock::now();
    auto t2=t1;
    while(1)
    {
        uint8_t readBuf[2];
        QMutex mutex;
        mutex.lock();
        if(read(fd,readBuf,2)!=2)
            Q_ASSERT(ReadErr);
        double Value=readBuf[0]<<8 | readBuf[1];
        Value*=VPS/bitResolution;
        mutex.unlock();
        t0+=tstep;
        t2=steady_clock::now();
        dsec delta=t2-t1;

        double  t=delta.count();
        emit GetSignal(Value,t);
        this_thread::sleep_until(t0);

        t1=t2;

    }
}
