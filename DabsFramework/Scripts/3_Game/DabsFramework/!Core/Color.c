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
	SOFT_LIGHT
};

// https://en.wikipedia.org/wiki/X11_LinearColor_names
// ARGB format
class LinearColor: int
{
	static const int ALPHA_CHANNEL = 0;
	static const int RED_CHANNEL = 1;
	static const int GREEN_CHANNEL = 2;
	static const int BLUE_CHANNEL = 3;
	
	static const LinearColor ALICE_BLUE = 0xFFF0F8FF;
	static const LinearColor ANTIQUE_WHITE = 0xFFFAEBD7;
	static const LinearColor AQUA = 0xFF00FFFF;
	static const LinearColor AQUAMARINE = 0xFF7FFFD4;
	static const LinearColor AZURE = 0xFFF0FFFF;
	static const LinearColor BEIGE = 0xFFF5F5DC;
	static const LinearColor BISQUE = 0xFFFFE4C4;
	static const LinearColor BLACK = 0xFF000000;
	static const LinearColor BLANCHED_ALMOND = 0xFFFFEBCD;
	static const LinearColor BLUE = 0xFF0000FF;
	static const LinearColor BLUE_VIOLET = 0xFF8A2BE2;
	static const LinearColor BROWN = 0xFFA52A2A;
	static const LinearColor BURLYWOOD = 0xFFDEB887;
	static const LinearColor CADET_BLUE = 0xFF5F9EA0;
	static const LinearColor CHARTREUSE = 0xFF7FFF00;
	static const LinearColor CHOCOLATE = 0xFFD2691E;
	static const LinearColor CORAL = 0xFFFF7F50;
	static const LinearColor CORNFLOWER_BLUE = 0xFF6495ED;
	static const LinearColor CORNSILK = 0xFFFFF8DC;
	static const LinearColor CRIMSON = 0xFFDC143C;
	static const LinearColor CYAN = 0xFF00FFFF;
	static const LinearColor DARK_BLUE = 0xFF00008B;
	static const LinearColor DARK_CYAN = 0xFF008B8B;
	static const LinearColor DARK_GOLDENROD = 0xFFB8860B;
	static const LinearColor DARK_GRAY = 0xFFA9A9A9;
	static const LinearColor DARK_GREEN = 0xFF006400;
	static const LinearColor DARK_KHAKI = 0xFFBDB76B;
	static const LinearColor DARK_MAGENTA = 0xFF8B008B;
	static const LinearColor DARK_OLIVE_GREEN = 0xFF556B2F;
	static const LinearColor DARK_ORANGE = 0xFFFF8C00;
	static const LinearColor DARK_ORCHID = 0xFF9932CC;
	static const LinearColor DARK_RED = 0xFF8B0000;
	static const LinearColor DARK_SALMON = 0xFFE9967A;
	static const LinearColor DARK_SEA_GREEN = 0xFF8FBC8F;
	static const LinearColor DARK_SLATE_BLUE = 0xFF483D8B;
	static const LinearColor DARK_SLATE_GRAY = 0xFF2F4F4F;
	static const LinearColor DARK_TURQUOISE = 0xFF00CED1;
	static const LinearColor DARK_VIOLET = 0xFF9400D3;
	static const LinearColor DEEP_PINK = 0xFFFF1493;
	static const LinearColor DEEP_SKY_BLUE = 0xFF00BFFF;
	static const LinearColor DIM_GRAY = 0xFF696969;
	static const LinearColor DODGER_BLUE = 0xFF1E90FF;
	static const LinearColor FIREBRICK = 0xFFB22222;
	static const LinearColor FLORAL_WHITE = 0xFFFFFAF0;
	static const LinearColor FOREST_GREEN = 0xFF228B22;
	static const LinearColor FUCHSIA = 0xFFFF00FF;
	static const LinearColor GHOST_WHITE = 0xFFF8F8FF;
	static const LinearColor GOLD = 0xFFFFD700;
	static const LinearColor GOLDENROD = 0xFFDAA520;
	static const LinearColor GRAY = 0xFFBEBEBE;
	static const LinearColor WEB_GRAY = 0xFF808080;
	static const LinearColor GREEN = 0xFF00FF00;
	static const LinearColor WEB_GREEN = 0xFF008000;
	static const LinearColor GREEN_YELLOW = 0xFFADFF2F;
	static const LinearColor HONEYDEW = 0xFFF0FFF0;
	static const LinearColor HOT_PINK = 0xFFFF69B4;
	static const LinearColor INDIAN_RED = 0xFFCD5C5C;
	static const LinearColor INDIGO = 0xFF4B0082;
	static const LinearColor IVORY = 0xFFFFFFF0;
	static const LinearColor KHAKI = 0xFFF0E68C;
	static const LinearColor LAVENDER = 0xFFE6E6FA;
	static const LinearColor LAVENDER_BLUSH = 0xFFFFF0F5;
	static const LinearColor LAWN_GREEN = 0xFF7CFC00;
	static const LinearColor LEMON_CHIFFON = 0xFFFFFACD;
	static const LinearColor LIGHT_BLUE = 0xFFADD8E6;
	static const LinearColor LIGHT_CORAL = 0xFFF08080;
	static const LinearColor LIGHT_CYAN = 0xFFE0FFFF;
	static const LinearColor LIGHT_GOLDENROD = 0xFFFAFAD2;
	static const LinearColor LIGHT_GRAY = 0xFFD3D3D3;
	static const LinearColor LIGHT_GREEN = 0xFF90EE90;
	static const LinearColor LIGHT_PINK = 0xFFFFB6C1;
	static const LinearColor LIGHT_SALMON = 0xFFFFA07A;
	static const LinearColor LIGHT_SEA_GREEN = 0xFF20B2AA;
	static const LinearColor LIGHT_SKY_BLUE = 0xFF87CEFA;
	static const LinearColor LIGHT_SLATE_GRAY = 0xFF778899;
	static const LinearColor LIGHT_STEEL_BLUE = 0xFFB0C4DE;
	static const LinearColor LIGHT_YELLOW = 0xFFFFFFE0;
	static const LinearColor LIME = 0xFF00FF00;
	static const LinearColor LIME_GREEN = 0xFF32CD32;
	static const LinearColor LINEN = 0xFFFAF0E6;
	static const LinearColor MAGENTA = 0xFFFF00FF;
	static const LinearColor MAROON = 0xFFB03060;
	static const LinearColor WEB_MAROON = 0xFF800000;
	static const LinearColor MEDIUM_AQUAMARINE = 0xFF66CDAA;
	static const LinearColor MEDIUM_BLUE = 0xFF0000CD;
	static const LinearColor MEDIUM_ORCHID = 0xFFBA55D3;
	static const LinearColor MEDIUM_PURPLE = 0xFF9370DB;
	static const LinearColor MEDIUM_SEA_GREEN = 0xFF3CB371;
	static const LinearColor MEDIUM_SLATE_BLUE = 0xFF7B68EE;
	static const LinearColor MEDIUM_SPRING_GREEN = 0xFF00FA9A;
	static const LinearColor MEDIUM_TURQUOISE = 0xFF48D1CC;
	static const LinearColor MEDIUM_VIOLET_RED = 0xFFC71585;
	static const LinearColor MIDNIGHT_BLUE = 0xFF191970;
	static const LinearColor MINT_CREAM = 0xFFF5FFFA;
	static const LinearColor MISTY_ROSE = 0xFFFFE4E1;
	static const LinearColor MOCCASIN = 0xFFFFE4B5;
	static const LinearColor NAVAJO_WHITE = 0xFFFFDEAD;
	static const LinearColor NAVY_BLUE = 0xFF000080;
	static const LinearColor OLD_LACE = 0xFFFDF5E6;
	static const LinearColor OLIVE = 0xFF808000;
	static const LinearColor OLIVE_DRAB = 0xFF6B8E23;
	static const LinearColor ORANGE = 0xFFFFA500;
	static const LinearColor ORANGE_RED = 0xFFFF4500;
	static const LinearColor ORCHID = 0xFFDA70D6;
	static const LinearColor PALE_GOLDENROD = 0xFFEEE8AA;
	static const LinearColor PALE_GREEN = 0xFF98FB98;
	static const LinearColor PALE_TURQUOISE = 0xFFAFEEEE;
	static const LinearColor PALE_VIOLET_RED = 0xFFDB7093;
	static const LinearColor PAPAYA_WHIP = 0xFFFFEFD5;
	static const LinearColor PEACH_PUFF = 0xFFFFDAB9;
	static const LinearColor PERU = 0xFFCD853F;
	static const LinearColor PINK = 0xFFFFC0CB;
	static const LinearColor PLUM = 0xFFDDA0DD;
	static const LinearColor POWDER_BLUE = 0xFFB0E0E6;
	static const LinearColor PURPLE = 0xFFA020F0;
	static const LinearColor WEB_PURPLE = 0xFF800080;
	static const LinearColor REBECCA_PURPLE = 0xFF663399;
	static const LinearColor RED = 0xFFFF0000;
	static const LinearColor ROSY_BROWN = 0xFFBC8F8F;
	static const LinearColor ROYAL_BLUE = 0xFF4169E1;
	static const LinearColor SADDLE_BROWN = 0xFF8B4513;
	static const LinearColor SALMON = 0xFFFA8072;
	static const LinearColor SANDY_BROWN = 0xFFF4A460;
	static const LinearColor SEA_GREEN = 0xFF2E8B57;
	static const LinearColor SEASHELL = 0xFFFFF5EE;
	static const LinearColor SIENNA = 0xFFA0522D;
	static const LinearColor SILVER = 0xFFC0C0C0;
	static const LinearColor SKY_BLUE = 0xFF87CEEB;
	static const LinearColor SLATE_BLUE = 0xFF6A5ACD;
	static const LinearColor SLATE_GRAY = 0xFF708090;
	static const LinearColor SNOW = 0xFFFFFAFA;
	static const LinearColor SPRING_GREEN = 0xFF00FF7F;
	static const LinearColor STEEL_BLUE = 0xFF4682B4;
	static const LinearColor TAN = 0xFFD2B48C;
	static const LinearColor TEAL = 0xFF008080;
	static const LinearColor THISTLE = 0xFFD8BFD8;
	static const LinearColor TOMATO = 0xFFFF6347;
	static const LinearColor TURQUOISE = 0xFF40E0D0;
	static const LinearColor VIOLET = 0xFFEE82EE;
	static const LinearColor WHEAT = 0xFFF5DEB3;
	static const LinearColor WHITE = 0xFFFFFFFF;
	static const LinearColor WHITE_SMOKE = 0xFFF5F5F5;
	static const LinearColor YELLOW = 0xFFFFFF00;
	static const LinearColor YELLOW_GREEN = 0xFF9ACD32;
	
