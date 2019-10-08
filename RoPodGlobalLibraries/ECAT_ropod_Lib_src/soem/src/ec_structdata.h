/* module input/output structure config for SOEM Master 
// (c) 2008 R.Solberg IME Technologies
*/
/*EL1xxx------------------------------*/
typedef struct PACKED
{
	uint8	   inbits;
} in_EL1008t;	

typedef struct PACKED
{
	uint8	   inbits;
} in_EL1014t;	

typedef struct PACKED
{
	uint8	   inbits;
} in_EL1018t;	


/*EL2xxx------------------------------*/
typedef struct PACKED
{
	uint8	   outbits;
} out_EL2004t;	

typedef struct PACKED
{
	uint8	   outbits;
} out_EL2008t;	

typedef struct PACKED
{
    uint16      pwm1;
    uint16      pwm2;
} out_EL2502t;


/*EL3xxx------------------------------*/
typedef struct PACKED
{
	int16	   status1;
	int16	   invalue1;
	int16	   status2;
	int16	   invalue2;
	int16	   status3;
	int16	   invalue3;
	int16	   status4;
	int16	   invalue4;
	int16	   status5;
	int16	   invalue5;
	int16	   status6;
	int16	   invalue6;
	int16	   status7;
	int16	   invalue7;
	int16	   status8;
	int16	   invalue8;

} in_EL3008t;

typedef struct PACKED
{
	uint8	   status1;
	int16	   invalue1;
	uint8	   status2;
	int16	   invalue2;
} in_EL3102t;	


/*EL4xxx------------------------------*/
typedef struct PACKED
{
	int16	   outvalue1;
	int16	   outvalue2;
} out_EL4132t;

typedef struct PACKED
{
	int16	   outvalue1;
	int16	   outvalue2;
    int16	   outvalue3;
    int16	   outvalue4;
    int16	   outvalue5;
    int16	   outvalue6;
    int16	   outvalue7;
    int16	   outvalue8;
} out_EL4038t;

/*EL5xxx------------------------------*/
typedef struct PACKED
{
	uint8	   control;
	int16	   outvalue;
} out_EL5101t;	

typedef struct PACKED
{
	uint8	   status;
	int16	   invalue;
	int16	   latch;
} in_EL5101t;

typedef struct PACKED
{
	uint16	   control0;
	int32	   outvalue0;
	uint16	   control1;
	int32	   outvalue1;
} out_EL5152t;

typedef struct PACKED
{
	uint16	   status0;
	int32	   invalue0;
	int32	   period0;
	uint16	   status1;
	int32	   invalue1;
	int32	   period1;
} in_EL5152t;

/*TU0001 -- intelligent slave for Robocup, contains AD, SerialCAM interface, shootercontrol, acceleration */
typedef struct PACKED
{
	uint16	   CAM1control;
	uint32	   CAM1data;
	uint8	   SHTcontrol;	
	uint8	   SHTdutycycle;
	uint8	   SHTpulselength;
	uint8	   SHTencodermax;
} out_TU0001t;	

typedef struct PACKED
{ 
	uint8         status;
	uint8         counter;
	uint8         diginput;
	int16         analog[8];
	uint16        CAM1status;
	int16         CAM1posX;        
	int16         CAM1posY;        
	int16         CAM1posZ;        
	int16         CAM1velX;        
	int16         CAM1velY;        
	int16         CAM1velZ;        
	uint32        CAM1timestamp;
	uint8         SHTstatus;
	int16         SHTvalue;
        int16         AccelerationX;
        int16         AccelerationY;
        int16         AccelerationZ;
} in_TU0001t;	

/*TU_ES_030 -- intelligent slave for SERGIO Arm */
typedef struct PACKED
{
	uint8       port;
	int16       m_1;
    int16       m_2;
    int16       m_3;
    int16       aout_1;
    int16       aout_2;
} out_TU_ES_030t;	

typedef struct PACKED
{ 
    uint8       port;
    uint16      encoder_1;
    uint16      encoder_2;
    uint16      encoder_3;
    int16       current_1;
    int16       current_2;
    int16       current_3;
    uint16      caliper_1;
    uint16      caliper_2; 
    uint16      force_1;
    uint16      force_2;
    uint16      force_3;
    uint16      pos_1;
    uint16      pos_2;
    uint16      pos_3;
    uint16      spare_ai_1;
    uint16      spare_ai_2;
    uint16      time_stamp;            
} in_TU_ES_030t;

