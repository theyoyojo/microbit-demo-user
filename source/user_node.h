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

#ifndef USERNODE_H
#define USERNODE_H "user_node.h"

#include "MicroBit.h"

// User Node states
// Deprecated: DEMO, MESSAGE, LISTEN
typedef enum nodeState {
    UNASSIGNED,
    LISTEN_A, TEAM_A,
    LISTEN_B, TEAM_B
} NodeState ;

namespace ECG {

class UserNode {

private:

// The current state of the node
static NodeState _state ;

// A packet buffer for incomming transmissions
static PacketBuffer _recvPacketBuffer ;

// A packet buffer for outgoing transmissions
static PacketBuffer _sendPacketBuffer ;

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
      * Event handler for B-D & A-D
      * 
      * @param e: An object representing the raised event passed
      * to the function via the messageBus
      */
    void onButtonABDown(MicroBitEvent e) ;

    /**
      * Event handler for incomming datagrams
      * 
      * @param e: An object representing the raised event passed
      * to the function via the messageBus
      */
    void onDatagramRecipt(MicroBitEvent e) ;

    /**
      * Broadcast a signal to the network
      * 
      * @param sig: The signal to be broadcast (see signal.h)
      */
    void broadcastSignal(int sig) ;

    /**
      * Display an animation to signify the execution of a broadcast
      * 
      * @param msDelay: A number of milliseconds toify the execution of a broadcast
      * 
      * @param msDelay: A number of milliseconds to pause between stages of the animation
      */
    void broadcastAnimation(int msDelay) ;
    void broadcastAnimation() ;

    /**
      * Display an animation to instruct the user as to how to play the game
      * 
      * @param msDelay: A number of milliseconds to pause between stages of the animation
      */
    void waitingForInputAnimation(int msDelay) ;
    void waitingForInputAnimation() ;

    /**
      * Primary execution loop called by main()
      */
    void loop() ;

} ; // class UserNode

} // namspace ECG

#endif // USERNODE_H