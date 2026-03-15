# MonoGUI with SH1106 Display Guide

## Overview

✅ **MonoGUI 现在完全兼容 SSD1306 和 SH1106 显示屏！**

所有 widget 函数现在使用 `Adafruit_GFX&` 基类，这意味着：
- 可以使用 SSD1306 显示屏
- 可以使用 SH1106 显示屏
- 无需修改 widget 代码
- 完全向后兼容

## Key Differences

### SSD1306 vs SH1106

| Feature | SSD1306 | SH1106 |
|---------|---------|--------|
| Resolution | 128x64 | 128x64 |
| I2C Address | 0x3C | 0x3C |
| Buffer Size | 1024 bytes | 1024 bytes |
| Initialization | `Adafruit_SSD1306` | `Adafruit_SH1106` |
| Begin Method | `begin(SSD1306_SWITCHCAPVCC, 0x3C)` | `begin(SH1106_I2C_ADDRESS, reset)` |

## PlatformIO Configuration

Add to your `platformio.ini`:

```ini
[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino

lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SH1106
    MonoGUI
```

## Code Example

### ✅ 使用 SH1106（推荐新方法）

```cpp
#define USE_SH1106  // 在包含 MonoGUI 之前定义

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <MonoGUI.h>

#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);

void setup() {
  Wire.begin();
  display.begin(SH1106_I2C_ADDRESS, OLED_RESET);
  display.clearDisplay();

  // 使用 MonoGUI widgets - 完全相同的代码！
  MyTextBox txt(0, 20, 15, 0);
  txt.setText("Hello SH1106");
  txt.TextBoxShow(display);

  display.display();
}

void loop() {
  // 你的代码
}
```

### ✅ 使用 SSD1306（原始方法）

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MonoGUI.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // 使用 MonoGUI widgets - 完全相同的代码！
  MyTextBox txt(0, 20, 15, 0);
  txt.setText("Hello SSD1306");
  txt.TextBoxShow(display);

  display.display();
}

void loop() {
  // 你的代码
}
```

## Conditional Compilation

For code that supports both displays:

```cpp
#ifdef USE_SH1106
  #include <Adafruit_SH1106.h>
  Adafruit_SH1106 display(OLED_RESET);
#else
  #include <Adafruit_SSD1306.h>
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

void setup() {
  #ifdef USE_SH1106
    display.begin(SH1106_I2C_ADDRESS, OLED_RESET);
  #else
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  #endif

  display.clearDisplay();

  // MonoGUI widgets work with both displays
  MyTextBox txt(0, 20, 15, 0);
  txt.setText("Universal Display");
  txt.TextBoxShow(display);

  display.display();
}
```

## Pin Connections

### I2C Connection (ESP32/Arduino)

```
Display    ESP32/Arduino
--------   -------------
VCC        3.3V or 5V
GND        GND
SCL        D22/SCL (ESP32) or A5/UNO
SDA        D21/SDA (ESP32) or A4/UNO
```

## Troubleshooting

### Display Not Working

1. **Check I2C Address**: Run I2C scanner to verify address
   ```cpp
   #include <Wire.h>
   void setup() {
     Wire.begin();
     Serial.begin(115200);
     for(byte addr = 1; addr < 127; addr++) {
       Wire.beginTransmission(addr);
       if(Wire.endTransmission() == 0) {
         Serial.print("Found device at 0x");
         Serial.println(addr, HEX);
       }
     }
   }
   ```

2. **Check Wiring**: Verify SCL/SDA connections
3. **Check Reset Pin**: Try different reset pin values (-1, 4, 16)

### Library Conflicts

**重要：避免重复包含库头文件**

如果你同时安装了 SSD1306 和 SH1106 库，确保：

```cpp
// ✅ 正确：只包含一个库
#include <Adafruit_SH1106.h>   // 选择这一个
#include <MonoGUI.h>

// ❌ 错误：不要同时包含两个库
#include <Adafruit_SSD1306.h>
#include <Adafruit_SH1106.h>
#include <MonoGUI.h>  // 会导致冲突！
```

**常见错误：**
```
error: redefinition of 'class Adafruit_SH1106'
```

**解决方法：**
1. 在你的代码中只包含一个显示库
2. 确保在包含 MonoGUI.h **之前** 包含显示库
3. 不要让 MonoGUI.h 自动包含显示库（它现在不会这样做了）

## Performance Notes

- Both displays have similar performance
- SH1106 may have slightly different refresh behavior
- MonoGUI widgets are optimized for both displays

## Examples

See `examples/SH1106_Widgets/` for a complete working example with all widget types.

## 🔧 关键改进

### 之前的问题：
```cpp
// ❌ 错误：cannot convert 'Adafruit_SH1106' to 'Adafruit_SSD1306&'
Adafruit_SH1106 display(-1);
MyTextBox txt(0, 20, 15, 0);
txt.TextBoxShow(display);  // 错误！
```

### 现在的解决方案：
```cpp
// ✅ 现在可以正常工作！
Adafruit_SH1106 display(-1);
MyTextBox txt(0, 20, 15, 0);
txt.TextBoxShow(display);  // 正常工作！
```

所有 widget 函数现在都使用 `Adafruit_GFX&` 基类引用：
- `TextBoxShow(Adafruit_GFX &display)`
- `CheckBoxShow(Adafruit_GFX &display)`
- `Show(Adafruit_GFX &display)` (ButtonBox, ComboBox, SymbolBox)
- `msgBox(Adafruit_GFX &display, String msg)`
- `topBar(Adafruit_GFX &display, int ws)`
- `drawQrCode(Adafruit_GFX &display, ...)`

## ⚠️ 重要：正确的包含顺序

**必须在 MonoGUI.h 之前包含显示库头文件！**

```cpp
// ✅ 正确的顺序：
#include <Adafruit_SH1106.h>  // 或 <Adafruit_SSD1306.h>
#include <MonoGUI.h>

// ❌ 错误的顺序（会导致编译错误）：
#include <MonoGUI.h>
#include <Adafruit_SH1106.h>  // 太晚了！
```

## 📋 从 SSD1306 迁移到 SH1106

1. **更改包含的头文件**（在 MonoGUI.h 之前）：
   ```cpp
   // 从：
   #include <Adafruit_SSD1306.h>
   #include <MonoGUI.h>

   // 到：
   #include <Adafruit_SH1106.h>
   #include <MonoGUI.h>
   ```

2. **更改显示屏初始化**：
   ```cpp
   // 从：
   Adafruit_SSD1306 display(128, 64, &Wire, -1);
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

   // 到：
   Adafruit_SH1106 display(-1);
   display.begin(SH1106_I2C_ADDRESS, -1);
   ```

3. **无需修改 MonoGUI widget 代码！** 🎉

For more information, visit:
- Adafruit SH1106 Library: https://github.com/adafruit/Adafruit-SH1106-library
- Adafruit GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
