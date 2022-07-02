#include "./pca9685.h"

using namespace PCA9685;

PCA9685_Controller::PCA9685_Controller(const uint8_t& _bus_num, const uint8_t& _slave_address)
	:I2CController(_bus_num),
	on_lo_register(pwm_tx_data[0]), on_lo_value(pwm_tx_data[1]), 
	on_hi_register(pwm_tx_data[2]), on_hi_value(pwm_tx_data[3]), 
	off_lo_register(pwm_tx_data[4]), off_lo_value(pwm_tx_data[5]), 
	off_hi_register(pwm_tx_data[6]), off_hi_value(pwm_tx_data[7])
{
	set_slave_address(_slave_address);
}

void PCA9685_Controller::sleep()
{
	wait_for_being_idle();
	/*Bit4 is 1 to make it sleep. But other bit has no reason so it may make side effect.*/
	std::array<uint8_t, 2> tx_data({ MODE1_REGISTER , 0x10});
	write_bytes(tx_data);
}
void PCA9685_Controller::wake_up()
{
	wait_for_being_idle();
	/*Bit4 is 0 to make it wake up. But other bit has no reason so it may make side effect.*/
	std::array<uint8_t, 2> tx_data({ MODE1_REGISTER , 0x00});
	write_bytes(tx_data);
}

void PCA9685_Controller::start()
{
	temporary_2byte[0] = MODE1_REGISTER; /*Mode 1 */
	temporary_2byte[1] = 0x01;
	write_bytes(temporary_2byte);
	temporary_2byte[0] = MODE2_REGISTER; /*Mode 2*/
	temporary_2byte[1] = 0x04; /*totem pole*/
	write_bytes(temporary_2byte);
}

void PCA9685_Controller::reset(bool _start)
{
	sleep();
	std::this_thread::sleep_for(std::chrono::microseconds(1));
	temporary_2byte[0] = MODE1_REGISTER; /*Mode 1 */
	temporary_2byte[1] = 0x80;
	write_bytes(temporary_2byte);
	if (_start == true)
	{
		start();
	}
}

void PCA9685_Controller::set_PWM_duty_of_channel(const double& _duty_ratio, const uint8_t& _channel)
{
	set_register_to_send(_channel);
	set_duty_in_hex(_duty_ratio);
	write_pwm_data();
}
void PCA9685_Controller::set_register_to_send(const uint8_t& _channel)
{
	on_lo_register = _channel * 4 + 6;
	on_hi_register = on_lo_register + 1;
	off_lo_register = on_hi_register + 1;
	off_hi_register = off_lo_register + 1;
}
void PCA9685_Controller::set_duty_in_hex(const double& _duty_ratio)
{
	if (_duty_ratio <= 0)
	{
		on_lo_value = 0x00;
		on_hi_value = 0x10;
		off_lo_value = 0x00;
		off_hi_value = 0x00;
	}
	else if (_duty_ratio >= 100)
	{
		on_lo_value = 0x00;
		on_hi_value = 0x00;
		off_lo_value = 0x00;
		off_hi_value = 0x10;
	}
	else
	{
		uint16_t prescaled_duty = static_cast<uint16_t> (static_cast<uint64_t>(_duty_ratio * 4096) / 100);
		on_lo_value = 0x00;
		on_hi_value = 0x00;
		off_lo_value = static_cast<uint8_t>(prescaled_duty % 256);
		off_hi_value = static_cast<uint8_t>(prescaled_duty / 256);
	}
}
void PCA9685_Controller::write_pwm_data()
{
	for(size_t i=0; i < 8; i += 2)
	{
		write_bytes(&(pwm_tx_data[i]),2);
	}
}

void PCA9685_Controller::change_freq(unsigned int _freq_in_Hz)
{
	constexpr unsigned int osc_clock = 25000000;
	double prescale_value = std::round(osc_clock / 4096 / _freq_in_Hz) - 1;
	if(prescale_value < 3)
	{
		std::cout<<"Failed to change frequency: Too high frequency\n";
		return;
	}
	if(prescale_value > 255)
	{
		std::cout<<"Failed to change frequency: Too low frequency\n";
		return;
	}
	sleep();
	std::this_thread::sleep_for(std::chrono::microseconds(1));
	temporary_2byte[0] = PRESCALE_REGISTER;
	temporary_2byte[1] = static_cast<uint8_t>(static_cast<unsigned int>(prescale_value));
	std::cout<<"Set val: "<<static_cast<int>(temporary_2byte[1])<<std::endl;
	write_bytes(temporary_2byte);
	wake_up();
}