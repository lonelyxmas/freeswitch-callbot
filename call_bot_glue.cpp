#include <cstdlib>
#include <algorithm>
#include <future>
#include <switch.h>
#include <switch_json.h>
#include <grpc++/grpc++.h>
#include "mod_call_bot.h"
#include "simple_buffer.h"
#include "smartivrphonegateway.pb.h"
#include "smartivrphonegateway.grpc.pb.h"

#include <fstream>
#include <iostream>
#define CHUNKSIZE (320)
typedef struct WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t RIFF[4] = {'R', 'I', 'F', 'F'}; // RIFF Header Magic header
    uint32_t ChunkSize;                     // RIFF Chunk Size
    uint8_t WAVE[4] = {'W', 'A', 'V', 'E'}; // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t fmt[4] = {'f', 'm', 't', ' '}; // FMT header
    uint32_t Subchunk1Size = 16;           // Size of the fmt chunk
    uint16_t AudioFormat = 1;              // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM
                                           // Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t NumOfChan = 1;                // Number of channels 1=Mono 2=Sterio
    uint32_t SamplesPerSec = 8000;         // Sampling Frequency in Hz
    uint32_t bytesPerSec = 8000 * 2;       // bytes per second
    uint16_t blockAlign = 2;               // 2=16-bit mono, 4=16-bit stereo
    uint16_t bitsPerSample = 16;           // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t Subchunk2ID[4] = {'d', 'a', 't', 'a'}; // "data"  string
    uint32_t Subchunk2Size;                        // Sampled data length
} wav_hdr;

using smartivrphonegateway::Config;
using smartivrphonegateway::SmartIVRRequest;
using smartivrphonegateway::SmartIVRResponse;
using smartivrphonegateway::SmartIVRResponseType;
using smartivrphonegateway::Status;

namespace
{
    int case_insensitive_match(std::string s1, std::string s2)
    {
        std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
        std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
        if (s1.compare(s2) == 0)
            return 1; // The strings are same
        return 0;     // not matched
    }
}

