

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
			return Math.FMod(60 * ((g - b) / diff) + 360, 360);
		}
 
        // if cmax equal g then compute h
        else if (cmax == g) {
			return Math.FMod(60 * ((b - r) / diff) + 120, 360);
		}
 
        // if cmax equal b then compute h
        else if (cmax == b) {
			return Math.FMod(60 * ((r - g) / diff) + 240, 360);
		}
		
		return 0;
	}
}