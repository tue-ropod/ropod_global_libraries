/*
 *
 * MATLAB function code for Simple Open EtherCAT master
 * (c) IME Technologies 2008 & M.j.g. v.d. Molengraft
 *
 */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>
#include "stdio.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "math.h"

#include <string.h>
#include "soem.h"
#include "ec.h"

#include "ec_structdata.h"

int rval;
uint16 ecvar;
struct timeval tv1,tv2,tv3;
struct sched_param schedp;
static char IOmap[4096];/* EtherCAT I/O buffer */
static int bitmask[8];

static int nEK1100=0, nEL1008=0, nEL1018=0, nEL1014=0, nEL2004=0, nEL2008=0, nEL2502=0,
        nEL3008=0, nEL3102=0, nEL4132=0, nEL4038=0, nEL5101=0, nTU0001=0, nEL6022=0, 
        nEL5152=0, nTU_ES_030=0, nTU_ES_030v2=0, nTU_EightWheelBot=0,
        nROPODSmartWheel=0 , nROPODSmartWheel2=0;

static encoder_data_t* pEL5152_data[MAX_NUMBER_OF_SLAVES];
static encoder_data_t* pEL5101_data[MAX_NUMBER_OF_SLAVES];

int ec_EL1008_di_read_chan(double *pvalue, int ichan, int ilink)
{
    int ivalue;
    
    if ( (ilink<=0) || (ilink>nEL1008) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    ivalue=(in_EL1008_[ilink]->inbits & bitmask[ichan] )>>ichan;
    
    *pvalue=(double) ivalue;
    
    return 0;
}
/*================================================================================*/

int ec_EL1014_di_read_chan(double *pvalue, int ichan, int ilink)
{
    int ivalue;
    
    if ( (ilink<=0) || (ilink>nEL1014) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    ivalue=(in_EL1014_[ilink]->inbits & bitmask[ichan] )>>ichan;
    
    *pvalue=(double) ivalue;
    
    return 0;
}
/*================================================================================*/

int ec_EL1018_di_read_chan(double *pvalue, int ichan, int ilink)
{
    int ivalue;
    
    if ( (ilink<=0) || (ilink>nEL1018) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    ivalue=(in_EL1018_[ilink]->inbits & bitmask[ichan] )>>ichan;
    
    *pvalue=(double) ivalue;
    
    return 0;
}
/*=========================================================================*/

int ec_EL2004_do_write_chan(double value, int ichan, int ilink)
{
    int ivalue;
    
    if ( (ilink<=0) || (ilink>nEL2004) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    ivalue=(int) value;
    
    switch (ivalue) {
        case 0:
            out_EL2004_[ilink]->outbits = (out_EL2004_[ilink]->outbits & (~bitmask[ichan]));
            break;
        case 1:
            out_EL2004_[ilink]->outbits = (out_EL2004_[ilink]->outbits | bitmask[ichan]);
            break;
    }
    
    return 0;
}
/*================================================================================*/

int ec_EL2008_do_write_chan(double value, int ichan, int ilink)
{
    int ivalue;
    
    if ( (ilink<=0) || (ilink>nEL2008) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    ivalue=(int) value;
    
    
    switch (ivalue) {
        case 0:
            out_EL2008_[ilink]->outbits = (out_EL2008_[ilink]->outbits & (~bitmask[ichan]));
            break;
        case 1:
            out_EL2008_[ilink]->outbits = (out_EL2008_[ilink]->outbits | bitmask[ichan]);
            break;
    }
    
    return 0;
}
/*=========================================================================*/
/** input value between 0-1 represents 0 to 100% duty cycle */
int ec_EL2502_pwm_write_chan(double value, int ichan, int ilink)
{
    int ivalue,res;
    
    if ( (ilink<=0) || (ilink>nEL2502) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    res = 65535;
    
    if(value > 1.0){
        value = 1.0;
    }else if(value < 0.0){
        value = 0.0;
    }
    
    ivalue=(uint16) (value*res);
    
    switch (ichan) {
        case 0:
            out_EL2502_[ilink]->pwm1 = ivalue; //ivalue;
            break;
        case 1:
            out_EL2502_[ilink]->pwm2 = ivalue;
            break;
    };
    
    return 0;
}
/*=========================================================================*/
int ec_EL3008_adc_read_chan(double *pvalue, int ichan, int ilink)
{
    double res;
    int ivalue=0;
    
    if ( (ilink<=0) || (ilink>nEL3008) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    res=20.0/65535;
    
    switch(ichan){
        case 0 :
            ivalue=in_EL3008_[ilink]->invalue1;
            break;
        case 1 :
            ivalue=in_EL3008_[ilink]->invalue2;
            break;
        case 2 :
            ivalue=in_EL3008_[ilink]->invalue3;
            break;
        case 3 :
            ivalue=in_EL3008_[ilink]->invalue4;
            break;
        case 4 :
            ivalue=in_EL3008_[ilink]->invalue5;
            break;
        case 5 :
            ivalue=in_EL3008_[ilink]->invalue6;
            break;
        case 6 :
            ivalue=in_EL3008_[ilink]->invalue7;
            break;
        case 7 :
            ivalue=in_EL3008_[ilink]->invalue8;
            break;
        default :
            printf("ERROR: %s:%d, incorrect channel : %d for slave with link ID : %d\n",__FILE__,__LINE__,ichan, ilink);
            return -1;
            break;
    }
    
    if (ivalue>32768) {
        ivalue=-(65535-ivalue);
    }
    
    *pvalue=((double) ivalue)*res;
    
    return 0;
}
/*=========================================================================*/
int ec_EL3102_adc_read_chan(double *pvalue, int ichan, int ilink)
{
    double res;
    int ivalue=0;
    
    if ( (ilink<=0) || (ilink>nEL3102) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    res=20.0/65535;
    
    if (ichan==0){
        ivalue=in_EL3102_[ilink]->invalue1;
    }
    else if (ichan==1){
        ivalue=in_EL3102_[ilink]->invalue2;
    }
    
    if (ivalue>32768) {
        ivalue=-(65535-ivalue);
    }
    
    *pvalue=((double) ivalue)*res;
    
    return 0;
}
/*=========================================================================*/

int ec_EL4132_dac_write_chan(double voltage_f, int ichan, int ilink)
{
    double ttt;
    int voltage_i;
    
    if ( (ilink<=0) || (ilink>nEL4132) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    ttt=(voltage_f * 32767.0) / 10.0;
    voltage_i = (int) ttt;
    
    /* analog output */
    if (ichan==0){
        out_EL4132_[ilink]->outvalue1=voltage_i;
    }
    else if (ichan==1){
        out_EL4132_[ilink]->outvalue2=voltage_i;
    }
    
    return 0;
}
/*================================================================================*/

int ec_EL4038_dac_write_chan(double voltage_f, int ichan, int ilink)
{
    double ttt;
    int voltage_i;
    
    if ( (ilink<=0) || (ilink>nEL4038) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    /* saturation */
    if(voltage_f>10.0) {
        voltage_f=10.0;
    } else if (voltage_f<-10.0){
        voltage_f=-10.0;
    }
    
    ttt=(voltage_f * 32767.0) / 10.0;
    voltage_i = (int16) ttt;
    
    /* analog output */
    if (ichan==0){
        out_EL4038_[ilink]->outvalue1=voltage_i;
    }
    else if (ichan==1){
        out_EL4038_[ilink]->outvalue2=voltage_i;
    }
    else if (ichan==2){
        out_EL4038_[ilink]->outvalue3=voltage_i;
    }
    else if (ichan==3){
        out_EL4038_[ilink]->outvalue4=voltage_i;
    }
    else if (ichan==4){
        out_EL4038_[ilink]->outvalue5=voltage_i;
    }
    else if (ichan==5){
        out_EL4038_[ilink]->outvalue6=voltage_i;
    }
    else if (ichan==6){
        out_EL4038_[ilink]->outvalue7=voltage_i;
    }
    else if (ichan==7){
        out_EL4038_[ilink]->outvalue8=voltage_i;
    }
    
    return 0;
}
/*=========================================================================*/

int ec_EL5101_enc_read_chan(double *pvalue, int ilink)
{
    long int ivalue;
    
    if ( (ilink<=0) || (ilink>nEL5101) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    ivalue=in_EL5101_[ilink]->invalue;
    
    if (ivalue>EL_5101_COUNTER_MAX/2) {
        ivalue=-(EL_5101_COUNTER_MAX-ivalue);
    }
    /* initialize during first three samples because the counter values received from EtherCAT is zero in the first sample */
    if (pEL5101_data[ilink]->initialized[0] < 3) {
        pEL5101_data[ilink]->count0[0] = ivalue;
        pEL5101_data[ilink]->initialized[0]++;
    }

    /* correct with initial counter value */
    ivalue=ivalue-pEL5101_data[ilink]->count0[0];
    
    /* keep track of over/underflow */
    if ( (ivalue-pEL5101_data[ilink]->prev_count[0])>20000 ) { /* assume underflow! */
        pEL5101_data[ilink]->irevol[0]--;
    } else if ( (pEL5101_data[ilink]->prev_count[0]-ivalue)>20000 ) { /* assume overflow! */
        pEL5101_data[ilink]->irevol[0]++;
    }
    
    /* store counter value */
    pEL5101_data[ilink]->prev_count[0]=ivalue;
    
    ivalue=ivalue+pEL5101_data[ilink]->irevol[0]*EL_5101_COUNTER_MAX;
    
    *pvalue=(double) ivalue;

    return 0;
}
int ec_EL5152_enc_read_chan(double *pvalue, int ilink)
{
    long int ivalue[2];
    int i;
    
    if ( (ilink<=0) || (ilink>nEL5152) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    ivalue[0]=in_EL5152_[ilink]->invalue0;
    ivalue[1]=in_EL5152_[ilink]->invalue1;
    
    for(i=0;i<2;i++){
        if (ivalue[i]>EL_5152_COUNTER_MAX/2) {
            ivalue[i]=-(EL_5152_COUNTER_MAX-ivalue[i]);
        }
        /* initialize during first three samples because the counter values received from EtherCAT is zero in the first sample */
        if (pEL5152_data[ilink]->initialized[i] < 3) {
            pEL5152_data[ilink]->count0[i] = ivalue[i];
            pEL5152_data[ilink]->initialized[i]++;
        }
        
        /* correct with initial counter value */
        ivalue[i] = ivalue[i]-pEL5152_data[ilink]->count0[i];
        
        /* keep track of over/underflow */
        if ( (ivalue[i]-pEL5152_data[ilink]->prev_count[i])>20000 ) { /* assume underflow! */
            pEL5152_data[ilink]->irevol[i]--;
        } else if ( (pEL5152_data[ilink]->prev_count[i]-ivalue[i])>20000 ) { /* assume overflow! */
            pEL5152_data[ilink]->irevol[i]++;
        }
        
        /* store counter value */
        pEL5152_data[ilink]->prev_count[i]=ivalue[i];
        
        ivalue[i] = ivalue[i] + pEL5152_data[ilink]->irevol[i]*EL_5152_COUNTER_MAX;
        
        pvalue[i]=(double) ivalue[i];
    }
    
    return 0;
}
/*================================================================================*/

int ec_TU0001_combi_read_chan(double *pvalue, int ichan, int ilink)
{
    double res, res2;
    res = 10.0/1024;
    res2= 24.0/32768; /* acceleration range +- 24G */
    
    if ( (ilink<=0) || (ilink>nTU0001) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    switch (ichan){
        case 0:
            *pvalue=(double) in_TU0001_[ilink]->status;
            break;
        case 1:
            *pvalue=(double) in_TU0001_[ilink]->counter;
            break;
        case 2:
            *pvalue=(double) in_TU0001_[ilink]->diginput;
            break;
        case 3:
            *pvalue=((double) in_TU0001_[ilink]->analog[0]) * res;
            break;
        case 4:
            *pvalue=((double) in_TU0001_[ilink]->analog[1]) * res;
            break;
        case 5:
            *pvalue=((double) in_TU0001_[ilink]->analog[2]) * res;
            break;
        case 6:
            *pvalue=((double) in_TU0001_[ilink]->analog[3]) * res;
            break;
        case 7:
            *pvalue=((double) in_TU0001_[ilink]->analog[4]) * res;
            break;
        case 8:
            *pvalue=((double) in_TU0001_[ilink]->analog[5]) * res;
            break;
        case 9:
            *pvalue=((double) in_TU0001_[ilink]->analog[6]) * res;
            break;
        case 10:
            *pvalue=((double) in_TU0001_[ilink]->analog[7]) * res;
            break;
        case 11:
            *pvalue=(double) in_TU0001_[ilink]->CAM1status;
            break;
        case 12:
            *pvalue=(double) in_TU0001_[ilink]->CAM1posX*0.001;
            break;
        case 13:
            *pvalue=(double) in_TU0001_[ilink]->CAM1posY*0.001;
            break;
        case 14:
            *pvalue=(double) in_TU0001_[ilink]->CAM1posZ*0.001;
            break;
        case 15:
            *pvalue=(double) in_TU0001_[ilink]->CAM1velX*0.001;
            break;
        case 16:
            *pvalue=(double) in_TU0001_[ilink]->CAM1velY*0.001;
            break;
        case 17:
            *pvalue=(double) in_TU0001_[ilink]->CAM1velZ*0.001;
            break;
        case 18:
            *pvalue=(double) in_TU0001_[ilink]->CAM1timestamp;
            break;
        case 19:
            *pvalue=(double) in_TU0001_[ilink]->SHTstatus;
            break;
        case 20:
            *pvalue=(double) in_TU0001_[ilink]->SHTvalue;
            break;
        case 21:
            *pvalue=(double) in_TU0001_[ilink]->AccelerationX*res2;
            break;
        case 22:
            *pvalue=(double) in_TU0001_[ilink]->AccelerationY*res2;
            break;
        case 23:
            *pvalue=(double) in_TU0001_[ilink]->AccelerationZ*res2;
            break;
    }
    
    return 0;
}
/*================================================================================*/

int ec_TU0001_combi_write_chan(double outputvalue, int ichan, int ilink)
{
    if ( (ilink<=0) || (ilink>nTU0001) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    switch (ichan){
        case 0: /* CAM1control*/
            out_TU0001_[ilink]->CAM1control = (uint16) outputvalue;
            break;
        case 1: /* CAM1data*/
            out_TU0001_[ilink]->CAM1data = (uint32) outputvalue;
            break;
        case 2: /* SHTcontrol */
            out_TU0001_[ilink]->SHTcontrol = (uint8) outputvalue;
            break;
        case 3: /* SHTdutycycle 0-1*/
            if(outputvalue > 1){
                out_TU0001_[ilink]->SHTdutycycle = 255;
            }else{
                out_TU0001_[ilink]->SHTdutycycle = (uint8) (outputvalue * 255);
            }
            break;
        case 4: /* SHTpulselength ms*/
            if(outputvalue > 255){
                out_TU0001_[ilink]->SHTpulselength = 255;
            }else{
                out_TU0001_[ilink]->SHTpulselength = (uint8) outputvalue;
            }
            break;
        case 5: /* SHTencodermax*/
            if(outputvalue > 255){
                out_TU0001_[ilink]->SHTencodermax = 255;
            }else{
                out_TU0001_[ilink]->SHTencodermax = (uint8) outputvalue;
            }
            break;
    }

    return 0;
}
/*================================================================================*/
int ec_TU_ES_030_read_chan(double *pvalue, int ichan, int ilink)
{
    if ( (ilink<=0) || (ilink>nTU_ES_030) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    switch (ichan){
        case 0:
            *pvalue=(double) in_TU_ES_030_[ilink]->port;
            break;
        case 1:
            *pvalue=(double) in_TU_ES_030_[ilink]->encoder_1;
            break;
        case 2:
            *pvalue=(double) in_TU_ES_030_[ilink]->encoder_2;
            break;
        case 3:
            *pvalue=(double) in_TU_ES_030_[ilink]->encoder_3;
            break;
        case 4:
            *pvalue=(double) in_TU_ES_030_[ilink]->current_1;
            break;
        case 5:
            *pvalue=(double) in_TU_ES_030_[ilink]->current_2;
            break;
        case 6:
            *pvalue=(double) in_TU_ES_030_[ilink]->current_3;
            break;
        case 7:
            *pvalue=(double) in_TU_ES_030_[ilink]->caliper_1;
            break;
        case 8:
            *pvalue=(double) in_TU_ES_030_[ilink]->caliper_2;
            break;
        case 9:
            *pvalue=(double) in_TU_ES_030_[ilink]->force_1;
            break;
        case 10:
            *pvalue=(double) in_TU_ES_030_[ilink]->force_2;
            break;
        case 11:
            *pvalue=(double) in_TU_ES_030_[ilink]->force_3;
            break;
        case 12:
            *pvalue=(double) in_TU_ES_030_[ilink]->pos_1;
            break;
        case 13:
            *pvalue=(double) in_TU_ES_030_[ilink]->pos_2;
            break;
        case 14:
            *pvalue=(double) in_TU_ES_030_[ilink]->pos_3;
            break; 
        case 15:
            *pvalue=(double) in_TU_ES_030_[ilink]->spare_ai_1;
            break;
        case 16:
            *pvalue=(double) in_TU_ES_030_[ilink]->spare_ai_2;
            break;
        case 17:
            *pvalue=(double) in_TU_ES_030_[ilink]->time_stamp;
            break;
    }
    
    return 0;
}
/*================================================================================*/

int ec_TU_ES_030_write_chan(double outputvalue, int ichan, int ilink)
{
    if ( (ilink<=0) || (ilink>nTU_ES_030) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    switch (ichan){
        case 0: 
            out_TU_ES_030_[ilink]->port = (uint8) outputvalue;
            break;
        case 1: 
            out_TU_ES_030_[ilink]->m_1 = (int16) outputvalue;
            break;
        case 2: 
            out_TU_ES_030_[ilink]->m_2 = (int16) outputvalue;
            break;
        case 3: 
            out_TU_ES_030_[ilink]->m_3 = (int16) outputvalue;
            break;
        case 4: 
            out_TU_ES_030_[ilink]->aout_1 = (int16) outputvalue;
            break;
        case 5: 
            out_TU_ES_030_[ilink]->aout_2 = (int16) outputvalue;
            break;
    }    

    return 0;
}

/*================================================================================*/
int ec_TU_ES_030v2_read_chan(double *pvalue, int ichan, int ilink)
{
    if ( (ilink<=0) || (ilink>nTU_ES_030v2) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    switch (ichan){
        case 0:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->mstate1;
            break;
        case 1:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->count1;
            break;
        case 2:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->timestamp1;
            break;
        case 3:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->velocity1;
            break;
        case 4:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->current1;
            break;
        case 5:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->mstate2;
            break;
        case 6:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->count2;
            break;
        case 7:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->timestamp2;
            break;
        case 8:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->velocity2;
            break;
        case 9:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->current2;
            break;
        case 10:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->mstate3;
            break;
        case 11:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->count3;
            break;
        case 12:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->timestamp3;
            break;
        case 13:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->velocity3;
            break;
        case 14:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->current3;
            break; 
        case 15:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->digital;
            break;
        case 16:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->caliper1;
            break;
        case 17:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->caliper2;
            break;
        case 18:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->force1;
            break;
        case 19:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->force2;
            break;
        case 20:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->force3;
            break;
        case 21:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->pos1;
            break;
        case 22:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->pos2;
            break;
        case 23:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->pos3;
            break;
        case 24:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->analog1;
            break;
        case 25:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->analog2;
            break;
        case 26:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->linevoltage;
            break;
        case 27:
            *pvalue=(double) in_TU_ES_030v2_[ilink]->ectime;
            break;            
    }
    
    return 0;
}
/*================================================================================*/

int ec_TU_ES_030v2_write_chan(double outputvalue, int ichan, int ilink)
{
    if ( (ilink<=0) || (ilink>nTU_ES_030v2) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    switch (ichan){
        case 0: 
            out_TU_ES_030v2_[ilink]->mcom1 = (uint8) outputvalue;
            break;
        case 1: 
            out_TU_ES_030v2_[ilink]->setpoint1 = (int16) outputvalue;
            break;
        case 2: 
            out_TU_ES_030v2_[ilink]->ff1 = (int16) outputvalue;
            break;
        case 3: 
            out_TU_ES_030v2_[ilink]->mcom2 = (uint8) outputvalue;
            break;
        case 4: 
            out_TU_ES_030v2_[ilink]->setpoint2 = (int16) outputvalue;
            break;
        case 5: 
            out_TU_ES_030v2_[ilink]->ff2 = (int16) outputvalue;
            break;
        case 6: 
            out_TU_ES_030v2_[ilink]->mcom3 = (uint8) outputvalue;
            break;
        case 7: 
            out_TU_ES_030v2_[ilink]->setpoint3 = (int16) outputvalue;
            break;
        case 8: 
            out_TU_ES_030v2_[ilink]->ff3 = (int16) outputvalue;
            break;
        case 9: 
            out_TU_ES_030v2_[ilink]->digital = (uint8) outputvalue;
            break;
        case 10: 
            out_TU_ES_030v2_[ilink]->aout1 = (int16) outputvalue;
            break;
        case 11: 
            out_TU_ES_030v2_[ilink]->aout2 = (int16) outputvalue;
            break;

    }    

    return 0;
}

int ec_TU_EightWheelBot_read_chan(double *pvalue, int ichan, int ilink)
{    
    if ( (ilink<=0) || (ilink>nTU_EightWheelBot) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    switch (ichan){
        case 0:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->State;
            break;
            
        case 1:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M1_Update;
            break;
        case 2:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M1_Status;
            break;
        case 3:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M1_Current;
            break;
        case 4:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M1_Velocity;
            break;
        case 5:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M1_Position;
            break;
        case 6:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M1_Voltage;
            break;
        case 7:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M1_Variable_1;
            break;
        case 8:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M1_Variable_2;
            break;
            
        case 9:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M2_Update;
            break;
        case 10:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M2_Status;
            break;
        case 11:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M2_Current;
            break;
        case 12:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M2_Velocity;
            break;
        case 13:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M2_Position;
            break;
        case 14:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M2_Voltage;
            break;
        case 15:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M2_Variable_1;
            break;
        case 16:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M2_Variable_2;
            break;
            
        case 17:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M3_Update;
            break;
        case 18:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M3_Status;
            break;
        case 19:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M3_Current;
            break;
        case 20:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M3_Velocity;
            break;
        case 21:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M3_Position;
            break;
        case 22:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M3_Voltage;
            break;
        case 23:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M3_Variable_1;
            break;
        case 24:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M3_Variable_2;
            break;
            
        case 25:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M4_Update;
            break;
        case 26:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M4_Status;
            break;
        case 27:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M4_Current;
            break;
        case 28:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M4_Velocity;
            break;
        case 29:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M4_Position;
            break;
        case 30:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M4_Voltage;
            break;
        case 31:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M4_Variable_1;
            break;
        case 32:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M4_Variable_2;
            break;
            
        case 33:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M5_Update;
            break;
        case 34:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M5_Status;
            break;
        case 35:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M5_Current;
            break;
        case 36:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M5_Velocity;
            break;
        case 37:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M5_Position;
            break;
        case 38:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M5_Voltage;
            break;
        case 39:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M5_Variable_1;
            break;
        case 40:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M5_Variable_2;
            break;
            
        case 41:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M6_Update;
            break;
        case 42:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M6_Status;
            break;
        case 43:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M6_Current;
            break;
        case 44:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M6_Velocity;
            break;
        case 45:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M6_Position;
            break;
        case 46:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M6_Voltage;
            break;
        case 47:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M6_Variable_1;
            break;
        case 48:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M6_Variable_2;
            break;
            
        case 49:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M7_Update;
            break;
        case 50:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M7_Status;
            break;
        case 51:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M7_Current;
            break;
        case 52:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M7_Velocity;
            break;
        case 53:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M7_Position;
            break;
        case 54:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M7_Voltage;
            break;
        case 55:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M7_Variable_1;
            break;
        case 56:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M7_Variable_2;
            break;
            
        case 57:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M8_Update;
            break;
        case 58:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M8_Status;
            break;
        case 59:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M8_Current;
            break;
        case 60:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M8_Velocity;
            break;
        case 61:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M8_Position;
            break;
        case 62:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M8_Voltage;
            break;
        case 63:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M8_Variable_1;
            break;
        case 64:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->M8_Variable_2;
            break;
            
        case 65:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G1_Update;
            break;
        case 66:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G1_Temperature;
            break;
        case 67:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G1_Accell_x;
            break;
        case 68:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G1_Accell_y;
            break;
        case 69:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G1_Accell_z;
            break;
        case 70:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G1_Gyro_x;
            break;
        case 71:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G1_Gyro_y;
            break;
        case 72:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G1_Gyro_z;
            break;
            
        case 73:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G2_Update;
            break;
        case 74:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G2_Temperature;
            break;
        case 75:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G2_Accell_x;
            break;
        case 76:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G2_Accell_y;
            break;
        case 77:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G2_Accell_z;
            break;
        case 78:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G2_Gyro_x;
            break;
        case 79:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G2_Gyro_y;
            break;
        case 80:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G2_Gyro_z;
            break;
            
        case 81:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G3_Update;
            break;
        case 82:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G3_Temperature;
            break;
        case 83:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G3_Accell_x;
            break;
        case 84:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G3_Accell_y;
            break;
        case 85:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G3_Accell_z;
            break;
        case 86:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G3_Gyro_x;
            break;
        case 87:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G3_Gyro_y;
            break;
        case 88:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G3_Gyro_z;
            break;
            
        case 89:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G4_Update;
            break;
        case 90:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G4_Temperature;
            break;
        case 91:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G4_Accell_x;
            break;
        case 92:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G4_Accell_y;
            break;
        case 93:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G4_Accell_z;
            break;
        case 94:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G4_Gyro_x;
            break;
        case 95:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G4_Gyro_y;
            break;
        case 96:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G4_Gyro_z;
            break;
            
        case 97:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G5_Update;
            break;
        case 98:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G5_Temperature;
            break;
        case 99:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G5_Accell_x;
            break;
        case 100:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G5_Accell_y;
            break;
        case 101:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G5_Accell_z;
            break;
        case 102:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G5_Gyro_x;
            break;
        case 103:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G5_Gyro_y;
            break;
        case 104:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G5_Gyro_z;
            break;
            
        case 105:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G6_Update;
            break;
        case 106:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G6_Temperature;
            break;
        case 107:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G6_Accell_x;
            break;
        case 108:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G6_Accell_y;
            break;
        case 109:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G6_Accell_z;
            break;
        case 110:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G6_Gyro_x;
            break;
        case 111:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G6_Gyro_y;
            break;
        case 112:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G6_Gyro_z;
            break;
            
        case 113:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G7_Update;
            break;
        case 114:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G7_Temperature;
            break;
        case 115:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G7_Accell_x;
            break;
        case 116:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G7_Accell_y;
            break;
        case 117:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G7_Accell_z;
            break;
        case 118:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G7_Gyro_x;
            break;
        case 119:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G7_Gyro_y;
            break;
        case 120:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G7_Gyro_z;
            break;
            
        case 121:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G8_Update;
            break;
        case 122:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G8_Temperature;
            break;
        case 123:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G8_Accell_x;
            break;
        case 124:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G8_Accell_y;
            break;
        case 125:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G8_Accell_z;
            break;
        case 126:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G8_Gyro_x;
            break;
        case 127:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G8_Gyro_y;
            break;
        case 128:
            *pvalue=(double) in_TU_EightWheelBot_[ilink]->G8_Gyro_z;
            break;
    }
    
    return 0;
}
/*================================================================================*/

int ec_TU_EightWheelBot_write_chan(double outputvalue, int ichan, int ilink)
{
    if ( (ilink<=0) || (ilink>nTU_EightWheelBot) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    switch (ichan){
        case 0: 
            out_TU_EightWheelBot_[ilink]->Command = (uint16) outputvalue;
            break;
        
        case 1: 
            out_TU_EightWheelBot_[ilink]->M1_Drivemode = (uint8) outputvalue;
            break;
        case 2: 
            out_TU_EightWheelBot_[ilink]->M1_Command = (uint8) outputvalue;
            break;
        case 3: 
            out_TU_EightWheelBot_[ilink]->M1_Velocity_SP = (int16) outputvalue;
            break;
        case 4: 
            out_TU_EightWheelBot_[ilink]->M1_Current_SP = (int16) outputvalue;
            break;
            
        case 5: 
            out_TU_EightWheelBot_[ilink]->M2_Drivemode = (uint8) outputvalue;
            break;
        case 6: 
            out_TU_EightWheelBot_[ilink]->M2_Command = (uint8) outputvalue;
            break;
        case 7: 
            out_TU_EightWheelBot_[ilink]->M2_Velocity_SP = (int16) outputvalue;
            break;
        case 8: 
            out_TU_EightWheelBot_[ilink]->M2_Current_SP = (int16) outputvalue;
            break;
            
        case 9: 
            out_TU_EightWheelBot_[ilink]->M3_Drivemode = (uint8) outputvalue;
            break;
        case 10: 
            out_TU_EightWheelBot_[ilink]->M3_Command = (uint8) outputvalue;
            break;
        case 11: 
            out_TU_EightWheelBot_[ilink]->M3_Velocity_SP = (int16) outputvalue;
            break;
        case 12: 
            out_TU_EightWheelBot_[ilink]->M3_Current_SP = (int16) outputvalue;
            break;
            
        case 13: 
            out_TU_EightWheelBot_[ilink]->M4_Drivemode = (uint8) outputvalue;
            break;
        case 14: 
            out_TU_EightWheelBot_[ilink]->M4_Command = (uint8) outputvalue;
            break;
        case 15: 
            out_TU_EightWheelBot_[ilink]->M4_Velocity_SP = (int16) outputvalue;
            break;
        case 16: 
            out_TU_EightWheelBot_[ilink]->M4_Current_SP = (int16) outputvalue;
            break;
            
        case 17: 
            out_TU_EightWheelBot_[ilink]->M5_Drivemode = (uint8) outputvalue;
            break;
        case 18: 
            out_TU_EightWheelBot_[ilink]->M5_Command = (uint8) outputvalue;
            break;
        case 19: 
            out_TU_EightWheelBot_[ilink]->M5_Velocity_SP = (int16) outputvalue;
            break;
        case 20: 
            out_TU_EightWheelBot_[ilink]->M5_Current_SP = (int16) outputvalue;
            break;
            
        case 21: 
            out_TU_EightWheelBot_[ilink]->M6_Drivemode = (uint8) outputvalue;
            break;
        case 22: 
            out_TU_EightWheelBot_[ilink]->M6_Command = (uint8) outputvalue;
            break;
        case 23: 
            out_TU_EightWheelBot_[ilink]->M6_Velocity_SP = (int16) outputvalue;
            break;
        case 24: 
            out_TU_EightWheelBot_[ilink]->M6_Current_SP = (int16) outputvalue;
            break;
            
        case 25: 
            out_TU_EightWheelBot_[ilink]->M7_Drivemode = (uint8) outputvalue;
            break;
        case 26: 
            out_TU_EightWheelBot_[ilink]->M7_Command = (uint8) outputvalue;
            break;
        case 27: 
            out_TU_EightWheelBot_[ilink]->M7_Velocity_SP = (int16) outputvalue;
            break;
        case 28: 
            out_TU_EightWheelBot_[ilink]->M7_Current_SP = (int16) outputvalue;
            break;
            
        case 29: 
            out_TU_EightWheelBot_[ilink]->M8_Drivemode = (uint8) outputvalue;
            break;
        case 30: 
            out_TU_EightWheelBot_[ilink]->M8_Command = (uint8) outputvalue;
            break;
        case 31: 
            out_TU_EightWheelBot_[ilink]->M8_Velocity_SP = (int16) outputvalue;
            break;
        case 32: 
            out_TU_EightWheelBot_[ilink]->M8_Current_SP = (int16) outputvalue;
            break;
    }

    return 0;
}

/*************** Smart Wheel read-write functions ***********************/

int ec_ROPOD_SmartWheel_read_sens(double *pvalue, int ichan, int ilink)
{    
    if ( (ilink<=0) || (ilink>nROPODSmartWheel) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    switch (ichan){
        case 0:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->status1;
            break;            
        case 1:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->status2;
            break;
        case 2:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->sensor_ts;
            break;
        case 3:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->setpoint_ts;            
            break;
        /*****************/    
        case 4:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->encoder_1;
            break;
        case 5:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->velocity_1;
            break;
            
        case 6:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_1_d;
            break;
        case 7:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_1_q;
            break;
        case 8:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_1_u;
            break;            
        case 9:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_1_v;
            break;
        case 10:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_1_w;
            break;
            
        case 11:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->voltage_1;
            break;
        case 12:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->voltage_1_u;
            break;
        case 13:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->voltage_1_v;
            break;
        case 14:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->voltage_1_w;
            break;
            
        case 15:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->temperature_1;
            break;
        /*****************/    
        case 16:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->encoder_2;
            break;            
        case 17:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->velocity_2;
            break;            
            
        case 18:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_2_d;
            break;
        case 19:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_2_q;
            break;
        case 20:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_2_u;
            break;
        case 21:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_2_v;
            break;
        case 22:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->current_2_w;
            break;
            
        case 23:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->voltage_2;
            break;
        case 24:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->voltage_2_u;
            break;            
        case 25:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->voltage_2_v;
            break;
        case 26:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->voltage_2_w;
            break;
            
        case 27:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->temperature_2;            
            break;
        /*****************/                  
        case 28:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->encoder_pivot;
            break;
        case 29:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->velocity_pivot;
            break;
            
        case 30:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->voltage_bus;
            break;
            
        case 31:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->imu_ts;
            break;
            
        case 32:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->accel_x;
            break;
        case 33:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->accel_y;
            break;
        case 34:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->accel_z;
            break;
            
        case 35:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->gyro_x;
            break;
        case 36:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->gyro_y;
            break;
        case 37:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->gyro_z;
            break;
            
        case 38:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->temperature_imu;
            break;
            
        case 39:
            *pvalue=(double) sens_ROPOD_SmartWheel_t_[ilink]->pressure;
            break;            

    }
    
    return 0;
}
/*================================================================================*/

int ec_ROPOD_SmartWheel_write_cmd(double outputvalue, int ichan, int ilink)
{
    if ( (ilink<=0) || (ilink>nROPODSmartWheel) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    switch (ichan){
        case 0: 
            cmd_ROPOD_SmartWheel_t_[ilink]->command1 = (uint16_t) outputvalue;
            break;        
        case 1: 
            cmd_ROPOD_SmartWheel_t_[ilink]->command2 = (uint16_t) outputvalue;
            break;
            
        case 2: 
            cmd_ROPOD_SmartWheel_t_[ilink]->setpoint1 = (float) outputvalue;
            break;
        case 3: 
            cmd_ROPOD_SmartWheel_t_[ilink]->setpoint2 = (float) outputvalue;
            break;
            
        case 4: 
            cmd_ROPOD_SmartWheel_t_[ilink]->limit1_p = (float) outputvalue;
            break;            
        case 5: 
            cmd_ROPOD_SmartWheel_t_[ilink]->limit1_n = (float) outputvalue;
            break;
        case 6: 
            cmd_ROPOD_SmartWheel_t_[ilink]->limit2_p = (float) outputvalue;
            break;
        case 7: 
            cmd_ROPOD_SmartWheel_t_[ilink]->limit2_n = (float) outputvalue;
            break;
            
        case 8: 
            cmd_ROPOD_SmartWheel_t_[ilink]->timestamp = (float) outputvalue;
            break;
            
   
    }

    return 0;
}







/*************** Smart Wheel 2 read-write functions ***********************/

int ec_ROPOD_SmartWheel2_read_sens(double *pvalue, int ichan, int ilink)
{    
    if ( (ilink<=0) || (ilink>nROPODSmartWheel2) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    
    switch (ichan){
        case 0:
            *pvalue=(uint32_t) sens_ROPOD_SmartWheel2_t_[ilink]->abs_enc[0];
            break;     
        case 1:
            *pvalue=(uint32_t) sens_ROPOD_SmartWheel2_t_[ilink]->abs_enc[1];
            break;           
        case 2:
            *pvalue=(float) sens_ROPOD_SmartWheel2_t_[ilink]->abs_spd[0];
            break;   
            break;              
        case 3:
            *pvalue=(float) sens_ROPOD_SmartWheel2_t_[ilink]->abs_spd[1];
            break;       


    }
    
    return 0;
}
/*================================================================================*/

int ec_ROPOD_SmartWheel2_write_cmd(double outputvalue, int ichan, int ilink)
{
    if ( (ilink<=0) || (ilink>nROPODSmartWheel2) ) {
        printf("ERROR: %s:%d, incorrect link ID, could not find slave with link ID : %d\n",__FILE__,__LINE__,ilink);
        return -1;
    }
    switch (ichan){
        case 0: 
            cmd_ROPOD_SmartWheel2_t_[ilink]->enable = (uint8_t) outputvalue;
            break;     
        case 1: 
            cmd_ROPOD_SmartWheel2_t_[ilink]->c_setpoint = (float) outputvalue;
            break;     
        case 2: 
            cmd_ROPOD_SmartWheel2_t_[ilink]->v_setpoint = (float) outputvalue;
            break;                

    }

    return 0;
}





/*================================================================================*/
int ec_io(void)
{
    
    if (ec_slave[0].state == EC_STATE_OPERATIONAL )
    {
        ec_send_processdata();
        memset(ec_slave[0].inputs, 0, ec_slave[0].Ibytes);
        ec_receive_processdata(EC_TIMEOUTRET);
        return 0;
    }
    else
    {
        printf("IO not succeeded.\n");
        return -1;
    }
    
}

int ec_i(void)
{
    
    if (ec_slave[0].state == EC_STATE_OPERATIONAL )
    {
        ec_receive_processdata(EC_TIMEOUTRET);
        return 0;
    }
    else
    {
        printf("ec_receive_processdata not succeeded.\n");
        return -1;
    }
    
}

int ec_o(void)
{
    
    if (ec_slave[0].state == EC_STATE_OPERATIONAL )
    {
        ec_send_processdata();
        return 0;
    }
    else
    {
        printf("ec_send_processdata not succeeded.\n");
        return -1;
    }
    
}

int ec_initial(char* netif)
{
    int cnt;
    uint32 EL6022_ec_index[MAX_NUMBER_OF_SLAVES];
    
    printf("Starting initialization\n");
    
    /* initialise SOEM, bind socket to ifname */
    if (ec_init(netif))
    {
        
        printf("Master socket bind on %s succeeded.\n",netif);
        
        /* bit masks voor digital I/O */
        bitmask[0]=0x01;
        bitmask[1]=0x02;
        bitmask[2]=0x04;
        bitmask[3]=0x08;
        bitmask[4]=0x10;
        bitmask[5]=0x20;
        bitmask[6]=0x40;
        bitmask[7]=0x80;
        
        /* find and auto-config slaves */
        if ( ec_config(TRUE, &IOmap) > 0 )
        {
            
            printf("%d slaves found and configured.\n",ec_slavecount);
            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE);
            /* read indevidual slave state and store in ec_slave[] */
            ec_readstate();
            for(cnt = 1; cnt <= ec_slavecount ; cnt++)
            {
                printf("I found an Ethercat Slave");
                printf("Slave:%d Name:%s Output size:%3dbits Input size:%3dbits State:%2d\n",
                        cnt, ec_slave[cnt].name, ec_slave[cnt].Obits, ec_slave[cnt].Ibits, ec_slave[cnt].state);
                
                /*slave configuration*/
                
                switch (ec_slave[cnt].eep_id) {
                    case 0x044c2c52:
                        nEK1100++;
                        break;
                    case 0x03f03052:
                        in_EL1008_[nEL1008+1] = (in_EL1008t*) ec_slave[cnt].inputs;
                        nEL1008++;
                        break;
                    case 0x03fa3052:
                        in_EL1018_[nEL1018+1] = (in_EL1018t*) ec_slave[cnt].inputs;
                        nEL1018++;
                        break;
                    case 0x03f63052:
                        in_EL1014_[nEL1014+1] = (in_EL1014t*) ec_slave[cnt].inputs;
                        nEL1014++;
                        break;
                    case 0x7d43052:
                        out_EL2004_[nEL2004+1] = (out_EL2004t*) ec_slave[cnt].outputs;
                        nEL2004++;
                        break;
                    case 0x7d83052:
                        out_EL2008_[nEL2008+1] = (out_EL2008t*) ec_slave[cnt].outputs;
                        nEL2008++;
                        break;
                    case 0x09c63052:
                        out_EL2502_[nEL2502+1] = (out_EL2502t*) ec_slave[cnt].outputs;
                        nEL2502++;
                        break;
                    case 0x0bc03052:
                        in_EL3008_[nEL3008+1] = (in_EL3008t*) ec_slave[cnt].inputs;
                        nEL3008++;
                        break;
                    case 0x0c1e3052:
                        in_EL3102_[nEL3102+1] = (in_EL3102t*) ec_slave[cnt].inputs;
                        nEL3102++;
                        break;
                    case 0x10243052:
                        out_EL4132_[nEL4132+1] = (out_EL4132t*) ec_slave[cnt].outputs;
                        nEL4132++;
                        break;
                    case 0x0fc63052:
                        out_EL4038_[nEL4038+1] = (out_EL4038t*) ec_slave[cnt].outputs;
                        nEL4038++;
                        break;
                    case 0x13ed3052:
                        out_EL5101_[nEL5101+1] = (out_EL5101t*) ec_slave[cnt].outputs;
                        in_EL5101_[nEL5101+1] = (in_EL5101t*) ec_slave[cnt].inputs;
                        pEL5101_data[nEL5101+1] = (encoder_data_t*) calloc(1, sizeof(encoder_data_t));
                        nEL5101++;
                        break;
                    case 0x14203052:
                        out_EL5152_[nEL5152+1] = (out_EL5152t*) ec_slave[cnt].outputs;
                        in_EL5152_[nEL5152+1] = (in_EL5152t*) ec_slave[cnt].inputs;
                        pEL5152_data[nEL5152+1] = (encoder_data_t*) calloc(1, sizeof(encoder_data_t));
                        nEL5152++;
                        break;
                    case 0x0904001:
                        out_TU0001_[nTU0001+1] = (out_TU0001t*) ec_slave[cnt].outputs;
                        in_TU0001_[nTU0001+1] = (in_TU0001t*) ec_slave[cnt].inputs;
                        nTU0001++;
                        break;
                    case 0x17863052:
                        EL6022_ec_index[nEL6022] = cnt;
                        nEL6022++;
                        break;
                        /*
                    case 0x001630df:eep_id
                        out_TU_ES_030_[nTU_ES_030+1] = (out_TU_ES_030t*) ec_slave[cnt].outputs;
                        in_TU_ES_030_[nTU_ES_030+1] = (in_TU_ES_030t*) ec_slave[cnt].inputs;
                        nTU_ES_030++;
                        break;
                         */
                    case 0x001630df:
                        out_TU_ES_030v2_[nTU_ES_030v2+1] = (out_TU_ES_030v2t*) ec_slave[cnt].outputs;
                        in_TU_ES_030v2_[nTU_ES_030v2+1] = (in_TU_ES_030v2t*) ec_slave[cnt].inputs;
                        nTU_ES_030v2++;
                        break;
                    case 0x01601005:
                        out_TU_EightWheelBot_[nTU_EightWheelBot+1] = (out_TU_EightWheelBot_t*) ec_slave[cnt].outputs;
                        in_TU_EightWheelBot_[nTU_EightWheelBot+1] = (in_TU_EightWheelBot_t*) ec_slave[cnt].inputs;
                        nTU_EightWheelBot++;
                        break;
                    case 0x01705011:
                        /* Cesar Lopez: Here the convention seems to be that output refers to data transmitted to the EC device 
                            and input to data received from the EC device*/
                        printf("I found a smartwheel!");
                        cmd_ROPOD_SmartWheel_t_[nROPODSmartWheel+1] = (cmd_ROPOD_SmartWheel_t*) ec_slave[cnt].outputs;
                        sens_ROPOD_SmartWheel_t_[nROPODSmartWheel+1] = (sens_ROPOD_SmartWheel_t*) ec_slave[cnt].inputs;
                        nROPODSmartWheel++;    
                        break;
                    case 0x01701001: // SMartwheel 2
                        /* Cesar Lopez: Here the convention seems to be that output refers to data transmitted to the EC device 
                            and input to data received from the EC device*/
                        cmd_ROPOD_SmartWheel2_t_[nROPODSmartWheel2+1] = (cmd_ROPOD_SmartWheel2_t*) ec_slave[cnt].outputs;
                        sens_ROPOD_SmartWheel2_t_[nROPODSmartWheel2+1] = (sens_ROPOD_SmartWheel2_t*) ec_slave[cnt].inputs;
                        nROPODSmartWheel2++;    
                        break;                        
                    default:
                        printf("Unregistered slave detected\n");
                        break;
                }
            }
            if(nEL6022 > 0){
                RS485EcOpen(EL6022_ec_index,nEL6022);
            }
        }
        else
        {
            printf("No EtherCAT slaves found. Are slaves connected to %s port?\n",netif);
            return -1;
        }
    }    
    else
    {
        printf("Master socket bind on %s NOT succeeded.\n",netif);
        return -1;
    }
    
    return 0;
}


int ec_scan(void)
{
    
    printf("Request operational state for all slaves\n");
    ec_slave[0].state = EC_STATE_OPERATIONAL;
    /* request OP state for all slaves */
    ec_writestate(0);
    /* wait for all slaves to reach OP state */
    ec_statecheck(0, EC_STATE_OPERATIONAL,  EC_TIMEOUTSTATE);
    
    if (ec_slave[0].state == EC_STATE_OPERATIONAL )
    {
        return 0;
    }
    else
    {
        printf("Not all slaves reached operational state.\n");
        ec_slave[0].state = EC_STATE_SAFE_OP;
        /* request safe state for all slaves */
        ec_writestate(0);
        return -1;
    }
    
}


int ec_exit(void)
{
    int i = 0;
    
    printf("Request safe operational state for all slaves\n");
    ec_slave[0].state = EC_STATE_SAFE_OP;
    /* request SAFE_OP state for all slaves */
    ec_writestate(0);
    
    printf("End SOEM, close socket\n");
    /* stop SOEM, close socket */
    ec_close();
    
    /* free encoder data */
    for(i=1;i<=nEL5152;++i){
        free(pEL5152_data[i]);
    }
    for(i=1;i<=nEL5101;++i){
        free(pEL5101_data[i]);
    }
    return 0;
}

int ec_getstate(void)
{
    return ec_slave[0].state;
}