class GStreamer
{
public:
    GStreamer(
        switch_core_session_t *session, uint32_t channels, char *lang, int interim) : m_session(session),
                                                                                      m_writesDone(false),
                                                                                      m_connected(false),
                                                                                      m_playing(false),
                                                                                      m_language(lang),
                                                                                      m_interim(interim),
                                                                                      m_audioBuffer(CHUNKSIZE, 15)
    {
        switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, " Create GStreamer\n");
        // const char *var;
        // char sessionId[256];
        // switch_channel_t *channel = switch_core_session_get_channel(session);
        strncpy(m_sessionId, switch_core_session_get_uuid(session), 256);
    }

    ~GStreamer()
    {
        switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(m_session), SWITCH_LOG_INFO, "GStreamer::~GStreamer - deleting channel and stub: %p\n", (void *)this);
    }

    void print_request()
    {
        // cJSON *jResult = cJSON_CreateObject();
        // cJSON *jIsPlaying = cJSON_CreateBool(m_request.is_playing());
        // cJSON *jKeyPress = cJSON_CreateString(m_request.key_press().c_str());
        // cJSON *jConversationId = cJSON_CreateString(m_request.mutable_config()->conversation_id().c_str());
        // cJSON *jAudioContent = cJSON_CreateString(m_request.audio_content().c_str());
        // cJSON_AddItemToObject(jResult, "is_playing", jIsPlaying);
        // cJSON_AddItemToObject(jResult, "key_press", jKeyPress);
        // cJSON_AddItemToObject(jResult, "conversation_id", jConversationId);
        // cJSON_AddItemToObject(jResult, "audio_content", jAudioContent);

        // char *json = cJSON_PrintUnformatted(jResult);
        // switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "GStreamer %p sending message: %s\n", this, json);
        // free(json);
        // cJSON_Delete(jResult);
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "GStreamer %p audio isplaying: %d\n", this, m_request.is_playing());
    }

    void print_response(SmartIVRResponse response)
    {
        // cJSON *jResult = cJSON_CreateObject();
        // cJSON *jType = cJSON_CreateNumber(response.type());
        // cJSON *jTextAsr = cJSON_CreateString(response.text_asr().c_str());
        // cJSON *jTextBot = cJSON_CreateString(response.text_bot().c_str());
        // cJSON *jForwardSipJson = cJSON_CreateString(response.forward_sip_json().c_str());
        // cJSON *jStatusCode = cJSON_CreateNumber(response.status().code());
        // cJSON *jStatusMessage = cJSON_CreateString(response.status().message().c_str());
        // cJSON *jAudioContent = cJSON_CreateString(response.audio_content().c_str());
        // cJSON_AddItemToObject(jResult, "type", jType);
        // cJSON_AddItemToObject(jResult, "text_asr", jTextAsr);
        // cJSON_AddItemToObject(jResult, "text_bot", jTextBot);
        // cJSON_AddItemToObject(jResult, "forward_sip_json", jForwardSipJson);
        // cJSON_AddItemToObject(jResult, "status_code", jStatusCode);
        // cJSON_AddItemToObject(jResult, "status_message", jStatusMessage);
        // cJSON_AddItemToObject(jResult, "audio_content", jAudioContent);

        // char *json = cJSON_PrintUnformatted(jResult);
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "type: %d\n", response.type());
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "text_asr: %s\n", response.text_asr().c_str());
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "text_bot: %s\n", response.text_bot().c_str());
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "forward_sip_json: %s\n", response.forward_sip_json().c_str());
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "audio_content length: %d\n", response.audio_content().length());
        // free(json);
        // cJSON_Delete(jResult);
    }

    void createInitMessage()
    {
        switch_channel_t *channel = switch_core_session_get_channel(m_session);

        const char *var = switch_channel_get_variable(channel, "CALLBOT_MASTER_URI");
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "GStreamer %p creating grpc channel to %s\n", this, var);
        const char *var_session_id = switch_channel_get_variable(channel, "SESSION_ID");
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "GStreamer %p start master with session id %s\n", this, var_session_id);

        std::shared_ptr<grpc::Channel> grpcChannel = grpc::CreateChannel(var, grpc::InsecureChannelCredentials());
        if (!grpcChannel)
        {
            switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "GStreamer %p failed creating grpc channel to %s\n", this, var);
            throw std::runtime_error(std::string("Error creating grpc channel to ") + var);
        }

        m_stub = std::move(smartivrphonegateway::SmartIVRPhonegateway::NewStub(grpcChannel));

        /* set configuration parameters which are carried in the RecognitionInitMessage */
        auto streaming_config = m_request.mutable_config();
        std::string conversation_id(var_session_id);
        streaming_config->set_conversation_id(conversation_id);
        m_request.set_is_playing(m_playing);
        m_request.set_key_press("");
        m_request.set_audio_content("");
    }

    void connect()
    {
        assert(!m_connected);
        // Begin a stream.
        createInitMessage();
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "GStreamer %p creating streamer\n", this);
        m_streamer = m_stub->CallToBot(&m_context);
        m_connected = true;

        // read thread is waiting on this
        m_promise.set_value();

        // Write the first request, containing the config only.
        // switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "GStreamer %p sending initial message with conversationId: %s\n", this, m_request.mutable_config()->conversation_id().c_str());
        print_request();
        m_streamer->Write(m_request);
        // m_request.clear_config();

        // send any buffered audio
        int nFrames = m_audioBuffer.getNumItems();
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "GStreamer %p got stream ready, %d buffered frames\n", this, nFrames);
        if (nFrames)
        {
            char *p;
            do
            {
                p = m_audioBuffer.getNextChunk();
                if (p)
                {
                    write(p, CHUNKSIZE);
                }
            } while (p);
        }
    }

    bool write(void *data, uint32_t datalen)
    {
        if (!m_connected)
        {
            if (datalen % CHUNKSIZE == 0)
            {
                m_audioBuffer.add(data, datalen);
            }
            return true;
        }
        m_request.clear_audio_content();
        m_request.set_audio_content(data, datalen);
        m_request.set_is_playing(m_playing);
        // print_request();
        bool ok = m_streamer->Write(m_request);
        return ok;
    }

    uint32_t nextMessageSize(void)
    {
        uint32_t size = 0;
        m_streamer->NextMessageSize(&size);
        return size;
    }

    bool read(SmartIVRResponse *response)
    {
        return m_streamer->Read(response);
    }

    grpc::Status finish()
    {
        return m_streamer->Finish();
    }

    void startTimers()
    {
        // nr_asr::StreamingRecognizeRequest request;
        // auto msg = request.mutable_control_message()->mutable_start_timers_message();
        // m_streamer->Write(request);
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "GStreamer %p sent start timers control message\n", this);
    }

    void writesDone()
    {
        // grpc crashes if we call this twice on a stream
        if (!m_connected)
        {
            cancelConnect();
        }
        else if (!m_writesDone)
        {
            m_streamer->WritesDone();
            m_writesDone = true;
        }
    }

    bool waitForConnect()
    {
        std::shared_future<void> sf(m_promise.get_future());
        sf.wait();
        return m_connected;
    }

    bool waitForPlayingDone()
    {
        std::shared_future<void> sf(m_playing_promise.get_future());
        sf.wait();
        return m_playing;
    }

    void cancelConnect()
    {
        assert(!m_connected);
        m_promise.set_value();
    }

    bool isConnected()
    {
        return m_connected;
    }

    void playAudio(std::string audio_content, switch_core_session_t *session)
    {
        // create audio thread
        // switch_threadattr_t *thd_attr = NULL;
        // switch_memory_pool_t *pool = switch_core_session_get_pool(session);
        // switch_threadattr_create(&thd_attr, pool);
        // switch_threadattr_stacksize_set(thd_attr, SWITCH_THREAD_STACKSIZE);
        // switch_thread_create(&m_audio_thread, thd_attr, audio_play_thread, cb, pool);
    }

    bool cancelAudio()
    {
        assert(!m_playing);
        switch_thread_exit(m_audio_thread, SWITCH_STATUS_SUCCESS);
        m_playing_promise.set_value();
    }

    void setResponseQueue(switch_queue_t *queue)
    {
        m_response_queue = queue;
    }

    unsigned int getResponseQueueSize()
    {
        return switch_queue_size(m_response_queue);
    }

    switch_status_t addResponseToQueue(SmartIVRResponse *response)
    {
        return switch_queue_push(m_response_queue, response);
    }

    SmartIVRResponse getResponseFromQueue()
    {
        void *pop;
        switch_queue_pop(m_response_queue, &pop);
        return *(SmartIVRResponse *)pop;
    }

