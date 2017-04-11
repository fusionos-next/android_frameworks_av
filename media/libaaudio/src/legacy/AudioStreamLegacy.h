/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LEGACY_AUDIO_STREAM_LEGACY_H
#define LEGACY_AUDIO_STREAM_LEGACY_H


#include <aaudio/AAudio.h>

#include "AudioStream.h"
#include "AAudioLegacy.h"
#include "utility/FixedBlockAdapter.h"

namespace aaudio {


typedef void (*aaudio_legacy_callback_t)(int event, void* user, void *info);

enum {
    /**
     * Request that the callback function should fill the data buffer of an output stream,
     * or process the data of an input stream.
     * The address parameter passed to the callback function will point to a data buffer.
     * For an input stream, the data is read-only.
     * The value1 parameter will be the number of frames.
     * The value2 parameter is reserved and will be set to zero.
     * The callback should return AAUDIO_CALLBACK_RESULT_CONTINUE or AAUDIO_CALLBACK_RESULT_STOP.
     */
            AAUDIO_CALLBACK_OPERATION_PROCESS_DATA,

    /**
     * Inform the callback function that the stream was disconnected.
     * The address parameter passed to the callback function will be NULL.
     * The value1 will be an error code or AAUDIO_OK.
     * The value2 parameter is reserved and will be set to zero.
     * The callback return value will be ignored.
     */
            AAUDIO_CALLBACK_OPERATION_DISCONNECTED,
};
typedef int32_t aaudio_callback_operation_t;


class AudioStreamLegacy : public AudioStream, public FixedBlockProcessor {
public:
    AudioStreamLegacy();

    virtual ~AudioStreamLegacy();

    aaudio_legacy_callback_t getLegacyCallback();

    // This is public so it can be called from the C callback function.
    // This is called from the AudioTrack/AudioRecord client.
    virtual void processCallback(int event, void *info) = 0;

    void processCallbackCommon(aaudio_callback_operation_t opcode, void *info);

    // Implement FixedBlockProcessor
    int32_t onProcessFixedBlock(uint8_t *buffer, int32_t numBytes) override;

protected:
    FixedBlockAdapter         *mBlockAdapter = nullptr;
    aaudio_wrapping_frames_t   mPositionWhenStarting = 0;
    int32_t                    mCallbackBufferSize = 0;
};

} /* namespace aaudio */

#endif //LEGACY_AUDIO_STREAM_LEGACY_H
