## Descrição do Projeto

Este projeto consiste na implementação de uma interface gráfica simples em um display OLED controlado por uma **ESP32-CAM (AI Thinker)**, com interação baseada em movimento utilizando um sensor acelerômetro **ADXL345**.

O sistema exibe um rosto estilizado composto por olhos e um sorriso. A posição dos olhos é dinamicamente ajustada de acordo com os dados de aceleração detectados pelo sensor ADXL345, permitindo uma interação visual responsiva ao movimento físico do dispositivo.

Além disso, o sistema implementa um comportamento de piscada automática dos olhos em intervalos de aproximadamente 5 segundos.

O desenvolvimento foi realizado utilizando a plataforma Arduino IDE, com bibliotecas específicas para controle gráfico, comunicação I2C e leitura de sensores.

### Ambiente de Desenvolvimento

- **IDE:** Arduino IDE 2.3.8
- **Placa:** AI Thinker ESP32-CAM
- **Pacote de suporte ESP32:** Espressif Systems v3.3.7
- **URL de Preferências:** https://dl.espressif.com/dl/package_esp32_index.json

### Bibliotecas Utilizadas

- Adafruit GFX Library
- Adafruit SSD1306
- Adafruit ADXL345
- Adafruit Unified Sensor

Todas as bibliotecas devem ser instaladas juntamente com suas dependências através do gerenciador de bibliotecas da Arduino IDE.

---

## ⚠️ Gravação do Código (IMPORTANTE)

Siga exatamente este processo para gravar o código na ESP32-CAM:

1. Remova a ESP32-CAM da protoboard (se já estiver montada)
2. Conecte a ESP32-CAM ao **módulo programador (ESP32-CAM-MB ou similar)**
3. Conecte o módulo ao computador via USB
4. Na Arduino IDE:
   - Selecione a placa **AI Thinker ESP32-CAM**
   - Escolha a porta correta
5. Faça o upload do código
6. Após o upload:
   - Abra o **Monitor Serial**
   - Configure para **115200 baud**
   - Pressione o botão **RESET** na ESP32-CAM
7. Aguarde a inicialização completa
8. Depois:
   - Remova a ESP32-CAM do módulo
   - Coloque na protoboard
   - Realize as conexões elétricas
9. Conecte apenas os caminhos abaixo do módulo até a esp32-cam :
   - **5V**
   - **GND**

---

## Conexões de Hardware

As conexões elétricas devem ser realizadas conforme descrito abaixo:

| Componente    | Pino do Componente | Pino na ESP32-CAM   |
| ------------- | ------------------ | ------------------- |
| Display OLED  | VCC                | 3.3V                |
| Display OLED  | GND                | GND                 |
| Display OLED  | SDA                | GPIO 14             |
| Display OLED  | SCL                | GPIO 15             |
| ADXL345       | VCC                | 3.3V                |
| ADXL345       | CS                 | 3.3V                |
| ADXL345       | GND                | GND                 |
| ADXL345       | SDA                | GPIO 14             |
| ADXL345       | SCL                | GPIO 15             |
| ADXL345       | SDO                | GND (endereço 0x68) |
| MODULO DA CAM | 5V                 | 5V                  |
| MODULO DA CAM | GND                | GND                 |

**Observação:**  
Os dispositivos OLED e ADXL345 compartilham o mesmo barramento I2C (GPIO 14 e GPIO 15).

---

## Funcionamento

Após a gravação do firmware na ESP32-CAM:

1. O display OLED inicializa e exibe um rosto estilizado.
2. Os olhos se movem conforme a inclinação detectada pelo sensor MPU-6050.
3. Um evento de piscada é executado automaticamente em intervalos de aproximadamente 5 segundos.
4. O sistema opera continuamente enquanto estiver energizado.
