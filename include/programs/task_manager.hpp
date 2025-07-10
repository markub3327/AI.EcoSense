#pragma once

#include "../lib/Task/src/taskInterface.hpp"
#include "bluetooth.hpp"
#include "monitoring.hpp"
#include "pump.hpp"

class TaskManager {

public:
    Bluetooth task_1; // every 1 s
    Monitoring task_2; // every 1 min
    Pump task_3;   // every 1 min

    TaskInterface* tasks[3] = { &task_1, &task_2, &task_3 };

    TaskManager() : task_1(1000), task_2(60000), task_3(60000) {}

    void begin() {
        task_3.setService(task_2.getService());

        for (const auto & it : this->tasks) {
          it->begin();
        }

        task_1.addService(task_2.getService());
    }

    void manage() {
        for (const auto & it : this->tasks) {
          it->run();
        }
    }
};