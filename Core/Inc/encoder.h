/*
 * encoder.h
 *
 *  Created on: 28 Eki 2026
 *      Author: deniz
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include <stdint.h>

#define ENCODER_COUNTS_PER_REV       100U
#define ENCODER_SAMPLE_PERIOD_MS     1000U

typedef enum
{
    ENCODER_DIRECTION_CCW  = -1,
    ENCODER_DIRECTION_STOP =  0,
    ENCODER_DIRECTION_CW   =  1
} EncoderDirection_t;

typedef struct
{
    uint32_t previous_counter;
    int32_t delta_pulses;
    int64_t total_pulses;
    uint32_t rpm_x100;
    EncoderDirection_t direction;
} EncoderData_t;

void Encoder_Init(EncoderData_t *encoder, uint32_t initial_counter);
void Encoder_Update(EncoderData_t *encoder, uint32_t current_counter);
const char *Encoder_DirectionToString(EncoderDirection_t direction);

#endif /* INC_ENCODER_H_ */