/*TU_ES_030V2 -- intelligent slave for SERGIO Arm, update Ketels */
typedef struct PACKED
{
	uint8       mcom1;
	int16       setpoint1;
    int16       ff1;
	uint8       mcom2;
	int16       setpoint2;
    int16       ff2;
    uint8       mcom3;
	int16       setpoint3;
    int16       ff3;
    uint8       digital;
    int16       aout1;
    int16       aout2;
} out_TU_ES_030v2t;	

typedef struct PACKED
{ 
    uint8       mstate1;
    uint32      count1;
    uint32      timestamp1;
    int16       velocity1;
    int16       current1;
    uint8       mstate2;
    uint32      count2;
    uint32      timestamp2;
    int16       velocity2;
    int16       current2;
    uint8       mstate3;
    uint32      count3;
    uint32      timestamp3;
    int16       velocity3;
    int16       current3;
    uint8       digital;
    uint16      caliper1;
    uint16      caliper2;
    uint16      force1;
    uint16      force2;
    uint16      force3;
    uint16      pos1;
    uint16      pos2;
    uint16      pos3;
    uint16      analog1;
    uint16      analog2;
    uint16      linevoltage;
    uint16      ectime;         
} in_TU_ES_030v2t;	

/*TU_EightWheelBot -- intelligent slave for TU 8-wheel-robot, Piggyback FB1111-0142 based */
typedef struct PACKED
{
	uint8       Command;        // Unused

    uint8       M1_Drivemode;   // 0x00 = Velocity control, 0x01 = current only control
	uint8       M1_Command;     // Bitmap, see Mx command
	int16       M1_Velocity_SP; // Velocity setpoint in 0.01 Rad/s units
	int16       M1_Current_SP;  // Current limit setpoint in 0.01A units
    
    uint8       M2_Drivemode;   // 0x00 = Velocity control, 0x01 = current only control
	uint8       M2_Command;     // Bitmap, see Mx command
	int16       M2_Velocity_SP; // Velocity setpoint in 0.01 Rad/s units
	int16       M2_Current_SP;  // Current limit setpoint in 0.01A units
    
    uint8       M3_Drivemode;   // 0x00 = Velocity control, 0x01 = current only control
	uint8       M3_Command;     // Bitmap, see Mx command
	int16       M3_Velocity_SP; // Velocity setpoint in 0.01 Rad/s units
	int16       M3_Current_SP;  // Current limit setpoint in 0.01A units
    
    uint8       M4_Drivemode;   // 0x00 = Velocity control, 0x01 = current only control
	uint8       M4_Command;     // Bitmap, see Mx command
	int16       M4_Velocity_SP; // Velocity setpoint in 0.01 Rad/s units
	int16       M4_Current_SP;  // Current limit setpoint in 0.01A units
    
    uint8       M5_Drivemode;   // 0x00 = Velocity control, 0x01 = current only control
	uint8       M5_Command;     // Bitmap, see Mx command
	int16       M5_Velocity_SP; // Velocity setpoint in 0.01 Rad/s units
	int16       M5_Current_SP;  // Current limit setpoint in 0.01A units
    
    uint8       M6_Drivemode;   // 0x00 = Velocity control, 0x01 = current only control
	uint8       M6_Command;     // Bitmap, see Mx command
	int16       M6_Velocity_SP; // Velocity setpoint in 0.01 Rad/s units
	int16       M6_Current_SP;  // Current limit setpoint in 0.01A units
    
    uint8       M7_Drivemode;   // 0x00 = Velocity control, 0x01 = current only control
	uint8       M7_Command;     // Bitmap, see Mx command
	int16       M7_Velocity_SP; // Velocity setpoint in 0.01 Rad/s units
	int16       M7_Current_SP;  // Current limit setpoint in 0.01A units
    
    uint8       M8_Drivemode;   // 0x00 = Velocity control, 0x01 = current only control
	uint8       M8_Command;     // Bitmap, see Mx command
	int16       M8_Velocity_SP; // Velocity setpoint in 0.01 Rad/s units
	int16       M8_Current_SP;  // Current limit setpoint in 0.01A units
} out_TU_EightWheelBot_t;	

