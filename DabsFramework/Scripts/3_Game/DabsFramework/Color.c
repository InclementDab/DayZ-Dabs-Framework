// https://en.wikipedia.org/wiki/X11_color_names
// ARGB format
class Color: int
{
	static const Color ALICE_BLUE = 0xFFF0F8FF;
	static const Color ANTIQUE_WHITE = 0xFFFAEBD7;
	static const Color AQUA = 0xFF00FFFF;
	static const Color AQUAMARINE = 0xFF7FFFD4;
	static const Color AZURE = 0xFFF0FFFF;
	static const Color BEIGE = 0xFFF5F5DC;
	static const Color BISQUE = 0xFFFFE4C4;
	static const Color BLACK = 0xFF000000;
	static const Color BLANCHED_ALMOND = 0xFFFFEBCD;
	static const Color BLUE = 0xFF0000FF;
	static const Color BLUE_VIOLET = 0xFF8A2BE2;
	static const Color BROWN = 0xFFA52A2A;
	static const Color BURLYWOOD = 0xFFDEB887;
	static const Color CADET_BLUE = 0xFF5F9EA0;
	static const Color CHARTREUSE = 0xFF7FFF00;
	static const Color CHOCOLATE = 0xFFD2691E;
	static const Color CORAL = 0xFFFF7F50;
	static const Color CORNFLOWER_BLUE = 0xFF6495ED;
	static const Color CORNSILK = 0xFFFFF8DC;
	static const Color CRIMSON = 0xFFDC143C;
	static const Color CYAN = 0xFF00FFFF;
	static const Color DARK_BLUE = 0xFF00008B;
	static const Color DARK_CYAN = 0xFF008B8B;
	static const Color DARK_GOLDENROD = 0xFFB8860B;
	static const Color DARK_GRAY = 0xFFA9A9A9;
	static const Color DARK_GREEN = 0xFF006400;
	static const Color DARK_KHAKI = 0xFFBDB76B;
	static const Color DARK_MAGENTA = 0xFF8B008B;
	static const Color DARK_OLIVE_GREEN = 0xFF556B2F;
	static const Color DARK_ORANGE = 0xFFFF8C00;
	static const Color DARK_ORCHID = 0xFF9932CC;
	static const Color DARK_RED = 0xFF8B0000;
	static const Color DARK_SALMON = 0xFFE9967A;
	static const Color DARK_SEA_GREEN = 0xFF8FBC8F;
	static const Color DARK_SLATE_BLUE = 0xFF483D8B;
	static const Color DARK_SLATE_GRAY = 0xFF2F4F4F;
	static const Color DARK_TURQUOISE = 0xFF00CED1;
	static const Color DARK_VIOLET = 0xFF9400D3;
	static const Color DEEP_PINK = 0xFFFF1493;
	static const Color DEEP_SKY_BLUE = 0xFF00BFFF;
	static const Color DIM_GRAY = 0xFF696969;
	static const Color DODGER_BLUE = 0xFF1E90FF;
	static const Color FIREBRICK = 0xFFB22222;
	static const Color FLORAL_WHITE = 0xFFFFFAF0;
	static const Color FOREST_GREEN = 0xFF228B22;
	static const Color FUCHSIA = 0xFFFF00FF;
	static const Color GHOST_WHITE = 0xFFF8F8FF;
	static const Color GOLD = 0xFFFFD700;
	static const Color GOLDENROD = 0xFFDAA520;
	static const Color GRAY = 0xFFBEBEBE;
	static const Color WEB_GRAY = 0xFF808080;
	static const Color GREEN = 0xFF00FF00;
	static const Color WEB_GREEN = 0xFF008000;
	static const Color GREEN_YELLOW = 0xFFADFF2F;
	static const Color HONEYDEW = 0xFFF0FFF0;
	static const Color HOT_PINK = 0xFFFF69B4;
	static const Color INDIAN_RED = 0xFFCD5C5C;
	static const Color INDIGO = 0xFF4B0082;
	static const Color IVORY = 0xFFFFFFF0;
	static const Color KHAKI = 0xFFF0E68C;
	static const Color LAVENDER = 0xFFE6E6FA;
	static const Color LAVENDER_BLUSH = 0xFFFFF0F5;
	static const Color LAWN_GREEN = 0xFF7CFC00;
	static const Color LEMON_CHIFFON = 0xFFFFFACD;
	static const Color LIGHT_BLUE = 0xFFADD8E6;
	static const Color LIGHT_CORAL = 0xFFF08080;
	static const Color LIGHT_CYAN = 0xFFE0FFFF;
	static const Color LIGHT_GOLDENROD = 0xFFFAFAD2;
	static const Color LIGHT_GRAY = 0xFFD3D3D3;
	static const Color LIGHT_GREEN = 0xFF90EE90;
	static const Color LIGHT_PINK = 0xFFFFB6C1;
	static const Color LIGHT_SALMON = 0xFFFFA07A;
	static const Color LIGHT_SEA_GREEN = 0xFF20B2AA;
	static const Color LIGHT_SKY_BLUE = 0xFF87CEFA;
	static const Color LIGHT_SLATE_GRAY = 0xFF778899;
	static const Color LIGHT_STEEL_BLUE = 0xFFB0C4DE;
	static const Color LIGHT_YELLOW = 0xFFFFFFE0;
	static const Color LIME = 0xFF00FF00;
	static const Color LIME_GREEN = 0xFF32CD32;
	static const Color LINEN = 0xFFFAF0E6;
	static const Color MAGENTA = 0xFFFF00FF;
	static const Color MAROON = 0xFFB03060;
	static const Color WEB_MAROON = 0xFF800000;
	static const Color MEDIUM_AQUAMARINE = 0xFF66CDAA;
	static const Color MEDIUM_BLUE = 0xFF0000CD;
	static const Color MEDIUM_ORCHID = 0xFFBA55D3;
	static const Color MEDIUM_PURPLE = 0xFF9370DB;
	static const Color MEDIUM_SEA_GREEN = 0xFF3CB371;
	static const Color MEDIUM_SLATE_BLUE = 0xFF7B68EE;
	static const Color MEDIUM_SPRING_GREEN = 0xFF00FA9A;
	static const Color MEDIUM_TURQUOISE = 0xFF48D1CC;
	static const Color MEDIUM_VIOLET_RED = 0xFFC71585;
	static const Color MIDNIGHT_BLUE = 0xFF191970;
	static const Color MINT_CREAM = 0xFFF5FFFA;
	static const Color MISTY_ROSE = 0xFFFFE4E1;
	static const Color MOCCASIN = 0xFFFFE4B5;
	static const Color NAVAJO_WHITE = 0xFFFFDEAD;
	static const Color NAVY_BLUE = 0xFF000080;
	static const Color OLD_LACE = 0xFFFDF5E6;
	static const Color OLIVE = 0xFF808000;
	static const Color OLIVE_DRAB = 0xFF6B8E23;
	static const Color ORANGE = 0xFFFFA500;
	static const Color ORANGE_RED = 0xFFFF4500;
	static const Color ORCHID = 0xFFDA70D6;
	static const Color PALE_GOLDENROD = 0xFFEEE8AA;
	static const Color PALE_GREEN = 0xFF98FB98;
	static const Color PALE_TURQUOISE = 0xFFAFEEEE;
	static const Color PALE_VIOLET_RED = 0xFFDB7093;
	static const Color PAPAYA_WHIP = 0xFFFFEFD5;
	static const Color PEACH_PUFF = 0xFFFFDAB9;
	static const Color PERU = 0xFFCD853F;
	static const Color PINK = 0xFFFFC0CB;
	static const Color PLUM = 0xFFDDA0DD;
	static const Color POWDER_BLUE = 0xFFB0E0E6;
	static const Color PURPLE = 0xFFA020F0;
	static const Color WEB_PURPLE = 0xFF800080;
	static const Color REBECCA_PURPLE = 0xFF663399;
	static const Color RED = 0xFFFF0000;
	static const Color ROSY_BROWN = 0xFFBC8F8F;
	static const Color ROYAL_BLUE = 0xFF4169E1;
	static const Color SADDLE_BROWN = 0xFF8B4513;
	static const Color SALMON = 0xFFFA8072;
	static const Color SANDY_BROWN = 0xFFF4A460;
	static const Color SEA_GREEN = 0xFF2E8B57;
	static const Color SEASHELL = 0xFFFFF5EE;
	static const Color SIENNA = 0xFFA0522D;
	static const Color SILVER = 0xFFC0C0C0;
	static const Color SKY_BLUE = 0xFF87CEEB;
	static const Color SLATE_BLUE = 0xFF6A5ACD;
	static const Color SLATE_GRAY = 0xFF708090;
	static const Color SNOW = 0xFFFFFAFA;
	static const Color SPRING_GREEN = 0xFF00FF7F;
	static const Color STEEL_BLUE = 0xFF4682B4;
	static const Color TAN = 0xFFD2B48C;
	static const Color TEAL = 0xFF008080;
	static const Color THISTLE = 0xFFD8BFD8;
	static const Color TOMATO = 0xFFFF6347;
	static const Color TURQUOISE = 0xFF40E0D0;
	static const Color VIOLET = 0xFFEE82EE;
	static const Color WHEAT = 0xFFF5DEB3;
	static const Color WHITE = 0xFFFFFFFF;
	static const Color WHITE_SMOKE = 0xFFF5F5F5;
	static const Color YELLOW = 0xFFFFFF00;
	static const Color YELLOW_GREEN = 0xFF9ACD32;
	
