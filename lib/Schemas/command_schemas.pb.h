/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.5 */

#ifndef PB_COMMAND_SCHEMAS_PB_H_INCLUDED
#define PB_COMMAND_SCHEMAS_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _ColorPalette { 
    ColorPalette_RAINBOW = 0, 
    ColorPalette_FOREST = 1, 
    ColorPalette_OCEAN = 2, 
    ColorPalette_PARTY = 3, 
    ColorPalette_HEAT = 4, 
    ColorPalette_SPRING_ANGEL = 5, 
    ColorPalette_SCOUTY = 6, 
    ColorPalette_PURPLE_HEAT = 7, 
    ColorPalette_LAVA = 8, 
    ColorPalette_CLOUD = 9, 
    /* parrot = 8; */
    ColorPalette_SAGA = 10, 
    ColorPalette_SAGA2 = 11, 
    /* sunset */
    ColorPalette_SUNSET = 12, 
    /* rivendell */
    ColorPalette_RIVENDELL = 13, 
    /* ocean breeze */
    ColorPalette_OCEAN_BREEZE = 14, 
    /* 50_shades_of_pink
 shades_of_pink = 14;
 sun */
    ColorPalette_SUN = 15, 
    /* Analogous_1 = 16;
 pinksplash
 pinksplash = 17;
 Coral_reef = 18;
 es_ocean_breeze_068 = 19; */
    ColorPalette_SHADES_OF_PINK = 20, 
    /* es_vintage_01 = 21;
 departure = 22; */
    ColorPalette_LANDSCAPE = 23, 
    /* es_landscape_33 = 24; */
    ColorPalette_RAINBOWSHERBET = 25, 
    ColorPalette_BLUE_PINK = 26, 
    ColorPalette_HULT = 27, 
    /* GMT_drywet = 28; */
    ColorPalette_JUL01 = 29, 
    /* es_vintage_57 = 30;
 ib15 = 31; */
    ColorPalette_FUSCHIA = 32, 
    /* es_emerald_dragon_08 = 33;
 lava = 34;
 fire = 35; */
    ColorPalette_COLORFULL = 36, 
    /* Magenta_Evening = 37;
 Pink_Purple = 38;
 autumn = 39;
 BlacK_Blue_Magenta_White = 40;
 BlacK_Magenta_Red = 41;
 BlacK_Red_Magenta_Yellow = 42; */
    ColorPalette_BLUE_CYAN_YELLOW = 43 
} ColorPalette;

typedef enum _Shape1D { 
    Shape1D_BALL = 0, 
    Shape1D_COMET = 1, 
    Shape1D_COMET_BOUNCE = 2, 
    Shape1D_NYAN = 3, 
    Shape1D_NYAN_BOUNCE = 4, 
    Shape1D_SOLID = 5 
} Shape1D;

typedef enum _Shape2D { 
    Shape2D_SQUARE = 0, 
    Shape2D_CIRCLE = 1, 
    Shape2D_RING = 2 
} Shape2D;

typedef enum _MovementType { 
    MovementType_LINEAR = 0, 
    MovementType_SINE = 1, 
    MovementType_SAWTOOTH = 2 
} MovementType;

typedef enum _SlopeType { 
    SlopeType_LINE = 0, 
    SlopeType_FILL = 1, 
    SlopeType_COLOR_SHIFT = 2 
} SlopeType;

/* Struct definitions */
typedef struct _Kaleidoscope { 
    char dummy_field;
} Kaleidoscope;

typedef struct _Twang { 
    char dummy_field;
} Twang;

typedef struct _FlickerSparkle { 
    bool has_color;
    int32_t color; 
    bool has_sparkle_width;
    int32_t sparkle_width; 
    bool has_sparles_per_second;
    int32_t sparles_per_second; 
    bool has_duration;
    int32_t duration; 
    bool has_flicker_delay;
    int32_t flicker_delay; 
    bool has_flicker_window;
    int32_t flicker_window; 
    bool has_max_flickers;
    int32_t max_flickers; 
} FlickerSparkle;

typedef struct _Gravity { 
    bool has_color;
    int32_t color; 
    bool has_launch_speed;
    int32_t launch_speed; 
    bool has_ball_rate;
    int32_t ball_rate; 
    bool has_width;
    int32_t width; 
} Gravity;

