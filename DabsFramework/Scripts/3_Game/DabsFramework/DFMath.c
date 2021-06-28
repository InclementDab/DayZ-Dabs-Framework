class DFMath
{
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
			return Remainder(60 * ((g - b) / diff) + 360, 360);
		}
 
        // if cmax equal g then compute h
        else if (cmax == g) {
			return Remainder(60 * ((b - r) / diff) + 120, 360);
		}
 
        // if cmax equal b then compute h
        else if (cmax == b) {
			return Remainder(60 * ((r - g) / diff) + 240, 360);
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
	    float x = c * (1 - Math.AbsFloat(Remainder(h / 60, 2) - 1));
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
	
	static float Remainder(float num, float divisor)
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