typedef struct PACKED
{ 
    uint8       State;          // Cyclic counter, increments with every internal slave cycle

    uint8       M1_Update;     	// Cyclic counter, increments after each packet exchange with motor controller
    uint8       M1_Status;      // Bitmap, see Mx status
    int16       M1_Current;    	// Motor phase current in 0.01A units
    int16       M1_Velocity;   	// Motor speed in 0.01 Rad/s units
    uint16      M1_Position;   	// Absolute motor position in 1/65536 rev units, 1 revolution is 16bit
    uint16      M1_Voltage;     // Bus voltage in 0.01V units
    uint16      M1_Variable_1;	// Absolute pivot position in 1/16536 rev units, 1 revolution is 16bit
    uint16      M1_Variable_2;	// unused

    uint8       M2_Update;     	// Cyclic counter, increments after each packet exchange with motor controller
    uint8       M2_Status;      // Bitmap, see Mx status
    int16       M2_Current;    	// Motor phase current in 0.01A units
    int16       M2_Velocity;   	// Motor speed in 0.01 Rad/s units
    uint16      M2_Position;   	// Absolute motor position in 1/65536 rev units, 1 revolution is 16bit
    uint16      M2_Voltage;     // Bus voltage in 0.01V units
    uint16      M2_Variable_1;	// Absolute pivot position in 1/16536 rev units, 1 revolution is 16bit
    uint16      M2_Variable_2;	// unused

    uint8       M3_Update;     	// Cyclic counter, increments after each packet exchange with motor controller
    uint8       M3_Status;      // Bitmap, see Mx status
    int16       M3_Current;    	// Motor phase current in 0.01A units
    int16       M3_Velocity;   	// Motor speed in 0.01 Rad/s units
    uint16      M3_Position;   	// Absolute motor position in 1/65536 rev units, 1 revolution is 16bit
    uint16      M3_Voltage;     // Bus voltage in 0.01V units
    uint16      M3_Variable_1;	// Absolute pivot position in 1/16536 rev units, 1 revolution is 16bit
    uint16      M3_Variable_2;	// unused
    
    uint8       M4_Update;     	// Cyclic counter, increments after each packet exchange with motor controller
    uint8       M4_Status;      // Bitmap, see Mx status
    int16       M4_Current;    	// Motor phase current in 0.01A units
    int16       M4_Velocity;   	// Motor speed in 0.01 Rad/s units
    uint16      M4_Position;   	// Absolute motor position in 1/65536 rev units, 1 revolution is 16bit
    uint16      M4_Voltage;     // Bus voltage in 0.01V units
    uint16      M4_Variable_1;	// Absolute pivot position in 1/16536 rev units, 1 revolution is 16bit
    uint16      M4_Variable_2;	// unused
    
    uint8       M5_Update;     	// Cyclic counter, increments after each packet exchange with motor controller
    uint8       M5_Status;      // Bitmap, see Mx status
    int16       M5_Current;    	// Motor phase current in 0.01A units
    int16       M5_Velocity;   	// Motor speed in 0.01 Rad/s units
    uint16      M5_Position;   	// Absolute motor position in 1/65536 rev units, 1 revolution is 16bit
    uint16      M5_Voltage;     // Bus voltage in 0.01V units
    uint16      M5_Variable_1;	// Absolute pivot position in 1/16536 rev units, 1 revolution is 16bit
    uint16      M5_Variable_2;	// unused
    
    uint8       M6_Update;     	// Cyclic counter, increments after each packet exchange with motor controller
    uint8       M6_Status;      // Bitmap, see Mx status
    int16       M6_Current;    	// Motor phase current in 0.01A units
    int16       M6_Velocity;   	// Motor speed in 0.01 Rad/s units
    uint16      M6_Position;   	// Absolute motor position in 1/65536 rev units, 1 revolution is 16bit
    uint16      M6_Voltage;     // Bus voltage in 0.01V units
    uint16      M6_Variable_1;	// Absolute pivot position in 1/16536 rev units, 1 revolution is 16bit
    uint16      M6_Variable_2;	// unused
    
    uint8       M7_Update;     	// Cyclic counter, increments after each packet exchange with motor controller
    uint8       M7_Status;      // Bitmap, see Mx status
    int16       M7_Current;    	// Motor phase current in 0.01A units
    int16       M7_Velocity;   	// Motor speed in 0.01 Rad/s units
    uint16      M7_Position;   	// Absolute motor position in 1/65536 rev units, 1 revolution is 16bit
    uint16      M7_Voltage;     // Bus voltage in 0.01V units
    uint16      M7_Variable_1;	// Absolute pivot position in 1/16536 rev units, 1 revolution is 16bit
    uint16      M7_Variable_2;	// unused
    
    uint8       M8_Update;     	// Cyclic counter, increments after each packet exchange with motor controller
    uint8       M8_Status;      // Bitmap, see Mx status
    int16       M8_Current;    	// Motor phase current in 0.01A units
    int16       M8_Velocity;   	// Motor speed in 0.01 Rad/s units
    uint16      M8_Position;   	// Absolute motor position in 1/65536 rev units, 1 revolution is 16bit
    uint16      M8_Voltage;     // Bus voltage in 0.01V units
    uint16      M8_Variable_1;	// Absolute pivot position in 1/16536 rev units, 1 revolution is 16bit
    uint16      M8_Variable_2;	// unused

	uint8       G1_Update;      // Cyclic counter, increments after each packet exchange
	int8        G1_Temperature; // Temperature of gyro sensor in 0.5C units
	int16       G1_Accell_x;    // Acceleration in local x axis 1/512 G units
	int16       G1_Accell_y;    // Acceleration in local y axis 1/512 G units
    int16       G1_Accell_z;    // Acceleration in local z axis 1/512 G units
	int16       G1_Gyro_x;      // Gyro velocity in local x axis 1/2000 deg/sec units
	int16       G1_Gyro_y;      // Gyro velocity in local y axis 1/2000 deg/sec units
	int16       G1_Gyro_z;      // Gyro velocity in local z axis 1/2000 deg/sec units
    
    uint8       G2_Update;      // Cyclic counter, increments after each packet exchange
	int8        G2_Temperature; // Temperature of gyro sensor in 0.5C units
	int16       G2_Accell_x;    // Acceleration in local x axis 1/512 G units
	int16       G2_Accell_y;    // Acceleration in local y axis 1/512 G units
    int16       G2_Accell_z;    // Acceleration in local z axis 1/512 G units
	int16       G2_Gyro_x;      // Gyro velocity in local x axis 1/2000 deg/sec units
	int16       G2_Gyro_y;      // Gyro velocity in local y axis 1/2000 deg/sec units
	int16       G2_Gyro_z;      // Gyro velocity in local z axis 1/2000 deg/sec units
    
    uint8       G3_Update;      // Cyclic counter, increments after each packet exchange
	int8        G3_Temperature; // Temperature of gyro sensor in 0.5C units
	int16       G3_Accell_x;    // Acceleration in local x axis 1/512 G units
	int16       G3_Accell_y;    // Acceleration in local y axis 1/512 G units
    int16       G3_Accell_z;    // Acceleration in local z axis 1/512 G units
	int16       G3_Gyro_x;      // Gyro velocity in local x axis 1/2000 deg/sec units
	int16       G3_Gyro_y;      // Gyro velocity in local y axis 1/2000 deg/sec units
	int16       G3_Gyro_z;      // Gyro velocity in local z axis 1/2000 deg/sec units
    
    uint8       G4_Update;      // Cyclic counter, increments after each packet exchange
	int8        G4_Temperature; // Temperature of gyro sensor in 0.5C units
	int16       G4_Accell_x;    // Acceleration in local x axis 1/512 G units
	int16       G4_Accell_y;    // Acceleration in local y axis 1/512 G units
    int16       G4_Accell_z;    // Acceleration in local z axis 1/512 G units
	int16       G4_Gyro_x;      // Gyro velocity in local x axis 1/2000 deg/sec units
	int16       G4_Gyro_y;      // Gyro velocity in local y axis 1/2000 deg/sec units
	int16       G4_Gyro_z;      // Gyro velocity in local z axis 1/2000 deg/sec units
    
    uint8       G5_Update;      // Cyclic counter, increments after each packet exchange
	int8        G5_Temperature; // Temperature of gyro sensor in 0.5C units
	int16       G5_Accell_x;    // Acceleration in local x axis 1/512 G units
	int16       G5_Accell_y;    // Acceleration in local y axis 1/512 G units
    int16       G5_Accell_z;    // Acceleration in local z axis 1/512 G units
	int16       G5_Gyro_x;      // Gyro velocity in local x axis 1/2000 deg/sec units
	int16       G5_Gyro_y;      // Gyro velocity in local y axis 1/2000 deg/sec units
	int16       G5_Gyro_z;      // Gyro velocity in local z axis 1/2000 deg/sec units
    
    uint8       G6_Update;      // Cyclic counter, increments after each packet exchange
	int8        G6_Temperature; // Temperature of gyro sensor in 0.5C units
	int16       G6_Accell_x;    // Acceleration in local x axis 1/512 G units
	int16       G6_Accell_y;    // Acceleration in local y axis 1/512 G units
    int16       G6_Accell_z;    // Acceleration in local z axis 1/512 G units
	int16       G6_Gyro_x;      // Gyro velocity in local x axis 1/2000 deg/sec units
	int16       G6_Gyro_y;      // Gyro velocity in local y axis 1/2000 deg/sec units
	int16       G6_Gyro_z;      // Gyro velocity in local z axis 1/2000 deg/sec units
    
    uint8       G7_Update;      // Cyclic counter, increments after each packet exchange
	int8        G7_Temperature; // Temperature of gyro sensor in 0.5C units
	int16       G7_Accell_x;    // Acceleration in local x axis 1/512 G units
	int16       G7_Accell_y;    // Acceleration in local y axis 1/512 G units
    int16       G7_Accell_z;    // Acceleration in local z axis 1/512 G units
	int16       G7_Gyro_x;      // Gyro velocity in local x axis 1/2000 deg/sec units
	int16       G7_Gyro_y;      // Gyro velocity in local y axis 1/2000 deg/sec units
	int16       G7_Gyro_z;      // Gyro velocity in local z axis 1/2000 deg/sec units
    
    uint8       G8_Update;      // Cyclic counter, increments after each packet exchange
	int8        G8_Temperature; // Temperature of gyro sensor in 0.5C units
	int16       G8_Accell_x;    // Acceleration in local x axis 1/512 G units
	int16       G8_Accell_y;    // Acceleration in local y axis 1/512 G units
    int16       G8_Accell_z;    // Acceleration in local z axis 1/512 G units
	int16       G8_Gyro_x;      // Gyro velocity in local x axis 1/2000 deg/sec units
	int16       G8_Gyro_y;      // Gyro velocity in local y axis 1/2000 deg/sec units
	int16       G8_Gyro_z;      // Gyro velocity in local z axis 1/2000 deg/sec units
} in_TU_EightWheelBot_t;	

