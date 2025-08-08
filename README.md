# BRAIN

## Task Scheduler

- Executes tasks periodically
- Stores tasks in nodes (linked list)
- Uses dynamic memory allocation

## Task Types

- **Basic Task:** Runs a fixed number of times, then is deleted.
- **Endless Task:** Runs indefinitely.
- **Scheduled Task:** Runs at specific intervals for a fixed number of times.
- **Scheduled Endless Task:** Runs at specific intervals indefinitely.

## Usage

### 1. Include Header Files

~~~c
#include <sys.h>    // Includes 'System' structure and other headers
#include <funcs.h>  // Optional: includes utility functions
~~~

### 2. Define a Task Function

~~~c
// Example function for a task node
// Must be void and take a void* argument
void foo(void *arg)
{
    int *p = (int *)arg;
    int i = *p;

    Serial.println(i);

    i = i * 2;
    *p = i;
}
~~~

### 3. System Setup

~~~c
void setup()
{
    // System configuration
    Sys_setup();
    // ... (add tasks here)
}
~~~

### 4. Creating and Adding a Scheduled Task

~~~c
// Create a pointer to use in the function
int *pointer = (int *)malloc(sizeof(int)); 
*pointer = 10;

// Create a Task_arg with the pointer and a 1000 ms period
Task_arg *task_arg = Task_arg_create(pointer, 1000);

// Create a scheduled task that runs 10 times
Task task = Task_create(foo, task_arg, 10, Scheduled_Task);

// Add the task to the system's task node list
Task_node_add(&System.tasks, task);
~~~

### 5. Running Tasks in the Main Loop

~~~c
void loop()
{
    Task_node_run(&System.tasks); // Run all tasks
}
~~~
