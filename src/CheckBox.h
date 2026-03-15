/**
 * CheckBox Widget
 * Checkbox widget for monochrome OLED displays
 */

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class MyCheckBox {
public:
    int x, y;
    char text[20];
    bool Checked;
    bool isSelect;

    /**
     * Constructor
     * @param px X position
     * @param py Y position
     * @param ptext Label text
     * @param pchecked Initial checked state
     */
    MyCheckBox(int px = 0, int py = 0, const char *ptext = "", bool pchecked = false);

    /**
     * Toggle checkbox state
     */
    void Toggle();

    /**
     * Display the checkbox on screen
     * @param display Reference to OLED display object
     */
    void CheckBoxShow(Adafruit_SSD1306 &display);

    /**
     * Set checked state
     * @param state New state
     */
    void setChecked(bool state);

    /**
     * Get checked state
     * @return Current state
     */
    bool getState();
};

#endif // CHECKBOX_H
