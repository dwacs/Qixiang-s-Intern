#include"sml_meter.h"


void smlMeter::openPort()
{
    serial = new QSerialPort;
    serial->setPortName("ttymxc2");
    if ( serial->open(QIODevice::ReadWrite))   //打开串口读写
    {
         serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);       // 设置数据位(数据位为8位)
         serial->setParity(QSerialPort::NoParity);           //设置校验位(无校验位)
         serial->setStopBits(QSerialPort::OneStop);       // 设置停止位(停止位为1)
        serial->setFlowControl(QSerialPort::NoFlowControl); // 设置流控制(无数据流控制)
        //return re = true;

        QString textstr = "Open "+serial->portName()+" sucessed!!!!!!";
        qDebug() << "serialport:" << textstr;
    }
    else
    {
        QString textstr = "Open "+serial->portName()+" failed!!!!!!!!";
        qDebug() << "serialport:" << textstr;
       // return re = false;
    }
   // qDebug("DC meter %d initialization finished.", DCMeter_Comamand.meter_addres);
    //ReadSerialNum();


    QObject::connect(serial,&QSerialPort::readyRead, this, &smlMeter::readData);

}

void smlMeter::readData()
{
smlBuf.append(serial->readAll());         // after reading all the data from the serial port
dataParse();               //parse the data

   smlBuf.clear();
}


void smlMeter::dataParse()
{ QByteArray tempData=nullptr;
    tempData=smlBuf.toHex();            //convert all data to hex format

    if(tempData.left(8)=="1b1b1b1b")                                                 //find the "start sml"
       {
        smlData.append(tempData);

    if(tempData.contains("1b1b1b1b1a"))           //find the "end of the sml"
    { getData(smlData);
       // qDebug()<<"endofsmlbyoneline";
              smlData.clear();
    }
    }
    else{                                                                           // if "start sml" not found, the data is in transmission
    if(tempData.contains("1b1b1b1b"))            //if end of the file, start read
    {smlData.append(tempData);

        getData(smlData);                      //Extract the data from the sml data pack

        smlData.clear();
//smlData.append(tempData);
    }
    else{smlData.append(tempData);}             //if "end of the sml" not found, means the data still in transmission, continue append the data.
    }

}


void smlMeter::getData(QByteArray data)
{ int getList;
    int dataLength=0;
    QString objName;

   getList=data.indexOf("726500000701");   //find the getList response
   getList=data.indexOf("77",getList);    //find the client data head "77"
getClientdata( getList,data);                 // Parse the data, and record the client data

   getList=data.indexOf("7677",getList);   //find the obj name

   getList+=2;
if(data.mid(getList,2)=="77")          // find the meter data head "77"
{


     dataLength=setData(getList,data);           // read the manufacture ID
    dataLength=setData(dataLength,data);          // read the device number
      dataLength=setData(dataLength,data);        //read the energy
      dataLength=setData(dataLength,data);         // read the active power
        dataLength=setData(dataLength,data);     //read the property number
       setData(dataLength,data);                  //read the voltage



}
else{qDebug()<<"get data not found";}

}


