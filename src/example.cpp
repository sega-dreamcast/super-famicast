/* png example for KOS 1.1.x
 * Jeffrey McBeth / Morphogenesis
 * <mcbeth@morphogenesis.2y.net>
 *
 * Heavily borrowed from from 2-D example
 * AndrewK / Napalm 2001
 * <andrewk@napalm-x.com>
 */

#include <kos.h>
#include <os/process.h>
#include <plx/font.h>
#include <png/png.h>
#include "vorbisfile.h"
#include <mp3/sndmp3.h>
#include <math.h>

#include "snes9x.h"
#include "memmap.h"
#include "debug.h"
#include "cpuexec.h"
#include "ppu.h"
#include "snapshot.h"
#include "apu.h"
#include "display.h"
#include "gfx.h"
#include "soundux.h"
#include "spc700.h"
#include "dc_menu.h"
#include "dc_utils.h"
#include "pvr_texture.h"
#include "XML.h"
#include "dc_controller.h"
#include "dc_file_browser.h"

#ifndef timercmp
#define timercmp(tvp, uvp, cmp)\
        ((tvp)->tv_sec cmp (uvp)->tv_sec ||\
        (tvp)->tv_sec == (uvp)->tv_sec &&\
        (tvp)->tv_usec cmp (uvp)->tv_usec)
#endif

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define SUPER_FAMICAST_FREQ	22050

#define ANALOG_THRESHOLD	(128 / 2)

#define IN_GAME_COLOR	PVR_PACK_COLOR(1.0, 0.4, 0.4, 0.6)

#define COLOR_255_TO_FLOAT(base255)	(((float) base255) / ((float) 0xFF))

char rom_filename[0x100];
const char *progname = "*SuperFamiNet*";

uint32 joypads [4] = {0};
int NumControllers = 4;

unsigned int GFX_Screen_Size;
void* clear_cache_start;
unsigned int clear_cache_size;

#define SNES_TEXTURE_ADDRS_SIZE 4
static pvr_ptr_t snes_texture_addrs[SNES_TEXTURE_ADDRS_SIZE];
static pvr_poly_hdr_t snes_pvr_poly_headers_filter_none[SNES_TEXTURE_ADDRS_SIZE];
static pvr_poly_hdr_t snes_pvr_poly_headers_filter_bilinear[SNES_TEXTURE_ADDRS_SIZE];

char theme_dir[0x100];

plx_font_t* fnt;
plx_fcxt_t* cxt;

DCMenu main_menu;
DCMenu options_menu;
DCMenu controller_menu;
DCMenu theme_menu;
DCFileBrowser rom_browser;

CPVRTexture background_texture;
CPVRTexture* rom_icon = NULL;
CPVRTexture* folder_icon = NULL;

int32 main_texture_index = -1;
int32 options_texture_index = -1;
int32 roms_texture_index = -1;
int32 controllers_texture_index = -1;
int32 theme_texture_index = -1;
int32 load_rom_texture_index = -1;

bool do_menu = true;
bool game_loaded = false;
bool bilinear_filtering = true;
bool use_analog[] = {false, false, false, false};
bool auto_save_sram = false;
bool sound_enabled = false;

bool has_little = false;
float little_x = -1;
float little_y = -1;
float little_x2 = -1;
float little_y2 = -1;

float adj_little_x = -1;
float adj_little_y = -1;
float adj_little_x2 = -1;
float adj_little_y2 = -1;

bool single_game_mode = false;
bool checking_single = false;
bool just_entered_game = false;

bool in_main_menu_system = false;

int texture_index = 0;

void ShowMsg(const char* str);
void InitTheme();
void ShowSplashIndex(int32 my_index, uint16 duration, uint16 fade_duration, void (*work_func)() = NULL);
void ShowSplash(const char* filename, uint16 duration, uint16 fade_duration, void (*work_func)() = NULL);

struct
{
	int32 x1, y1, x2, y2;
	float xscale, yscale;
} screen_adjustments = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1.0f};

struct SSuperFamicastSettings
{
	uint16 version;
	//VERSION 1
	int32 screen_x1;
	int32 screen_y1;
	int32 screen_x2;
	int32 screen_y2;
	float screen_xscale;
	float screen_yscale;
	bool bilinear_filtering;
	bool use_analog[4];
	uint32 SkipFrames;
	bool8  DisplayFrameRate;
	char theme_dir[0x100];
	bool auto_save_sram;
	bool sound_enabled;
	//VERSION 2
};

bool music_is_playing = false;
FILE* ogg_fp = NULL;
OggVorbis_File vf;

#define SFCAST_SOUND_BUF_SIZE	MAX_BUFFER_SIZE
#define OGG_BUF_SIZE 65536
uint8 sound_buf[OGG_BUF_SIZE];
uint8 ogg_sound_buf[OGG_BUF_SIZE];
uint8* pcm_ptr = ogg_sound_buf;
int32 pcm_count = 0;			/* bytes in buffer */
int32 last_read = 0;			/* number of bytes the sndstream driver grabbed at last callback */
int tempcounter = 0;
static int current_section;

void* interfaceGetSound(int samples_requested, int* samples_returned);

#define I_HAVE_MENU_MUSIC 1

void StartMusic()
{
#if I_HAVE_MENU_MUSIC
	if (!music_is_playing)
	{
		char ctemp[0x1000];
		sprintf(ctemp, "/cd/themes/%s/music.ogg", theme_dir);
		ogg_fp = fopen(ctemp, "rb");
		if (ogg_fp)
		{
			if (ov_open(ogg_fp, &vf, NULL, 0) < 0)
			{
				fclose(ogg_fp);
				return;
			}
			memset(ogg_sound_buf, 0, OGG_BUF_SIZE);
			vorbis_info* vi = ov_info(&vf, -1);
			snd_stream_init(interfaceGetSound);
			snd_stream_start(vi->rate, vi->channels - 1);
			music_is_playing = true;
		}
	}
#endif
}

void StopMusic()
{
#if I_HAVE_MENU_MUSIC
	if (music_is_playing)
	{
		snd_stream_stop();
		snd_stream_shutdown();
		pcm_count = 0;
		last_read = 0;
		pcm_ptr = ogg_sound_buf;
		ov_clear(&vf);
		fclose(ogg_fp);
		ogg_fp = NULL;
		music_is_playing = false;
	}
#endif
}

bool FindFirstVMU(char* outPath)
{
	file_t fpd = fs_open("/vmu", O_DIR | O_RDONLY);
	if (!fpd)
		return false;
	dirent_t* dirinfo = fs_readdir(fpd);
	if (dirinfo)
	{
		sprintf(outPath, "/vmu/%s", dirinfo->name);
		fs_close(fpd);
		return true;
	}
	fs_close(fpd);
	return false;
}

void SaveSettings()
{
	file_t fpd = fs_open("/vmu", O_DIR | O_RDONLY);
	if (!fpd)
		return;
	dirent_t* dirinfo;
	uint8 vmu_count = 0;
	while ((dirinfo = fs_readdir(fpd)))
	{
		++vmu_count;
		char ctemp[0x100];
		sprintf(ctemp, "/vmu/%s/sfcast01.cfg", dirinfo->name);
		FILE* fp = fopen(ctemp, "wb");
		if (!fp)
		{
			sprintf(ctemp, "Error opening file on VMU %s", dirinfo->name);
			ShowMsg(ctemp);
			fs_close(fpd);
			return;
		}
		
		SSuperFamicastSettings out_settings;
		out_settings.version = 1;
		out_settings.screen_x1 = screen_adjustments.x1;
		out_settings.screen_y1 = screen_adjustments.y1;
		out_settings.screen_x2 = screen_adjustments.x2;
		out_settings.screen_y2 = screen_adjustments.y2;
		out_settings.screen_xscale = screen_adjustments.xscale;
		out_settings.screen_yscale = screen_adjustments.yscale;
		out_settings.bilinear_filtering = bilinear_filtering;
		out_settings.use_analog[0] = use_analog[0];
		out_settings.use_analog[1] = use_analog[1];
		out_settings.use_analog[2] = use_analog[2];
		out_settings.use_analog[3] = use_analog[3];
		out_settings.SkipFrames = Settings.SkipFrames;
		out_settings.DisplayFrameRate = Settings.DisplayFrameRate;
		out_settings.auto_save_sram = auto_save_sram;
		strcpy(out_settings.theme_dir, theme_dir);
		out_settings.sound_enabled = sound_enabled;
		
		fwrite(&out_settings, sizeof(SSuperFamicastSettings), 1, fp);
		fclose(fp);
		sprintf(ctemp, "Settings saved to VMU %s", dirinfo->name);
		ShowMsg(ctemp);
		break;
	}
	fs_close(fpd);
}

