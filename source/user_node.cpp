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

/**
  user_node.cpp

  This file implements class UserNode as declared and described in user_node.h
**/

#include "user_node.h"
#include "signals.h"
#include "images.h"
#include "wait_api.h"

// Time in ms to wait between iterations of loop()
#define LOOP_DELAY 500

// Default delay for animations
#define DEFAULT_ANIMATION_DELAY LOOP_DELAY

using namespace ECG ;

// Definition of static variables declared in user_node.h

MicroBit uBit ;

NodeState UserNode::_state ;

PacketBuffer UserNode::_recvPacketBuffer(1) ;

PacketBuffer UserNode::_sendPacketBuffer(1) ;

int UserNode::_iFrameLoadingAnimation ;

// Implementation of constructor

UserNode::UserNode() {

  // Device initialization
  uBit.init() ;
  uBit.radio.enable() ;

  // All nodes boot to an unassigned state
  _state = UNASSIGNED ;

  // Register button event handlers

  // Button A
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOWN,
   this, &UserNode::onButtonADown) ;

  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_UP,
  this, &UserNode::onButtonAUp) ;

  // Button B
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_DOWN,
  this, &UserNode::onButtonBDown) ;

  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_UP,
  this, &UserNode::onButtonBUp) ;

  // Register radio signal handler
  uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM,
  this, &UserNode::onDatagramRecipt) ;
}

void UserNode::broadcastSignal(int sig) {

  // Set PacketBuffer object data payload to param sig
  _sendPacketBuffer[0] = sig ;

  // Send it.
  uBit.radio.datagram.send(_sendPacketBuffer) ;

  // Display an animation to the user and simultaneously restrict user
  // input as described in header
  broadcastAnimation() ;
}

/*
  The actions of the following button press event handlers are dependent
  on the current state of the device, so I used the same switch(_state)
  structure for all of them, describing device behavior in each state.
  Most events in most states require no action to be taken, signified by
  a single break statement under their case. The default case is always
  implemented, though it is almost certainly unnecessary.
*/

void UserNode::onButtonADown(MicroBitEvent e) {
  switch(_state) {
    case UNASSIGNED:
      // State transition from UNASSIGNED to LISTEN_A on event button A down
      _state = LISTEN_A ;
      break ;
    case LISTEN_A:
      break ;
    case TEAM_A:
      // If the user is on Team A, pressing the A button should broadcast
      // Team A's conversion signal
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
      // State transition from LISTEN_A to UNASSIGNED on event button A released
      _state = UNASSIGNED ;
      break ;
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
      // State transition from UNASSIGNED to LISTEN_B on event button B down
      _state = LISTEN_B ;
      break ;
    case LISTEN_A:
      break ;
    case TEAM_A:
      break ;
    case LISTEN_B:
      break ;
    case TEAM_B:
      // If the user is on Team B, pressing the B button should broadcast
      // Team B's conversion signal
      broadcastSignal(SIG_B) ;
      break ;
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
      // State transition from LISTEN_B to UNASSIGNED on event button B released
      _state = UNASSIGNED ;
      break ;
    case TEAM_B:
      break ;
    default:
      break ;
  }
}

void UserNode::onDatagramRecipt(MicroBitEvent e) {

  _recvPacketBuffer = uBit.radio.datagram.recv() ;

  uBit.serial.printf("DATAGRAM RECEIVED! (%d)\r\n",
  _recvPacketBuffer[0]) ;

  // Unlike the other event handlers, the actions of this function
  // are dependant on the signal received first and the state second
  switch(_recvPacketBuffer[0]) {
    case SIG_R:
      uBit.serial.printf("GOT SIG_R\r\n") ;
      // On recipt of the reset signal, the device should reset to the boot state
      _state = BOOT_STATE ;
      break ;
    case SIG_A:
      uBit.serial.printf("GOT SIG_A\r\n") ;
      // State transition from LISTEN_A to TEAM_A on event SIG_A received
      if (_state == LISTEN_A) {
        _state = TEAM_A ;
      }
      break ;
    case SIG_B:
      uBit.serial.printf("GOT SIG_B\r\n") ;
      // State transition from LISTEN_B to TEAM_B on event SIG_B received
      if (_state == LISTEN_B) {
        _state = TEAM_B ;
      }
      break ;
    default:
      break ;
  }
}

void UserNode::broadcastAnimation(int msDelay) {

  /*
  This function will block the processor until complete
  in order to delay repeated signal broadcasts

  In addition, this function is also the only one to bypass the device abstraction
  layer and directly access the mbed api (via wait_api.h, included above)
  */

  for (int i = 0; i < ECG::Images::nFramesRingAnimation; i++) {
    uBit.display.print(ECG::Images::ringAnimation[i]) ;
    wait_ms(msDelay) ;
  }
}

void UserNode::broadcastAnimation() {
  broadcastAnimation(DEFAULT_ANIMATION_DELAY) ;
}

void UserNode::incrementFrameLoadingAnimation() {

  // Prefix increment does the job as a side effect of the bounds check :)
  if (++_iFrameLoadingAnimation >= ECG::Images::nFramesLoadingAnimation) {
    _iFrameLoadingAnimation = 0 ;
  }
  uBit.serial.printf("Printed loadingAnimation frame #%d\r\n",_iFrameLoadingAnimation) ;
}

void UserNode::waitingForInputAnimation(int msDelay) {

  /*
  Instead of printing the entire animation at once, each call to this function
  prints the next stage of the animation in order to return to the caller
  as soon as possible and act on new user input
  */

  incrementFrameLoadingAnimation() ;
  uBit.display.print(ECG::Images::loadingAnimation[_iFrameLoadingAnimation]) ;
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

  uBit.sleep(LOOP_DELAY) ;
}