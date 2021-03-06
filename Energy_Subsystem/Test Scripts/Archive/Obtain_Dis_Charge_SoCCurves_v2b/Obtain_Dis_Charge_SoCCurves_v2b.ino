//libs
#include <SPI.h>
#include <SD.h>
#include <String.h>
#include <LinkedList.h>

/*
Aim:
    From the discharge and charge curves, obtain the SoC lookup table
    And save it in the files "dvSoc1.csv" "cvSoc1.csv"

Intended flow chart:
    1. Access discharge voltage table and save in an array
        1a. It only generates one row per every 10 row, using a moving average mindset
    2. Using the discharge voltage table and the array size, save it in a new file alongside the SoC
    3. Repeat for charge voltage table

This version uses LINKEDLIST, and is compiled with "Linkedlist" by Ivan Seidel, version 1.2.2. It should probably work with
newer versions, but unfortunately the newer versions of the library does not compile on my computer.

ABANDONED: LINKEDLISTS DO NOT ALLOW EFFICIENT ACCESS OF DATA
*/

// Which cell are you characterising?
int CELL = 1;

// SD
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 10;

//SD vars
File myFile;

//array of parsed data
LinkedList<float> d_v;
LinkedList<float> c_v;
float d_SoC = 1; 
float c_SoC = 0; 

//to generate an average of past 10 values (not moving average)
float v0 = 0;
float v1 = 0;
float v3 = 0;
float v4 = 0;
float v5 = 0;
float v6 = 0;
float v7 = 0;
float v8 = 0;
float v9 = 0;

int arrpointer = 0;       //array pointer
int column = 1;           //column to parse
int columncount = 1;      //start at column 1 not 0
int rowincrement = 10;     //increment the number of parse rows - set to 1 for all rows -> no more than 10,000/20 = 500 rows
int rowcount = 1;         //start at row 1 not 0
String datatemp;          //temp to store datapoint

// File names (Placeholder)
String discharge_filename = "dv1.csv";
String charge_filename = "cv1.csv";
String discharge_SoC_filename = "dvSoc1.csv";
String charge_SoC_filename = "cvSoc1.csv";

String dataString;

void setup(){

    Serial.begin(9600);

    //Check for the SD Card
    Serial.println("\nInitializing SD card...");
    if (!SD.begin(chipSelect)) {
    Serial.println("* is a card inserted?");
    while (true) {} //It will stick here FOREVER if no SD is in on boot
    } else {
    Serial.println("Wiring is correct and a card is present.");
    }

    // Batcurves stores the discharge and charge curves of the battery and compares it to the SoC
    if (CELL == 1) {
        discharge_filename = "dv1.csv";
        charge_filename = "cv1.csv";
        discharge_SoC_filename = "dvSoc1.csv";
        charge_SoC_filename = "cvSoc1.csv";
    } else if (CELL == 2) {
        discharge_filename = "dv2.csv";
        charge_filename = "cv2.csv";
        discharge_SoC_filename = "dvSoC2.csv";
        charge_SoC_filename = "cvSoC2.csv";
    }  else if (CELL == 3) {
        discharge_filename = "dv3.csv";
        charge_filename = "cv3.csv";
        discharge_SoC_filename = "dvSoC3.csv";
        charge_SoC_filename = "cvSoC3.csv";
    } else {
        Serial.println("\nWe only have 3 cells...");
    }

    /////////////////////
    // DISCHARGE FILES //
    /////////////////////
    //Read Discharge File
    myFile = SD.open(discharge_filename, FILE_WRITE);  
    if(myFile) {   
        Serial.println("File open");   
        while (myFile.available()) {     
            float filedata = myFile.read();      
            ////////// START PARSE //////////     
            if((filedata != ',')&&(filedata != ' ')){ //read data vals into string       
                if(rowcount % rowincrement == 0){ // to be fair we don't need all values to form a voltage lookup table
                    // Column 1 is Voltage, Column 2 is current measured (not neccessary)             
                    if(columncount == column){ //push datapoint to array  
                        datatemp = datatemp += (filedata -'0');
                        d_v[arrpointer] = datatemp.toFloat();
                    }
                }        
            }          
            if (filedata == ','){ //end of datapoint
                columncount++;  
            }      
            if (filedata == '\n'){ //end of row         
            //print data temp at end of line
            Serial.print(datatemp.toFloat());
            Serial.println();      
            //reset vars to read next row
            rowcount++; 
            columncount = 1; 
            datatemp = "";
            arrpointer++;  
            }
            ////////// END PARSE //////////    
        }    
    } else {
        Serial.println("File not open");
    }
    myFile.close();

    // Now determine discharge SoC as well and concatenate to new SD
    myFile = SD.open(discharge_SoC_filename, FILE_WRITE);
    if(myFile) {        
        for(int i = 0; i < arrpointer; i++){ //All Subsequent Values
            dataString = String(d_v[i]) + "," + String(d_SoC);
            Serial.println(dataString);
            myFile.println(dataString);
            d_SoC = d_SoC - 1/arrpointer;
        }
    } else {
        Serial.println("File not open");
    }

    /////////////////////
    //  CHARGE FILES   //
    /////////////////////
    //Read charge File
    myFile = SD.open(charge_filename, FILE_WRITE);  
    if(myFile) {   
        Serial.println("File open");   
        while (myFile.available()) {     
            float filedata = myFile.read();      
            ////////// START PARSE //////////     
            if((filedata != ',')&&(filedata != ' ')){ //read data vals into string       
                if(rowcount % rowincrement == 0){ // to be fair we don't need all values to form a voltage lookup table
                    // Column 1 is Voltage, Column 2 is current measured (not neccessary)             
                    if(columncount == column){ //push datapoint to array  
                        datatemp = datatemp += (filedata -'0');
                        c_v[arrpointer] = datatemp.toFloat();
                    }
                }        
            }             
            if (filedata == ','){ //end of datapoint
                columncount++;  
            }      
            if (filedata == '\n'){ //end of row         
            //print data temp at end of line
            Serial.print(datatemp.toFloat());
            Serial.println();      
            //reset vars to read next row
            rowcount++; 
            columncount = 1; 
            datatemp = "";
            arrpointer++;  
            }
            ////////// END PARSE //////////    
        }    
    } else {
        Serial.println("File not open");
    }
    myFile.close();

    // Now determine charge SoC as well and concatenate to new SD
    myFile = SD.open(charge_SoC_filename, FILE_WRITE);
    if(myFile) {        
        for(int i = 0; i < arrpointer; i++){ //All Subsequent Values
            dataString = String(c_v[i]) + "," + String(c_SoC);
            Serial.println(dataString);
            myFile.println(dataString);
            c_SoC = c_SoC + 1/arrpointer;
        }
    } else {
        Serial.println("File not open");
    }   
}


void loop(void) {
  // nothing repeats
}
