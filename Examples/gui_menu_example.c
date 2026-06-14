#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "gui_menu.h"
#include "gui_types.h"


/******* lists of options for each menu *******/


// level 1
const char *main_options[] = {
		"Communication",
		"LoRa",
		"Location",
		"Sensors",
		"Status",
		"Settings",
		"Help"
};

// level 2
const char *submenu6_Settings[] = {
		"Intensity",
		"BackLight Timer",
		"Date & Time",
		"FW Update",
		"I/O Config"
};

// LEVEL 3
const char *submenu61_Intensity[] =
{
		"Manual",
		"Auto"
};

const char *submenu62_BkLt_Timeout[] =
{
		"15",
		"30",
		"60",
		"120"
};

const char *submenu63_TimeZone[] =
{
		"Manual",
		"GPS"
};

const char *submenu64_FWUpdate[] =
{
		"Roll BAck",
		"Load New"
};

const char *submenu65_IOconfig[] =
{
		"Home",
		"Parking",
		"Garden",
		"Vehicles",
		"Stroller",
		"General",
};

// level 4
const char *submenu611_Intensity_manual[] =
{
		" 5","10","15","20","25","30","35","40","45","50",
		"55","60","65","70","75","80","85","90","95","100"
};


// level 5
const char *submenu631_DateTime[] =
{
		"dd","MM","YYYY","hh","mm","TZ"
};


const char *submenu632_GPS_Date[] =
{
		"Zone","Update"
};


/******* selected_index *******/
static uint8_t home_screen_idx = 0;

static uint8_t main_idx        = 0;

static uint8_t comm_idx        = 0;
static uint8_t LoRa_idx        = 0;
static uint8_t Location_idx    = 0;
static uint8_t Sensors_idx     = 0;
//static uint8_t Status_idx      = 0;
static uint8_t Settings_idx    = 0;
static uint8_t Help_idx        = 0;

static uint8_t Intensity_idx       = 0;
static uint8_t BkLt_Timeout_idx    = 0;
static uint8_t DateTime_idx        = 0;
static uint8_t FWUpdate_idx        = 0;
static uint8_t IO_config_idx       = 0;


/******* Menu structs *******/

// level 0
static Menu menu_home;

// level 1
static Menu menu_main;

// level 2
static Menu menu_comm;
static Menu menu_LoRa;
static Menu menu_Location;
static Menu menu_Sensors;
static Menu menu_Status;
static Menu menu_Settings;
static Menu menu_Help;

// level 3
static Menu menu_Intensity;
static Menu menu_DateTime;
static Menu menu_BkLt_Timer;
static Menu menu_FWUpdate;
static Menu menu_IO_Coonfig;

// level 4
static Menu menu_intensity_man;
static Menu menu_Intensity_2;

static Menu menu_DateTime_Man;
static Menu menu_DateTime_GPS;

static Menu menu_FW_roll_back;
static Menu menu_FW_load_new;


/*******  Children *******/
// Level 0
static Menu *home_children[] = {
    &menu_main
};

// Level 1 - Home Screen
static Menu *main_children[] = {
    &menu_comm,
    &menu_LoRa,
    &menu_Location,
    &menu_Sensors,
    &menu_Status,
    &menu_Settings,
    &menu_Help
};

// Level 2 - Main Menu
static Menu *Settings_children[] = {
    &menu_Intensity,
    &menu_BkLt_Timer,
    &menu_DateTime,
    &menu_FWUpdate,
	&menu_IO_Coonfig
};

// Level 3 - Settings
static Menu *Intensity_children[] = {
        &menu_intensity_man,
        &menu_Intensity_2
};

static Menu *DateTime_children[] = {
        &menu_DateTime_Man,
        &menu_DateTime_GPS
};

static Menu *FWUpdate_children[] = {
        &menu_FW_roll_back,
        &menu_FW_load_new
};


int intnsty = 0;

