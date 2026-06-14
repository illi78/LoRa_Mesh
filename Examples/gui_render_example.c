#include "gui_render.h"
#include "gui_menu.h"
#include "gui_types.h"
#include "ST7735.h"

#include "rtc.h"

#include "ScreenSaver.h"


/*************************************************
 * @brief  screen saver, home screen
 * @param  None
 * @retval None
 *
 *************************************************/
void Form_HomeScreen (Menu *menu)
{
	ST7735_DrawImage(0, 0, ST7735_WIDTH ,ST7735_HEIGHT, *IMAGE_HOME_SCREEN );
}


/*************************************************
 *
 * @param mark: select or de-select the icon.
 * if you move to another icon then you must remove
 * the marking from the previous one
 *
 * There are 2 lines and 2 columns of icons in the menu.
 *
 * @param row: select the line where you want to mark\un-mark
 * @param col: select the column where you want to mark\un-mark
 *
 * @param color: mark: ST7735_RED , unmark: ST7735_WHITE
 *************************************************/
uint8_t Icon_Mark (uint8_t row , uint8_t col , uint16_t color)
{


    for(int x = 0; x < (ICON_LENGTH + 4); x++) {		// horizontal lines
        ST7735_DrawPixel( x + (col * (ICON_LENGTH + 4)) , 0 + (row * (ICON_LENGTH + 4))       , color);
        ST7735_DrawPixel( x + (col * (ICON_LENGTH + 4)) , 1 + (row * (ICON_LENGTH + 4))       , color);
        ST7735_DrawPixel( x + (col * (ICON_LENGTH + 4)) , ((row + 1) * (ICON_LENGTH + 4)) - 2 , color);
        ST7735_DrawPixel( x + (col * (ICON_LENGTH + 4)) , ((row + 1) * (ICON_LENGTH + 4)) - 1 , color);
    }

    for(int y = 0; y < (ICON_LENGTH + 4); y++) {		// vertical lines
        ST7735_DrawPixel( 0 + (col * (ICON_LENGTH + 4))       , y + (row * (ICON_LENGTH + 4)) , color);
        ST7735_DrawPixel( 1 + (col * (ICON_LENGTH + 4))       , y + (row * (ICON_LENGTH + 4)) , color);
        ST7735_DrawPixel( ((col + 1) * (ICON_LENGTH + 4)) - 2 , y + (row * (ICON_LENGTH + 4)) , color);
        ST7735_DrawPixel( ((col + 1) * (ICON_LENGTH + 4)) - 1 , y + (row * (ICON_LENGTH + 4)) , color);
    }

	return sysOK;

}


/******* Settings *******/


void Form_Intensity (Menu *menu)
{
    // print static information like headers...

    //clean screen
    ST7735_FillScreen(ST7735_Fluorescent_blue);

    // print current title
    ST7735_WriteString( 30    ,  4, "Intensity"     , Font_11x18, ST7735_Orange, ST7735_Fluorescent_blue);
}


