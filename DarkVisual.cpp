#include "DarkVisual.h"
#include "Screenshot.h"

void usage()
{
	op("DarkVisual v 0.1.0");
	op("");
	op("How to use:");
	op("");
	op("1.\tMove the mouse onto the title bar of targeting window.");
	op("2.\tPress F6 on your keyboard to start.");
	op("3.\tClose the window or press F8 when you wanna quit.");
	op("");
	op("");
	op("");
	return;
}

double getScale() { // get the scale of current monitor
	// program from internet

	HWND hWnd = GetDesktopWindow();
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
	int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;
	int cyPhysical = dm.dmPelsHeight;

	double horzScale = ((double)cxPhysical / (double)cxLogical);
	double vertScale = ((double)cyPhysical / (double)cyLogical);

	return (horzScale + vertScale) / 2.0;
}

int main(int argc, char* argv[])
{
	ios::sync_with_stdio(0);
	cin.tie(0);
	usage();

	while (!KEY_DOWN(VK_F6)) {};				// wait for press F6 to start

	POINT pNow = { 0,0 };						// get the window handle which the mouse pointer is pointing on
	GetCursorPos(&pNow);
	HWND hWnd = WindowFromPoint(pNow);
	if (!hWnd) {								// failed to get hWnd
		exit(1);
	}

	RECT rect;									// init
	Screenshot ss;
	Mat src;
	Mat img;
	Mat inv;
	Mat ans;
	register const double scale = getScale();	// get screen scale for calculating the position of rect. the screenshot
	register int sizeX;
	register int sizeY;

	namedWindow("invd", WINDOW_KEEPRATIO);		// create window

	// set window properties
	HWND invd_hWnd = FindWindow(NULL, L"invd");	// get hWnd of display window
	
	SetWindowLong(								// set 'borderless windowed mode'
		invd_hWnd, 
		GWL_STYLE,
		GetWindowLong(
			invd_hWnd, 
			GWL_EXSTYLE | WS_EX_TOPMOST
		)
	);
	
	SetWindowLong(								// set 'mouse penetration'
		invd_hWnd,
		GWL_EXSTYLE,
		GetWindowLong(
			invd_hWnd, 
			GWL_EXSTYLE
		) | WS_EX_TRANSPARENT | WS_EX_LAYERED);
	
	ShowWindow(invd_hWnd, SW_SHOW);				// refresh window


	register int tmpx, tmpy;
	register int a, b, c, d;
	/// Main loop
	while (!KEY_DOWN(VK_F8)) {					// if press F8 then force stop
		if (!GetWindowRect(hWnd, &rect))		// if the targeting window is no longer exist then terminate
		{
			break;
		}
		
		src = ss.getScreenshot();				// tack screenshot

		tmpx = rect.right * scale - abs(rect.left * scale);
		tmpy = rect.bottom * scale - abs(rect.top * scale);
		a = MAX(rect.left * scale, 0);
		b = MAX(rect.top * scale, 0);
		c = ((tmpx + rect.left * scale < src.cols) ? (tmpx) : (src.cols - rect.left * scale));
		d = ((tmpy + rect.top * scale < src.rows) ? (tmpy) : (src.rows) - rect.top * scale);

		img = src(Rect(a,b,c,d));				// rect. the part that needed
		
		//bitwise_not(img, inv);					// invert the color
		inv = img - Scalar(100, 100, 100);
		/*
		resize(									// resize the image
			inv,
			ans,
			Size(
				(MIN(rect.right, img.cols) - MAX(rect.left, 0)) / scale,
				(MIN(rect.bottom, img.rows) - MAX(rect.top, 0)) / scale
			),
			INTER_AREA
		);
		*/

		SetWindowPos(							// set the position of display window
			invd_hWnd,
			HWND_TOPMOST,
			rect.right,
			rect.top,
			/*
			rect.right - MAX(rect.left, 0),
			rect.bottom - MAX(rect.top, 0),
			*/
			rect.right - rect.left,
			rect.bottom - rect.top,
			SWP_SHOWWINDOW
		);

		imshow("invd", inv);					// show the inverted window on the new window
		waitKey(1000 / 60);						// maintain the FPS
	}

	// end
	cv::destroyWindow("invd");
	return 0;
}
