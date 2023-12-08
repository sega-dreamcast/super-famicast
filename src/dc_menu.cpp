#include "dc_menu.h"
#include "dc_controller.h"
#include "scherzo_snd_stream.h"

extern struct
{
	int32 x1, y1, x2, y2;
	float xscale, yscale;
} screen_adjustments;

DCMenuItem::DCMenuItem()
{
	m_text = NULL;
	m_extra_text = NULL;
	m_selectCallback = NULL;
	m_current_choice = 0;
	choice_style = CHOICE_STYLE_BELOW;
	user_data = NULL;
	m_icon_tex = NULL;
	color = 0x11111111;
}

DCMenuItem::~DCMenuItem()
{
	if (m_text)
		delete[] m_text;
	if (m_extra_text)
		delete[] m_extra_text;
	for (unsigned int k = 0; k < m_choice_texts.size(); ++k)
		delete[] m_choice_texts[k];
}

void DCMenuItem::SetText(const char* str)
{
	int slen = strlen(str);
	m_text = new char[slen + 1];
	memcpy(m_text, str, slen);
	m_text[slen] = '\0';
}

void DCMenuItem::SetSelectCallback(DC_MENU_CALLBACK func)
{
	m_selectCallback = func;
}

void DCMenuItem::AddChoice(const char* text, int value, bool selected)
{
	int slen = strlen(text);
	char* strtemp = new char[slen + 1];
	memcpy(strtemp, text, slen);
	strtemp[slen] = '\0';
	if (selected)
		m_current_choice = m_choice_texts.size();
	m_choice_texts.push(strtemp);
	m_choice_values.push(value);
}

void DCMenuItem::SetIcon(CPVRTexture* icon_tex)
{
	m_icon_tex = icon_tex;
}

DCMenu::DCMenu(plx_fcxt_t* font_context, DCMenu* parent)
{
	m_current_index = 0;
	m_color = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_selected_color = PVR_PACK_COLOR(1.0f, 1.0f, 0.0f, 0.0f);
	m_running = false;
	m_font_context = font_context;
	m_current_pulse = 0;
	m_pulse_up = false;
	if (parent)
	{
		m_parent = parent;
		x = parent->x;
 		y = parent->y;
 		depth = parent->depth;
 		max_width = parent->max_width;
 		max_height = parent->max_height;
 		vertical_gap = parent->vertical_gap;
 		repeat_wait = parent->repeat_wait;
 		m_draw_extra_callback = parent->m_draw_extra_callback;
 		m_background = parent->m_background;
 		m_color = parent->m_color;
 		m_selected_color = parent->m_selected_color;
 		m_font_size = parent->m_font_size;
 		pulse = parent->pulse;
 		pulse_speed = parent->pulse_speed;
 		poll_sound_stream = parent->poll_sound_stream;
 		if (parent->m_title)
 		{
 			m_title = new char[strlen(parent->m_title) + 1];
 			strcpy(m_title, parent->m_title);
		}
		else
			m_title = NULL;
		title_color = parent->title_color;
		title_border_color = parent->title_border_color;
		title_x = parent->title_x;
		title_y = parent->title_y;
		title_slant = parent->title_slant;
		title_size = parent->title_size;
	}
	else
	{
		m_color = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_selected_color = PVR_PACK_COLOR(1.0f, 1.0f, 0.0f, 0.0f);
		m_parent = NULL;
		x = 10;
 		y = MIN_Y;
 		depth = 10;
 		max_width = 640;
 		max_height = 480 - MIN_Y;
		vertical_gap = 10;
		repeat_wait = 15;
		m_draw_extra_callback = NULL;
		m_background = NULL;
		m_font_size = -1.0f;
		pulse = true;
		pulse_speed = 0.012f;
		poll_sound_stream = false;
		m_title = NULL;
		title_color = 0xFFFFFFFF;
		title_border_color = 0xFF000000;
		title_x = 0;
		title_y = 0;
		title_slant = 0;
		title_size = -1.0f;
	}
	force_callback_on_select = false;
	m_current_wait = 0;
	m_left_analog_buildup = 0.0f;
	m_right_analog_buildup = 0.0f;
	double_height = false;
	user_data = NULL;
}

DCMenu::~DCMenu()
{
	for (uint32 k = 0; k < m_items.size(); ++k)
		delete m_items[k];
	if (m_title)
		delete[] m_title;
}

void DCMenu::SetBackground(CPVRTexture* back_tex)
{
	m_background = back_tex;
}

void DCMenu::SetColor(uint32 color)
{
	m_color = color;
}

void DCMenu::SetSelectedColor(uint32 color)
{
	m_selected_color = color;
}

void DCMenu::SetFontContext(plx_fcxt_t* font_context)
{
	m_font_context = font_context;
}

