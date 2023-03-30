#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#include"Initialize.h"
#include"Global.h"
#include <time.h>
int APIENTRY WinMain(HINSTANCE hlnstance, HINSTANCE hPrevlnstance, LPSTR , int nCmdShow) {
	srand(time(NULL));
   InitWindow(hlnstance);
   lnitD3D(FloatWindow(hlnstance,nCmdShow));

   return DoMainLoop();
   
}

