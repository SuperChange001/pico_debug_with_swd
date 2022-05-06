#include "pico/stdlib.h" // include a lib

#define RED_LED 18

void led_on(int gpio_pin) //low mean turn on
{
    gpio_put(gpio_pin,0);
}

void led_off(int gpio_pin) //high mean turn off
{
    gpio_put(gpio_pin,1);
}

int main()
{
    char dummy_array[1024];
    // init pins of LEDs
    gpio_init(18);
    gpio_init(19);
    gpio_init(20);

    // set directions of pins
    gpio_set_dir(18, GPIO_OUT);
    gpio_set_dir(19, GPIO_OUT);
    gpio_set_dir(20, GPIO_OUT);
    

    // Turn on LEDs
    led_on(RED_LED); 
    led_off(19); 
    led_off(20); 



    int pwm_cnt=0;
    int cnt_interal=0;
    while(1){
        dummy_array[0] = 'h';
        dummy_array[1] = 'e';
        dummy_array[2] = 'l';
        dummy_array[3] = 'l';
        dummy_array[4] = 'o';
        // led_on(RED_LED); 
        // sleep_ms(1000);
        // led_off(RED_LED); 
        // sleep_ms(1000);

        led_on(RED_LED);
        sleep_ms(pwm_cnt);
        led_off(RED_LED); 
        sleep_ms(10-pwm_cnt);

        cnt_interal++;
        if(cnt_interal==10){
            cnt_interal=0;
            pwm_cnt++;

            if (pwm_cnt==10)
                pwm_cnt = 0;
        }
           

        
    }
}


