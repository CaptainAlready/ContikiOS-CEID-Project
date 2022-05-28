#include "contiki.h"
#include "dev/button-sensor.h"
#include <stdio.h> /* For printf() */

int counter = 0;

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);

  /* Setup a periodic timer that expires after 10 seconds. */
  etimer_set(&timer, CLOCK_SECOND);

  while(1) {

    printf("Hello, world, counter = %d\n",counter);
    /* Wait for the periodic timer to expire and then restart the timer and check if button was pressed. */
   
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer) || (ev == sensors_event && data == &button_sensor));
    if(ev == sensors_event && data == &button_sensor) counter = 0;  //if button pressed....
      else counter++;
      
    etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