void Form_Intensity_options (Menu *menu)
{
    // show current menu's options
    uint16_t y_pos = 40;  // spacing
    for (uint8_t i = 0; i < menu->num_options; i++)
    {
        uint16_t color = (i == *(menu->selected_index)) ? ST7735_Fluorescent_blue : ST7735_BLACK;
        uint16_t BG_color = (i == *(menu->selected_index)) ? ST7735_BLACK : ST7735_Fluorescent_blue;

        ST7735_WriteString(((1 + (i * 8)) * Font_11x18.width), y_pos, menu->options[i], Font_11x18, color, BG_color);
    }

    uint16_t color = (0 == *(menu->selected_index)) ? ST7735_BLACK : Light_gray;

    // horizontal line
    for (int x = (ST7735_XSTART + 26) ; x <= (ST7735_WIDTH - 1 - 26)  ; x++)
    {
        ST7735_DrawPixel( x   , 100 , color);
        ST7735_DrawPixel( x   , 101 , color);
        ST7735_DrawPixel( x   , 102 , color);
        ST7735_DrawPixel( x   , 103 , color);
        ST7735_DrawPixel( x   , 114 , color);
        ST7735_DrawPixel( x   , 115 , color);
        ST7735_DrawPixel( x   , 116 , color);
        ST7735_DrawPixel( x   , 117 , color);
    }

    // vertical line
    for (int x = (104) ; x < (114)  ; x++)
    {
        ST7735_DrawPixel( 26  , x   , color);
        ST7735_DrawPixel( 27  , x   , color);
        ST7735_DrawPixel( 28  , x   , color);
        ST7735_DrawPixel( 29  , x   , color);
        ST7735_DrawPixel( 130 , x   , color);
        ST7735_DrawPixel( 131 , x   , color);
        ST7735_DrawPixel( 132 , x   , color);
        ST7735_DrawPixel( 133 , x   , color);
    }
}


void Form_Intensity_auto (Menu *menu)
{
    // Not implemented yet
    ;
}


void Form_Intensity_Manual (Menu *menu)
{
    static uint8_t old_intensity = 5;

    uint8_t val = (*(menu->selected_index) + 1) * 5;
	uint8_t new_intensity = BackLight_Dimmer(val);
	HAL_RTCEx_BKUPWrite (&hrtc, RTC_BKP_DR2, (uint32_t)(val) );
	val = HAL_RTCEx_BKUPRead (&hrtc, RTC_BKP_DR2);

	char CH[5];
	sprintf(CH , "%d%%" , val);

    ST7735_FillRectangleFast(9 * Font_7x10.width, 80, 4 * 9 * Font_7x10.width, Font_7x10.height, ST7735_Fluorescent_blue);
	ST7735_WriteString(9 * Font_7x10.width, 80, CH, Font_7x10, ST7735_BLACK, ST7735_Fluorescent_blue);

	ST7735_FillRectangleFast((25 + old_intensity), 104,  5, 10, ST7735_Fluorescent_blue);
	ST7735_FillRectangleFast((25 + new_intensity), 104,  5, 10, ST7735_BLACK);

	old_intensity = new_intensity;
}


void Display_timeout (Menu *menu)
{
    // print static information like headers...

	uint16_t x_pos = 20 + 4 * Font_11x18.width;
    //clean screen
    ST7735_FillScreen(ST7735_Fluorescent_blue);

    // print current title
    ST7735_WriteString( 14    ,  4, "BkLt Timeout" , Font_11x18, ST7735_Orange, ST7735_Fluorescent_blue);

    for (uint8_t i = 0; i < menu->num_options; i++)
    {
        ST7735_WriteString(x_pos, 4 + Font_11x18.height + 8 + (i * (Font_11x18.height + 5)), "sec", Font_11x18, ST7735_BLACK, ST7735_Fluorescent_blue);
    }

}


void Display_timeout_Vals (Menu *menu)
{

    // show current menu's options
    uint16_t x_pos = 20;  // spacing
    for (uint8_t i = 0; i < menu->num_options; i++)
    {
        uint16_t color = (i == *(menu->selected_index)) ? ST7735_Fluorescent_blue : ST7735_BLACK;
        uint16_t BG_color = (i == *(menu->selected_index)) ? ST7735_BLACK : ST7735_Fluorescent_blue;

        ST7735_WriteString(x_pos, 4 + Font_11x18.height + 8 + (i * (Font_11x18.height + 5)), menu->options[i], Font_11x18, color, BG_color);
    }
    // 15 30 60 120
    uint8_t val = *menu->selected_index;

    HAL_RTCEx_BKUPWrite (&hrtc, RTC_BKP_DR3, val );
}


