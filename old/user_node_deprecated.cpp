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
#include "images.h"

#define DEFAULT_ANIMATION_DELAY 100

using namespace ECG ;

MicroBit uBit ;

NodeState UserNode::_state = DEMO ;

PacketBuffer UserNode::_recvPacketBuffer(1) ;

PacketBuffer UserNode::_sendPacketBuffer(1) ;

const char * UserNode::_charMsg = "Hello, teachers!" ;

ManagedString UserNode::_strMsg(_charMsg) ;

UserNode::UserNode() {
    uBit.init() ;

    uBit.serial.printf("constructor is called\r\n") ;

    _state = DEMO ;

    _strMsg = _charMsg ;

    uBit.radio.enable() ;

    // Register button handlers
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOWN , this, &UserNode::onButtonADown) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_UP, this, &UserNode::onButtonAUp) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_DOWN, this, &UserNode::onButtonBDown) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_UP, this, &UserNode::onButtonBUp) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_DOWN, this, &UserNode::onButtonABDown) ;

    // Register radio signal handler
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, this, &UserNode::onDatagramRecipt) ;
}

void UserNode::onButtonADown(MicroBitEvent e) {
    // TODO: listen for unassign root signal while assigned
    switch(_state) {
        case DEMO:
            uBit.display.print("a") ;
            break ;
        case GAME_UNASSIGNED:
            uBit.display.print(ECG::Images::square_hollow) ;
            _state = GAME_LISTEN_A ;
            break ;
    }
}

void UserNode::onButtonAUp(MicroBitEvent e) {
    switch(_state) {
        case DEMO:
            uBit.display.print("A") ;
            break ;
        case GAME_LISTEN_A:
            uBit.display.clear() ;
            _state = GAME_UNASSIGNED ;
            break ;
    }
}

void UserNode::onButtonBDown(MicroBitEvent e) {
    switch(_state) {
        case DEMO:
            uBit.display.print("b") ;
            break ;
        case GAME_UNASSIGNED:
            uBit.display.print(ECG::Images::triangle_hollow) ;
            _state = GAME_LISTEN_B ;
            break ;
    }
}

void UserNode::onButtonBUp(MicroBitEvent e) {
    switch(_state) {
        case DEMO:
            uBit.display.print("B") ;
            break ;
        case GAME_LISTEN_B:
            uBit.display.clear() ;
            _state = GAME_UNASSIGNED ;
            break ;
    }
}

void UserNode::onButtonABDown(MicroBitEvent e) {
    switch(_state) {
        case DEMO:
            // TODO: make this block display
            broadcastAnimation() ;
            uBit.sleep(500) ;
            downloadAnimation() ;
            uBit.sleep(500) ;
            break ;
        case GAME_TEAM_A:
            broadcastAnimation() ;
            _sendPacketBuffer[0] = SIG_UA ;
            uBit.radio.datagram.send(_recvPacketBuffer) ;
            break ;
        case GAME_TEAM_B:
            broadcastAnimation() ;
            _sendPacketBuffer[0] = SIG_UB ;
            uBit.radio.datagram.send(_recvPacketBuffer) ;
            break ;
    }
}

void UserNode::loop() {
    uBit.serial.printf("state = %d\r\n", _state) ;
    uBit.sleep(1000) ;

    // Execution path of this loop depends on state
    switch (_state) {
        case DEMO:
            uBit.serial.printf("in demo state\r\n") ;
            break ;
        case MESSAGE:
            uBit.serial.printf("in message state\r\n") ;
            break ;
        case LISTEN:
            uBit.serial.printf("in listen state\r\n") ;
            break ;
        case GAME_UNASSIGNED:
            uBit.serial.printf("in Game Unassigned state\r\n") ;
            break ;
        case GAME_LISTEN_A:
            uBit.serial.printf("in Game Listen A state\r\n") ;
            break ;
        case GAME_LISTEN_B:
            uBit.serial.printf("in Game Listen B state\r\n") ;
            break ;
        case GAME_TEAM_A:
            uBit.serial.printf("in Game Team A state\r\n") ;
            uBit.display.print(ECG::Images::square_filled) ;
            break ;
        case GAME_TEAM_B:
            uBit.serial.printf("in Game Team B state\r\n") ;
            uBit.display.print(ECG::Images::triangle_filled) ;
            break ;
        default:
            // Don't do anything
            break ;
    }
}