/*************************** ROPOD Smart Wheel ***************************/

#define SW_STAT1_ENABLED1          0x0001
#define SW_STAT1_ENABLED2          0x0002
#define SW_STAT1_ENC_1_OK			0x0004
#define SW_STAT1_ENC_2_OK			0x0008
#define SW_STAT1_ENC_PIVOT_OK		0x0010
#define SW_STAT1_UNDERVOLTAGE		0x0020
#define SW_STAT1_OVERVOLTAGE		0x0040
#define SW_STAT1_OVERCURRENT_1		0x0080
#define SW_STAT1_OVERCURRENT_2		0x0100
#define SW_STAT1_OVERTEMP_1		0x0200
#define SW_STAT1_OVERTEMP_2		0x0400

#define SW_STAT2_UNUSED			0x0000

typedef struct PACKED{
  uint16_t		status1;			// Status bits as defined in STAT1_
  uint16_t		status2;			// Status bits as defined in STAT2_
  uint64_t		sensor_ts;			// EtherCAT timestamp (ns) on sensor acquisition 
  uint64_t		setpoint_ts;		// EtherCAT timestamp (ns) of last setpoint data
  float			encoder_1;			// encoder 1 value in rad (no wrapping at 2PI)
  float			velocity_1;			// encoder 1 velocity in rad/s
  float			current_1_d;		// motor 1 current direct in amp
  float			current_1_q;		// motor 1 current quadrature in amp
  float			current_1_u;		// motor 1 current phase U in amp
  float			current_1_v;		// motor 1 current phase V in amp
  float			current_1_w;		// motor 1 current phase W in amp
  float			voltage_1;			// motor 1 voltage from pwm in volts
  float			voltage_1_u;		// motor 1 voltage from phase U in volts
  float			voltage_1_v;		// motor 1 voltage from phase V	in volts
  float			voltage_1_w;		// motor 1 voltage from phase W in volts
  float			temperature_1;		// motor 1 estimated temperature in K
  float			encoder_2;			// encoder 2 value in rad (no wrapping at 2PI)
  float			velocity_2;         // encoder 2 velocity in rad/s
  float			current_2_d;		// motor 2 current direct in amp
  float			current_2_q;		// motor 2 current quadrature in amp
  float			current_2_u;		// motor 2 current phase U in amp
  float			current_2_v;		// motor 2 current phase V in amp
  float			current_2_w;		// motor 2 current phase W in amp
  float			voltage_2;			// motor 2 voltage from pwm in volts
  float			voltage_2_u;		// motor 2 voltage from phase U in volts
  float			voltage_2_v;		// motor 2 voltage from phase V	in volts
  float			voltage_2_w;		// motor 2 voltage from phase W in volts
  float			temperature_2;		// motor 2 estimated temperature in K
  float			encoder_pivot;		// encoder pivot value in rad (wrapping at -PI and +PI)
  float			velocity_pivot;		// encoder pivot velocity in rad/s
  float			voltage_bus;		// bus voltage in volts
  uint64_t		imu_ts;				// EtherCAT timestamp (ns) of IMU sensor acquisition
  float			accel_x;			// IMU accelerometer X-axis in m/s2
  float			accel_y;			// IMU accelerometer Y-axis in m/s2
  float			accel_z;			// IMU accelerometer Z-axis in m/s2
  float			gyro_x;				// IMU gyro X-axis in rad/s
  float			gyro_y;				// IMU gyro Y-axis in rad/s
  float			gyro_z;				// IMU gyro Z-axis in rad/s
  float			temperature_imu;	// IMU temperature in K	
  float			pressure;			// barometric pressure in Pa absolute
}sens_ROPOD_SmartWheel_t;

