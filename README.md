# MonoGUI Library

Arduino GUI library for monochrome OLED displays (SSD1306 128x64 pixels).

## Features

- **TextBox**: Text input field with type filtering (numeric, uppercase)
- **CheckBox**: Checkbox widget with toggle functionality
- **ButtonBox**: Clickable button widget
- **ComboBox**: Value/item selector with increment/decrement
- **SymbolBox**: Symbol selection widget
- **Utility Functions**: Message boxes, status bar, QR code placeholder

## Dependencies

- Adafruit GFX Library
- Adafruit SSD1306 Library
- Adafruit SH1106 Library (alternative to SSD1306)

## Hardware

- ESP32 or Arduino microcontroller
- SSD1306 or SH1106 OLED display (128x64 pixels)
- Input device (rotary encoder, buttons, or serial)

## Installation

1. Download this library as a ZIP file
2. In Arduino IDE, go to Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file

Or copy the `MonoGUI` folder to your Arduino `libraries` folder.

## Quick Start

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

  // Create widgets
  MyTextBox textBox(0, 20, 15, 0);
  textBox.setText("Hello World");
  textBox.isSelect = true;
  textBox.TextBoxShow(display);

  display.display();
}

void loop() {
  // Handle input and update widgets
}
```

## Widgets

### TextBox
```cpp
MyTextBox txt(0, 20, 15, 0);  // x, y, length, type(0=all,1=numeric,2=uppercase)
txt.setText("Hello");
txt.addChar('A');
txt.backspace();
txt.TextBoxShow(display);
```

### CheckBox
```cpp
MyCheckBox chk(0, 35, "Enable", false);
chk.Toggle();
chk.setChecked(true);
chk.CheckBoxShow(display);
```

### ButtonBox
```cpp
MyButtonBox btn(0, 50, "Click Me", true);
btn.setState(true);
if (btn.isClicked()) {
  // Handle click
}
btn.Show(display);
```

### ComboBox
```cpp
// Value mode
MyComboBox cmbValue(0, 20, 10, true);
cmbValue.setValue(100);
cmbValue.Show(display);

// Item mode
const char* items[] = {"Option1", "Option2", "Option3"};
MyComboBox cmbItem(0, 35, 10, false);
cmbItem.SelectItem(items, 3);
cmbItem.Show(display);
```

### SymbolBox
```cpp
MySymbolBox sym(0, 20, '/', '-');
sym.setSymbol('+');
sym.Show(display);
```

## Examples

See the `examples/` folder for complete example sketches:

- **BasicWidgets**: Demonstrates all widget types (SSD1306)
- **InteractiveMenu**: Simple menu navigation
- **TextInput**: Text input with virtual keyboard
- **SH1106_Widgets**: Using MonoGUI with SH1106 display

## License

This library is extracted from the ESP32APRS_T-TWR project.
Original project: https://github.com/nakhonthai/ESP32APRS_T-TWR

## Contributing

Contributions are welcome! Please feel free to submit pull requests.
