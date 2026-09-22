#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "printf/usb_printf.h"
#include "pwm/pwm.h"
#include "servo/servo.h"

// Access to the ADC variable
extern ADC_HandleTypeDef hadc1;

#define SERVO_GPIO_PORT PWM_PORT_B
#define SERVO_GPIO_PIN 4

extern "C" void main_cpp() {
    bool error = false;

    PwmDriver_t servo_pwm;
    Servo_t servo;

    if (!Pwm_InitByPin(&servo_pwm, SERVO_GPIO_PORT,
                SERVO_GPIO_PIN, SERVO_FREQUENCY_HZ, 0)) {
        error = true;
        printf("PWM init failed\n");
    }

    if (!error && !Servo_Init(&servo, &servo_pwm)) {
        error = true;
        printf("Servo init failed\n");
    }

    while (1) {
        if (error) {
            printf("Init error\n");
            HAL_Delay(1000);
            continue;
        }

        // Reading ADC
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
        uint32_t adc_raw = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);

        // Calculating the angle for potenciometer and servo
        uint32_t pot_angle = (adc_raw * 270) / 4095;
        uint16_t target_angle = (uint16_t)pot_angle;

        // Cutting off excess angle values
        if (target_angle > SERVO_MAX_ANGLE) {
            target_angle = SERVO_MAX_ANGLE;
        }

        // Aplying the result and printing to the terminal
        Servo_SetAngle(&servo, target_angle);
        printf("RAW data: %lu\nPotentiometer angle: %lu\nServo angle: %u\n", adc_raw, pot_angle, target_angle);

        HAL_Delay(50);
    }
}

