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
#include "wait_api.h"

// Default delay for animations
#define DEFAULT_ANIMATION_DELAY 500

using namespace ECG ;

// Definition of static variables declared in user_node.h

MicroBit uBit ;

NodeState UserNode::_state ;

PacketBuffer UserNode::_recvPacketBuffer(1) ;

PacketBuffer UserNode::_sendPacketBuffer(1) ;

int UserNode::_nFrameLoadingAnimation ;

// Implementation of user_node.h follows

UserNode::UserNode() {

    // Device initialization
    uBit.init() ;
    uBit.radio.enable() ;

    // All nodes begin in an unassigned state
    _state = UNASSIGNED ;

    // Register button handlers
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOWN , this, &UserNode::onButtonADown) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_UP, this, &UserNode::onButtonAUp) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_DOWN, this, &UserNode::onButtonBDown) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_UP, this, &UserNode::onButtonBUp) ;
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_DOWN, this, &UserNode::onButtonABDown) ;

    // Register radio signal handler
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, this, &UserNode::onDatagramRecipt) ;
}

void UserNode::broadcastSignal(int sig) {
    _sendPacketBuffer[0] = sig ;
    uBit.radio.datagram.send(_sendPacketBuffer) ;
    broadcastAnimation() ;
}

void UserNode::onButtonADown(MicroBitEvent e) {
    switch(_state) {
        case UNASSIGNED:
            _state = LISTEN_A ;
            break ;
        case LISTEN_A:
            break ;
        case TEAM_A:
            broadcastSignal(SIG_A) ;
            break ;
        case LISTEN_B:
            break ;
        case TEAM_B:
            break ;
        default:
            break ;
    }
}

void UserNode::onButtonAUp(MicroBitEvent e) {
    switch(_state) {
        case UNASSIGNED:
            break ;
        case LISTEN_A:
            _state = UNASSIGNED ;
        case TEAM_A:
            break ;
        case LISTEN_B:
            break ;
        case TEAM_B:
            break ;
        default:
            break ;
    }
}

void UserNode::onButtonBDown(MicroBitEvent e) {
    switch(_state) {
        case UNASSIGNED:
            _state = LISTEN_B ;
        case LISTEN_A:
            break ;
        case TEAM_A:
            break ;
        case LISTEN_B:
            break ;
        case TEAM_B:
            broadcastSignal(SIG_B) ;
        default:
            break ;
    }
}

void UserNode::onButtonBUp(MicroBitEvent e) {
    switch(_state) {
        case UNASSIGNED:
            break ;
        case LISTEN_A:
            break ;
        case TEAM_A:
            break ;
        case LISTEN_B:
            _state = UNASSIGNED ;
            break ;
        case TEAM_B:
            break ;
        default:
            break ;
    }
}

// This handler currently does not do anything.
void UserNode::onButtonABDown(MicroBitEvent e) {}

void UserNode::onDatagramRecipt(MicroBitEvent e) {

    _recvPacketBuffer = uBit.radio.datagram.recv() ;

    uBit.serial.printf("DATAGRAM RECEIVED! (%d)\r\n",
    _recvPacketBuffer[0]) ;

    switch(_recvPacketBuffer[0]) {

        case SIG_R:
            uBit.serial.printf("GOT SIG_R\r\n") ;
            _state = UNASSIGNED ;
            break ;
        case SIG_A:
            uBit.serial.printf("GOT SIG_A\r\n") ;
            if (_state == LISTEN_A) {
                _state = TEAM_A ;
            }
            break ;
        case SIG_B:
            uBit.serial.printf("GOT SIG_B\r\n") ;
            if (_state == LISTEN_B) {
                _state = TEAM_B ;
            }
            break ;
        default:
            break ;
    }
}

void UserNode::broadcastAnimation(int msDelay) {

    // This function will block the scheduler until complete
    // in order to delay repeated signal broadcasts

    uBit.display.print(ECG::Images::centerRing) ;
    wait_ms(msDelay) ;

    uBit.display.print(ECG::Images::middleRing) ;
    wait_ms(msDelay) ;

    uBit.display.print(ECG::Images::outerRing) ;
    wait_ms(msDelay) ;
}

void UserNode::broadcastAnimation() {
    broadcastAnimation(DEFAULT_ANIMATION_DELAY) ;
}

void UserNode::incrementFrameLoadingAnimation() {

    // Prefix increment does the job as a side effect of the bounds check :)
    if (++_nFrameLoadingAnimation >= ECG::Images::nLoadingAnimationFrames) {
        _nFrameLoadingAnimation = 0 ;
    }
    uBit.serial.printf("Printed loading frame #%d\r\n",_nFrameLoadingAnimation) ;
}

void UserNode::waitingForInputAnimation(int msDelay) {
    incrementFrameLoadingAnimation() ;
    uBit.display.print(ECG::Images::loading[_nFrameLoadingAnimation]) ;
}

void UserNode::waitingForInputAnimation() {
    waitingForInputAnimation(DEFAULT_ANIMATION_DELAY) ;
}

void UserNode::loop() {
    switch(_state) {
        case UNASSIGNED:
            uBit.serial.printf("in U\r\n") ;
            waitingForInputAnimation() ;
            break ;
        case LISTEN_A:
            uBit.serial.printf("in LA\r\n") ;
            uBit.display.print("a") ;
            break ;
        case LISTEN_B:
            uBit.serial.printf("in LB\r\n") ;
            uBit.display.print("b") ;
            break ;
        case TEAM_A:
            uBit.serial.printf("in TA\r\n") ;
            uBit.display.print("A") ;
            break ;
        case TEAM_B:
            uBit.serial.printf("in TB\r\n") ;
            uBit.display.print("B") ;
            break ;
        default:
            break ;
    }
    uBit.sleep(500) ;

    uBit.serial.printf("radio.enable returns: (%d)\r\n",uBit.radio.enable()) ;
}