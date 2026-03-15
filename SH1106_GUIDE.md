# MonoGUI with SH1106 Display Guide

## Overview

MonoGUI now supports both SSD1306 and SH1106 OLED displays. The SH1106 is a compatible alternative to SSD1306 with similar specifications but different initialization.

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

### Using SH1106

```cpp
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

  // Use MonoGUI widgets
  MyTextBox txt(0, 20, 15, 0);
  txt.setText("Hello SH1106");
  txt.TextBoxShow(display);

  display.display();
}

void loop() {
  // Your code here
}
```

### Using SSD1306 (Original)

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

  // Use MonoGUI widgets
  MyTextBox txt(0, 20, 15, 0);
  txt.setText("Hello SSD1306");
  txt.TextBoxShow(display);

  display.display();
}

void loop() {
  // Your code here
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

If you have both SSD1306 and SH1106 libraries installed, use only one in your code:

```cpp
// Choose ONE:
#include <Adafruit_SSD1306.h>  // OR
#include <Adafruit_SH1106.h>   // NOT BOTH
```

## Performance Notes

- Both displays have similar performance
- SH1106 may have slightly different refresh behavior
- MonoGUI widgets are optimized for both displays

## Examples

See `examples/SH1106_Widgets/` for a complete working example with all widget types.

## Migration from SSD1306 to SH1106

1. Replace `#include <Adafruit_SSD1306.h>` with `#include <Adafruit_SH1106.h>`
2. Update display initialization:
   ```cpp
   // From:
   Adafruit_SSD1306 display(128, 64, &Wire, -1);
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

   // To:
   Adafruit_SH1106 display(-1);
   display.begin(SH1106_I2C_ADDRESS, -1);
   ```
3. No changes needed for MonoGUI widget code!

For more information, visit:
- Adafruit SH1106 Library: https://github.com/adafruit/Adafruit-SH1106-library
- Adafruit GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
