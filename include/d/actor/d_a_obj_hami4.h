#ifndef D_A_OBJ_HAMI4_H
#define D_A_OBJ_HAMI4_H

#include "f_op/f_op_actor.h"

class daObjHami4_c : public fopAc_ac_c {
public:
    void _create() {}
    void _delete() {}
    void _draw() {}
    void _execute() {}

    void CreateHeap();
    void CreateInit();
    void set_mtx();
    void daObjHami4_close_stop();
    void daObjHami4_open_demo_wait();
    void daObjHami4_open_demo();
    void daObjHami4_open_stop();

public:
    /* Place member variables here */
};

#endif /* D_A_OBJ_HAMI4_H */