private:
    switch_core_session_t *m_session;
    grpc::ClientContext m_context;
    std::shared_ptr<grpc::Channel> m_channel;
    std::unique_ptr<smartivrphonegateway::SmartIVRPhonegateway::Stub> m_stub;
    SmartIVRRequest m_request;
    std::unique_ptr<grpc::ClientReaderWriterInterface<SmartIVRRequest, SmartIVRResponse>> m_streamer;
    bool m_writesDone;
    bool m_connected;
    bool m_interim;
    std::string m_language;
    std::promise<void> m_promise;
    SimpleBuffer m_audioBuffer;
    char m_sessionId[256];
    switch_queue_t *m_response_queue;
    bool m_playing;
    std::promise<void> m_playing_promise;
    switch_thread_t *m_audio_thread;
};

static std::vector<uint8_t> parse_byte_array(std::string str)
{
    std::vector<uint8_t> vec(str.begin(), str.end());
    return vec;
}

static switch_status_t play_audio(switch_channel_t *channel, switch_core_session_t *session, std::vector<uint8_t> audio_data)
{
    auto fsize = audio_data.size();
    switch_status_t status = SWITCH_STATUS_FALSE;
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: write frame to session %d!\n", fsize);
    const char *var_session_id = switch_core_session_get_uuid(session);
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread: write file: %s.wav\n", var_session_id);
    std::string fileName(var_session_id);
    fileName += ".wav";
    // write byte to pcm file
    wav_hdr wav;
    wav.ChunkSize = fsize + sizeof(wav_hdr) - 8;
    wav.Subchunk2Size = fsize;
    std::ofstream out(fileName.c_str(), std::ios::binary);
    if (!out)
    {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: error create file!\n");
        return status;
    }
    if (!out.write(reinterpret_cast<const char *>(&wav), sizeof(wav)))
    {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: Error writing WAV file header!\n");
        return status;
    }
    // int16_t d;
    if (!out.write(reinterpret_cast<char *>(&audio_data[0]), fsize * sizeof(uint8_t)))
    {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: Error writing audio data to WAV file!\n");
        return status;
    }
    out.close();
    fileName = "/" + fileName;
    status = switch_ivr_play_file(session, NULL, fileName.c_str(), NULL);

    return status;
}

