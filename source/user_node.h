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
  user_node.h

  This file declares a class that represents the device and functionality of a 
  network node that is intended to be used as an interface to play the demonstration
  game implemented by this project and it's root (network control) companion project.
**/

#ifndef USERNODE_H
#define USERNODE_H "user_node.h"

#include "MicroBit.h"

// User Node states
typedef enum nodeState {
  UNASSIGNED,
  LISTEN_A, TEAM_A,
  LISTEN_B, TEAM_B
} NodeState ;

namespace ECG {

class UserNode {

private:

// An abstraction of the device functionality provided by the API
static MicroBit _uBit ;

// The current state of the node
static NodeState _state ;

// The initial and default state of the device
static NodeState const _bootState ;

// The number of milliseconds to wait between each iteration of the main device loop
static int const _msDeviceTick ;

// A packet buffer intended for incomming transmissions
static PacketBuffer _recvPacketBuffer ;

// A packet buffer indended for outgoing transmissions
static PacketBuffer _sendPacketBuffer ;

// The position of the device in the loading animation, in ticks
static unsigned int _iTickLoadingAnimation ;

// The number of device ticks that each frame in the loading animation is shown for
static int const _ticksPerFrameLoadingAnimation ;

// The number of milleseconds to wait between moving to the next frame
// in the broadcast animation
static int const _msPerFrameBroadcastAnimation ;

public:

  /**
    * Default constructor used to initialize device functions
    */ 
  UserNode() ;

  /**
    * Event handler for A-D
    * 
    * @param e: An object representing the raised event passed
    * to the function via the messageBus
    */
  void onButtonADown(MicroBitEvent e) ;

  /**
    * Event handler for A-U
    * 
    * @param e: An object representing the raised event passed
    * to the function via the messageBus
    */
  void onButtonAUp(MicroBitEvent e) ;

  /**
    * Event handler for B-D
    * 
    * @param e: An object representing the raised event passed
    * to the function via the messageBus
    */
  void onButtonBDown(MicroBitEvent e) ;

  /**
    * Event handler for B-U
    * 
    * @param e: An object representing the raised event passed
    * to the function via the messageBus
    */ 
  void onButtonBUp(MicroBitEvent e) ;

  /**
    * Event handler for incomming datagrams
    * 
    * @param e: An object representing the raised event passed
    * to the function via the messageBus
    */
  void onDatagramRecipt(MicroBitEvent e) ;

  /**
    * Broadcast a signal to the network. Use of this functionality
    * has an artificial cooldown implemented in broadcastAnimation()
    * 
    * @param sig: The signal to be broadcast (see signal.h)
    */
  void broadcastSignal(int sig) ;

  /**
    * Display an animation to signify the execution of a broadcast.
    * Blocks processor for the duration of the animation.
    * param msDelay * ECG::Images:nFramesLoadingAnimation milliseconds (to be precise)
    * In this version, that results in a 1.5 second artificial cooldown time to
    * restrict spamming of the broadcast function.
    */
  void broadcastAnimation() ;

  /**
    * Update the display to show the appropriate frame of an animation to indicate the
    * device being in a state of waiting for input. Looks like a loading animation.
    */
  void updateLoadingAnimation() ;

  /**
    * Primary execution loop called by main()
    */
  void loop() ;

} ; // class UserNode

} // namespace ECG

#endif // USERNODE_H