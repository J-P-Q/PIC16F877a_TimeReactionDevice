# PIC16F877a Time Reaction Device
A time reaction device made with the PIC16F877a microcontroller in baremetal c. 

## Hardware
- PIC16F877a
- 4 digit 7 segment
- 1 push button
- 1 passive buzzer
- 1 7805 voltage regulator (9V battery)

## How it works
The device remains in idle until the push button is press to start the game/round. An LCG random number generator sets the free-running timer's end and lights up the led. When the user presses the button before the 5 second window, their time reaction is displayed on the 7 segment and a tune plays. Otherwise, the player loses the game and a different tune plays on the passive buzzer. 

## Challenges
The PWM's duty cycle register apparently does not scale to the period. So if the period was set to 127 and the duty to 127, it would be 100% duty cycle and not 50% of the half from full period. This issue was resolved after the duty was made to adjust and adapt to the frequency set in a new function.  

## Demo
[Watch demo on Youtube] 

## Build
Written in C using MPLAB with VSCode as the editor