static switch_status_t transfer_call(switch_channel_t *channel, switch_core_session_t *session, std::string forward_sip_json)
{
    switch_status_t status = SWITCH_STATUS_FALSE;
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread: transfer call with sip_json: %s!\n", forward_sip_json.c_str());
    const char *extension = switch_channel_get_variable(channel, "TRANSFER_EXTENSION");
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread: transfer call with %s\n", extension);
    if (!extension)
    {
        extension = "10004";
    }
    status = switch_ivr_session_transfer(session, extension, NULL, NULL);
    return status;
}

static void *SWITCH_THREAD_FUNC process_response_thread(switch_thread_t *thread, void *obj)
{
    // process response
    struct cap_cb *cb = (struct cap_cb *)obj;
    GStreamer *streamer = (GStreamer *)cb->streamer;
    char *sessionUUID = cb->sessionId;
    bool connected = streamer->waitForConnect();
    if (!connected)
    {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_NOTICE, "callbot grpc read thread exiting since we didnt connect\n");
        return nullptr;
    }
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "process_response_thread running .... \n");

    SmartIVRResponseType previousType = SmartIVRResponseType::CALL_END;
    SmartIVRResponse response;
    while (streamer->isConnected())
    {
        unsigned int queueSize = streamer->getResponseQueueSize();
        if (queueSize > 1)
        {
            switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "process_response_thread queue size:%d \n", queueSize);
            response = streamer->getResponseFromQueue();
            switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "process_response_thread got response .... \n");
            streamer->print_response(response);

            // switch_core_session_t *session = switch_core_session_locate(sessionUUID);
            // if (!session)
            // {
            //     switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: session %s is gone!\n", cb->sessionId);
            // }
            // else
            // {
            //     switch_channel_t *channel = switch_core_session_get_channel(session);
            //     SmartIVRResponseType responseType = response.type();
            //     if (previousType == SmartIVRResponseType::CALL_WAIT)
            //     {
            //         switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: before type is CALL_WAIT unhold call now!\n");
            //         switch_channel_stop_broadcast(channel);
            //         switch_channel_wait_for_flag(channel, CF_BROADCAST, SWITCH_FALSE, 5000, NULL);
            //         if (switch_ivr_play_file(session, NULL, "silence_stream://100", NULL) == SWITCH_STATUS_SUCCESS)
            //         {
            //             switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: unhold call success!\n");
            //         }
            //     }

            //     switch (responseType)
            //     {
            //     case SmartIVRResponseType::RECOGNIZE:
            //         switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread Got type RECOGNIZE.\n");
            //         break;

            //     case SmartIVRResponseType::RESULT_ASR:
            //         switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread Got type RESULT_ASR.\n");
            //         break;

            //     case SmartIVRResponseType::RESULT_TTS:
            //         switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread Got type RESULT_TTS.\n");
            //         switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: playing audio ........\n");
            //         streamer->setIsPlaying(true);
            //         if (play_audio(channel, session, parse_byte_array(response.audio_content())) == SWITCH_STATUS_SUCCESS)
            //         {
            //             switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: playing audio success!\n");
            //         }
            //         else
            //         {
            //             switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: playing audio failed!\n");
            //         }
            //         streamer->setIsPlaying(false);
            //         break;

            //     case SmartIVRResponseType::CALL_WAIT:
            //         switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread Got type CALL_WAIT.\n");
            //         if (switch_ivr_broadcast(sessionUUID, switch_channel_get_hold_music(channel), SMF_ECHO_ALEG | SMF_HOLD_BLEG | SMF_LOOP) == SWITCH_STATUS_SUCCESS)
            //         {
            //             switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: hold call success!\n");
            //         }
            //         else
            //         {
            //             switch_channel_clear_flag(channel, CF_HOLD);
            //             switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: hold call failed!\n");
            //         }
            //         break;
            //     case SmartIVRResponseType::CALL_FORWARD:
            //         switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread Got type CALL_FORWARD.\n");
            //         if (transfer_call(channel, session, response.forward_sip_json()) == SWITCH_STATUS_SUCCESS)
            //         {
            //             switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: transfer success!\n");
            //         }
            //         else
            //         {
            //             switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "grpc_read_thread: transfer failed!\n");
            //         }
            //         break;
            //     case SmartIVRResponseType::CALL_END:
            //         switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread Got type CALL_END.\n");
            //         switch_channel_hangup(channel, SWITCH_CAUSE_NORMAL_CLEARING);
            //         break;
            //     default:
            //         switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread Got unknown type.\n");
            //         break;
            //     }

            //     previousType = responseType;
            // }
            // switch_core_session_rwunlock(session);
        }
    }
    return nullptr;
}