typedef struct _MappedParticles { 
    bool has_color;
    int32_t color; 
    bool has_shape;
    Shape2D shape; 
    bool has_x;
    int32_t x; 
    bool has_y;
    int32_t y; 
    bool has_size;
    int32_t size; 
    bool has_fade_distance;
    int32_t fade_distance; 
} MappedParticles;

typedef struct _MappedPingPong { 
    bool has_color;
    int32_t color; 
    bool has_direction;
    int32_t direction; 
    bool has_fade_distance;
    int32_t fade_distance; 
} MappedPingPong;

typedef struct _MappedShape { 
    bool has_color;
    int32_t color; 
    bool has_shape;
    Shape2D shape; 
    bool has_x;
    int32_t x; 
    bool has_y;
    int32_t y; 
    bool has_size;
    int32_t size; 
    bool has_fade_distance;
    int32_t fade_distance; 
} MappedShape;

typedef struct _MappedSlope { 
    bool has_color;
    int32_t color; 
    bool has_slope_type;
    SlopeType slope_type; 
    bool has_x1;
    int32_t x1; 
    bool has_y1;
    int32_t y1; 
    bool has_x2;
    int32_t x2; 
    bool has_y2;
    int32_t y2; 
    bool has_fade_distance;
    int32_t fade_distance; 
} MappedSlope;

typedef struct _MappedTriangle { 
    bool has_color;
    int32_t color; 
    bool has_x1;
    int32_t x1; 
    bool has_y1;
    int32_t y1; 
    bool has_x2;
    int32_t x2; 
    bool has_y2;
    int32_t y2; 
    bool has_x3;
    int32_t x3; 
    bool has_y3;
    int32_t y3; 
} MappedTriangle;

typedef struct _PingPong { 
    bool has_shape;
    Shape1D shape; 
    bool has_movement;
    MovementType movement; 
    bool has_color;
    int32_t color; 
    bool has_width;
    int32_t width; 
    bool has_max_height;
    int32_t max_height; 
    bool has_count;
    int32_t count; 
    bool has_period_ms;
    int32_t period_ms; 
    bool has_offset_ms;
    int32_t offset_ms; 
} PingPong;

typedef struct _Pixel { 
    bool has_color;
    int32_t color; 
    bool has_led_index;
    int32_t led_index; 
} Pixel;

typedef struct _PixelRGB { 
    bool has_red;
    int32_t red; 
    bool has_green;
    int32_t green; 
    bool has_blue;
    int32_t blue; 
    bool has_led_index;
    int32_t led_index; 
} PixelRGB;

typedef struct _Rain { 
    bool has_color;
    int32_t color; 
    bool has_width;
    int32_t width; 
    bool has_drop_density;
    int32_t drop_density; 
    bool has_drop_speed;
    int32_t drop_speed; 
} Rain;

typedef struct _Rainbow { 
    bool has_speed;
    int32_t speed; 
    bool has_wave_size;
    int32_t wave_size; 
    bool has_rainbow_size;
    int32_t rainbow_size; 
} Rainbow;

typedef struct _Render { 
    bool has_shape;
    Shape1D shape; 
    bool has_color;
    int32_t color; 
    bool has_position;
    int32_t position; 
    bool has_width;
    int32_t width; 
} Render;

typedef struct _SingleColor { 
    bool has_color;
    int32_t color; 
} SingleColor;

typedef struct _Sparkle { 
    bool has_color;
    int32_t color; 
    bool has_sparkle_width;
    int32_t sparkle_width; 
    bool has_sparle_rate;
    int32_t sparle_rate; 
    bool has_duration;
    int32_t duration; 
} Sparkle;

typedef struct _White { 
    bool has_color_temperature;
    int32_t color_temperature; 
} White;

typedef struct _CommandParams { 
    int32_t index; 
    bool has_enabled;
    bool enabled; 
    bool has_brightness;
    int32_t brightness; 
    bool has_strip_index;
    int32_t strip_index; 
    bool has_color_palette;
    ColorPalette color_palette; 
    pb_size_t which_type_params;
    union {
        White white;
        SingleColor single_color;
        Pixel pixel;
        PixelRGB pixel_rgb;
        Rainbow rainbow;
        Sparkle sparkle;
        FlickerSparkle flicker_sparkle;
        PingPong ping_pong;
        Render render;
        Rain rain;
        Gravity gravity;
        Kaleidoscope kaleidoscope;
        MappedShape mapped_shape;
        MappedSlope mapped_slope;
        MappedTriangle mapped_triangle;
        MappedParticles mapped_particles;
        MappedPingPong mapped_ping_pong;
        Twang twang;
    } type_params; 
} CommandParams;


