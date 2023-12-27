//
// Generated by dtk
// Translation Unit: d_a_bg.cpp
//

#include "d/actor/d_a_bg.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_magma.h"
#include "d/d_grass.h"
#include "d/d_tree.h"
#include "d/d_wood.h"
#include "d/d_flower.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_lib.h"
#include "m_Do/m_Do_mtx.h"
#include "SSystem/SComponent/c_lib.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "stdio.h"

/* 800D8434-800D8478       .text setArcName__6daBg_cFv */
const char* daBg_c::setArcName() {
    static char arcName[32];
    sprintf(arcName, "Room%d", fopAcM_GetParam(this));
    return arcName;
}

/* 800D8478-800D8514       .text createMatAnm__FP12J3DModelDataUs */
BOOL createMatAnm(J3DModelData* modelData, u16 mat_no) {
    if (mat_no != 0xFFFF) {
        J3DMaterial * mat = modelData->getMaterialNodePointer(mat_no);
        if (mat->getMaterialAnm() == NULL) {
            J3DMaterialAnm * anm = new J3DMaterialAnm();
            if (anm == NULL)
                return FALSE;
            mat->setMaterialAnm(anm);
        }
    }

    return TRUE;
}

/* 800D8514-800D862C       .text create__13daBg_btkAnm_cFP12J3DModelDataP19J3DAnmTextureSRTKey */
BOOL daBg_btkAnm_c::create(J3DModelData* modelData, J3DAnmTextureSRTKey* anmData) {
    anm = new mDoExt_btkAnm();
    if (anm == NULL)
        return FALSE;

    if (!anm->init(modelData, anmData, true, J3DFrameCtrl::LOOP_REPEAT_e, 1.0f, 0, -1, false, 0))
        return FALSE;

    // probably an inline
    J3DAnmTextureSRTKey * data = anm->getBtkAnm();
    for (u16 i = 0; i < data->getUpdateMaterialNum(); i++) {
        u16 mat_no = data->getUpdateMaterialID(i);
        if (!createMatAnm(modelData, mat_no))
            return FALSE;
    }

    return TRUE;
}

/* 800D862C-800D86C4       .text entry__13daBg_btkAnm_cFP12J3DModelData */
void daBg_btkAnm_c::entry(J3DModelData* modelData) {
    anm->entry(modelData, 0.0f);

    const char * name = anm->getBtkAnm()->getUpdateMaterialName()->getName(0);
    if (name[0] == 'S' && name[1] == 'C' && name[2] == '_' && name[3] == '0' && name[4] == '1')
        special = 1;
    else
        special = 0;
}

/* 800D86C4-800D8728       .text play__13daBg_btkAnm_cFv */
void daBg_btkAnm_c::play() {
    if (special == 1) {
        anm->setFrame(g_dComIfG_gameInfo.play.mTimerInfo.mTimerType);
    } else {
        anm->play();
    }
}

/* 800D8728-800D8878       .text create__13daBg_brkAnm_cFP12J3DModelDataP15J3DAnmTevRegKey */
BOOL daBg_brkAnm_c::create(J3DModelData* modelData, J3DAnmTevRegKey* anmData) {
    anm = new mDoExt_brkAnm();
    if (anm == NULL)
        return FALSE;

    if (!anm->init(modelData, anmData, true, J3DFrameCtrl::LOOP_REPEAT_e, 1.0f, 0, -1, false, 0))
        return FALSE;

    // probably an inline
    J3DAnmTevRegKey * data = anm->getBrkAnm();
    for (u16 i = 0; i < data->getCRegUpdateMaterialNum(); i++) {
        u16 mat_no = data->getCRegUpdateMaterialID(i);
        if (!createMatAnm(modelData, mat_no))
            return FALSE;
    }

    for (u16 i = 0; i < data->getKRegUpdateMaterialNum(); i++) {
        u16 mat_no = data->getKRegUpdateMaterialID(i);
        if (!createMatAnm(modelData, mat_no))
            return FALSE;
    }

    return TRUE;
}

/* 800D8878-800D88B4       .text entry__13daBg_brkAnm_cFP12J3DModelData */
void daBg_brkAnm_c::entry(J3DModelData* modelData) {
    anm->entry(modelData, 0.0f);
    special = 0;
}

/* 800D88B4-800D88D8       .text play__13daBg_brkAnm_cFv */
void daBg_brkAnm_c::play() {
    anm->play();
}

/* 800D88D8-800D88F8       .text checkCreateHeap__FP10fopAc_ac_c */
static BOOL checkCreateHeap(fopAc_ac_c* i_ac) {
    return ((daBg_c*)i_ac)->createHeap();
}