bool DCMenu::PollController()
{
	DCController* controller = g_controllers;
	controller->Poll();
	unsigned int item_count = m_items.size();
	
	if (controller->IsPressed(CONT_DPAD_UP))
 	{
 		if (controller->JustPressed(CONT_DPAD_UP))
 			m_current_wait = repeat_wait + 1;
  		if (m_current_wait > repeat_wait)
  		{
 			--m_current_index;
 			if (m_current_index < 0)
 				m_current_index = 0;
 			m_current_wait = 0;
		}
		else
			++m_current_wait;
	}
	if (controller->IsPressed(CONT_DPAD_DOWN))
 	{
 		if (controller->JustPressed(CONT_DPAD_DOWN))
 			m_current_wait = repeat_wait + 1;
  		if (m_current_wait > repeat_wait)
  		{
 			++m_current_index;
 			if (m_current_index == ((int16) item_count))
 				--m_current_index;
 			m_current_wait = 0;
		}
		else
			++m_current_wait;
	}
	
	DCMenuItem* item = m_items[m_current_index];
	unsigned int choice_count = item->m_choice_texts.size();
	
	if (choice_count > 0)
	{
		DCMenuItem* item = m_items[m_current_index];
		uint16 old_item_choice = item->m_current_choice;
		if (controller->IsPressed(CONT_DPAD_LEFT))
		{
			if (controller->JustPressed(CONT_DPAD_LEFT))
   				m_current_wait = repeat_wait + 1;
       		if (m_current_wait > repeat_wait)
			{
				--item->m_current_choice;
				if (item->m_current_choice < 0)
					item->m_current_choice = choice_count - 1;
				m_current_wait = 0;
			}
			else
				++m_current_wait;
		}
		if (controller->IsPressed(CONT_DPAD_RIGHT))
		{
			if (controller->JustPressed(CONT_DPAD_RIGHT))
   				m_current_wait = repeat_wait + 1;
       		if (m_current_wait > repeat_wait)
			{
				++item->m_current_choice;
				if (item->m_current_choice == ((int16) choice_count))
					item->m_current_choice = 0;
				m_current_wait = 0;
			}
			else
				++m_current_wait;
		}
		if (old_item_choice != item->m_current_choice && item->m_selectCallback)
			item->m_selectCallback(this, item, item->m_choice_values[item->m_current_choice]);
	}
	
	if (controller->GetLTrig() > 5)
 	{
 		m_left_analog_buildup += (((float) controller->GetLTrig()) - 5.0f) / 245.0f;
 		if (m_left_analog_buildup > 1.0f)
 		{
 			m_left_analog_buildup = 0.0f;
 			--m_current_index;
 		}
	}
	else
		m_left_analog_buildup = 0.0f;
	
	if (controller->GetRTrig() > 5)
 	{
 		m_right_analog_buildup += (((float) controller->GetRTrig()) - 5.0f) / 245.0f;
 		if (m_right_analog_buildup > 1.0f)
 		{
 			m_right_analog_buildup = 0.0f;
 			++m_current_index;
 		}
	}
	else
		m_right_analog_buildup = 0.0f;
	
	if (m_current_index < 0)
		m_current_index = 0;
	if (m_current_index > ((int16) (item_count - 1)))
		m_current_index = item_count - 1;
	
	if (controller->JustPressed(CONT_B))
	{
		m_running = false;
		return false;
	}
	if (controller->JustPressed(CONT_A) && m_items.size() != 0)
	{
		if (item->m_selectCallback)
		{
			if (choice_count == 0)
				item->m_selectCallback(this, item, -9999);
			else if (force_callback_on_select)
				item->m_selectCallback(this, item, item->m_choice_values[item->m_current_choice]);
		}
	}
	return true;
}

void DCMenu::SetCurrentIndex(int16 index)
{
	if (m_items.size() == 0)
		return;
	m_current_index = index;
	if (m_current_index < 0)
		m_current_index = 0;
	else if (m_current_index >= ((int16) m_items.size()))
		m_current_index = (int16) (m_items.size() - 1);
}

DCMenuItem* DCMenu::AddItem(const char* text, DC_MENU_CALLBACK selectCallback)
{
	DCMenuItem* temp = new DCMenuItem();
	temp->SetText(text);
	temp->SetSelectCallback(selectCallback);
	temp->color = m_color;
	m_items.push(temp);
	return temp;
}

void DCMenu::RemoveAllItems()
{
	for (uint32 k = 0; k < m_items.size(); ++k)
		delete m_items[k];
	m_items.removeAll();
	m_current_index = 0;
}

void DCMenu::SetTitle(const char* sztitle)
{
	if (m_title)
		delete[] m_title;
	if (sztitle)
	{
		m_title = new char[strlen(sztitle) + 1];
		strcpy(m_title, sztitle);
	}
	else
		m_title = NULL;
}

#define PACK_ALPHA(c, a) ((((uint8) ((a) * 255)) << 24) | (c & 0x00FFFFFF))

