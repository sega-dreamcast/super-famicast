#include <kos.h>
#include <stdio.h>
#include "dc_utils.h"
#include "dc_vmu.h"
#include "dc_menu.h"
#include <math.h>
#include "XArray.h"

/* ============================================================ */
/* sq */

#include <dc/sq.h>

/* ============================================================ */
/* controller handling */
dc_maple_info_t dc_maple_controller_info[DC_MAPLE_INFO_SIZE];
dc_maple_info_t dc_maple_mouse_info[DC_MAPLE_INFO_SIZE];
dc_maple_info_t dc_maple_keyboard_info[DC_MAPLE_INFO_SIZE];

extern struct
{
	int32 x1, y1, x2, y2;
	float xscale, yscale;
} screen_adjustments;

void dc_maple_init ()
{
	int n;
	maple_device_t *dev;
	dc_maple_info_t *p;

	struct _typemap
	{
		char *name;
		dc_maple_controller_type_t type;
	} typemap[] = {
		{"Arcade Stick", DC_MAPLE_CONTROLLER_ARCADESTICK},
		{NULL, DC_MAPLE_CONTROLLER_NORMAL},
	};
	struct _typemap *tp;

	// controller
	n = 0;
	p = dc_maple_controller_info;
	for (;;)
	{
		dev = maple_enum_type (n, MAPLE_FUNC_CONTROLLER);
		p->dev = dev;
		p->type.c_type = DC_MAPLE_CONTROLLER_NORMAL;

		tp = typemap;
		while (tp->name)
		{
			if (!strncmp(tp->name, dev->info.product_name, strlen(tp->name)))
			{
				p->type.c_type = tp->type;
				break;
			}

			++tp;
		}

		++n;
		++p;

		if (!dev)
			break;
	}

	// mouse
	n = 0;
	p = dc_maple_mouse_info;
	for (;;)
	{
		dev = maple_enum_type (n, MAPLE_FUNC_MOUSE);
		p->dev = dev;
		p->type.m_type = DC_MAPLE_MOUSE_3BUTTON;

		++n;
		++p;

		if (!dev)
			break;
	}

	// keyboard
	n = 0;
	p = dc_maple_keyboard_info;
	for (;;)
	{
		dev = maple_enum_type (n, MAPLE_FUNC_KEYBOARD);
		p->dev = dev;
		p->type.k_type = DC_MAPLE_KEYBOARD_JP106;

		++n;
		++p;

		if (!dev)
			break;
	}
}


/* ------------------------------------------------------------ */
/* screen rect */

dc_screen_offset_t dc_screen_offset = {0, 16};

void
dc_screen_offset_init ()
{
  dc_screen_offset.x = 0;
  dc_screen_offset.y = 16;
}

/* ------------------------------------------------------------ */

int load_bmp (uint16** pRaw, const char *filename, uint32* width, uint32* height, uint32 fmt)
{
	FILE *fp = NULL;
	uint8* bmp = NULL;
	uint8 *p;
	uint16* raw = NULL;
	uint32 i;
	uint32 area;
	uint32 bmp_size;

	fp = fopen(filename, "rb");
	if (!fp)
		goto error;
	
	fseek(fp, 18, SEEK_SET);
	fread(width, sizeof(uint32), 1, fp);
	fread(height, sizeof(uint32), 1, fp);
	
	area = (*width) * (*height);
	bmp_size = area * 3;
	bmp = new uint8[bmp_size];
	//raw = *pRaw = new uint16[area];
	raw = *pRaw = (uint16*) memalign(32, bmp_size);

	fseek(fp, 54, SEEK_SET);
	if (fread(bmp, bmp_size, 1, fp) != 1) 
		goto error;

	fclose(fp);

	p = bmp;
	uint16 r, g, b;
	
	for (i = 0; i < area; ++i)
	{
		b = *p++ * 32 / 256;
		g = *p++ * 64 / 256;
		r = *p++ * 32 / 256;

		*raw++ = (r << 11) | (g << 5) | (b << 0);
	}
	
	if (bmp)
		delete[] bmp;

	return 0;

error:
	if (fp)
 		fclose(fp);
	if (bmp)
		delete[] bmp;
	if (raw)
		free(raw);
	*pRaw = NULL;
	return -1;
}

void
dc_pvr_init ()
{ 
  dc_screen_offset_init ();
}