void menu_init(void)
{

	intnsty = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
	Intensity_man_idx = (uint8_t)(intnsty / 5);

    // fill structs
    current_menu = &menu_home;
// Level 0
    // Home Screen (LIST)
    menu_home = (Menu){
        .title = "Home Screen",
        .type = MENU_TYPE_LIST,
        .options = home_screen_options,
        .num_options = 1,
        .selected_index = &home_screen_idx,
        .parent = &menu_home,
        .children = home_children,
		.render = Form_HomeScreen
    };

// Level 1
    // Main Menu (LIST)
    menu_main = (Menu){
        .title = "Main Menu",
        .type = MENU_TYPE_LIST,
        .options = main_options,
        .num_options = 7,
        .selected_index = &main_idx,
        .parent = &menu_home,
        .children = main_children,
		.on_enter = menu_generic_enter,
		.render = Form_Main_Menu
    };

// Level 2
    // Communication Menu (LIST)
    menu_comm = (Menu){
        .title = "Communication",
        .type = MENU_TYPE_LIST,
        .options = submenu1_Communication,
        .num_options = 2,
        .selected_index = &comm_idx,
        .parent = &menu_main,
        .children = Communication_children
    };

    // LoRa Menu (LIST)
    menu_LoRa = (Menu){
        .title = "LoRa",
        .type = MENU_TYPE_LIST,
        .options = submenu2_LoRa,
        .num_options = 6,
        .selected_index = &LoRa_idx,
        .parent = &menu_main,
        .children = LoRa_children
    };

    // Location Menu (LIST)
    menu_Location = (Menu){
        .title = "Location",
        .type = MENU_TYPE_LIST,
        .options = submenu3_Location,
        .num_options = 3,
        .selected_index = &Location_idx,
        .parent = &menu_main,
        .children = Location_children
    };

    // Sensors Menu (LIST)
    menu_Sensors = (Menu){
        .title = "Sensors",
        .type = MENU_TYPE_LIST,
        .options = submenu4_Sensors,
        .num_options = 4,
        .selected_index = &Sensors_idx,
        .parent = &menu_main,
        .children = Sensors_children
    };

    // Status Menu (LIST)
    menu_Status = (Menu){
        .title = "Status",
        .type = MENU_TYPE_SCREEN,
        .options = NULL,
        .num_options = 0,
        .selected_index = 0,
        .parent = &menu_main,
        .children = NULL
    };

    // Settings Menu (LIST)
    menu_Settings = (Menu){
        .title = "Settings",
        .type = MENU_TYPE_LIST,
        .options = submenu6_Settings,
        .num_options = 5,
        .selected_index = &Settings_idx,
        .parent = &menu_main,
        .children = Settings_children
    };

    // Help Menu (LIST)
    menu_Help = (Menu){
        .title = "Help",
        .type = MENU_TYPE_LIST,
        .options = submenu7_Help,
        .num_options = 3,
        .selected_index = &Help_idx,
        .parent = &menu_main,
        .children = Help_children
    };

// Level 3.6 - Settings
    menu_Intensity      = (Menu){
        .title          = "Intensity",
        .type           = MENU_TYPE_LIST,
        .options        = submenu61_Intensity,
        .num_options    = 2,
        .selected_index = &Intensity_idx,
        .parent         = &menu_Settings,
        .children       = Intensity_children,
		.render         = Form_Intensity_options,
		.on_enter       = Form_Intensity
    };

    menu_BkLt_Timer = (Menu){
        .title          = "BkLt Timeout",
        .type           = MENU_TYPE_LIST,
        .options        = submenu62_BkLt_Timeout,
        .num_options    = 4,
        .selected_index = &BkLt_Timeout_idx,
        .parent         = &menu_Settings,
		.render         = Display_timeout_Vals,
		.on_enter       = Display_timeout
    };

    menu_DateTime       = (Menu){
        .title          = "Date & Time",
        .type           = MENU_TYPE_LIST,
        .options        = submenu63_TimeZone,
        .num_options    = 2,
        .selected_index = &DateTime_idx,
        .parent         = &menu_Settings,
        .children       = DateTime_children,
		.render         = Form_DateTime_options,
		.on_enter       = Form_DateTime
    };

    menu_FWUpdate = (Menu){
        .title          = "FW Update",
        .type           = MENU_TYPE_LIST,
        .options        = submenu64_FWUpdate,
        .num_options    = 2,
        .selected_index = &FWUpdate_idx,
        .parent         = &menu_Settings,
        .children       = FWUpdate_children
    };

    menu_IO_Coonfig = (Menu){
        .title          = "IO Config",
        .type           = MENU_TYPE_LIST,
        .options        = submenu65_IOconfig,
        .num_options    = 6,
        .selected_index = &IO_config_idx,
        .parent         = &menu_Settings,
		.render         = Form_IO_Coonfig_options,
		.on_enter       = Form_IO_Coonfig
    };

    // Level 4
// intensity
    menu_intensity_man  = (Menu){
        .title          = "Intensity Manual",
        .type           = MENU_TYPE_LIST,
        .options        = submenu611_Intensity_manual,
        .num_options    = 20,
        .selected_index = &Intensity_man_idx,
        .parent         = &menu_Intensity,
        .render         = Form_Intensity_Manual
    };

// date & time
    menu_DateTime_Man  = (Menu){
        .title          = "DateTime Man",
        .type           = MENU_TYPE_LIST,
        .options        = submenu631_DateTime,
        .num_options    = 6,
        .selected_index = &DateTime_man_idx,
        .parent         = &menu_DateTime,
        .render         = Form_DateTime_Manual,
		.on_enter       = DateTime_Manual_enter,
		.on_exit        = Form_DateTime_Manual_leave
    };

    menu_DateTime_GPS   = (Menu){
        .title          = "DateTime GPS",
        .type           = MENU_TYPE_LIST,
        .options        = submenu632_GPS_Date,
        .num_options    = 2,
        .selected_index = &DateTime_gps_idx,
        .parent         = &menu_DateTime,
        .render         = Form_DateTime_gps,
		.on_exit        = Form_DateTime_Manual_leave
    };

// firmware
	menu_FW_roll_back   = (Menu){
        .title          = "Roll BAck",
        .type           = MENU_TYPE_LIST,
        .options        = submenu641_NO_YES,
        .num_options    = 2,
        .selected_index = &FW_roll_back_idx,
        .parent         = &menu_FWUpdate,
        .render         = Form_change_options,
		.on_enter       = Form_FW_roll_back
    };

	menu_FW_load_new    = (Menu){
        .title          = "Load New",
        .type           = MENU_TYPE_LIST,
        .options        = submenu641_NO_YES,
        .num_options    = 2,
        .selected_index = &FW_load_new_idx,
        .parent         = &menu_FWUpdate,
        .render         = Form_change_options,
		.on_enter       = Form_FW_load_new
    };

}


