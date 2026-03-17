//Ronia Palatty 10745379
#include "PinNames.h"
#include "mbed.h"
#include "USBMouse.h"
#include "networkcode.h"
#include "EthernetInterface.h"
#include "TCPSocket.h"
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string.h>
#include <fifo.h>

#define FINALRESPONSE "exit"
BusOut onBoardLEDs(LED1, LED2, LED3);

using namespace uop_msb;
using namespace std;

LCD_16X2_DISPLAY disp;

//LEDs
DigitalOut led3(LED3);

//USB Mouse Interface
USBMouse mouse(true, ABS_MOUSE);
//used to enable/disable mouse
bool MouseState;
//mouse will be enabled from the start
bool MouseEnabled = true;

//DigitalOut objects for LEDs
DigitalOut lcdBacklight(LCD_BKL_PIN);
DigitalOut redLED(LED1);
DigitalOut yellowLED(LED2);

//Push Buttons
DigitalIn blueButton(USER_BUTTON);
//to enable/disable mouse using button
DigitalIn button(USER_BUTTON);

//variables to debounce enable/disable button
//when button is pressed
bool buttonPress = false;
//when button is released
bool buttonRelease = true;
//to show debouncer is  active
bool debouncerActive = false;
//debouncer timer
Ticker debouncerTimer;

//to show x and y origin on screen
float xStart = (float)(X_MAX_ABS - X_MIN_ABS)/ 2;  
//float XPos = 0; 
float yStart = (float)(Y_MAX_ABS - Y_MIN_ABS)/ 2;
//float YPos = 0; 

//signals when initialisation is complete
//Semaphore netInitDone(0);
//Mutex mutex;    
//bool Continue;  

//for network connections - from task 391
EthernetInterface net;
TCPSocket socket;


//used to debounce user button
void buttonDebouncer()
{
    debouncerActive = false;
    if (button.read() == 1 && buttonRelease) 
    {
        buttonPress = true;
        buttonRelease = false;
    }
    if (button.read() == 0 && buttonPress) 
    {
        buttonPress = false;
        buttonRelease = true;
    }
}


/*void MouseMode() 
{
    bool MouseEnabled = true;
    while (1) 
    {   
        if (buttonPress && !debouncerActive) 
        {
            debouncerActive = true;
            if (MouseEnabled) 
            {
                //mouse is disabled
                MouseEnabled = false;
                mouse.disconnect();
                printf("Board Mouse Disabled\n");
            } else 
            {
                //mouse is enabled
                MouseEnabled = true;
                mouse.connect();
                printf("Board Mouse Enabled\n");
            }
        }
    }
}*/

//to connect program over network using TCP - from task 391
void networkInitialization() 
{
    //connect to network via cmd
    //cd into directory that contains TCP client exe file
    //ensure to change IP to your own device
    //TCP-Client.exe <IP Adress> 8080

    //to set network parameters
    net.set_network("192.168.137.246", "255.255.255.0", "192.168.137.1");//change IP here
    net.connect();

    //to retrieve network address
    SocketAddress a;
    net.get_ip_address(&a);

    //prints the network address
    printf("IP address: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");

}

