/**
 * MonoGUI Library - SH1106 Display Example
 *
 * This example demonstrates how to use MonoGUI with SH1106 OLED display
 * instead of SSD1306. The SH1106 is compatible but requires different
 * initialization.
 */

// Define USE_SH1106 before including MonoGUI to use SH1106 display
#define USE_SH1106

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <MonoGUI.h>

// Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)

// Create SH1106 display instance
Adafruit_SH1106 display(OLED_RESET);

// Widget instances
MyTextBox myTextBox(0, 10, 15, 0);      // Text input (all characters)
MyCheckBox myCheckBox(0, 25, "Enable", false);  // Checkbox
MyButtonBox myButton(0, 40, "Click Me", true);  // Button
MyComboBox myCombo(0, 55, 10, true);    // Value combobox

int currentWidget = 0;  // Current selected widget
const int totalWidgets = 4;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("MonoGUI with SH1106 Display");

  // Initialize I2C
  Wire.begin();

  // Initialize SH1106 display
  // SH1106 typically uses I2C address 0x3C
  display.begin(SH1106_I2C_ADDRESS, OLED_RESET);

  // Clear the buffer
  display.clearDisplay();

  // Initialize widgets
  myTextBox.setText("Hello");
  myCheckBox.setChecked(false);

  // Display all widgets
  displayWidgets();

  Serial.println("Widgets displayed. Use serial input to navigate:");
  Serial.println("1: Select TextBox");
  Serial.println("2: Select CheckBox");
  Serial.println("3: Select Button");
  Serial.println("4: Select ComboBox");
  Serial.println("c: Toggle CheckBox");
  Serial.println("b: Button click");
  Serial.println("+: Increment ComboBox");
  Serial.println("-: Decrement ComboBox");
}

void loop() {
  // Handle serial input for demo purposes
  if(Serial.available()) {
    char input = Serial.read();

    switch(input) {
      case '1':
        selectWidget(0);
        break;
      case '2':
        selectWidget(1);
        break;
      case '3':
        selectWidget(2);
        break;
      case '4':
        selectWidget(3);
        break;
      case 'c':
        myCheckBox.Toggle();
        Serial.print("CheckBox: ");
        Serial.println(myCheckBox.getState() ? "Checked" : "Unchecked");
        displayWidgets();
        break;
      case 'b':
        myButton.setState(!myButton.getState());
        Serial.println("Button clicked!");
        displayWidgets();
        break;
      case '+':
        myCombo.setValue(myCombo.getValue() + 1);
        Serial.print("ComboBox value: ");
        Serial.println(myCombo.getValue());
        displayWidgets();
        break;
      case '-':
        myCombo.setValue(myCombo.getValue() - 1);
        Serial.print("ComboBox value: ");
        Serial.println(myCombo.getValue());
        displayWidgets();
        break;
    }
  }
}

void selectWidget(int index) {
  // Deselect all widgets
  myTextBox.isSelect = false;
  myCheckBox.isSelect = false;
  myButton.isSelect = false;
  myCombo.isSelect = false;

  // Select specified widget
  switch(index) {
    case 0:
      myTextBox.isSelect = true;
      Serial.println("TextBox selected");
      break;
    case 1:
      myCheckBox.isSelect = true;
      Serial.println("CheckBox selected");
      break;
    case 2:
      myButton.isSelect = true;
      Serial.println("Button selected");
      break;
    case 3:
      myCombo.isSelect = true;
      Serial.println("ComboBox selected");
      break;
  }

  currentWidget = index;
  displayWidgets();
}

void displayWidgets() {
  display.clearDisplay();

  // Display title
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("SH1106 MonoGUI");

  // Display all widgets
  myTextBox.CheckBoxShow(display);
  myCheckBox.CheckBoxShow(display);
  myButton.Show(display);
  myCombo.Show(display);

  display.display();
}
