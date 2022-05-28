/* Broadcast a message between two motes through an exchange of Aknowledgment messages. 
To broadcast a message press the mote button.
Interrupts happen when a mote broadcasts a message without receiving an AK for the previous message.

Compile Using SKY mote. */


#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <string.h>
#include <stdio.h> /* For printf() */
#include "dev/button-sensor.h"
#include "leds.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "Test"
#define LOG_LEVEL LOG_LEVEL_INFO

int sent,received = 0;      //message exchange flags
unsigned int time_sent = 0; //to measure time through RTIMER
int timeout = 0;            //interrupt flag
 
/* Configuration */
#define SEND_INTERVAL (8 * CLOCK_SECOND)

#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"
static linkaddr_t coordinator_addr =  {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
#endif /* MAC_CONF_WITH_TSCH */

/*---------------------------------------------------------------------------*/
PROCESS(nullnet_example_process, "NullNet broadcast example");
AUTOSTART_PROCESSES(&nullnet_example_process);

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest)
{
  if(len == sizeof(unsigned)) {
    unsigned count;
    received = 1;
    memcpy(&count, data, sizeof(count));

    //insert delays for troubleshouting
    clock_delay(60000);
    clock_delay(60000);
    clock_delay(60000);
    clock_delay(60000);
    clock_delay(60000);
    clock_delay(60000);
 
 
    //receiver sends AK
    if(received == 1 && sent == 0 && timeout == 0){
    LOG_INFO("Sending AK %u to ", count);
    LOG_INFO_LLADDR(NULL);
    LOG_INFO_("\n");
    
    memcpy(nullnet_buf, &count, sizeof(count));
    nullnet_len = sizeof(count);

    NETSTACK_NETWORK.output(NULL);
    received = 0;
    clock_delay(60000);
    clock_delay(60000);
    clock_delay(60000);
    } //sender receives AK
   else if(sent == 1 && timeout == 0){

    //turn leds off in case of succesful message exchange
    leds_off(LEDS_GREEN);
    leds_off(LEDS_YELLOW);
    sent = 0;
    received = 0;
    LOG_INFO("AK received from ");
    LOG_INFO_LLADDR(src);
    LOG_INFO("\n in %u clock Ticks ",(RTIMER_NOW() - time_sent));
    LOG_INFO_("\n");
    time_sent = 0;
    clock_delay(60000);
    clock_delay(60000);
    clock_delay(60000);
    } //Button has ben pressed more than one time...interrupt
    else if(sent > 1 ){

    //turn leds off 
    leds_off(LEDS_GREEN);
    leds_off(LEDS_YELLOW);
    sent = 0;
    received = 0;
    LOG_INFO("timeout");
    LOG_INFO_("\n");
    timeout = 1;
    clock_delay(60000);
    clock_delay(60000);
    clock_delay(60000);
    }
  }
}
  

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nullnet_example_process, ev, data)
{

  static unsigned count = 0;
  
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);


#if MAC_CONF_WITH_TSCH
  tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));
#endif /* MAC_CONF_WITH_TSCH */



  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&count;
  nullnet_len = sizeof(count);
  nullnet_set_input_callback(input_callback);


  while(1) {
    PROCESS_WAIT_EVENT_UNTIL((ev == sensors_event && data == &button_sensor));
    //set the flags
    sent ++;
    timeout = 0;
    received = 0;

    //using leds for troubleshooting 
    leds_on(LEDS_GREEN);
    LOG_INFO("Sending %u to ", count);
    time_sent = RTIMER_NOW();
    
    
    LOG_INFO_LLADDR(NULL);
    LOG_INFO_("\n");
    

    leds_on(LEDS_YELLOW);
    memcpy(nullnet_buf, &count, sizeof(count));
    nullnet_len = sizeof(count);
    
    NETSTACK_NETWORK.output(NULL);
    count++;
  }

  PROCESS_END();
}

