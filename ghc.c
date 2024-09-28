
/*
* @brief Serial: 85048a62
* @file ghc.c
  @author: Paul Moggach
  @author: Devansh Patel
  @since: 2024-04-13
  @param: main function
  @return: void
*/

#include "ghcontrol.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep function
#include <string.h>

int main(void)
{
    int logged;
	setpoint_s sets = {0};
	control_s ctrl = {0};
	reading_s creadings = {0};
	alarmlimit_s alimits = { 0 };
    //alarm_s warn[NALARMS];

    alarm_s * arecord;
    arecord = (alarm_s *) calloc(1,sizeof(alarm_s));
    if(arecord == NULL)
    {
        fprintf(stderr,"\nCannot allocate memory\n");
        return EXIT_FAILURE;
    }

	GhControllerInit();
	struct fb_t *fb;
	fb = ShInit(fb);

	while(1)
	{
        logged = GhLogData("ghdata.txt", creadings);
	    sets = GhSetTargets();
	    alimits = GhSetAlarmLimits();
		creadings = GhGetReadings(); //commented out
		ctrl = GhSetControls(sets, creadings);
		arecord = GhSetAlarms(arecord, alimits, creadings);
		GhDisplayAll (creadings, sets, fb);
		GhDisplayReadings(creadings);
		GhDisplayTargets(sets);
		GhDisplayControls(ctrl);
		GhDisplayAlarms(arecord);
		GhDelay(GHUPDATE);
	}
	//fprintf(stdout,"Press ENTER to continue...");
	//fgetc(stdin);

	return EXIT_SUCCESS;
}




