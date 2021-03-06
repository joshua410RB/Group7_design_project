#ifndef POWER_H
#define POWER_H

#include <Arduino.h>
#include <MovingAverage.h>
#include <Wire.h>
#include <INA219_WE.h>
#include <SPI.h>
#include <SD.h>

struct Stats {
    float q1_now;
    float q2_now;
    float q3_now;

    float SoC_1;
    float SoC_2;
    float SoC_3;
};

class SMPS {
    public:
        SMPS();
        void init(); // grab from SD card, sensors
        // Stats get_Stats(); // likely sent back to control
        // void write_stats(); // write stats to SD card

        // collected from SMPS
        // void trigger_Update(bool update); //when count = 1000
        // void sendVI(float v1, float v2, float v3, float curMeasure);
        // void triggerError();
        // void reset();

        // sends requested state back to the SMPS current controller
        // only get state when instruction is called,
        // otherwise, SMPS will decide its state (determined by default state transitions)
        // int get_state();
        
        // void decode_command(int cmd); //TODO:
        // float estimate_range(float disTravelled, float SoC_drop);
        // float estimate_chargeTime(); //TODO:

        //TODO: Need to consider balancing for charging as well. Handle in main Arduino file
        // void charge(); // 250mA
        // void rapid_charge(); // 500mA  
        // void discharge(); // 500mA
        // void rapid_discharge(); // 1A
        // void stop(); // stop charge or discharge 

        // Recalibrate SOH
        // void recalibrate_SOH(); //called by control     
        // bool get_recalibrate(); // instruct Arduino to recalibrate.
        // void send_current_cap(float q1, float q2, float q3); 
        // void record_curve(int state_num, float V_1, float V_2, float V_3);
        // void create_SoC_table();

        // Helper function for cv and dv
        // float determine_cv_threshold();
        // float determine_dv_threshold();

        //Compute SoC
        void compute_SOC(int state_num, float V_1, float V_2, float V_3, float charge_1, float charge_2, float charge_3);
        float get_SOC(int cell_num);
        
        // Helper functions called by compute_SOC()
        float lookup_c_table(int cell_num, float V_1, float V_2, float V_3);
        float lookup_d_table(int cell_num, float V_1, float V_2, float V_3);

        bool command_running = 0; 
        // NOTE: command_running even when there is an error
        // to reset, call reset()

        int SD_CS = 10;
    
    private:
        int state;
        int prev_state = -1;
        bool recalibrating;
        bool error;

        // float V_1, V_2, V_3;
        // float current_measure;

        // Need to install Moving Average Library for this
        // Initialise within init method
        MovingAverage<float> SoC_1_arr = MovingAverage<float>(5);
        // MovingAverage<float> SoC_2_arr = MovingAverage<float>(5);
        // MovingAverage<float> SoC_3_arr = MovingAverage<float>(5);
        int arr_size = 0; // compute manually when FIFO not full

        float current_ref;

        float q1_0 = 1793;
        //float q2_0 = 2000.5;
        //float q3_0 = 1921.75;

        float q1_now; //, q2_now, q3_now;
        // float SoH_1, SoH_2, SoH_3;
        float SoC_1; //, SoC_2, SoC_3;

        // TODO: load these values from initialisation files
        // These values are decided after reading the entire discharge
        // or charge cycle (post recalibration, deterministic)
        float d_ocv_l = 3050;
        float d_ocv_u = 3300;
        float c_ocv_u = 3450;
        float c_ocv_l = 3300;

        float SoC_LT = 20;
        float SoC_HT = 80;

        String discharge_SoC_filename = "dv_SoC.csv";
        String charge_SoC_filename = "cv_SoC.csv";
        String threshold_filename = "thresholds.csv";
        String dataString;
        File myFile;

        float d_v_1[100] = {};
        float c_v_1[100] = {};

        /*
        float d_v_2[100] = {};
        float c_v_2[100] = {};
        float d_v_3[100] = {};
        float c_v_3[100] = {};
        */   

        float d_SoC[100] = {};
        float c_SoC[100] = {};

        int c_iterator = 0;
        int d_iterator = 0;
};

#endif
