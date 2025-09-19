class Raycast: Managed
{
	ref Ray Source;
	ref Ray Bounce;
	float Distance;
	
	Object Hit;
	int HitComponent;
	
#ifdef DIAG_DEVELOPER
	float Radius;
#endif
	
	float Length()
	{
		if  (!Source || !Bounce) {
			return 0.0;
		}
		
		return vector.Distance(Source.Position, Bounce.Position);
	}
	
	void Debug(LinearColor color = 0xFF87CEEB, ShapeFlags flags = 104/*ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.NOOUTLINE*/)
	{	
#ifdef DIAG_DEVELOPER
		vector perpend = Source.Direction.Perpend();
		if (perpend.Length() < 0.5) {
			perpend = Source.Direction * vector.Aside;
		}
		
		float length = vector.Distance(Bounce.Position, Source.Position);
		vector dir_and_up[4];
		Math3D.DirectionAndUpMatrix(perpend, Source.Direction, dir_and_up);
		dir_and_up[3] = Source.Position + Source.Direction * length * 0.5;
		
		Source.Debug(length / 2, LinearColor.BLUE, flags);
		Bounce.Debug(length / 2, LinearColor.GREEN, flags);
		
		Shape shape;// = Shape.CreateSphere(LinearColor.BLUE, flags, Bounce.Position, 0.1);
		//Debug.AddShape(shape, flags);
		//shape = Shape.CreateSphere(LinearColor.RED, flags, Source.Position, 0.1);
		//Debug.AddShape(shape, flags);
		
		shape = Shape.CreateCylinder(color, flags, vector.Zero, Radius, length);
		shape.SetMatrix(dir_and_up);
		Debug.AddShape(shape, flags);
		
		if (Hit) {
			shape = Shape.CreateSphere(LinearColor.BLUE, flags | ShapeFlags.NOZBUFFER, Bounce.Position, Radius / 2);
			Debug.AddShape(shape, flags);
		}
#endif
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