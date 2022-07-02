#include <iostream>
#include "./i2c_controller.h"
#include "./pca9685.h"
#include <thread>

int main()
{
    PCA9685::PCA9685_Controller a_pca9685_controller(1,0x40);
    a_pca9685_controller.start();
    a_pca9685_controller.change_freq(50);
    a_pca9685_controller.set_PWM_duty_of_channel(0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    #if 1
    for(size_t i=0; i<10; ++i)
    {
        a_pca9685_controller.set_PWM_duty_of_channel(7.5+2.5*(i%2), 0);
        a_pca9685_controller.set_PWM_duty_of_channel(i*10+25, 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    #endif
    a_pca9685_controller.set_PWM_duty_of_channel(00, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    a_pca9685_controller.reset(true);
  
    std::cout<<"Hello, world!"<<std::endl;
    return 0;
}
