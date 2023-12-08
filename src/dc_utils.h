#ifndef _DC_UTILS_H_
#define _DC_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MIN_Y	16

#if 0
}
#endif

//#include "types.h"
#include <kos.h>
#include <plx/font.h>

extern const char *progname;

/* ------------------------------------------------------------ */
/* controller handling */

/* low level interface */
typedef enum {
  DC_MAPLE_CONTROLLER_NORMAL,
  DC_MAPLE_CONTROLLER_ARCADESTICK, 
} dc_maple_controller_type_t;

typedef enum {
  DC_MAPLE_MOUSE_3BUTTON,
} dc_maple_mouse_type_t;

typedef enum {
  DC_MAPLE_KEYBOARD_JP106,
} dc_maple_keyboard_type_t;

typedef struct {
  maple_device_t *dev;
  
  union {
    dc_maple_controller_type_t c_type;
    dc_maple_mouse_type_t m_type;
    dc_maple_keyboard_type_t k_type;
  } type;
} dc_maple_info_t;

#define DC_MAPLE_INFO_SIZE 16
extern dc_maple_info_t dc_maple_controller_info[DC_MAPLE_INFO_SIZE];
extern dc_maple_info_t dc_maple_mouse_info[DC_MAPLE_INFO_SIZE];
extern dc_maple_info_t dc_maple_keyboard_info[DC_MAPLE_INFO_SIZE];
void dc_maple_init ();

typedef struct {
  float x, y;
} dc_screen_offset_t;

extern dc_screen_offset_t dc_screen_offset;

extern pvr_vertex_t main_vert;

void dc_screen_offset_init();

void dc_pvr_init ();

int load_bmp(uint16** raw, const char *filename, uint32* width, uint32* height, uint32 fmt = PVR_TXRFMT_RGB565);
void DrawStringWithBorder(const char* str, plx_fcxt_t* pCxt, point_t point, uint32 color, uint32 border_color);
bool BrowseForFile(const char* szStartingDir, char* szResult, plx_fcxt_t* font, int16 x, int16 y, int16 w, int16 h, int16 depth, cont_state_t* prevstate = NULL);
bool scherzo_bmp_load_texture(const char* filename, pvr_ptr_t* tex, uint32 *w, uint32 *h, uint32 *tex_width, uint32 *tex_height);
char* read_text_file(const char* filename);
bool hexstr_to_uint32(const char* str, uint32* retval);
void dc_sq_cpy(void *dest, void *src, int n);
void dc_wait_sq_cpy_done();

#ifdef __cplusplus
}
#endif

#endif

