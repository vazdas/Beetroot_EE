#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"

#define ADC_CHANNEL	ADC_CHANNEL_3 // ADC1_CH3: GPIO4


extern "C" void app_main() {

    esp_err_t err = ESP_OK;

    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t adc_unit_config = {};
    adc_unit_config.unit_id = ADC_UNIT_1;
    err = adc_oneshot_new_unit(&adc_unit_config, &adc_handle);
    if (err != ESP_OK) {
        printf("Failed to initialize ADC, err = %d\n", err);
        return;
    }

    adc_oneshot_chan_cfg_t adc_channel_config = {};
    adc_channel_config.bitwidth = ADC_BITWIDTH_DEFAULT;
    adc_channel_config.atten = ADC_ATTEN_DB_12;
    err = adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &adc_channel_config);
    if (err != ESP_OK) {
        printf("Failed to configure ADC channel, err = %d\n", err);
        return;
    }

    adc_cali_handle_t calibration_handle = nullptr;
    adc_cali_curve_fitting_config_t calibration_config = {};
    calibration_config.unit_id = ADC_UNIT_1;
    calibration_config.chan = ADC_CHANNEL;
    calibration_config.atten = ADC_ATTEN_DB_12;
    calibration_config.bitwidth = ADC_BITWIDTH_DEFAULT;
    bool calibration_enabled =
        adc_cali_create_scheme_curve_fitting(&calibration_config, &calibration_handle) == ESP_OK;


    printf("RAW    U_manual(mV)   U_cali(mV)   Error(%%)\n");
    printf("---------------------------------------------\n");

    while (1) {
        int adc_raw = 0;
        err = adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_raw);

        if (err == ESP_OK && calibration_enabled) {
            // Getting the calibrated voltage
            int voltage_mv = 0;
            adc_cali_raw_to_voltage(calibration_handle, adc_raw, &voltage_mv);

            // Calculating manual voltage
            float u_manual = (adc_raw / 4095.0) * 3300.0;

            // Calculating the error
            float error = 0.0;
            if (voltage_mv > 0) {
                float diff = u_manual - voltage_mv;
                if (diff < 0) diff = -diff;
                error = (diff / voltage_mv) * 100.0;
            }

            // Output the results
            printf("%-6d %-14.1f %-12d %-8.2f\n", adc_raw, u_manual, voltage_mv, error);
        }

        // Reading the values each 100ms
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

