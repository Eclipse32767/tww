//
// Generated by dtk
// Translation Unit: m_Do_MemCard.cpp
//

#include "m_Do/m_Do_MemCard.h"
#include "m_Do/m_Do_MemCardRWmng.h"
#include "m_Do/m_Do_Reset.h"
#include "JSystem/JKernel/JKRThread.h"
#include "dolphin/card.h"
#include "global.h"
#include "MSL_C/string.h"

u8 MemCardStack[0x1000] ALIGN_DECL(32);
OSThread MemCardThread;
u8 MemCardWorkArea0[0xa000] ALIGN_DECL(32);

mDoMemCd_Ctrl_c g_mDoMemCd_control;

/* 80018D6C-80018D70       .text __ct__15mDoMemCd_Ctrl_cFv */
mDoMemCd_Ctrl_c::mDoMemCd_Ctrl_c() {
}

/* 80018D70-80018E18       .text ThdInit__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::ThdInit() {
    CARDInit();
    mCardCommand = 0;
    mCardState = 0;
    field_0x1659 = 0;
    field_0x165A = 2;
    field_0x1660 = CARD_NO_COMMAND;
    mCommand = CARD_NO_COMMAND;
    mCardSlot = 0;
    OSInitMutex(&mMutex);
    OSInitCond(&mCond);
    s32 priority = OSGetThreadPriority(OSGetCurrentThread());
    OSCreateThread(&MemCardThread, mDoMemCd_main, NULL, &MemCardStack[ARRAY_SIZE(MemCardStack)], ARRAY_SIZE(MemCardStack), priority + 1, 1);
    OSResumeThread(&MemCardThread);
}

/* 80018E18-80018EC8       .text main__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::main() {
    while (true) {
        OSLockMutex(&mMutex);
        while (mCommand == CARD_NO_COMMAND)
            OSWaitCond(&mCond, &mMutex);

        switch (mCommand) {
        case CARD_RESTORE:
            restore();
            break;
        case CARD_STORE:
            store();
            break;
        case CARD_FORMAT:
            format();
            break;
        case CARD_ATTACH:
            attach();
            break;
        case CARD_DETACH:
            detach();
            break;
#if VERSION == VERSION_PAL
        case CARD_RESTORE2:
            restore2();
            break;
#endif
        }

        mCommand = CARD_NO_COMMAND;
        OSUnlockMutex(&mMutex);
    }
}

/* 80018EC8-80018FD8       .text update__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::update() {
    if (mDoRst::isReset()) {
        OSLockMutex(&mMutex);
        mCommand = CARD_DETACH;
        field_0x165A = 3;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    } else {
        if (getStatus(0) != 14) {
            if (CARDProbe(0) && getStatus(0) == 0) {
                OSLockMutex(&mMutex);
                field_0x165A = 0;
                mCommand = CARD_ATTACH;
                OSUnlockMutex(&mMutex);
                OSSignalCond(&mCond);
            } else if (!CARDProbe(0) && getStatus(0) != 0) {
                OSLockMutex(&mMutex);
                field_0x165A = 1;
                mCommand = CARD_DETACH;
                OSUnlockMutex(&mMutex);
                OSSignalCond(&mCond);
            }
        }
    }
}

/* 80018FD8-8001906C       .text restore__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::restore() {
    CARDFileInfo cardInfo;
    s32 ret = CARDOpen(mCardSlot, "gczelda", &cardInfo);
    if (ret == CARD_ERROR_READY) {
        if (!mDoMemCdRWm_Restore(&cardInfo, this, 0x1650)) {
            field_0x1660 = 3;
        } else {
            setCardState(ret);
        }
        CARDClose(&cardInfo);
    } else {
        setCardState(ret);
    }
}

/* 8001906C-80019174       .text store__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::store() {
    CARDFileInfo cardInfo;
    s32 ret;
    if (field_0x1660 == 2) {
        ret = CARDCreate(mCardSlot, "gczelda", 0x18000, &cardInfo);
        if (ret == CARD_ERROR_READY)
            field_0x1660 = 1;
        else if (ret == CARD_ERROR_EXIST)
            field_0x1660 = 1;
        else
            setCardState(ret);
    }

    if (field_0x1660 == 1) {
        ret = CARDOpen(mCardSlot, "gczelda", &cardInfo);
        if (ret == CARD_ERROR_READY) {
            ret = mDoMemCdRWm_Store(&cardInfo, this, 0x1650);
            if (ret != CARD_ERROR_READY)
                setCardState(ret);
            else
                field_0x1660 = 4;
            CARDClose(&cardInfo);
        } else {
            setCardState(ret);
        }
    } else {
        setCardState(ret);
    }
}

/* 80019174-800191C4       .text command_format__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::command_format() {
    if (OSTryLockMutex(&mMutex)) {
        mCommand = CARD_FORMAT;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

/* 800191C4-80019238       .text save__15mDoMemCd_Ctrl_cFPvUlUl */
void mDoMemCd_Ctrl_c::save(void* data, u32 size, u32 offset) {
    if (OSTryLockMutex(&mMutex)) {
        memcpy(&mData[offset], data, size);
        mCommand = CARD_STORE;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

/* 80019238-80019288       .text load__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::load() {
    if (OSTryLockMutex(&mMutex)) {
        mCommand = CARD_RESTORE;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

/* 80019288-8001931C       .text LoadSync__15mDoMemCd_Ctrl_cFPvUlUl */
s32 mDoMemCd_Ctrl_c::LoadSync(void* dst, u32 size, u32 offset) {
    s32 ret = 0;
    if (OSTryLockMutex(&mMutex)) {
        if (field_0x1660 == 3) {
            memcpy(dst, &mData[offset], size);
            field_0x1660 = 1;
            ret = 1;
        } else {
            field_0x1660 = 2;
            ret = 2;
        }
        OSUnlockMutex(&mMutex);
    }
    return ret;
}

/* 8001931C-8001939C       .text SaveSync__15mDoMemCd_Ctrl_cFv */
s32 mDoMemCd_Ctrl_c::SaveSync() {
    s32 ret = 0;
    if (OSTryLockMutex(&mMutex)) {
        if (field_0x1660 == 4) {
            field_0x1660 = 1;
            ret = 1;
#if VERSION != VERSION_JPN
        } else if (field_0x1660 == 1) {
            ret = 0;
#endif
        } else {
            ret = 2;
        }
        OSUnlockMutex(&mMutex);
    }
    return ret;
}

#if VERSION == VERSION_PAL
void mDoMemCd_Ctrl_c::restore2() {
    if (field_0x1660 != 1) {
        return;
    }
    CARDFileInfo cardInfo;
    s32 ret = CARDOpen(mCardSlot, "gczelda", &cardInfo);
    if (ret == CARD_ERROR_READY) {
        if (!mDoMemCdRWm_Restore2(&cardInfo)) {
            field_0x1660 = 3;
        } else {
            setCardState(ret);
        }
        CARDClose(&cardInfo);
    } else {
        setCardState(ret);
    }
}

void mDoMemCd_Ctrl_c::load2() {
    if (OSTryLockMutex(&mMutex)) {
        mCommand = CARD_RESTORE2;
        OSUnlockMutex(&mMutex);
        OSSignalCond(&mCond);
    }
}

s32 mDoMemCd_Ctrl_c::LoadSync2() {
    s32 ret = 0;
    if (OSTryLockMutex(&mMutex)) {
        if (field_0x1660 == 3) {
            field_0x1660 = 1;
            ret = 1;
        } else {
            ret = 2;
        }
        OSUnlockMutex(&mMutex);
    }
    return ret;
}
#endif

/* 8001939C-80019480       .text getStatus__15mDoMemCd_Ctrl_cFUl */
u32 mDoMemCd_Ctrl_c::getStatus(u32) {
    if (OSTryLockMutex(&mMutex)) {
        s32 ret;
        switch (field_0x1660) {
        case 1: ret = 2; break;
        case 2: ret = 1; break;
        case 3: ret = 3; break;
        case 4: ret = 4; break;
        case 5: ret = 5; break;
        case 0: ret = 0; break;
        case 6: ret = 7; break;
        case 7: ret = 6; break;
        case 8: ret = 11; break;
        case 9: ret = 12; break;
        case 10: ret = 9; break;
        case 11: ret = 10; break;
        case 12: ret = 8; break;
        case 13: ret = 14; break;
        }
        OSUnlockMutex(&mMutex);
        return ret;
    } else {
        return 14;
    }
}

/* 80019480-800194CC       .text format__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::format() {
    s32 ret = CARDFormat(mCardSlot);
    if (ret == CARD_ERROR_READY) {
        field_0x1660 = 5;
    } else {
        setCardState(ret);
    }
}

/* 800194CC-8001953C       .text FormatSync__15mDoMemCd_Ctrl_cFv */
s32 mDoMemCd_Ctrl_c::FormatSync() {
    s32 ret = 0;
    if (OSTryLockMutex(&mMutex)) {
        if (field_0x1660 == 5) {
            field_0x1660 = 2;
            ret = 1;
        } else {
            ret = 2;
        }
        OSUnlockMutex(&mMutex);
    }
    return ret;
}

/* 8001953C-80019614       .text attach__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::attach() {
    s32 mem_size, sector_size;
    s32 ret = CARDProbeEx(mCardSlot, &mem_size, &sector_size);
    if (ret == CARD_ERROR_WRONGDEVICE) {
        field_0x1660 = 10;
    } else if (sector_size != 0x2000) {
        field_0x1660 = 11;
    } else {
        if (mount()) {
            if (loadfile()) {
                field_0x1660 = 1;
            } else {
                switch (checkspace()) {
                case 0: field_0x1660 = 2; break;
                case 1: field_0x1660 = 8; break;
                case 2: field_0x1660 = 9; break;
                case 3: break;
                }
            }
        }
    }
}

/* 80019614-8001964C       .text detach__15mDoMemCd_Ctrl_cFv */
void mDoMemCd_Ctrl_c::detach() {
    CARDUnmount(mCardSlot);
    field_0x1660 = 0;
}

/* 8001964C-8001972C       .text mount__15mDoMemCd_Ctrl_cFv */
s32 mDoMemCd_Ctrl_c::mount() {
    s32 ret = CARDMount(mCardSlot, MemCardWorkArea0, NULL);

    switch (ret) {
    case CARD_ERROR_IOERROR:
    case CARD_ERROR_FATAL_ERROR:
        field_0x1660 = 12;
        return 0;
    case CARD_ERROR_NOCARD:
        field_0x1660 = 0;
        return 0;
    case CARD_ERROR_BROKEN:
    case CARD_ERROR_READY: {
        ret = CARDCheck(mCardSlot);
        if (ret == CARD_ERROR_READY)
            return 1;
        field_0x1660 = 7;
        return 0;
    } break;
    case CARD_ERROR_ENCODING:
        field_0x1660 = 6;
        return 0;
    default:
        return 0;
    }
}

/* 8001972C-8001978C       .text loadfile__15mDoMemCd_Ctrl_cFv */
s32 mDoMemCd_Ctrl_c::loadfile() {
    CARDFileInfo cardInfo;
    s32 ret;
    ret = CARDOpen(mCardSlot, "gczelda", &cardInfo);
    if (ret == CARD_ERROR_READY) {
        CARDClose(&cardInfo);
        return TRUE;
    } else {
        setCardState(ret);
        return FALSE;
    }
}

/* 8001978C-80019818       .text checkspace__15mDoMemCd_Ctrl_cFv */
s32 mDoMemCd_Ctrl_c::checkspace() {
    s32 ret;
    s32 block_size, block_count;
    ret = CARDFreeBlocks(mCardSlot, &block_size, &block_count);
    if (ret != CARD_ERROR_READY) {
        setCardState(ret);
        return 3;
    } else {
        if (block_size < 0x18000)
            return 1;
        return (block_count < 1) ? 2 : 0;
    }
}

/* 80019818-800198C4       .text setCardState__15mDoMemCd_Ctrl_cFl */
void mDoMemCd_Ctrl_c::setCardState(s32 state) {
    switch (state) {
    case CARD_ERROR_FATAL_ERROR:
    case CARD_ERROR_IOERROR:
        field_0x1660 = 12;
        break;
    case CARD_ERROR_NOCARD:
        field_0x1660 = 0;
        break;
    case CARD_ERROR_BROKEN:
    case CARD_ERROR_READY: {
        s32 ret = CARDCheck(mCardSlot);
        if (ret == CARD_ERROR_READY)
            break;
        field_0x1660 = 7;
        break;
    } break;
    case CARD_ERROR_ENCODING:
        field_0x1660 = 6;
        break;
#if VERSION == VERSION_PAL
    case CARD_ERROR_NOFILE:
        field_0x1660 = 2;
        break;
#endif
    }
}

/* 800198C4-80019918       .text mDoMemCd_main__FPv */
int mDoMemCd_main(void*) {
    { JKRThread thread(OSGetCurrentThread(), 0); }

    JKRHeap* heap = NULL;
    heap->becomeCurrentHeap();

    g_mDoMemCd_control.main();
    return 0;
}
