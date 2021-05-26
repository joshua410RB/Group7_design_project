#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "RoverFunctions.h"
#include "DriveInterface.h"
#include "FPGAInterface.h"
#include "config.h"

#define BASE_ADDRESS 0x40000
#define FPGA_I2C_ADDRESS 0x55

#define COLLISION_THRESHOLD 10  // currently in cm

#define RSSI_UPDATE_INTERVAL 100

// WiFi stuff
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// mqtt settings
const char* mqttServer = MQTT_SERVER; // MQTT server IP
const char* mqttUser = MQTT_USER;
const char* mqttPassword = MQTT_PASSWORD;
const int mqttPort = MQTT_PORT;

// some objects and data structures
WiFiClient espClient;
PubSubClient mqtt(espClient);
DriveInterface drive(&Serial);
FPGAInterface fpga(&Wire);
RoverDataStructure rover;

Obstacle obstacles[5];

// flags
int updateFlag=0;
int busyFlag=0;
int obstacleDetectFlag=0;
int collisionFlag=0;

unsigned long ptime=0;

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if(strcmp(topic,"drive/discrete")==0)  {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload, length);
    rover.direction=doc["direction"];
    rover.distance=doc["distance"];
    rover.speed=doc["speed"];
    rover.drive_mode=1;
    if(busyFlag==0)  {
      updateFlag=1;
    }
  }
  else if(strcmp(topic,"drive/t2c")==0) {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload, length);
    rover.target_x=doc["x"];
    rover.target_y=doc["y"];
    rover.speed=doc["speed"];
    rover.drive_mode=2;
    if(busyFlag==0)  {
      updateFlag=1;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  initWiFi(ssid,password);

  mqtt.setServer(mqttServer, mqttPort);
  mqtt.setCallback(mqtt_callback);

  fpga.setBusFrequency(400000);
  fpga.setSlaveAddress(FPGA_I2C_ADDRESS);
  fpga.setBaseAddress(BASE_ADDRESS);
  fpga.begin(GPIO_NUM_13, GPIO_NUM_12);

  drive.setBaudrate(115200);
  drive.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()==WL_CONNECTED) {
    if(mqtt.connected()) { // MQTT broker connected
      if(drive.fetchData()) { // check if data available from arduino
        rover.battery_level=drive.getBatteryLevel();
        rover.rover_range=drive.getRange();
        rover.obstacle_detected=drive.getObstacle();
        rover.alert=drive.getAlert();
        rover.x_axis=drive.getAxisX();
        rover.y_axis=drive.getAxisY();
        rover.rover_heading=drive.getRoverHeading();
        rover.battery_SOH=drive.getBatterySOH();
        rover.battery_state=drive.getBatteryState();
        
        // send rover data to MQTT broker
        publishPosition(&mqtt, &rover);
        publishBatteryStatus(&mqtt, &rover);
        publishRoverStatus(&mqtt, &rover);

        busyFlag=rover.alert;
      }
      
      ColourObject obj; // fetch data from FPGA
      for(int i=0;i<5;i++)  {
        obj=fpga.readByIndex(i);
        if(obj.detected>0)  {
          if(obj.distance<COLLISION_THRESHOLD)  {
            collisionFlag=1;
          }
          Obstacle obs = convertObjectToObstacle(&rover, obj, i);
          publishObstacle(&mqtt, obs);
        }
      }

      if(millis()-ptime>=RSSI_UPDATE_INTERVAL)  { // publish RSSI
        publishRSSI(&mqtt, WiFi.RSSI());
        publishPosition(&mqtt, &rover);
        ptime=millis();
      }

      mqtt.loop();
      if(updateFlag==1) {
        if(rover.speed==0)  {
          rover.drive_mode=0;
        }
        drive.writeSystemTime(millis());
        drive.sendUpdates();
        updateFlag=0;
      }
    }
    else  { // MQTT broker not connected
      connectMQTT(&mqtt, mqttUser, mqttPassword);
    }
  }
  else  {
    rover.drive_mode=0;
    drive.sendUpdates();
    initWiFi(ssid,password);
    connectMQTT(&mqtt, mqttUser, mqttPassword);
  }
}