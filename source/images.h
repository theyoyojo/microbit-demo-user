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
    images.h

    This file declares pointers to a number of useful
    MicroBitImage object arrays and provides an interface for their use.
**/

#ifndef IMAGES_H
#define IMAGES_H "images.h"

#include "MicroBitImage.h"

namespace ECG {

// As this is essentially a public image library, I used a struct to default all members to public
struct Images {


// Ring growing from center animation

// Array of images in animation
static MicroBitImage ringAnimation[] ;

// Number of images in animation
static int nFramesRingAnimation ;


// Small spinning line in center animation

// Array of images in animation
static MicroBitImage loadingAnimation[] ;

// Number of images in animation
static int nFramesLoadingAnimation ;


} ; // struct Images

} // namespace ECG

#endif // IMAGES_H