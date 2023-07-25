/** @brief Gh control functions
 * @file ghcontrol.c
 */
#include "ghcontrol.h"
#include "sensehat.h"
#include <cstring>
#include <string.h>

#if SENSEHAT
SenseHat Sh;
#endif

/** Prints Gh Controller Title
 * @version CENG153, serial:10000000962290dd
 * @author Paul Moggach
 * @author zeel patel
 * @since 2021-05-07
 * @param sname string with Operator's name
 * @return void
 */
void GhDisplayHeader(const char * sname)
{
	fprintf(stdout,"%s's CENG153 Greenhouse Controller\n",sname);
}

/**Retrieves the serial number of the Greenhouse Controller.
 * @version CENG153, serial: 10000000962290dd
 * @author Paul Moggach
 * @author Zeel Patel
 * @since 2021-05-07
 * @return The serial number as a 64-bit unsigned integer.
 */
u_int64_t GhGetSerial(void)
{
	static u_int64_t serial =0;
	FILE * fp;
	char buf[SYSINFOBUFSZ];
	char searchstring[] = SEARCHSTR;
	fp = fopen ("/proc/cpuinfo", "r");
	if (fp !=NULL)
	  {
	      while (fgets(buf, sizeof(buf), fp) != NULL)
	              {
                      if (!strncasecmp(searchstring, buf,strlen(searchstring)))
	                       {
				                   sscanf(buf+strlen(searchstring),"%Lx",&serial);
                         }
	               }
	      fclose(fp);
	  }
    if(serial==0)
	  {
        system("uname -a");
        system("ls --fu /usr/lib/codeblocks | grep -Po '\\.\\K[^ ]+'>stamp.txt")	;
        fp = fopen ("stamp.txt","r");
        if (fp !=NULL)
                {
	                   while (fgets(buf, sizeof(buf), fp) !=NULL)
			     {
                         sscanf(buf, "%Lx", &serial);
			     }
     fclose(fp);
	            }
	}
	return serial;
}
/**Generates a random integer within the given range.
 * @version CENG153, serial: 10000000962262290dd
 * @author Paul Moggach
 * @since 2021-05-07
 * @param range The range of random numbers (exclusive upper bound).
 * @return A random integer within the range.
 */
int GhGetRandom(int range){
	return rand() % range;
	}
/**Delays the execution for the specified number of milliseconds.
 * @version CENG153, serial: 10000000962290dd
 * @author Paul Moggach
 * @author Zeel Patel
 * @since 2021-05-07
 * @param milliseconds The number of milliseconds to delay.
 * @return void
 */
