#include "main.h"
#include "pros/misc.h"

#define LEFT_FRONT_MOTOR_PORT 1
#define LEFT_BACK_MOTOR_PORT -2
#define RIGHT_FRONT_MOTOR_PORT 4
#define RIGHT_BACK_MOTOR_PORT -3
#define FRONT_INTAKE_PORT 6
#define MIDDLE_INTAKE_PORT 11


// Create motor objects



pros::Motor leftFrontMotor(LEFT_FRONT_MOTOR_PORT);
pros::Motor leftBackMotor(LEFT_BACK_MOTOR_PORT );
pros::Motor rightFrontMotor(RIGHT_FRONT_MOTOR_PORT);
pros::Motor rightBackMotor(RIGHT_BACK_MOTOR_PORT);

pros::Motor intake1B(FRONT_INTAKE_PORT);
pros::Motor intake2U(MIDDLE_INTAKE_PORT);





/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {


}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	


	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs


		//	intake code
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {

			pros::lcd::set_text(2, "pressed");

			intake1B.move(-60);
			intake2U.move(-60);
			
			} 

		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			pros::lcd::set_text(3, "pressed!!");
			intake1B.move_voltage(127);
			intake2U.move_voltage(127);
			}

		else {
			intake1B.move_voltage(0);
			intake2U.move_voltage(0);
			}


		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);
		int leftV = dir + turn; 
		int rightV = dir - turn;

//dead zone to prevent spinning in place when no joystick is pressed
		if (abs(rightV)<= 10){ 
		rightV = 0;
	}
		else if (abs(leftV) <=10){
		leftV = 0;
	}
		   
	leftFrontMotor.move_velocity(-leftV);
    leftBackMotor.move_velocity(leftV);
    rightFrontMotor.move_velocity(rightV);
    rightBackMotor.move_velocity(-rightV);

    

                pros::delay(10);                               // Run for 10 ms then update
	}
}