# 使用 ESP32 在 Arduino 中配置 TFT

本文以 Arduino IDE、ESP32 和 SPI 接口的 TFT 屏幕为例，介绍环境安装、TFT_eSPI 配置和基本显示测试。不同屏幕模块的控制器、分辨率、引脚和背光电路可能不同，配置前请先核对屏幕资料或模块背面的标注。

## 一、配置 Arduino 和 ESP32 开发环境

1. 安装 Arduino IDE。
2. 在 Arduino IDE 的开发板管理器中安装 Espressif Systems 提供的 `esp32` 开发板支持包。
3. 连接开发板，在“工具”菜单中选择对应的 ESP32 开发板型号和串口。若看不到串口，先检查 USB 数据线、USB 转串口驱动和设备管理器。
4. 先上传一个最小的 Blink 程序，确认开发板、串口和上传流程正常，再接入 TFT 排查显示问题。

开发板型号和 ESP32 Arduino Core 版本应与实际硬件匹配。按住开发板的 BOOT 键再开始上传，通常可以帮助部分板卡进入下载模式；这不是 TFT 配置步骤，也不应作为每次上传的固定操作。

## 二、安装并选择 TFT_eSPI

在 Arduino IDE 的库管理器中搜索并安装 `TFT_eSPI`。库配置通常位于 Arduino 库目录下的 `TFT_eSPI/User_Setup.h` 和 `TFT_eSPI/User_Setup_Select.h`，**不是写在 `.ino` 草图里的配置**。

`User_Setup_Select.h` 决定实际使用哪一份设置。可以直接编辑默认的 `User_Setup.h`，也可以建立自己的 setup 文件并在选择文件中启用它。确保最终只启用一份与当前屏幕相符的 setup；修改后重新编译，并从编译输出确认 Arduino 使用的是你修改的那份 TFT_eSPI 库。电脑上安装了多个同名库时，Arduino 可能选中另一份。

库升级可能覆盖库目录中的自定义设置。升级前请备份；更稳妥的做法是保留单独的自定义 setup 文件，并在 `User_Setup_Select.h` 中明确选择它。

### 2.1 先确认屏幕和接线

核对屏幕模块的控制器型号（例如 ILI9341、ST7789）、接口类型、分辨率以及模块引脚定义。SPI 屏常见信号如下，模块未必全部引出：

| 屏幕信号 | 用途 | 配置项 |
| --- | --- | --- |
| SCLK / SCK | SPI 时钟 | `TFT_SCLK` |
| MOSI / SDA / DIN | ESP32 向屏幕发送数据 | `TFT_MOSI` |
| MISO / SDO | 屏幕向 ESP32 返回数据，很多屏幕不需要 | `TFT_MISO` |
| CS | 片选 | `TFT_CS` |
| DC / A0 | 数据/命令选择 | `TFT_DC` |
| RST / RES | 屏幕复位 | `TFT_RST` |
| BL / LED | 背光控制或电源 | 依模块电路处理 |

`SDA` 在 SPI 模块上通常表示 SPI 的 MOSI，不代表 I2C；不要仅凭丝印就把它接到 ESP32 的 I2C SDA。所有电路必须共地。ESP32 GPIO 是 3.3 V 逻辑，不能默认认为屏幕模块的输入端可承受 5 V；电源电压也应按模块规格供给。

### 2.2 设置驱动、引脚和 SPI

在所选 setup 文件中，根据实际屏幕选择驱动宏，并配置接线使用的 GPIO。例如下面配置**仅供经典 ESP32-WROOM 开发板搭配 SPI ILI9341 时参考**，请根据自己的板卡引脚和屏幕模块修改：

```c
#define ILI9341_DRIVER

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   33
#define TFT_DC   27
#define TFT_RST  26

#define SPI_FREQUENCY  27000000
```

不要同时启用多个控制器驱动宏。上面的 GPIO 不是屏幕的固定标准接线；其他 ESP32 型号或开发板可能使用不同的 SPI 引脚。尤其要避开被板载 Flash/PSRAM 占用的引脚，并检查启动绑带脚（strapping pins）是否会因屏幕或外接电路的电平影响启动。GPIO 34–39 仅能作为输入，不能用作屏幕输出信号。背光有独立控制脚时，按模块电路及库的设置处理；若背光由模块直接接电源控制，则不需要把它当作 SPI 信号配置。

先用较低的 `SPI_FREQUENCY`（例如 `20000000`）和短接线验证，再逐步提高。线长、面包板接触、供电和屏幕模块都会影响稳定上限；频率越高不一定越好。

### 2.3 设置颜色、分辨率和偏移

