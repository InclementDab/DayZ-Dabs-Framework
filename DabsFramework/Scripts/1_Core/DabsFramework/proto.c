// Converts ARGB mask into its counterparts
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

// converts RGB int values to HSV
// rgb values are 0..255
// h: 0 - 360
// s: 0 - 100
// v: 0 - 100
static void RGBtoHSV(int r, int g, int b, out float h, out float s, out float v)
{
	RGBFtoHSV(r / 255, g / 255, b / 255, h, s, v);
}

// Converts RGB float values to HSV (Hue, Saturation, Value)
// rgb values are 0..1
// h: 0 - 360
// s: 0 - 100
// v: 0 - 100
static void RGBFtoHSV(float r, float g, float b, out float h, out float s, out float v) 
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
		h = Math.FMod(60 * ((g - b) / diff) + 360, 360);
	}

    // if cmax equal g then compute h
    else if (cmax == g) {
		h = Math.FMod(60 * ((b - r) / diff) + 120, 360);
	}

       // if cmax equal b then compute h
    else if (cmax == b) {
		h = Math.FMod(60 * ((r - g) / diff) + 240, 360);
	}
	
	if (cmax == 0) {
		s = 0;
	} else {
		s = (diff / cmax) * 100;
	}
	
	v = cmax * 100;
}

static void HSVtoRGBF(float h, float s, float v, out float r, out float g, out float b)
{
	// check for out of bounds
    if (h > 360 || h < 0 || s > 100 || s < 0 || v > 100 || v < 0) {
        return;
    }

	s /= 100;
	v /= 100;
    float c = s * v;
	int vv = h / 60;		
    float x = c * (1 - Math.AbsFloat(Math.FMod(h / 60, 2) - 1));
    float m = v - c;
	
    if (h >= 0 && h < 60) {
       	r = c; g = x; b = 0;
    }
    else if (h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    }
    else if (h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    }
    else if (h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    }
    else if (h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    }
    else {
        r = c; g = 0; b = x;
    }
	
	r += m; g += m; b += m;
}

// h: 0 - 360
// s: 0 - 100
// v: 0 - 100
// a: 0 - 255
static int HSVtoARGB(float h, float s, float v, int a)
{
	float r, g, b;
	HSVtoRGBF(h, s, v, r, g, b);
	
	return ARGBF(a / 255, r, g, b);
}