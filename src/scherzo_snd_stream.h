#ifndef _SCHERZO_SND_STREAM_H_
#define _SCHERZO_SND_STREAM_H_

#include <kos.h>

void scherzo_snd_stream_filter_add(snd_stream_filter_t filtfunc, void * obj);
void scherzo_snd_stream_filter_remove(snd_stream_filter_t filtfunc, void * obj);
void scherzo_snd_stream_prefill();
int scherzo_snd_stream_init(void* (*callback)(int, int *));
void scherzo_snd_stream_shutdown();
void scherzo_snd_stream_queue_enable();
void scherzo_snd_stream_queue_disable();
void scherzo_snd_stream_start(uint32 freq, int st);
void scherzo_snd_stream_queue_go();
void scherzo_snd_stream_stop();
int scherzo_snd_stream_poll();
void scherzo_snd_stream_volume(int vol);

#endif
