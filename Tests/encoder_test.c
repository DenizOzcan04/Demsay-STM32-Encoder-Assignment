#ifdef ENCODER_HOST_TEST

#include "encoder.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void assert_state(const EncoderData_t *encoder,
                         int32_t delta,
                         int64_t total,
                         uint32_t rpm_x100,
                         EncoderDirection_t direction)
{
    assert(encoder->delta_pulses == delta);
    assert(encoder->total_pulses == total);
    assert(encoder->rpm_x100 == rpm_x100);
    assert(encoder->direction == direction);
}

int main(void)
{
    EncoderData_t encoder;

    Encoder_Init(&encoder, 0U);
    assert_state(&encoder, 0, 0, 0U, ENCODER_DIRECTION_STOP);

    Encoder_Update(&encoder, 100U);
    assert_state(&encoder, 100, 100, 6000U, ENCODER_DIRECTION_CW);

    Encoder_Update(&encoder, 50U);
    assert_state(&encoder, -50, 50, 3000U, ENCODER_DIRECTION_CCW);

    Encoder_Update(&encoder, 50U);
    assert_state(&encoder, 0, 50, 0U, ENCODER_DIRECTION_STOP);

    Encoder_Init(&encoder, UINT32_MAX - 9U);
    Encoder_Update(&encoder, 10U);
    assert_state(&encoder, 20, 20, 1200U, ENCODER_DIRECTION_CW);

    Encoder_Init(&encoder, 10U);
    Encoder_Update(&encoder, UINT32_MAX - 9U);
    assert_state(&encoder, -20, -20, 1200U, ENCODER_DIRECTION_CCW);

    assert(strcmp(Encoder_DirectionToString(ENCODER_DIRECTION_CW), "CW") == 0);
    assert(strcmp(Encoder_DirectionToString(ENCODER_DIRECTION_CCW), "CCW") == 0);
    assert(strcmp(Encoder_DirectionToString(ENCODER_DIRECTION_STOP), "STOP") == 0);

    puts("All encoder tests passed.");
    return 0;
}

#endif /* ENCODER_HOST_TEST */
