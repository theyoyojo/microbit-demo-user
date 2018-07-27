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

typedef enum nodeState {
    DEMO, MESSAGE, LISTEN,
    GAME_UNASSIGNED,
    GAME_LISTEN_A, GAME_TEAM_A,
    GAME_LISTEN_B, GAME_TEAM_B
} NodeState ;

namespace ECG {

class UserNode {

private:

static NodeState _state ;

static PacketBuffer _recvPacketBuffer ;

static PacketBuffer _sendPacketBuffer ;

static const char * _charMsg ;

static ManagedString _strMsg ;

public:

    /**
      * Default constructor used to initialize device functions
      */ 
    UserNode() ;

    /**
      * Event handler for A-D
      */
    void onButtonADown(MicroBitEvent e) ;

    /**
      * Event handler for A-U
      */
    void onButtonAUp(MicroBitEvent e) ;

    /**
      * Event handler for B-D
      */
    void onButtonBDown(MicroBitEvent e) ;

    /**
      * Event handler for B-U
      */ 
    void onButtonBUp(MicroBitEvent e) ;

    /**
      * Event handler for B-D & A-D
      */
    void onButtonABDown(MicroBitEvent e) ;

    /**
      * Event handler for incomming datagrams
      */
    void onDatagramRecipt(MicroBitEvent e) ;

    /**
      * Display an animation to signify the execution of a broadcast
      * 
      * @param msDelay: A number of milliseconds to pause between stages of the animation
      */
    void broadcastAnimation(int msDelay) ;
    void broadcastAnimation() ;

    /**
      * Display an animation to signify a download of relevant data
      * 
      * @param msDelay: A number of milliseconds to pause between stages of the animation
      */
    void downloadAnimation(int msDelay) ;
    void downloadAnimation() ;

    /**
      * Display an animation to instruct the user as to how to play the game
      * 
      * @param msDelay: A number of milliseconds to pause between stages of the animation
      */
    void newGameAnimation(int msDelay) ;
    void newGameAnimation() ;

    /**
      * Primary execution loop called by main()
      */
    void loop() ;

} ; // class UserNode

} // namspace ECG

#endif // USERNODE_H