void UserNode::onDatagramRecipt(MicroBitEvent e) {
    uBit.serial.printf("data detected\r\n") ;

    // Extract signal from received packet
    _recvPacketBuffer = uBit.radio.datagram.recv() ;

    int signal = _recvPacketBuffer[0] ;

    switch(signal) {
        // Reset and demo are essentially the same
        case SIG_RR:
        case SIG_RD:
            uBit.display.clear() ;
            _state = DEMO ;
            break ;
        // Ending the game just switches player to listen mode
        case SIG_RL :
        case SIG_RGG:
            _state = LISTEN ;
            break ;
        case SIG_RMSG:
            _state = MESSAGE ;
            uBit.display.scroll(_strMsg) ;
            break ;
        // Un-assigning a node is the same as starting a new game for them
        // If they are listening, that is
        case SIG_RU:
            if (_state != GAME_LISTEN_A && _state != GAME_LISTEN_B) {
                break ;
            }
            else {
                // Fall through to next case
            }
            break ;
        case SIG_RNG:
            _state = LISTEN ;
            newGameAnimation() ;
            _state = GAME_UNASSIGNED ;
            break ;
        case SIG_UA:
            if (_state == GAME_LISTEN_A) {
                _state = GAME_TEAM_A ;
            }
            else {
                // If they do not consent to conversion, tolerate it
            }
            break ;
        case SIG_UB:
            if (_state == GAME_LISTEN_B) {
                _state = GAME_TEAM_B ;
            }
            break ;
        default:
            uBit.serial.printf("Unknown signal received, ask devs to investigate.\r\n") ;
            break ;
    }
}

void UserNode::broadcastAnimation(int msDelay) {

   uBit.display.print(ECG::Images::centerRing) ;
   uBit.sleep(msDelay) ;

   uBit.display.print(ECG::Images::middleRing) ;
   uBit.sleep(msDelay) ;

   uBit.display.print(ECG::Images::outerRing) ;
   uBit.sleep(msDelay) ;
}

void UserNode::broadcastAnimation() {
    broadcastAnimation(DEFAULT_ANIMATION_DELAY) ;
}

void UserNode::downloadAnimation(int msDelay) {

   uBit.display.print(ECG::Images::outerRing) ;
   uBit.sleep(msDelay) ;

   uBit.display.print(ECG::Images::middleRing) ;
   uBit.sleep(msDelay) ;

   uBit.display.print(ECG::Images::centerRing) ;
   uBit.sleep(msDelay) ;
}

void UserNode::downloadAnimation() {
    downloadAnimation(DEFAULT_ANIMATION_DELAY) ;
}

void UserNode::newGameAnimation(int msDelay) {

    // TODO: consider the order of this animation

    uBit.display.print(ECG::Images::square_hollow) ;
    uBit.sleep(msDelay) ;
    uBit.display.print(ECG::Images::left_arrow) ;
    uBit.sleep(msDelay) ;
    uBit.display.print("A") ;
    uBit.sleep(msDelay) ;
    uBit.display.print(ECG::Images::square_filled) ;
    uBit.sleep(msDelay) ;

    uBit.display.print("&") ;
    uBit.sleep(msDelay) ;

    uBit.display.print(ECG::Images::triangle_hollow) ;
    uBit.sleep(msDelay) ;
    uBit.display.print(ECG::Images::right_arrow) ;
    uBit.sleep(msDelay) ;
    uBit.display.print("B") ;
    uBit.sleep(msDelay) ;
    uBit.display.print(ECG::Images::triangle_filled) ;
    uBit.sleep(msDelay) ;

    uBit.display.scroll("Go!") ;
    uBit.sleep(msDelay) ;
    uBit.display.clear() ;

}

void UserNode::newGameAnimation() {
    // Default delay for instructions will be one second
    newGameAnimation(1000) ;
}