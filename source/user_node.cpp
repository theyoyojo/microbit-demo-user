/*
The MIT License (MIT)

Copyright (c) 2018 Joel Savitz

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "user_node.h"
#include "signals.h"

using namespace ECG ;

MicroBit uBit ;

PacketBuffer UserNode::tempPacketBuffer(1) ;

UserNode::UserNode() {
    uBit.init() ;

    uBit.serial.printf("constructor is called\r\n") ;

    state = DEMO ;

    uBit.radio.enable() ;

    // Register button handlers
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_UP, this, &UserNode::onButtonAUp) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_UP, this, &UserNode::onButtonBUp) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_DOWN, this, &UserNode::onButtonABDown) ;

    // Register radio signal handler
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, this, &UserNode::onDatagramRecipt) ;
}

void UserNode::onButtonAUp(MicroBitEvent e) {
    // Stub
    uBit.serial.printf("onButtonAUp()\r\n") ;
    state = MESSAGE ;
    uBit.serial.printf("state = %d\r\n",state ) ;
}

void UserNode::onButtonBUp(MicroBitEvent e) {
    // Stub
    uBit.serial.printf("onButtonBUp()\r\n") ;
    state = DEMO ;

    uBit.serial.printf("state = %d\r\n",state ) ;
}

void UserNode::onButtonABDown(MicroBitEvent e) {
    // Stub
    uBit.serial.printf("onButtonABDown()\r\n") ;
}

void UserNode::loop() {
    uBit.serial.printf("state = %d\r\n", state) ;
    uBit.sleep(1000) ;

    // Execution path of this loop depends on state
    // TODO: continue here after integer bug fixes
    switch (state) {
        case DEMO:
            uBit.serial.printf("in demo state\r\n") ;
            break ;
        case MESSAGE:
            uBit.serial.printf("in message state\r\n") ;
            break ;
        case LISTEN:

        case GAME_UNASSIGNED:

        case GAME_LISTEN_A:

        case GAME_LISTEN_B:

        case GAME_TEAM_A:

        case GAME_TEAM_B:

        default:

        break ;
    }
}

void UserNode::onDatagramRecipt(MicroBitEvent e) {
    uBit.serial.printf("data detected\r\n") ;

    // Extract signal from received packet
    tempPacketBuffer = uBit.radio.datagram.recv() ;

    int signal = tempPacketBuffer[0] ;

    switch(signal) {
        // Reset and demo are essentially the same
        case SIG_RR:
        case SIG_RD:
            state = DEMO ;
            break ;
        // Ending the game just switches player to listen mode
        case SIG_RL :
        case SIG_RGG:
            state = LISTEN ;
            break ;
        case SIG_RMSG:
            state = MESSAGE ;
            break ;
        // Un-assigning a node is the same as starting a new game for them
        // If they are listening, that is
        case SIG_RU:
            if (state != GAME_LISTEN_A && state != GAME_LISTEN_B) {
                break ;
            }
            else {
                // Fall through to next caseShould the user
            }
            break ;
        case SIG_RNG:
            state = GAME_UNASSIGNED ;
            break ;
        case SIG_UA:
            if (state == GAME_LISTEN_A) {
                state = GAME_TEAM_A ;
            }
            else {
                // If they do not consent to conversion, tolerate it
            }
            break ;
        case SIG_UB:
            if (state == GAME_LISTEN_B) {
                state = GAME_TEAM_B ;
            }
            break ;
        default:
            uBit.serial.printf("Unknown signal received, ask devs to investigate.\r\n") ;
            break ;
    }
}