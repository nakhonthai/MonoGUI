/**
 * MONO_GUI - Arduino GUI Library for Monochrome OLED Displays
 * Based on ESP32APRS_T-TWR GUI components
 *
 * This library provides GUI components for SSD1306 OLED displays:
 * - TextBox: Text input widget
 * - CheckBox: Checkbox widget
 * - ButtonBox: Button widget
 * - ComboBox: Dropdown/selection widget
 * - SymbolBox: Symbol selection widget
 * - Menu rendering system
 *
 * @author Extracted from ESP32APRS_T-TWR project
 * @version 1.0.0
 */

#ifndef MONOGUI_H
#define MONOGUI_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

// Note: MonoGUI now works with Adafruit_GFX base class
// Users must include their preferred display library BEFORE MonoGUI.h:
//   #include <Adafruit_SSD1306.h>  // For SSD1306
//   #include <Adafruit_SH1106.h>   // For SH1106 (any variant)
//   #include <MonoGUI.h>

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Forward declarations
class MyTextBox;
class MyCheckBox;
class MyButtonBox;
class MyComboBox;
class MySymbolBox;

// Include all component headers
#include "TextBox.h"
#include "CheckBox.h"
#include "ButtonBox.h"
#include "ComboBox.h"
#include "SymbolBox.h"

// Utility functions - Use Adafruit_GFX base class for display compatibility
void msgBox(Adafruit_GFX &display, String msg);
void topBar(Adafruit_GFX &display, int ws);
void drawQrCode(Adafruit_GFX &display, const char *text, int x, int y, int scale);

#endif // MONOGUI_H
