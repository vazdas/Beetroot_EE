#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

constexpr adc_channel_t ADC_CHAN_LED = ADC_CHANNEL_3;   // Analog input for LED (GPIO4)
constexpr adc_channel_t ADC_CHAN_MOTOR = ADC_CHANNEL_4; // Analog input for DC Motor (GPIO5)

constexpr uint16_t FREQUENCY_HZ = 5000;

constexpr gpio_num_t PWM_PIN_LED = GPIO_NUM_15;         // Led PWM output
constexpr gpio_num_t PWM_PIN_MOTOR = GPIO_NUM_16;       // DC Motor PWM output

extern "C" void app_main() {
    esp_err_t err;

    // ADC Configuration
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t adc_cfg = {};
    adc_cfg.unit_id = ADC_UNIT_1;
    adc_oneshot_new_unit(&adc_cfg, &adc_handle);

    adc_oneshot_chan_cfg_t chan_cfg = {};
    chan_cfg.atten = ADC_ATTEN_DB_12;
    chan_cfg.bitwidth = ADC_BITWIDTH_DEFAULT;

    // Configuring separate channels for the LED and the motor.
    adc_oneshot_config_channel(adc_handle, ADC_CHAN_LED, &chan_cfg);
    adc_oneshot_config_channel(adc_handle, ADC_CHAN_MOTOR, &chan_cfg);

    // PWM Configuration (LEDC)
    // Setting up a timer
    ledc_timer_config_t timer_cfg = {};
    timer_cfg.speed_mode = LEDC_LOW_SPEED_MODE;
    timer_cfg.duty_resolution = LEDC_TIMER_12_BIT; // 0-4095
    timer_cfg.timer_num = LEDC_TIMER_0;
    timer_cfg.freq_hz = FREQUENCY_HZ;
    timer_cfg.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timer_cfg);

    // Setting up a LED channel
    ledc_channel_config_t led_chan = {};
    led_chan.gpio_num = PWM_PIN_LED;
    led_chan.speed_mode = LEDC_LOW_SPEED_MODE;
    led_chan.channel = LEDC_CHANNEL_0;
    led_chan.timer_sel = LEDC_TIMER_0;
    led_chan.duty = 0;
    led_chan.hpoint = 0;
    ledc_channel_config(&led_chan);

    // Setting up a DC Motor channel
    ledc_channel_config_t motor_chan = {};
    motor_chan.gpio_num = PWM_PIN_MOTOR;
    motor_chan.speed_mode = LEDC_LOW_SPEED_MODE;
    motor_chan.channel = LEDC_CHANNEL_1;
    motor_chan.timer_sel = LEDC_TIMER_0;
    motor_chan.duty = 0;
    motor_chan.hpoint = 0;
    ledc_channel_config(&motor_chan);

    while (1) {
        int raw_led = 0;
        int raw_motor = 0;

        // Reading potentiometers (0 - 4095)
        adc_oneshot_read(adc_handle, ADC_CHAN_LED, &raw_led);
        adc_oneshot_read(adc_handle, ADC_CHAN_MOTOR, &raw_motor);

        // Pass the values ​​to the PWM channels
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, raw_led);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, raw_motor);

        // Applying the changes
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

        // Very short delay for smooth reaction
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

