// todo proto.c
void InverseARGB(int argb, out int a, out int r, out int g, out int b)
{
	b = argb & 255;
	argb >>= 8;
	g = argb & 255;
	argb >>= 8;
	r = argb & 255;
	argb >>= 8;
	a = argb & 255;
}

void InverseARGBF(int argb, out float a, out float r, out float g, out float b)
{
	int ai, ri, gi, bi;
	InverseARGB(argb, ai, ri, gi, bi);
	a = ai / 255; 
	r = ri / 255; 
	g = gi / 255; 
	b = bi / 255;
}

class DFMath
{
	static void RGBtoHSV(float r, float g, float b, out float h, out float s, out float v) 
	{
		// h, s, v = hue, saturation, value
        float cmax = Math.Max(r, Math.Max(g, b)); // maximum of r, g, b
        float cmin = Math.Min(r, Math.Min(g, b)); // minimum of r, g, b
        float diff = cmax - cmin; // diff of cmax and cmin.
         
        // if cmax and cmax are equal then h = 0
        if (cmax == cmin) {
            h = 0;
		}
 
        // if cmax equal r then compute h
        else if (cmax == r) {
			h = FMod(60 * ((g - b) / diff) + 360, 360);
		}
 
        // if cmax equal g then compute h
        else if (cmax == g) {
			h = FMod(60 * ((b - r) / diff) + 120, 360);
		}
 
        // if cmax equal b then compute h
        else if (cmax == b) {
			h = FMod(60 * ((r - g) / diff) + 240, 360);
		}
		
		if (cmax == 0) {
			s = 0;
		} else {
			s = (diff / cmax) * 100;
		}
		
		v = cmax * 100;
	}
	
    static float RGBtoHue(float r, float b, float g)
	{ 
        // h, s, v = hue, saturation, value
        float cmax = Math.Max(r, Math.Max(g, b)); // maximum of r, g, b
        float cmin = Math.Min(r, Math.Min(g, b)); // minimum of r, g, b
        float diff = cmax - cmin; // diff of cmax and cmin.
        float h = -1, s = -1;
         
        // if cmax and cmax are equal then h = 0
        if (cmax == cmin) {
            return 0;
		}
 
        // if cmax equal r then compute h
        else if (cmax == r) {
			return FMod(60 * ((g - b) / diff) + 360, 360);
		}
 
        // if cmax equal g then compute h
        else if (cmax == g) {
			return FMod(60 * ((b - r) / diff) + 120, 360);
		}
 
        // if cmax equal b then compute h
        else if (cmax == b) {
			return FMod(60 * ((r - g) / diff) + 240, 360);
		}
		
		return 0;
	}
	
	static void HSVtoRGB(float h, float s, float v, out float rgb[3])
	{
	    if (h > 360 || h < 0 || s > 100 || s < 0 || v > 100 || v < 0) {
	        return;
	    }

		s /= 100;
		v /= 100;
	    float c = s * v;
		int vv = h / 60;		
	    float x = c * (1 - Math.AbsFloat(FMod(h / 60, 2) - 1));
	    float m = v - c;
		
	    if (h >= 0 && h < 60) {
	       	rgb[0] = c; rgb[1] = x; rgb[2] = 0;
	    }
	    else if (h >= 60 && h < 120) {
	        rgb[0] = x; rgb[1] = c; rgb[2] = 0;
	    }
	    else if (h >= 120 && h < 180) {
	        rgb[0] = 0; rgb[1] = c; rgb[2] = x;
	    }
	    else if (h >= 180 && h < 240) {
	        rgb[0] = 0; rgb[1] = x; rgb[2] = c;
	    }
	    else if (h >= 240 && h < 300) {
	        rgb[0] = x; rgb[1] = 0; rgb[2] = c;
	    }
	    else {
	        rgb[0] = c; rgb[1] = 0; rgb[2] = x;
	    }
		
		for (int i = 0; i < 3; i++) {
			rgb[i] = (rgb[i] + m);
		}
	}
	
	static float FMod(float num, float divisor)
	{
		return num - ((int)(num / divisor) * divisor);
	}
	
	static float SmoothLerp(float a, float b, float t)
	{
		return (b - a) * SmoothTime(t) + a;
	}
	
	static float SmoothTime(float t)
	{
		return (-Math.Cos(Math.PI * t) / 2) + 0.5;
	}
}