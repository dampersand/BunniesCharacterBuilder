#include "Engine.h"



int Engine::buttonRouter(WPARAM wParam)
{
	switch(LOWORD(wParam))
	{
	case ID_ST_ADD:
	case ID_ST_SUB:
	case ID_IQ_ADD:
	case ID_IQ_SUB:
	case ID_DX_ADD:
	case ID_DX_SUB:
	case ID_HT_ADD:
	case ID_HT_SUB: //if a base-stat button made the call
		bigFour.engineReceiver(LOWORD(wParam));
		break;
	default:
		MessageBox(NULL, L"shit, not a recognized word", NULL, NULL);
		return -1; //handle an error here
	}
	return 0;
}