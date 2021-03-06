#include "ControlInterface.h"

ControlInterface::ControlInterface(HardwareSerial *ser)    {
    serial=ser;
}

int ControlInterface::begin()   {
    serial->begin(baudrate);
    serial->setTimeout(serial_timeout);
    return 1;
}

void ControlInterface::setBaudrate(long brate)  {
    baudrate=brate;
}

void ControlInterface::setTimeout(long tm)   {
    serial_timeout=tm;
}

void ControlInterface::flushReadBuffer()    {
    while(serial->available()>0)    {
        char c=serial->read();
    }
}

int ControlInterface::fetchData()  {
    if(serial->available()>=control_packet_size)    {
        int tmp[ctrl_size];
        byte high_byte;
        byte low_byte;
        for(int i=0;i<ctrl_size;i++)    {
            high_byte=serial->read();
            low_byte=serial->read();
            tmp[i]=(int16_t)((high_byte<<8)+low_byte);
        }
        // load received control values into respective variables
        if(tmp[8]==0)   {       //update control fields only if "reset" is 0
            drive_mode=tmp[0];
            direction=tmp[1];
            speed=tmp[2];
            distance=tmp[3];
            target_x=tmp[4];
            target_y=tmp[5];
        }        
        system_time=(tmp[6]<<16)+tmp[7];
        reset|=tmp[8];
        return 1;
    }
    else    {
        return 0;
    }
}

void ControlInterface::sendUpdates()    {
    send_integer(alert);
    send_integer(x_axis);
    send_integer(y_axis);
    send_integer(rover_heading);
    send_integer(total_distance>>16);
    send_integer(total_distance&65535);
}

int ControlInterface::getDriveMode() const  {
    return drive_mode;
}

int ControlInterface::getDirection() const  {
    return direction;
}

int ControlInterface::getSpeed() const  {
    return speed;
}

int ControlInterface::getDistance() const   {
    return distance;
}

int ControlInterface::getTargetX() const    {
    return target_x;
}

int ControlInterface::getTargetY() const    {
    return target_y;
}

unsigned long ControlInterface::getSystemTime() const   {
    return system_time;
}

int ControlInterface::getReset()   {  //sets the value of "reset" to 0 upon calls
    int tmp=reset;
    reset=0;
    return tmp;
}

void ControlInterface::writeAlert(int alrt) {
    alert=alrt;
}

void ControlInterface::writeAxisX(int x)    {
    x_axis=x;
}

void ControlInterface::writeAxisY(int y)    {
    y_axis=y;
}

void ControlInterface::writeRoverHeading(int heading)   {
    rover_heading=heading;
}

void ControlInterface::writeTotalDistance(unsigned long dist)    {
    total_distance=dist;
}

void ControlInterface::send_integer(int d)  {   // send integer MSB first
    serial->write(d>>8);
    serial->write(d&255);
}