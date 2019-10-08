/*
        ec_ROPOD_SmartWheel2

        (c) Cesar Lopez, 2017

        Outputs:
  y[0]		encoder_wheel;			// value in rad (no wrapping at 2PI)
  y[1]		encoder_pivot;			// value in rad (no wrapping at 2PI)
  y[2]		velocity_wheel;			// velocity in rad/s
  y[3]		velocity_pivot;			// velocity in rad/s


	Inputs: 
  u[0]      enable;			// Command bits as defined in COM1_
  u[1]		c_setpoint;			// Setpoint current
  u[2]		v_setpoint;			// Setpoint velocity

					

        Parameter: link id
*/
#define S_FUNCTION_NAME ec_ROPOD_SmartWheel2
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#define NSTATES         0
#define NINPUTS         3
#define NOUTPUTS        4
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
        	ec_ROPOD_SmartWheel2_read_sens(&y[ireadchan], ireadchan, ilink);
	}

/*      write channel */
        for (iwritechan=0; iwritechan<NINPUTS; iwritechan++) {
            ec_ROPOD_SmartWheel2_write_cmd(U(iwritechan), iwritechan, ilink);
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
