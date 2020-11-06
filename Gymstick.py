#!/usr/bin/python

"""
* Gymstick WalkingPad 433.92MHz
*
* Code by Antti Kirjavainen (antti.kirjavainen [_at_] gmail.com)
*
* This is a Python implementation for the WalkingPad, for
* the Raspberry Pi. Plug your transmitter to BOARD PIN 16 (BCM/GPIO23).
*
* HOW TO USE
* ./Gymstick.py [25-bit_binary_string]
*
* More info on the protocol in Gymstick.ino and RemoteCapture.ino here:
* https://github.com/akirjavainen/gymstick
*
"""

import time
import sys
import os
import RPi.GPIO as GPIO


TRANSMIT_PIN = 16  # BCM PIN 23 (GPIO23, BOARD PIN 16)
REPEAT_COMMAND = 4


# Microseconds (us) converted to seconds for time.sleep() function:
GYMSTICK_RADIO_SILENCE = 0.0066

GYMSTICK_SHORT = 0.00034
GYMSTICK_LONG = 0.000975

GYMSTICK_COMMAND_BIT_ARRAY_SIZE = 25


# ------------------------------------------------------------------
def sendGymstickCommand(command):

    if len(str(command)) is not GYMSTICK_COMMAND_BIT_ARRAY_SIZE:
        print("Your (invalid) command was", len(str(command)), "bits long.")
        print
        printUsage()

    # Prepare:
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(TRANSMIT_PIN, GPIO.OUT)

    # Send command:
    for t in range(REPEAT_COMMAND):
        doGymstickTribitSend(command)

    # Radio silence at the end of last command:
    transmitLow(GYMSTICK_RADIO_SILENCE)

    # Disable output to transmitter and clean up:
    exitProgram()
# ------------------------------------------------------------------


# ------------------------------------------------------------------
def doGymstickTribitSend(command):

    # There is no AGC/preamble

    for i in command:

        if i == '0':  # HIGH-LOW-LOW
            transmitHigh(GYMSTICK_SHORT)
            transmitLow(GYMSTICK_LONG)

        elif i == '1':  # HIGH-HIGH-LOW
            transmitHigh(GYMSTICK_LONG)
            transmitLow(GYMSTICK_SHORT)

        else:
            print("Invalid character", i, "in command! Exiting...")
            exitProgram()

    # Radio silence:
    transmitLow(GYMSTICK_RADIO_SILENCE)
# ------------------------------------------------------------------


# ------------------------------------------------------------------
def transmitHigh(delay):
    GPIO.output(TRANSMIT_PIN, GPIO.HIGH)
    time.sleep(delay)
# ------------------------------------------------------------------


# ------------------------------------------------------------------
def transmitLow(delay):
    GPIO.output(TRANSMIT_PIN, GPIO.LOW)
    time.sleep(delay)
# ------------------------------------------------------------------


# ------------------------------------------------------------------
def printUsage():
    print("Usage:")
    print(os.path.basename(sys.argv[0]), "[command_string]")
    print
    print("Correct command length is", GYMSTICK_COMMAND_BIT_ARRAY_SIZE, "bits.")
    print
    exit()
# ------------------------------------------------------------------


# ------------------------------------------------------------------
def exitProgram():
    # Disable output to transmitter and clean up:
    GPIO.output(TRANSMIT_PIN, GPIO.LOW)
    GPIO.cleanup()
    exit()
# ------------------------------------------------------------------


# ------------------------------------------------------------------
# Main program:
# ------------------------------------------------------------------
if len(sys.argv) < 2:
    printUsage()

sendGymstickCommand(sys.argv[1])

