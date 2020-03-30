[中文readme](README_CN.md)

### hardware need

nodemcu，ST7789 TFT display 135*240, DS3231

can also work without display, read from serial com port, may need some change to remove rely on DS3231.

### pin connect

| TFT display | nodemcu |
| ----------- | ------- |
| GND         | GND     |
| VCC         | 3V3     |
| SCL/SCLK    | D5      |
| SDA/MOSI    | D7      |
| RES/RST     | D0      |
| DC          | D3      |
| CS          | D4      |
| BLK         | NC      |

| DS3231 | nodemcu |
| ------ | ------- |
| SCL    | D1      |
| SDA    | D2      |
| VCC    | 3V3     |
| GND    | GND     |

before use, add your wifi ssid, password and bilibili uid. 