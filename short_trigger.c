/* Red Pitaya external trigger pulse generation Example */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rp.h"

int a;


int main(int argc, char **argv){

	/*size of the acquisition buffer*/
	uint32_t buff_size = 16384;

	/* Print error, if rp_Init() function failed */
	if(rp_Init() != RP_OK){
		fprintf(stderr, "Rp api init failed!\n");
	}
	
	/*define wave form*/
	rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);

	/*define frequency*/
	rp_GenFreq(RP_CH_1, 20000);

	/*define amplitude between 0 and 1*/
	rp_GenAmp(RP_CH_1, 0.5);

	/*define generation mode*/
	rp_GenMode(RP_CH_1, RP_GEN_MODE_BURST);

	/*define cycle number*/
	rp_GenBurstCount(RP_CH_1, 2);

 
	/* decimation n (=1,8...) : frequency = 125/n MHz*/
	rp_AcqSetDecimation(8);

	/*level of the trigger activation in volt*/
	rp_AcqSetTriggerLevel(1); 

	/*acquisition trigger delay*/
	rp_AcqSetTriggerDelayNs(0);


	for (int i=0 ; i<10 ; i++) {

		/*allocation of buffer size in memory*/
		float *buff = (float *)malloc(buff_size * sizeof(float));

		/*start acquisition must be set before trigger initiation*/
		rp_AcqStart();

		/*trigger source*/
		rp_AcqSetTriggerSrc(RP_TRIG_SRC_EXT_PE);

		/*init trigger state*/
		rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;


		/*waiting for trigger*/
		while(1){
			rp_AcqGetTriggerState(&state);
			if(state == RP_TRIG_STATE_TRIGGERED){
				break;
			}
		}

		/*generation start*/
		rp_GenOutEnable(RP_CH_1);

		/*putt acquisition in buffer*/
		rp_AcqGetOldestDataV(RP_CH_1, &buff_size, buff);

		/*release memory*/
		free(buff);
	}

	/* Release rp resources */
	rp_Release();

	return 0;
}