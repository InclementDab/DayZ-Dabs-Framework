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
	
	Raycast PerformRaycast(Object ignore = null, float distance = 1000.0, PhxInteractionLayers layers = -1)
	{
		Ray output_ray = new Ray();
		
		Raycast raycast = new Raycast();
		raycast.Source = this;
		raycast.Bounce = output_ray;
		raycast.Distance = distance;
		float fraction;
		if (!DayZPhysics.RayCastBullet(Position, Position + Direction.Normalized() * distance, layers, ignore, raycast.Hit, output_ray.Position, output_ray.Direction, fraction)) {			
			return null;
		}
		
		return raycast;
	}
	
	Raycast PerformRaycastRVEX(float radius = 0.0, float distance = 1000.0, int interaction_type = ObjIntersectView, array<Object> excludes = null)
	{
		set<Object> rv_results = new set<Object>();
		int hit_component;
		
		Raycast raycast = new Raycast();
		raycast.Source = this;	
		raycast.Distance = distance;
		vector direction;
		vector position;
		
		RaycastRVParams raycast_params = new RaycastRVParams(Position, Position + Direction.Normalized() * distance);
		raycast_params.radius = radius;
		raycast_params.flags = CollisionFlags.NEARESTCONTACT;
		raycast_params.type = interaction_type;
		raycast_params.sorted = true;
		array<ref RaycastRVResult> results = {};
		if (!DayZPhysics.RaycastRVProxy(raycast_params, results, excludes) || results.Count() == 0) {
			return null;
		}
	
		RaycastRVResult result = results[0];	
		if (!result) {
			return null;
		}
			
		raycast.Bounce = new Ray(result.pos, result.dir);
		raycast.HitComponent = result.component;
		raycast.Hit = result.obj;
		return raycast;
	}
	
	Raycast PerformRaycastRV(Object ignore = null, Object with = null, float radius = 0.0, float distance = 1000.0, int interaction_type = ObjIntersectView)
	{
		set<Object> rv_results = new set<Object>();
		int hit_component;
		
		Raycast raycast = new Raycast();
		raycast.Source = this;	
		raycast.Distance = distance;
		vector direction;
		vector position;
		
		RaycastRVParams raycast_params = new RaycastRVParams(Position, Position + Direction.Normalized() * distance);
		raycast_params.ignore = ignore;
		raycast_params.with = with;
		raycast_params.radius = radius;
		raycast_params.flags = CollisionFlags.NEARESTCONTACT;
		raycast_params.type = interaction_type;
		raycast_params.sorted = true;
		array<ref RaycastRVResult> results = {};
		if (!DayZPhysics.RaycastRVProxy(raycast_params, results, null) || results.Count() == 0) {
			return null;
		}
			
		RaycastRVResult result = results[0];
		raycast.Bounce = new Ray(result.pos, result.dir);
		raycast.HitComponent = result.component;
		raycast.Hit = result.obj;
		return raycast;
	}
	
	void Debug(LinearColor color = -1, ShapeFlags flags = ShapeFlags.ONCE)
	{
		//Shape.CreateArrow(Position, GetPoint(0.5), 1.0, color, flags);
		vector debug_matrix[4];
		vector perpend = Direction.Perpend();
		if (perpend.Length() == 0) {
			perpend = Direction * vector.Aside;
		}
		
		Math3D.DirectionAndUpMatrix(perpend, Direction.Normalized(), debug_matrix);
		debug_matrix[3] = Position;
		
		vector cylinder_start_position = vector.Up * Direction.Length() * 0.5;
		Shape cylinder = Shape.CreateCylinder(color, flags, cylinder_start_position, 0.05, Direction.Length());
		cylinder.SetMatrix(debug_matrix);
		
		float cylinder_current_position = Direction.Length() / 2;
		int arrowhead_iterations = 32;
		for (int i = 0; i < arrowhead_iterations; i++) {
			float width = ((arrowhead_iterations - i) / arrowhead_iterations) * Direction.Length() / 4;
			cylinder = Shape.CreateCylinder(color, flags, cylinder_start_position + vector.Up * cylinder_current_position, width, 0.05);
			cylinder.SetMatrix(debug_matrix);
			cylinder_current_position += Direction.Length() / (arrowhead_iterations * 2);
		}
	}
}