#pragma once

#include "../lib/Task/src/taskInterface.hpp"
#include "../bluetooth.hpp"
#include "../monitoring.hpp"
#include "../control/pump.hpp"
#include "../../services/datetime.hpp"
#include "power_manager.hpp"

class TaskManager {

public:
    Bluetooth task_1; // every 1 s
    Monitoring task_2; // every 1 min
    Pump task_3;   // every 1 min
    PowerManager task_4;   // every 1 min

    DateTime dateTime;

    TaskInterface* tasks[4] = { &task_1, &task_2, &task_3, &task_4 };

    TaskManager() : task_1(1000), task_2(60000), task_3(dateTime, 60000), task_4(60000), dateTime("19B11701-F8F2-537E-4F6C-D104768A1215") {}

    void begin() {
        task_2.addCharacteristic(task_1.getService());
        task_3.addCharacteristic(task_1.getService());
        task_4.addCharacteristic(task_1.getService());

        dateTime.addCharacteristic(task_1.getService());
        dateTime.begin();

        for (const auto & it : this->tasks) {
          it->begin();
        }
    }

    void manage() {
        for (const auto & it : this->tasks) {
          it->run();
        }

        dateTime.update();
    }
};