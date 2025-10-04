Nesse projeto foi usado a placa **BitDogLab** que possui a **Raspberry Pi Pico w** e o 
📟 **OLED SSD1306** integrados e a parti usamos os sensores de ☀️ **luminosidade BH1750** e ⚙️ **Micro servo Motor contínuo SG90**.

---

## 🧠 **Explicação do Código**

### 🧩 **Objetivo**

O programa lê a **intensidade luminosa (em lux)** do sensor BH1750 e, conforme o valor, simula o movimento de um **servo contínuo** representando ângulos de **0°, 90° e 180°**.
Essas informações são exibidas no **display OLED SSD1306** e enviadas também pela **porta serial (USB)**.

---

### ⚙️ **Principais Componentes**

| Componente                | Função                                | Comunicação                     |
| ------------------------- | ------------------------------------- | ------------------------------- |
| BH1750                    | Mede luminosidade (lux)               | I2C (porta i2c0, pinos 0 e 1)   |
| SSD1306                   | Mostra os dados (display OLED 128x64) | I2C (porta i2c1, pinos 14 e 15) |
| Servo contínuo (simulado) | Representa o movimento conforme a luz | PWM (pino GPIO 2)               |

---

### 🧾 **Etapas do Programa**

#### 1. **Inicialização**

```c
stdio_init_all();
```

Ativa o uso da **porta serial** (para exibir valores no terminal).

#### 2. **Configuração do BH1750**

Define o canal I²C (pinos 0 e 1), inicializa o sensor e aguarda estabilidade.

#### 3. **Configuração do OLED**

Cria uma segunda comunicação I²C (pinos 14 e 15), inicializa o display e exibe uma mensagem inicial:

```
Servo e BH1750
```

#### 4. **Configuração do Servo**

Inicializa a estrutura de simulação (`servo_sim_t`) com tempo base de **700 ms por 90°**.

#### 5. **Loop Principal**

A cada 1,2 segundos (`sleep_ms(1200)`):

1. Lê o valor de luminosidade (em lux).

   ```c
   float lux = bh1750_read_lux(I2C_PORT_SENSOR);
   ```

2. Define o ângulo do servo:

   ```c
   float angle = (lux < 100) ? 0.0f : (lux < 200) ? 90.0f : 180.0f;
   ```

   * Lux < 100 → servo em 0°
   * 100 ≤ Lux < 200 → servo em 90°
   * Lux ≥ 200 → servo em 180°

3. Move o servo:

   ```c
   servo_sim_set_angle(&servo, angle);
   ```

   A função simula o tempo necessário para atingir o novo ângulo.

4. Exibe as informações no display OLED:

   ```
   Lux: xxx.x
   Angulo: xxx deg
   tempo: xxx ms
   ```

5. Envia as mesmas informações para o **terminal serial** (útil para debug ou monitoramento).

---

## 💡 **Resultados Esperados**

| Nível de luz (lux) | Ângulo servo | Exibição OLED                 | Movimento simulado              |
| ------------------ | ------------ | ----------------------------- | ------------------------------- |
| < 100              | 0°           | “Lux: xx.x / Angulo: 0 deg”   | Servo parado na posição inicial |
| 100–199            | 90°          | “Lux: xx.x / Angulo: 90 deg”  | Servo gira até 90°              |
| ≥ 200              | 180°         | “Lux: xx.x / Angulo: 180 deg” | Servo gira até o fim (180°)     |

No **terminal serial**, você verá algo como:

```
Lux: 85.6, Angulo: 0 deg, tempo: 1800ms
Lux: 132.4, Angulo: 90 deg, tempo: 1800ms
Lux: 260.1, Angulo: 180 deg, tempo: 1800ms
```

E no **display OLED**, os dados serão atualizados em tempo real conforme a luminosidade variar.

---

## ✅ **Resumo Técnico**

| Item        | Valor                            |
| ----------- | -------------------------------- |
| Placa       | Raspberry Pi Pico                |
| Sensores    | BH1750 (lux), SSD1306 (OLED)     |
| Saída       | Servo contínuo (simulado em PWM) |
| Linguagem   | C (Pico SDK)                     |
| Comunicação | I²C duplo + PWM                  |
| Atualização | A cada 1,2 segundos              |

---