/* 800D88F8-800D8C50       .text createHeap__6daBg_cFv */
BOOL daBg_c::createHeap() {
    static char l_modelName[4][13] = {
        "model.bmd",
        "model1.bmd",
        "model2.bmd",
        "model3.bmd",
    };
    static char l_modelName2[4][13] = {
        "model.bdl",
        "model1.bdl",
        "model2.bdl",
        "model3.bdl",
    };
    static char l_btkName[4][13] = {
        "model.btk",
        "model1.btk",
        "model2.btk",
        "model3.btk",
    };
    static char l_brkName[4][13] = {
        "model.brk",
        "model1.brk",
        "model2.brk",
        "model3.brk",
    };

    const char * arcName = setArcName();
    u32 roomNo = fopAcM_GetParam(this);

    BgModel * bgm = bg;
    for (int i = 0; i < 4; bgm++, i++) {
        J3DModelData * modelData = (J3DModelData *) dComIfG_getStageRes(arcName, l_modelName[i]);
        if (modelData == NULL)
            modelData = (J3DModelData *) dComIfG_getStageRes(arcName, l_modelName2[i]);
        if (modelData == NULL)
            continue;

        u32 diffFlag = 0x11000022;

        for (u16 mat_no = 0; mat_no < modelData->getMaterialNum(); mat_no++)
            modelData->getMaterialNodePointer(mat_no)->setMaterialAnm(NULL);

        J3DAnmTextureSRTKey * btk = (J3DAnmTextureSRTKey *) dComIfG_getStageRes(arcName, l_btkName[i]);
        if (btk != NULL) {
            bgm->btk = new daBg_btkAnm_c();
            if (bgm->btk == NULL)
                return FALSE;
            if (!bgm->btk->create(modelData, btk))
                return FALSE;
            diffFlag |= 0x00001200;
        } else {
            bgm->btk = NULL;
        }

        J3DAnmTevRegKey * brk = (J3DAnmTevRegKey *) dComIfG_getStageRes(arcName, l_brkName[i]);
        if (brk != NULL) {
            bgm->brk = new daBg_brkAnm_c();
            if (bgm->brk == NULL)
                return FALSE;
            if (!bgm->brk->create(modelData, brk))
                return FALSE;
        } else {
            bgm->brk = NULL;
        }

        bgm->model = mDoExt_J3DModel__create(modelData, 0, diffFlag);
        if (bgm->model == NULL)
            return FALSE;
        bgm->mpTevStr = new dKy_tevstr_c();
        if (bgm->mpTevStr == NULL)
            return FALSE;
        dKy_tevstr_init(bgm->mpTevStr, roomNo, 0xFF);
    }

    cBgD_t * dzb = (cBgD_t *) dComIfG_getStageRes(arcName, "room.dzb");
    if (dzb != NULL) {
        bgw = new dBgW();
        if (bgw == NULL)
            return FALSE;
        if (bgw->Set(dzb, cBgW::GLOBAL_e, NULL))
            return FALSE;
        dComIfGp_roomControl_setBgW(roomNo, bgw);
        bgw->mWallCorrectPriority = 0;
    } else {
        bgw = NULL;
    }

    return TRUE;
}

/* 800D8C50-800D8DB8       .text __dt__6daBg_cFv */
daBg_c::~daBg_c() {
    s32 roomNo = fopAcM_GetParam(this);

    if (heap != NULL && bgw != NULL) {
        dComIfG_Bgsp()->Release(bgw);
        dComIfGp_roomControl_setBgW(roomNo, NULL);
    }
    if (dComIfGp_getMagma() != NULL)
        dComIfGp_getMagma()->deleteRoom(roomNo);
    if (dComIfGp_getGrass() != NULL)
        dComIfGp_getGrass()->deleteRoom(roomNo);
    if (dComIfGp_getTree() != NULL)
        dComIfGp_getTree()->deleteRoom(roomNo);
    if (dComIfGp_getWood() != NULL)
        dComIfGp_getWood()->delete_room(roomNo);
    if (dComIfGp_getFlower() != NULL)
        dComIfGp_getFlower()->deleteRoom(roomNo);

    dComIfGp_roomControl_offStatusFlag(roomNo, 0x10);
}

BOOL daBg_c::draw() {
    s32 roomNo = fopAcM_GetParam(this);
    BgModel * bgm = &bg[0];
    J3DModel * model;

    dComIfGd_setListBG();
    mDoLib_clipper::changeFar(100000.0f);

    for (s32 i = 0; i < 4; i++, bgm++) {
        model = bgm->model;
        if (model == NULL)
            continue;

        if (bgm->btk != NULL)
            bgm->btk->anm->entryFrame();
        if (bgm->brk != NULL)
            bgm->brk->anm->entryFrame();
        model->calc();
        mDoLib_clipper::clip(model);
        g_env_light.settingTevStruct(TEV_TYPE_BG0 + i, NULL, bgm->mpTevStr);
        g_env_light.setLightTevColorType(model, bgm->mpTevStr);
        mDoExt_modelEntryDL(model);
    }
    
    mDoLib_clipper::resetFar();
    dComIfGd_setList();
    g_env_light.settingTevStruct(TEV_TYPE_BG0, NULL, dComIfGp_roomControl_getTevStr(roomNo));
    return TRUE;
}

