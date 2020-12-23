// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// This application uses the Azure IoT Hub device SDK for Java
// For samples see: https://github.com/Azure/azure-iot-sdk-java/tree/master/device/iot-device-samples

package com.microsoft.docs.iothub.samples;

import com.microsoft.azure.sdk.iot.device.*;
import com.microsoft.azure.sdk.iot.device.DeviceTwin.*;
import com.google.gson.Gson;

import java.io.*;
import java.net.URISyntaxException;
import java.util.Random;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;

//import java.io.File;
//import java.io.PipedInputStream;  //Pipe IO
//import java.io.PipedOutputSteam;


import java.io.BufferedReader;
import java.io.FileReader;
import com.google.gson.Gson;
//package testMaven.json;

//import org.apache.commons.io.FileUtils;
//import net.sf.json.JSONArray;
//import net.sf.json.JSONObject;

public class SimulatedDevice {
//private BufferedReader pipeReader; 




// The device connection string to authenticate the device with your IoT hub.
  // Using the Azure CLI:
  // az iot hub device-identity show-connection-string --hub-name {YourIoTHubName} --device-id MyJavaDevice --output table
  private static String connString = "HostName=testmeterevent.azure-devices.net;DeviceId=testmeter1;SharedAccessKey=tlu5i0xwbHPDRFV1VyQTRwVZ4sMXaQbbdZJllYBwS8c=";

  // Using the MQTT protocol to connect to IoT Hub
  private static IotHubClientProtocol protocol = IotHubClientProtocol.MQTT;
  private static DeviceClient testMeter1;

  // Define method response codes
  private static final int METHOD_SUCCESS = 200;
  private static final int METHOD_NOT_DEFINED = 404;
  private static final int INVALID_PARAMETER = 400;

  private static int telemetryInterval = 500;

  // Specify the telemetry to send to your IoT hub.
 
//public class PipeProducer{
//private BufferedReader pipeReader;

//public PipeProducer(String namedPipe) throws IOException{
//this.pipeReader = new BufferedReader(new FileReader(new File(namedPipe)));
//}
//}

 private static class meterData
{   
	
	public String ChargerId;
	public String Energy;
    public String MeterId;
    public String Power;
   public String TimeStamp;
	public String Voltage;
    
    
    public String getPipeData(){
String line = null;
System.out.println("dwacs");
StringBuffer content = new StringBuffer();
  String namedPipe1="/home/root/build-smlC141Test-imx_c141-Debug/ptin";
 String namedPipe2="/home/root/build-smlC141Test-imx_c141-Debug/ptout";
File pipe1 =new File(namedPipe1);

File pipe2 =new File(namedPipe2);
try{
BufferedReader reader = new BufferedReader(new FileReader(pipe2));
//char [] buf = new char[512];

while ((line=reader.readLine())!=null)
{content.append(line);
	//System.out.println("readingline"+line);
}
//reader.read(buf);

//System.out.println("reading"+content);
}
catch (Exception e){
System.out.println("wrong pipe");
} 

  return content.toString();
}
    

    public String toString(){
	return "Data[energy="+ Energy +", ChargerId="+ ChargerId +", MeterId="+ MeterId +", POWER="+ Power +", TimeStamp="+ TimeStamp +"]";
}

public String setTimeStamp()
{
	return ""+ TimeStamp +"";
}
public String setEnergy()
{
	return ""+ Energy +"";
}
public String setChargerId()
{
	return ""+ ChargerId +"";
}
public String setMeterId()
{
	return ""+ MeterId +"";
}
public String setPower()
{
	return ""+ Power +"";
}
public String setVoltage()
{
	return ""+ Voltage +"";
}
}


private static class AppMessageCallback implements MessageCallback {
  public IotHubMessageResult execute(Message msg, Object context) {
    System.out.println("Received message from hub: "
      + new String(msg.getBytes(), Message.DEFAULT_IOTHUB_MESSAGE_CHARSET));

    return IotHubMessageResult.COMPLETE;
  }
}



 private static class TelemetryDataPoint {
   // public double temperature;
   // public double humidity;
   // public int Voltage;
   // public int Power;
    //public int AccumulatedPower;
    public String Voltage;
    public String Power;
    public String Energy;
    public String TimeStamp; 
    public String MeterId;
    public String ChargerId;

    // Serialize object to JSON format.
    public String serialize() {
      Gson gson = new Gson();
      return gson.toJson(this);
    }





}


  // Print the acknowledgement received from IoT Hub for the method acknowledgement sent.
  protected static class DirectMethodStatusCallback implements IotHubEventCallback
  {
    public void execute(IotHubStatusCode status, Object context)
    {
      System.out.println("Direct method # IoT Hub responded to device method acknowledgement with status: " + status.name());
    }
  }