/* Helper constants for enums */
#define _ColorPalette_MIN ColorPalette_RAINBOW
#define _ColorPalette_MAX ColorPalette_BLUE_CYAN_YELLOW
#define _ColorPalette_ARRAYSIZE ((ColorPalette)(ColorPalette_BLUE_CYAN_YELLOW+1))

#define _Shape1D_MIN Shape1D_BALL
#define _Shape1D_MAX Shape1D_SOLID
#define _Shape1D_ARRAYSIZE ((Shape1D)(Shape1D_SOLID+1))

#define _Shape2D_MIN Shape2D_SQUARE
#define _Shape2D_MAX Shape2D_RING
#define _Shape2D_ARRAYSIZE ((Shape2D)(Shape2D_RING+1))

#define _MovementType_MIN MovementType_LINEAR
#define _MovementType_MAX MovementType_SAWTOOTH
#define _MovementType_ARRAYSIZE ((MovementType)(MovementType_SAWTOOTH+1))

#define _SlopeType_MIN SlopeType_LINE
#define _SlopeType_MAX SlopeType_COLOR_SHIFT
#define _SlopeType_ARRAYSIZE ((SlopeType)(SlopeType_COLOR_SHIFT+1))


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define CommandParams_init_default               {0, false, true, false, 255, false, 0, false, ColorPalette_RAINBOW, 0, {White_init_default}}
#define White_init_default                       {false, 0}
#define SingleColor_init_default                 {false, 226}
#define Pixel_init_default                       {false, 0, false, 0}
#define PixelRGB_init_default                    {false, 130, false, 130, false, 130, false, 0}
#define Rainbow_init_default                     {false, 30, false, 100, false, 150}
#define Sparkle_init_default                     {false, 1, false, 15, false, 10, false, 100}
#define FlickerSparkle_init_default              {false, 255, false, 90, false, 215, false, 8, false, 7, false, 119, false, 80}
#define PingPong_init_default                    {false, Shape1D_BALL, false, MovementType_LINEAR, false, 65, false, 20, false, 255, false, 1, false, 6000, false, 0}
#define Render_init_default                      {false, Shape1D_BALL, false, 210, false, 20, false, 20}
#define Rain_init_default                        {false, 1, false, 15, false, 10, false, 10}
#define Gravity_init_default                     {false, 13, false, 0, false, 5, false, 70}
#define Kaleidoscope_init_default                {0}
#define MappedSlope_init_default                 {false, 1, false, SlopeType_FILL, false, 0, false, 0, false, 255, false, 255, false, 5}
#define MappedShape_init_default                 {false, 1, false, Shape2D_SQUARE, false, 128, false, 128, false, 50, false, 50}
#define MappedTriangle_init_default              {false, 1, false, 0, false, 0, false, 255, false, 0, false, 128, false, 255}
#define MappedParticles_init_default             {false, 177, false, Shape2D_CIRCLE, false, 128, false, 128, false, 50, false, 50}
#define MappedPingPong_init_default              {false, 123, false, 0, false, 5}
#define Twang_init_default                       {0}
#define CommandParams_init_zero                  {0, false, 0, false, 0, false, 0, false, _ColorPalette_MIN, 0, {White_init_zero}}
#define White_init_zero                          {false, 0}
#define SingleColor_init_zero                    {false, 0}
#define Pixel_init_zero                          {false, 0, false, 0}
#define PixelRGB_init_zero                       {false, 0, false, 0, false, 0, false, 0}
#define Rainbow_init_zero                        {false, 0, false, 0, false, 0}
#define Sparkle_init_zero                        {false, 0, false, 0, false, 0, false, 0}
#define FlickerSparkle_init_zero                 {false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0}
#define PingPong_init_zero                       {false, _Shape1D_MIN, false, _MovementType_MIN, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0}
#define Render_init_zero                         {false, _Shape1D_MIN, false, 0, false, 0, false, 0}
#define Rain_init_zero                           {false, 0, false, 0, false, 0, false, 0}
#define Gravity_init_zero                        {false, 0, false, 0, false, 0, false, 0}
#define Kaleidoscope_init_zero                   {0}
#define MappedSlope_init_zero                    {false, 0, false, _SlopeType_MIN, false, 0, false, 0, false, 0, false, 0, false, 0}
#define MappedShape_init_zero                    {false, 0, false, _Shape2D_MIN, false, 0, false, 0, false, 0, false, 0}
#define MappedTriangle_init_zero                 {false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0}
#define MappedParticles_init_zero                {false, 0, false, _Shape2D_MIN, false, 0, false, 0, false, 0, false, 0}
#define MappedPingPong_init_zero                 {false, 0, false, 0, false, 0}
#define Twang_init_zero                          {0}

