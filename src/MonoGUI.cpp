/**
 * MONO_GUI - Arduino GUI Library for Monochrome OLED Displays
 * Implementation file
 */

#include "MonoGUI.h"

// ========================================================================
// TEXTBOX IMPLEMENTATION
// ========================================================================

MyTextBox::MyTextBox(int px, int py, int plength, int ptype) {
    x = px;
    y = py;
    length = plength;
    type = ptype;
    memset(text, 0, sizeof(text));
    isSelect = false;
}

void MyTextBox::TextBox(const char *defaultText) {
    strncpy(text, defaultText, sizeof(text) - 1);
    text[sizeof(text) - 1] = '\0';
}

void MyTextBox::TextBoxShow(Adafruit_SSD1306 &display) {
    display.setTextColor(WHITE);
    display.setCursor(x, y);

    // Draw border if selected
    if (isSelect) {
        display.drawRect(x - 2, y - 2, length * 6 + 4, 12, WHITE);
    }

    // Draw text background
    display.fillRect(x, y, length * 6, 10, BLACK);

    // Draw text
    display.print(text);

    // Draw cursor if selected
    if (isSelect) {
        int textLen = strlen(text);
        display.setCursor(x + textLen * 6, y);
        display.print('_');
    }
}

void MyTextBox::setText(const char *newText) {
    strncpy(text, newText, sizeof(text) - 1);
    text[sizeof(text) - 1] = '\0';
}

const char* MyTextBox::getText() {
    return text;
}

void MyTextBox::addChar(char c) {
    int len = strlen(text);
    if (len < length - 1) {
        // Filter based on type
        if (type == 1 && !isdigit(c)) return; // Numeric only
        if (type == 2 && !isupper(c) && !isdigit(c)) return; // Uppercase only

        text[len] = c;
        text[len + 1] = '\0';
    }
}

void MyTextBox::backspace() {
    int len = strlen(text);
    if (len > 0) {
        text[len - 1] = '\0';
    }
}

void MyTextBox::clear() {
    memset(text, 0, sizeof(text));
}

// ========================================================================
// CHECKBOX IMPLEMENTATION
// ========================================================================

MyCheckBox::MyCheckBox(int px, int py, const char *ptext, bool pchecked) {
    x = px;
    y = py;
    strncpy(text, ptext, sizeof(text) - 1);
    text[sizeof(text) - 1] = '\0';
    Checked = pchecked;
    isSelect = false;
}

void MyCheckBox::Toggle() {
    Checked = !Checked;
}

void MyCheckBox::CheckBoxShow(Adafruit_SSD1306 &display) {
    display.setTextColor(WHITE);

    // Draw checkbox
    display.drawRect(x, y, 10, 10, WHITE);
    if (Checked) {
        display.fillRect(x + 2, y + 2, 6, 6, WHITE);
    }

    // Draw selection indicator
    if (isSelect) {
        display.drawPixel(x - 1, y + 5, WHITE);
        display.drawPixel(x + 11, y + 5, WHITE);
    }

    // Draw label
    display.setCursor(x + 14, y + 1);
    display.print(text);
}

void MyCheckBox::setChecked(bool state) {
    Checked = state;
}

bool MyCheckBox::getState() {
    return Checked;
}

// ========================================================================
// BUTTONBOX IMPLEMENTATION
// ========================================================================

MyButtonBox::MyButtonBox(int px, int py, const char *ptext, bool pborder) {
    x = px;
    y = py;
    strncpy(text, ptext, sizeof(text) - 1);
    text[sizeof(text) - 1] = '\0';
    Border = pborder;
    Checked = false;
    isSelect = false;
}

void MyButtonBox::Toggle() {
    Checked = !Checked;
}

void MyButtonBox::Show(Adafruit_SSD1306 &display) {
    int textWidth = strlen(text) * 6;
    int boxWidth = textWidth + 8;

    display.setTextColor(WHITE);

    if (Border) {
        display.drawRect(x, y, boxWidth, 12, WHITE);
    }

    if (Checked || isSelect) {
        display.fillRect(x + 1, y + 1, boxWidth - 2, 10, WHITE);
        display.setTextColor(BLACK);
    }

    display.setCursor(x + 4, y + 2);
    display.print(text);
    display.setTextColor(WHITE);
}

void MyButtonBox::setState(bool state) {
    Checked = state;
}

bool MyButtonBox::getState() {
    return Checked;
}

bool MyButtonBox::isClicked() {
    return Checked;
}

// ========================================================================
// COMBOBOX IMPLEMENTATION
// ========================================================================