  // Print the acknowledgement received from IoT Hub for the telemetry message sent.
  private static class EventCallback implements IotHubEventCallback {
    public void execute(IotHubStatusCode status, Object context) {
      System.out.println("IoT Hub responded to message with status: " + status.name());

      if (context != null) {
        synchronized (context) {
          context.notify();
        }
      }
    }
  }

  protected static class DirectMethodCallback implements DeviceMethodCallback
  {
    private void setTelemetryInterval(int interval)
    {
      System.out.println("Direct method # Setting telemetry interval (seconds): " + interval);
      telemetryInterval = interval * 1000;
    }

    @Override
    public DeviceMethodData call(String methodName, Object methodData, Object context)
    {
      DeviceMethodData deviceMethodData;
      String payload = new String((byte[])methodData);
      switch (methodName)
      {
        case "SetTelemetryInterval" :
        {
          int interval;
          try {
            int status = METHOD_SUCCESS;
            interval = Integer.parseInt(payload);
            System.out.println(payload);
            setTelemetryInterval(interval);
            deviceMethodData = new DeviceMethodData(status, "Executed direct method " + methodName);
          } catch (NumberFormatException e) {
            int status = INVALID_PARAMETER;
            deviceMethodData = new DeviceMethodData(status, "Invalid parameter " + payload);
          }
          break;
        }
        default:
        {
          int status = METHOD_NOT_DEFINED;
          deviceMethodData = new DeviceMethodData(status, "Not defined direct method " + methodName);
        }
      }
      return deviceMethodData;
    }
  }

  private static class MessageSender implements Runnable {
    public void run() {
      try {
        // Initialize the simulated telemetry.
       // double minTemperature = 20;
      //  double minHumidity = 60;
        Random rand = new Random();

        while (true) {


//BufferedReader reader =new BufferedReader(new FileReader(pipe2));

//char [] buf = new char [1024];
//reader.read(buf);
//System.out.println("piplemessage:"+buf);



         // int currentTemperature = 110 ;
         // int currentHumidity = 33 ;
int energy = 12 ;

          TelemetryDataPoint telemetryDataPoint = new TelemetryDataPoint();

   



 
        
          
Gson gson = new Gson();
meterData smlData =new meterData();
String content=smlData.getPipeData();
  System.out.println("size of content="+ content.length());
  //System.out.println("rawdata="+ content+"===rawdata");

if(content.length()<185){         
meterData datafromJson=gson.fromJson(content,meterData.class);  //the first fromjson is an object of meterData

telemetryDataPoint.Voltage = datafromJson.setVoltage();
telemetryDataPoint.Power = datafromJson.setPower();
telemetryDataPoint.Energy=datafromJson.setEnergy();
telemetryDataPoint.TimeStamp=datafromJson.setTimeStamp();
telemetryDataPoint.MeterId=datafromJson.setMeterId();
telemetryDataPoint.ChargerId=datafromJson.setChargerId();



System.out.println("readingmessage: "+datafromJson);}

else
{
	System.out.println("Message dropped: ");
	telemetryDataPoint.Voltage = "NULL";
telemetryDataPoint.Power = "NULL";
telemetryDataPoint.Energy="NULL";
telemetryDataPoint.TimeStamp="NULL";
telemetryDataPoint.MeterId="testMeter1";
telemetryDataPoint.ChargerId="NULL";
}



          // Add the telemetry to the message body as JSON.
          String msgStr = telemetryDataPoint.serialize();
          Message msg = new Message(msgStr);

          // Add a custom application property to the message.
          // An IoT hub can filter on these properties without access to the message body.
         //msg.setProperty("temperatureAlert", (ChargerID=="testCharger") ? "true" : "false");
          
          System.out.println("Sending message: " + msgStr);

          Object lockobj = new Object();

          // Send the message.
          EventCallback callback = new EventCallback();
         testMeter1.sendEventAsync(msg, callback, lockobj);

          synchronized (lockobj) {
            lockobj.wait();
          }
          Thread.sleep(telemetryInterval);
        }
      } catch (InterruptedException e) {
        System.out.println("Finished.");
      }
    }
  }

  public static void main(String[] args) throws IOException, URISyntaxException {

    // Connect to the IoT hub.
    testMeter1 = new DeviceClient(connString, protocol);
    testMeter1.open();

    // Register to receive direct method calls.
    testMeter1.subscribeToDeviceMethod(new DirectMethodCallback(), null, new DirectMethodStatusCallback(), null);
    
    // Create new thread and start sending messages 
    MessageSender sender = new MessageSender();
    ExecutorService executor = Executors.newFixedThreadPool(1);
    executor.execute(sender);

    // Stop the application.
    System.out.println("Press ENTER to exit.");
    System.in.read();
    executor.shutdownNow();
    testMeter1.closeNow();
  }
}