void Form_IO_Coonfig (Menu *menu)
{
    // print static information like headers...

    //clean screen
    ST7735_FillScreen(ST7735_Fluorescent_blue);

    // print current title
    ST7735_WriteString( 14    ,  4, "I/O Config" , Font_11x18, ST7735_Orange, ST7735_Fluorescent_blue);
}


void Form_IO_Coonfig_options (Menu *menu)
{

    // show current menu's options
    uint16_t x_pos = 20;  // spacing
    for (uint8_t i = 0; i < menu->num_options; i++)
    {
        uint16_t color = (i == *(menu->selected_index)) ? ST7735_Fluorescent_blue : ST7735_BLACK;
        uint16_t BG_color = (i == *(menu->selected_index)) ? ST7735_BLACK : ST7735_Fluorescent_blue;

        ST7735_WriteString(x_pos, 4 + Font_11x18.height + 8 + (i * (Font_7x10.height + 5)), menu->options[i], Font_7x10, color, BG_color);
    }
    // Home, Parking, Garden", Vehicles, Stroller, General
    uint8_t val = *menu->selected_index;

    HAL_RTCEx_BKUPWrite (&hrtc, RTC_BKP_DR4, val );
}


/*
 * Header and static info
 */
void Form_DateTime (Menu *menu)		// ILLI
{
	int TZ = 0;

    // read the offset from back-up register
    TZ = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);

    // save the offset in gGpsData_X structure
    if (gps_data_Lock() == pdTRUE)
    {
    	gGpsData_X.timeZone = TZ;
    	gps_data_Unlock();
    }

	// print static information like headers...

	//clean screen
	ST7735_FillScreen(ST7735_Fluorescent_blue);

	// print current title
	ST7735_WriteString(20 ,  4, "Date & Time"     , Font_11x18, ST7735_Orange, ST7735_Fluorescent_blue);

	// print header
	ST7735_WriteString (1 * Font_7x10.width, 80, "DD MM YYYY   hh mm TZ", Font_7x10, ST7735_BLACK, ST7735_Fluorescent_blue);

	ST7735_WriteString(3  * Font_7x10.width, 106, "-"  , Font_7x10, ST7735_BLACK, ST7735_Fluorescent_blue);
	ST7735_WriteString(6  * Font_7x10.width, 106, "-20", Font_7x10, ST7735_BLACK, ST7735_Fluorescent_blue);
	ST7735_WriteString(16 * Font_7x10.width, 106, ":"  , Font_7x10, ST7735_BLACK, ST7735_Fluorescent_blue);

	print_current_DateTime();

	// ask for data...
	gui_event_type_t evt = GUI_GPS_LOC_SUBSCRIBE;
	xQueueSend(qGuiEvents, &evt, 0);
	xTaskNotify(ApplicationTask_Handler, EVT_GUI_REQUEST, eSetBits);

}


/*
 * On manual Date & Time:
 * prints the current values of date and time
 */
void print_current_DateTime(void)		// ILLI
{
    char CH[4] = {0};

    // TEMPORARY: local reading directly from RTC:
    HAL_RTC_GetTime(&hrtc,
                    &gGuiState.time,
                    RTC_FORMAT_BIN);

    HAL_RTC_GetDate(&hrtc,
                    &gGuiState.date,
                    RTC_FORMAT_BIN);


//	if (HAL_RTC_GetTime(&hrtc, &gGuiState.time, RTC_FORMAT_BIN) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	if (HAL_RTC_GetDate(&hrtc, &gGuiState.date, RTC_FORMAT_BIN) != HAL_OK)
//	{
//		Error_Handler();
//	}

	// print date & time values
	sprintf(CH , "%02d" , (int)gGuiState.date.Date);
    print_dateTime_values(CH, 0);

	sprintf(CH , "%02d" , (int)gGuiState.date.Month);
    print_dateTime_values(CH, 1);

	sprintf(CH , "%02d" , (int)gGuiState.date.Year);
    print_dateTime_values(CH, 2);

	sprintf(CH , "%02d" , (int)gGuiState.time.Hours);
    print_dateTime_values(CH, 3);

	sprintf(CH , "%02d" , (int)gGuiState.time.Minutes);
    print_dateTime_values(CH, 4);

}


