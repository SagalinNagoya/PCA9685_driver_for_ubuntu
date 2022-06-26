# PCA9685 driver for ubuntu

# 1. Abstract
This is single PCA9685 driver which is no need of external dependencies(such as wiringPi, and adafruit library).
I designed internal I2C driver using ubuntu and the PCA9685 is derived from it.
This program is free-program with no WARRANTY.

# 2. Why C++?
I wanted to use it with ROS nodes. ROS node could be written in both of python and C++.
Also, I wanted fast response so I discarded using python.

# 3. Tested system
This program is tested by main.cpp which calls function of the program.
Tested environment: Ubuntu 22 LTS in raspberry Pi 4
I programmed reset and sleep method but they are not tested.

# 4. How to use
## 3.1 Build
Add dependencies of ./src/i2c_controller.cpp and ./src/pca9685.cpp on your CMakeList.txt
## 3.2 Initialization
```cpp
#include "./pca9685.h"
int main()
{
  PCA9685::PCA9685_Controller a_pca9685_controller(1,0x40); /*Create controller object*/
  a_pca9685_controller.start(); /*Start in totem pole mode*/
}
```

The first parameter of a_pca9685_controller() is index of the I2C bus of the system(such as "/dev/i2c-1"). And the second argument is slave address of the I2C driver.
Before you call start() method, the driver may not give any response.(Caution: But the bus is occupied!)

## 3.3 Set duty ratio of channels
```cpp
a_pca9685_controller.set_PWM_duty_of_channel(30, 2);
```
To set duty ratio of arbitrary channel, you may call set_PWM_duty_of_channel()method. The first argument in duty ratio in percent(double 0~100). And the second argument is the channel to set duty ratio. If you set duty ratio equal to or less than 0, it fixes it to be always off(Duty 0%). Also, if you set duty ratio over 100, it fixes duty ratio to 100%.
