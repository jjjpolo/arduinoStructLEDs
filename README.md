# IMPLEMENTING A CIRCULAR STRUCT IN ARDUINO
## Description
This is a demo of how to implement a data stucts in Arduino.
The main function is always walking through all the elements in a cricular struct and toggling its state.
Serial incoming data interruption allows you to add or delte elements.
When adding a new element you need to specify the firs state of the element (ON/OFF).

## Serial data frame demo
The data frame is composed by: Command (three bytes) + PinNumber (two bytes) + stateParam(1 byte)
# Adding a new element:
This command will add a structure for pin 13 starting in ON State
add131

# Removig a element:
This command will delte a structre (if exists) for pin 13 (stateParam does not matter)
del131