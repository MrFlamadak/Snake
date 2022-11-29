/* mipslabwork.c
/MOOOOOFAS COOOOOOOODE, NEVER STOP OR DONT POP


   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdio.h>
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declarations for these labs */
#include <math.h>


volatile int* trisE;
volatile int* portE;
int timeoutcount = 0;
int prime = 1234567;
volatile int* iec0;
volatile int* ipc2;
int direction = 1;
int speed = 1;
int apple_x = 20;
int apple_y = 10;
int random_x = 0;
int random_y = 12;
int score = 0;

 //applex = rand() % 31;
 //appley = rand() % 31;


#define SOUTH 1
#define NORTH 3
#define EAST 2
#define WEST 4

#define HEIGHT 4
#define WIDTH 32

int mytime = 0x5957;
//snake, obj written by moffa
struct snake {
  int x;
  int y;
  int direction;
};

struct obj
{
  int x;
  int y;
};

struct snake Psnake;
struct obj apple;

char textstring[] = "text, more text, and even more text!";

int rand_array[] = {
13, 30, 32, 3, 19, 11, 25, 17,
8, 19, 20, 24, 5, 21, 22, 16,
28, 13, 7, 23, 4, 17, 29, 8,
11, 22, 3, 10, 7, 32, 31, 23
};
//Writen by Erik borg, modified by Mostafa Aziz Zuher
void init_apple(struct obj apple)
{

  apple.x = rand_array[(random_x + 1) % 30];
  apple.y = rand_array[(random_y + 3) % 30];
}



/*
void set_pixel(int x, int y){
  int i = (y-1) / 8;
  icon[x + i*32] = 1 << (y - i * 8);
}*/
//Written by Roy
void set_pixel(int x, int y, uint8_t *gameboard)
{
  int gameIndex = ((y - 1) / 8) * 32 + x - 1;         // calculations for the index on the gameboard
  int gameValue = pow(2, (y - 1) % 8);              // calculations for the value on the index
  gameboard[gameIndex] = (gameboard[gameIndex] + (uint8_t) gameValue) ;        // set the pixels
}

/*

void clearGameBoard(uint8_t *data){
  int i = 0;
  while(i < 128){   // 32 * 4 = 128;
      data[i++] = 0;
    }
}*/

/* This function written 2022-07-12 by Erik Sirborg */
void clearGameBoard(uint8_t *gameboard)
{
  int counter = 0;
  while(counter < 128)
  {
    gameboard[counter] = 0;
    counter++;
  }
}
//Trajectory of snake function written by Mostafa Aziz Zuher
void trajectory(struct snake snake){
  if(getbtns() == 0x4) //button 4(left)
  {
    snake.direction = snake.direction+1 %4;
  }
  // button 3
  if(getbtns() == 0x2)//button 2 (right)
  {
   snake.direction = snake.direction-1 % 4;
  }
}
// movement of snake in trajectory function written by Mostafa Aziz Zuher
 void move(struct snake snake)
 {

  if(snake.direction == SOUTH)
    snake.y++ % 24;

  if (snake.direction == WEST)
    snake.x-- % 32;

  if(snake.direction == NORTH)
    snake.y-- % 24;

  if(snake.direction == EAST)
    snake.x++ % 32;
}
//Snake eats/collides with apple function
void collision(struct snake Psnake, struct obj apple)
{
    if((Psnake.x & Psnake.y) == (apple.x & apple.y))
    {
        init_apple();
        createapple();
        //add 1 to score, understand and write later
    }
}
//create apple and snake functions written by Mostafa?
void createapple(struct obj apple, uint8_t *gameboard)
{

    set_pixel(apple.x, apple.y, gameboard);

}

void createsnake(struct snake Psnake, uint8_t *gameboard)
{
    set_pixel(Psnake.x, Psnake.y, gameboard);
}

/* Interrupt Service Routine */
void user_isr( void )
{
  timeoutcount++;
  if(timeoutcount == 10)
  {
    clearGameBoard(icon);
    time2string(textstring, mytime);

    tick(&mytime);
    timeoutcount = 0;
    display_string(0, "       SCORE:");
    createsnake(Psnake, gameboard);
    trajectory(Psnake);
    move(Psnake);
                             // clear the timer2 interrupt flag
  collision(Psnake, apple);

    display_update();
display_image(30, icon);
//speed += 0.1;
// reset timeoutcount
  }
  IFSCLR(0) = 0x100;

}

/* Lab-specific initialization goes here */
void labinit( void )
{
  // ALLA VARIABLER:
  // Assignment 1
//  trisE = (volatile int*) 0xbf886100;                 // address of port
 // *trisE = 0x00;                                      // set all bits to 0 (output)
  TRISESET = 0x00;

 // portE = (volatile int*) 0xbf886110;                 // address of port
  //*portE = 0x00;
  TRISD = 0xfe0;                                      // initialize bits 5-11 to 1 (input
                                     // initialize the start value to 0 bits
PORTE = score;
TRISDSET = 0x00;
TRISFSET = 0x2;

  // Assignment 2
  T2CON = 0x0;                                        // reset timer2
  T2CONSET = 0x70;                                    // set prescale on timer2 to 1:256
  PR2 = ((80000000 / 256) / 10);                      // frequency = 80 M / 256 / 10 = 31250
  TMR2 = 0x0;                                         // initialize timer2 to 0
  T2CONSET = 0x8000;                                  // start timer2

  // Assignment 3
  /*iec0 = (volatile int*) 0xbf881060;                  // address of IEC(0)
  *iec0 = 0x100;                                      // enable timer2
  ipc2 = (volatile int*) 0xbf8810b0;                  // address of IPC(2)
  *ipc2 = 0xf8;                                       // set the highest priority and sub-priority for timer2
*/

IECSET(0) = 0x100;
IPCSET(2) = 0xf8;

  enable_interrupt();
  return;
}

void labwork( void ) {
  prime = nextprime(prime);
  PORTE = score;

}
