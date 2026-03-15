/**
 * ComboBox Widget
 * Dropdown/Selection widget for monochrome OLED displays
 */

#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class MyComboBox {
public:
    int x, y, length;
    char item[20][20];
    unsigned long current;
    unsigned char current_index;
    bool isValue;
    bool isSelect;

    /**
     * Constructor
     * @param px X position
     * @param py Y position
     * @param plength Field length
     * @param pisvalue Value mode (true) or item mode (false)
     */
    MyComboBox(int px = 0, int py = 0, int plength = 10, bool pisvalue = true);

    /**
     * Select from value range
     * @param val_min Minimum value
     * @param val_max Maximum value
     * @param step Step increment
     */
    void SelectValue(long val_min, long val_max, long step);

    /**
     * Select from item list
     * @param items Array of items
     * @param count Number of items
     */
    void SelectItem(const char* items[], int count);

    /**
     * Display the combobox on screen
     * @param display Reference to OLED display object
     */
    void Show(Adafruit_SSD1306 &display);

    /**
     * Set current value
     * @param value Value to set
     */
    void setValue(unsigned long value);

    /**
     * Get current value
     * @return Current value
     */
    unsigned long getValue();

    /**
     * Set current index
     * @param index Index to set
     */
    void setIndex(unsigned char index);

    /**
     * Get current index
     * @return Current index
     */
    unsigned char getIndex();

    /**
     * Increment selection
     */
    void increment();

    /**
     * Decrement selection
     */
    void decrement();
};

#endif // COMBOBOX_H
