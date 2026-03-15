/**
 * SymbolBox Widget
 * Symbol selection widget for monochrome OLED displays
 */

#ifndef SYMBOLBOX_H
#define SYMBOLBOX_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class MySymbolBox {
public:
    int x, y;
    char table;
    char symbol;
    bool isSelect;
    bool onSelect;

    /**
     * Constructor
     * @param px X position
     * @param py Y position
     * @param ptable Symbol table
     * @param psymbol Initial symbol
     */
    MySymbolBox(int px = 0, int py = 0, char ptable = '/', char psymbol = '-');

    /**
     * Select symbol from table
     * @param symbols Array of symbols
     * @param count Number of symbols
     */
    void SelectItem(const char* symbols, int count);

    /**
     * Display the symbolbox on screen
     * @param display Reference to OLED display object
     */
    void Show(Adafruit_SSD1306 &display);

    /**
     * Set symbol
     * @param psymbol Symbol to set
     */
    void setSymbol(char psymbol);

    /**
     * Get symbol
     * @return Current symbol
     */
    char getSymbol();

    /**
     * Increment symbol
     */
    void increment();

    /**
     * Decrement symbol
     */
    void decrement();
};

#endif // SYMBOLBOX_H