/*
 * When leaving the screen, we should restore the previous data
 * INCLUDE if "Ent to SAVE" message was shown.
 */
void Form_DateTime_Manual_leave(void)
{
	// clean part of the screen
	ST7735_FillRectangleFast(7  , 54 , 143 , 50 , ST7735_Fluorescent_blue);

	// print header
	ST7735_WriteString (1 * Font_7x10.width, 80, "DD MM YYYY   hh mm TZ", Font_7x10, ST7735_BLACK, ST7735_Fluorescent_blue);

	print_current_DateTime();
}

/*
 * 2 options: Manual or GPS set
 */
void Form_DateTime_options (Menu *menu)
{
	// show current menu's options
	uint16_t y_pos = 50;  // spacing
	for (uint8_t i = 0; i < menu->num_options; i++)
	{
		uint16_t color = (i == *(menu->selected_index)) ? ST7735_Fluorescent_blue : ST7735_BLACK;
		uint16_t BG_color = (i == *(menu->selected_index)) ? ST7735_BLACK : ST7735_Fluorescent_blue;

		ST7735_WriteString(((1 + (i * 8)) * Font_11x18.width), y_pos, menu->options[i], Font_11x18, color, BG_color);
	}
}


/*
 * In manual Date & Time, will set the values.
 * Currently jumps between the fields
 * Next: typing "Select" will increment the values.
 */
void Form_DateTime_Manual (Menu *menu)
{
    static uint8_t old_idx = 0;
    uint8_t        idx     = *(menu->selected_index);
    char CH[4]             = {0};

//	// read RTC date & time
//	HAL_RTC_GetTime(&hrtc, &gGuiState.time, RTC_FORMAT_BIN);
//	HAL_RTC_GetDate(&hrtc, &gGuiState.date, RTC_FORMAT_BIN);

	switch (old_idx)
	{
        case 0:
        	sprintf(CH , "%02d" , (int)gGuiState.date.Date);
            break;

        case 1:
        	sprintf(CH , "%02d" , (int)gGuiState.date.Month);
            break;

        case 2:
        	sprintf(CH , "%02d" , (int)gGuiState.date.Year);
            break;

        case 3:
        	sprintf(CH , "%02d" , (int)gGuiState.time.Hours);
            break;

        case 4:
        	sprintf(CH , "%02d" , (int)gGuiState.time.Minutes);
            break;

        case 5:
        	Form_DateTime(menu);
        	Form_DateTime_options (menu->parent);
            break;
	}

	if (old_idx < 5)
	{
        ST7735_WriteString (date_index[old_idx] * Font_7x10.width, 106, CH, Font_7x10, ST7735_BLACK, ST7735_Fluorescent_blue);
    }

    switch (idx)
	{
        case 0:
        	sprintf(CH , "%02d" , (int)gGuiState.date.Date);
            break;

        case 1:
        	sprintf(CH , "%02d" , (int)gGuiState.date.Month);
            break;

        case 2:
        	sprintf(CH , "%02d" , (int)gGuiState.date.Year);
            break;

        case 3:
        	sprintf(CH , "%02d" , (int)gGuiState.time.Hours);
            break;

        case 4:
        	sprintf(CH , "%02d" , (int)gGuiState.time.Minutes);
            break;

        case 5:
        	print_ENTtoSAVE();
            break;
	}

	if (idx < 5)
	{
        ST7735_WriteString (date_index[    idx] * Font_7x10.width, 106, CH, Font_7x10, ST7735_Fluorescent_blue, ST7735_BLACK);
	}

    old_idx = idx;
}


