#include <pebble.h>
#include <stdio.h>
#include <string.h>

/*
DéFitness - Pundamainia
Wensen Liu, Telon Yan, Brian Liang, Sherry Fan
10/1/16-10/2/16

A "fitness/training program in C for Pebble devices."*
*satisfaction not garunteed
*/

static Window * s_window;
static TextLayer * s_text_layer;
static void config_provider(Window *s_window);

static void down_single_click_handler_next(ClickRecognizerRef recognizer, void *context);
static void up_single_click_handler_next(ClickRecognizerRef recognizer, void *context);
static void select_single_click_handler_next(ClickRecognizerRef recognizer, void *context);
static void back_single_click_handler_next(ClickRecognizerRef recognizer, void *context);
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
//static TextLayer *s_time_layer;
//"arrays"
static int windowNumber = 0;
static char * windows[9];
static int difficultyLevel = 1;
static char * challenges[3];
static int challengeNum = 0;
static char * difficulties[3];

//time-related things
static int countSecs = 0;
static int countMins = 0;
static int callClock = 0;
static int difficultyTime = 1;

/*********************************************************************************************************************************************************************************************************************
WINDOW PRINTING        */
static void windowVersion(char * leString){                    //Basic "drawing" method, draws all windows
  // Create a window and get information about the window
	s_window = window_create();
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
	
  // Create a text layer and set the text
	s_text_layer = text_layer_create(bounds);
	text_layer_set_text(s_text_layer, leString);
  
  // Set the font and text alignment
	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
  
  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 10);

	// Push the window, setting the window animation to 'true'
	window_stack_push(s_window, true);
	
	// App Logging!
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed welcome window!");
  window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider);
  
}
/*********************************************************************************************************************************************************************************************************************
BUTTON STUFF ADVANCEMENT        */
static void config_provider(Window *s_window) {
 // single click / repeat-on-hold config:
  challengeNum = 0;
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler_next);
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler_next);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler_next);
    window_single_click_subscribe(BUTTON_ID_BACK, back_single_click_handler_next);
}

