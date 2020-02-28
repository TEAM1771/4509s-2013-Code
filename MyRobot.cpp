#include "WPILib.h"
#include "Dashboard.h"
/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	Joystick lStick;
	Joystick rStick;
	Joystick manStick;
	Jaguar leftJag1;
	Jaguar leftJag2;
	Jaguar rightJag1;
	Jaguar rightJag2;
	Jaguar shooterJag1;
	Jaguar shooterJag2;
	Jaguar uppieDownieJag;
	Jaguar leftyRightyJag;
	Compressor theBlower;
	Solenoid shifter;
	Solenoid pullerUpperDude1;
	Solenoid pullerUpperDude2;
	Solenoid ballcrusher; //shooter cilinder (sorry bad at spelling)
	Counter hurts;// encoder 
	DriverStationLCD *p_dsLCD;
	bool virgin;//to test if it is the first time in the autonomous period
	double stuff;
	int v;
	double pert;
	double rpm;
public:
	RobotDemo(void):
		lStick(1), 
		rStick(2), 
		manStick(3),
		leftJag1(1), 
		leftJag2(2), 
		rightJag1(3), 
		rightJag2(4),
		shooterJag1(5),
		shooterJag2(6),
		uppieDownieJag(7),
		leftyRightyJag(8),
		theBlower(1,1), 
		shifter(3),
		pullerUpperDude1(1),
		pullerUpperDude2(2),
		ballcrusher(4),
		hurts(5)
{
		p_dsLCD = DriverStationLCD::GetInstance();
		pert=0;
		v=3250;
		hurts.Start();
		//hurts.SetUpSource(5);
		theBlower.Start();
		//pullerUpperDude1.Set(false);
		pullerUpperDude2.Set(true);
		virgin = true;
		ballcrusher.Set(false);
		shifter.Set(true); //Start in low gear
}
	void Autonomous(void)
	{
		while (IsAutonomous())
		{
			//autoshooting a number of frisbees can change depending if we want to start with a different number of frisbees
			if (virgin == true)
			{	
				shooterJag1.Set(-.65);
				shooterJag2.Set(-.65);
				rpm = hurts.GetPeriod();
				rpm = 1/(rpm+1);//so not dividing by 0
				rpm = rpm / 30;
				rpm = rpm*60;
				
				Wait(2);
			for( int frisbee = 4; frisbee > 0; frisbee -- )
			{
				shooterJag1.Set(-.65);
				shooterJag2.Set(-.65);
				
				ballcrusher.Set(true);
				Wait(.1);
				ballcrusher.Set(false);
				Wait(2);
			}
				virgin = false;
			}
			
		}
		
	}

	/********************************************************
	 *  Runs the motors with tank steering, and other stuff *
	 ********************************************************/
	void OperatorControl(void)
	{
		//myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			
			//Driver control of pneumatics
			if(manStick.GetRawButton(4)) //rise
			{
				pullerUpperDude1.Set(true);
			}
			else
			{
				pullerUpperDude1.Set(false);
			}
			
			if(manStick.GetRawButton(6))
			{
				pullerUpperDude2.Set(true);
			}
			else
			{
				pullerUpperDude2.Set(false);
			}
			//Manual Shooting

			/*
			for(int w=0;w<10;w++)
			{
				stuff = 0;
				stuff = hurts.GetPeriod();
				pert = pert+stuff;
				Wait(.1);
			}
			*/
			//rpm = pert/50000;//3250(desired result)
			rpm = hurts.GetPeriod();
			if(rpm==0)
			{
				rpm=1;
			}
			rpm = 1/(rpm);
			rpm = rpm / 30;
			rpm = rpm*60;
			p_dsLCD->Printf(DriverStationLCD::kUser_Line6, 1,"RPM: %f \n",rpm );
			p_dsLCD->UpdateLCD();

			
		
			if (rpm-v>500)
			{
				shooterJag1.Set(-0);
				shooterJag2.Set(-0);
			}
			else if(rpm-v<500)
			{
				shooterJag1.Set(-1);
				shooterJag2.Set(-1);
			}
			else
			{
				shooterJag1.Set(-.65);
				shooterJag2.Set(-.65);
			}
				
	/*shooterJag1.Set(-.65);
	  shooterJag2.Set(-.65)*/
			if(manStick.GetRawButton(1))
			{
				ballcrusher.Set(1);
			}
			 else if(manStick.GetRawButton(2))
			  {
				  shooterJag1.Set(-.5);
				  shooterJag2.Set(-.5); 
			  }
			 else
			  {
			  ballcrusher.Set(false);
			  }
			
		    //shift the shifters
			if(rStick.GetRawButton(11) || lStick.GetRawButton(6))
			{
				shifter.Set(false);
			}
			else
			{
				shifter.Set(true);
			}
			//Enables or Disables the robot from the joystick
			if (manStick.GetRawButton(11))
			{
				theBlower.Stop();
			}
			
			if (manStick.GetRawButton(12))
			{
				theBlower.Start();
			}
			leftJag1.Set(lStick.GetY());
			leftJag2.Set(lStick.GetY());
			rightJag1.Set(-rStick.GetY());
			rightJag2.Set(-rStick.GetY());
			//myRobot.TankDrive(lstick,rstick); // drive with tank style (use both sticks(one for each side))
			Wait(0.005);				// wait for a motor update time
		}
	}
};

	/* Runs during test mode (still havent found a reason for test mode)
	 */

START_ROBOT_CLASS(RobotDemo);
