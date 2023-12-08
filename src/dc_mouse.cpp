#include "dc_mouse.h"

DCMouse g_mice[4];

DCMouse::DCMouse()
{
	m_dev = NULL;
	memset(&m_old_state, 0, sizeof(mouse_state_t));
	memset(&m_state, 0, sizeof(mouse_state_t));
}

DCMouse::~DCMouse()
{
	
}

void DCMouse::Poll()
{
	m_old_state = m_state;
	m_state = *((mouse_state_t*) maple_dev_status (m_dev));
}


