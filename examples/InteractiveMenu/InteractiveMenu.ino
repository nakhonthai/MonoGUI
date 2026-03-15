/**
 * MONO_GUI Interactive Menu Example
 *
 * This example demonstrates a simple interactive menu system
 * using MONO_GUI widgets. Shows navigation, selection, and
 * settings adjustment.
 *
 * Hardware:
 * - ESP32 or Arduino
 * - SSD1306 OLED display (128x64) connected via I2C
 * - Rotary encoder or buttons (simulated via Serial)
 *
 * Dependencies:
 * - Adafruit GFX Library
 * - Adafruit SSD1306 Library
 * - MonoGUI Library
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MonoGUI.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Menu structure
enum MenuState {
  MENU_MAIN,
  MENU_SETTINGS,
  MENU_ABOUT,
  MENU_SETTINGS_WIFI,
  MENU_SETTINGS_DISPLAY,
  MENU_SETTINGS_BACKLIGHT
};

MenuState currentMenu = MENU_MAIN;
int menuIndex = 0;

// Settings values
bool wifiEnabled = true;
int brightness = 50;
bool backlight = true;
String ssid = "MyNetwork";

// Widgets for settings page
MyCheckBox chkWifi(0, 25, "WiFi", false);
MyComboBox cmbBrightness(60, 25, 3, true);
MyCheckBox chkBacklight(0, 45, "Backlight", false);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Initialize widgets with current settings
  chkWifi.setChecked(wifiEnabled);
  cmbBrightness.setValue(brightness);
  chkBacklight.setChecked(backlight);

  Serial.println("MonoGUI Interactive Menu Example");
  Serial.println("=================================");
  Serial.println("Commands:");
  Serial.println("  u: Move up");
  Serial.println("  d: Move down");
  Serial.println("  s: Select/Enter");
  Serial.println("  b: Back");
  Serial.println("  t: Toggle checkbox/Adjust value");
  Serial.println("  +/-: Adjust brightness");
}

void loop() {
  display.clearDisplay();
  displayMenu();
  display.display();

  // Check for serial input
  if (Serial.available()) {
    handleInput(Serial.read());
  }

  delay(100);
}

void displayMenu() {
  switch (currentMenu) {
    case MENU_MAIN:
      displayMainMenu();
      break;
    case MENU_SETTINGS:
      displaySettingsMenu();
      break;
    case MENU_ABOUT:
      displayAboutMenu();
      break;
    case MENU_SETTINGS_WIFI:
      displayWifiSettings();
      break;
    case MENU_SETTINGS_DISPLAY:
      displayDisplaySettings();
      break;
  }
}

void displayMainMenu() {
  topBar(display, 2);
  display.setCursor(0, 15);

  const char* items[] = {
    "> Settings",
    "  About",
    "  Status"
  };

  for (int i = 0; i < 3; i++) {
    display.setCursor(0, 15 + i * 12);

    if (i == menuIndex) {
      display.print(">");
    } else {
      display.print(" ");
    }

    display.print(items[i]);
  }
}

void displaySettingsMenu() {
  topBar(display, 2);
  display.setCursor(0, 12);
  display.print("SETTINGS:");

  const char* items[] = {
    "WiFi",
    "Display",
    "Back"
  };

  for (int i = 0; i < 3; i++) {
    display.setCursor(5, 25 + i * 12);

    if (i == menuIndex) {
      display.print(">");
    } else {
      display.print(" ");
    }

    display.print(items[i]);
  }
}

void displayAboutMenu() {
  topBar(display, 0);
  display.setCursor(0, 15);
  display.print("MONO_GUI Library");
  display.setCursor(0, 30);
  display.print("Version: 1.0.0");
  display.setCursor(0, 45);
  display.print("ESP32 GUI Lib");

  display.setCursor(0, 57);
  display.print("[Back: b]");
}

void displayWifiSettings() {
  topBar(display, 1);
  display.setCursor(0, 12);
  display.print("WiFi Settings");

  chkWifi.CheckBoxShow(display);

  display.setCursor(0, 40);
  display.print("SSID: ");
  display.setCursor(0, 52);
  String shortSsid = ssid.substring(0, 15);
  display.print(shortSsid);

  display.setCursor(0, 57);
  display.print("[Toggle: t] [Back: b]");
}

void displayDisplaySettings() {
  topBar(display, 1);
  display.setCursor(0, 12);
  display.print("Display Settings");

  cmbBrightness.Show(display);

  display.setCursor(0, 40);
  display.print("Brightness: ");
  display.print(brightness);
  display.print("%");

  chkBacklight.Show(display);

  display.setCursor(0, 57);
  display.print("[Adj: +/-] [Back: b]");
}

void handleInput(char cmd) {
  switch (cmd) {
    case 'u': // Up
      if (menuIndex > 0) menuIndex--;
      break;

    case 'd': // Down
      if (currentMenu == MENU_MAIN && menuIndex < 2) menuIndex++;
      else if (currentMenu == MENU_SETTINGS && menuIndex < 2) menuIndex++;
      break;

    case 's': // Select/Enter
      enterMenu();
      break;

    case 'b': // Back
      goBack();
      break;

    case 't': // Toggle
      if (currentMenu == MENU_SETTINGS_WIFI) {
        chkWifi.Toggle();
        wifiEnabled = chkWifi.getState();
        Serial.print("WiFi: ");
        Serial.println(wifiEnabled ? "ON" : "OFF");
      } else if (currentMenu == MENU_SETTINGS_DISPLAY) {
        chkBacklight.Toggle();
        backlight = chkBacklight.getState();
        Serial.print("Backlight: ");
        Serial.println(backlight ? "ON" : "OFF");
      }
      break;

    case '+': // Increase
      if (currentMenu == MENU_SETTINGS_DISPLAY) {
        if (brightness < 100) brightness += 5;
        cmbBrightness.setValue(brightness);
        Serial.print("Brightness: ");
        Serial.println(brightness);
      }
      break;

    case '-': // Decrease
      if (currentMenu == MENU_SETTINGS_DISPLAY) {
        if (brightness > 0) brightness -= 5;
        cmbBrightness.setValue(brightness);
        Serial.print("Brightness: ");
        Serial.println(brightness);
      }
      break;
  }
}

void enterMenu() {
  switch (currentMenu) {
    case MENU_MAIN:
      if (menuIndex == 0) {
        currentMenu = MENU_SETTINGS;
        menuIndex = 0;
        Serial.println("Enter: Settings");
      } else if (menuIndex == 1) {
        currentMenu = MENU_ABOUT;
        Serial.println("Enter: About");
      }
      break;

    case MENU_SETTINGS:
      if (menuIndex == 0) {
        currentMenu = MENU_SETTINGS_WIFI;
        Serial.println("Enter: WiFi Settings");
      } else if (menuIndex == 1) {
        currentMenu = MENU_SETTINGS_DISPLAY;
        Serial.println("Enter: Display Settings");
      } else if (menuIndex == 2) {
        currentMenu = MENU_MAIN;
        menuIndex = 0;
        Serial.println("Back to Main");
      }
      break;

    case MENU_SETTINGS_WIFI:
      // No sub-menus, just settings
      break;

    case MENU_SETTINGS_DISPLAY:
      // No sub-menus, just settings
      break;
  }
}

void goBack() {
  switch (currentMenu) {
    case MENU_SETTINGS:
    case MENU_ABOUT:
      currentMenu = MENU_MAIN;
      menuIndex = 0;
      Serial.println("Back to Main");
      break;

    case MENU_SETTINGS_WIFI:
    case MENU_SETTINGS_DISPLAY:
      currentMenu = MENU_SETTINGS;
      menuIndex = 0;
      Serial.println("Back to Settings");
      break;
  }
}