/* Field tags (for use in manual encoding/decoding) */
#define FlickerSparkle_color_tag                 1
#define FlickerSparkle_sparkle_width_tag         2
#define FlickerSparkle_sparles_per_second_tag    3
#define FlickerSparkle_duration_tag              4
#define FlickerSparkle_flicker_delay_tag         5
#define FlickerSparkle_flicker_window_tag        6
#define FlickerSparkle_max_flickers_tag          7
#define Gravity_color_tag                        1
#define Gravity_launch_speed_tag                 2
#define Gravity_ball_rate_tag                    3
#define Gravity_width_tag                        4
#define MappedParticles_color_tag                1
#define MappedParticles_shape_tag                2
#define MappedParticles_x_tag                    3
#define MappedParticles_y_tag                    4
#define MappedParticles_size_tag                 5
#define MappedParticles_fade_distance_tag        6
#define MappedPingPong_color_tag                 1
#define MappedPingPong_direction_tag             2
#define MappedPingPong_fade_distance_tag         3
#define MappedShape_color_tag                    1
#define MappedShape_shape_tag                    2
#define MappedShape_x_tag                        3
#define MappedShape_y_tag                        4
#define MappedShape_size_tag                     5
#define MappedShape_fade_distance_tag            6
#define MappedSlope_color_tag                    1
#define MappedSlope_slope_type_tag               2
#define MappedSlope_x1_tag                       3
#define MappedSlope_y1_tag                       4
#define MappedSlope_x2_tag                       5
#define MappedSlope_y2_tag                       6
#define MappedSlope_fade_distance_tag            7
#define MappedTriangle_color_tag                 1
#define MappedTriangle_x1_tag                    2
#define MappedTriangle_y1_tag                    3
#define MappedTriangle_x2_tag                    4
#define MappedTriangle_y2_tag                    5
#define MappedTriangle_x3_tag                    6
#define MappedTriangle_y3_tag                    7
#define PingPong_shape_tag                       1
#define PingPong_movement_tag                    2
#define PingPong_color_tag                       3
#define PingPong_width_tag                       4
#define PingPong_max_height_tag                  5
#define PingPong_count_tag                       6
#define PingPong_period_ms_tag                   7
#define PingPong_offset_ms_tag                   8
#define Pixel_color_tag                          1
#define Pixel_led_index_tag                      2
#define PixelRGB_red_tag                         1
#define PixelRGB_green_tag                       2
#define PixelRGB_blue_tag                        3
#define PixelRGB_led_index_tag                   4
#define Rain_color_tag                           1
#define Rain_width_tag                           2
#define Rain_drop_density_tag                    3
#define Rain_drop_speed_tag                      4
#define Rainbow_speed_tag                        1
#define Rainbow_wave_size_tag                    2
#define Rainbow_rainbow_size_tag                 3
#define Render_shape_tag                         1
#define Render_color_tag                         2
#define Render_position_tag                      3
#define Render_width_tag                         4
#define SingleColor_color_tag                    1
#define Sparkle_color_tag                        1
#define Sparkle_sparkle_width_tag                2
#define Sparkle_sparle_rate_tag                  3
#define Sparkle_duration_tag                     4
#define White_color_temperature_tag              1
#define CommandParams_index_tag                  1
#define CommandParams_enabled_tag                2
#define CommandParams_brightness_tag             3
#define CommandParams_strip_index_tag            4
#define CommandParams_color_palette_tag          5
#define CommandParams_white_tag                  6
#define CommandParams_single_color_tag           7
#define CommandParams_pixel_tag                  8
#define CommandParams_pixel_rgb_tag              9
#define CommandParams_rainbow_tag                10
#define CommandParams_sparkle_tag                11
#define CommandParams_flicker_sparkle_tag        12
#define CommandParams_ping_pong_tag              13
#define CommandParams_render_tag                 14
#define CommandParams_rain_tag                   15
#define CommandParams_gravity_tag                16
#define CommandParams_kaleidoscope_tag           17
#define CommandParams_mapped_shape_tag           18
#define CommandParams_mapped_slope_tag           19
#define CommandParams_mapped_triangle_tag        20
#define CommandParams_mapped_particles_tag       21
#define CommandParams_mapped_ping_pong_tag       22
#define CommandParams_twang_tag                  23

