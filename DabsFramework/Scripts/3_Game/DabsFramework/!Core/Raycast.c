class Raycast: Managed
{
	ref Ray Source;
	ref Ray Bounce;
	float Distance;
	
	Object Hit;
	int HitComponent;
	
	float Length()
	{
		if  (!Source || !Bounce) {
			return 0.0;
		}
		
		return vector.Distance(Source.Position, Bounce.Position);
	}
	
	void Debug(LinearColor color = 0xFF87CEEB)
	{		
		Shape.CreateSphere(color, ShapeFlags.ONCE | ShapeFlags.ADDITIVE, Bounce.Position, 0.2);
		
		Bounce.Debug();
	}
	
	Raycast Continue(Object ignore = null)
	{
		return Ray(Bounce.Position, Source.Direction).PerformRaycast(ignore, Distance - Length());
	}
	
	Raycast Redo(Object ignore = null)
	{
		return Source.PerformRaycast(ignore);
	}
}