	static const Color DISCORD = Color.Create(88, 101, 242);
	static const Color TWITTER = Color.Create(29, 161, 242);
	static const Color YOUTUBE = Color.Create(255, 0, 0);
	static const Color REDDIT = Color.Create(255, 69, 0);
	
	static Color Create(int r, int g, int b)
	{
		return 255 << 24 | r << 16 | g << 8 | b;
	}
	
	static Color Create(int a, int r, int g, int b)
	{
		return a << 24 | r << 16 | g << 8 | b;
	}
		
	static Color CreateF(float r, float g, float b)
	{
		return 255 << 24 | (int)(r * 255.0) << 16 | (int)(g * 255.0) << 8 | (int)(b * 255.0);
	}
	
	static Color CreateF(float a, float r, float g, float b)
	{
		return (int)(a * 255.0) << 24 | (int)(r * 255.0) << 16 | (int)(g * 255.0) << 8 | (int)(b * 255.0);
	}
	
	void SetAlpha(int alpha)
	{
		value = (alpha << 24) ^ value;
	}
	
	void SetRed(int red)
	{
		value = (red << 16) ^ value;	
	}	
	
	void SetGreen(int green)
	{
		value = (green << 8) ^ value;	
	}	
	
	void SetBlue(int blue)
	{
		value = (blue << 0) ^ value;
	}
}

