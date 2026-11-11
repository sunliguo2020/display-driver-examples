# 0.96 寸 8 针插接屏：Arduino + TFT_eSPI

此示例按本目录 STM32 SPI 例程中的屏幕参数编写：ST7735S、80 × 160，屏幕坐标有 24 像素列偏移。示例开发板为经典 ESP32 DevKit / ESP32-WROOM；其他开发板请按其 SPI 引脚调整配置。

## 接线

请按屏幕模组上的引脚丝印接线，模组的实际排列顺序可能不同。屏幕逻辑电平按 3.3 V 连接。

| 屏幕引脚 | 经典 ESP32 引脚 | 说明 |
| --- | --- | --- |
| GND | GND | 共地 |
| VCC | 3V3 | 3.3 V 供电 |
| SCL / SCK | GPIO 18 | SPI 时钟 |
| SDA / MOSI | GPIO 23 | SPI 主机输出；不是 I2C SDA |
| RES / RST | GPIO 26 | 屏幕复位 |
| DC | GPIO 27 | 数据/命令选择 |
| CS | GPIO 33 | SPI 片选 |
| BLK / BL | 3V3 | 背光常亮；手册例程为高电平点亮 |

该屏没有接出 MISO，示例不使用读回功能。若模组背光电路或供电要求与此不同，请以模组资料为准。

## 配置 TFT_eSPI

TFT_eSPI 的驱动和引脚在库的 setup 文件中配置，不能在 `.ino` 中通过创建 `TFT_eSPI` 对象来替代。

1. 在 Arduino IDE 安装 `TFT_eSPI`。
2. 将本目录的 `Setup_ESP32_ST7735S_80x160.h` 复制到 Arduino 库目录下的 `TFT_eSPI/User_Setups/`。
3. 打开库目录中的 `User_Setup_Select.h`，注释默认的 `#include <User_Setup.h>` 及其他已启用的 setup，只启用这一项：

   ```cpp
   #include <User_Setups/Setup_ESP32_ST7735S_80x160.h>
   ```

4. 用 Arduino IDE 打开 `ST7735S_80x160_ESP32/ST7735S_80x160_ESP32.ino`，选择对应的 ESP32 开发板和端口，编译并上传。

setup 使用 `ST7735_REDTAB160x80`，其 80 × 160 尺寸及列偏移与手册例程 `LCD_Address_Set()` 中的 `x + 24` 相对应。若画面错位或颜色不正确，先核对屏幕具体型号与库版本；不同批次可能需要调整 tab 配置或 RGB/BGR 顺序。
