class Raycast: Managed
{
	ref Ray Source;
	ref Ray Bounce;
	
	Object Hit;
	int HitComponent;
	
	float Length()
	{
		if  (!Source || !Bounce) {
			return 0.0;
		}
		
		return vector.Distance(Source.Position, Bounce.Position);
	}
	
	void Debug()
	{		
		Shape.CreateSphere(Color.SKY_BLUE, ShapeFlags.ONCE | ShapeFlags.ADDITIVE, Bounce.Position, 0.2);
		
		Bounce.Debug();
	}
	
	Raycast Continue(Object ignore = null)
	{
		return Ray(Bounce.Position, Source.Direction).PerformRaycast(ignore);
	}
	
	Raycast Redo(Object ignore = null)
	{
		return Source.PerformRaycast(ignore);
	}
}