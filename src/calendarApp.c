#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include <math.h>
#include "resource_ids.auto.h"

#define MY_UUID { 0xB4, 0x1E, 0x3D, 0xCF, 0x61, 0x62, 0x41, 0x47, 0x9C, 0x58, 0x64, 0x3E, 0x10, 0x91, 0xFB, 0x93 }
PBL_APP_INFO(MY_UUID,
             "Calendar", "William Heaton",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_STANDARD_APP);

static int offset = 0;

Window window;

Layer month_layer;
Layer days_layer;

const bool black = false;  // Is the background black
const bool grid = true; // show the grid
const bool invert = true; // Invert colors on today's date

const char daysOfWeek[7][3] = {"S","M","T","W","Th","F","S"};
const char months[12][12] = {"January","Feburary","March","April","May","June","July","August","September","October", "November", "December"};


//TODO  This function is really emparissing - there as got to be a better way
char* intToStr(int i){
    if(i==0) return "0";
    if(i==1) return "1";
    if(i==2) return "2";
    if(i==3) return "3";
    if(i==4) return "4";
    if(i==5) return "5";
    if(i==6) return "6";
    if(i==7) return "7";
    if(i==8) return "8";
    if(i==9) return "9";
    if(i==10) return "10";
    if(i==11) return "11";
    if(i==12) return "12";
    if(i==13) return "13";
    if(i==14) return "14";
    if(i==15) return "15";
    if(i==16) return "16";
    if(i==17) return "17";
    if(i==18) return "18";
    if(i==19) return "19";
    if(i==20) return "20";
    if(i==21) return "21";
    if(i==22) return "22";
    if(i==23) return "23";
    if(i==24) return "24";
    if(i==25) return "25";
    if(i==26) return "26";
    if(i==27) return "27";
    if(i==28) return "28";
    if(i==29) return "29";
    if(i==30) return "30";
    if(i==31) return "31";
    if(i==32) return "32";
    if(i==33) return "33";
    if(i==34) return "34";
    if(i==35) return "35";
    if(i==36) return "36";
    if(i==37) return "37";
    if(i==38) return "38";
    if(i==39) return "39";
    if(i==40) return "40";
    if(i==41) return "41";
    if(i==42) return "42";
    if(i==43) return "43";
    if(i==44) return "44";
    if(i==45) return "45";
    if(i==46) return "46";
    if(i==47) return "47";
    if(i==48) return "48";
    if(i==49) return "49";
    if(i==50) return "50";
    if(i==51) return "51";
    if(i==52) return "52";
    if(i==53) return "53";
    if(i==54) return "54";
    if(i==55) return "55";
    if(i==56) return "56";
    if(i==57) return "57";
    if(i==58) return "58";
    if(i==59) return "59";
    if(i==50) return "50";
    if(i==51) return "51";
    if(i==52) return "52";
    if(i==53) return "53";
    if(i==54) return "54";
    if(i==55) return "55";
    if(i==56) return "56";
    if(i==57) return "57";
    if(i==58) return "58";
    if(i==59) return "59";
    if(i==60) return "60";
    if(i==61) return "61";
    if(i==62) return "62";
    if(i==63) return "63";
    if(i==64) return "64";
    if(i==65) return "65";
    if(i==66) return "66";
    if(i==67) return "57";
    if(i==68) return "68";
    if(i==69) return "69";
    if(i==70) return "70";
    if(i==71) return "71";
    if(i==72) return "72";
    if(i==73) return "73";
    if(i==74) return "74";
    if(i==75) return "75";
    if(i==76) return "76";
    if(i==77) return "77";
    if(i==78) return "78";
    if(i==79) return "79";
    if(i==80) return "80";
    if(i==81) return "81";
    if(i==82) return "82";
    if(i==83) return "83";
    if(i==84) return "84";
    if(i==85) return "85";
    if(i==86) return "86";
    if(i==87) return "87";
    if(i==88) return "88";
    if(i==89) return "89";
    if(i==90) return "90";
    if(i==91) return "91";
    if(i==92) return "92";
    if(i==93) return "93";
    if(i==94) return "94";
    if(i==95) return "95";
    if(i==96) return "96";
    if(i==97) return "97";
    if(i==98) return "98";
    if(i==99) return "99";
    
    return "error";
}
int wdayOfFirst(int wday,int mday){
    int a = wday - ((mday-1)%7);
    if(a<0) a = a+7;
    return a;
}
int wdayOfFirstOffset(int wday,int mday,int ofs){
    int a = wday - ((mday-1)%7);
    if(a<0) a = a+7;

    int b;
    if(ofs>0)
        b = a + (abs(ofs)%7); 
    else
        b = a - (abs(ofs)%7); 
    
    if(b<0) b = b+7;
    if(b>6) b = b-7;
    
    return b;
}
int daysInMonth(int mon, int year){
    mon = mon+1;
    if(mon == 4 || mon == 6 || mon == 9 || mon == 11)
        return 30;
    else if( mon == 2 ){
        if(year%400==0)
            return 29;
        else if(year%100==0)
            return 28;
        else if(year%4==0)
            return 29;
        else 
            return 28;
    }
    else
        return 31;
}
void setColors(GContext* ctx){
    if(black){
        window_set_background_color(&window, GColorBlack);
        graphics_context_set_stroke_color(ctx, GColorWhite);
        graphics_context_set_fill_color(ctx, GColorBlack);
        graphics_context_set_text_color(ctx, GColorWhite);
    }else{
        window_set_background_color(&window, GColorWhite);
        graphics_context_set_stroke_color(ctx, GColorBlack);
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_context_set_text_color(ctx, GColorBlack);
    }
}
void setInvColors(GContext* ctx){
    if(!black){
        graphics_context_set_stroke_color(ctx, GColorWhite);
        graphics_context_set_fill_color(ctx, GColorBlack);
        graphics_context_set_text_color(ctx, GColorWhite);
    }else{
        window_set_background_color(&window, GColorWhite);
        graphics_context_set_stroke_color(ctx, GColorBlack);
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_context_set_text_color(ctx, GColorBlack);
    }
}
void days_layer_update_callback(Layer *me, GContext* ctx) {
    (void)me;
    
    
    setColors(ctx);
    
    PblTm currentTime;
    get_time(&currentTime);
    
    int mon = currentTime.tm_mon;
    int year = currentTime.tm_year+1900;
    
    int j = 0;
    int od = 0;
    while(j < abs(offset)){
        j++;
        if(offset > 0){
            od = od + daysInMonth(mon,year);
            mon++;
            if(mon>11){
                mon = mon - 12;
                year = year+1;
            }
        }else{
            mon--;
            if(mon < 0){
                mon = mon + 12;
                year = year - 1;
            }
            od = od - daysInMonth(mon,year);
        }
    }
    
    int dom = daysInMonth(mon,year);
    int dow = wdayOfFirstOffset(currentTime.tm_wday,currentTime.tm_mday,od);
    
    
    int l = 2;      // position of left side of left column
    int b = 167;    // position of bottom of bottom row
    int d = 7;      // number of columns (days of the week)
    int lw = 20;    // width of cells 
    int w = ceil(((float) dow + (float) dom)/7);
    int bh;
    
    if(w == 4)
        bh = 30;
    else if(w == 5)
        bh = 24;
    else
        bh = 20;
    
    int r = l+d*lw; // position of right side of right column (calculated)
    int t = b-w*bh; // position of top of top row (calculated)
    int i;
    
    setColors(ctx);
    if(grid){
        // draw horizontal lines
        for(i=1;i<=w;i++){
            graphics_draw_line(ctx, GPoint(l, b-i*bh), GPoint(r, b-i*bh));
        }
        // draw vertical lines
        for(i=1;i<d;i++){
            graphics_draw_line(ctx, GPoint(l+i*lw, t), GPoint(l+i*lw, b));
        }
    }
    // draw days of week
    for(i=0;i<7;i++){
        graphics_text_draw(ctx, daysOfWeek[i],  fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(l+i*lw+1, 30, lw-1, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL); 
    }
    
    
    int wknum = 0;
    dow = dow - 1;
    int fh;
    GFont font;
    
    for(i=1;i<=dom;i++){
        dow++;
        if(dow > 6){
            dow = 0;
            wknum ++;
        }

        
        if(i==currentTime.tm_mday && offset == 0){
            if(invert){
                setInvColors(ctx);
                graphics_fill_rect(ctx,GRect(l+dow*lw+1, b-(w-wknum)*bh+1, lw-1, bh-1),0,GCornerNone);
            }
            font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
            fh = 20;

        }else{
            font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
            fh = 16;
        }
        graphics_text_draw(ctx, intToStr(i),  font, GRect(l+dow*lw+1, b-(-0.5+w-wknum)*bh-fh/2-1, lw-1, fh), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL); 
        
        if(invert)
            setColors(ctx);       
    }
}

void month_layer_update_callback(Layer *me, GContext* ctx) {
    (void)me;
    PblTm currentTime;
    get_time(&currentTime);
    
    setColors(ctx);
    
    int mon = currentTime.tm_mon+offset;
    int year = currentTime.tm_year+1900;
    
    while(mon > 11 || mon < 0){
        if(mon>11){
            mon = mon - 12;
            year = year+1;
        }else if(mon < 0){
            mon = mon + 12;
            year = year - 1;
        }
    }
    
    char str[80];
    strcpy (str,months[mon]);
    strcat (str," ");
    strcat (str,intToStr((year/100)));
    if((year%100)<10)
        strcat (str,intToStr(0));
        
    strcat (str,intToStr((year%100)));
    
    graphics_text_draw(ctx, str,  fonts_get_system_font(FONT_KEY_GOTHIC_24), GRect(0, 0, 144, 30), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}



void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  (void)recognizer;
  (void)window;

    offset = offset - 1;
    
    layer_mark_dirty(&month_layer);
    layer_mark_dirty(&days_layer);
}


void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    offset = offset + 1;
    
    layer_mark_dirty(&month_layer);
    layer_mark_dirty(&days_layer);
}

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
    (void)recognizer;
    (void)window;
    offset = 0;
    
    layer_mark_dirty(&month_layer);
    layer_mark_dirty(&days_layer);
}

void config_provider(ClickConfig **config, Window *window) {
  (void)window;


  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
  
  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;

  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
  config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

    window_init(&window, "Calendar");
    window_stack_push(&window, true /* Animated */);
    window_set_fullscreen(&window, true);
    
    setColors(ctx);
    
    layer_init(&month_layer, window.layer.frame);
    month_layer.update_proc = &month_layer_update_callback;
    layer_add_child(&window.layer, &month_layer);

    layer_init(&days_layer, window.layer.frame);
    days_layer.update_proc = &days_layer_update_callback;
    layer_add_child(&window.layer, &days_layer);
    
    window_set_click_config_provider(&window, (ClickConfigProvider) config_provider);
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *t) {
    (void)ctx;
    layer_mark_dirty(&month_layer);
    layer_mark_dirty(&days_layer);
}
void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    .tick_info = {
        .tick_handler = &handle_tick,
        .tick_units = DAY_UNIT
    }
  };
  app_event_loop(params, &handlers);
}


