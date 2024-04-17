modded class Math
{
	static const float EPSILON = 1.19209e-07;
	
	// Modulo for floating point numbers
    static float FMod(float num, float divisor)
	{
		return num - (Floor(num / divisor) * divisor);
	}
	
	static float SmoothLerp(float a, float b, float t)
	{
		return (b - a) * SmoothTime(t) + a;
	}
	
	static float SmoothTime(float t)
	{
		return (-Math.Cos(Math.PI * t) / 2) + 0.5;
	}

	static vector LerpVector(vector p1, vector p2, float time)
	{
		return Vector(Math.Lerp(p1[0], p2[0], time), Math.Lerp(p1[1], p2[1], time), Math.Lerp(p1[2], p2[2], time));
	}
	
	static vector SmoothLerpVector(vector p1, vector p2, float time)
	{
		return Vector(Math.SmoothLerp(p1[0], p2[0], time), Math.SmoothLerp(p1[1], p2[1], time), Math.SmoothLerp(p1[2], p2[2], time));
	}
	
	static float Exp(float v)
	{
		return Pow(EULER, v);
	}
	
	static float Ln(float y)
	{
	    int log2;
	    float divisor, x, result;
	
	    log2 = Msb((int)y); // See: https://stackoverflow.com/a/4970859/6630230
	    int int_divisor = (1 << log2);
		divisor = (float)int_divisor;
	    x = y / divisor;    // normalized value between [1.0, 2.0]
	
	    result = -1.7417939 + (2.8212026 + (-1.4699568 + (0.44717955 - 0.056570851 * x) * x) * x) * x;
	    result += ((float)log2) * 0.69314718; // ln(2) = 0.69314718
	
	    return result;
	}
	
	static int Msb(int v)
	{
		int r = 0;
		while (v) {
		    r++;
			v >>= 1;
		}
		
		return r;
	}
	
	// Works very similar to Clamp except it rolls over instead of clamping
	/* 
		Examples:
	
		int value = 20;
		value = Math.Rollover(value, 0, 15);
		Print(value);
		
		value = 5
	*/
	
	static int Rollover(int value, int min, int max)
	{
		while (value < min) {
			value += max;
		}
		
		return value % max;
	}
}