MyComboBox::MyComboBox(int px, int py, int plength, bool pisvalue) {
    x = px;
    y = py;
    length = plength;
    current = 0;
    current_index = 0;
    isValue = pisvalue;
    isSelect = false;
    memset(item, 0, sizeof(item));
}

void MyComboBox::SelectValue(long val_min, long val_max, long step) {
    // This would be called from user input handler
    // Not implemented here - handled by increment/decrement
}

void MyComboBox::SelectItem(const char* items[], int count) {
    for (int i = 0; i < count && i < 20; i++) {
        strncpy(item[i], items[i], 19);
        item[i][19] = '\0';
    }
}

void MyComboBox::Show(Adafruit_SSD1306 &display) {
    display.setTextColor(WHITE);
    display.setCursor(x, y);

    // Draw border if selected
    if (isSelect) {
        display.drawRect(x - 2, y - 2, length * 6 + 10, 12, WHITE);
    }

    // Draw background
    display.fillRect(x, y, length * 6 + 6, 10, BLACK);

    // Draw value or item
    if (isValue) {
        display.print(current);
    } else {
        display.print(item[current_index]);
    }

    // Draw arrow
    display.setCursor(x + length * 6 + 2, y);
    display.print(isSelect ? (char)0x18 : (char)0x19); // Up or down arrow
}

void MyComboBox::setValue(unsigned long value) {
    current = value;
}

unsigned long MyComboBox::getValue() {
    return current;
}

void MyComboBox::setIndex(unsigned char index) {
    if (index < 20) {
        current_index = index;
    }
}

unsigned char MyComboBox::getIndex() {
    return current_index;
}

void MyComboBox::increment() {
    if (isValue) {
        current++;
    } else {
        if (current_index < 19) current_index++;
    }
}

void MyComboBox::decrement() {
    if (isValue) {
        if (current > 0) current--;
    } else {
        if (current_index > 0) current_index--;
    }
}

// ========================================================================
// SYMBOLBOX IMPLEMENTATION
// ========================================================================

MySymbolBox::MySymbolBox(int px, int py, char ptable, char psymbol) {
    x = px;
    y = py;
    table = ptable;
    symbol = psymbol;
    isSelect = false;
    onSelect = false;
}

void MySymbolBox::SelectItem(const char* symbols, int count) {
    // Selection handled externally via increment/decrement
}

void MySymbolBox::Show(Adafruit_SSD1306 &display) {
    display.setTextColor(WHITE);
    display.setCursor(x, y);

    // Draw border if selected
    if (isSelect) {
        display.drawRect(x - 2, y - 2, 14, 14, WHITE);
    }

    // Draw background
    display.fillRect(x, y, 10, 10, BLACK);

    // Draw symbol
    display.drawChar(x + 1, y + 1, symbol, WHITE, BLACK, 1);
}

void MySymbolBox::setSymbol(char psymbol) {
    symbol = psymbol;
}

char MySymbolBox::getSymbol() {
    return symbol;
}

void MySymbolBox::increment() {
    symbol++;
}

void MySymbolBox::decrement() {
    symbol--;
}

// ========================================================================
// UTILITY FUNCTIONS
// ========================================================================

void msgBox(Adafruit_SSD1306 &display, String msg) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);

    // Word wrap
    int x = 0, y = 0;
    int wordStart = 0;

    for (int i = 0; i < msg.length(); i++) {
        if (msg[i] == ' ' || i == msg.length() - 1) {
            String word = msg.substring(wordStart, i + 1);
            if (x + word.length() * 6 > SCREEN_WIDTH) {
                x = 0;
                y += 10;
                if (y > SCREEN_HEIGHT - 10) y = 0;
            }
            display.setCursor(x, y);
            display.print(word);
            x += word.length() * 6;
            wordStart = i + 1;
        }
    }

    display.display();
    delay(2000);
}

void topBar(Adafruit_SSD1306 &display, int ws) {
    display.fillRect(0, 0, SCREEN_WIDTH, 10, WHITE);
    display.setTextColor(BLACK);
    display.setCursor(2, 1);

    switch(ws) {
        case 0: display.print("MONO_GUI"); break;
        case 1: display.print("EDIT MODE"); break;
        case 2: display.print("MENU"); break;
        default: display.print("READY"); break;
    }

    display.setTextColor(WHITE);
}

void drawQrCode(Adafruit_SSD1306 &display, const char *text, int x, int y, int scale) {
    // Simplified QR code placeholder
    // For full QR code generation, integrate a QR code library
    display.drawRect(x, y, 30 * scale, 30 * scale, WHITE);
    display.setCursor(x + 5, y + 12 * scale);
    display.print("QR");
    display.setCursor(x + 5, y + 22 * scale);
    display.print("CODE");
}
