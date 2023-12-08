#include "dc_controller.h"

DCController g_controllers[4];

DCController::DCController()
{
	m_dev = NULL;
	m_old_state;
	memset(&m_old_state, 0, sizeof(cont_state_t));
	memset(&m_state, 0, sizeof(cont_state_t));
}

DCController::~DCController()
{
	
}

void DCController::Poll()
{
	m_old_state = m_state;
	m_state = *((cont_state_t*) maple_dev_status (m_dev));
}


