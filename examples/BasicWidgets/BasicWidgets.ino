/**
 * MONO_GUI Basic Widgets Example
 *
 * This example demonstrates all available widget types:
 * - TextBox
 * - CheckBox
 * - ButtonBox
 * - ComboBox (value mode)
 * - ComboBox (item mode)
 * - SymbolBox
 *
 * Hardware:
 * - ESP32 or Arduino
 * - SSD1306 OLED display (128x64) connected via I2C
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

// Create widget instances
MyTextBox myTextBox(0, 15, 12, 0);
MyCheckBox myCheckBox(0, 30, "Enable", false);
MyButtonBox myButton(0, 45, "Click Me", true);
MyComboBox myComboValue(70, 15, 5, true);
MyComboBox myComboItem(70, 30, 8, false);
MySymbolBox mySymbol(70, 45, '/', '-');

const char* comboItems[] = {"Item1", "Item2", "Item3", "Item4"};

int currentPage = 0;
unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Initialize I2C with ESP32 default pins
  Wire.begin();

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Initialize widgets
  myTextBox.setText("Hello");
  myCheckBox.setChecked(true);
  myComboValue.setValue(50);
  myComboItem.SelectItem(comboItems, 4);
  mySymbol.setSymbol('+');

  // Show welcome message
  topBar(display, 0);
  display.setCursor(0, 20);
  display.print("MONO_GUI Library");
  display.setCursor(0, 35);
  display.print("Basic Widgets");
  display.setCursor(0, 50);
  display.print("Example");
  display.display();

  delay(2000);

  Serial.println("MonoGUI Basic Widgets Example");
  Serial.println("==============================");
  Serial.println("Commands:");
  Serial.println("  1-6: Select page (1=TextBox, 2=CheckBox, 3=Button)");
  Serial.println("       4=ComboValue, 5=ComboItem, 6=Symbol)");
  Serial.println("  t: Toggle selected widget");
  Serial.println("  u: Increment value");
  Serial.println("  d: Decrement value");
  Serial.println("  s: Enter text (for TextBox)");
  Serial.println("  c: Clear text (for TextBox)");
}

void loop() {
  unsigned long currentTime = millis();

  // Update display every 100ms
  if (currentTime - lastUpdate >= 100) {
    lastUpdate = currentTime;
    displayPage(currentPage);
  }

  // Check for serial input
  if (Serial.available()) {
    char cmd = Serial.read();
    handleInput(cmd);
  }
}

void displayPage(int page) {
  display.clearDisplay();

  // Show top bar
  String pageNames[] = {"TextBox", "CheckBox", "Button", "ComboVal", "ComboItm", "Symbol"};
  topBar(display, 0);

  // Show current page indicator
  display.setCursor(90, 1);
  display.print("[");
  display.print(page + 1);
  display.print("/6]");

  // Display widgets based on current page
  switch (page) {
    case 0: // TextBox
      display.setCursor(0, 55);
      display.print("Text: ");
      display.print(myTextBox.getText());
      myTextBox.TextBoxShow(display);
      break;

    case 1: // CheckBox
      display.setCursor(0, 55);
      display.print("State: ");
      display.print(myCheckBox.getState() ? "ON" : "OFF");
      myCheckBox.CheckBoxShow(display);
      break;

    case 2: // Button
      display.setCursor(0, 55);
      display.print("Click: ");
      display.print(myButton.isClicked() ? "YES" : "NO");
      myButton.Show(display);
      break;

    case 3: // ComboBox Value
      display.setCursor(0, 55);
      display.print("Value: ");
      display.print(myComboValue.getValue());
      myComboValue.Show(display);
      break;

    case 4: // ComboBox Item
      display.setCursor(0, 55);
      display.print("Item: ");
      display.print(myComboItem.getIndex());
      myComboItem.Show(display);
      break;

    case 5: // SymbolBox
      display.setCursor(0, 55);
      display.print("Symbol: ");
      display.print(mySymbol.getSymbol());
      mySymbol.Show(display);
      break;
  }

  display.display();
}

void handleInput(char cmd) {
  switch (cmd) {
    case '1':
      currentPage = 0;
      Serial.println("Page: TextBox");
      break;
    case '2':
      currentPage = 1;
      Serial.println("Page: CheckBox");
      break;
    case '3':
      currentPage = 2;
      Serial.println("Page: Button");
      break;
    case '4':
      currentPage = 3;
      Serial.println("Page: ComboBox Value");
      break;
    case '5':
      currentPage = 4;
      Serial.println("Page: ComboBox Item");
      break;
    case '6':
      currentPage = 5;
      Serial.println("Page: SymbolBox");
      break;

    case 't': // Toggle
      switch (currentPage) {
        case 1: myCheckBox.Toggle(); Serial.println("CheckBox toggled"); break;
        case 2: myButton.Toggle(); Serial.println("Button clicked"); break;
      }
      break;

    case 'u': // Increment
      switch (currentPage) {
        case 3: myComboValue.increment(); Serial.print("Value: "); Serial.println(myComboValue.getValue()); break;
        case 4: myComboItem.increment(); Serial.print("Item: "); Serial.println(myComboItem.getIndex()); break;
        case 5: mySymbol.increment(); Serial.print("Symbol: "); Serial.println(mySymbol.getSymbol()); break;
      }
      break;

    case 'd': // Decrement
      switch (currentPage) {
        case 3: myComboValue.decrement(); Serial.print("Value: "); Serial.println(myComboValue.getValue()); break;
        case 4: myComboItem.decrement(); Serial.print("Item: "); Serial.println(myComboItem.getIndex()); break;
        case 5: mySymbol.decrement(); Serial.print("Symbol: "); Serial.println(mySymbol.getSymbol()); break;
      }
      break;

    case 's': // Set text
      if (currentPage == 0) {
        Serial.println("Enter text (max 12 chars):");
        while (!Serial.available());
        String newText = Serial.readStringUntil('\n');
        newText.trim();
        myTextBox.setText(newText.c_str());
        Serial.print("Text set to: ");
        Serial.println(myTextBox.getText());
      }
      break;

    case 'c': // Clear text
      if (currentPage == 0) {
        myTextBox.clear();
        Serial.println("Text cleared");
      }
      break;
  }
}
