modded class Math
{
	// Modulo for floating point numbers
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