void DrawStringWithBorder(const char* str, plx_fcxt_t* pCxt, point_t point, uint32 color, uint32 border_color)
{
	float old_font_size = pCxt->size;
	
	pCxt->size = pCxt->size * screen_adjustments.yscale;
	point.x = screen_adjustments.x1 + (point.x * screen_adjustments.xscale);
	point.y = screen_adjustments.y1 + (point.y * screen_adjustments.yscale);

	pCxt->color = color;
	plx_fcxt_begin(pCxt);
	plx_fcxt_setpos_pnt(pCxt, &point);
	plx_fcxt_draw(pCxt, str);
	plx_fcxt_end(pCxt);
	
	--point.z;
	--point.x;
	--point.y;
	pCxt->color = border_color;
	plx_fcxt_begin(pCxt);
	plx_fcxt_setpos_pnt(pCxt, &point);
	plx_fcxt_draw(pCxt, str);
	plx_fcxt_end(pCxt);
	
	point.x += 2;
	plx_fcxt_begin(pCxt);
	plx_fcxt_setpos_pnt(pCxt, &point);
	plx_fcxt_draw(pCxt, str);
	plx_fcxt_end(pCxt);
	
	point.y += 2;
	plx_fcxt_begin(pCxt);
	plx_fcxt_setpos_pnt(pCxt, &point);
	plx_fcxt_draw(pCxt, str);
	plx_fcxt_end(pCxt);
	
	point.x -= 2;
	plx_fcxt_begin(pCxt);
	plx_fcxt_setpos_pnt(pCxt, &point);
	plx_fcxt_draw(pCxt, str);
	plx_fcxt_end(pCxt);
	
	pCxt->size = old_font_size;
}

bool _browse_success = false;
char* _browse_dir = NULL;
plx_fcxt_t* _browse_font = NULL;
int16 _browse_x = 0;
int16 _browse_y = 0;
int16 _browse_w = 0;
int16 _browse_h = 0;
int16 _browse_depth = 0;

void _OnChooseFile(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	sprintf(_browse_dir, "%s/%s", pMenu->user_data, pMenuItem->GetText());
	_browse_success = true;
	pMenu->Stop(true);
}

void _OnNavDir(DCMenu* pMenu, DCMenuItem* pMenuItem, int value)
{
	char my_dir[0x100];
	DCMenu browse_menu(_browse_font, pMenu);
	browse_menu.x = _browse_x;
 	browse_menu.y = _browse_y;
 	browse_menu.depth = 15;
 	browse_menu.max_width = _browse_w;
 	browse_menu.max_height = _browse_h;
	if (!pMenu)
		strcpy(my_dir, _browse_dir);
	else
		sprintf(my_dir, "%s/%s", (char*) pMenu->user_data, pMenuItem->GetText());
	browse_menu.user_data = my_dir;
	file_t fpd = fs_open(my_dir, O_DIR | O_RDONLY);
	if (!fpd)
		return;
	dirent_t* dirinfo;
	while ((dirinfo = fs_readdir(fpd)))
	{
		if (dirinfo->size == -1)
			browse_menu.AddItem(dirinfo->name, _OnNavDir);
		else
			browse_menu.AddItem(dirinfo->name, _OnChooseFile);
	}
	fs_close(fpd);
	browse_menu.Run();
}

// allocate memory for szResult before calling. A size of 256 should be enough.
bool BrowseForFile(const char* szStartingDir, char* szResult, plx_fcxt_t* font, int16 x, int16 y, int16 w, int16 h, int16 depth, cont_state_t* prevstate)
{
	strcpy(szResult, szStartingDir);
	_browse_dir = szResult;
	_browse_font = font;
	_browse_x = x;
	_browse_y = y;
	_browse_w = w;
	_browse_h = h;
	_browse_depth = depth;
	_browse_success = false;
	_OnNavDir(NULL, (DCMenuItem*) prevstate, 0);
	return _browse_success;
}