void DCMenu::Draw()
{
	unsigned int item_count = m_items.size();
	if (item_count == 0)
		return;
	if (m_font_size >= 0.0f)
	{
		m_original_font_size = m_font_context->size;
		m_font_context->size = m_font_size;
	}
	
	float outleft, outup, outright, outdown;
	plx_fcxt_char_metrics(m_font_context, 'T', &outleft, &outup, &outright, &outdown);
	int16 ystep = ((int16) outup) + vertical_gap;
	int16 visible_row_count = max_height / ystep;
	if (double_height)
		visible_row_count = visible_row_count / 2;
	int16 first_half_visible_count = visible_row_count / 2;
	int16 last_half_visible_count = first_half_visible_count;
	if ((visible_row_count % 2) == 1)
		last_half_visible_count += 1;
	int16 curx;
	int16 cury = y + ((int16) outup);
	point_t w;
	w.z = (float) depth;
	DCMenuItem* curitem = NULL;
	pvr_list_begin(PVR_LIST_TR_POLY);
	
	if (m_title)
	{
		w.x = (float) title_x;
		w.y = (float) title_y + outup;
		float old_slant = m_font_context->slant;
		m_font_context->slant = title_slant;
		if (title_size > 0)
			m_font_context->size = title_size;
		DrawStringWithBorder(m_title, m_font_context, w, title_color, title_border_color);
		m_font_context->slant = old_slant;
		m_font_context->size = m_font_size;
	}
	
	unsigned int k = 0;
	if (item_count > visible_row_count)
	{
		if (m_current_index > first_half_visible_count)
			k = m_current_index - first_half_visible_count;
		if ((item_count - m_current_index) < last_half_visible_count && item_count > first_half_visible_count)
			k = item_count - visible_row_count;
	}
	unsigned int final_k = k + visible_row_count;

	uint32 tempcolor = m_selected_color;
	if (pulse)
	{
		if (m_pulse_up)
			m_current_pulse += pulse_speed;
		else
			m_current_pulse -= pulse_speed;
		if (m_current_pulse >= 0.10 || m_current_pulse <= -0.10)
			m_pulse_up = !m_pulse_up;
		float tempalpha = 0.90f + m_current_pulse;
		if (tempalpha > 1.0f)
			tempalpha = 1.0f;
		tempcolor = PACK_ALPHA(tempcolor, tempalpha);
	}
	
	char temptext[0x100];
	int32 curnull;
	
	for (; k < final_k && k < item_count; ++k)
	{
		curx = x;
		curitem = m_items[k];
		strcpy(temptext, curitem->m_text);
		curnull = strlen(temptext);
		while (true)
		{
			plx_fcxt_str_metrics(m_font_context, temptext, &outleft, &outup, &outright, &outdown);
			if (outright > max_width)
			{
				--curnull;
				if (curnull == 0)
					return;
				temptext[curnull] = '\0';
			}
			else
				break;
		}
		
		if (curitem->m_icon_tex)
		{
			curitem->m_icon_tex->Draw(curx, cury - outup, depth);
			//curitem->m_icon_tex->DrawFullscreen(depth);
			curx += curitem->m_icon_tex->GetWidth() + (ystep / 2);
		}
		
		w.x = (float) curx;
		w.y = (float) cury;
		DrawStringWithBorder(temptext, m_font_context, w,
  						(((int16) k) == m_current_index) ? tempcolor : curitem->color, PVR_PACK_COLOR(1.0f, 0, 0, 0));
		unsigned int choice_count = curitem->m_choice_texts.size();
		if (choice_count > 0)
		{
			if (curitem->choice_style == CHOICE_STYLE_BELOW)
			{
				cury += ystep;
				curx = x + ystep;
			}
			else if (curitem->choice_style == CHOICE_STYLE_NEXT_TO)
				curx = x + outright + ystep;
			for (unsigned int n = 0; n < choice_count; ++n)
			{
				const char* choice_text = curitem->m_choice_texts[n];
				uint32 mycolor;
				mycolor = (n == curitem->m_current_choice) ? m_selected_color: m_color;
				plx_fcxt_str_metrics(m_font_context, choice_text, &outleft, &outup, &outright, &outdown);
				w.x = (float) curx;
				w.y = (float) cury;
				DrawStringWithBorder(choice_text, m_font_context, w, mycolor, PVR_PACK_COLOR(1.0f, 0, 0, 0));
				curx += outright + (ystep / 2);
			}
		}
		cury += ystep;
	}
	if (m_draw_extra_callback)
		m_draw_extra_callback(this);
	if (m_background)
		m_background->DrawFullscreen(depth - 2);
	pvr_list_finish ();
	if (m_font_size >= 0.0f)
		m_font_context->size = m_original_font_size;
}

bool DCMenu::Run()
{
	m_running = true;
	bool success;
	while (m_running)
	{
		pvr_wait_ready();
		pvr_scene_begin();
		Draw();
		pvr_scene_finish();
		if (poll_sound_stream)
			scherzo_snd_stream_poll();
		success = PollController();
	}
	return success;
}

void DCMenu::Stop(bool cascade = false)
{
	m_running = false;
	if (cascade && m_parent)
		m_parent->Stop(true);
}