void LoadSettings()
{
	file_t fpd = fs_open("/vmu", O_DIR | O_RDONLY);
	if (!fpd)
		return;
	dirent_t* dirinfo;
	bool loaded = false;
	while ((dirinfo = fs_readdir(fpd)))
	{
		char ctemp[0x100];
		sprintf(ctemp, "/vmu/%s/sfcast01.cfg", dirinfo->name);
		FILE* fp = fopen(ctemp, "rb");
		if (!fp)
			continue;
		
		SSuperFamicastSettings out_settings;
		
		fread(&out_settings, sizeof(SSuperFamicastSettings), 1, fp);
		fclose(fp);
		
		switch (out_settings.version)
		{
		default:
			screen_adjustments.x1 = out_settings.screen_x1;
			screen_adjustments.y1 = out_settings.screen_y1;
			screen_adjustments.x2 = out_settings.screen_x2;
			screen_adjustments.y2 = out_settings.screen_y2;
			screen_adjustments.xscale = out_settings.screen_xscale;
			screen_adjustments.yscale = out_settings.screen_yscale;
			bilinear_filtering = out_settings.bilinear_filtering;
			use_analog[0] = out_settings.use_analog[0];
			use_analog[1] = out_settings.use_analog[1];
			use_analog[2] = out_settings.use_analog[2];
			use_analog[3] = out_settings.use_analog[3];
			Settings.SkipFrames = out_settings.SkipFrames;
			Settings.DisplayFrameRate = out_settings.DisplayFrameRate;
			auto_save_sram = out_settings.auto_save_sram;
			strcpy(theme_dir, out_settings.theme_dir);
			sound_enabled = out_settings.sound_enabled;
		};
		loaded = true;
		break;
	}
	//if (!loaded)
	//	ShowMsg("No VMU contains settings");
}

static pvr_init_params_t pvr_params = 
{
  /* Enable opaque and translucent polygons with size 16 */
  { PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0 },
  
  /* Vertex buffer size 512K */
  512*1024
};

static void texture_init ()
{
	//SNES SCREEN TEXTURES
	int i;
	pvr_poly_cxt_t poly;
	pvr_ptr_t addr;

	for (i = 0; i < SNES_TEXTURE_ADDRS_SIZE; ++i) 
	{
		addr = pvr_mem_malloc (256 * 256 * 2);
		snes_texture_addrs[i] = addr; 
		pvr_poly_cxt_txr (&poly, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565 | PVR_TXRFMT_NONTWIDDLED, 256, 256, addr, PVR_FILTER_NONE);
		pvr_poly_compile (snes_pvr_poly_headers_filter_none + i, &poly);
		pvr_poly_cxt_txr (&poly, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565 | PVR_TXRFMT_NONTWIDDLED, 256, 256, addr, PVR_FILTER_BILINEAR);
		pvr_poly_compile (snes_pvr_poly_headers_filter_bilinear + i, &poly);
	}
}

static void dc_snesscreen_commit_texture(pvr_poly_hdr_t *poly, uint32 color = 0xffffffff)
{
	pvr_vertex_t vert;

	pvr_prim (poly, sizeof(pvr_poly_hdr_t));

	vert.flags = PVR_CMD_VERTEX;
	vert.x = screen_adjustments.x1;
	vert.y = screen_adjustments.y2;
	vert.z = 10;
	vert.u = 0;
	vert.v = 224.0f/256.0f;
	vert.argb = color;
	vert.oargb = 0;
	pvr_prim (&vert, sizeof(vert));

	vert.x = screen_adjustments.x1;
	vert.y = screen_adjustments.y1;
	vert.u = 0;
	vert.v = 0;
	pvr_prim (&vert, sizeof(vert));

	vert.x = screen_adjustments.x2;
	vert.y = screen_adjustments.y2;
	vert.u = 1.0;
	vert.v = 224.0f/256.0f;
	pvr_prim (&vert, sizeof(vert));

	vert.flags = PVR_CMD_VERTEX_EOL;
	vert.x = screen_adjustments.x2;
	vert.y = screen_adjustments.y1;
	vert.u = 1.0;
	vert.v = 0;
	pvr_prim (&vert, sizeof(vert));
}

