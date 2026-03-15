/**
 * ButtonBox Widget
 * Button widget for monochrome OLED displays
 */

#ifndef BUTTONBOX_H
#define BUTTONBOX_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

class MyButtonBox {
public:
    int x, y;
    char text[20];
    bool Checked;
    bool Border;
    bool isSelect;

    /**
     * Constructor
     * @param px X position
     * @param py Y position
     * @param ptext Button text
     * @param pborder Show border
     */
    MyButtonBox(int px = 0, int py = 0, const char *ptext = "", bool pborder = true);

    /**
     * Toggle button state (pressed/released)
     */
    void Toggle();

    /**
     * Display the button on screen
     * @param display Reference to OLED display object
     */
    void Show(Adafruit_GFX &display);

    /**
     * Set button state
     * @param state New state
     */
    void setState(bool state);

    /**
     * Get button state
     * @return Current state
     */
    bool getState();

    /**
     * Check if button is clicked
     * @return true if button is pressed
     */
    bool isClicked();
};

#endif // BUTTONBOX_H