/* Struct field encoding specification for nanopb */
#define CommandParams_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, INT32,    index,             1) \
X(a, STATIC,   OPTIONAL, BOOL,     enabled,           2) \
X(a, STATIC,   OPTIONAL, INT32,    brightness,        3) \
X(a, STATIC,   OPTIONAL, INT32,    strip_index,       4) \
X(a, STATIC,   OPTIONAL, UENUM,    color_palette,     5) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,white,type_params.white),   6) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,single_color,type_params.single_color),   7) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,pixel,type_params.pixel),   8) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,pixel_rgb,type_params.pixel_rgb),   9) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,rainbow,type_params.rainbow),  10) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,sparkle,type_params.sparkle),  11) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,flicker_sparkle,type_params.flicker_sparkle),  12) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,ping_pong,type_params.ping_pong),  13) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,render,type_params.render),  14) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,rain,type_params.rain),  15) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,gravity,type_params.gravity),  16) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,kaleidoscope,type_params.kaleidoscope),  17) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,mapped_shape,type_params.mapped_shape),  18) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,mapped_slope,type_params.mapped_slope),  19) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,mapped_triangle,type_params.mapped_triangle),  20) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,mapped_particles,type_params.mapped_particles),  21) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,mapped_ping_pong,type_params.mapped_ping_pong),  22) \
X(a, STATIC,   ONEOF,    MESSAGE,  (type_params,twang,type_params.twang),  23)
#define CommandParams_CALLBACK NULL
#define CommandParams_DEFAULT (const pb_byte_t*)"\x10\x01\x18\xff\x01\x20\x00\x00"
#define CommandParams_type_params_white_MSGTYPE White
#define CommandParams_type_params_single_color_MSGTYPE SingleColor
#define CommandParams_type_params_pixel_MSGTYPE Pixel
#define CommandParams_type_params_pixel_rgb_MSGTYPE PixelRGB
#define CommandParams_type_params_rainbow_MSGTYPE Rainbow
#define CommandParams_type_params_sparkle_MSGTYPE Sparkle
#define CommandParams_type_params_flicker_sparkle_MSGTYPE FlickerSparkle
#define CommandParams_type_params_ping_pong_MSGTYPE PingPong
#define CommandParams_type_params_render_MSGTYPE Render
#define CommandParams_type_params_rain_MSGTYPE Rain
#define CommandParams_type_params_gravity_MSGTYPE Gravity
#define CommandParams_type_params_kaleidoscope_MSGTYPE Kaleidoscope
#define CommandParams_type_params_mapped_shape_MSGTYPE MappedShape
#define CommandParams_type_params_mapped_slope_MSGTYPE MappedSlope
#define CommandParams_type_params_mapped_triangle_MSGTYPE MappedTriangle
#define CommandParams_type_params_mapped_particles_MSGTYPE MappedParticles
#define CommandParams_type_params_mapped_ping_pong_MSGTYPE MappedPingPong
#define CommandParams_type_params_twang_MSGTYPE Twang

#define White_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color_temperature,   1)
#define White_CALLBACK NULL
#define White_DEFAULT (const pb_byte_t*)"\x08\x00\x00"

#define SingleColor_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1)
#define SingleColor_CALLBACK NULL
#define SingleColor_DEFAULT (const pb_byte_t*)"\x08\xe2\x01\x00"

#define Pixel_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, INT32,    led_index,         2)
#define Pixel_CALLBACK NULL
#define Pixel_DEFAULT (const pb_byte_t*)"\x08\x00\x10\x00\x00"

#define PixelRGB_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    red,               1) \
X(a, STATIC,   OPTIONAL, INT32,    green,             2) \
X(a, STATIC,   OPTIONAL, INT32,    blue,              3) \
X(a, STATIC,   OPTIONAL, INT32,    led_index,         4)
#define PixelRGB_CALLBACK NULL
#define PixelRGB_DEFAULT (const pb_byte_t*)"\x08\x82\x01\x10\x82\x01\x18\x82\x01\x20\x00\x00"

