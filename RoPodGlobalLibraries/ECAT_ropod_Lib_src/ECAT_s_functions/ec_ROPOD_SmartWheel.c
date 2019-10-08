/*
        ec_ROPOD_SmartWheel

        (c) Cesar Lopez, 2017

        Outputs:
  y[0]		status1;			// Status bits as defined in STAT1_
  y[1]		status2;			// Status bits as defined in STAT2_
  y[2]		sensor_ts;			// EtherCAT timestamp (ns) on sensor acquisition 
  y[3]		setpoint_ts;		// EtherCAT timestamp (ns) of last setpoint data
  y[4]		encoder_1;			// encoder 1 value in rad (no wrapping at 2PI)
  y[5]		velocity_1;			// encoder 1 velocity in rad/s
  y[6]		current_1_d;		// motor 1 current direct in amp
  y[7]		current_1_q;		// motor 1 current quadrature in amp
  y[8]		current_1_u;		// motor 1 current phase U in amp
  y[9]		current_1_v;		// motor 1 current phase V in amp
  y[10]		current_1_w;		// motor 1 current phase W in amp
  y[11]		voltage_1;			// motor 1 voltage from pwm in volts
  y[12]		voltage_1_u;		// motor 1 voltage from phase U in volts
  y[13]		voltage_1_v;		// motor 1 voltage from phase V	in volts
  y[14]		voltage_1_w;		// motor 1 voltage from phase W in volts
  y[15]		temperature_1;		// motor 1 estimated temperature in K
  y[16]		encoder_2;			// encoder 2 value in rad (no wrapping at 2PI)
  y[17]		velocity_2;         // encoder 2 velocity in rad/s
  y[18]		current_2_d;		// motor 2 current direct in amp
  y[19]		current_2_q;		// motor 2 current quadrature in amp
  y[20]		current_2_u;		// motor 2 current phase U in amp
  y[21]		current_2_v;		// motor 2 current phase V in amp
  y[22]		current_2_w;		// motor 2 current phase W in amp
  y[23]		voltage_2;			// motor 2 voltage from pwm in volts
  y[24]		voltage_2_u;		// motor 2 voltage from phase U in volts
  y[25]		voltage_2_v;		// motor 2 voltage from phase V	in volts
  y[26]		voltage_2_w;		// motor 2 voltage from phase W in volts
  y[27]		temperature_2;		// motor 2 estimated temperature in K
  y[28]		encoder_pivot;		// encoder pivot value in rad (wrapping at -PI and +PI)
  y[29]		velocity_pivot;		// encoder pivot velocity in rad/s
  y[30]		voltage_bus;		// bus voltage in volts
  y[31]		imu_ts;				// EtherCAT timestamp (ns) of IMU sensor acquisition
  y[32]		accel_x;			// IMU accelerometer X-axis in m/s2
  y[33]		accel_y;			// IMU accelerometer Y-axis in m/s2
  y[34]		accel_z;			// IMU accelerometer Z-axis in m/s2
  y[35]		gyro_x;				// IMU gyro X-axis in rad/s
  y[36]		gyro_y;				// IMU gyro Y-axis in rad/s
  y[37]		gyro_z;				// IMU gyro Z-axis in rad/s
  y[38]		temperature_imu;	// IMU temperature in K	
  y[39]		pressure;			// barometric pressure in Pa absolute

	Inputs: 
  u[0]      command1;			// Command bits as defined in COM1_
  u[1]		command2;			// Command bits as defined in COM2_
  u[2]		setpoint1;			// Setpoint 1
  u[3]		setpoint2;			// Setpoint 2
  u[4]		limit1_p;			// Upper limit 1
  u[5]		limit1_n;			// Lower limit 1
  u[6]		limit2_p;			// Upper limit 2
  u[7]		limit2_n;			// Lower limit 2
  u[8]		timestamp;			// EtherCAT timestamp (ns) setpoint execution
					

        Parameter: link id
*/
#define S_FUNCTION_NAME ec_ROPOD_SmartWheel
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define NSTATES         0
#define NINPUTS         9
#define NOUTPUTS        40
#define NPARAMS         1

#define LINK_ID         ssGetSFcnParam(S,0)

#define U(element) (*uPtrs[element])  /* Pointer to Input Port0 */

#include <math.h>
#include "ec.h"

/*====================*
 * S-function methods *
 *====================*/

static void mdlInitializeSizes(SimStruct *S)
{
	ssSetNumSFcnParams(S,NPARAMS);

	ssSetNumContStates(S,NSTATES);
	ssSetNumDiscStates(S,0);

	if (!ssSetNumInputPorts(S,1)) return;
	ssSetInputPortWidth(S,0,NINPUTS);
	ssSetInputPortDirectFeedThrough(S,0,0); 

	if (!ssSetNumOutputPorts(S,1)) return;
	ssSetOutputPortWidth(S,0,NOUTPUTS);

	ssSetNumSampleTimes(S,1);
	ssSetNumRWork(S,0);
	ssSetNumIWork(S,0);
	ssSetNumPWork(S,0);
	ssSetNumModes(S,0);
	ssSetNumNonsampledZCs(S,0);
}





static void mdlInitializeSampleTimes(SimStruct *S)
{
	ssSetSampleTime(S,0,CONTINUOUS_SAMPLE_TIME);
	ssSetOffsetTime(S, 0, FIXED_IN_MINOR_STEP_OFFSET);
}






static void mdlOutputs(SimStruct *S, int_T tid)
{
        real_T *y=ssGetOutputPortRealSignal(S,0);
        InputRealPtrsType uPtrs=ssGetInputPortRealSignalPtrs(S,0);      
#ifndef MATLAB_MEX_FILE
        int_T ilink, ireadchan, iwritechan;

        ilink=(int_T) (*(mxGetPr(LINK_ID)));
/*      read channel */
        for (ireadchan=0; ireadchan<NOUTPUTS; ireadchan++) {
        	ec_ROPOD_SmartWheel_read_sens(&y[ireadchan], ireadchan, ilink);
	}

/*      write channel */
        for (iwritechan=0; iwritechan<NINPUTS; iwritechan++) {
            ec_ROPOD_SmartWheel_write_cmd(U(iwritechan), iwritechan, ilink);
        }
#endif
}





static void mdlTerminate(SimStruct *S)
{
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
