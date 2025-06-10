#ifndef SFSAMP_H__
#define SFSAMP_H__

#if defined(SAMPFUNCS_SAMPVER_037R1)
#include "samp/v0.3.7-R1/SFSAMP.h"
#elif defined(SAMPFUNCS_SAMPVER_037R3_1)
#include "samp/v0.3.7-R3-1/SFSAMP.h"
#elif defined(SAMPFUNCS_SAMPVER_037R5)
#include "samp/v0.3.7-R5/SFSAMP.h"
#elif defined(SAMPFUNCS_SAMPVER_03DL)
#include "samp/v0.3.DL/SFSAMP.h"
#else
#error "Please define one of the SAMPFUNCS_SAMPVER_* macros for the target SA-MP version before including SFSAMP.h"
#endif

#endif // SFSAMP_H__
