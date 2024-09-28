/** @brief Gh control functions
*   @file ghcontrol.c
*/

#include "ghcontrol.h"

// Alarm Message Array
/**
 * @brief Array of alarm names corresponding to alarm codes
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 */

const char alarmnames[NALARMS][ALARMNMSZ] = {"No Alarms","High Temperature","Low Temperature","High Humidity", "Low Humidity","HighPressure","Low Pressure"};

/**
 * @brief Logs sensor data to a file
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param fname File name to log the data
 * @param ghdata Sensor readings for temperature, humidity, and pressure
 * @return int Returns 1 if data is successfully logged, 0 if file cannot be opened
 */
int GhLogData(char * fname, reading_s ghdata)
{
    FILE *fp;
    char ltime[CTIMESTRSZ];

    fp = fopen(fname,"a");
    if(fp == NULL)
    {
        fprintf(stderr,"\nCan't open file, data not retrieved!\n");
        return 0;
    }

    strcpy(ltime, ctime(&ghdata.rtime));
    ltime[3] = ',';
    ltime[7] = ',';
    ltime[10] = ',';
    ltime[19] = ',';

    fprintf(fp,"\n%.24s,%5.1lf,%5.1lf,%6.1lf",ltime,ghdata.temperature,ghdata.humidity,ghdata.pressure);
    fclose(fp);
    return 1;
}

/** Prints Gh Controller Title
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param sname string with Operator's name
 * @return void
*/
void GhDisplayHeader(const char * sname)
{
	fprintf(stdout, "%s's CENG153 Greenhouse Controller\n", sname);
}

/** Prints a random number
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param range printing random number
 * @return void
 */
int GhGetRandom(int range)
{
	return rand() % range;
}

/**
 * @brief Delays the program execution for the specified number of milliseconds
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param milliseconds An integer representing the duration of the delay in milliseconds
 * @return void
 */