void DateTime_Manual_enter(Menu *menu)
{
    HAL_RTC_GetTime(&hrtc, &gGuiState.time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &gGuiState.date, RTC_FORMAT_BIN);
}




void Form_DateTime_gps (Menu *menu)
{
//	Custom_Form = true;
	int TZ = 0;
	char str[4] = {"\0"};
	uint16_t color;
	uint16_t BG_color;

    // read the offset from back-up register
    TZ = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);

    // save the offset in gGpsData_X structure
    if (gps_data_Lock() == pdTRUE)
    {
    	gGpsData_X.timeZone = TZ;
    	gps_data_Unlock();
    }

//	if (gps_data_Lock() == pdTRUE)
//	{
//		TZ = gGpsData_X.timeZone;
//		gps_data_Unlock();
//	}

//	// ask for data...
//	gui_event_type_t evt = GUI_GPS_LOC_SUBSCRIBE;
//	xQueueSend(qGuiEvents, &evt, 0);
//	xTaskNotify(ApplicationTask_Handler, EVT_GUI_REQUEST, eSetBits);
//
	// clean part of the screen
	ST7735_FillRectangleFast(7  , 75 , 146 , 25 , ST7735_Fluorescent_blue);

	// show current menu's options
	for (uint8_t i = 0; i < menu->num_options; i++)
	{
		color    = (i == *(menu->selected_index)) ? ST7735_Fluorescent_blue : ST7735_BLACK;
		BG_color = (i == *(menu->selected_index)) ? ST7735_BLACK : ST7735_Fluorescent_blue;

		ST7735_WriteString(Font_11x18.width ,(75 + (Font_7x10.height + 5) * i) , menu->options[i], Font_7x10, color, BG_color);
	}

	// print time-zone value
	sprintf(str , "%+d" , TZ);
	ST7735_WriteString(Font_11x18.width + (10 * Font_7x10.width) ,(75 + (Font_7x10.height + 5) * 0) , str, Font_7x10, ST7735_BLACK, ST7735_Fluorescent_blue);
	if (TZ > -10 && TZ < 10)	// returning from 2 digits to 1 digit
	{
		ST7735_FillRectangleFast(Font_11x18.width + (12 * Font_7x10.width), (75 + (Font_7x10.height + 5) * 0), Font_7x10.width, Font_7x10.height, ST7735_Fluorescent_blue);
	}

//	// show the current values from GPS structure
//	if (*(menu->selected_index) == 1)
//	{
//		char gps_date[11] = {"\0"};
//		char gps_time[6] = {"\0"};
//		if (gps_data_Lock() == pdTRUE)
//		{
//			// currently for tests
//			// add "% 100" to secure snprintf() (It demands butes depending type: uint8_t can be 255 --> 3 chars)
//			snprintf (gps_date , sizeof(gps_date) , "%02d-%02d-20%02d", gGpsData_X.gDate.Date % 100  , gGpsData_X.gDate.Month % 100  , gGpsData_X.gDate.Year % 100);
//			snprintf (gps_time , sizeof(gps_time) , "%02d:%02d"       , gGpsData_X.gTime.Hours % 100 , gGpsData_X.gTime.Minutes % 100);
//
//			ST7735_WriteString( 1 * Font_7x10.width, (106) , gps_date, Font_7x10, ST7735_RED, ST7735_Fluorescent_blue);
//			ST7735_WriteString(14 * Font_7x10.width, (106) , gps_time, Font_7x10, ST7735_RED, ST7735_Fluorescent_blue);
//
//			gps_data_Unlock();
//		}
//	}

}


