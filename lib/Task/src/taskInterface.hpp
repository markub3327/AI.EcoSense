#pragma once

enum TaskState {
  RUNNING = 0,
  READY   = 1,
  WAITING = 2,
};

class TaskInterface {
    // state of the task
    TaskState state = READY;

protected:
    virtual void update() = 0;

    void setState(const TaskState state) { this->state = state; }

public:
    virtual ~TaskInterface() = default;

    virtual void begin() = 0;

    virtual void run() = 0;

    TaskState getState() const { return this->state; }
};