void menu_handle_button(button_id_t btn)
{
    switch (btn)
    {
        case BTN_NEXT:
            if (current_menu->type == MENU_TYPE_LIST)
            {
                (*current_menu->selected_index)++;
                if (*current_menu->selected_index >= current_menu->num_options)
                    *current_menu->selected_index = 0;

                ln = sprintf(stre , "(%s) %s\r\n" , current_menu->title , current_menu->options[*current_menu->selected_index]);
                UART_Send((uint8_t *)stre , ln);
//                gui_render_menu(current_menu);
                gui_render_menu_options(current_menu);
            }
            break;

        case BTN_PREV:
            if (current_menu->type == MENU_TYPE_LIST)
            {
                if (*current_menu->selected_index == 0)
                    *current_menu->selected_index = current_menu->num_options - 1;
                else
                    (*current_menu->selected_index)--;

                ln = sprintf(stre , "(%s) %s\r\n" , current_menu->title , current_menu->options[*current_menu->selected_index]);
                UART_Send((uint8_t *)stre , ln);
//                gui_render_menu(current_menu);
                gui_render_menu_options(current_menu);
            }
            break;

        case BTN_SELECT:
            if (current_menu->type == MENU_TYPE_LIST)
            {
                if (current_menu == &menu_DateTime_Man)
                {
                	datetime_increment(*current_menu->selected_index);

                	current_menu->render(current_menu);

                	return;
                }

                if (current_menu == &menu_DateTime_GPS)
                {
                	if (*current_menu->selected_index == 0)
                	{
                        IncrementTimeZone();

                        current_menu->render(current_menu);

                        return;
                	}
                	else
                    	if (*current_menu->selected_index == 1)
                    	{
                    		update_rtc_from_gps(current_menu);

                            current_menu = current_menu->parent;
                        	Form_DateTime (current_menu);


                            current_menu->render(current_menu);

                            return;
                    	}

                }

                if (current_menu == &menu_GPIO)
                {
                	gpio_toggle(*current_menu->selected_index);
                    return;
                }

                if (current_menu->children)
                {
                    Menu *next = current_menu->children[*current_menu->selected_index];

                    // reset render state
                    next->render_state = 0xFFFFFFFF;

                    current_menu = next;

                    // call on_enter
                    if (next->on_enter)
                        next->on_enter(current_menu);
                }

                ln = sprintf(stre , "%s\r\n" , current_menu->title);
                UART_Send((uint8_t *)stre , ln);
                gui_render_menu(current_menu);
            }
            break;

        case BTN_BACK:
            if (current_menu->parent)
            {
                if (/*current_menu->type == MENU_TYPE_SCREEN &&*/ current_menu->on_exit)
                    current_menu->on_exit();

                current_menu = current_menu->parent;
                current_menu->render_state = 0xFFFFFFFF;

                ln = sprintf(stre , "%s\r\n" , current_menu->title);
                UART_Send((uint8_t *)stre , ln);
                gui_render_menu(current_menu);
            }
            break;
    }
}


void gui_print_data(gui_event_data_t *data)
{
	uint8_t type = data->type;
    switch (type)
    {
        case GUI_GPS_LOC:

            if (current_menu == &menu_GPS_Location)
            {
                form_gps_loc_data(data);
            }
            else
            if ((current_menu == &menu_DateTime_GPS) /*&& (*current_menu->selected_index)*/)
            {
            	form_current_DateTime_from_gps(data);
            }

            break;

        case GUI_GPS_POWER:

            if (current_menu == &menu_GPS_Satellites)
            {
                form_gps_power_data(data);
            }

            break;



    }
}

/************ END OF EXAMPLE ************/
