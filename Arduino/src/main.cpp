#include <Arduino.h>

#include "../include/programs/manager/task_manager.hpp"


TaskManager task_manager;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("AI.EcoSense");
    Serial.println("Starting...");

    pinMode(LED_BUILTIN, OUTPUT);

    analogReadResolution(12);

    task_manager.begin();
}

void loop() {
    task_manager.manage();
}