// https://en.wikipedia.org/wiki/Blend_modes
enum BlendMode
{
	// f(a, b) = b
	NORMAL, 
	
	// f(a, b) = ab;
	MULTIPLY,
	
	// f(a, b) = 1 - (1 - a)(1 - b)
	SCREEN,
	
	// f(a, b) = { 2ab,   					if a < 0.5
	//			 { 1 - 2(1 - a)(1 - b),		otherwise
	OVERLAY,
	
	// f(a, b) = { 2ab,   					if b < 0.5
	//			 { 1 - 2(1 - a)(1 - b),		otherwise
	HARD_LIGHT,
	
	// f(a, b) = { 2ab + (a * a)(1 - 2b),   		if b < 0.5
	//			 { 2a(1 - b) + sqrt(a) * (2b - 1) ,	otherwise
	SOPFT_LIGHT
};

// https://en.wikipedia.org/wiki/X11_color_names
// ARGB format
class Color: int
{
	static const int ALPHA_CHANNEL = 0;
	static const int RED_CHANNEL = 1;
	static const int GREEN_CHANNEL = 2;
	static const int BLUE_CHANNEL = 3;
	
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
	
	// 0: hue [0, 360]
	// 1: saturation [0, 1.0]
	// 2: value [0, 1.0]
	// 3: alpha [0, 1.0]
	static Color CreateHSV(int hue, float saturation, float value, float alpha = 1.0)
	{
		hue 		= Math.Clamp(hue, 0, 360);
		saturation 	= Math.Clamp(saturation, 0.0, 1.0);
		value 		= Math.Clamp(value, 0.0, 1.0);
				
		int hexasphere = hue / 60;
		Print(hexasphere);
		float c = saturation * value;	
	    float x = c * (1 - Math.AbsFloat(Math.FMod(hexasphere, 2) - 1));
	    float m = value - c;
		float r, g, b;
		switch (hexasphere) {
			case 0: {
				r = c; 
				g = x; 
				b = 0;
				break;
			}
			
			case 1: {
				r = x; 
				g = c; 
				b = 0;
				break;
			}
			
			case 2: {
				r = 0; 
				g = c; 
				b = x;
				break;
			}
			
			case 3: {
				r = 0; 
				g = x; 
				b = c;
				break;
			}
			
			case 4: {
				r = x; 
				g = 0; 
				b = c;
				break;
			}
			
			case 5: {
				r = c; 
				g = 0; 
				b = x;
				break;
			}
		}		
				
		r += m; g += m; b += m;
		return CreateF(alpha, r, g, b);
	}
	
	static Color Blend(Color a, Color b, BlendMode blend)
	{
		switch (blend) {
			case BlendMode.NORMAL: {
				return b;
			}
		}
		
		return a;
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
	
	// 0 = A, 1 = R, 2 = G, 3 = B
	void Set(int n, uint8 val)
	{
		value = (val << (n * 8)) ^ value;
	}

	string ToHex()
	{
		return Encoding.ToHex(value, 8);
	}
	
/*
	vector ToVector()
	{
		
	}*/
}

typedef int Color;