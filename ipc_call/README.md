# Inter-Processor Call: ``ipc_call``

Example of `ipc_call` use case. By using the multicore capability of ESP32, the beginning 15 seconds of Protocol CPU and Application CPU will print `Assalamualaikum World!` on the serial monitor. After that Application CPU starts blinking the built-in LED of ESP32 and stops sending the message as mentioned earlier. There are two FreeRTOS tasks: sending the message in the Protocol CPU and blinking the LED in the Application CPU. The `ipc_call` in Application CPU performs the sending of `Assalamualaikum World!` message. This case requires [Semaphores](https://www.freertos.org/Embedded-RTOS-Binary-Semaphores.html).


## Wiring
- LED built in --> GPIO 2

## Build and Flash
You can build and flash ESP-IDF projects using `idf.py`. Go to the project directory and run:
```
source \path\to\esp-idf\export.sh
idf.py build clean
idf.py -p (PORT) flash # check PORT using ls /dev
```

To open the serial monitor in your terminal, type this command :
```
make monitor
```

To close the serial monitor in your terminal:

> CTRL + T + X

## Note

Please edit some lines in `sdkconfig` file into these.

```
.
.
.
# CONFIG_ESPTOOLPY_FLASHSIZE_1MB is not set
# CONFIG_ESPTOOLPY_FLASHSIZE_2MB is not set
CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y
# CONFIG_ESPTOOLPY_FLASHSIZE_8MB is not set
# CONFIG_ESPTOOLPY_FLASHSIZE_16MB is not set
# CONFIG_ESPTOOLPY_FLASHSIZE_32MB is not set
# CONFIG_ESPTOOLPY_FLASHSIZE_64MB is not set
# CONFIG_ESPTOOLPY_FLASHSIZE_128MB is not set
CONFIG_ESPTOOLPY_FLASHSIZE="4MB"
.
.
.
CONFIG_ESP_IPC_TASK_STACK_SIZE=2048
CONFIG_ESP_IPC_USES_CALLERS_PRIORITY=y
CONFIG_ESP_IPC_ISR_ENABLE=y
.
.
.
```
