### 需要的硬件

nodemcu，ST7789 TFT 显示屏 135*240, DS3231

也可以脱离tft显示屏工作，通过串口读出，可能需要改动程序以移除对ds3231的依赖

### 引脚连接

| TFT 显示屏 | nodemcu |
| ---------- | ------- |
| GND        | GND     |
| VCC        | 3V3     |
| SCL/SCLK   | D5      |
| SDA/MOSI   | D7      |
| RES/RST    | D0      |
| DC         | D3      |
| CS         | D4      |
| BLK        | NC      |

| DS3231 | nodemcu |
| ------ | ------- |
| SCL    | D1      |
| SDA    | D2      |
| VCC    | 3V3     |
| GND    | GND     |

在编译下载前，添加你的wifi ssid，密码和b站uid。