/**
 * MONO_GUI Text Input Example
 *
 * This example demonstrates text input using a virtual keyboard
 * displayed on the OLED screen. Ideal for projects without a physical
 * keyboard.
 *
 * Hardware:
 * - ESP32 or Arduino
 * - SSD1306 OLED display (128x64) connected via I2C
 * - Rotary encoder or buttons (simulated via Serial in this example)
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

// Virtual keyboard layout
const char keyboard[4][10] = {
  "1234567890",
  "QWERTYUIOP",
  "ASDFGHJKL-",
  "ZXCVBNM. _"
};

// Input state
MyTextBox inputField(0, 0, 21, 0);
int cursorX = 0;
int cursorY = 0;
bool capsLock = false;
bool inputMode = true;

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

  inputField.setText("Enter text");

  Serial.println("MonoGUI Text Input Example");
  Serial.println("==========================");
  Serial.println("Use Serial Monitor to simulate input:");
  Serial.println("  w/a/s/d: Move cursor left/down/up/right");
  Serial.println("  space: Select character");
  Serial.println("  b: Backspace");
  Serial.println("  c: Caps lock");
  Serial.println("  e: Finish input");
}

void loop() {
  display.clearDisplay();

  if (inputMode) {
    displayTextInput();
  } else {
    displayResult();
  }

  display.display();

  // Check for serial input
  if (Serial.available()) {
    handleInput(Serial.read());
  }

  delay(100);
}

void displayTextInput() {
  // Show input field at top
  display.setCursor(0, 0);
  display.print("Text Input:");
  inputField.y = 10;
  inputField.TextBoxShow(display);

  // Show caps lock indicator
  display.setCursor(110, 0);
  if (capsLock) {
    display.print("A");
  } else {
    display.print("a");
  }

  // Show keyboard
  displayKeyboard();
}

void displayKeyboard() {
  int keyboardStartY = 25;

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 10; col++) {
      char c = keyboard[row][col];
      int x = col * 12 + 4;
      int y = keyboardStartY + row * 10;

      // Draw cursor
      if (row == cursorY && col == cursorX) {
        display.drawRect(x - 1, y - 1, 10, 10, WHITE);
      }

      // Draw character
      display.setCursor(x, y);
      char displayChar = (capsLock && c >= 'a' && c <= 'z') ? c - 32 : c;
      display.print(displayChar);
    }
  }

  // Instructions at bottom
  display.setCursor(0, 57);
  display.print("WASD:Move SP:Sel B:BS C:Cap E:End");
}

void displayResult() {
  display.setCursor(0, 0);
  display.print("Input Complete:");

  display.setCursor(0, 20);
  display.print("Text:");

  // Word wrap for result
  String text = inputField.getText();
  int x = 0, y = 35;
  for (int i = 0; i < text.length(); i++) {
    if (x > SCREEN_WIDTH - 6) {
      x = 0;
      y += 10;
    }
    display.setCursor(x, y);
    display.print(text[i]);
    x += 6;
  }

  display.setCursor(0, 57);
  display.print("Press any key to restart");
}

void handleInput(char cmd) {
  if (!inputMode) {
    // Restart input mode on any key
    inputMode = true;
    inputField.setText("");
    cursorX = 0;
    cursorY = 0;
    return;
  }

  switch (cmd) {
    case 'w': // Up
      if (cursorY > 0) cursorY--;
      break;

    case 's': // Down
      if (cursorY < 3) cursorY++;
      break;

    case 'a': // Left
      if (cursorX > 0) cursorX--;
      break;

    case 'd': // Right
      if (cursorX < 9) cursorX++;
      break;

    case ' ': // Select character
      {
        char c = keyboard[cursorY][cursorX];
        if (capsLock && c >= 'a' && c <= 'z') {
          c -= 32;
        }
        inputField.addChar(c);
        Serial.print("Added: ");
        Serial.println(c);
      }
      break;

    case 'b': // Backspace
    case 127: // DEL key
      inputField.backspace();
      Serial.println("Backspace");
      break;

    case 'c': // Caps lock
      capsLock = !capsLock;
      Serial.print("Caps lock: ");
      Serial.println(capsLock ? "ON" : "OFF");
      break;

    case 'e': // Finish input
      inputMode = false;
      Serial.print("Final text: ");
      Serial.println(inputField.getText());
      break;
  }
}
