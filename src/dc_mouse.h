#ifndef _DC_MOUSE_H_
#define _DC_MOUSE_H_

#include <kos.h>

class DCMouse
{
public:
	DCMouse();
	virtual ~DCMouse();
	
	void SetDevice(maple_device_t* dev) { m_dev = dev; }
	maple_device_t* GetDevice() { return m_dev; }
	void Poll();
	
	mouse_state_t* GetState() { return &m_state; }
	
	bool IsPressed(uint16 which)
	{
		return (m_state.buttons & which);
	}
	
	bool JustPressed(uint16 which)
	{
		return ((m_state.buttons & which) && !(m_old_state.buttons & which));
	}
	
	int GetDX() { return m_state.dx; }
	int GetDY() { return m_state.dy; }
	int GetDZ() { return m_state.dz; }
	
	/*
	int GetLTrig() { return m_state.ltrig; }
	int GetRTrig() { return m_state.rtrig; }
	int GetJoyX() { return m_state.joyx; }
	int GetJoyY() { return m_state.joyy; }
	*/
	
protected:
	maple_device_t* m_dev;
	mouse_state_t m_state;
	mouse_state_t m_old_state;
};

extern DCMouse g_mice[4];

#endif