	static const LinearColor DISCORD = LinearColor.Create(88, 101, 242);
	static const LinearColor TWITTER = LinearColor.Create(29, 161, 242);
	static const LinearColor YOUTUBE = LinearColor.Create(255, 0, 0);
	static const LinearColor REDDIT = LinearColor.Create(255, 69, 0);
	
	static LinearColor Create(int r, int g, int b)
	{
		return 255 << 24 | r << 16 | g << 8 | b;
	}
	
	static LinearColor Create(int a, int r, int g, int b)
	{
		return a << 24 | r << 16 | g << 8 | b;
	}
		
	static LinearColor CreateF(float r, float g, float b)
	{
		return 255 << 24 | (int)(r * 255.0) << 16 | (int)(g * 255.0) << 8 | (int)(b * 255.0);
	}
	
	static LinearColor CreateF(float a, float r, float g, float b)
	{
		return (int)(a * 255.0) << 24 | (int)(r * 255.0) << 16 | (int)(g * 255.0) << 8 | (int)(b * 255.0);
	}

	void Unpack(out int a, out int r, out int g, out int b)
	{
		a = GetAlpha();
		r = GetRed();
		g = GetGreen();
		b = GetBlue();
	}
	
	// 0: hue [0, 360]
	// 1: saturation [0, 1.0]
	// 2: value [0, 1.0]
	// 3: alpha [0, 1.0]
	static LinearColor CreateHSV(int hue, float saturation, float value, float alpha = 1.0)
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
	