void dc_sq_cpy(void *dest, void *src, int n)
{
  uint32 *sq;
  uint32 *d, *s;
  
  d = (uint32 *)(0xe0000000 | (((uint32)dest) & 0x03ffffe0));
  s = (uint32 *)(src);
  
  /* Set store queue memory area as desired */
  QACR0 = ((((uint32)dest)>>26)<<2)&0x1c;
  QACR1 = ((((uint32)dest)>>26)<<2)&0x1c;
  
  n >>= 6;
  while (n--) 
  {
    /* sq0 */ 
    sq = d;
    *sq++ = *s++; *sq++ = *s++;
    *sq++ = *s++; *sq++ = *s++;
    *sq++ = *s++; *sq++ = *s++;
    *sq++ = *s++; *sq++ = *s++;
    asm("pref @%0" : : "r" (d));
    d += 8;
    
    /* sq1 */
    sq = d;
    *sq++ = *s++; *sq++ = *s++;
    *sq++ = *s++; *sq++ = *s++;
    *sq++ = *s++; *sq++ = *s++;
    *sq++ = *s++; *sq++ = *s++;
    asm("pref @%0" : : "r" (d));
    d += 8;
  }
}

void dc_wait_sq_cpy_done ()
{
  /* wait for both store queues to complete */
  *((uint32 *)(0xe0000000)) = 0;
  *((uint32 *)(0xe0000020)) = 0;
}

bool scherzo_bmp_load_texture(const char* filename, pvr_ptr_t* tex, uint32 *w, uint32 *h, uint32 *tex_width, uint32 *tex_height)
{
	uint16* raw = NULL;
	load_bmp(&raw, filename, w, h);
	if (!raw)
		return false;
	*tex_width = *w;
    *tex_height = *h;
    uint32 w_count = 0;
    uint32 h_count = 0;
    uint32 last_val;
    while ((*tex_width = (last_val = *tex_width) / 2) != 0)
    	++w_count;
	if ((last_val % 2) != 0)
		++w_count;
	while ((*tex_height = (last_val = *tex_height) / 2) != 0)
    	++h_count;
	if ((last_val % 2) != 0)
		++h_count;
	*tex_width = (int32) powf(2, w_count);
	*tex_height = (int32) powf(2, h_count);
	printf("w = %i, h = %i\n", *w, *h);
	printf("tex_width = %i, tex_height = %i\n", *tex_width, *tex_height);
	uint16 tex_byte_size = sizeof(uint16) * (*tex_width) * (*tex_height);
	*tex = pvr_mem_malloc(tex_byte_size);
	
	uint16* temp_dest = (uint16*) *tex;
	for (uint16 y = 0; y < *h; ++y)
		sq_cpy(&(temp_dest[y * (*tex_width)]), &(raw[((*h - y) - 1) * (*w)]), sizeof(uint16) * (*w));
	
	free(raw);
	return true;
}

char* read_text_file(const char* filename)
{
	FILE* fp = fopen(filename, "rb");
	if (!fp)
		return NULL;
	fseek(fp, 0, SEEK_END);
	uint32 filelen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* sztemp = new char[filelen + 1];
	fread(sztemp, sizeof(char), filelen, fp);
	sztemp[filelen] = NULL;
	fclose(fp);
	return sztemp;
}

bool hexstr_to_uint32(const char* str, uint32* retval)
{
	uint32 num = 0;
	int32 len = strlen(str);
	if (len == 0)
		return false;
	uint32 digit = 0;
	uint32 temppow;
	for (int32 k = len - 1; k >= 0; --k)
	{
		temppow = pow(16, digit);
		switch (str[k])
		{
		case '0':
			break;
		case '1':
			num += temppow;
			break;
		case '2':
			num += temppow * 2;
			break;
		case '3':
			num += temppow * 3;
			break;
		case '4':
			num += temppow * 4;
			break;
		case '5':
			num += temppow * 5;
			break;
		case '6':
			num += temppow * 6;
			break;
		case '7':
			num += temppow * 7;
			break;
		case '8':
			num += temppow * 8;
			break;
		case '9':
			num += temppow * 9;
			break;
		case 'a':
		case 'A':
			num += temppow * 10;
			break;
		case 'b':
		case 'B':
			num += temppow * 11;
			break;
		case 'c':
		case 'C':
			num += temppow * 12;
			break;
		case 'd':
		case 'D':
			num += temppow * 13;
			break;
		case 'e':
		case 'E':
			num += temppow * 14;
			break;
		case 'f':
		case 'F':
			num += temppow * 15;
			break;
		default:
		{
			printf("hexstr_to_uint32() bad char: %c\n", str[k]);
			return false;
		}
		};
		++digit;
	}
	*retval = num;
	return true;
}