- **分辨率由屏幕型号和驱动配置决定。** 对驱动要求手动指定宽高的屏幕，按模块规格设置库所需的宽高宏；不要因为旋转后的宽高变化而改错物理分辨率。
- **偏移不是通用常量。** 某些屏幕控制器或非标准尺寸模块需要设置行列偏移，具体宏及适用方向取决于 TFT_eSPI 版本和所选驱动。按该版本对应 setup 示例和模块资料设置，不要把其他屏幕的偏移值照搬过来。
- `tft.setRotation(0..3)` 用于选择方向；旋转后可读 `tft.width()` 和 `tft.height()` 获取当前逻辑尺寸。若画面裁切或边缘错位，再检查分辨率、驱动对应的偏移和方向，而不是只反复改坐标。
- 若红蓝颜色互换，检查驱动的 RGB/BGR 颜色顺序设置；若颜色整体反相或显示异常，再核对模块是否需要反相显示设置。相关宏因驱动和屏幕版本而异。

## 三、最小显示测试

完成库配置和接线后，上传以下草图。先观察能否稳定显示纯色、文字和边框，再运行后面的图形示例。

```cpp
#include <TFT_eSPI.h>

TFT_eSPI tft;

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);

  Serial.printf("TFT size: %d x %d\n", tft.width(), tft.height());
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("TFT OK");
  tft.drawRect(10, 50, tft.width() - 20, tft.height() - 60, TFT_GREEN);
}

void loop() {
}
```

打开串口监视器并设为 115200 baud，可查看实际逻辑宽高。确认测试正常后，再调整旋转、偏移、颜色和 SPI 频率。

## 四、常见踩坑点与排查顺序

| 现象 | 优先检查 |
| --- | --- |
| 编译失败，提示没有定义驱动或 setup 不匹配 | `User_Setup_Select.h` 是否启用了正确且唯一的 setup；驱动宏拼写是否正确；Arduino 是否正在编译另一份 TFT_eSPI。 |
| 程序上传成功，但屏幕全黑 | 背光是否亮（背光亮不代表 SPI 已正常）；屏幕供电和共地；CS、DC、RST、SCLK、MOSI 是否与 setup 一致；控制器驱动是否选对。 |
| 屏幕白屏、花屏或偶尔有画面 | 核对接线和驱动；先降低 SPI 时钟、缩短导线并检查供电；检查复位脚和 GPIO 是否被其他功能占用。 |
| 只有部分区域显示，或图像整体偏移 | 核对控制器、物理分辨率和该模块对应的行列偏移；逐个测试旋转方向。不要直接套用另一块屏幕的 offset。 |
| 红色和蓝色互换、颜色反相 | 检查 RGB/BGR 顺序和屏幕反相设置，确认设置与具体模块匹配。 |
| ESP32 无法启动或反复重启 | 检查是否使用了 Flash/PSRAM 引脚、输入专用 GPIO，或让屏幕电路改变启动绑带脚电平；先断开屏幕验证开发板能否独立启动。 |
| 屏幕背光不亮，但绘图配置看起来正确 | 背光可能需要单独供电或控制；检查模块的 BL/LED 电路和极性，不要把背光脚误当成 SPI 引脚。 |

建议按“先单独验证 ESP32 上传 → 核对供电和接线 → 确认实际加载的 setup 与驱动 → 降低 SPI 频率 → 最后调整偏移和颜色”的顺序排查。一次只改一类设置，便于判断问题来源。

## 五、图形绘制示例

下面的示例演示基本图形和文字绘制。它假设前面的 TFT_eSPI setup 已经正确配置。

```cpp
#include <TFT_eSPI.h>

TFT_eSPI tft;

void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
}

void loop() {
  tft.fillScreen(TFT_BLACK);

  tft.drawPixel(5, 5, TFT_RED);
  tft.drawLine(10, 20, 60, 20, TFT_GREEN);

  tft.drawRect(10, 40, 15, 10, TFT_BLUE);
  tft.fillRect(30, 40, 15, 10, TFT_YELLOW);

  tft.drawCircle(65, 35, 7, TFT_MAGENTA);
  tft.fillCircle(85, 35, 7, TFT_CYAN);

  tft.drawTriangle(75, 80, 85, 70, 85, 80, TFT_ORANGE);
  tft.fillTriangle(105, 80, 115, 70, 115, 80, TFT_PURPLE);

  tft.drawRoundRect(10, 75, 20, 20, 5, TFT_PINK);
  tft.fillRoundRect(35, 75, 20, 20, 5, TFT_BLUE);

  tft.setCursor(10, 10);
  tft.println("Hello, TFT!");

  delay(2000);
}
```

更多实际示例可参考仓库中的 [ESP32 TFT 示例目录](../ESP32TFT/esp32点亮tft/)。
