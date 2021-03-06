/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Atmark Techno, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "ModbusDev.h"
#include "ModbusDevRTU.h"
#include "UartDriveMsg.h"
#include "SendRTApp.h"
#include "vector.h"

// ModbusDev structure
typedef struct ModbusDev {
    ModbusCtx* ctx;
    int devId;
}ModbusDev;

// Initialization and cleanup
vector 
ModbusDev_Initialize() {
    return vector_init(sizeof(ModbusDev));
}

void
ModbusDev_Destroy(vector modbusDevVec) {
    ModbusDev* modbusDev = vector_get_data(modbusDevVec);
    for (int i = 0, n = vector_size(modbusDevVec); i < n; ++i) {
        ModbusDevRTU_Destroy(modbusDev->ctx);
        free(modbusDev);
        modbusDev++;
    }
}

// Create Modbus RTU
ModbusDev* 
ModbusDev_NewModbusRTU(int devId, int baud) {
    ModbusDev* newObj;

    newObj = (ModbusDev*)malloc(sizeof(ModbusDev));
    newObj->ctx = ModbusDevRTU_Initialize(devId, baud);

    newObj->devId = devId;

    return newObj;
}

// Get ModubsDev*
ModbusDev* 
ModbusDev_GetModbusDev(int devID, vector modbusDevVec) {
    ModbusDev* modbusDev = vector_get_data(modbusDevVec);
    for (int i = 0, n = vector_size(modbusDevVec); i < n; ++i) {
        if (modbusDev->devId == devID) {
            return modbusDev;
        }
        modbusDev++;
    }
    return NULL;
}

// Connect
bool 
ModbusDev_Connect(ModbusDev* me) {
    return ModbusDevRTU_Connect(me->ctx);
}

// Read single register
bool 
ModbusDev_ReadSingleRegister(ModbusDev* me, int regAddr, unsigned short* dst) {
    return ModbusDevRTU_ReadSingleRegister(me->ctx, regAddr, dst);
}

bool
ModbusDev_ReadSingleInputRegister(ModbusDev* me, int regAddr, unsigned short* dst) {
    return ModbusDevRTU_ReadSingleInputRegister(me->ctx, regAddr, dst);
}

// Write single register
bool
ModbusDev_WriteSingleRegister(ModbusDev* me, int regAddr, uint16_t value) {
    return ModbusDevRTU_WriteSingleRegister(me->ctx, regAddr, value);
}