	static LinearColor Blend(LinearColor a, LinearColor b, BlendMode blend)
	{
		switch (blend)
		{
			case BlendMode.NORMAL: {
				return b;
			}
			case BlendMode.MULTIPLY: {
				return a * b;
			}
			case BlendMode.SCREEN: {
				return 1 - (1 - a) * (1 - b);
			}
			case BlendMode.OVERLAY: {
				if (a < 0.5) {
					return 2 * a * b;
				}
				return 1 - 2 * (1 - a) * (1 - b);
			}
			case BlendMode.HARD_LIGHT: {
				if (b < 0.5) {
					return 2 * a * b;
				}
				return 1 - 2 * (1 - a) * (1 - b);
			}
			case BlendMode.SOFT_LIGHT: {
				if (b < 0.5) {
					return 2 * a * b + a * a * (1 - 2 * b);
				}
				return 2 * a * (1 - b) + Math.SqrInt(a) * (2 * b - 1);
			}
		}
		return a;
	}

	int GetAlpha()
	{
		return value >> 24 & 0xFF;
	}

	int GetRed()
	{
		return value >> 16 & 0xFF;
	}

	int GetGreen()
	{
		return value >> 8 & 0xFF;
	}

	int GetBlue()
	{
		return value & 0xFF;
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

	// determine perceived brightness of RGB color
	// https://stackoverflow.com/a/596243
	float GetLuminance()
	{
		int r = 0.299 * (GetRed() * GetRed());
		int g = 0.587 * (GetGreen() * GetGreen());
		int b = 0.144 * (GetBlue() * GetBlue());
		return Math.Sqrt(r + g + b);
	}

	string ToHex()
	{
		return Encoding.ToHex(value, 8);
	}
	
	vector ToVector()
	{
		return Vector(GetRed(), GetGreen(), GetBlue());
	}
}

typedef int LinearColor;