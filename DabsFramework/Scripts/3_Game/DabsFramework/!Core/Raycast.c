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
		Shape cylinder = Shape.CreateCylinder(color, ShapeFlags.ONCE | ShapeFlags.ADDITIVE, vector.Zero, 0.5, 0.01);
		vector perpend = Bounce.Direction.Perpend();
		if (perpend.Length() < 0.5) {
			perpend = Bounce.Direction * vector.Aside;
		}
		
		vector dir_and_up[4];
		Math3D.DirectionAndUpMatrix(perpend, Bounce.Direction, dir_and_up);
		dir_and_up[3] = Bounce.Position;
		cylinder.SetMatrix(dir_and_up);
		
		Bounce.Debug(color);
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