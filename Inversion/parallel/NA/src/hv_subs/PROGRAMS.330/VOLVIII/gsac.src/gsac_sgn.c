#include	<stdio.h>
#include	"gsac_docommand.h"
#include        "gsac.h"
#include        "gsac_plot.h"
#include        "gsac_sac.h"
#include        "gsac_arg.h"
#include        "gsac_sachdr.h"

extern struct sacfile_ *sacdata;
extern int *sortptr;



void gsac_set_param_sgn(int ncmd, char **cmdstr)
{
}

void gsac_exec_sgn(void)
{
	int i, k, npts, ntrc;
	float depmax, depmin, depmen;
	int indmax, indmin;
	float x;

	/* only apply the filtering if permitted */

	/* if there are no traces return */
	ntrc = gsac_control.number_itraces;
	if(ntrc < 1)
		return;

	/* process the traces */
	for ( k=0 ; k < ntrc ; k ++){
		npts = sacdata[k].sachdr.ihdr[H_NPTS];
		if(npts > 0){
			for(i=0; i < npts ; i++){
				x = sacdata[k].sac_data[i];
				if(x >= 0)
					sacdata[k].sac_data[i] =  1.0;
				else
					sacdata[k].sac_data[i] = -1.0;
			}
			/* get the DFT */
			/* redetermine the depmax depmin depmen */
			getmxmn(sacdata[k].sac_data, npts,&depmax, &depmin, &depmen,&indmax,&indmin);
			sacdata[k].sachdr.rhdr[H_TIMMAX] = sacdata[k].sachdr.rhdr[H_B]  + ( indmax)*sacdata[k].sachdr.rhdr[H_DELTA] ;
			sacdata[k].sachdr.rhdr[H_TIMMIN] = sacdata[k].sachdr.rhdr[H_B]  + ( indmin)*sacdata[k].sachdr.rhdr[H_DELTA] ;
			sacdata[k].sachdr.rhdr[H_DEPMIN] = depmin;
			sacdata[k].sachdr.rhdr[H_DEPMAX] = depmax;
			sacdata[k].sachdr.rhdr[H_DEPMEN] = depmen;
		}
	}
}
