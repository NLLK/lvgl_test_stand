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

lv_obj_t* gui_widget_room_parameter_delimiter(lv_obj_t* parent){
/*Create an array for the points of the line*/
    static lv_point_precise_t line_points[] = { {0, 5}, {0, 25}};

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_color_hex(0x4D4C4C));
    lv_style_set_line_rounded(&style_line, true);

    /*Create a line and apply the new style*/
    lv_obj_t * line1;
    line1 = lv_line_create(parent);
    lv_line_set_points(line1, line_points, 2);     /*Set the points*/
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_set_size(line1, 2,30);
    return line1;
}

lv_obj_t* gui_widget_room_parameter(lv_obj_t* parent, GUI_RoomParameter paramType, const char* text) {
    // Create the main container (104x30)
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);  // Make background transparent
    lv_obj_remove_style(container, NULL, LV_PART_MAIN);
    lv_obj_set_size(container, 90, 30);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_column(container, 0, 0);  // Remove padding between flex items
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    // Create image (original size 44x44, scaled by 175)
    lv_obj_t* img = lv_img_create(container);
    lv_obj_set_style_bg_opa(img, LV_OPA_TRANSP, 0);
    lv_obj_remove_style(img, NULL, LV_PART_MAIN);
    LV_IMG_DECLARE(co2_icon);
    lv_img_set_src(img, &co2_icon);
    lv_img_set_zoom(img, 155);
    lv_obj_set_size(img, 25,25);

    switch (paramType)
    {
    case GUI_RoomParameter::TEMP:
        LV_IMG_DECLARE(temperature_icon);
        lv_img_set_src(img, &temperature_icon);
        break;
    case GUI_RoomParameter::HUMIDITY:
        LV_IMG_DECLARE(humidity_icon);
        lv_img_set_src(img, &humidity_icon);
        break;
    case GUI_RoomParameter::CO2:
        lv_img_set_zoom(img, 175);
        LV_IMG_DECLARE(co2_icon);
        lv_img_set_src(img, &co2_icon);
        lv_obj_set_size(img, 30,30);
        break;
    default:
        break;
    }
    lv_obj_set_style_pad_all(img, 0, 0);
    lv_obj_set_style_margin_all(img, 0, 0);

    lv_obj_set_style_align(img, LV_ALIGN_LEFT_MID, 0);

    // Create label for text
    lv_obj_t* label = lv_label_create(container);
    lv_obj_set_style_bg_opa(label, LV_OPA_TRANSP, 0);
    lv_obj_remove_style(label, NULL, LV_PART_MAIN);
    lv_obj_set_style_pad_all(label, 0, 0);
    lv_obj_set_style_margin_all(label, 0, 0);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_flex_grow(label, 1);
    // lv_obj_set_style_margin_all(label, 0, 0);

    if (paramType == GUI_RoomParameter::CO2) return container;

    lv_obj_t* label_units = lv_label_create(container);
    lv_obj_set_style_bg_opa(label_units, LV_OPA_TRANSP, 0);
    lv_obj_remove_style(label_units, NULL, LV_PART_MAIN);
    lv_obj_set_style_text_font(label_units, &lv_font_montserrat_20, 0);
    switch (paramType)
    {
    case GUI_RoomParameter::TEMP:
        lv_label_set_text_static(label_units, "°C");
        break;
    case GUI_RoomParameter::HUMIDITY:
        lv_label_set_text_static(label_units, "%");
        break;
    // case GUI_RoomParameter::CO2:
    //     lv_label_set_text_static(label_units, "ppm");
    //     lv_obj_set_style_text_font(label_units, &lv_font_montserrat_12, 0);
    //     break;
    default:
        break;
    }

    lv_obj_set_style_margin_all(label_units, 0, 0);
    lv_obj_set_style_pad_all(label_units, 0, 0);

    return container;
}

int gui_init(){
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
                          font_normal);

    font_large     = &lv_font_montserrat_48;
    font_normal    = &lv_font_montserrat_12;

    lv_obj_set_style_text_font(lv_screen_active(), font_large, 0);
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x0), 0);

    lv_obj_t * label1 = lv_label_create(lv_screen_active());
    lv_label_set_text(label1, "12:34");
    lv_obj_set_style_transform_scale(label1, 500, 0);
    lv_obj_set_pos(label1, 50, 40);

    lv_obj_t* room_parameters = lv_obj_create(lv_screen_active());
    lv_obj_set_style_bg_opa(room_parameters, LV_OPA_TRANSP, 0);  // Make background transparent
    lv_obj_remove_style(room_parameters, NULL, LV_PART_MAIN);
    lv_obj_set_size(room_parameters, 320, 30);
    lv_obj_set_flex_flow(room_parameters, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(room_parameters, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(room_parameters, 0, 0);
    lv_obj_set_style_border_width(room_parameters, 0, 0);
    // lv_obj_set_style_pad_column(room_parameters, 0, 0);
    lv_obj_clear_flag(room_parameters, LV_OBJ_FLAG_SCROLLABLE);

    gui_widget_room_parameter(room_parameters, GUI_RoomParameter::TEMP, "+30");
    gui_widget_room_parameter_delimiter(room_parameters);
    gui_widget_room_parameter(room_parameters, GUI_RoomParameter::HUMIDITY, "99");
    gui_widget_room_parameter_delimiter(room_parameters);
    gui_widget_room_parameter(room_parameters, GUI_RoomParameter::CO2, "1234");
}