#define Rainbow_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    speed,             1) \
X(a, STATIC,   OPTIONAL, INT32,    wave_size,         2) \
X(a, STATIC,   OPTIONAL, INT32,    rainbow_size,      3)
#define Rainbow_CALLBACK NULL
#define Rainbow_DEFAULT (const pb_byte_t*)"\x08\x1e\x10\x64\x18\x96\x01\x00"

#define Sparkle_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, INT32,    sparkle_width,     2) \
X(a, STATIC,   OPTIONAL, INT32,    sparle_rate,       3) \
X(a, STATIC,   OPTIONAL, INT32,    duration,          4)
#define Sparkle_CALLBACK NULL
#define Sparkle_DEFAULT (const pb_byte_t*)"\x08\x01\x10\x0f\x18\x0a\x20\x64\x00"

#define FlickerSparkle_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, INT32,    sparkle_width,     2) \
X(a, STATIC,   OPTIONAL, INT32,    sparles_per_second,   3) \
X(a, STATIC,   OPTIONAL, INT32,    duration,          4) \
X(a, STATIC,   OPTIONAL, INT32,    flicker_delay,     5) \
X(a, STATIC,   OPTIONAL, INT32,    flicker_window,    6) \
X(a, STATIC,   OPTIONAL, INT32,    max_flickers,      7)
#define FlickerSparkle_CALLBACK NULL
#define FlickerSparkle_DEFAULT (const pb_byte_t*)"\x08\xff\x01\x10\x5a\x18\xd7\x01\x20\x08\x28\x07\x30\x77\x38\x50\x00"

#define PingPong_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, UENUM,    shape,             1) \
X(a, STATIC,   OPTIONAL, UENUM,    movement,          2) \
X(a, STATIC,   OPTIONAL, INT32,    color,             3) \
X(a, STATIC,   OPTIONAL, INT32,    width,             4) \
X(a, STATIC,   OPTIONAL, INT32,    max_height,        5) \
X(a, STATIC,   OPTIONAL, INT32,    count,             6) \
X(a, STATIC,   OPTIONAL, INT32,    period_ms,         7) \
X(a, STATIC,   OPTIONAL, INT32,    offset_ms,         8)
#define PingPong_CALLBACK NULL
#define PingPong_DEFAULT (const pb_byte_t*)"\x18\x41\x20\x14\x28\xff\x01\x30\x01\x38\xf0\x2e\x40\x00\x00"

#define Render_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, UENUM,    shape,             1) \
X(a, STATIC,   OPTIONAL, INT32,    color,             2) \
X(a, STATIC,   OPTIONAL, INT32,    position,          3) \
X(a, STATIC,   OPTIONAL, INT32,    width,             4)
#define Render_CALLBACK NULL
#define Render_DEFAULT (const pb_byte_t*)"\x10\xd2\x01\x18\x14\x20\x14\x00"

#define Rain_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, INT32,    width,             2) \
X(a, STATIC,   OPTIONAL, INT32,    drop_density,      3) \
X(a, STATIC,   OPTIONAL, INT32,    drop_speed,        4)
#define Rain_CALLBACK NULL
#define Rain_DEFAULT (const pb_byte_t*)"\x08\x01\x10\x0f\x18\x0a\x20\x0a\x00"

#define Gravity_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, INT32,    launch_speed,      2) \
X(a, STATIC,   OPTIONAL, INT32,    ball_rate,         3) \
X(a, STATIC,   OPTIONAL, INT32,    width,             4)
#define Gravity_CALLBACK NULL
#define Gravity_DEFAULT (const pb_byte_t*)"\x08\x0d\x10\x00\x18\x05\x20\x46\x00"

#define Kaleidoscope_FIELDLIST(X, a) \

#define Kaleidoscope_CALLBACK NULL
#define Kaleidoscope_DEFAULT NULL

#define MappedSlope_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, UENUM,    slope_type,        2) \
X(a, STATIC,   OPTIONAL, INT32,    x1,                3) \
X(a, STATIC,   OPTIONAL, INT32,    y1,                4) \
X(a, STATIC,   OPTIONAL, INT32,    x2,                5) \
X(a, STATIC,   OPTIONAL, INT32,    y2,                6) \
X(a, STATIC,   OPTIONAL, INT32,    fade_distance,     7)
#define MappedSlope_CALLBACK NULL
#define MappedSlope_DEFAULT (const pb_byte_t*)"\x08\x01\x10\x01\x18\x00\x20\x00\x28\xff\x01\x30\xff\x01\x38\x05\x00"

