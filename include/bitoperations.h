#ifndef BITOPERATIONS_H
#define BITOPERATIONS_H

#include "typedefs.h"

void setU8(u8* pVar, u8 byte);
void setS8(s8* pVar, u8 byte);
void setU16(u16* pVar, u8 byte);
void setS16(s16* pVar, u8 byte);
void setU32(u32* pVar, u8 byte);
void setS32(s32* pVar, u8 byte);
void setU64(u64* pVar, u8 byte);
void setS64(s64* pVar, u8 byte);

void clearU8(u8* pVar, u8 byte);
void clearS8(s8* pVar, u8 byte);
void clearU16(u16* pVar, u8 byte);
void clearS16(s16* pVar, u8 byte);
void clearU32(u32* pVar, u8 byte);
void clearS32(s32* pVar, u8 byte);
void clearU64(u64* pVar, u8 byte);
void clearS64(s64* pVar, u8 byte);

u8 isSetU8(u8 var, u8 byte);
s8 isSetS8(s8 var, u8 byte);
u16 isSetU8(u16 var, u8 byte);
s16 isSetS8(s16 var, u8 byte);
u32 isSetU8(u32 var, u8 byte);
s32 isSetS8(s32 var, u8 byte);
u64 isSetU8(u64 var, u8 byte);
s64 isSetS8(s64 var, u8 byte);

u8 isClearU8(u8 var, u8 byte);
s8 isClearS8(s8 var, u8 byte);
u16 isClearU8(u16 var, u8 byte);
s16 isClearS8(s16 var, u8 byte);
u32 isClearU8(u32 var, u8 byte);
s32 isClearS8(s32 var, u8 byte);
u64 isClearU8(u64 var, u8 byte);
s64 isClearS8(s64 var, u8 byte);

#endif
