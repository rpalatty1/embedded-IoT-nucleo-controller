# Networked USB HID Mouse Controller (Nucleo F429ZI)

**Module:** COMP2008 – Embedded Programming & IoT  

## Overview
Built a multi-threaded embedded system that receives network commands via Ethernet and controls a host PC mouse using USB HID.
Demonstrated low-level embedded, networking, and real-time system skills.

## Technologies
- C/C++ (Mbed OS RTOS)  
- Nucleo F429ZI (ARM Cortex-M)  
- Ethernet (TCP communication)  
- USB HID (mouse emulation)  
- Multi-threading, mutex, GPIO interrupts  

## Key Features
- Remote mouse control via network commands  
- Enable/disable device using debounced onboard button  
- Thread-safe FIFO buffer (producer–consumer pattern)  
- Multi-threaded design (network + USB tasks)  
- Event-driven and power-efficient implementation  
- Error handling with logging and system status indicators  

## Commands
- 'set x' / 'set y' – move mouse  
- 'click' – left click  
- 'read x' / 'read y' – get position  
- 'reset' – return to origin  
- 'exit' – disconnect client  

## Skills
- Embedded systems & RTOS development  
- Concurrency (threads, synchronisation, deadlock prevention)  
- Network programming in constrained environments  
- Hardware interfacing (USB, GPIO, Ethernet)  
- Robust system design & debugging  

## Deliverables
- Source code  
- Serial output logs  
- Test evidence (methodology & results)  