/* SMARTWHEEL SETPOINT MODES
 * 
 * Mode TORQUE
 *   Setpoint 1		= Current in Amp for motor 1
 *   Setpoint 2		= Current in Amp for motor 2
 *   Upper limit 1 	= Most positive velocity (rad/s) allowed for motor 1
 *   Lower limit 1  = Most negative velocity (rad/s) allowed for motor 1
 *   Upper limit 2 	= Most positive velocity (rad/s) allowed for motor 2
 *   Lower limit 2  = Most negative velocity (rad/s) allowed for motor 2
 *
 * Mode DTORQUE
 *   Setpoint 1		= Common current in Amp
 *   Setpoint 2		= Differential current in Amp
 *   Upper limit 1 	= Most positive velocity (rad/s) allowed for linear motion
 *   Lower limit 1  = Most negative velocity (rad/s) allowed for linear motion
 *   Upper limit 2 	= Most positive velocity (rad/s) allowed for pivot motion
 *   Lower limit 2  = Most negative velocity (rad/s) allowed for pivot motion
 *
 * Mode VELOCITY
 *   Setpoint 1		= Velocity in rad/s for motor 1
 *   Setpoint 2		= Velocity in rad/s for motor 2
 *   Upper limit 1 	= Most positive current (amp) allowed for motor 1
 *   Lower limit 1  = Most negative current (amp) allowed for motor 1
 *   Upper limit 2 	= Most positive current (amp) allowed for motor 2
 *   Lower limit 2  = Most negative current (amp) allowed for motor 2
 *
 * Mode DVELOCITY
 *   Setpoint 1		= Common velocity in rad/s
 *   Setpoint 2		= Differential velocity in rad/s
 *   Upper limit 1 	= Most positive current (amp) allowed for linear motion
 *   Lower limit 1  = Most negative current (amp) allowed for linear motion
 *   Upper limit 2 	= Most positive current (amp) allowed for pivot motion
 *   Lower limit 2  = Most negative current (amp) allowed for pivot motion
 *
*/

