#pragma once
#include "./i2c_controller.h"
#include <thread>
#include <array>
#include <memory>

namespace PCA9685{

constexpr uint8_t MODE1_REGISTER = 0x00;
constexpr uint8_t MODE2_REGISTER = 0x01;

class PCA9685_Controller : public I2CController
{
private:
	std::array<uint8_t,2> temporary_2byte;
	std::array<uint8_t,8> pwm_tx_data;
	uint8_t& on_lo_register;
	uint8_t& on_lo_value;
	uint8_t& on_hi_register;
	uint8_t& on_hi_value;
	uint8_t& off_lo_register;
	uint8_t& off_lo_value;
	uint8_t& off_hi_register;
	uint8_t& off_hi_value;
	void set_register_to_send(const uint8_t& _channel);
	void set_duty_in_hex(const double& _duty_ratio);
	void write_pwm_data();
public:
	PCA9685_Controller(const uint8_t& _bus_num, const uint8_t& _slave_address);
	void sleep();
	void start();
	void reset(bool _start);
	void set_PWM_duty_of_channel(const double& _duty_ratio, const uint8_t& _channel);
};
} /*namespace PCA9685*/