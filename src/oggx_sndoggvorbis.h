/* KallistiOS 1.2.0

   sndoggvorbis.h
   (c)2001 Thorsten Titze

   An Ogg/Vorbis player library using sndstream and the official Xiphophorus
   libogg and libvorbis libraries.

   $Id: sndoggvorbis.h,v 1.2 2003/04/24 02:53:30 bardtx Exp $
   
*/

#ifndef __SNDOGGVORBIS_H
#define __SNDOGGVORBIS_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <stdio.h>

int oggx_sndoggvorbis_init();
int oggx_sndoggvorbis_start(const char *filename,int loop);
int oggx_sndoggvorbis_start_fd(FILE *f, int loop);
void oggx_sndoggvorbis_stop();
void oggx_sndoggvorbis_shutdown();

int oggx_sndoggvorbis_isplaying();

void oggx_sndoggvorbis_volume(int vol);

void oggx_sndoggvorbis_mainloop();
void oggx_sndoggvorbis_wait_start();

void oggx_sndoggvorbis_setbitrateinterval(int interval);
long oggx_sndoggvorbis_getbitrate();
long oggx_sndoggvorbis_getposition();

char *oggx_sndoggvorbis_getcommentbyname(const char *commentfield);
char *oggx_sndoggvorbis_getartist();
char *oggx_sndoggvorbis_gettitle();
char *oggx_sndoggvorbis_getgenre();

/* Enable/disable queued waiting */
void oggx_sndoggvorbis_queue_enable();
void oggx_sndoggvorbis_queue_disable();

/* Wait for the song to be queued */
void oggx_sndoggvorbis_queue_wait();

/* Queue the song to start if it's in QUEUED */
void oggx_sndoggvorbis_queue_go();

__END_DECLS

#endif	/* __SNDOGGVORBIS_H */