#define MappedShape_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, UENUM,    shape,             2) \
X(a, STATIC,   OPTIONAL, INT32,    x,                 3) \
X(a, STATIC,   OPTIONAL, INT32,    y,                 4) \
X(a, STATIC,   OPTIONAL, INT32,    size,              5) \
X(a, STATIC,   OPTIONAL, INT32,    fade_distance,     6)
#define MappedShape_CALLBACK NULL
#define MappedShape_DEFAULT (const pb_byte_t*)"\x08\x01\x18\x80\x01\x20\x80\x01\x28\x32\x30\x32\x00"

#define MappedTriangle_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, INT32,    x1,                2) \
X(a, STATIC,   OPTIONAL, INT32,    y1,                3) \
X(a, STATIC,   OPTIONAL, INT32,    x2,                4) \
X(a, STATIC,   OPTIONAL, INT32,    y2,                5) \
X(a, STATIC,   OPTIONAL, INT32,    x3,                6) \
X(a, STATIC,   OPTIONAL, INT32,    y3,                7)
#define MappedTriangle_CALLBACK NULL
#define MappedTriangle_DEFAULT (const pb_byte_t*)"\x08\x01\x10\x00\x18\x00\x20\xff\x01\x28\x00\x30\x80\x01\x38\xff\x01\x00"

#define MappedParticles_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, UENUM,    shape,             2) \
X(a, STATIC,   OPTIONAL, INT32,    x,                 3) \
X(a, STATIC,   OPTIONAL, INT32,    y,                 4) \
X(a, STATIC,   OPTIONAL, INT32,    size,              5) \
X(a, STATIC,   OPTIONAL, INT32,    fade_distance,     6)
#define MappedParticles_CALLBACK NULL
#define MappedParticles_DEFAULT (const pb_byte_t*)"\x08\xb1\x01\x10\x01\x18\x80\x01\x20\x80\x01\x28\x32\x30\x32\x00"

#define MappedPingPong_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    color,             1) \
X(a, STATIC,   OPTIONAL, INT32,    direction,         2) \
X(a, STATIC,   OPTIONAL, INT32,    fade_distance,     3)
#define MappedPingPong_CALLBACK NULL
#define MappedPingPong_DEFAULT (const pb_byte_t*)"\x08\x7b\x10\x00\x18\x05\x00"

#define Twang_FIELDLIST(X, a) \

#define Twang_CALLBACK NULL
#define Twang_DEFAULT NULL

extern const pb_msgdesc_t CommandParams_msg;
extern const pb_msgdesc_t White_msg;
extern const pb_msgdesc_t SingleColor_msg;
extern const pb_msgdesc_t Pixel_msg;
extern const pb_msgdesc_t PixelRGB_msg;
extern const pb_msgdesc_t Rainbow_msg;
extern const pb_msgdesc_t Sparkle_msg;
extern const pb_msgdesc_t FlickerSparkle_msg;
extern const pb_msgdesc_t PingPong_msg;
extern const pb_msgdesc_t Render_msg;
extern const pb_msgdesc_t Rain_msg;
extern const pb_msgdesc_t Gravity_msg;
extern const pb_msgdesc_t Kaleidoscope_msg;
extern const pb_msgdesc_t MappedSlope_msg;
extern const pb_msgdesc_t MappedShape_msg;
extern const pb_msgdesc_t MappedTriangle_msg;
extern const pb_msgdesc_t MappedParticles_msg;
extern const pb_msgdesc_t MappedPingPong_msg;
extern const pb_msgdesc_t Twang_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define CommandParams_fields &CommandParams_msg
#define White_fields &White_msg
#define SingleColor_fields &SingleColor_msg
#define Pixel_fields &Pixel_msg
#define PixelRGB_fields &PixelRGB_msg
#define Rainbow_fields &Rainbow_msg
#define Sparkle_fields &Sparkle_msg
#define FlickerSparkle_fields &FlickerSparkle_msg
#define PingPong_fields &PingPong_msg
#define Render_fields &Render_msg
#define Rain_fields &Rain_msg
#define Gravity_fields &Gravity_msg
#define Kaleidoscope_fields &Kaleidoscope_msg
#define MappedSlope_fields &MappedSlope_msg
#define MappedShape_fields &MappedShape_msg
#define MappedTriangle_fields &MappedTriangle_msg
#define MappedParticles_fields &MappedParticles_msg
#define MappedPingPong_fields &MappedPingPong_msg
#define Twang_fields &Twang_msg

