
#include "main.h"
#include "input.h"


BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
MouseInput Input::m_MouseInput;

void Input::Init()
{

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
	m_MouseInput = None;
}

void Input::Uninit()
{


}

void Input::Update()
{

	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState( m_KeyState );

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

void Input::SetMouseInput(MouseInput mouseInput)
{
	m_MouseInput = mouseInput;
}