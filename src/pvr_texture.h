#ifndef _PVR_TEXTURE_H_
#define _PVR_TEXTURE_H_

#include <kos.h>
#include "XArray.h"

class CPVRTexture
{
public:
	CPVRTexture();
	virtual ~CPVRTexture();
	
	int LoadBMP(const char* filename, pvr_list_t list = PVR_LIST_TR_POLY, int filtering = PVR_FILTER_BILINEAR);
	void FreeBMP(uint32 index);
	void FreeAll();
	void SetCurrentIndex(uint32 index);
	int32 GetCurrentIndex() { return m_current_index; }
	void Draw(int16 x, int16 y, int16 z, float alpha = 1.0f);
	void DrawFullscreen(int16 z, float alpha = 1.0f);
	void SetScale(float x_scale, float y_scale);
	uint32 GetWidth() { return m_width; }
	uint32 GetHeight() { return m_height; }
	
	unsigned int load_count;
	
protected:
	int32 m_current_index;
	XArray<uint16*> m_raws;
	XArray<char*> m_raw_filenames;
	float m_x_scale;
	float m_y_scale;
	pvr_ptr_t m_ptr;
	pvr_poly_hdr_t m_hdr;
	uint32 m_width;
	uint32 m_height;
 	uint32 m_tex_width;
	uint32 m_tex_height;
};

#endif

