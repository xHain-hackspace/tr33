class Leds
{

public:
  virtual void init() = 0;

  // set leds
  virtual void set_led(uint8_t strip_index, int led, CRGB color) = 0;
  virtual void fade_led(uint8_t strip_index, int led, CRGB target, float amount) = 0;
  virtual uint8_t random_strip(uint8_t strip_index) = 0;
  virtual uint16_t strip_length(uint8_t strip_index) = 0;

  // commands
  virtual void all_white();
  virtual void rainbow_sine(char *data);
  virtual void ping_pong(char *data);
  virtual void gravity(char *data);
  virtual void rain(char *data);
  virtual void beats(char *data);
  virtual void mapped_swipe(char *data);
  virtual void mapped_shape(char *data);
  virtual void kaleidoscope(char *data);
  virtual void random_walk(char *data);
  virtual void debug(char *data);
  virtual void fireworks(char *data);
  virtual void rotating_sectors(char *data);
  virtual void fill(char *data);
  virtual void twang(char *data);
  virtual void joystick(char *data);

  // events - rendered once
  virtual void gravity_event();
};