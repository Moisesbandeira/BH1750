// Projeto: BH1750 + Servo Contínuo + OLED 128x64
// Autor: Moisés Lourenço
// Data: 21/09/2025
// Descrição: Lê luminosidade com BH1750, move servo contínuo simulado os graus
//            para 0/90/180 graus conforme luminosidade e mostra dados no OLED.
//            Usando I2C para BH1750 e OLED, PWM para servo.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "servo.h"
#include "ssd1306.h"
#include "bh1750.h"

// ==== Pinos ====
#define SERVO_PIN   2       // GPIO do servo contínuo (360° )    

// I2C BH1750 (i2c0)
#define I2C_PORT_SENSOR i2c0        
#define SDA_SENSOR 0
#define SCL_SENSOR 1

// I2C SSD1306 (i2c1)   
#define I2C_PORT_OLED i2c1
#define SDA_OLED 14
#define SCL_OLED 15

int main() {
    stdio_init_all();
    // I2C BH1750
    i2c_init(I2C_PORT_SENSOR, 100 * 1000);
    gpio_set_function(SDA_SENSOR, GPIO_FUNC_I2C);
    gpio_set_function(SCL_SENSOR, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_SENSOR);
    gpio_pull_up(SCL_SENSOR);
    bh1750_init(I2C_PORT_SENSOR);
    sleep_ms(200);

    // I2C SSD1306
    i2c_init(I2C_PORT_OLED, 400000);
    gpio_set_function(SDA_OLED, GPIO_FUNC_I2C);
    gpio_set_function(SCL_OLED, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_OLED);
    gpio_pull_up(SCL_OLED);
    ssd1306_init(I2C_PORT_OLED);
    // Tela inicial
    ssd1306_clear();
    ssd1306_draw_string(18, 0, "Servo e BH1750");
    ssd1306_show();
    // Servo: usa 700ms padrão
    uint32_t rotation_time_ms = 700;// tempo para 90°
    servo_sim_t servo;
    servo_sim_init(&servo, SERVO_PIN, (float)rotation_time_ms);
      
    while (true) {
        // Lê luminosidade
        float lux = bh1750_read_lux(I2C_PORT_SENSOR);
        // Mapeia para 0/90/180 graus
        float angle = (lux < 100) ? 0.0f : (lux < 200) ? 90.0f : 180.0f;
        // Envia pela serial os valores
        printf("Lux: %.1f, Angulo: %.0f deg, tempo: %ums\n", lux, angle, (uint32_t)(180.0f / servo.deg_per_ms));
        // Simula o angulo movendo o servo 360°
        servo_sim_set_angle(&servo, angle);
        // Display
        ssd1306_clear();
        ssd1306_draw_string(18, 0, "Servo e BH1750");
        char line1[24], line2[24], line3[24];
        snprintf(line1, sizeof(line1), "Lux: %.1f", lux);
        snprintf(line2, sizeof(line2), "Angulo: %.0f deg", angle);
        snprintf(line3, sizeof(line3), "tempo: %ums", (uint32_t)(180.0f / servo.deg_per_ms));
        ssd1306_draw_string(6, 20, line1);
        ssd1306_draw_string(6, 36, line2);
        ssd1306_draw_string(6, 52, line3);
        ssd1306_show();
        sleep_ms(1200);
   }
}
