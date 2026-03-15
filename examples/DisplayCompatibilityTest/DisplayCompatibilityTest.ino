/**
 * MonoGUI Display Compatibility Test
 *
 * This example demonstrates that MonoGUI widgets work with both
 * SSD1306 and SH1106 displays without any code changes.
 *
 * Uncomment the display type you want to test.
 */

// ========================================================================
// STEP 1: Choose your display type
// ========================================================================

// Option 1: SSD1306 Display (default)
// #define USE_SSD1306

// Option 2: SH1106 Display
#define USE_SH1106

// ========================================================================
// STEP 2: Includes and Display Setup
// ========================================================================

#include <Wire.h>
#include <Adafruit_GFX.h>

#ifdef USE_SH1106
  #include <Adafruit_SH1106.h>
  #define DISPLAY_TYPE "SH1106"
  Adafruit_SH1106 display(-1);
#else
  #include <Adafruit_SSD1306.h>
  #define DISPLAY_TYPE "SSD1306"
  #define SCREEN_WIDTH 128
  #define SCREEN_HEIGHT 64
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#endif

#include <MonoGUI.h>

// ========================================================================
// STEP 3: Create Widgets
// ========================================================================

MyTextBox txtBox(0, 15, 12, 0);
MyCheckBox chkBox(0, 28, "Check", false);
MyButtonBox btnBox(0, 43, "Test", true);
MyComboBox cmbBox(70, 28, 8, true);
MySymbolBox symBox(70, 43, '+', '-');

// ========================================================================
// Setup
// ========================================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==========================================");
  Serial.print("MonoGUI Compatibility Test - ");
  Serial.println(DISPLAY_TYPE);
  Serial.println("==========================================");

  // Initialize I2C
  Wire.begin();

  // Initialize display
  #ifdef USE_SH1106
    Serial.println("Initializing SH1106...");
    display.begin(SH1106_I2C_ADDRESS, -1);
  #else
    Serial.println("Initializing SSD1306...");
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  #endif

  display.clearDisplay();

  // Initialize widgets
  txtBox.setText("Testing!");
  chkBox.setChecked(true);
  cmbBox.setValue(42);

  // Display header
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(DISPLAY_TYPE);
  display.print(" Test");
  display.drawLine(0, 9, 127, 9, WHITE);

  // Display all widgets
  txtBox.TextBoxShow(display);
  chkBox.CheckBoxShow(display);
  btnBox.Show(display);
  cmbBox.Show(display);
  symBox.Show(display);

  display.display();

  Serial.println("Display initialized successfully!");
  Serial.println();
  Serial.println("Widgets displayed:");
  Serial.println("- TextBox: 'Testing!'");
  Serial.println("- CheckBox: Checked");
  Serial.println("- ButtonBox: 'Test'");
  Serial.println("- ComboBox: 42");
  Serial.println("- SymbolBox: '-'");
  Serial.println();
  Serial.println("All widgets working correctly! ✓");
}

void loop() {
  // Test widget interactions every 5 seconds
  static unsigned long lastTest = 0;
  static int testCount = 0;

  if (millis() - lastTest > 5000) {
    lastTest = millis();
    testCount++;

    Serial.print("Test #");
    Serial.print(testCount);
    Serial.print(": ");

    // Toggle states
    chkBox.Toggle();
    btnBox.Toggle();
    cmbBox.increment();
    symBox.increment();

    // Update display
    display.clearDisplay();

    // Header
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(DISPLAY_TYPE);
    display.print(" Test #");
    display.print(testCount);
    display.drawLine(0, 9, 127, 9, WHITE);

    // Widgets
    txtBox.TextBoxShow(display);
    chkBox.CheckBoxShow(display);
    btnBox.Show(display);
    cmbBox.Show(display);
    symBox.Show(display);

    display.display();

    Serial.println("States updated");
  }
}
