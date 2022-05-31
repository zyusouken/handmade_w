/*
	HEADER HEADER HEADER HEADER HEADER HEADER 
	HEADER HEADER HEADER HEADER HEADER HEADER 
	HEADER HEADER HEADER HEADER HEADER HEADER
	========================================= */

#include<windows.h>

//Replaced __clrcall with CALLBACK because of errors, plus Casey uses CALLLBACK.
int CALLBACK
WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,	
	LPSTR     lpCmdLine,
	int       nCmdShow
){
	MessageBoxA
	(
		0,
		"This is Handmade Hero.",
		"Handmade Hero",
		MB_OK|MB_ICONINFORMATION
	);
	return(0);
}
