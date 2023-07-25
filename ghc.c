/** @brief serial:10000000962290dd
 *  @file lab04.c
 */

#include "ghcontrol.h"

int main(void)
{

	time_t now;
	 reading_s creadings = {0};
         control_s ctrl = {0};
	 setpoint_s sets = {0};
 	srand(time(NULL));
	int logged=0;

        sets=GhSetTargets();
	GhControllerInit();
	while(1)
	{

		now = time(NULL);
		 reading_s GhGetReadings(void);
		creadings = GhGetReadings();
logged= GhLogData("ghdata.txt",creadings);
	ctrl=GhSetControls(sets,creadings);
	GhDisplayAll(creadings,sets);
		GhDisplayReadings(creadings);
		GhDisplayTargets(sets);
		GhDisplayControls(ctrl);
		GhDelay(GHUPDATE);
	}

	fprintf(stdout,"Press ENTER to continue...");
     	fgetc(stdin);


	return EXIT_FAILURE;
}


