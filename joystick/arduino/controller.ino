#include "tundra_mapped_input.h"

#define RP2040_BB_LED           (25)
#define ANALOG_X A2
#define ANALOG_Y A1

// Create TMI object to communicate with Tundra Tracker
TMI tundra_tracker;
void csn_irq( uint gpio, uint32_t event_mask );  // Chip select IRQ must be top level so let's make one and connect it later in setup

// Make a top level struct that packs the button data along with the rest of the controller analog values
typedef struct __attribute__( ( packed, aligned( 1 ) ) )
{
  unsigned int x:10;
  unsigned int y:10;
} 
controller_data_t;

// declare the controller struct for use
controller_data_t controller_data;

// Some other variables to blink an LED
uint32_t interval = 100; //refresh time in ms
uint8_t increment = 10;
uint32_t next_time = 0;
bool led_state = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(RP2040_BB_LED, OUTPUT);
  /* 
  pinMode(ANALOG_X, INPUT);
  pinMode(ANALOG_Y, INPUT);
  //*/
  controller_data.x=0;
  controller_data.y=0;

  // init the communication to Tundra Tracker, setup the CS irq callback (this has to be at Top level in arduino it seems)
  tundra_tracker.init( );
  gpio_set_irq_enabled_with_callback( tundra_tracker.get_cs_pin(), GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &csn_irq );

  //for demo only,   
}

char mesbuf[128];
// the loop function runs over and over again forever
void loop() {
  // Flag will be true when the library is ready for new data
  if ( tundra_tracker.data_ready() )
  {
    // Copy our controller struct to the data packet
    tundra_tracker.send_data( &controller_data, sizeof(controller_data) );

    // House keeping function for the library that should be ran right after data is ready
    tundra_tracker.handle_rx_data( );
  }
  
  // Framework for a subroutine that runs every 250ms, not nessesary for all examples but slows down how fast our data is incrementing and allows us
  // to blink an LED at human speeds
  if ( millis() > next_time ){
    int valx=analogRead(ANALOG_X);
    int valy=analogRead(ANALOG_Y);
    
    controller_data.x=valx;
    controller_data.y=valy;


    // Twiddle the LED
    digitalWrite(RP2040_BB_LED, led_state ^= true);
    //controller_data.x++;
    //controller_data.y++;

    sprintf(mesbuf,"X:%4d Y:%4d",valx,valy);
    Serial.println(mesbuf);

    // Setup the next refresh time
    next_time = millis() + interval;
  }
}

// Callback for SPI Chipselect, just connect in the tmi irq function
void csn_irq( uint gpio, uint32_t event_mask )
{
  tundra_tracker.csn_irq( gpio, event_mask );
}