#define SW_COM1_ENABLE1          0x0001
#define SW_COM1_ENABLE2          0x0002
#define SW_COM1_MODE_TORQUE	  (0x0 << 2)
#define SW_COM1_MODE_DTORQUE	  (0x1 << 2)
#define SW_COM1_MODE_VELOCITY	  (0x2 << 2)
#define SW_COM1_MODE_DVELOCITY	  (0x3 << 2)
#define SW_COM1_EMERGENCY1		  0x0010	
#define SW_COM1_EMERGENCY2		  0x0020
#define SW_COM1_USE_TS			  0x8000

#define SW_COM2_UNUSED			  0x0000

typedef struct PACKED{
  uint16_t      command1;			// Command bits as defined in COM1_
  uint16_t		command2;			// Command bits as defined in COM2_
  float			setpoint1;			// Setpoint 1
  float			setpoint2;			// Setpoint 2
  float			limit1_p;			// Upper limit 1
  float			limit1_n;			// Lower limit 1
  float			limit2_p;			// Upper limit 2
  float			limit2_n;			// Lower limit 2
  uint64_t		timestamp;			// EtherCAT timestamp (ns) setpoint execution
}cmd_ROPOD_SmartWheel_t;

typedef struct {
  float			wheeldistance;			// distance between wheels in m
  float			casterdistance;			// caster offset in m
  float			pivotoffset;			// pivot encoder offset in rad
  float         p_vc_pgain;				// pivot velocity control P gain
  float         p_vc_igain;				// pivot velocity control I gain
  float         p_vc_ilimit;			// pivot velocity control I limiter
  float			p_vc_ffgain;			// pivot velocity model feed forward gain	
  float         m1resistance;			// motor 1 phase resistance in ohm
  float			m1inductance;			// motor 1 phase inductance in henry
  float         m1kv;					// motor 1 speed constant in rad/vs
  float			m1thermalr;				// motor 1 thermal resistance in K/W
  float			m1thermalt;				// motor 1 thermal time constant s
  float			m1diameter;				// motor 1 wheel diameter in m
  float         m1_cc_pgain;			// motor 1 current control P gain
  float         m1_cc_igain;			// motor 1 current control I gain
  float         m1_cc_ilimit;			// motor 1 current control I limiter
  float			m1_cc_ffgain;			// motor 1 current model feed forward gain
  float         m1_vc_pgain;			// motor 1 velocity control P gain
  float         m1_vc_igain;			// motor 1 velocity control I gain
  float         m1_vc_ilimit;			// motor 1 velocity control I limiter
  float			m1_vc_ffgain;			// motor 1 velocity model feed forward gain
  float			m1_enc_offset;			// motor 1 encoder offset in rad
  float			m1_enc_ppairs;			// motor 1 pole pairs, negative for phase reversal
  float         m2resistance;			// motor 2 phase resistance in ohm
  float			m2inductance;			// motor 2 phase inductance in henry
  float         m2kv;					// motor 2 speed constant in rad/vs
  float			m2thermalr;				// motor 2 thermal resistance in K/W
  float			m2thermalt;				// motor 2 thermal time constant s
  float			m2diameter;				// motor 2 wheel diameter in m
  float         m2_cc_pgain;			// motor 2 current control P gain
  float         m2_cc_igain;			// motor 2 current control I gain
  float         m2_cc_ilimit;			// motor 2 current control I limiter
  float			m2_cc_ffgain;			// motor 2 current model feed forward gain
  float         m2_vc_pgain;			// motor 2 velocity control P gain
  float         m2_vc_igain;			// motor 2 velocity control I gain
  float         m2_vc_ilimit;			// motor 2 velocity control I limiter
  float			m2_vc_ffgain;			// motor 2 velocity model feed forward gain
  float			m2_enc_offset;			// motor 2 encoder offset in rad
  float			m2_enc_ppairs;			// motor 2 pole pairs, negative for phase reversal
}cntr_param_ROPOD_SmartWheel_t;

