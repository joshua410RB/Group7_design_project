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
        
        // Recalibrate SOH
        // void recalibrate_SOH(); //called by control     
        // bool get_recalibrate(); // instruct Arduino to recalibrate.
        void send_current_cap(float q1, float q2, float q3); 
        void record_curve(int state_num, float V_1, float V_2, float V_3);
        void clear_lookup();
        void create_SoC_table();

        // Helper function for cv and dv
        // float determine_cv_threshold();
        // float determine_dv_threshold();

        //Compute SoC
        // void compute_SOC(int state_num, float V_1, float V_2, float V_3, float charge_1, float charge_2, float charge_3);
        
        // Helper functions called by compute_SOC()
        // float lookup_c_table(int cell_num, float V_1, float V_2, float V_3);
        // float lookup_d_table(int cell_num, float V_1, float V_2, float V_3);

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
        MovingAverage<float> SoC_1_arr = MovingAverage<float>(120);
        // MovingAverage<float> SoC_2_arr = MovingAverage<float>(5);
        // MovingAverage<float> SoC_3_arr = MovingAverage<float>(5);
        int arr_size = 0; // compute manually when FIFO not full

        float current_ref;

        float q1_0 = 1793;
        float q2_0 = 2000.5;
        float q3_0 = 1921.75;

        float q1_now, q2_now, q3_now;
        float SoH_1, SoH_2, SoH_3;
        float SoC_1, SoC_2, SoC_3;

        // TODO: load these values from initialisation files
        // These values are decided after reading the entire discharge
        // or charge cycle (post recalibration, deterministic)
        float d_ocv_l_1 = 3150;
        float d_ocv_u_1 = 3250;
        float c_ocv_u_1 = 3450;
        float c_ocv_l_1 = 3300;

        /*
        float d_ocv_l_2 = 3100;
        float d_ocv_u_2 = 3200;
        float c_ocv_u_2 = 3400;
        float c_ocv_l_2 = 3300;

        float d_ocv_l_3 = 3100;
        float d_ocv_u_3 = 3200;
        float c_ocv_u_3 = 3400;
        float c_ocv_l_3 = 3300;
        */

        String discharge_SoC_filename = "dv_SoC.csv";
        String charge_SoC_filename = "cv_SoC.csv";
        String threshold_filename = "thresholds.csv";
        String dataString;
        File myFile;

        float d_v_1[100] = {};
        float c_v_1[100] = {};

        float d_v_2[100] = {};
        float c_v_2[100] = {};
        float d_v_3[100] = {};
        float c_v_3[100] = {};

        float d_SoC[100] = {};
        float c_SoC[100] = {};

        int c_iterator = 0;
        int d_iterator = 0;
};

#endif