void AdjustTimeByOffset(RTC_DateTypeDef *date, RTC_TimeTypeDef *time, int offsetHours)
{
    struct tm tm_time;

    // Convert RTC structures to struct tm
    tm_time.tm_sec  = time->Seconds;
    tm_time.tm_min  = time->Minutes;
    tm_time.tm_hour = time->Hours;

    tm_time.tm_mday = date->Date;
    tm_time.tm_mon  = date->Month - 1;      // struct tm months: 0-11
    tm_time.tm_year = date->Year + 100;     // struct tm year = years since 1900

    // Apply hour offset
    tm_time.tm_hour += offsetHours;

    // Normalize using mktime
    time_t raw_time = mktime(&tm_time);
    if (raw_time == -1) {
        // Failed to normalize, handle error if needed
        return;
    }

    // Get normalized values
    struct tm *new_tm = localtime(&raw_time);

    // Write back to RTC structures
    time->Seconds = new_tm->tm_sec;
    time->Minutes = new_tm->tm_min;
    time->Hours   = new_tm->tm_hour;

    date->Date  = new_tm->tm_mday;
    date->Month = new_tm->tm_mon + 1;
    date->Year  = new_tm->tm_year - 100;
}


void update_rtc_from_gps (Menu *menu)
{
	RTC_DateTypeDef gDate;		// date
	RTC_TimeTypeDef gTime;		// time
	int tzone;

	if (gps_data_Lock() == pdTRUE)
	{
		memcpy (&gDate, &gGpsData_X.gDate, sizeof(RTC_DateTypeDef));
		memcpy (&gTime, &gGpsData_X.gTime, sizeof(RTC_TimeTypeDef));
		tzone = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
//		memcpy (&tzone , &gGpsData_X.timeZone, sizeof(int));
		gps_data_Unlock();
	}


	AdjustTimeByOffset(&gDate, &gTime, tzone);

	if (HAL_RTC_SetDate(&hrtc, &gDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_RTC_SetTime(&hrtc, &gTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	memcpy (&gGuiState.date, &gDate, sizeof(RTC_DateTypeDef));
	memcpy (&gGuiState.time, &gTime, sizeof(RTC_TimeTypeDef));

    print_DONE();
	logger(RTC_SAVED);
	vTaskDelay(pdMS_TO_TICKS(1000));

}


/******* Data Forms *******/

void form_gps_power_data (gui_event_data_t *data)
{
	uint8_t sat_num [12] = {0};
	uint8_t sat_SNR [12] = {0};
	uint8_t qty = 0;
	bool stts = false;
	char str[3] = {"\0"};

	if (gps_data_Lock() == pdTRUE)
	{
		qty = gGpsSats_X.NumOfSats;
		for (int idx = 0 ; idx < qty ; idx++)
		{
			sat_num[idx] = gGpsSats_X.SatChann[idx].sat_ID;
			sat_SNR[idx] = gGpsSats_X.SatChann[idx].sat_SNR;

			stts         = gGpsData_X.status;
		}

		gps_data_Unlock();
	}

	snprintf (str , sizeof(str) , "%d" , qty % 100);
	ST7735_WriteString( ST7735_XSTART + 80 + (8 * Font_7x10.width),  30, str       , Font_7x10, ST7735_Fluorescent_blue, ST7735_BLACK);
	if (qty < 10)
	{
		ST7735_FillRectangleFast( ST7735_XSTART + 80  + (1 * Font_7x10.width),  30, 1 * Font_7x10.width, Font_7x10.height, ST7735_BLACK );
	}

	if (stts)
	{
		      ST7735_WriteString( ST7735_XSTART + 4  + (4 * Font_7x10.width),  30, "VALID"      , Font_7x10, ST7735_GREEN, ST7735_BLACK);
	}
	else
	{
		ST7735_FillRectangleFast( ST7735_XSTART + 4  + (4 * Font_7x10.width),  30, 5 * Font_7x10.width, Font_7x10.height, ST7735_BLACK );
	}



//	char str[3];
	uint16_t col = 0;
	for (uint8_t i = 0 ; qty <= 11 ? i < qty : i < 11 ; i++)
	{
		// print channel number
		itoa(sat_num[i] , str , 10);
		ST7735_WriteString( ST7735_XSTART + 4 + ((2 * i) * Font_7x10.width),  116, str       , Font_5x7, ST7735_Fluorescent_blue, ST7735_BLACK);

		ST7735_FillRectangleFast( ST7735_XSTART + 4 + ((2 * i) * Font_7x10.width), 40, Font_7x10.width, (116 - 40), ST7735_BLACK );

		// print columns with colors
		if (sat_SNR[i] < 10)                          col = ST7735_RED;
		else if (sat_SNR[i] >= 10 && sat_SNR[i] < 20) col = ST7735_Orange;
		else if (sat_SNR[i] >= 20 && sat_SNR[i] < 30) col = ST7735_YELLOW;
		else if (sat_SNR[i] >= 30 && sat_SNR[i] < 50) col = ST7735_GREEN;
		else if (sat_SNR[i] >= 50 )                   col = ST7735_YELLOW;

		// in case of (SNR = 0) - change it to 2 to show something..
		if (sat_SNR[i] < 2)
		{
			ST7735_FillRectangleFast( ST7735_XSTART + 4 + ((2 * i) * Font_7x10.width), (116 - 2 - sat_SNR[i] + 0), Font_7x10.width, sat_SNR[i] + 2, col );
		}
		else
			ST7735_FillRectangleFast( ST7735_XSTART + 4 + ((2 * i) * Font_7x10.width), (116 - 2 - sat_SNR[i]), Font_7x10.width, sat_SNR[i], col );
	}
}


void form_gps_loc_data   (gui_event_data_t *data)
{
	PayloadGPS_t gpsLoc;
	char str[15] = {"\0"};

	// 1. copy to local struct (sensors data)
    memcpy(&gpsLoc,
           data->buff,
           sizeof(PayloadGPS_t));

    uint8_t latDeg;
	uint8_t latMin;
	uint8_t latSec;
	char    latSign;
	microdeg_to_DMS(gpsLoc.latitude_1e6, &latDeg, &latMin, &latSec, &latSign, 1);

    uint8_t lonDeg;
	uint8_t lonMin;
	uint8_t lonSec;
	char    lonSign;
	microdeg_to_DMS(gpsLoc.longitude_1e6, &lonDeg, &lonMin, &lonSec, &lonSign, 0);

    // LATitude		1234.5678
    snprintf (str , sizeof(str) , "%2d.%d" , (int)(gpsLoc.latitude_1e6) / 1000000 , (int)(gpsLoc.latitude_1e6) % 1000000);
    ST7735_WriteString( ST7735_XSTART + 4  , 42  , str , Font_7x10, ST7735_GREEN, ST7735_BLACK);

    snprintf (str , sizeof(str) , "%02d*%02d'%02d %c" , latDeg, latMin, latSec, latSign);
    ST7735_WriteString( ST7735_XSTART + 4  , 67  , str , Font_7x10, ST7735_GREEN, ST7735_BLACK);


    // LONgtitude		12345.6789
    snprintf (str , sizeof(str) , "%3d.%d" , (int)(gpsLoc.longitude_1e6) / 1000000 , (int)(gpsLoc.longitude_1e6) % 1000000);
    ST7735_WriteString( ST7735_XSTART + 80 , 42  , str , Font_7x10, ST7735_GREEN, ST7735_BLACK);

    snprintf (str , sizeof(str) , "%02d*%02d'%02d %c" , lonDeg, lonMin, lonSec, lonSign);
    ST7735_WriteString( ST7735_XSTART + 80 , 67  , str , Font_7x10, ST7735_GREEN, ST7735_BLACK);

    static uint8_t clrSTRspd = 0;
    static uint8_t clrSTRcrs = 0;
    uint8_t val;

    // Speed over ground
    val = snprintf (str , sizeof(str) , "%3d.%dkm/h" , (int)(gpsLoc.speed_x10) / 10 , (int)(gpsLoc.speed_x10) % 10);
    ST7735_WriteString( ST7735_XSTART + 4  , 92  , str , Font_7x10, ST7735_GREEN, ST7735_BLACK);

    if (clrSTRspd > val)
    {
        ST7735_FillRectangleFast( ST7735_XSTART + 4 + (Font_7x10.width) * val  , 92 , (Font_7x10.width) * (clrSTRspd - val) , Font_7x10.height , ST7735_BLACK);
    }
    clrSTRspd = val;

    // course
    val = snprintf (str , sizeof(str) , "%3d.%d*" , (int)(gpsLoc.course_x10) / 10 , (int)(gpsLoc.course_x10) % 10);
    ST7735_WriteString( ST7735_XSTART + 80 , 92  , str , Font_7x10, ST7735_GREEN, ST7735_BLACK);

    if (clrSTRcrs > val)
    {
        ST7735_FillRectangleFast( ST7735_XSTART + 80 + (Font_7x10.width) * val , 92 , (Font_7x10.width) * (clrSTRcrs - val) , Font_7x10.height , ST7735_BLACK);
    }

    // date
    val = snprintf (str , sizeof(str) , "%02d//%02d//%02d" , gpsLoc.gDate.Date , gpsLoc.gDate.Month , gpsLoc.gDate.Year);
    ST7735_WriteString( ST7735_XSTART + 4 , 112  , str , Font_7x10, ST7735_YELLOW, ST7735_BLACK);

    // time
    val = snprintf (str , sizeof(str) , "%02d:%02d:%02d" , gpsLoc.gTime.Hours , gpsLoc.gTime.Minutes , gpsLoc.gTime.Seconds);
    ST7735_WriteString( ST7735_XSTART + 4+96 , 112  , str , Font_7x10, ST7735_YELLOW, ST7735_BLACK);
}


void form_current_DateTime_from_gps (gui_event_data_t *data)
{
	PayloadGPS_t gpsLoc;
	char str[15] = {"\0"};

    uint8_t val;


	// 1. copy to local struct (sensors data)
    memcpy(&gpsLoc,
           data->buff,
           sizeof(PayloadGPS_t));

    // date
    val = snprintf (str , sizeof(str) , "%02d/%02d/20%02d" , gpsLoc.gDate.Date , gpsLoc.gDate.Month , gpsLoc.gDate.Year);
    ST7735_WriteString( 1 * Font_7x10.width , 106  , str , Font_7x10, ST7735_RED, ST7735_Fluorescent_blue);

    // time
    val = snprintf (str , sizeof(str) , "%02d:%02d:%02d" , gpsLoc.gTime.Hours , gpsLoc.gTime.Minutes , gpsLoc.gTime.Seconds);
    ST7735_WriteString(14 * Font_7x10.width , 106  , str , Font_7x10, ST7735_RED, ST7735_Fluorescent_blue);

}


//====================================================

void exit_gps_pwr (void)
{
	// cancel receiving data...
	gui_event_type_t evt = GUI_GPS_POWER_UNSUBSCRIBE;
	xQueueSend(qGuiEvents, &evt, 0);
	xTaskNotify(ApplicationTask_Handler, EVT_GUI_REQUEST, eSetBits);
}


void exit_gps_loc (void)
{
	// cancel receiving data...
	gui_event_type_t evt = GUI_GPS_LOC_UNSUBSCRIBE;
	xQueueSend(qGuiEvents, &evt, 0);
	xTaskNotify(ApplicationTask_Handler, EVT_GUI_REQUEST, eSetBits);
}


void exit_env (void)
{
	// cancel receiving data...
	gui_event_type_t evt = GUI_ENV_UNSUBSCRIBE;
	xQueueSend(qGuiEvents, &evt, 0);
	xTaskNotify(ApplicationTask_Handler, EVT_GUI_REQUEST, eSetBits);
}


/************ END OF EXAMPLE ************/
