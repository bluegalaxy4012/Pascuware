namespace config
{
	inline bool running = true;
	inline bool activeMenu = false;
	inline bool activeESP = false;
	inline bool activeTriggerbot = false;


    inline struct ScreenSize {
        static int GetWidth() {
            static int width = InitWidth();
            return width;
        }
        static int GetHeight() {
            static int height = InitHeight();
            return height;
        }
    private:
        static int InitWidth() {
            RECT desktop;
            GetWindowRect(GetDesktopWindow(), &desktop);
            return desktop.right;
        }
        static int InitHeight() {
            RECT desktop;
            GetWindowRect(GetDesktopWindow(), &desktop);
            return desktop.bottom;
        }
    } screenSize;
}