typedef struct {
  float			voltage_min;			// minimum allowed voltage in enabled state in V
  float			voltage_min_time;		// time constant for minimum voltage filter in s
  float			voltage_max;			// maximum allowed voltage in enabled state in V
  float			current_max;			// maximum allowed peak current in A
  float			drive_temp_max;			// maximum allowed drive temperature in K
  float			motor_temp_max;			// maximum allowed motor temperature in K
  float			motor_current_limit;	// motor current limit in A
  float			motor_speed_limit;		// motor speed limit in rad/s
  float			watchdog_time;			// OP mode watchdog time out in s	
  float			distance_total;			// total travel distance in m	
  float			pivot_total;			// total pivots in rad
}drv_param_ROPOD_SmartWheel_t;

typedef struct {
  float			p_encoder_gain;			// pivot encoder gain in rad/bit
  float			bv_offset;				// bus voltage offset in V
  float			bv_gain;				// bus voltage sensor gain in V/bit
  float			m1_cu_offset;			// motor 1 current phase U offset in A	
  float			m1_cu_gain;				// motor 1 current phase U gain in A/bit
  float			m1_cv_offset;			// motor 1 current phase V offset in A
  float			m1_cv_gain;				// motor 1 current phase V gain in A/bit
  float			m1_cw_offset;			// motor 1 current phase W offset in A
  float			m1_cw_gain;				// motor 1 current phase W gain in A/bit
  float			m1_vu_offset;			// motor 1 voltage phase U offset in V
  float			m1_vu_gain;				// motor 1 voltage phase U gain in V/bit
  float			m1_vv_offset;			// motor 1 voltage phase U offset in V
  float			m1_vv_gain;				// motor 1 voltage phase U gain in V/bit
  float			m1_vw_offset;			// motor 1 voltage phase U offset in V
  float			m1_vw_gain;				// motor 1 voltage phase U gain in V/bit
  float			m1_encoder_gain;		// motor 1 encoder gain in rad/bit
  float			m2_cu_offset;			// motor 2 current phase U offset in A
  float			m2_cu_gain;				// motor 2 current phase U gain in A/bit
  float			m2_cv_offset;			// motor 2 current phase V offset in A
  float			m2_cv_gain;				// motor 2 current phase V gain in A/bit
  float			m2_cw_offset;			// motor 2 current phase W offset in A
  float			m2_cw_gain;				// motor 2 current phase W gain in A/bit
  float			m2_vu_offset;			// motor 2 voltage phase U offset in V
  float			m2_vu_gain;				// motor 2 voltage phase U gain in V/bit
  float			m2_vv_offset;			// motor 2 voltage phase U offset in V
  float			m2_vv_gain;				// motor 2 voltage phase U gain in V/bit
  float			m2_vw_offset;			// motor 2 voltage phase U offset in V
  float			m2_vw_gain;				// motor 2 voltage phase U gain in V/bit
  float			m2_encoder_gain;		// motor 2 encoder gain in rad/bit
  float			imu_acc_x_offset;		// IMU accelerometer X offset in m/s2
  float			imu_acc_x_gain;			// IMU accelerometer X gain in m/s2 / bit
  float			imu_acc_y_offset;		// IMU accelerometer Y offset in m/s2
  float			imu_acc_y_gain;			// IMU accelerometer Y gain in m/s2 / bit
  float			imu_acc_z_offset;		// IMU accelerometer Z offset in m/s2
  float			imu_acc_z_gain;			// IMU accelerometer Z gain in m/s2 / bit
  float			imu_gyro_x_offset;		// IMU gyro X offset in rad/s
  float			imu_gyro_x_gain;		// IMU gyro X gain in rad/s / bit
  float			imu_gyro_y_offset;		// IMU gyro Y offset in rad/s
  float			imu_gyro_y_gain;		// IMU gyro Y gain in rad/s / bit
  float			imu_gyro_z_offset;		// IMU gyro Z offset in rad/s
  float			imu_gyro_z_gain;		// IMU gyro Z gain in rad/s / bit
}sens_param_ROPOD_SmartWheel_t;