typedef int Color;

/*
typedef int Color;

//@ Storage format of color is ARGB
class Color: int
{	
	private void Color();
	private void ~Color();
	

	
	static Color FromHSV(float hue, float saturation, float value, int alpha = 255)
	{
		int hexasphere = Math.FMod(hue, 360.0) / 60;
		int trientsphere = Math.FMod(hue, 360.0) / 120;
		int hemisphere = Math.FMod(hue, 360.0) / 180;
		Print(hexasphere);
		Print(trientsphere);
		Print(hemisphere);
		
		hue 		= Math.FMod(hue, 60.0);		
		saturation 	= Math.FMod(saturation, 100.0) / 100.0;
		value 		= Math.FMod(value, 100.0) / 100.0;
		
	    float c = saturation * value;
	    float x = c * (1.0 - Math.AbsFloat(Math.FMod(hue / 60.0, 2.0) - 1.0));
	    float m = value - c;
		
		int argb;
		//argb = (x << (trientsphere * 8 + 
		
		
		int r, g, b;
		// trientsphere 0
		// hemisphere 0
	    if (hue >= 0 && hue < 60) {
	       	r = c + m;  
			g = x + m;  
			b = 0 + m; 
	    }
		
		// trientsphere 0
		// hemisphere 0
		if (hue >= 180 && hue < 240) {
	        r = 0 + m; 
			g = x + m;   
			b = c + m; 
	    }
		
		// trientsphere 1
		// hemisphere
		if (hue >= 60 && hue < 120) {
			r = x + m; 
			g = c + m; 
			b = 0 + m; 
	    }
		
		// trientsphere 1
		if (hue >= 240 && hue < 300) {
			r = x + m;  
			g = 0 + m;  
			b = c + m; 
	    }
		
		// trientsphere 2
		if (hue >= 120 && hue < 180) {
			r = 0 + m; 
			g = c + m; 
			b = x + m; 
	    }
	    
		// trientsphere 2
		if (hue >= 300 && hue < 360) {
			r = c + m; 
			g = 0 + m; 
			b = x + m;
		}
		
		return ARGB(alpha, r, g, b);
	}
}
*/