#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30100_PulseOximeter.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected() {
    Serial.println("Beat!");
}

void setup() {
    Serial.begin(115200);
    
    // Initialize OLED
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("OLED failed"); for(;;); 
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Initialize MAX30100
    Serial.print("Initializing MAX30100...");
    if (!pox.begin()) {
        Serial.println("FAILED");
        display.setCursor(0,0);
        display.print("Sensor Failed!");
        display.display();
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // Optional: Lower LED current to save power or fix communication issues
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
    pox.update(); // Must be called constantly for accurate readings

    // Update display every 1 second
    if (millis() - tsLastReport > 1000) {
        display.clearDisplay();
        
        display.setCursor(0,0);
        display.println("BPM: " + String(pox.getHeartRate()));
        
        display.setCursor(0,20);
        display.println("SpO2: " + String(pox.getSpO2()) + "%");

        display.display();
        tsLastReport = millis();
    }
}
