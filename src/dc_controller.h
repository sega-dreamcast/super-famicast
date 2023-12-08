#ifndef _DC_CONTROLLER_H_
#define _DC_CONTROLLER_H_

#include <kos.h>

class DCController
{
public:
	DCController();
	virtual ~DCController();
	
	void SetDevice(maple_device_t* dev) { m_dev = dev; }
	maple_device_t* GetDevice() { return m_dev; }
	void Poll();
	
	cont_state_t* GetState() { return &m_state; }
	
	bool IsPressed(uint16 which)
	{
		return (m_state.buttons & which);
	}
	
	bool JustPressed(uint16 which)
	{
		return ((m_state.buttons & which) && !(m_old_state.buttons & which));
	}
	
	int GetLTrig() { return m_state.ltrig; }
	int GetRTrig() { return m_state.rtrig; }
	int GetJoyX() { return m_state.joyx; }
	int GetJoyY() { return m_state.joyy; }
	
protected:
	maple_device_t* m_dev;
	cont_state_t m_state;
	cont_state_t m_old_state;
};

extern DCController g_controllers[4];

#endif

