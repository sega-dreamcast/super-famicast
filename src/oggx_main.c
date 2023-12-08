#include <kos.h>

static kthread_t * thd = NULL;

void oggx_sndoggvorbis_mainloop();
void oggx_sndoggvorbis_wait_start();
void oggx_sndoggvorbis_thd_quit();

static void oggx_sndserver_thread(void *blagh) {
	printf("sndserver: pid is %d\n", thd_get_current()->tid);
	oggx_sndoggvorbis_mainloop();
}

int oggx_sndoggvorbis_init() {
	if (thd) {
		printf("sndserver: already initialized!\n");
		return -1;
	}

	printf("sndserver: initializing sndoggvorbis 0.7 [OggVorbis 1.0 based]\n");
	thd = thd_create(oggx_sndserver_thread, NULL);
	if (thd != NULL) {
		/* Wait until the oggvorbis decoder thread is ready */
		oggx_sndoggvorbis_wait_start();
		printf("sndserver: successfully created thread\n");
		return 0;
	} else {
		printf("sndserver: error creating thread\n");
		return -1;
	}
}

void oggx_sndoggvorbis_thd_quit();

void oggx_sndoggvorbis_shutdown() {
	if (!thd) {
		printf("sndserver: not initialized!\n");
		return;
	}

	oggx_sndoggvorbis_thd_quit();
	thd_wait(thd);
	
	thd_destroy(thd);
	thd = NULL;

	printf("sndserver: exited successfully\n");
}