int smlMeter::setData(int position,QByteArray data)
{ smlMeterData smlData;
    QByteArray objName;
QStringList objList;
bool ok;
objList<<"8181c78203ff"<<"0100000009ff"<<"0100010800ff"<<"0100100700ff"<<"0100000000ff"<<"01000c0700ff";

 uint length=0;

 int newPosition;
 newPosition=position+2;         //after 77
length=data.mid(newPosition,2).toUInt()*2-2;  //find next data length  || Length*2 because xFF was recorded in string format, so one Byte in string format should x2.
newPosition+=2;

objName=data.mid(newPosition,length);          //set data first is object name
newPosition+=length;
length=data.mid(newPosition,2).toUInt();
switch(objList.indexOf(objName))
{
case 0:   //manufacture ID
   {
   newPosition+=8;
length=data.mid(newPosition,2).toUInt()*2-2;

newPosition+=2;
manufacturerID.status="NULL";
manufacturerID.valTime="NULL";
manufacturerID.unit=0;
manufacturerID.scaler=0;
manufacturerID.value=data.mid(newPosition,length);
manufacturerID.valueSignature="NULL";
newPosition+=(length+2); //+2for value sig +2 for datalength of value

break;
}
case 1: { //device number
    newPosition+=8;
QByteArray test;

    length=data.mid(newPosition,2).toUInt(&ok,16)*2-2;

    newPosition+=2;
    deviceNumber.status="NULL";
   deviceNumber.valTime="NULL";
    deviceNumber.unit=0;
    deviceNumber.scaler=0;

    deviceNumber.value=data.mid(newPosition,length);
    deviceNumber.valueSignature="NULL";
    newPosition+=(length+2);

    break;}
case 2:{              //energy

  length=otectString(newPosition,data);//
 // qDebug()<<"lengthnew"<<length<<"lengthnew";
  if (length==0)
  {newPosition+=2;   //01 xx.
    energy.status="NULL";
  }
  else
  {newPosition+=2;
      energy.status=data.mid(newPosition,length);
      newPosition+=length;}


length=otectString(newPosition,data);
//qDebug()<<"lengthnew"<<length<<"lengthnew";
if (length==0)  {

    newPosition+=2;   //01 xx.
        energy.valTime="NULL";
        }
    else {
        newPosition+=2;
          energy.valTime=data.mid(newPosition,length);
         newPosition+=length;
    }


length=otectString(newPosition,data);
//qDebug()<<"lengthnew"<<length<<"lengthnew";
if (length==0)  {newPosition+=2;   //01 xx.
          energy.unit=0;
          }
      else {
          newPosition+=2;
            energy.unit=data.mid(newPosition,length).toUInt(&ok,16);

           newPosition+=length;
      }



length=otectString(newPosition,data);
//qDebug()<<"lengthnew"<<length<<"lengthnew";
if (length==0)  {newPosition+=2;   //01 xx.
          energy.scaler=0;
          }
      else {
          newPosition+=2;

            energy.scaler=data.mid(newPosition,length).toUInt(&ok,16)-256;

           newPosition+=length;
      }

length=otectString(newPosition,data);
if (length==0)  {newPosition+=2;   //01 xx.
          energy.value="NULL";
          }
      else {
          newPosition+=2;
            energy.value=data.mid(newPosition,length);
           newPosition+=length;
      }
length=otectString(newPosition,data);
if (length==0)  {newPosition+=2;   //01 xx.
          energy.valueSignature="NULL";
          }
      else {
          newPosition+=2;
            energy.valueSignature=data.mid(newPosition,length);
           newPosition+=length;
      }

break;
}
case 3:{ //active power
   // qDebug()<<"nowposddddd"<<data.mid(position,4)<<"dddddnow";
      length=otectString(newPosition,data);
     // qDebug()<<"lengthnew"<<length<<"lengthnew";
      if (length==0)
      {newPosition+=2;   //01 xx.
        power.status="NULL";
      }
      else
      {newPosition+=2;
          power.status=data.mid(newPosition,length);
          newPosition+=length;}


    length=otectString(newPosition,data);
   // qDebug()<<"lengthnew"<<length<<"lengthnew";
    if (length==0)  {

        newPosition+=2;   //01 xx.
            power.valTime="NULL";
            }
        else {
            newPosition+=2;
              power.valTime=data.mid(newPosition,length);
             newPosition+=length;
        }


    length=otectString(newPosition,data);
    //qDebug()<<"lengthnew"<<length<<"lengthnew";
    if (length==0)  {newPosition+=2;   //01 xx.
              power.unit=0;
              }
          else {
              newPosition+=2;
                power.unit=data.mid(newPosition,length).toUInt(&ok,16);

               newPosition+=length;
          }



    length=otectString(newPosition,data);
    //qDebug()<<"lengthnew"<<length<<"lengthnew";
    if (length==0)  {newPosition+=2;   //01 xx.
              power.scaler=0;
              }
          else {
              newPosition+=2;

                power.scaler=data.mid(newPosition,length).toUInt(&ok,16)-256;

               newPosition+=length;
          }

    length=otectString(newPosition,data);
    if (length==0)  {newPosition+=2;   //01 xx.
              power.value="NULL";
              }
          else {
              newPosition+=2;
                power.value=data.mid(newPosition,length);
               newPosition+=length;
          }
    length=otectString(newPosition,data);
    if (length==0)  {newPosition+=2;   //01 xx.
              power.valueSignature="NULL";
              }
          else {
              newPosition+=2;
                power.valueSignature=data.mid(newPosition,length);
               newPosition+=length;
          }



    break;
}
case 4:           // property number
{ newPosition+=8;
    length=data.mid(newPosition,2).toUInt(&ok,16)*2-2;
   // qDebug()<<"newpos"<<length<<data.mid(newPosition,length)<<"value";
    newPosition+=2;
    propertyNo.status="NULL";
    propertyNo.valTime="NULL";
    propertyNo.unit=0;
    propertyNo.scaler=0;
    propertyNo.value=data.mid(newPosition,length);
    propertyNo.valueSignature="NULL";
    newPosition+=(length+2); //+2for value sig +2 for datalength of value

    break;
}
case 5:          // voltage
{


      length=otectString(newPosition,data);

      if (length==0)
      {newPosition+=2;   //01 xx.
        voltage.status="NULL";
      }
      else
      {newPosition+=2;
          voltage.status=data.mid(newPosition,length);
          newPosition+=length;}


    length=otectString(newPosition,data);

    if (length==0)  {

        newPosition+=2;   //01 xx.
            voltage.valTime="NULL";
            }
        else {
            newPosition+=2;
              voltage.valTime=data.mid(newPosition,length);
             newPosition+=length;
        }


    length=otectString(newPosition,data);

    if (length==0)  {newPosition+=2;   //01 xx.
              voltage.unit=0;
              }
          else {
              newPosition+=2;
                voltage.unit=data.mid(newPosition,length).toUInt(&ok,16);

               newPosition+=length;
          }

    length=otectString(newPosition,data);

    if (length==0)  {newPosition+=2;   //01 xx.
              voltage.scaler=0;
              }
          else {
              newPosition+=2;

                voltage.scaler=data.mid(newPosition,length).toUInt(&ok,16)-256;

               newPosition+=length;
          }

    length=otectString(newPosition,data);

    if (length==0)  {newPosition+=2;   //01 xx.
              voltage.value="NULL";
              }
          else {
              newPosition+=2;
                voltage.value=data.mid(newPosition,length);
               newPosition+=length;
          }
    length=otectString(newPosition,data);
    if (length==0)  {newPosition+=2;   //01 xx.
              voltage.valueSignature="NULL";
              }
          else {
              newPosition+=2;
                voltage.valueSignature=data.mid(newPosition,length);
               newPosition+=length;
          }


    break;
}
default:
    break;
}
return newPosition;
}