void GhDelay(int milliseconds){
	long wait;
	clock_t now,start;
	wait = milliseconds*(CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;
	while((now-start)<wait)
	{
		now = clock();
	}

	}

/**Initializes the Greenhouse Controller.
 * @version CENG153, serial: 10000000962290dd
 * @author Zeel Patel
 * @since 2021-05-07
 * @return void
 */

void GhControllerInit(void)
{


	GhDisplayHeader("ZEEL PATEL");
	srand((unsigned) time(NULL));
	GhSetTargets();
          
}

/**Displays the targets of the Greenhouse Controller.
 * @version CENG153, serial: 10000000962290dd
 * @author Zeel Patel
 * @since 2021-05-07
 * @return void
 */
void GhDisplayTargets( setpoint_s spts)
{
	fprintf(stdout," Setpoints\tT: %5.1fC \tH: %5.1f%%\n",spts.temperature,spts.humidity);
}




/** Get the current readings for temperature, humidity, and pressure
 * @version CENG153, serial: 10000000962290dd
 * @author Paul Moggach
 * @author zeel patel
 * @since 2023-06-13
 * @param void
 * @return a struct containing the current readings for temperature, humidity, and pressure
*/
reading_s GhGetReadings(void)
{
          reading_s now = {0};

           now.rtime = time(NULL);
           now.temperature = GhGetTemperature();
           now.humidity = GhGetHumidity();
           now.pressure = GhGetPressure();
        
   	   return now;
}

/**Retrieves the humidity reading.
 * @version CENG153, serial: 10000000962290dd
 * @author Zeel Patel
 * @since 2021-05-07
 * @return The humidity reading.
 */
float GhGetHumidity(void){
#if SIMHUMIDITY

	return GhGetRandom(USHUMID-LSHUMID) + LSHUMID;
#else
return Sh.GetHumidity();
#endif
}
/**Retrieves the pressure reading.
 * @version CENG153, serial:10000000962290dd
 * @author Zeel Patel
 * @since 2021-05-07
 * @return The pressure reading.
 */
float GhGetPressure(void){
#if SIMPRESSURE
	return GhGetRandom(USPRESS-LSPRESS) + LSPRESS;
#else
	return Sh.GetPressure();
#endif
}
/**Retrieves the te)mperature reading.
 * @version CENG153, serial: 10000000962290dd
 * @author Zeel Patel
 * @since 2021-05-07
 * @return The temperature reading.
 */
float GhGetTemperature(void){
	#if SIMTEMPERATURE
	return GetRandom(USTEMP-LSTEMP)+LSTEMP;
#else
	return Sh.GetTemperature();
#endif
}

/** Display the greenhouse controls.
 * @version CENG153, serial: 10000000962260dd
 * @author Paul Moggach
 * @author zeel patel
 * @since 2023-06-23
 * @param ctrl The struct containing the control values for heater and humidifier
 * @return void
 */


void GhDisplayControls( control_s ctrl){
        fprintf(stdout," Controls \tHeater:%d \tHumidifier:%d\n",ctrl.heater,ctrl.humidifier);
}

/** Display the greenhouse sensor readings
 * @version CENG153, serial: 10000000962260dd
 * @author Paul Moggach
 * @author zeel patel
 * @since 2023-06-23
 * @param rdata The struct containing the current readings for temperature, humidity, and pressure

 * @return void
 */

void GhDisplayReadings( reading_s rdata)
{
         fprintf(stdout,"\n%s Readings\tT: %5.1fC\tH: %5.1f%%\tP: %6.1fmB\n",ctime(&rdata.rtime),rdata.temperature,rdata.humidity,rdata.pressure);
}


/** Set the target values for temperature and humidity
 * @version CENG153, serial: 10000000962290dd
 * @author Paul Moggach
 * @author zeel patel
 * @since 2023-06-23
 * @param void
 * @return a struct containing the target values for temperature and humidity
*/

setpoint_s GhSetTargets(void)
{  
	 setpoint_s cpoints;
	cpoints=GhRetrieveSetpoints("setpoints.dat");
	if(cpoints.temperature==0){
	cpoints.temperature = STEMP;
	cpoints.humidity = SHUMID;
	GhSaveSetpoints("setpoints.dat",cpoints);
	}
	return cpoints;
}

/** Set the controls for heating and humidification based on target values and current readings
 * @version CENG153, serial: 10000000962290dd
 * @author Paul Moggach
 * @author zeel patel
 * @since 2023-06-23
 * @param target The struct containing the target values for temperature and humidity
 * @param rdata The struct containing the current readings for temperature and humidity
 * @return a struct containing the control values for the heater and humidifier
*/

 control_s GhSetControls(setpoint_s target, reading_s rdata)
{
	  control_s cset;
	 if(rdata.temperature < target.temperature)
	 {	 
		 cset.heater = ON;
	 }
         else
	 {
		 cset.heater = OFF;
	 }

         if(rdata.humidity < target.humidity)
	 {
		 cset.humidifier = ON;
	 }
         else 
	 {
		 cset.humidifier = OFF;
	 }

	return cset;
}

int GhLogData(const char *fname, struct readings ghdata)
{


FILE *fp;
char ltime[CTIMESTRSZ];
fp=fopen(fname,"a");
if(fp == NULL){

return 0;
}

strcpy(ltime,ctime(&ghdata.rtime));

ltime[3]=',';
ltime[7]=',';
ltime[10]=',';
ltime[19]=',';

fprintf(fp,"\n%.24s,%5.1f,%5.1f,%6.1f",ltime,ghdata.temperature,ghdata.humidity,ghdata.pressure);
fclose(fp);
return 1;
}

int GhSaveSetpoints(const char * fname, setpoint_s spts){

FILE *fp;
fp=fopen(fname,"w");
if (fp==NULL){
	return 0;}
else{
fwrite(&spts,sizeof(spts),1,fp);
}
fclose(fp);
return 1;
}

 setpoint_s GhRetrieveSetpoints(const char * fname){

 setpoint_s spts ={0.0};
FILE *fp;
fp=fopen(fname,"r");
if(fp==NULL){


return spts;
}
else {
fread(&spts,sizeof(spts),1,fp);
}
fclose(fp);
return spts;


}

int GhSetVerticalBar(int bar ,COLOR_SENSEHAT pxc,uint8_t value){
	int i;
	if(value>7){value=7;}
	if(bar>=0&& bar<8&& value>=0 &&value<=8)
	{
	
		for(i=0;i<=value;i++)
		{
			Sh.LightPixel(i,bar,pxc);
		}
		for(i=value+1;i<8;i++)
		{
			Sh.LightPixel(i,bar,BLACK);
		}

		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
void GhDisplayAll( reading_s rd, setpoint_s sd){
	int rv,sv,avh,avl;

	Sh.WipeScreen();
	rv =(int) (8.0 * (((rd.temperature-LSTEMP) / (USTEMP-LSTEMP))+0.05))-1.0;
	sv =(int) (8.0 * (((sd.temperature-LSTEMP) / (USTEMP-LSTEMP))+0.05))-1.0;
	GhSetVerticalBar(TBAR,GREEN,rv);
	Sh.LightPixel(sv,TBAR,MAGENTA);

	rv =(int) (8.0 * (((rd.humidity-LSHUMID) / (USHUMID-LSHUMID))+0.05))-1.0;
	sv =(int) (8.0 * (((sd.humidity-LSHUMID) / (USHUMID-LSHUMID))+0.05))-1.0;
	GhSetVerticalBar(HBAR,GREEN,rv);
	Sh.LightPixel(sv,HBAR,MAGENTA);

	rv =(int) (8.0 * (((rd.pressure-LSPRESS) / (USPRESS-LSPRESS))+0.05))-1.0;
			GhSetVerticalBar(PBAR,GREEN,rv);
			}

