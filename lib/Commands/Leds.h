class Leds
{

public:
  virtual void init();

  // set leds
  virtual void set_led(uint8_t strip_index, int led, CRGB color);
  virtual void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
  virtual uint8_t random_strip(uint8_t strip_index);
  virtual uint16_t strip_length(uint8_t strip_index);

  // commands
  virtual void all_white();
  virtual void single_color(char *data);
  virtual void rainbow_sine(char *data);
  virtual void ping_pong(char *data);
  virtual void gravity(char *data);
  virtual void show_number(char *data);
  virtual void rain(char *data);
  virtual void beats(char *data);
  virtual void mapped_swipe(char *data);
  virtual void mapped_shape(char *data);

  // events - rendered once
  virtual void gravity_event();
  virtual void beat(char *data);
  virtual void pixel(char *data);
  virtual void pixel_rgb(char *data);
};