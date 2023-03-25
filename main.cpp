#include "mbed.h"
#include "uLCD_4DGL.h"
// main() runs in its own thread in the OS
InterruptIn PinB(D3);
DigitalOut PinA(D4);
DigitalOut LED(LED2);
PwmOut PWM1(D6);
AnalogIn Ain(A0), AO(A1);
uLCD_4DGL uLCD(D1, D0, D2);// tx rx ref
Thread t, t2, t3;
EventQueue queue(32 * EVENTS_EVENT_SIZE);

int sample = 0;

void isr_rise() {
    uLCD.color(WHITE);
    uLCD.printf("%d\n", sample);
    uLCD.text_width(1); // 4X size text
    uLCD.text_height(1);
    //ThisThread::sleep_for(1ms);
};

void func() {
    //LED = PinA;
    queue.call(isr_rise);
};

void pwm_ramp() {
    while (true) {
        PWM1.period_ms(5);
        PWM1 = Ain;
        ThisThread::sleep_for(1ms);
    }
};

void AOIn() {
    float ADC;
    while (true) {
        ADC = AO.read();
        printf("%f\n", ADC);
        if (ADC > 0.35) {
            PinA = 1;
        }
        else {
            PinA = 0;
        }
        LED = PinA;
        sample = sample + 1;
        ThisThread::sleep_for(1ms);
    }
};

int main()
{
    t.start(pwm_ramp);
    t2.start(AOIn);
    t3.start(callback(&queue, &EventQueue::dispatch_forever));

    PinB.rise(func);

}