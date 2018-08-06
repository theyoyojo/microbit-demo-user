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

#include "images.h"
#include "MicroBitImage.h"

using namespace ECG ;

MicroBitImage _centerRing(
    "0,0,0,0,0\n"
    "0,0,0,0,0\n"
    "0,0,255,0,0\n"
    "0,0,0,0,0\n"
    "0,0,0,0,0\n"
) ;

MicroBitImage _middleRing(
    "0,0,0,0,0\n"
    "0,255,255,255,0\n"
    "0,255,0,255,0\n"
    "0,255,255,255,0\n"
    "0,0,0,0,0\n"
) ;

MicroBitImage _outerRing(
    "255,255,255,255,255\n"
    "255,0,0,0,255\n"
    "255,0,0,0,255\n"
    "255,0,0,0,255\n"
    "255,255,255,255,255\n"
) ;

MicroBitImage _loading0(
    "0,0,0,0,0\n"
    "0,255,0,0,0\n"
    "0,0,255,0,0\n"
    "0,0,0,255,0\n"
    "0,0,0,0,0\n"
) ;

MicroBitImage _loading1(
    "0,0,0,0,0\n"
    "0,0,255,0,0\n"
    "0,0,255,0,0\n"
    "0,0,255,0,0\n"
    "0,0,0,0,0\n"
) ;

MicroBitImage _loading2(
    "0,0,0,0,0\n"
    "0,0,0,255,0\n"
    "0,0,255,0,0\n"
    "0,255,0,0,0\n"
    "0,0,0,0,0\n"
) ;

MicroBitImage _loading3(
    "0,0,0,0,0\n"
    "0,0,0,0,0\n"
    "0,255,255,255,0\n"
    "0,0,0,0,0\n"
    "0,0,0,0,0\n"
) ;

// Below, previously declared but undefined static
// MicroBitImage&'s are given definitions tying them
// to the representations above

MicroBitImage& Images::centerRing = _centerRing ;
MicroBitImage& Images::middleRing = _middleRing ;
MicroBitImage& Images::outerRing = _outerRing ;

// The loading animation is stored as an array

MicroBitImage Images::loading[] = {
    _loading0, // 1
    _loading1, // 2
    _loading2, // 3
    _loading3  // 4
    } ;

// And so it follows... 4
int Images::nLoadingAnimationFrames = 4 ;