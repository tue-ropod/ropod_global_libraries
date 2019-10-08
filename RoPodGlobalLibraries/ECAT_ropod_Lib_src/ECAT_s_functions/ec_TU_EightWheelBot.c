/*
        ec_TU_EightWheelBot

        (c) Ferry Schoenmakers, 2017

        Outputs:
        y[0]  = State

		y[1]  = M1_Update
		y[2]  = M1_Status
		y[3]  = M1_Current
		y[4]  = M1_Velocity
        y[5]  = M1_Position
        y[6]  = M1_Voltage
        y[7]  = M1_Variable_1
        y[8]  = M1_Variable_2
        |
        |
        V
        y[57]  = M8_Update
		y[58]  = M8_Status
		y[59]  = M8_Current
		y[60]  = M8_Velocity
        y[61]  = M8_Position
        y[62]  = M8_Voltage
        y[63]  = M8_Variable_1
        y[64]  = M8_Variable_2

        y[65]  = G1_Update
		y[66]  = G1_Temperature
		y[67]  = G1_Accell_x
		y[68]  = G1_Accell_y
        y[69]  = G1_Accell_z
        y[70]  = G1_Gyro_x
        y[71]  = G1_Gyro_y
        y[72]  = G1_Gyro_z
        |
        |
        V
        y[121]  = G8_Update
		y[122]  = G8_Temperature
		y[123]  = G8_Accell_x
		y[124]  = G8_Accell_y
        y[125]  = G8_Accell_z
        y[126]  = G8_Gyro_x
        y[127]  = G8_Gyro_y
        y[128]  = G8_Gyro_z

	Inputs: 
        u[0]  = Command

		u[1]  = M1_Drivemode
		u[2]  = M1_Command
		u[3]  = M1_Velocity_SP
		u[4]  = M1_Current_SP
        |
        |
        V
        u[29]  = M8_Drivemode
		u[30]  = M8_Command
		u[31]  = M8_Velocity_SP
		u[32]  = M8_Current_SP
					

        Parameter: link id
*/
#define S_FUNCTION_NAME ec_TU_EightWheelBot
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define NSTATES         0
#define NINPUTS         33
#define NOUTPUTS        129
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
        	ec_TU_EightWheelBot_read_chan(&y[ireadchan], ireadchan, ilink);
	}

/*      write channel */

        for (iwritechan=0; iwritechan<NINPUTS; iwritechan++) {
            ec_TU_EightWheelBot_write_chan(U(iwritechan), iwritechan, ilink);
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
