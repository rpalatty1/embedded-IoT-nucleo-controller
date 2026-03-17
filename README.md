# ELEC35X Coursework C1

#REFERRED COURSEWORK - RONIA PALATTY

The task was to build a device to control a mouse pointer on the screen over a network for
the purpose of remote UI testing. The Nucleo board will be connected to the host PC and
when instructed via network commands, will control of the mouse pointer. The blue button
on the Nucleo will cancel the mouse control.


##COMMANDS:
* set x - sets the x value of the mouse position (+x is to the right, -x is to the left)
* set y - sets the y value of the mouse position (+y is up, -y is down)
* click - mouse performs a left click
* read x - reads the last known x position of the mouse
* read y - reads the last known y position of the mouse
* reset - resets the positioning of mouse back to the origin (0,0)
* exit - diconnects the client from the server



This is the starter code for ELEC350/351 C1. You should use the ARMC6 compiler to build this code in Mbed Studio.

**A pre-requisite for this is to complete the lab on networking https://github.com/UniversityOfPlymouth-Electronics/Embedded-Systems/blob/master/level6/network_programming.md**

## Using the Module Support Board

Demo code has now been integrated into the module support board library.

In main.cpp, you can uncomment the following two lines to test your board:

```C++
//UOP_MSB_TEST  board;  //This class is purely for testing. Do no use it otherwise!!!!!!!!!!!
//board.test();         //Look inside here to see how this works
```

If you look inside the class member function `board.test()` you will see commented examples of how to use various aspects of the module support board, including the SD card.

*Don't forget to comment our or remove these two lines before you write your own code!*

