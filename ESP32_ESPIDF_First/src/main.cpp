#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"

#define LED_OUT GPIO_NUM_16
#define BUTTON_IN GPIO_NUM_15

static bool IRAM_ATTR timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data) {
    bool led_state = 
};

extern "C" void app_main() {

    gpio_config_t gpio_led_conf = {};
    gpio_led_conf.pin_bit_mask = 1ULL << LED_OUT;
    gpio_led_conf.mode = GPIO_MODE_OUTPUT;
    gpio_led_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_led_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_led_conf.intr_type = GPIO_INTR_DISABLE;

    gpio_config_t gpio_button_conf = {};
    gpio_button_conf.pin_bit_mask = 1ULL << BUTTON_IN;
    gpio_button_conf.mode = GPIO_MODE_INPUT;
    gpio_button_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_button_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_button_conf.intr_type = GPIO_INTR_DISABLE;

    gptimer_handle_t timer = NULL;

    gptimer_config_t timer_conf = {};
    timer_conf.clk_src = GPTIMER_CLK_SRC_DEFAULT;
    timer_conf.direction = GPTIMER_COUNT_UP;
    timer_conf.resolution_hz = 1000;

    gptimer_new_timer(&timer_conf, &timer);

    gptimer_alarm_config_t alarm_conf = {};
    alarm_conf.alarm_count = 1000;
    alarm_conf.reload_count = 0;
    alarm_conf.flags.auto_reload_on_alarm = true;

    gptimer_set_alarm_action(timer, &alarm_conf);


    gpio_config(&gpio_led_conf);
    gpio_config(&gpio_button_conf);

    gpio_set_level(LED_OUT, 0);

    while (1) {
        int btn_state = gpio_get_level(BUTTON_IN);
        
        gpio_set_level(LED_OUT, btn_state);

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