/* **************** */


typedef struct PACKED{
   uint8_t      enable;
   float        c_setpoint;
   float        v_setpoint;
}cmd_ROPOD_SmartWheel2_t;

typedef struct {
   uint16_t		raw_adc[8];
   float		cur[3];
   int32_t      raw_adc2[6];
   uint32_t		inc_enc;
   uint32_t		inc_idx;
   int32_t		inc_spd;
   uint32_t		abs_enc[3];
   float		abs_spd[3];
   uint32_t 	data[4];
   float		fl[2];
}sens_ROPOD_SmartWheel2_t;


/* ************************************* */



//couple structures to pointerarrays---------------------*/	

in_EL1008t		*in_EL1008_[MAX_NUMBER_OF_SLAVES];
in_EL1014t		*in_EL1014_[MAX_NUMBER_OF_SLAVES];
in_EL1018t		*in_EL1018_[MAX_NUMBER_OF_SLAVES];

out_EL2004t		*out_EL2004_[MAX_NUMBER_OF_SLAVES];
out_EL2008t		*out_EL2008_[MAX_NUMBER_OF_SLAVES];
out_EL2502t		*out_EL2502_[MAX_NUMBER_OF_SLAVES];

in_EL3008t		*in_EL3008_[MAX_NUMBER_OF_SLAVES];
in_EL3102t		*in_EL3102_[MAX_NUMBER_OF_SLAVES];

out_EL4132t		*out_EL4132_[MAX_NUMBER_OF_SLAVES];
out_EL4038t		*out_EL4038_[MAX_NUMBER_OF_SLAVES];

out_EL5101t		*out_EL5101_[MAX_NUMBER_OF_SLAVES];
in_EL5101t		*in_EL5101_[MAX_NUMBER_OF_SLAVES];
out_EL5152t		*out_EL5152_[MAX_NUMBER_OF_SLAVES];
in_EL5152t		*in_EL5152_[MAX_NUMBER_OF_SLAVES];

/* maximum of 2 TU0001 slaves */
out_TU0001t		*out_TU0001_[2];
in_TU0001t		*in_TU0001_[2];

out_TU_ES_030t		*out_TU_ES_030_[6];
in_TU_ES_030t		*in_TU_ES_030_[6];

out_TU_ES_030v2t    *out_TU_ES_030v2_[6];
in_TU_ES_030v2t		*in_TU_ES_030v2_[6];

out_TU_EightWheelBot_t	*out_TU_EightWheelBot_[1];
in_TU_EightWheelBot_t   *in_TU_EightWheelBot_[1];         

#define MAX_NUMBER_SMARTWHEELS 4

cmd_ROPOD_SmartWheel_t	 *cmd_ROPOD_SmartWheel_t_[MAX_NUMBER_SMARTWHEELS];
sens_ROPOD_SmartWheel_t  *sens_ROPOD_SmartWheel_t_[MAX_NUMBER_SMARTWHEELS];  

#define MAX_NUMBER_SMARTWHEELS2 8
cmd_ROPOD_SmartWheel2_t	 *cmd_ROPOD_SmartWheel2_t_[MAX_NUMBER_SMARTWHEELS2];
sens_ROPOD_SmartWheel2_t  *sens_ROPOD_SmartWheel2_t_[MAX_NUMBER_SMARTWHEELS2];  
