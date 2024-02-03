enum WidgetAnimatorProperty
{
	POSITION_X 	= 1,
	POSITION_Y 	= 2,

	SIZE_W		= 4,
	SIZE_H		= 8,

	ROTATION_X	= 16, // Roll
	ROTATION_Y	= 32, // Pitch
	ROTATION_Z	= 64, // Yaw

	// all color values are in float from 0...1
	COLOR_A		= 128, 
	
	COLOR_R		= 256,
	COLOR_G		= 512,
	COLOR_B		= 1024,
	
	COLOR_H		= 2048,
	COLOR_S		= 4096,
	COLOR_V		= 8192,
	
	EXACT_TEXT 	= 16384,
}