// static void *SWITCH_THREAD_FUNC audio_player_thread(switch_thread_t *thread, void *obj)
// {
//     // play audio
// }

static void *SWITCH_THREAD_FUNC grpc_read_thread(switch_thread_t *thread, void *obj)
{
    struct cap_cb *cb = (struct cap_cb *)obj;
    GStreamer *streamer = (GStreamer *)cb->streamer;
    char *sessionUUID = cb->sessionId;

    bool connected = streamer->waitForConnect();
    if (!connected)
    {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_NOTICE, "callbot grpc read thread exiting since we didnt connect\n");
        return nullptr;
    }
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread running .... \n");
    // Read responses
    SmartIVRResponse response;
    while (streamer->read(&response))
    { // Returns false when no more to read.
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "grpc_read_thread got response .... \n");
        streamer->print_response(response);

        // push response to ProcessResponseQueue
        // if (streamer->addResponseToQueue(&response) == SWITCH_STATUS_SUCCESS)
        // {
        //     switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "addResponseToQueue: transfer success!\n");
        // }
        // else
        // {
        //     switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "addResponseToQueue: transfer failed!\n");
        // }
    }
    return nullptr;
}

extern "C"
{

    switch_status_t call_bot_init()
    {
        return SWITCH_STATUS_SUCCESS;
    }

    switch_status_t call_bot_cleanup()
    {
        return SWITCH_STATUS_SUCCESS;
    }
    switch_status_t call_bot_session_init(switch_core_session_t *session, responseHandler_t responseHandler,
                                          uint32_t samples_per_second, uint32_t channels, char *lang, int interim, char *bugname, void **ppUserData)
    {

        switch_channel_t *channel = switch_core_session_get_channel(session);
        auto read_codec = switch_core_session_get_read_codec(session);
        uint32_t sampleRate = read_codec->implementation->actual_samples_per_second;
        struct cap_cb *cb;
        int err;

        cb = (struct cap_cb *)switch_core_session_alloc(session, sizeof(*cb));
        strncpy(cb->sessionId, switch_core_session_get_uuid(session), MAX_SESSION_ID);
        strncpy(cb->bugname, bugname, MAX_BUG_LEN);
        cb->end_of_utterance = 0;

        switch_mutex_init(&cb->mutex, SWITCH_MUTEX_NESTED, switch_core_session_get_pool(session));
        if (sampleRate != 8000)
        {
            switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "call_bot_session_init:  initializing resampler\n");
            cb->resampler = speex_resampler_init(channels, sampleRate, 8000, SWITCH_RESAMPLE_QUALITY, &err);
            if (0 != err)
            {
                switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_ERROR, "%s: Error initializing resampler: %s.\n",
                                  switch_channel_get_name(channel), speex_resampler_strerror(err));
                return SWITCH_STATUS_FALSE;
            }
        }
        else
        {
            switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "%s: no resampling needed for this call\n", switch_channel_get_name(channel));
        }
        cb->responseHandler = responseHandler;

        // allocate vad if we are delaying connecting to the recognizer until we detect speech
        if (switch_channel_var_true(channel, "START_RECOGNIZING_ON_VAD"))
        {
            switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "call_bot_session_init:  initializing vad\n");
            cb->vad = switch_vad_init(sampleRate, channels);
            if (cb->vad)
            {
                const char *var;
                int mode = 2;
                int silence_ms = 150;
                int voice_ms = 250;
                int debug = 0;

                if (var = switch_channel_get_variable(channel, "RECOGNIZER_VAD_MODE"))
                {
                    mode = atoi(var);
                }
                if (var = switch_channel_get_variable(channel, "RECOGNIZER_VAD_SILENCE_MS"))
                {
                    silence_ms = atoi(var);
                }
                if (var = switch_channel_get_variable(channel, "RECOGNIZER_VAD_VOICE_MS"))
                {
                    voice_ms = atoi(var);
                }
                switch_vad_set_mode(cb->vad, mode);
                switch_vad_set_param(cb->vad, "silence_ms", silence_ms);
                switch_vad_set_param(cb->vad, "voice_ms", voice_ms);
                switch_vad_set_param(cb->vad, "debug", debug);
            }
        }

        GStreamer *streamer = NULL;
        switch_memory_pool_t *pool = switch_core_session_get_pool(session);
        try
        {
            switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "call_bot_session_init:  allocating streamer\n");
            streamer = new GStreamer(session, channels, lang, interim);
            // create response queue
            switch_queue_t *response_queue;
            switch_queue_create(&response_queue, 5, pool);
            streamer->setResponseQueue(response_queue);
            cb->streamer = streamer;
        }
        catch (std::exception &e)
        {
            switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_ERROR, "%s: Error initializing gstreamer: %s.\n",
                              switch_channel_get_name(channel), e.what());
            return SWITCH_STATUS_FALSE;
        }

        if (!cb->vad)
        {
            switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "call_bot_session_init:  no vad so connecting to callbot immediately\n");
            streamer->connect();
        }

        // create the read thread
        switch_threadattr_t *thd_attr = NULL;
        switch_threadattr_create(&thd_attr, pool);
        switch_threadattr_stacksize_set(thd_attr, SWITCH_MAX_STACKS);
        switch_thread_create(&cb->thread, thd_attr, grpc_read_thread, cb, pool);

        // create response process thread
        switch_memory_pool_t *pool1 = switch_core_session_get_pool(session);
        switch_threadattr_t *thd_process_attr = NULL;
        switch_threadattr_create(&thd_process_attr, pool1);
        switch_threadattr_stacksize_set(thd_process_attr, SWITCH_MAX_STACKS);
        switch_thread_create(&cb->process_thread, thd_process_attr, process_response_thread, cb, pool1);

        *ppUserData = cb;
        switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "call_bot_session_init:  initialized! \n");
        return SWITCH_STATUS_SUCCESS;
    }

    switch_status_t call_bot_session_cleanup(switch_core_session_t *session, int channelIsClosing, switch_media_bug_t *bug)
    {
        switch_channel_t *channel = switch_core_session_get_channel(session);

        if (bug)
        {
            struct cap_cb *cb = (struct cap_cb *)switch_core_media_bug_get_user_data(bug);
            switch_mutex_lock(cb->mutex);

            if (!switch_channel_get_private(channel, cb->bugname))
            {
                // race condition
                switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "%s Bug is not attached (race).\n", switch_channel_get_name(channel));
                switch_mutex_unlock(cb->mutex);
                return SWITCH_STATUS_FALSE;
            }
            switch_channel_set_private(channel, cb->bugname, NULL);

            // close connection and get final responses
            GStreamer *streamer = (GStreamer *)cb->streamer;

            if (streamer)
            {
                streamer->writesDone();

                switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "call_bot_session_cleanup: GStreamer (%p) waiting for read thread to complete\n", (void *)streamer);
                switch_status_t st;
                switch_thread_join(&st, cb->thread);
                switch_status_t st_process;
                switch_thread_join(&st_process, cb->process_thread);
                switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "call_bot_session_cleanup:  GStreamer (%p) read thread completed\n", (void *)streamer);

                delete streamer;
                cb->streamer = NULL;
                cb->thread = NULL;
                cb->process_thread = NULL;
            }

            if (cb->resampler)
            {
                speex_resampler_destroy(cb->resampler);
            }
            if (cb->vad)
            {
                switch_vad_destroy(&cb->vad);
                cb->vad = nullptr;
            }
            if (!channelIsClosing)
            {
                switch_core_media_bug_remove(session, &bug);
            }

            switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "call_bot_session_cleanup: Closed stream\n");

            switch_mutex_unlock(cb->mutex);

            return SWITCH_STATUS_SUCCESS;
        }

        switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "%s Bug is not attached.\n", switch_channel_get_name(channel));
        return SWITCH_STATUS_FALSE;
    }

    switch_bool_t call_bot_frame(switch_media_bug_t *bug, void *user_data)
    {
        switch_core_session_t *session = switch_core_media_bug_get_session(bug);
        struct cap_cb *cb = (struct cap_cb *)user_data;
        if (cb->streamer && !cb->end_of_utterance)
        {
            GStreamer *streamer = (GStreamer *)cb->streamer;
            uint8_t data[SWITCH_RECOMMENDED_BUFFER_SIZE];
            switch_frame_t frame = {};
            frame.data = data;
            frame.buflen = SWITCH_RECOMMENDED_BUFFER_SIZE;
            if (switch_mutex_trylock(cb->mutex) == SWITCH_STATUS_SUCCESS)
            {
                while (switch_core_media_bug_read(bug, &frame, SWITCH_TRUE) == SWITCH_STATUS_SUCCESS && !switch_test_flag((&frame), SFF_CNG))
                {
                    if (frame.datalen)
                    {
                        if (cb->vad && !streamer->isConnected())
                        {
                            switch_vad_state_t state = switch_vad_process(cb->vad, (int16_t *)frame.data, frame.samples);
                            if (state == SWITCH_VAD_STATE_START_TALKING)
                            {
                                switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_INFO, "detected speech, connect to callbot master now\n");
                                streamer->connect();
                                cb->responseHandler(session, "vad_detected", cb->bugname, NULL);
                            }
                        }

                        if (cb->resampler)
                        {
                            spx_int16_t out[SWITCH_RECOMMENDED_BUFFER_SIZE];
                            spx_uint32_t out_len = SWITCH_RECOMMENDED_BUFFER_SIZE;
                            spx_uint32_t in_len = frame.samples;
                            size_t written;

                            speex_resampler_process_interleaved_int(cb->resampler,
                                                                    (const spx_int16_t *)frame.data,
                                                                    (spx_uint32_t *)&in_len,
                                                                    &out[0],
                                                                    &out_len);
                            streamer->write(&out[0], sizeof(spx_int16_t) * out_len);
                        }
                        else
                        {
                            streamer->write(frame.data, sizeof(spx_int16_t) * frame.samples);
                        }
                    }
                }
                switch_mutex_unlock(cb->mutex);
            }
        }
        return SWITCH_TRUE;
    }
}
