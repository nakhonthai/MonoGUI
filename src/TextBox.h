/**
 * TextBox Widget
 * Text input field for monochrome OLED displays
 */

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

class MyTextBox {
public:
    int x, y, length;
    char text[50];
    int type; // 0=all, 1=numeric, 2=uppercase
    bool isSelect;

    /**
     * Constructor
     * @param px X position
     * @param py Y position
     * @param plength Field length (in characters)
     * @param ptype Input type: 0=all, 1=numeric, 2=uppercase
     */
    MyTextBox(int px = 0, int py = 0, int plength = 10, int ptype = 0);

    /**
     * Initialize the textbox with default text
     * @param defaultText Default text to display
     */
    void TextBox(const char *defaultText = "");

    /**
     * Display the textbox on screen
     * @param display Reference to OLED display object
     */
    void TextBoxShow(Adafruit_GFX &display);

    /**
     * Set text value
     * @param newText Text to set
     */
    void setText(const char *newText);

    /**
     * Get text value
     * @return Current text
     */
    const char* getText();

    /**
     * Add character to text
     * @param c Character to add
     */
    void addChar(char c);

    /**
     * Remove last character
     */
    void backspace();

    /**
     * Clear text
     */
    void clear();
};

#endif // TEXTBOX_H