//button handling
static void back_single_click_handler_next(ClickRecognizerRef recognizer, void *context) {
  //Window *s_window = (Window *)context;
    if (windowNumber > 0) {
        windowNumber--;
        windowVersion(windows[windowNumber]);
    }
}
static void down_single_click_handler_next(ClickRecognizerRef recognizer, void *context) {
  //Window *s_window = (Window *)context;
  if(windowNumber == 5 || windowNumber == 7 || windowNumber == 2 || windowNumber == 3){
    if(windowNumber == 5){
      callClock = 0;
      windowNumber = 8;
      windowVersion(windows[windowNumber]);
    }
    else if(windowNumber == 2){
      windowNumber++;
      windowVersion(windows[windowNumber]);
      difficultyLevel = 2;
      difficultyTime = 5;
    }
    else if(windowNumber == 3){
      challengeNum = 2;
      windowVersion(strcat(difficulties[difficultyLevel], challenges[challengeNum]));
    }
    else if(windowNumber == 7){
      windowNumber ++;
      windowVersion(windows[windowNumber]);
    }
    else {
      windowNumber++;
      windowVersion(windows[windowNumber]);
      windowNumber = 4;
    }
  }  
  else{         //winNum = {0,1,4,6,>=8}
    window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider);
  }
}
static void up_single_click_handler_next(ClickRecognizerRef recognizer, void *context) {
  //Window *s_window = (Window *)context;
   if(windowNumber == 1 ){
    windowNumber++;
    windowVersion(windows[windowNumber]);
  }
  else if(windowNumber == 8){
    windowNumber++;
    windowNumber %= 9;
    windowVersion(windows[windowNumber]);
  }
  else if(windowNumber == 2){
      windowNumber++;
      windowVersion(windows[windowNumber]);
      difficultyLevel = 0;
      difficultyTime = 1;
    }
  else if(windowNumber == 3){
      challengeNum = 0;
      windowVersion(strcat(difficulties[difficultyLevel], challenges[challengeNum]));
      windowNumber = 4;
    }
  else{
    window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider);
  }
}
static void select_single_click_handler_next(ClickRecognizerRef recognizer, void *context) {
  //Window *s_window = (Window *)context;
   if(windowNumber == 0 || windowNumber == 2 || windowNumber == 3 || windowNumber == 4 || windowNumber == 5 || windowNumber == 6){
    /*if(windowNumber == 1){
      windowVersion("Main Menu\n\nUnder\n\nConstruction");
      windowNumber = 1;
      window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider);
    }*/
    if(windowNumber == 2){
      windowNumber++;
      windowVersion(windows[windowNumber]);
      difficultyLevel = 1;
      difficultyTime = 3;
    }
    else if(windowNumber == 3){
      challengeNum = 1;
      windowVersion(strcat(difficulties[difficultyLevel], challenges[challengeNum]));
      windowNumber++;
      
    }
    else if(windowNumber == 4){
      windowVersion(windows[windowNumber]);
      windowNumber++;
    }
    else if(windowNumber == 5){
      windowVersion(windows[windowNumber]);
      //tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
      callClock = 100;                                             //we start to check things only when we want to, after a button event
      countSecs = 0;
      countMins = 0;
    }
    else {//windowNumber == 0
          challenges[0] = "Butt Kicks\n";                            //yes tabbing is weird...
          challenges[1] = "High Knees\n";
          challenges[2] = "Le Sprinta\n";
          difficulties[0] = "2 MINUTES ";
          difficulties[1] = "5 MINUTES ";
          difficulties[2] = "10 MINUTES ";
          windowNumber++;
          windowVersion(windows[windowNumber]);
          window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider);
       }
  }
    
}
/*********************************************************************************************************************************************************************************************************************
HEALTH HANDLERS, SUBSCRIPTION, ETC (SETUP) */
static void update_time() {
  // Get a tm structure
  //time_t now;
  struct tm info;  
  static char buffer[32];
  //double seconds;
  //time(&now);

    info.tm_mon = 0;                    //just necessary/manual input info
    info.tm_mday = 0;
    info.tm_hour = 0;
    info.tm_min = countMins;
    info.tm_sec = countSecs;  
    
  if(countMins >= difficultyTime) {
    windowNumber ++;
    windowVersion(windows[windowNumber]);
    callClock = 0;
  }
    
  if(countSecs < 59) {            //increment seconds
    countSecs++;
  }
  else {                    //increment minutes
    countMins++;
    countSecs = 0;
  }

  strftime(buffer, sizeof(buffer), "%X", &info );
  
  // Display this time on the TextLayer
  //text_layer_set_text(s_text_layer, strcat(buffer, "\n\nDistance: "));
  windowVersion(strcat(buffer, "\n\n\n\nQuit"));
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  if(callClock == 100){
    update_time();
  }  
}


/*********************************************************************************************************************************************************************************************************************
INIT AND DEINIT AND MAIN            */
static void init(void) {
  s_window = window_create();
  windows[0] = "\n\nWelcome!";
  windows[1]  = "Challenges"; 
  windows[2]  = "Difficulty 1\n\nDifficulty 2\n\nDifficulty 3"; 
  windows[3]  = "Butt Kicks\n\nHigh Knees\n\nSprint"; 
  windows[4]  = "\n\nStart ->";
  windows[5]  = "Time\n\n\n\nQuit"; 
  windows[6]  = "\n\nTIME!"; 
  windows[7]  = "Time: \n\nMax Speed: \n\nNext"; 
  windows[8]  = "Main Menu";
    
  challenges[0] = "Butt Kicks\n";
  challenges[1] = "High Knees\n";
  challenges[2] = "Le Sprinta\n";
  difficulties[0] = "1 MINUTE ";
  difficulties[1] = "3 MINUTES ";
  difficulties[2] = "5 MINUTES ";
    //displays first window, 0 index - welcome screen
  windowVersion(windows[windowNumber]);
  window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(void) {
	// Destroy the text layer
	text_layer_destroy(s_text_layer);
	
	// Destroy the window
	window_destroy(s_window);
  
  //text_layer_destroy(s_time_layer);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}