//to handle user input commands
void MouseController(bool &MouseState) 
{
    
    //opens and binds the TCP socket
    socket.open(&net);
    socket.bind(8080);

    //to set socket to listening 
    int err = socket.listen(10);
    if (err == 0) 
    {
        printf("Listening OK\n");
    } else 
    {
        printf("Listen error=%d\n", err);
        socket.close();
        net.disconnect();
        while (1);
    }

    //to run server until timeout
    bool Continue = true;

    //start position is the origin of screen
    float xPosition = xStart;  
    float yPosition = yStart; 

    do 
    {

        //to wait for a connection - from task 391
        TCPSocket *clt_sock = socket.accept();
        //critical error message if no response within 5s (as per requirement)
        //clt_sock->set_timeout(5000);
        clt_sock->set_timeout(20000); //20s
        printf("Connected\n");

        //to read the request
        char rxBuffer[65];
        nsapi_size_or_error_t N = clt_sock->recv(rxBuffer, sizeof(rxBuffer) - 1);
        
        if (N <= 0) 
        {
            //displays critical error message when response not recieved
            printf("Critical Error: No response within 5 seconds\n");
            break;
        }

        rxBuffer[N] = 0;
        string rx = string(rxBuffer);
        //printf("%s\r\n" , rx.c_str());

        //shows the user commands
        //sets x position of mouse
        //type set x, and enter desired position number
        if (rx.substr(0, 5) == "set x" && (MouseState == true)) 
        {   
            //moves mouse in x according to user input (not including manual mouse movement)
            string xStr = rx.substr(6);
            float ChangeX = stof(xStr); 
            //updates x position of mouse
            xPosition = xPosition + ChangeX; 
            mouse.move(xPosition, yPosition); 
            string response = "x set to " + xStr;
            clt_sock->send(response.c_str(), response.length()); 
        } 
        //sets y position of mouse
        //type set y, and enter desired position number
        else if (rx.substr(0, 5) == "set y" && (MouseState == true)) 
        {
            //moves mouse in y according to user input (not including manual mouse movement)
            string yStr = rx.substr(6);
            float ChangeY = stof(yStr); 
            //updates x position of mouse
            yPosition = yPosition - ChangeY; 
            mouse.move(xPosition, yPosition); 
            string response = "y set to " + yStr;
            clt_sock->send(response.c_str(), response.length());
        }
        //performs a left-click
        else if (rx == "click" && (MouseState == true)) 
        {
            //printf("Mouse has been clicked");
            clt_sock->send("Clicked", strlen("Clicked"));
            mouse.click(MOUSE_LEFT);
        }
        //reads last known x position
        else if (rx == "read x" && (MouseState == true)) 
        {
            string response = "Last known position of x is " + to_string(xPosition - xStart);
            clt_sock->send(response.c_str(), response.length());
        }
        else if (rx == "read y" && (MouseState == true)) 
        {
            string response = "Last known position of y is " + to_string(yPosition - yStart);
            clt_sock->send(response.c_str(), response.length());
        } 
        //reads current status of mouse (enabled/disabled)
        else if (rx == "status") 
        {
            if (MouseState == true)
            {
                clt_sock->send("enabled", strlen("enabled"));
            }
            else if (MouseState == false) 
            {
                clt_sock->send("disabled", strlen("disabled"));
            }
        } 
        //to reset mouse position back to origin
        else if (rx == "reset" && (MouseState == true))
        {
            clt_sock->send("mouse reset", strlen("mouse reset"));
            mouse.move(xStart,yStart);
            xPosition = xStart;
            yPosition = yStart;
            
        }
        //to close server and end program
        else if (rx == "exit") 
        {
            Continue = false;
            printf("terminating\n");
        }
        else if (MouseState == false)
        {
            clt_sock->send("mouse disabled", strlen("mouse disabled \n"));
        }

        //to send response
        const char *RESPONSE = "command being processed";
        nsapi_size_or_error_t ret = clt_sock->send(RESPONSE, strlen(RESPONSE));
        printf("Sent %d bytes\n", ret);

        //to close client socket
        clt_sock->close();

        ThisThread::sleep_for(100ms);
    } while (Continue);

    //to close network connection
    net.disconnect();
    printf("disconnected\n");
}

int main()

{   
    // START - UNCOMMENT THE FOLLOWING TWO LINES TO TEST YOUR BOARD AND SEE THE DEMO CODE WORKING
    //UOP_MSB_TEST  board;  //This class is purely for testing. Do no use it otherwise!!!!!!!!!!!
    //board.test();         //Look inside here to see how this works
    // END

    //mouse starts off as enabled
    bool MouseState = true;
    {
        
    //used to start network initialisation
    Thread netInitThread;
    netInitThread.start(networkInitialization);

    //used for mouse commands if mouse in enabled
    Thread mouseCommands;
    mouseCommands.start([&MouseState]() { MouseController(MouseState); });

    //debouncer to enable/disable mouse via blue button
    debouncerTimer.attach(&buttonDebouncer, 750ms);

    //loop to allow switch of mouse status between enabled and disabled
    while (1)
    {
        if (buttonPress && !debouncerActive)
        {
            debouncerActive = true;
            if (MouseState)
            {
                //to disable mouse
                MouseState = false;
                mouse.disconnect();
                printf("mouse disabled\n");
            }
            else
            {
                //to enable mouse
                MouseState = true;
                mouse.connect();
                printf("mouse enabled\n");
            }
        }
    }


    }
}