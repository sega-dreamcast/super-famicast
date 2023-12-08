#ifndef _DC_MENU_H_
#define _DC_MENU_H_

#include <kos.h>
#include <plx/font.h>
#include "XArray.h"
#include "dc_utils.h"
#include "pvr_texture.h"

class DCMenuItem;
class DCMenu;

typedef void (*DC_MENU_CALLBACK)(DCMenu* pMenu, DCMenuItem* pMenuItem, int value);
typedef void (*DC_MENU_DRAW_EXTRA_CALLBACK)(DCMenu* pMenu);

#define CHOICE_STYLE_NEXT_TO	1
#define CHOICE_STYLE_BELOW		2

class DCMenuItem
{
public:
	DCMenuItem();
	~DCMenuItem();
	
	void SetText(const char* str);
	const char* GetText() { return m_text; }
	void SetSelectCallback(DC_MENU_CALLBACK func);
	void AddChoice(const char* text, int value, bool selected = false);
	void SetIcon(CPVRTexture* icon_tex);
	
	friend class DCMenu;
	
	uint8 choice_style;
	int m_current_choice;
	void* user_data;
	uint32 color;
	
protected:
	char* m_text;
	char* m_extra_text;
	DC_MENU_CALLBACK m_selectCallback;
	XArray<char*> m_choice_texts;
	XArray<int> m_choice_values;
	CPVRTexture* m_icon_tex;
};

class DCMenu
{
public:
	DCMenu(plx_fcxt_t* font_context = NULL, DCMenu* parent = NULL);
	~DCMenu();
	
	void SetTitle(const char* sztitle);
	void SetBackground(CPVRTexture* back_tex);
	void SetColor(uint32 color);
	void SetSelectedColor(uint32 color);
	void SetFontContext(plx_fcxt_t* font_context);
	void SetFontSize(float size) { m_font_size = size; }
	bool PollController();
	void SetCurrentIndex(int16 index);
	int16 GetCurrentIndex() { return m_current_index; }
	DCMenuItem* AddItem(const char* text, DC_MENU_CALLBACK selectCallback);
	void RemoveAllItems();
	void SetDrawExtraCallback(DC_MENU_DRAW_EXTRA_CALLBACK func) { m_draw_extra_callback = func; }
	bool Run();
	void Stop(bool cascade = false);
	
	int16 x;
	int16 y;
	int16 depth;
	int16 max_width;
	int16 max_height;
	uint16 vertical_gap;
	uint16 repeat_wait;
	bool double_height;
	void* user_data;
	bool pulse;
	float pulse_speed;
	bool force_callback_on_select;
	bool poll_sound_stream;
	uint32 title_color;
	uint32 title_border_color;
	int16 title_x;
	int16 title_y;
	int16 title_slant;
	int16 title_size;
	
protected:
	void Draw();

	float m_current_pulse;
	bool m_pulse_up;
	CPVRTexture* m_background;
	float m_left_analog_buildup;
	float m_right_analog_buildup;
	float m_font_size;
	float m_original_font_size;
	uint16 m_current_wait;
	DCMenu* m_parent;
	bool m_running;
	DC_MENU_DRAW_EXTRA_CALLBACK m_draw_extra_callback;
	uint32 m_color;
	uint32 m_selected_color;
	plx_fcxt_t* m_font_context;
	int16 m_current_index;
	XArray<DCMenuItem*> m_items;
	char* m_title;
};

#endif
