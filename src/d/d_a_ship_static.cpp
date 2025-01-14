/**
 * d_a_ship_static.cpp
 * King of Red Lions - Static
 */

#include "d/actor/d_a_ship.h"

/* 80067D94-80067E78       .text initStartPos__8daShip_cFPC4cXyzs */
void daShip_c::initStartPos(const cXyz* pos, short rotY) {
    current.pos = *pos;
    old.pos = *pos;
    shape_angle.y = rotY;
    current.angle.y = shape_angle.y;
    m0358 &= ~0x10;
    mGravity = -2.5f;

    mWaveL.remove();
    mWaveR.remove();
    mSplash.remove();
    mTrack.remove();
    m1970.end();
    m1984.end();
    m1998.end();
    m19AC.end();
    m19C0.end();
}
