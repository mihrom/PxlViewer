#include "pxlplayback.h"


PxlPlayback::PxlPlayback()
{

}


void PxlPlayback::run(int step_msec)
{
    timeout_step = step_msec;
    running = true;
    repeats = 0;

    start_time = chrono::steady_clock::now()-chrono::milliseconds(time);
}


void PxlPlayback::stop()
{
    running = false;
}


bool PxlPlayback::timerStep()
{
    auto end_time = chrono::steady_clock::now();
    time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    uint next_frame = 0;

    while(next_frame < count_frames && time >= frames_times[next_frame+1])
        ++next_frame;

    if(next_frame >= count_frames){
        next_frame = 0;
        time = 0;
        start_time = chrono::steady_clock::now();
        repeats++;
    }

    if(repeats == count_of_repeats)
        stop();

    if(next_frame == curent_frame_number)
        return false;

    curent_frame_number = next_frame;
    return true;
}


void PxlPlayback::prev()
{
    if(curent_frame_number == 0 || curent_frame_number >= count_frames)
        curent_frame_number = count_frames-1;
    else
        curent_frame_number--;

    time = frames_times[curent_frame_number];
}


void PxlPlayback::next()
{
    curent_frame_number ++;

    if(curent_frame_number >= count_frames)
        curent_frame_number = 0;

    time = frames_times[curent_frame_number];
}


bool PxlPlayback::jumpToFrame(uint16_t frame_number)
{
    if(frame_number >= count_frames)
        return false;

    curent_frame_number = frame_number;
    time = frames_times[curent_frame_number+1];
    return true;
}


void PxlPlayback::setFramesTimeouts(vector<uint16_t> timeouts)
{
    curent_frame_number = 0;
    time = 0;

    frames_timeouts = timeouts;
    count_frames = frames_timeouts.size();

    frames_times.clear();
    uint16_t counter {0};
    for(auto t: frames_timeouts) {
        frames_times.push_back(counter);
        counter += t;
    }

    frames_times.push_back(counter);
}


void PxlPlayback::setRepeats(uint8_t repeats)
{
    count_of_repeats = repeats;
}


bool PxlPlayback::isRunning() const
{
    return running;
}


uint16_t PxlPlayback::getCurrentFrameNumber() const
{
    if(curent_frame_number >= count_frames)
        return 0;

    return curent_frame_number;
}