/* Maximum encoded size of messages (where known) */
#define CommandParams_size                       117
#define FlickerSparkle_size                      77
#define Gravity_size                             44
#define Kaleidoscope_size                        0
#define MappedParticles_size                     57
#define MappedPingPong_size                      33
#define MappedShape_size                         57
#define MappedSlope_size                         68
#define MappedTriangle_size                      77
#define PingPong_size                            70
#define PixelRGB_size                            44
#define Pixel_size                               22
#define Rain_size                                44
#define Rainbow_size                             33
#define Render_size                              35
#define SingleColor_size                         11
#define Sparkle_size                             44
#define Twang_size                               0
#define White_size                               11

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef __cplusplus
/* Message descriptors for nanopb */
namespace nanopb {
template <>
struct MessageDescriptor<CommandParams> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 23;
    static inline const pb_msgdesc_t* fields() {
        return &CommandParams_msg;
    }
};
template <>
struct MessageDescriptor<White> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 1;
    static inline const pb_msgdesc_t* fields() {
        return &White_msg;
    }
};
template <>
struct MessageDescriptor<SingleColor> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 1;
    static inline const pb_msgdesc_t* fields() {
        return &SingleColor_msg;
    }
};
template <>
struct MessageDescriptor<Pixel> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 2;
    static inline const pb_msgdesc_t* fields() {
        return &Pixel_msg;
    }
};
template <>
struct MessageDescriptor<PixelRGB> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 4;
    static inline const pb_msgdesc_t* fields() {
        return &PixelRGB_msg;
    }
};
template <>
struct MessageDescriptor<Rainbow> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 3;
    static inline const pb_msgdesc_t* fields() {
        return &Rainbow_msg;
    }
};
template <>
struct MessageDescriptor<Sparkle> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 4;
    static inline const pb_msgdesc_t* fields() {
        return &Sparkle_msg;
    }
};
template <>
struct MessageDescriptor<FlickerSparkle> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 7;
    static inline const pb_msgdesc_t* fields() {
        return &FlickerSparkle_msg;
    }
};
template <>
struct MessageDescriptor<PingPong> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 8;
    static inline const pb_msgdesc_t* fields() {
        return &PingPong_msg;
    }
};
template <>
struct MessageDescriptor<Render> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 4;
    static inline const pb_msgdesc_t* fields() {
        return &Render_msg;
    }
};
template <>
struct MessageDescriptor<Rain> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 4;
    static inline const pb_msgdesc_t* fields() {
        return &Rain_msg;
    }
};
template <>
struct MessageDescriptor<Gravity> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 4;
    static inline const pb_msgdesc_t* fields() {
        return &Gravity_msg;
    }
};
template <>
struct MessageDescriptor<Kaleidoscope> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 0;
    static inline const pb_msgdesc_t* fields() {
        return &Kaleidoscope_msg;
    }
};
template <>
struct MessageDescriptor<MappedSlope> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 7;
    static inline const pb_msgdesc_t* fields() {
        return &MappedSlope_msg;
    }
};
template <>
struct MessageDescriptor<MappedShape> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 6;
    static inline const pb_msgdesc_t* fields() {
        return &MappedShape_msg;
    }
};
template <>
struct MessageDescriptor<MappedTriangle> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 7;
    static inline const pb_msgdesc_t* fields() {
        return &MappedTriangle_msg;
    }
};
template <>
struct MessageDescriptor<MappedParticles> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 6;
    static inline const pb_msgdesc_t* fields() {
        return &MappedParticles_msg;
    }
};
template <>
struct MessageDescriptor<MappedPingPong> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 3;
    static inline const pb_msgdesc_t* fields() {
        return &MappedPingPong_msg;
    }
};
template <>
struct MessageDescriptor<Twang> {
    static PB_INLINE_CONSTEXPR const pb_size_t fields_array_length = 0;
    static inline const pb_msgdesc_t* fields() {
        return &Twang_msg;
    }
};
}  // namespace nanopb

#endif  /* __cplusplus */


#endif