uint smlMeter::otectString(int position,QByteArray data)
{bool ok;
    uint length;
    QStringList otectString;
    otectString<<"0"<<"5"<<"6";                   //Parse the otect string, eg 0b means the following data length is  0b-01=10 Bytes.
      length=data.mid(position,2).toUInt();     //  59 means following data is integer 32 bit and the length is 9-1=8 Bytes
    if(length-1!=0)                             // 63 means unsigned 16. length is 3-1=2Bytes
   {length=data.mid(position,2).toUInt();
    switch(otectString.indexOf(data.mid(position,1)))
   {
   case 0:
      length=data.mid(position,2).toUInt(&ok,16)*2-2;
break;
   case 1:
           length=(data.mid(position,2).toUInt()-50)*2-2;
       break;
   case 2:
        length=(data.mid(position,2).toUInt()-60)*2-2;
       break;
   default:
       break;
   }
return length;
}
else
{
    return 0;
}
}

void smlMeter::getClientdata(int dataPosition,QByteArray data)
{
    uint  dataLength;
bool ok;
    dataPosition+=2;
    dataLength=otectString(dataPosition,data);

    if (dataLength==0)
    {dataPosition+=2;   //01 xx.
      client1.clientID="Not Set";
    }
    else
    {dataPosition+=2;
        client1.clientID=data.mid(dataPosition,dataLength);
        dataPosition+=dataLength;}

    dataLength=otectString(dataPosition,data);

    if (dataLength==0)  {
        dataPosition+=2;   //01 xx.
              client1.serverID="NULL";
              }
          else {
               dataPosition+=2;
                client1.serverID=data.mid(dataPosition,dataLength);

               dataPosition+=dataLength;
          }

    dataLength=otectString(dataPosition,data);

    if (dataLength==0)  {
        dataPosition+=2;
              client1.listName="NULL";
              }
          else {
               dataPosition+=2;
                client1.listName=data.mid(dataPosition,dataLength);

               dataPosition+=dataLength;
          }

    dataPosition=data.indexOf("65",dataPosition);
    dataLength=otectString(dataPosition,data);
qDebug()<<"dataLength"<<dataLength<<"xxx";
    if (dataLength==0)  {
        dataPosition+=2;
              client1.duration=0;

              }
          else {
        QString timeT;
               dataPosition+=2;
               timeT=data.mid(dataPosition,dataLength);
               client1.duration=data.mid(dataPosition,dataLength).toUInt(&ok,16);

               dataPosition+=dataLength;
          }
}

smlMeter::~smlMeter()
{delete serial;}
