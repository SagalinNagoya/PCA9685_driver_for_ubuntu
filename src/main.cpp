#include <iostream>
#include "./i2c_controller.h"
#include "./pca9685.h"
#include <thread>

int main()
{
    PCA9685::PCA9685_Controller a_pca9685_controller(1,0x40);
    a_pca9685_controller.start();


    for(size_t i=0; i<30; ++i)
    {
        a_pca9685_controller.set_PWM_duty_of_channel(30, 0);
        a_pca9685_controller.set_PWM_duty_of_channel(70, 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        a_pca9685_controller.set_PWM_duty_of_channel(70, 0);
        a_pca9685_controller.set_PWM_duty_of_channel(30, 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    a_pca9685_controller.set_PWM_duty_of_channel(0, 0);
    a_pca9685_controller.set_PWM_duty_of_channel(0, 2);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    //a_pca9685_controller.reset(true);
  
    std::cout<<"Hello, world!"<<std::endl;
    return 0;
}