static void commit_texture_fullscreen(pvr_poly_hdr_t* poly, uint16 width, uint16 height, uint16 tex_width, uint16 tex_height, float alpha)
{
	pvr_vertex_t vert;

	pvr_prim (poly, sizeof(pvr_poly_hdr_t));
	
	float u2 = ((float) width) / ((float) tex_width);
	float v2 = ((float) height) / ((float) tex_height);

	vert.flags = PVR_CMD_VERTEX;
	vert.x = 0;
	vert.y = SCREEN_HEIGHT;
	vert.z = 10;
	vert.u = 0;
	vert.v = v2;
	vert.argb = PVR_PACK_COLOR(alpha, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	pvr_prim (&vert, sizeof(vert));

	vert.x = 0;
	vert.y = 0;
	vert.u = 0;
	vert.v = 0;
	pvr_prim (&vert, sizeof(vert));

	vert.x = SCREEN_WIDTH;
	vert.y = SCREEN_HEIGHT;
	vert.u = u2;
	vert.v = v2;
	pvr_prim (&vert, sizeof(vert));

	vert.flags = PVR_CMD_VERTEX_EOL;
	vert.x = SCREEN_WIDTH;
	vert.y = 0;
	vert.u = u2;
	vert.v = 0;
	pvr_prim (&vert, sizeof(vert));
}

static void display_snes_screen ()
{
  pvr_wait_ready ();
  sq_cpy(snes_texture_addrs[texture_index], GFX.Screen, GFX_Screen_Size);
  //dcache_flush_range(GFX.Screen, GFX_Screen_Size);
  //pvr_txr_load_dma(GFX.Screen, snes_texture_addrs[texture_index], GFX_Screen_Size, 1);
  pvr_scene_begin ();
  pvr_list_begin (PVR_LIST_OP_POLY);
  if (bilinear_filtering)
    dc_snesscreen_commit_texture(snes_pvr_poly_headers_filter_bilinear + texture_index);
  else
    dc_snesscreen_commit_texture(snes_pvr_poly_headers_filter_none + texture_index);
  pvr_list_finish();
  pvr_scene_finish ();
  
  texture_index = (texture_index + 1) & (SNES_TEXTURE_ADDRS_SIZE - 1);
}

/* romdisk */
extern uint8 romdisk_boot[];
KOS_INIT_ROMDISK(romdisk_boot);

void _splitpath (const char *path, char *drive, char *dir, char *fname, char *ext)
{
    *drive = 0;

    char *slash = strrchr (path, '/');
    if (!slash)
	slash = strrchr (path, '\\');

    char *dot = strrchr (path, '.');

    if (dot && slash && dot < slash)
	dot = NULL;

    if (!slash)
    {
	strcpy (dir, "");
	strcpy (fname, path);
        if (dot)
        {
	    *(fname + (dot - path)) = 0;
	    strcpy (ext, dot + 1);
        }
	else
	    strcpy (ext, "");
    }
    else
    {
	strcpy (dir, path);
	*(dir + (slash - path)) = 0;
	strcpy (fname, slash + 1);
        if (dot)
	{
	    *(fname + (dot - slash) - 1) = 0;
    	    strcpy (ext, dot + 1);
	}
	else
	    strcpy (ext, "");
    }
}

void _makepath (char *path, const char *, const char *dir, const char *fname, const char *ext)
{
    if (dir && *dir)
    {
	strcpy (path, dir);
	strcat (path, "/");
    }
    else
	*path = 0;
    strcat (path, fname);
    if (ext && *ext)
    {
        strcat (path, ".");
        strcat (path, ext);
    }
}

const char *S9xGetFilename (const char *e)
{
	return NULL;
}

extern "C" const char *S9xGetFilenameInc (const char *e)
{
	return NULL;
}

extern "C" void S9xSyncSpeed(void)
{
    if (!Settings.TurboMode && Settings.SkipFrames == AUTO_FRAMERATE)
    {
	    static struct timeval next1 = {0, 0};
	    struct timeval now;

	    while (gettimeofday (&now, NULL) < 0) ;
		if (next1.tv_sec == 0)
		{
		  	next1 = now;
		   	next1.tv_usec++;
	    }

	    if (timercmp(&next1, &now, >))
	    {
	    	if (IPPU.SkippedFrames == 0)
	    	{
	    		do
	    		{
	    			CHECK_SOUND ();
	    		    S9xProcessEvents (FALSE);
	    		    while (gettimeofday (&now, NULL) < 0) ;
  		        } while (timercmp(&next1, &now, >));
 		    }
	     	IPPU.RenderThisFrame = TRUE;
	    	IPPU.SkippedFrames = 0;
    	}
    	else
		{
			if (IPPU.SkippedFrames < 10)
			{
				IPPU.SkippedFrames++;
				IPPU.RenderThisFrame = FALSE;
			}
			else
			{
				IPPU.RenderThisFrame = TRUE;
				IPPU.SkippedFrames = 0;
				next1 = now;
			}
		}
		next1.tv_usec += Settings.FrameTime;
		if (next1.tv_usec >= 1000000)
		{
			next1.tv_sec += next1.tv_usec / 1000000;
			next1.tv_usec %= 1000000;
		}
    }
    else
    {
	    if (++IPPU.FrameSkip >= (Settings.TurboMode ? Settings.TurboSkipFrames
						    : Settings.SkipFrames))
	    {
	    	IPPU.FrameSkip = 0;
	    	IPPU.SkippedFrames = 0;
	    	IPPU.RenderThisFrame = TRUE;
    	}
    	else
    	{
	    	IPPU.SkippedFrames++;
	    	IPPU.RenderThisFrame = FALSE;
    	}
    }
}

extern "C" bool8 S9xInitUpdate (void)
{
	return TRUE;
}

int update_count = 0;

extern "C" bool8 S9xDeinitUpdate (int Width, int Height, bool8 sixteen_bit)
{
	display_snes_screen();
	return TRUE;
}

extern "C" void S9xSetPalette(void)
{
	
}

bool FamicastSaveSRAM()
{
	char vmu_path[0x100];
	if (FindFirstVMU(vmu_path))
	{
		char ctemp[0x100];
		sprintf(ctemp, "%s/%X.srm", vmu_path, Memory.ROMCRC32);
		if (Memory.SaveSRAM(ctemp))
			return true;
	}
	return false;
}

extern "C" void S9xAutoSaveSRAM(void)
{
	if (auto_save_sram)
		FamicastSaveSRAM();
}

extern "C" void S9xMessage(int, int, const char *str)
{
	if (!checking_single)
    	S9xSetInfoString(str);
}

extern "C" void S9xLoadSDD1Data()
{
	Memory.FreeSDD1Data();
	Settings.SDD1 = FALSE;
	Settings.SDD1Pack = FALSE;
    if (strncmp (Memory.ROMName, "Star Ocean", 10) == 0
    	|| strncmp(Memory.ROMName, "STREET FIGHTER ALPHA2", 21) == 0)
    {
        Settings.SDD1 = TRUE;
		Settings.SDD1Pack = TRUE;
    }
    else
	{
		Settings.SDD1 = FALSE;
		Settings.SDD1Pack = FALSE;
	}
}

extern "C" bool8 S9xReadMousePosition (int which, int &x, int &y, uint32 &buttons)
{
	x = 0;
	y = 0;
	buttons = 0;
	return TRUE;
}

extern "C" bool8 S9xReadSuperScopePosition (int &x, int &y, uint32 &buttons)
{
	x = 0;
	y = 0;
	buttons = 0;
	return TRUE;
}

bool JustifierOffscreen()
{
	return false;
}

void JustifierButtons(uint32& justifiers)
{

}

uint32 S9xReadJoypad (int which1)
{
	if (which1 < NumControllers)
		return (0x80000000 | joypads [which1]);
	return 0;
}

bool8 S9xOpenSnapshotFile( const char *fname, bool8 read_only, STREAM *file)
{
	return TRUE;
}

void S9xCloseSnapshotFile( STREAM file)
{

}

const char *S9xBasename (const char *f)
{
	return NULL;
}

void S9xExtraUsage ()
{

}

void S9xParseArg (char **argv, int &i, int argc)
{

}

bool8 S9xOpenSoundDevice (int mode, bool8 pStereo, int BufferSize)
{
	so.playback_rate = SUPER_FAMICAST_FREQ;
    so.stereo = Settings.Stereo;
    so.sixteen_bit = Settings.SixteenBitSound;
    so.buffer_size = SFCAST_SOUND_BUF_SIZE;
    so.mute_sound = FALSE;
    so.encoded = 0;
    S9xSetPlaybackRate(so.playback_rate);
    
	memset(sound_buf, 0, SFCAST_SOUND_BUF_SIZE);
	
	return TRUE;
}

void S9xGenerateSound ()
{
	snd_stream_poll();
}

bool first_sfcastGetSound = false;

void* sfcastGetSound(int samples_requested, int* samples_returned)
{
	if (first_sfcastGetSound)
	{
		*samples_returned = samples_requested;
		first_sfcastGetSound = false;
		return sound_buf;
	}
	if ((samples_requested / 2) > (SFCAST_SOUND_BUF_SIZE / 4))
		samples_requested = SFCAST_SOUND_BUF_SIZE / 4;
	S9xMixSamplesO(sound_buf, samples_requested / 2, 0);
	*samples_returned = samples_requested;
	return sound_buf;
}

void* interfaceGetSound(int samples_requested, int* samples_returned)
{
	/*
	if ((samples_requested / 2) > (SFCAST_SOUND_BUF_SIZE / 4))
		samples_requested = SFCAST_SOUND_BUF_SIZE / 4;
	
	*samples_returned = samples_requested;
	
	return sound_buf;
	*/
	
	if (samples_requested > OGG_BUF_SIZE)
		samples_requested = OGG_BUF_SIZE;
	if (last_read > 0)
 	{
		pcm_count -= last_read;
		if (pcm_count < 0)
			pcm_count=0;
		memcpy(ogg_sound_buf, ogg_sound_buf + last_read, pcm_count);
		pcm_ptr = ogg_sound_buf + pcm_count;
	}
	long pcm_decoded = 0;
	while(pcm_count < samples_requested)
 	{
		pcm_decoded = ov_read(&vf, (char*) pcm_ptr, 4096, 0, 2, 1, &current_section);
		if (pcm_decoded == 0)
  		{
			if (ov_raw_seek(&vf, 0) < 0)
				dbglog(DBG_ERROR, "sndoggvorbis: can't ov_raw_seek to the beginning!\n");
   			else
				pcm_decoded = ov_read(&vf, (char*) pcm_ptr, 4096, 0, 2, 1, &current_section);
		}
		if (pcm_decoded < 0)
		{
			StopMusic();
			*samples_returned = 0;
			return NULL;
		}
		pcm_count += pcm_decoded;
		pcm_ptr += pcm_decoded;
		if (pcm_decoded == 0)
			break;
	}
	if (pcm_count > samples_requested)
		*samples_returned = samples_requested;
	else
		*samples_returned = pcm_count;
	last_read = *samples_returned;
	if (pcm_decoded == 0)
		return NULL;
	else
		return ogg_sound_buf;
}

void S9xExit( void)
{
	printf("S9xExit called... that shouldnt happen!\n");
	exit (1);
}

const char *S9xGetSnapshotDirectory ()
{
	return NULL;
}

extern "C" char* osd_GetPackDir()
{
	return NULL;
}

void OutOfMemory ()
{
    printf ("Snes9X: Memory allocation failure - not enough RAM/virtual memory available.\n\
        S9xExiting...\n");
    Memory.Deinit ();
    S9xDeinitAPU ();
    
    exit (1);
}

void S9xInitDisplay()
{
	GFX.Pitch = IMAGE_WIDTH * 2;
	GFX_Screen_Size = GFX.Pitch * IMAGE_HEIGHT;
	GFX.Screen = (uint8 *) memalign(32, GFX_Screen_Size);
	GFX.SubScreen = (uint8 *) malloc (GFX_Screen_Size);
	GFX.ZBuffer = (uint8 *) malloc ((GFX.Pitch >> 1) * IMAGE_HEIGHT);
	GFX.SubZBuffer = (uint8 *) malloc ((GFX.Pitch >> 1) * IMAGE_HEIGHT);
	
	clear_cache_size = GFX.Pitch * (IMAGE_HEIGHT / 2);
	clear_cache_start = GFX.Screen + (GFX_Screen_Size - clear_cache_size);
	
	if (!GFX.Screen || !GFX.SubScreen)
	{
		printf ("Cannot allocate screen buffer.\n");
		S9xExit ();
	}
	
	ZeroMemory (GFX.Screen, GFX_Screen_Size);
    if (GFX.SubScreen)
		ZeroMemory (GFX.SubScreen, GFX_Screen_Size);
}

void S9xSetTitle (const char * /*title*/)
{

}

void S9xProcessEvents (bool8 block)
{
	
}

void InGameMenuDrawExtra(DCMenu* pMenu)
{
	if (bilinear_filtering)
		dc_snesscreen_commit_texture(snes_pvr_poly_headers_filter_bilinear + texture_index, IN_GAME_COLOR);
	else
		dc_snesscreen_commit_texture(snes_pvr_poly_headers_filter_none + texture_index, IN_GAME_COLOR);
}

#define SCREEN_ADJUST_ANALOG_THRESHOLD		10
#define SCREEN_ADJUST_DIGITAL_THRESHOLD		3

void OnAdjustScreen(DCMenu* pMenu, DCMenuItem* pItem, int value)
{
	float x_accum = 0.0f;
	float y_accum = 0.0f;
	
	int up_accum = 0;
	int down_accum = 0;
	int left_accum = 0;
	int right_accum = 0;
	
	int ingameadjust = (int) pItem->user_data;
	
	if (ingameadjust)
	{
		uint16* textemp = (uint16*) snes_texture_addrs[texture_index];
		for (uint32 k = 0; k < 224; ++k)
		{
			uint16* rowtemp = &textemp[k*256];
			if (k == 0 || k == 223)
			{
				for (uint32 n = 0; n < 256; ++n)
					rowtemp[n] = 0xffffffff;
			}
			else
			{
				rowtemp[0] = 0xffffffff;
				rowtemp[255] = 0xffffffff;
			}
		}
	}
	
	cxt->color = 0xffffffff;
	
	float outleft, outup, outright, outdown;
	
	const char* szAdjustMsg = "* Adjust Screen *";
	const char* szDPadMsg = "Move With D-Pad";
	const char* szAnalogMsg = "Resize With Analog Stick";
	
	while (true)
	{
		pvr_wait_ready ();
		pvr_scene_begin ();
		if (ingameadjust)
		{
			pvr_list_begin (PVR_LIST_OP_POLY);
			if (bilinear_filtering)
				dc_snesscreen_commit_texture(snes_pvr_poly_headers_filter_bilinear + texture_index);
			else
				dc_snesscreen_commit_texture(snes_pvr_poly_headers_filter_none + texture_index);
			pvr_list_finish();
		}
		
		float oldfontsize = cxt->size;
		cxt->size = cxt->size * screen_adjustments.yscale;
		
		plx_fcxt_str_metrics(cxt, szAdjustMsg, &outleft, &outup, &outright, &outdown);
		point_t adjust_w;
		adjust_w.x = ((640.0f * screen_adjustments.xscale) - outright) / 2;
		adjust_w.y = (((480.0f * screen_adjustments.yscale) - outup) / 2) - (outup + 10);
		adjust_w.z = 15;
		
		plx_fcxt_str_metrics(cxt, szDPadMsg, &outleft, &outup, &outright, &outdown);
		point_t dpad_w;
		dpad_w.x = ((640.0f * screen_adjustments.xscale) - outright) / 2;
		dpad_w.y = (((480.0f * screen_adjustments.yscale) - outup) / 2);
		dpad_w.z = 15;
		
		plx_fcxt_str_metrics(cxt, szAnalogMsg, &outleft, &outup, &outright, &outdown);
		point_t analog_w;
		analog_w.x = ((640.0f * screen_adjustments.xscale) - outright) / 2;
		analog_w.y = (((480.0f * screen_adjustments.yscale) - outup) / 2) + (outup + 10);
		analog_w.z = 15;
		
		cxt->size = oldfontsize;
		
		pvr_list_begin(PVR_LIST_TR_POLY);
		
		if (!ingameadjust)
			background_texture.DrawFullscreen(10);
		
		DrawStringWithBorder(szAdjustMsg, cxt, adjust_w, 0xffffffff, PVR_PACK_COLOR(1.0f, 0, 0, 0));
		DrawStringWithBorder(szDPadMsg, cxt, dpad_w, 0xffffffff, PVR_PACK_COLOR(1.0f, 0, 0, 0));
		DrawStringWithBorder(szAnalogMsg, cxt, analog_w, 0xffffffff, PVR_PACK_COLOR(1.0f, 0, 0, 0));
		pvr_list_finish();
		
		pvr_scene_finish ();
		
		if (in_main_menu_system)
			snd_stream_poll();
		
		DCController* controller = g_controllers;
		controller->Poll();
		
		if (controller->GetJoyX() < -SCREEN_ADJUST_ANALOG_THRESHOLD || controller->GetJoyX() > SCREEN_ADJUST_ANALOG_THRESHOLD)
			x_accum += ((float) controller->GetJoyX()) / 100;
		
		if (controller->GetJoyY() < -SCREEN_ADJUST_ANALOG_THRESHOLD || controller->GetJoyY() > SCREEN_ADJUST_ANALOG_THRESHOLD)
			y_accum += ((float) controller->GetJoyY()) / 100;
		
		if (x_accum > 1.0f)
		{
			++screen_adjustments.x2;
			x_accum = 0.0f;
		}
		
		if (x_accum < -1.0f)
		{
			--screen_adjustments.x2;
			x_accum = 0.0f;
		}
		
		if (y_accum > 1.0f)
		{
			++screen_adjustments.y2;
			y_accum = 0.0f;
		}
		
		if (y_accum < -1.0f)
		{
			--screen_adjustments.y2;
			y_accum = 0.0f;
		}
		
		if (controller->IsPressed(CONT_DPAD_UP))
		{
			if (up_accum > SCREEN_ADJUST_DIGITAL_THRESHOLD)
			{
				--screen_adjustments.y1;
				--screen_adjustments.y2;
				up_accum = 0;
			}
			else
				++up_accum;
		}
		
		if (controller->IsPressed(CONT_DPAD_DOWN))
		{
			if (down_accum > SCREEN_ADJUST_DIGITAL_THRESHOLD)
			{
				++screen_adjustments.y1;
				++screen_adjustments.y2;
				down_accum = 0;
			}
			else
				++down_accum;
		}
		
		if (controller->IsPressed(CONT_DPAD_LEFT))
		{
			if (left_accum > SCREEN_ADJUST_DIGITAL_THRESHOLD)
			{
				--screen_adjustments.x1;
				--screen_adjustments.x2;
				left_accum = 0;
			}
			else
				++left_accum;
		}
		
		if (controller->IsPressed(CONT_DPAD_RIGHT))
		{
			if (right_accum > SCREEN_ADJUST_DIGITAL_THRESHOLD)
			{
				++screen_adjustments.x1;
				++screen_adjustments.x2;
				right_accum = 0;
			}
			else
				++right_accum;
		}
		
		screen_adjustments.xscale = (((float)screen_adjustments.x2) - ((float)screen_adjustments.x1)) / 640.0f;
		screen_adjustments.yscale = (((float)screen_adjustments.y2) - ((float)screen_adjustments.y1)) / 480.0f;
		
		if (controller->JustPressed(CONT_A) || controller->JustPressed(CONT_B))
			break;
	}
	if (has_little)
	{
		adj_little_x = screen_adjustments.x1 + (little_x * screen_adjustments.xscale);
		adj_little_y = screen_adjustments.y1 + (little_y * screen_adjustments.xscale);
		adj_little_x2 = screen_adjustments.x1 + (little_x2 * screen_adjustments.xscale);
		adj_little_y2 = screen_adjustments.y1 + (little_y2 * screen_adjustments.xscale);
	}
}

void OnResume(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	pMenu->Stop();
}

void OnReset(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	S9xSoftReset();
	pMenu->Stop();
}

void OnSaveSRAM(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	if (FamicastSaveSRAM())
		ShowMsg("SRAM saved successully");
	else
		ShowMsg("Error saving SRAM");
}
/*
int last_save_load_slot = 0;

void OnChooseSaveSlot(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	char vmu_path[0x100];
	if (FindFirstVMU(vmu_path))
	{
		char ctemp[0x1000];
		sprintf(ctemp, "%s/%X.%i", vmu_path, Memory.ROMCRC32, value);
		if (S9xFreezeGame(ctemp))
		{
			sprintf(ctemp, "Slot %i saved successfully", value);
			ShowMsg(ctemp);
		}
		else
		{
			sprintf(ctemp, "Error loading: %s/%X.%i", vmu_path, Memory.ROMCRC32, value);
			ShowMsg(ctemp);
		}
	}
	else
		ShowMsg("No VMU found");
}

void OnSaveState(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	DCMenu choose_state_menu(cxt, pMenu);
	choose_state_menu.force_callback_on_select = true;
	DCMenuItem* choice_item = choose_state_menu.AddItem("Choose a slot to save:", OnChooseSaveSlot);
	char ctemp[0x10];
	for (int k = 1; k <= 10; ++k)
	{
		sprintf(ctemp, "%i", k);
		choice_item->AddChoice(ctemp, k, k == last_save_load_slot);
	}
	choose_state_menu.Run();
}

void OnChooseLoadSlot(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	char vmu_path[0x100];
	if (FindFirstVMU(vmu_path))
	{
		char ctemp[0x1000];
		sprintf(ctemp, "%s/%X.%i", vmu_path, Memory.ROMCRC32, value);
		if (S9xUnfreezeGame(ctemp))
		{
			sprintf(ctemp, "Slot %i loaded successfully", value);
			ShowMsg(ctemp);
			pMenu->Stop(true);
		}
		else
		{
			sprintf(ctemp, "Error loading: %s/%X.%i", vmu_path, Memory.ROMCRC32, value);
			ShowMsg(ctemp);
		}
	}
	else
		ShowMsg("No VMU found");
}

void OnLoadState(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	DCMenu choose_state_menu(cxt, pMenu);
	choose_state_menu.force_callback_on_select = true;
	DCMenuItem* choice_item = choose_state_menu.AddItem("Choose a slot to load:", OnChooseLoadSlot);
	char ctemp[0x10];
	for (int k = 1; k <= 10; ++k)
	{
		sprintf(ctemp, "%i", k);
		choice_item->AddChoice(ctemp, k, k == last_save_load_slot);
	}
	choose_state_menu.Run();
}
*/
void InGameMenu()
{
	DCMenu in_game_menu(cxt);
	in_game_menu.x = 50;
 	in_game_menu.y = 36;
 	in_game_menu.depth = 15;
 	in_game_menu.max_width = SCREEN_WIDTH;
 	in_game_menu.max_height = SCREEN_HEIGHT - MIN_Y;
	in_game_menu.SetDrawExtraCallback(InGameMenuDrawExtra);
	in_game_menu.AddItem("Resume Game", OnResume);
	in_game_menu.AddItem("Save SRAM", OnSaveSRAM);
	//in_game_menu.AddItem("Save State", OnSaveState);
	//in_game_menu.AddItem("Load State", OnLoadState);
	DCMenuItem* adjust_item = in_game_menu.AddItem("Adjust Screen", OnAdjustScreen);
	adjust_item->user_data = (void*) 1;
	in_game_menu.AddItem("Reset Emulation", OnReset);
	in_game_menu.Run();
}

void ReadJoysticks ()
{
	DCController* controller;
	for (int k = 0; k < 4; ++k)
	{
		joypads[k] = 0;
		controller = g_controllers + k;
		if (!controller->GetDevice())
			continue;
		controller->Poll();
		if (k == 0 && just_entered_game && (controller->IsPressed(CONT_A) || controller->IsPressed(CONT_B)))
			continue;
		just_entered_game = false;
			
		if (use_analog[k])
		{
			if (controller->GetJoyY() < -ANALOG_THRESHOLD)
				joypads[k] |= SNES_UP_MASK;
			if (controller->GetJoyY() > ANALOG_THRESHOLD)
				joypads[k] |= SNES_DOWN_MASK;
			if (controller->GetJoyX() < -ANALOG_THRESHOLD)
				joypads[k] |= SNES_LEFT_MASK;
			if (controller->GetJoyX() > ANALOG_THRESHOLD)
				joypads[k] |= SNES_RIGHT_MASK;

			if (k == 0)
			{
				if (controller->IsPressed(CONT_DPAD_UP))
				{

				}
				else if (controller->IsPressed(CONT_DPAD_DOWN))
					joypads[k] |= SNES_SELECT_MASK;
				else if (controller->IsPressed(CONT_DPAD_LEFT))
				{
					if (controller->IsPressed(CONT_START))
					{
						Settings.Paused = TRUE;
						if (single_game_mode)
						{
							char ctemp[0x1000];
							sprintf(ctemp, "/cd/themes/%s/splash.bmp", theme_dir);
							ShowSplash(ctemp, 4000, 2000, InitTheme);
							single_game_mode = false;
						}
						return;
					}
					else if (controller->IsPressed(CONT_Y))
					{
						if (sound_enabled)
  							snd_stream_stop();
						InGameMenu();
						memset(sound_buf, 0, OGG_BUF_SIZE);
						if (sound_enabled)
						{
							first_sfcastGetSound = true;
  							snd_stream_start(SUPER_FAMICAST_FREQ, 1);
						}
					}
				}
				else if (controller->IsPressed(CONT_DPAD_RIGHT))
				{
					
				}
			}
		}
		else
		{
			if (k == 0)
			{
				if (controller->GetJoyY() < -ANALOG_THRESHOLD)
				{

				}
				else if (controller->GetJoyY() > ANALOG_THRESHOLD)
					joypads[k] |= SNES_SELECT_MASK;
				else if (controller->GetJoyX() < -ANALOG_THRESHOLD)
				{
					if (controller->IsPressed(CONT_START))
					{
						Settings.Paused = TRUE;
						if (single_game_mode)
						{
							char ctemp[0x1000];
							sprintf(ctemp, "/cd/themes/%s/splash.bmp", theme_dir);
							ShowSplash(ctemp, 4000, 2000, InitTheme);
							single_game_mode = false;
						}
						return;
					}
					else if (controller->IsPressed(CONT_Y))
					{
						if (sound_enabled)
  							snd_stream_stop();
						InGameMenu();
						memset(sound_buf, 0, OGG_BUF_SIZE);
						if (sound_enabled)
						{
							first_sfcastGetSound = true;
  							snd_stream_start(SUPER_FAMICAST_FREQ, 1);
						}
					}
				}
				else if (controller->GetJoyX() > ANALOG_THRESHOLD)
				{
					
				}
			}

			if (controller->IsPressed(CONT_DPAD_UP))
				joypads[k] |= SNES_UP_MASK;
			if (controller->IsPressed(CONT_DPAD_DOWN))
				joypads[k] |= SNES_DOWN_MASK;
			if (controller->IsPressed(CONT_DPAD_LEFT))
				joypads[k] |= SNES_LEFT_MASK;
			if (controller->IsPressed(CONT_DPAD_RIGHT))
				joypads[k] |= SNES_RIGHT_MASK;
		}
			
		if (controller->IsPressed(CONT_A))
			joypads[k] |= SNES_B_MASK;
		if (controller->IsPressed(CONT_B))
			joypads[k] |= SNES_A_MASK;
		if (controller->IsPressed(CONT_X))
			joypads[k] |= SNES_Y_MASK;
		if (controller->IsPressed(CONT_Y))
			joypads[k] |= SNES_X_MASK;
		if (controller->IsPressed(CONT_START))
			joypads[k] |= SNES_START_MASK;
		if (controller->GetRTrig() > 128)
			joypads[k] |= SNES_TR_MASK;
		if (controller->GetLTrig() > 128)
			joypads[k] |= SNES_TL_MASK;
	}
}

void S9xInitInputDevices ()
{
	
}

void S9xTextMode ()
{
	
}

void S9xGraphicsMode ()
{
	
}

void ShowSplashIndex(int32 my_index, uint16 duration, uint16 fade_duration, void (*work_func)())
{
	pvr_wait_ready();
	background_texture.SetCurrentIndex(my_index);
	
	uint16 fade_step = fade_duration / 60;
    if (fade_step == 0)
    	fade_step = 1;
	float alpha_step = 1.0f / ((float) fade_step);
	float cur_alpha = 0.0f;
	for (uint16 cur_fade = 0; cur_fade < (fade_step + 4); ++cur_fade)
	{
		if (cur_fade < fade_step)
		{
			if (cur_fade == (fade_step - 1))
				cur_alpha = 1.0f;
			else
				cur_alpha += alpha_step;
			if (cur_alpha > 1.0f)
				cur_alpha = 1.0f;
		}
		pvr_wait_ready();
		pvr_scene_begin();
		pvr_list_begin(PVR_LIST_TR_POLY);
		background_texture.DrawFullscreen(10, cur_alpha);
		pvr_list_finish();
		pvr_scene_finish();
	}
	if (work_func)
		work_func();
	else
		thd_sleep(duration);
	for (uint16 cur_fade = 0; cur_fade < (fade_step + 4); ++cur_fade)
	{
		if (cur_fade < fade_step)
		{
			if (cur_fade == (fade_step - 1))
				cur_alpha = 0.0f;
			else
				cur_alpha -= alpha_step;
			if (cur_alpha < 0.0f)
				cur_alpha = 0.0f;
		}
		pvr_wait_ready();
		pvr_scene_begin();
		pvr_list_begin(PVR_LIST_TR_POLY);
		background_texture.DrawFullscreen(10, cur_alpha);
		pvr_list_finish();
		pvr_scene_finish();
	}
}

void ShowSplash(const char* filename, uint16 duration, uint16 fade_duration, void (*work_func)())
{
	int32 my_index = background_texture.LoadBMP(filename);
	ShowSplashIndex(my_index, duration, fade_duration, work_func);
	background_texture.FreeBMP(my_index);
}

float current_little_x = 0;
float current_little_y = 0;
float current_little_x2 = 0;
float current_little_y2 = 0;
bool little_zoom_finished = false;
int32 little_zoom_steps = 30;
int32 little_current_step = 0;

void MainMenuDrawExtra(DCMenu* pMenu)
{
	if (main_texture_index >= 0 && background_texture.GetCurrentIndex() != main_texture_index)
		background_texture.SetCurrentIndex(main_texture_index);
	
	if (has_little && game_loaded)
	{
		pvr_poly_hdr_t* pHdr;
 		if (bilinear_filtering)
  			pHdr = snes_pvr_poly_headers_filter_bilinear + texture_index;
 		else
 			pHdr = snes_pvr_poly_headers_filter_none + texture_index;
		
		if (little_current_step < little_zoom_steps)
		{
			if (little_current_step == 0)
			{
				current_little_x = screen_adjustments.x1;
				current_little_y = screen_adjustments.y1;
				current_little_x2 = screen_adjustments.x2;
				current_little_y2 = screen_adjustments.y2;
			}
			else
			{
				current_little_x += ((adj_little_x - screen_adjustments.x1) / ((float) little_zoom_steps));
				current_little_y += ((adj_little_y - screen_adjustments.y1) / ((float) little_zoom_steps));
				current_little_x2 += ((adj_little_x2 - screen_adjustments.x2) / ((float) little_zoom_steps));
				current_little_y2 += ((adj_little_y2 - screen_adjustments.y2) / ((float) little_zoom_steps));
			}
			++little_current_step;
		}
		else
		{
			current_little_x = adj_little_x;
			current_little_y = adj_little_y;
			current_little_x2 = adj_little_x2;
			current_little_y2 = adj_little_y2;
		}
		
		pvr_vertex_t vert;

		pvr_prim (pHdr, sizeof(pvr_poly_hdr_t));

		vert.flags = PVR_CMD_VERTEX;
		vert.x = current_little_x;
		vert.y = current_little_y2;
		vert.z = 20;
		vert.u = 0;
		vert.v = 224.0f/256.0f;
		vert.argb = 0xFFFFFFFF;
		vert.oargb = 0;
		pvr_prim (&vert, sizeof(vert));

		vert.x = current_little_x;
		vert.y = current_little_y;
		vert.u = 0;
		vert.v = 0;
		pvr_prim (&vert, sizeof(vert));

		vert.x = current_little_x2;
		vert.y = current_little_y2;
		vert.u = 1.0;
		vert.v = 224.0f/256.0f;
		pvr_prim (&vert, sizeof(vert));

		vert.flags = PVR_CMD_VERTEX_EOL;
		vert.x = current_little_x2;
		vert.y = current_little_y;
		vert.u = 1.0;
		vert.v = 0;
		pvr_prim (&vert, sizeof(vert));
	}
}

void OptionsMenuDrawExtra(DCMenu* pMenu)
{
	if (options_texture_index >= 0 && background_texture.GetCurrentIndex() != options_texture_index)
		background_texture.SetCurrentIndex(options_texture_index);
}

void ControllerMenuDrawExtra(DCMenu* pMenu)
{
	if (controllers_texture_index >= 0 && background_texture.GetCurrentIndex() != controllers_texture_index)
		background_texture.SetCurrentIndex(controllers_texture_index);
}

void ThemeMenuDrawExtra(DCMenu* pMenu)
{
	if (theme_texture_index >= 0 && background_texture.GetCurrentIndex() != theme_texture_index)
		background_texture.SetCurrentIndex(theme_texture_index);
}

void RomMenuDrawExtra(DCMenu* pMenu)
{
	if (roms_texture_index >= 0 && background_texture.GetCurrentIndex() != roms_texture_index)
		background_texture.SetCurrentIndex(roms_texture_index);
}

bool load_success = false;

void LoadROMWorker()
{
	load_success = Memory.LoadROM(rom_filename);
}

void OnLoadGame(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	pvr_wait_ready();
	background_texture.SetCurrentIndex(roms_texture_index);
	if (rom_browser.Browse(true))
	{
		strcpy(rom_filename, rom_browser.GetChosenFilePath());
		uint32 saved_flags = CPU.Flags;
		StopMusic();
		ShowSplashIndex(load_rom_texture_index, 0, 0, LoadROMWorker);
		if (!load_success)
   		{
   			StartMusic();
   			char ctemp[0x100];
   			sprintf(ctemp, "Could not load rom: %s\n", rom_filename);
   			ShowMsg(ctemp);
   			pMenu->Stop();
   		}
   		else
   		{
   			game_loaded = true;
   			char vmu_path[0x100];
			if (FindFirstVMU(vmu_path))
			{
				char ctemp[0x100];
				sprintf(ctemp, "%s/%X.srm", vmu_path, Memory.ROMCRC32);
				Memory.LoadSRAM(ctemp);
			}
   			pMenu->Stop(true);
   		}
   		CPU.Flags = saved_flags;
	}
}

void OnChangeBilinear(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	bilinear_filtering = value ? true : false;
}

void OnChangeFrameskip(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	if (value == -1)
		Settings.SkipFrames = AUTO_FRAMERATE;
	else
		Settings.SkipFrames = value;
}

void OnChangeDisplayFrameRate(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	Settings.DisplayFrameRate = value;
}

void OnChangeAutoSRAM(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	auto_save_sram = value ? true : false;
}

bool go_through_with_it = false;

void OnChangeSoundDoReset(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	go_through_with_it = value ? true : false;
	if (g_controllers[0].JustPressed(CONT_A))
		pMenu->Stop();
}

void OnChangeSoundEnabled(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	if (game_loaded)
	{
		float old_font_size = cxt->size;
		cxt->size = cxt->size / 2;
		ShowMsg("WARNING: Enabling/Disabling sound requires a soft reset");
		go_through_with_it = false;
		
		float outleft, outup, outright, outdown;
		plx_fcxt_str_metrics(cxt, "Toggle sound anyways?", &outleft, &outup, &outright, &outdown);
		
		DCMenu confirm_reset_menu(cxt);
		//confirm_reset_menu.poll_sound_stream = true;
		confirm_reset_menu.force_callback_on_select = true;
		confirm_reset_menu.x = ((640.0f * screen_adjustments.xscale) - outright) / 2;
		confirm_reset_menu.y = (((480.0f * screen_adjustments.yscale) - outup) / 2) + (outup + 10);
		DCMenuItem* yes_no_item = confirm_reset_menu.AddItem("Toggle sound anyways?", OnChangeSoundDoReset);
		yes_no_item->AddChoice("Yes", 1);
		yes_no_item->AddChoice("No", 0, true);
		if (!confirm_reset_menu.Run() || !go_through_with_it)
		{
			pMenuItem->m_current_choice = pMenuItem->m_current_choice ? 0 : 1;
			cxt->size = old_font_size;
			return;
		}
		cxt->size = old_font_size;
	}
	if (value)
	{
		Settings.NextAPUEnabled = TRUE;
		Settings.SoundSync = TRUE;
		sound_enabled = true;
		S9xSetSoundMute(FALSE);
	}
	else
	{
		Settings.NextAPUEnabled = FALSE;
		Settings.SoundSync = FALSE;
		sound_enabled = false;
		S9xSetSoundMute(TRUE);
	}
	//if (game_loaded)
		S9xSoftReset();
}

void OnOptions(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	pvr_wait_ready();
	background_texture.SetCurrentIndex(options_texture_index);
	options_menu.RemoveAllItems();
	DCMenuItem* testchoice = options_menu.AddItem("Bilinear Filtering", OnChangeBilinear);
	testchoice->AddChoice("On", 1, bilinear_filtering);
	testchoice->AddChoice("Off", 0, !bilinear_filtering);
	
	testchoice = options_menu.AddItem("Frameskip", OnChangeFrameskip);
	testchoice->AddChoice("Auto", -1, Settings.DisplayFrameRate == AUTO_FRAMERATE);
	testchoice->AddChoice("0", 1, Settings.SkipFrames == 1);
	testchoice->AddChoice("1", 2, Settings.SkipFrames == 2);
	testchoice->AddChoice("2", 3, Settings.SkipFrames == 3);
	testchoice->AddChoice("3", 4, Settings.SkipFrames == 4);
	testchoice->AddChoice("4", 5, Settings.SkipFrames == 5);
	testchoice->AddChoice("5", 6, Settings.SkipFrames == 6);
	
	testchoice = options_menu.AddItem("Display Frame Rate", OnChangeDisplayFrameRate);
	testchoice->AddChoice("On", TRUE, Settings.DisplayFrameRate == TRUE);
	testchoice->AddChoice("Off", FALSE, Settings.DisplayFrameRate == FALSE);
	
	testchoice = options_menu.AddItem("Auto Save SRAM", OnChangeAutoSRAM);
	testchoice->AddChoice("On", 1, auto_save_sram);
	testchoice->AddChoice("Off", 0, !auto_save_sram);
	
	testchoice = options_menu.AddItem("Enable Sound", OnChangeSoundEnabled);
	testchoice->AddChoice("On", 1, sound_enabled);
	testchoice->AddChoice("Off", 0, !sound_enabled);
	
	DCMenuItem* adjust_item = options_menu.AddItem("Adjust Screen", OnAdjustScreen);
	adjust_item->user_data = (void*) 0;
	
	options_menu.Run();
}

void OnCredits(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	
}

uint8 cur_controller_detail = 0;

void OnChangeDirectionalPad(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	use_analog[cur_controller_detail] = (value ? true : false);
}

void OnConfigureControllerPlayer(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	cur_controller_detail = pMenu->GetCurrentIndex();
	DCMenu controller_detail_menu(cxt, pMenu);
	char ctemp[0x100];
	sprintf(ctemp, "Player %i", cur_controller_detail + 1);
	controller_detail_menu.SetTitle(ctemp);
	DCMenuItem* tempitem = controller_detail_menu.AddItem("Directional Pad", OnChangeDirectionalPad);
	tempitem->AddChoice("Digital", 0, !use_analog[cur_controller_detail]);
	tempitem->AddChoice("Analog", 1, use_analog[cur_controller_detail]);
	controller_detail_menu.Run();
}

void OnConfigureControllers(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	pvr_wait_ready();
	background_texture.SetCurrentIndex(controllers_texture_index);
	controller_menu.RemoveAllItems();
	controller_menu.AddItem("Player 1", OnConfigureControllerPlayer);
	controller_menu.AddItem("Player 2", OnConfigureControllerPlayer);
	controller_menu.AddItem("Player 3", OnConfigureControllerPlayer);
	controller_menu.AddItem("Player 4", OnConfigureControllerPlayer);
	controller_menu.Run();
}

void OnExit(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	
}

void OnSaveSettings(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	SaveSettings();
}

void OnSelectTheme(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	StopMusic();
	strcpy(theme_dir, pMenuItem->GetText());
	char* cur_thingy = strstr(theme_dir, " (current)");
	if (cur_thingy)
		*cur_thingy = '\0';
	background_texture.FreeAll();
	char ctemp[0x1000];
	sprintf(ctemp, "/cd/themes/%s/splash.bmp", theme_dir);
	ShowSplash(ctemp, 4000, 2000, InitTheme);
	pMenu->Stop();
	StartMusic();
}

void OnChangeTheme(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	pvr_wait_ready();
	background_texture.SetCurrentIndex(theme_texture_index);
	theme_menu.RemoveAllItems();
	file_t fpd = fs_open("/cd/themes", O_DIR | O_RDONLY);
	if (!fpd)
		return;
	dirent_t* dirinfo;
 	while ((dirinfo = fs_readdir(fpd)))
 	{
 		if (strcmp(dirinfo->name, theme_dir) == 0)
 		{
 			char ctemp[0x100];
 			sprintf(ctemp, "%s (current)", dirinfo->name);
 			theme_menu.AddItem(ctemp, OnSelectTheme);
		}
		else
			theme_menu.AddItem(dirinfo->name, OnSelectTheme);
	}
	fs_close(fpd);
	theme_menu.Run();
}

bool main_initted = false;

void ShowMainMenu()
{
	if (game_loaded && has_little)
	{
		little_zoom_finished = false;
		little_current_step = 0;
	}
	do
	{
		pvr_wait_ready();
		background_texture.SetCurrentIndex(main_texture_index);
		main_menu.RemoveAllItems();
		if (game_loaded)
			main_menu.AddItem("Resume Play", OnResume);
		main_menu.AddItem("Select Game", OnLoadGame);
		main_menu.AddItem("Options", OnOptions);
		main_menu.AddItem("Configure Controllers", OnConfigureControllers);
		main_menu.AddItem("Save Settings", OnSaveSettings);
		main_menu.AddItem("Change Theme", OnChangeTheme);
		//main_menu.AddItem("Credits", OnCredits);
		//main_menu.AddItem("Exit", OnExit);
		main_menu.Run();
	} while (!game_loaded);
	just_entered_game = true;
}

void TestVMU()
{
	DCMenu vmu_menu(cxt);
	vmu_menu.x = 50;
 	vmu_menu.y = MIN_Y;
 	vmu_menu.depth = 15;
 	vmu_menu.max_width = SCREEN_WIDTH;
 	vmu_menu.max_height = SCREEN_HEIGHT - MIN_Y;
 	
 	file_t fpd = fs_open("/vmu", O_DIR | O_RDONLY);
	if (!fpd)
		return;
	dirent_t* dirinfo;
	while ((dirinfo = fs_readdir(fpd)))
		vmu_menu.AddItem(dirinfo->name, NULL);
	vmu_menu.Run();
}

void ShowMsg(const char* str)
{
	float oldsize = cxt->size;
	cxt->size = screen_adjustments.yscale * cxt->size;
	float outleft, outup, outright, outdown;
	plx_fcxt_str_metrics(cxt, str, &outleft, &outup, &outright, &outdown);
	point_t pos;
	pos.x = ((640.0f * screen_adjustments.xscale) - outright) / 2;
	pos.y = (((480.0f * screen_adjustments.yscale) - outup) / 2) + (outup + 10);
	pos.z = 15;
	
	while (true)
	{
		pvr_wait_ready ();
		pvr_scene_begin ();
		pvr_list_begin(PVR_LIST_TR_POLY);
		DrawStringWithBorder(str, cxt, pos, 0xffffffff, PVR_PACK_COLOR(1.0f, 0, 0, 0));
		pvr_list_finish();
		pvr_scene_finish ();
		
		if (in_main_menu_system)
			snd_stream_poll();
		
		g_controllers[0].Poll();
		if (g_controllers[0].JustPressed(CONT_A) || g_controllers[0].JustPressed(CONT_B))
			break;
	}
	cxt->size = oldsize;
}

/*
	uint32 main_texture_index;
uint32 options_texture_index;
uint32 roms_texture_index;
	*/

void ScreenInit(DCMenu* pMenu, CXML* pXML)
{
	const char* sztemp = NULL;
	char ctemp[0x1000];
	if (pXML)
	{
		sztemp = pXML->GetAttribute("background");
		if (sztemp)
		{
			sprintf(ctemp, "/cd/themes/%s/%s", theme_dir, sztemp);
			if (pMenu == &main_menu)
			{
				main_texture_index = background_texture.LoadBMP(ctemp);
				main_menu.SetBackground(&background_texture);
			}
			else if (pMenu == rom_browser.GetMenu())
			{
				roms_texture_index = background_texture.LoadBMP(ctemp);
				rom_browser.GetMenu()->SetBackground(&background_texture);
			}
			else if (pMenu == &options_menu)
			{
				options_texture_index = background_texture.LoadBMP(ctemp);
				options_menu.SetBackground(&background_texture);
			}
			else if (pMenu == &controller_menu)
			{
				controllers_texture_index = background_texture.LoadBMP(ctemp);
				controller_menu.SetBackground(&background_texture);
			}
			else if (pMenu == &theme_menu)
			{
				theme_texture_index = background_texture.LoadBMP(ctemp);
				theme_menu.SetBackground(&background_texture);
			}
		}
		CXML* xtemp = pXML->Find("MENU");
		if (xtemp)
		{
			sztemp = xtemp->GetAttribute("x");
			if (sztemp)
				pMenu->x = atoi(sztemp);
			sztemp = xtemp->GetAttribute("y");
			if (sztemp)
				pMenu->y = atoi(sztemp);
			sztemp = xtemp->GetAttribute("max_width");
			if (sztemp)
				pMenu->max_width = atoi(sztemp);
			sztemp = xtemp->GetAttribute("max_height");
			if (sztemp)
				pMenu->max_height = atoi(sztemp);
			sztemp = xtemp->GetAttribute("color");
			if (sztemp)
			{
				uint32 colortemp;
				if (hexstr_to_uint32(sztemp, &colortemp))
					pMenu->SetColor(colortemp + 0xFF000000);
			}
			sztemp = xtemp->GetAttribute("selected_color");
			if (sztemp)
			{
				uint32 colortemp;
				if (hexstr_to_uint32(sztemp, &colortemp))
					pMenu->SetSelectedColor(colortemp + 0xFF000000);
			}
			sztemp = xtemp->GetAttribute("font_size");
			if (sztemp)
				pMenu->SetFontSize(atoi(sztemp));
		}
		
		xtemp = pXML->Find("TITLE");
		if (xtemp)
		{
			sztemp = xtemp->GetAttribute("x");
			if (sztemp)
				pMenu->title_x = atoi(sztemp);
			sztemp = xtemp->GetAttribute("y");
			if (sztemp)
				pMenu->title_y = atoi(sztemp);
			sztemp = xtemp->GetAttribute("slant");
			if (sztemp)
				pMenu->title_slant = atoi(sztemp);
			sztemp = xtemp->GetAttribute("size");
			if (sztemp)
				pMenu->title_size = atoi(sztemp);
			sztemp = xtemp->GetAttribute("color");
			if (sztemp)
			{
				uint32 colortemp;
				if (hexstr_to_uint32(sztemp, &colortemp))
					pMenu->title_color = colortemp + 0xFF000000;
			}
			sztemp = xtemp->GetAttribute("border");
			if (sztemp)
			{
				uint32 colortemp;
				if (hexstr_to_uint32(sztemp, &colortemp))
					pMenu->title_border_color = colortemp + 0xFF000000;
			}
			sztemp = xtemp->GetAttribute("text");
			if (sztemp)
				pMenu->SetTitle(sztemp);
		}
	}
}

void InitTheme()
{
	char ctemp[0x1000];
	sprintf(ctemp, "/cd/themes/%s/theme.xml", theme_dir);
	
	char* szDoc = read_text_file(ctemp);
	if (!szDoc)
	{
		ShowMsg("theme.xml not found");
		return;
	}
	
	CXML* root = CXML::Parse(szDoc);
	delete[] szDoc;
	if (!root)
	{
		ShowMsg("Error reading theme XML file");
		return;
	}
	
	const char* sztemp = NULL;
	
	// MAIN SCREEN
	main_menu.SetFontContext(cxt);
	main_menu.poll_sound_stream = true;
	main_menu.SetDrawExtraCallback(MainMenuDrawExtra);
	main_menu.SetTitle(NULL);
	main_menu.SetSelectedColor(PVR_PACK_COLOR(1.0f, 1.0f, COLOR_255_TO_FLOAT(0xA1), COLOR_255_TO_FLOAT(0x47)));
	main_menu.x = 20;
 	main_menu.y = MIN_Y + 20;
 	main_menu.depth = 15;
 	main_menu.max_width = SCREEN_WIDTH;
 	main_menu.max_height = SCREEN_HEIGHT - MIN_Y;
 	main_menu.title_slant = 0;
 	
	CXML* main_xml = root->Find("MAIN_SCREEN");
	if (main_xml)
	{
		ScreenInit(&main_menu, main_xml);
		CXML* mini_xml = main_xml->Find("PAUSED_MINI");
		if (mini_xml)
		{
			int16 little_width = -1;
			int16 little_height = -1;
			if ((sztemp = mini_xml->GetAttribute("x")))
				little_x = atoi(sztemp);
			if ((sztemp = mini_xml->GetAttribute("y")))
				little_y = atoi(sztemp);
			if ((sztemp = mini_xml->GetAttribute("width")))
				little_width = atoi(sztemp);
			if ((sztemp = mini_xml->GetAttribute("height")))
				little_height = atoi(sztemp);
			if (little_x >= 0 && little_y >= 0 && little_width > 0 && little_height > 0)
			{
				little_x2 = little_x + little_width;
				little_y2 = little_y + little_height;
				has_little = true;
			}
		}
	}
	
	if (has_little)
	{
		adj_little_x = screen_adjustments.x1 + (little_x * screen_adjustments.xscale);
		adj_little_y = screen_adjustments.y1 + (little_y * screen_adjustments.xscale);
		adj_little_x2 = screen_adjustments.x1 + (little_x2 * screen_adjustments.xscale);
		adj_little_y2 = screen_adjustments.y1 + (little_y2 * screen_adjustments.xscale);
	}
	
	// ROM SCREEN
	DCMenu* browser_menu = rom_browser.GetMenu();
	browser_menu->poll_sound_stream = true;
	browser_menu->SetTitle(NULL);
	browser_menu->SetDrawExtraCallback(RomMenuDrawExtra);
	browser_menu->SetFontContext(cxt);
	browser_menu->SetFontSize(36.0f);
	browser_menu->title_slant = 0;
	browser_menu->x = 20;
	browser_menu->y = 36;
	browser_menu->depth = 15;
	browser_menu->max_width = 620;
	browser_menu->max_width = 464;
	
	CXML* rom_xml = root->Find("ROM_SCREEN");
	if (rom_xml)
	{
		ScreenInit(browser_menu, rom_xml);
		sztemp = rom_xml->GetAttribute("folder_icon");
		if (sztemp)
		{
			sprintf(ctemp, "/cd/themes/%s/%s", theme_dir, sztemp);
			if (folder_icon)
				delete folder_icon;
			folder_icon = new CPVRTexture();
			folder_icon->LoadBMP(ctemp);
			rom_browser.SetFolderIcon(folder_icon);
		}
		sztemp = rom_xml->GetAttribute("rom_icon");
		if (sztemp)
		{
			sprintf(ctemp, "/cd/themes/%s/%s", theme_dir, sztemp);
			if (rom_icon)
				delete rom_icon;
			rom_icon = new CPVRTexture();
			rom_icon->LoadBMP(ctemp);
			rom_browser.SetDefaultIcon(rom_icon);
		}
		sztemp = rom_xml->GetAttribute("folder_color");
		if (sztemp)
		{
			uint32 colortemp;
			if (hexstr_to_uint32(sztemp, &colortemp))
				rom_browser.directory_color = colortemp + 0xFF000000;
		}
	}
	
	// OPTION SCREEN
	options_menu.SetFontContext(cxt);
	options_menu.poll_sound_stream = true;
	options_menu.SetTitle(NULL);
	options_menu.SetDrawExtraCallback(OptionsMenuDrawExtra);
	options_menu.SetSelectedColor(PVR_PACK_COLOR(1.0f, 1.0f, COLOR_255_TO_FLOAT(0xA1), COLOR_255_TO_FLOAT(0x47)));
	options_menu.title_slant = 0;
	options_menu.x = 20;
 	options_menu.y = MIN_Y + 20;
 	options_menu.depth = 15;
 	options_menu.max_width = SCREEN_WIDTH;
 	options_menu.max_height = SCREEN_HEIGHT - MIN_Y;
 	options_menu.double_height = true;
 	
	CXML* options_xml = root->Find("OPTIONS_SCREEN");
	if (options_xml)
		ScreenInit(&options_menu, options_xml);
	
	// CONTROLLERS SCREEN
	controller_menu.SetFontContext(cxt);
	controller_menu.poll_sound_stream = true;
	controller_menu.SetTitle(NULL);
	controller_menu.SetDrawExtraCallback(ControllerMenuDrawExtra);
	controller_menu.SetSelectedColor(PVR_PACK_COLOR(1.0f, 1.0f, COLOR_255_TO_FLOAT(0xA1), COLOR_255_TO_FLOAT(0x47)));
	controller_menu.title_slant = 0;
	controller_menu.x = 20;
 	controller_menu.y = MIN_Y + 20;
 	controller_menu.depth = 15;
 	controller_menu.max_width = SCREEN_WIDTH;
 	controller_menu.max_height = SCREEN_HEIGHT - MIN_Y;
 	
	CXML* controller_xml = root->Find("CONTROLLER_SCREEN");
	if (controller_xml)
		ScreenInit(&controller_menu, controller_xml);
	
	// THEME SCREEN
	theme_menu.SetFontContext(cxt);
	theme_menu.poll_sound_stream = true;
	theme_menu.SetTitle(NULL);
	theme_menu.SetDrawExtraCallback(ThemeMenuDrawExtra);
	theme_menu.SetSelectedColor(PVR_PACK_COLOR(1.0f, 1.0f, COLOR_255_TO_FLOAT(0xA1), COLOR_255_TO_FLOAT(0x47)));
	theme_menu.title_slant = 0;
	theme_menu.x = 20;
 	theme_menu.y = MIN_Y + 20;
 	theme_menu.depth = 15;
 	theme_menu.max_width = SCREEN_WIDTH;
 	theme_menu.max_height = SCREEN_HEIGHT - MIN_Y;
 	
	CXML* theme_xml = root->Find("THEME_SCREEN");
	if (theme_xml)
		ScreenInit(&theme_menu, theme_xml);
	
	CXML* load_rom_xml = root->Find("LOAD_ROM_IMAGE");
	if (load_rom_xml && (sztemp = load_rom_xml->GetAttribute("file")))
	{
		sprintf(ctemp, "/cd/themes/%s/%s", theme_dir, sztemp);
 		load_rom_texture_index = background_texture.LoadBMP(ctemp);
	}
	
	delete root;
}

void CheckSingleGame()
{
	if (g_controllers[0].GetJoyX() < -ANALOG_THRESHOLD && g_controllers[0].IsPressed(CONT_START))
		return;
	file_t fpd = fs_open("/cd", O_DIR | O_RDONLY);
	if (!fpd)
		return;
	checking_single = true;
	dirent_t* dirinfo;
	while ((dirinfo = fs_readdir(fpd)))
	{
		if (stricmp(dirinfo->name, "game.zip") == 0 ||
			stricmp(dirinfo->name, "game.sfc") == 0 ||
			stricmp(dirinfo->name, "game.smc") == 0 ||
			stricmp(dirinfo->name, "game.swc") == 0 ||
			stricmp(dirinfo->name, "game.fig") == 0)
		{
			sprintf(rom_filename, "/cd/%s", dirinfo->name);
			if (Memory.LoadROM(rom_filename))
			{
				game_loaded = true;
   				char vmu_path[0x100];
				if (FindFirstVMU(vmu_path))
				{
					char ctemp[0x100];
					sprintf(ctemp, "%s/%X.srm", vmu_path, Memory.ROMCRC32);
					Memory.LoadSRAM(ctemp);
				}
				single_game_mode = true;
			}
			else
			{
				char ctemp[0x100];
				sprintf(ctemp, "Load of single game failed: %s", dirinfo->name);
				ShowMsg(ctemp);
			}
			break;
		}
	}
	fs_close(fpd);
	checking_single = false;
}

extern "C" int main()
{
	strcpy(theme_dir, "default");
	vid_init(DM_640x480, PM_RGB565);
	pvr_init(&pvr_params);
	pvr_mem_stats();
	texture_init();
	pvr_mem_stats();
	dc_maple_init();
	for (int k = 0; k < 4; ++k)
    	g_controllers[k].SetDevice(dc_maple_controller_info[k].dev);
	
	fnt = plx_font_load("/rd/fonts/handelgothic.txf");
	printf("fnt->txr->w = %u, fnt->txr->h = %u\n", fnt->txr->w, fnt->txr->h);
	cxt = plx_fcxt_create(fnt, PVR_LIST_TR_POLY);
	cxt->size = cxt->size * 2.0f;
	rom_filename[0] = '\0';
	ZeroMemory (&Settings, sizeof (Settings));
	Settings.SkipFrames = AUTO_FRAMERATE;
    Settings.DisplayFrameRate = FALSE;
	LoadSettings();
	if (sound_enabled)
	{
		Settings.APUEnabled = Settings.NextAPUEnabled = TRUE;
		Settings.SoundSync = TRUE;
	}
	else
	{
		Settings.APUEnabled = Settings.NextAPUEnabled = FALSE;
		Settings.SoundSync = FALSE;
	}
	Settings.SoundPlaybackRate = 4;
    Settings.Stereo = TRUE;
    Settings.SoundBufferSize = 0;
    //Settings.InterpolatedSound = FALSE;
	Settings.JoystickEnabled = TRUE;
    Settings.CyclesPercentage = 100;
    Settings.DisableSoundEcho = FALSE;
    Settings.H_Max = SNES_CYCLES_PER_SCANLINE;
    Settings.ShutdownMaster = TRUE;
    Settings.FrameTimePAL = 20000;
    Settings.FrameTimeNTSC = 16667;
    Settings.FrameTime = Settings.FrameTimeNTSC;
    Settings.DisableSampleCaching = FALSE;
    Settings.DisableMasterVolume = FALSE;
    Settings.Mouse = TRUE;
    Settings.SuperScope = TRUE;
    Settings.MultiPlayer5 = TRUE;
    Settings.ControllerOption = SNES_JOYPAD;
    Settings.SixteenBit = TRUE;
    Settings.SixteenBitSound = TRUE;
    Settings.SupportHiRes = FALSE;
    Settings.NetPlay = FALSE;
    Settings.ServerName [0] = 0;
    Settings.ThreadSound = FALSE;
    Settings.AutoSaveDelay = 30;
    Settings.ApplyCheats = FALSE;
    Settings.TurboMode = FALSE;
    Settings.TurboSkipFrames = 15;
    Settings.StretchScreenshots = 1;
    Settings.ForceTransparency = TRUE;
    Settings.Transparency = TRUE;
    Settings.HBlankStart = (256 * Settings.H_Max) / SNES_HCOUNTER_MAX;
    if (!Memory.Init () || !S9xInitAPU())
    	OutOfMemory ();
   	S9xInitSound (Settings.SoundPlaybackRate, Settings.Stereo, Settings.SoundBufferSize);
   	if (!Settings.APUEnabled)
   		S9xSetSoundMute(TRUE);
	else
		S9xSetSoundMute(FALSE);
   	S9xSetRenderPixelFormat (RGB565);
   	S9xInitInputDevices ();
   	S9xInitDisplay ();
   	if (!S9xGraphicsInit ())
	   	OutOfMemory ();
	CheckSingleGame();
	if (!single_game_mode)
	{
		char ctemp[0x1000];
		sprintf(ctemp, "/cd/themes/%s/splash.bmp", theme_dir);
		ShowSplash(ctemp, 4000, 2000, InitTheme);
   		Settings.Paused = TRUE;
	}
   	bool first_time_through = true;
   	while (1)
    {
    	if (!single_game_mode && Settings.Paused)
    	{
  			if (sound_enabled && !first_time_through)
  				snd_stream_shutdown();
			first_time_through = false;
			in_main_menu_system = true;
			StartMusic();
			ShowMainMenu();
			StopMusic();
			in_main_menu_system = false;
			Settings.Paused = FALSE;
			if (sound_enabled)
			{
				memset(sound_buf, 0, OGG_BUF_SIZE);
				first_sfcastGetSound = true;
				snd_stream_init(sfcastGetSound);
				snd_stream_start(SUPER_FAMICAST_FREQ, 1);
			}
		}
    	S9xMainLoop ();
    	if (Settings.JoystickEnabled)
	    	ReadJoysticks();
    }
    	
	return 0;
}