void GhDelay(int milliseconds)
{
	long wait;
	clock_t now, start;

	wait = milliseconds * (CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;
	while( (now - start) < wait)
	{
		now = clock();
	}
}

/**
 * @brief Initializes the Greenhouse Controller
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @return void
 */
void GhControllerInit()
{
	srand((unsigned) time(NULL));
	GhDisplayHeader("Devansh Nileshkumar Patel");
}

/**
 * @brief Displays the current control settings for the heater and humidifier
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param heater A pointer to an integer representing the state of the heater (ON/OFF)
 * @param humidifier A pointer to an integer representing the state of the humidifier (ON/OFF)
 * @return void
 */
void GhDisplayControls(control_s cset)
{
	fprintf(stdout, " Controls\tHeater: %d\tHumidifier: %d\n", cset.heater ,cset.humidifier);
	//fprintf(stderr,"\n%s not implemented,\n","GhDisplayControls");
}

/**
 * @brief Displays the current sensor readings
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param structure to set target points
 * @param structure containing the sensor readings
 * @return void
 */
void GhDisplayReadings(reading_s rdata)
{
	fprintf(stdout, "\nUnit: %LX %s Readings\tT: %5.1fC\tH: %5.1f%\tP: %6.1fmb\n", ShGetSerial (), ctime(&rdata.rtime), rdata.temperature, rdata.humidity, rdata.pressure);
}
/**
 * @brief Displays the current sensor readings
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @return a structure containing temperature and humidity
 */
setpoint_s GhSetTargets()
{
    setpoint_s cpoints = GhRetrieveSetpoints("setpoints.dat");
    if(cpoints.temperature == 0)
    {
        cpoints.temperature = STEMP;
        cpoints.humidity = SHUMID;
    }
    return cpoints;
}

/**
 * @brief Sets the control values for the heater and humidifier based on sensor readings
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param A structure to store the values
 * @param a structure to set the points of temperature and humidity
   @param A structure for storing the reading
 * @return void
 */
control_s GhSetControls(setpoint_s target,reading_s rdata)
{
	control_s cset = {0};
	if (rdata.temperature < target.temperature){
		cset.heater = ON;
	}
	else {
		cset.heater = OFF;
		}
	if (rdata.humidity < target.humidity){
		cset.humidifier = ON;
	}
	else {
		cset.humidifier = OFF;
	}

	return cset;
}

void GhDisplayTargets(setpoint_s spts){
	fprintf(stdout," Setpoints\tT: %5.1lfC\tH: %5.1lf%\n",spts.temperature, spts.humidity);
}

/**
 * @brief Retrieves the humidity reading
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @return A float representing the humidity reading
 */
float GhGetHumidity(void)
{
#if SIMHUMIDITY
    return(float)GhGetRandom(USHUMID - LSHUMID + 1) + LSHUMID;
#else
	return ShGetHumidity();
#endif
}

/**
 * @brief Retrieves the pressure reading
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @return A float representing the pressure reading
 */
float GhGetPressure()
{
#if SIMPRESSURE
    return(float)GhGetRandom(USPRESS - LSPRESS + 1) + LSPRESS;
#else
	return ShGetPressure();
#endif
}

/**
 * @brief Retrieves the temperature reading
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @return A float representing the temperature reading
 */
float GhGetTemperature()
{
#if SIMTEMPERATURE
    return(float)GhGetRandom(USTEMP - LSTEMP + 1) + LSTEMP;
#else
    return ShGetTemperature();
#endif
}

/**
 * @brief Retrieves sensor readings for temperature, humidity, and pressure
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param readings a structure of floats to store the sensor readings
 * @return void
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

/**
 * @brief saves setpoints
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param integer to save setpoints
 * @return character for fname and struct for setpoints spts
 */

int GhSaveSetpoints(char * fname,setpoint_s spts)
{
    FILE *fp;

    fp = fopen(fname,"w");
    if(fp == NULL)
    {
        return 0;
    }

    fwrite(&spts, sizeof(spts), 1, fp);

    fclose(fp);
    return 1;
}

/**
 * @brief Retrives setpoints
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param struct representing setpoints
 * @return character having pointer fname
 */
setpoint_s GhRetrieveSetpoints(char * fname)
{
    setpoint_s spts = {0};
    FILE *fp;

    fp = fopen(fname,"r");
    if(fp == NULL)
    {
        return spts;
    }

    fread(&spts, sizeof(spts), 1, fp);
    fclose(fp);

    return spts;
}

/**
 * @brief sets vertical bar
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
   @param column's bar number from 0 - 7
 * @param pxc color of pixels
   @param struct fb framebuffer structure for matrix
   @param value presenting height
 * @return EXIT_SUCCESS if it successful otherwise EXIT_FAILURE
 */
int GhSetVerticalBar(int bar, COLOR_SENSEHAT pxc,uint8_t value, struct fb_t *fb)
{
	int i;

	if (value > 7){
		value = 7;
	}
	if (bar <0 || bar>= 8 || value <0 || value >7) {
		return EXIT_FAILURE;
	}
	for ( i = 0; i<= value;i++) {
		ShLightPixel (i, bar, pxc , fb);
	}
	for ( i = value +1 ; i< 8; i++) {
		ShLightPixel (bar, 1 , BLACK , fb);
	}

	return EXIT_SUCCESS;
}

/**
 * @brief Displays data on LED matrix
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
   @param struct rd sensor readings of temperature, humidity and pressure
 * @param struct setpoints sd for temperature and humidity
   @param struct fb Framebuffer for LED matrix
 * @return void
 */

void GhDisplayAll (reading_s rd, setpoint_s sd, struct fb_t *fb) {
	int rv, sv, avh , avl;
	COLOR_SENSEHAT pxc;

	ShWipeScreen(BLACK,fb);
    rv = (int)(8.0 * (((rd.temperature-LSTEMP) / (USTEMP-LSTEMP))+0.05))-1.0;
	pxc = GREEN;
	GhSetVerticalBar(TBAR, pxc ,rv, fb);

	sv = (int)(8.0 * (((rd.humidity-LSHUMID) / (USHUMID-LSHUMID))+0.05))-1.0;
	pxc = GREEN;
	GhSetVerticalBar(HBAR, pxc ,sv, fb);

	avh = (int)(8.0 * (((rd.pressure-LSPRESS) / (USPRESS-LSPRESS))+0.05))-1.0;
	pxc = GREEN;
	GhSetVerticalBar(PBAR, pxc ,avh, fb);

	sv = (int)(8.0 * (((sd.temperature-LSTEMP) / (USTEMP-LSTEMP))+0.05))-1.0;
	pxc = MAGENTA;
	ShLightPixel(sv,TBAR, pxc, fb);

	sv = (int)(8.0 * (((sd.humidity-LSHUMID) / (USHUMID-LSHUMID))+0.05))-1.0;
	pxc = MAGENTA;
	ShLightPixel(sv, HBAR , pxc, fb);
}

/**
 * @brief Sets the alarm limits for temperature, humidity, and pressure
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @return alarmlimit_s Returns the structure containing set alarm limits
 */

alarmlimit_s GhSetAlarmLimits()
{
    alarmlimit_s calarm = {0};
    calarm.hight = UPPERATEMP;
    calarm.highh = UPPERAHUMID;
    calarm.highp = UPPERAPRESS;
    calarm.lowt = LOWERATEMP;
    calarm.lowh = LOWERAHUMID;
    calarm.lowp = LOWERAPRESS;
    return calarm;
}

/**
 * @brief Displays active alarms
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param alarm_s alrm[NALARMS] Array of alarms to display
 * @return void
 */

void GhDisplayAlarms(alarm_s * head)
{
    alarm_s *cur = head;
    printf("\nAlarms\n");
    while(cur != NULL)
    {
        if(cur->code == HTEMP)
        {
            printf("High Temperature Alaram: %s",ctime(&cur->atime));
        }
        else if(cur->code == LTEMP)
        {
            printf("Low Temperature Alaram: %s",ctime(&cur->atime));
        }
        else if(cur->code == HHUMID)
        {
            printf("High Humidity Alaram: %s",ctime(&cur->atime));
        }
        else if(cur->code == LHUMID)
        {
            printf("High Humidity Alaram: %s",ctime(&cur->atime));
        }
        else if(cur->code == HPRESS)
        {
            printf("High Pressure Alaram: %s",ctime(&cur->atime));
        }
        else if(cur->code == LPRESS)
        {
            printf("Low Pressure Alaram: %s",ctime(&cur->atime));
        }
        cur=cur->next;
    }
}

/**
 * @brief Sets one alarm
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param alarm_e code Alarm code to set
 * @param time_t atime Alarm time
 * @param double value Alarm value
 * @param alarm_s head Pointer to the head of the alarm list
 * @return int Returns 1 if alarm set successfully, 0 otherwise
 */

int GhSetOneAlarm(alarm_e code,time_t atime,double value,alarm_s * head)
{
    alarm_s *cur = head;
    alarm_s *last = NULL;

    if(cur->code == NOALARM)
    {
        cur->code = code;
        cur->atime = atime;
        cur->value = value;
        cur->next = NULL;
        return 1;
    }

    while(cur != NULL)
    {
        if(cur->code == code)
        {
            return 0;
        }

        last = cur;
        cur=cur->next;
    }
    cur = (alarm_s *) calloc(1,sizeof(alarm_s));
    last->next = cur;

    cur->code = code;
    cur->atime = atime;
    cur->value = value;
    cur->next = NULL;
    return 1;
}

/**
 * @brief Clears one alarm
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param alarm_e code Alarm code to clear
 * @param alarm_s head Pointer to the head of the alarm list
 * @return alarm_s* Returns the updated head of the alarm list
 */

alarm_s * GhClearOneAlarm(alarm_e code,alarm_s * head)
{
    alarm_s *cur = head;
    alarm_s *last = head;

    if(cur->code == code && cur->next == NULL)
    {
        cur->code = NOALARM;
        return head;
    }

    if(cur->code == code && cur->next != NULL)
    {
        head = cur->next;
        free(cur);
        return head;
    }

    while(cur != NULL)
    {
        if(cur->code == code)
        {
            last->next = cur->next;
            free(cur);
            return head;
        }
        last = cur;
        cur = cur->next;
    }

    return head;
}

/**
 * @brief Sets alarms based on given alarm limits and reading data
 * @version CENG153, serial: 85048a62
 * @author Paul Moggach
 * @author Devansh Patel
 * @since 2024-04-13
 * @param alarm_s head Pointer to the head of the alarm list
 * @param alarmlimit_s alarmpt Alarm limits to compare with reading data
 * @param reading_s rdata Reading data to check against alarm limits
 * @return alarm_s* Returns the updated head of the alarm list
 */

alarm_s * GhSetAlarms(alarm_s * head,alarmlimit_s alarmpt, reading_s rdata)
{
    alarm_s *cur = head;
    while(cur != NULL)
    {
        cur->code = NOALARM;
        cur = cur->next;
    }

    if(rdata.temperature >= alarmpt.hight)
    {
        GhSetOneAlarm(HTEMP, rdata.rtime, rdata.temperature, head);
    }
    else
    {
        head = GhClearOneAlarm(HTEMP, head);
    }

    if(rdata.temperature <= alarmpt.lowt)
    {
        GhSetOneAlarm(LTEMP, rdata.rtime, rdata.temperature, head);
    }
    else
    {
        head = GhClearOneAlarm(LTEMP, head);
    }

    if(rdata.humidity >= alarmpt.highh)
    {
        GhSetOneAlarm(HHUMID, rdata.rtime, rdata.humidity, head);
    }
    else
    {
        head = GhClearOneAlarm(HHUMID, head);
    }

    if(rdata.humidity <= alarmpt.lowh)
    {
        GhSetOneAlarm(LHUMID, rdata.rtime, rdata.humidity, head);
    }
    else
    {
        head = GhClearOneAlarm(LHUMID, head);
    }

    if(rdata.pressure >= alarmpt.highp)
    {
        GhSetOneAlarm(HPRESS, rdata.rtime, rdata.pressure, head);
    }
    else
    {
        head = GhClearOneAlarm(HPRESS, head);
    }

    if(rdata.pressure <= alarmpt.lowp)
    {
        GhSetOneAlarm(LPRESS, rdata.rtime, rdata.pressure, head);
    }
    else
    {
        head = GhClearOneAlarm(LPRESS, head);
    }

    return head;
}


