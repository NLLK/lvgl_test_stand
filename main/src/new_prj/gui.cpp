#include "gui.h"

#include "lvgl/src/lvgl_private.h"
#include "lvgl/src/lvgl.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
} disp_size_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static disp_size_t disp_size;

static const lv_font_t * font_large;
static const lv_font_t * font_normal;


class Position{
public:
    Position(uint32_t x, uint32_t y){
        this->x = x;
        this->y = y;
    }
    uint32_t x = 0;
    uint32_t y = 0;

};

enum class GUI_RoomParameter {
    TEMP,
    HUMIDITY,
    CO2
};

// void gui_draw_room_parameter(GUI_RoomParameter paramType, Position pos){

//     LV_IMG_DECLARE(co2_icon);
//     lv_obj_t * img1 = lv_img_create(lv_screen_active());
//     lv_img_set_src(img1, &co2_icon);
//     lv_obj_set_style_transform_scale(img1, 175, 0);
//     lv_obj_set_pos(img1, pos.x, pos.y);

//     static lv_style_t style;
//     lv_style_init(&style);

//     uint16_t spacing = 10;

//     lv_obj_t * label_value = lv_label_create(lv_screen_active());
//     lv_label_set_text(label_value, "333");
//     lv_obj_set_pos(label_value, pos.x + co2_icon.header.w + spacing, pos.y);

//     lv_style_set_text_font(&style,&lv_font_montserrat_20);
//     lv_obj_add_style(label_value, &style, LV_STATE_DEFAULT);
// }

lv_obj_t* gui_draw_room_parameter(lv_obj_t* parent, GUI_RoomParameter paramType, Position pos, const char* text) {
    // Create the main container (104x30)
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, 104, 30);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_column(container, 0, 0);  // Remove padding between flex items
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    // Create image (original size 44x44, scaled by 175)
    lv_obj_t* img = lv_img_create(container);
    LV_IMG_DECLARE(co2_icon);
    lv_img_set_src(img, &co2_icon);

    lv_img_set_zoom(img, 175);
    lv_obj_set_size(img, 25,25);
    lv_obj_set_style_pad_all(img, 0, 0);
    lv_obj_set_style_margin_all(img, 0, 0);

    // Center the image vertically
    lv_obj_set_style_align(img, LV_ALIGN_LEFT_MID, 0);

    // Create label for text
    lv_obj_t* label = lv_label_create(container);
    lv_obj_set_style_pad_all(label, 0, 0);
    lv_obj_set_style_margin_all(label, 0, 0);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_flex_grow(label, 1);
    // lv_obj_set_style_margin_all(label, 0, 0);

    lv_obj_t* label_units = lv_label_create(container);
    lv_label_set_text_static(label_units, "ppm");
    lv_obj_set_style_text_font(label_units, &lv_font_montserrat_12, 0);
    lv_obj_set_style_margin_all(label_units, 0, 0);
    lv_obj_set_style_pad_all(label_units, 0, 0);
    // lv_obj_set_style_pad_left(label_units, -5, 0);  // Add some space between image and text


    return container;
}

int gui_init(){
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
                          font_normal);

    font_large     = &lv_font_montserrat_48;
    font_normal    = &lv_font_montserrat_12;

    lv_obj_set_style_text_font(lv_screen_active(), font_large, 0);

    lv_obj_t * label1 = lv_label_create(lv_screen_active());
    lv_label_set_text(label1, "12:34");
    lv_obj_set_style_transform_scale(label1, 500, 0);
    lv_obj_set_pos(label1, 50, 40);


    gui_draw_room_parameter(lv_screen_active(),GUI_RoomParameter::TEMP, Position(0,0), "1234");
    // lv_obj_t * label1 = lv_label_create(lv_screen_active());
    // lv_label_set_long_mode(label1, LV_LABEL_LONG_MODE_WRAP);     /*Break the long lines*/
    // lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    // lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label, align the lines to the center "
    //                   "and wrap long text automatically.");
    // lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
    // lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    // lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);

    // lv_obj_t * label2 = lv_label_create(lv_screen_active());
    // lv_label_set_long_mode(label2, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);     /*Circular scroll*/
    // lv_obj_set_width(label2, 150);
    // lv_label_set_text(label2, "It is a circularly scrolling text. ");
    // lv_obj_align(label2, LV_ALIGN_CENTER, 0, 40);

}
