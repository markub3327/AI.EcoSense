#pragma once

#include "../lib/Task/src/taskInterface.hpp"
#include "bluetooth.hpp"
#include "monitoring.hpp"
#include "pump.hpp"
#include "power_manager.hpp"

class TaskManager {

public:
    Bluetooth task_1; // every 1 s
    Monitoring task_2; // every 1 min
    Pump task_3;   // every 1 min
    PowerManager task_4;   // every 1 min

    TaskInterface* tasks[4] = { &task_1, &task_2, &task_3, &task_4 };

    TaskManager() : task_1(1000), task_2(60000), task_3(60000), task_4(60000) {}

    void begin() {
        task_2.addCharacteristic(task_1.getService());
        task_3.addCharacteristic(task_1.getService());
        task_4.addCharacteristic(task_1.getService());

        for (const auto & it : this->tasks) {
          it->begin();
        }
    }

    void manage() {
        for (const auto & it : this->tasks) {
          it->run();
        }
    }
};