/* 800D8DB8-800D8F34       .text daBg_Draw__FP6daBg_c */
static BOOL daBg_Draw(daBg_c* i_this) {
    return i_this->draw();
}

BOOL daBg_c::execute() {
    if (mUnloadTimer != 0) {
#if VERSION == VERSION_JPN
        mUnloadTimer = 0;
        fopAcM_delete(this);
#else
        if (cLib_calcTimer(&mUnloadTimer) == 0)
            fopAcM_delete(this);
#endif
        return TRUE;
    }

    if (dComIfGp_roomControl_checkStatusFlag(fopAcM_GetParam(this), 0x04)) {
#if VERSION == VERSION_JPN
        mUnloadTimer = 1;
#else
        if (strcmp(dComIfGp_getStartStageName(), "sea") == 0)
            mUnloadTimer = 16;
        else
            mUnloadTimer = 1;
#endif
    } else {
        BgModel * bgm = &bg[0];
        for (s32 i = 0; i < 4; i++, bgm++) {
            if (!mDoGph_gInf_c::isMonotone() || i == 2) {
                if (bgm->btk != NULL)
                    bgm->btk->play();
                if (bgm->brk != NULL)
                    bgm->brk->play();
            }
        }
    }

    return TRUE;
}

/* 800D8F34-800D903C       .text daBg_Execute__FP6daBg_c */
static BOOL daBg_Execute(daBg_c* i_this) {
    return i_this->execute();
}

BOOL daBg_c::isDelete() {
    if (mUnloadTimer == 0)
        return TRUE;
    else
        return FALSE;
}

/* 800D903C-800D904C       .text daBg_IsDelete__FP6daBg_c */
static BOOL daBg_IsDelete(daBg_c* i_this) {
    return i_this->isDelete();
}

/* 800D904C-800D9074       .text daBg_Delete__FP6daBg_c */
static BOOL daBg_Delete(daBg_c* i_this) {
    i_this->~daBg_c();
    return TRUE;
}

/* 800D9074-800D9094       .text daBg_Create__FP10fopAc_ac_c */
static s32 daBg_Create(fopAc_ac_c* i_ac) {
    return ((daBg_c*)i_ac)->create();
}

/* 800D9094-800D9318       .text create__6daBg_cFv */
s32 daBg_c::create() {
    fopAcM_SetupActor(this, daBg_c);

    s32 roomNo = fopAcM_GetParam(this);
    JKRExpHeap * roomHeap = g_dComIfG_gameInfo.play.getRoomControl()->getMemoryBlock(roomNo);
    if (roomHeap != NULL) {
        heap = JKRSolidHeap::create(-1, roomHeap, false);
        JUT_ASSERT(0x2fd, heap != 0);
        JKRHeap * oldHeap = mDoExt_setCurrentHeap(heap);
        s32 rt = createHeap();
        JUT_ASSERT(0x302, rt == 1);
        mDoExt_setCurrentHeap(oldHeap);
        heap->adjustSize();
    } else {
        if (!fopAcM_entrySolidHeap(this, checkCreateHeap, 0)) {
            dStage_escapeRestart();
            return cPhs_ERROR_e;
        }
    }

    BgModel * bgm = &bg[0];
    for (s32 i = 0; i < 4; i++, bgm++) {
        if (bgm->model == NULL)
            continue;
        J3DModelData * modelData = bgm->model->getModelData();
        if (bgm->btk != NULL)
            bgm->btk->entry(modelData);
        if (bgm->brk != NULL)
            bgm->brk->entry(modelData);
    }

    f32 transX, transZ;
    s16 angleY;
    if (dComIfGp_getMapTrans(roomNo, &transX, &transZ, &angleY)) {
        BgModel * bgm = &bg[0];
        J3DModel * model;
        for (s32 i = 0; i < 4; i++, bgm++) {
            model = bgm->model;
            if (model == NULL)
                continue;
            mDoMtx_stack_c::transS(transX, 0.0f, transZ);
            mDoMtx_stack_c::YrotM(angleY);
            model->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }

    if (bgw != NULL && dComIfG_Bgsp()->Regist(bgw, this)) {
        dStage_escapeRestart();
        return cPhs_ERROR_e;
    }

    dKy_tevstr_init(dComIfGp_roomControl_getTevStr(roomNo), roomNo, 0xFF);
    dComIfGp_roomControl_onStatusFlag(roomNo, 0x10);
    return cPhs_COMPLEATE_e;
}

actor_method_class l_daBg_Method = {
    (process_method_func)daBg_Create,
    (process_method_func)daBg_Delete,
    (process_method_func)daBg_Execute,
    (process_method_func)daBg_IsDelete,
    (process_method_func)daBg_Draw,
};

actor_process_profile_definition2 g_profile_BG = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_BG,
    &g_fpcLf_Method.mBase,
    sizeof(daBg_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x01C2,
    &l_daBg_Method,
    fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
    0,
};
