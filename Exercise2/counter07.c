/*
   Program that increments a counter every one second and displays the counter's value
   in binary form through a set of 3 leds.
   Mote : SkyMote
   
   Note : Colors RED,GREEN,YELLOW used to map the SkyMote Leds into a bitwise OR mask.
*/



#include "contiki.h"
#include "leds.h"
#include <stdio.h>
#include "dev/gpio-hal.h"

//counter to iterate through [0,7]
int counter = 0;     

//map SKYmote leds to an array
int arr[3] = {LEDS_YELLOW,LEDS_RED,LEDS_GREEN}; 


//function used to convert the counter var into binary in int array form
int* decToBinary(int n)     
{
    // array to store binary number
    static int binaryNum[3];
 
    //check if counter is zero 
    if(n == 0)
      binaryNum[0] = binaryNum[1] = binaryNum[2] = 0;

    // counter to binary array
    int i = 0;
    while (n > 0) {

        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
     
     
    return binaryNum;
}

/*function used to select indices from map to match counter binary value and return
  a led mask with the leds to be activated  */
leds_mask_t select(int counter)
{

//array to store counter in binary
int *p;

//set mask to 0(no leds on)
leds_mask_t leds = 0;

//get binary form of counter
p = decToBinary(counter);

/*find indices of arr that match the counter value and create 
the mask with the coresponding led values */
 for (int i = 0; i < 3; i++ ) 
      if(*(p + i) == 1)
       leds = leds | arr[i];
      
return leds;
}

// LET's a go
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);


PROCESS_THREAD(hello_world_process, ev, data)
{

  static struct etimer timer;

  PROCESS_BEGIN();

  /* Setup a periodic timer that expires after 1 second. */
  etimer_set(&timer, CLOCK_SECOND);

  while(1) {
    
    //reset timer
    if(counter == 8)
      counter = 0;
   
    //set leds to counter value   
    leds_set(select(counter));
    
    //wait a sec bruh
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    counter++;
    
    etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
