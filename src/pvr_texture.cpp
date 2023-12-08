#include "pvr_texture.h"
#include "dc_utils.h"
#include <math.h>

extern struct
{
	int32 x1, y1, x2, y2;
	float xscale, yscale;
} screen_adjustments;

CPVRTexture::CPVRTexture()
{
	m_ptr = NULL;
	//m_hdr;
	m_width = 0;
	m_height = 0;
 	m_tex_width = 0;
	m_tex_height = 0;
	m_x_scale = 1.0f;
	m_y_scale = 1.0f;
	load_count = 0;
	m_current_index = -1;
}

CPVRTexture::~CPVRTexture()
{
	for (uint32 k = 0; k < m_raws.size(); ++k)
	{
		if (m_raws[k])
			free(m_raws[k]);
		if (m_raw_filenames[k])
			delete[] m_raw_filenames[k];
	}
	if (m_ptr)
		pvr_mem_free(m_ptr);
}

int CPVRTexture::LoadBMP(const char* filename, pvr_list_t list, int filtering)
{
	for (uint32 k = 0; k < m_raw_filenames.size(); ++k)
	{
		if (m_raw_filenames[k] && stricmp(m_raw_filenames[k], filename) == 0)
			return k;
	}
	uint16* raw = NULL;
	load_bmp(&raw, filename, &m_width, &m_height);
	if (!raw)
		return -1;
	m_tex_width = m_width;
    m_tex_height = m_height;
    uint32 w_count = 0;
    uint32 h_count = 0;
    uint32 last_val;
    while ((m_tex_width = (last_val = m_tex_width) / 2) != 0)
    	++w_count;
	if ((last_val % 2) != 0)
		++w_count;
	while ((m_tex_height = (last_val = m_tex_height) / 2) != 0)
    	++h_count;
	if ((last_val % 2) != 0)
		++h_count;
	m_tex_width = (int32) powf(2, w_count);
	m_tex_height = (int32) powf(2, h_count);
	uint16 tex_byte_size = sizeof(uint16) * m_tex_width * m_tex_height;
	if (!m_ptr)
	{
		pvr_poly_cxt_t poly;
		m_ptr = pvr_mem_malloc(tex_byte_size);
		pvr_mem_stats();
		pvr_poly_cxt_txr(&poly, list, PVR_TXRFMT_RGB565 | PVR_TXRFMT_NONTWIDDLED, m_tex_width,
					 m_tex_height, m_ptr, filtering);
    	pvr_poly_compile(&m_hdr, &poly);
	}
	
	m_raws.push(raw);
	char* fntemp = new char[strlen(filename) + 1];
	strcpy(fntemp, filename);
	m_raw_filenames.push(fntemp);
	
	if (m_current_index == -1)
		SetCurrentIndex(0);
	
	return m_raws.size() - 1;
}

void CPVRTexture::FreeBMP(uint32 index)
{
	delete[] m_raw_filenames[index];
	m_raw_filenames[index] = NULL;
	free(m_raws[index]);
	m_raws[index] = NULL;
}

void CPVRTexture::FreeAll()
{
	for (uint32 k = 0; k < m_raw_filenames.size(); ++k)
	{
		delete[] m_raw_filenames[k];
		m_raw_filenames[k] = NULL;
		free(m_raws[k]);
		m_raws[k] = NULL;
	}
	m_raw_filenames.removeAll();
	m_raws.removeAll();
}

void CPVRTexture::SetCurrentIndex(uint32 index)
{
	if (m_current_index == index)
		return;
	m_current_index = index;
	uint16* raw = m_raws[index];
	uint16* temp_dest = (uint16*) m_ptr;
	if (m_width == m_tex_width)
		sq_cpy(temp_dest, raw, sizeof(uint16) * m_width * m_height);
	else
	{
		for (uint16 y = 0; y < m_height; ++y)
			sq_cpy(&(temp_dest[y * m_tex_width]), &(raw[((m_height - y) - 1) * m_width]), sizeof(uint16) * m_width);
	}
}

void CPVRTexture::Draw(int16 x, int16 y, int16 z, float alpha)
{
	pvr_vertex_t vert;

	pvr_prim (&m_hdr, sizeof(pvr_poly_hdr_t));
	
	float u2 = ((float) m_width) / ((float) m_tex_width);
	float v2 = ((float) m_height) / ((float) m_tex_height);
	
	float x2 = ((float) x) + (((float) m_width) * m_x_scale);
	float y2 = ((float) y) + (((float) m_height) * m_y_scale);
	
	float x1 = screen_adjustments.x1 + (((float) x) * screen_adjustments.xscale);
	float y1 = screen_adjustments.y1 + (((float) y) * screen_adjustments.yscale);
	x2 = screen_adjustments.x1 + (x2 * screen_adjustments.xscale);
	y2 = screen_adjustments.y1 + (y2 * screen_adjustments.yscale);

	vert.flags = PVR_CMD_VERTEX;
	vert.x = x1;
	vert.y = y2;
	vert.z = z;
	vert.u = 0;
	vert.v = v2;
	vert.argb = PVR_PACK_COLOR(alpha, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	pvr_prim (&vert, sizeof(vert));

	vert.x = x1;
	vert.y = y1;
	vert.u = 0;
	vert.v = 0;
	pvr_prim (&vert, sizeof(vert));

	vert.x = x2;
	vert.y = y2;
	vert.u = u2;
	vert.v = v2;
	pvr_prim (&vert, sizeof(vert));

	vert.flags = PVR_CMD_VERTEX_EOL;
	vert.x = x2;
	vert.y = y1;
	vert.u = u2;
	vert.v = 0;
	pvr_prim (&vert, sizeof(vert));
}

void CPVRTexture::DrawFullscreen(int16 z, float alpha = 1.0f)
{
	pvr_vertex_t vert;

	pvr_prim (&m_hdr, sizeof(pvr_poly_hdr_t));
	
	float u2 = ((float) m_width) / ((float) m_tex_width);
	float v2 = ((float) m_height) / ((float) m_tex_height);

	vert.flags = PVR_CMD_VERTEX;
	vert.x = screen_adjustments.x1;
	vert.y = screen_adjustments.y2;
	vert.z = z;
	vert.u = 0;
	vert.v = v2;
	vert.argb = PVR_PACK_COLOR(alpha, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	pvr_prim (&vert, sizeof(vert));

	vert.x = screen_adjustments.x1;
	vert.y = screen_adjustments.y1;
	vert.u = 0;
	vert.v = 0;
	pvr_prim (&vert, sizeof(vert));

	vert.x = screen_adjustments.x2;
	vert.y = screen_adjustments.y2;
	vert.u = u2;
	vert.v = v2;
	pvr_prim (&vert, sizeof(vert));

	vert.flags = PVR_CMD_VERTEX_EOL;
	vert.x = screen_adjustments.x2;
	vert.y = screen_adjustments.y1;
	vert.u = u2;
	vert.v = 0;
	pvr_prim (&vert, sizeof(vert));
}

void CPVRTexture::SetScale(float x_scale, float y_scale)
{
	m_x_scale = x_scale;
	m_y_scale = y_scale;
}
