class Ray: Managed
{
	vector Position;
	vector Direction;
	
	void Ray(vector position = vector.Zero, vector direction = vector.Zero)
	{
		Position = position;
		Direction = direction;
	}
	
	vector GetPoint(float distance)
	{
		return Position + Direction * distance;
	}
	
	Raycast PerformRaycast(Object ignore = null, float distance = 1000.0)
	{
		Ray output_ray = new Ray();
		
		Raycast raycast = new Raycast();
		raycast.Source = this;
		raycast.Bounce = output_ray;
		raycast.Distance = distance;
		float fraction;
		if (!DayZPhysics.RayCastBullet(Position, Position + Direction.Normalized() * distance, PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.CHARACTER | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR, ignore, raycast.Hit, output_ray.Position, output_ray.Direction, fraction)) {			
			output_ray.Position = Position + Direction.Normalized() * distance;
			output_ray.Direction = Direction;
		}
		
		return raycast;
	}
	
	Raycast PerformRaycastRV(Object ignore = null, Object with = null, float radius = 0.0, float distance = 1000.0)
	{
		Ray output_ray = new Ray();
		
		set<Object> rv_results = new set<Object>();
		int hit_component;
		
		Raycast raycast = new Raycast();
		raycast.Source = this;
		raycast.Bounce = output_ray;		
		raycast.Distance = distance;
		if (!DayZPhysics.RaycastRV(Position, Position + Direction.Normalized() * distance, output_ray.Position, output_ray.Direction, hit_component, rv_results, with, ignore, false, false, ObjIntersectView, radius)) {
			output_ray.Position = Position + Direction.Normalized() * distance;
			output_ray.Direction = Direction;
		}
		
		raycast.HitComponent = hit_component;
		raycast.Hit = rv_results[0];
		return raycast;
	}
	
	void Debug()
	{
		Shape.CreateArrow(Position, GetPoint(0.5), 1.0, COLOR_BLUE, ShapeFlags.ONCE);
	}
}