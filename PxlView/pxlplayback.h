#ifndef PXLPLAYBACK_H
#define PXLPLAYBACK_H

#include <vector>
#include <chrono>

#include "pxlfile.h"

using namespace std;


class PxlPlayback
{
    vector<uint16_t> frames_timeouts;
    vector<uint16_t> frames_times;
    uint count_frames { 0 };
    uint count_of_repeats { 0 };

    uint timeout_step { 0 };

    bool running { false };
    uint curent_frame_number { 0 };
    uint time { 0 };
    chrono::steady_clock::time_point start_time {};
    uint repeats { 0 };

public:
    PxlPlayback();

    void run(int step_msec);
    void stop();
    bool timerStep();

    void prev();
    void next();
    bool jumpToFrame(uint16_t frame_number);

    void setFramesTimeouts(vector<uint16_t> timeouts);
    void setRepeats(uint8_t repeats);
    bool isRunning() const;
    uint16_t getCurrentFrameNumber() const;

};

#endif // PXLPLAYBACK_H
