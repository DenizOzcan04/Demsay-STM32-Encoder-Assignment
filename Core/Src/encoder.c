/*
 * encoder.c
 *
 *  Created on: 28 Eki 2026
 *      Author: deniz
 */

#include "encoder.h"

#include <stddef.h>

void Encoder_Init(EncoderData_t *encoder, uint32_t initial_counter)
{
    if (encoder == NULL)
    {
        return;
    }

    encoder->previous_counter = initial_counter;
    encoder->delta_pulses = 0;
    encoder->total_pulses = 0;
    encoder->rpm_x100 = 0U;
    encoder->direction = ENCODER_DIRECTION_STOP;
}

void Encoder_Update(EncoderData_t *encoder, uint32_t current_counter)
{
    uint64_t absolute_delta;

    if (encoder == NULL)
    {
        return;
    }

    /*
     * Unsigned subtraction followed by conversion to int32_t also handles
     * the 32-bit timer counter wrapping from maximum to zero and vice versa.
     */
    encoder->delta_pulses =
        (int32_t)(current_counter - encoder->previous_counter);

    encoder->previous_counter = current_counter;
    encoder->total_pulses += encoder->delta_pulses;

    if (encoder->delta_pulses == 0)
    {
        encoder->direction = ENCODER_DIRECTION_STOP;
        encoder->rpm_x100 = 0U;
        return;
    }

    if (encoder->delta_pulses > 0)
    {
        encoder->direction = ENCODER_DIRECTION_CW;
        absolute_delta = (uint64_t)encoder->delta_pulses;
    }
    else
    {
        encoder->direction = ENCODER_DIRECTION_CCW;
        absolute_delta = (uint64_t)(-(int64_t)encoder->delta_pulses);
    }

    /*
     * RPM = pulses * 60000 / (counts_per_rev * sample_period_ms)
     *
     * The result is multiplied by 100 to retain two decimal places
     * without enabling floating-point printf support.
     */
    encoder->rpm_x100 =
        (uint32_t)((absolute_delta * 6000000ULL) /
        ((uint64_t)ENCODER_COUNTS_PER_REV *
         (uint64_t)ENCODER_SAMPLE_PERIOD_MS));
}

const char *Encoder_DirectionToString(EncoderDirection_t direction)
{
    switch (direction)
    {
        case ENCODER_DIRECTION_CW:
            return "CW";

        case ENCODER_DIRECTION_CCW:
            return "CCW";

        case ENCODER_DIRECTION_STOP:
        default:
            return "STOP";
    }
}

