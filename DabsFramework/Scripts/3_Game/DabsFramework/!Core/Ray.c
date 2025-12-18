class Ray: Managed
{
	vector Position;
	vector Direction;
	
	void Ray(vector position = vector.Zero, vector direction = vector.Zero)
	{
		Position = position;
		Direction = direction;
	}
	
	static Ray FromPoints(vector start, vector end)
	{
		return new Ray(start, (end - start).Normalized());
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
#ifdef DIAG_DEVELOPER
		raycast.Radius = 0.01; // Bullet has no radius. Arbitrary
#endif	
		float fraction;
		if (!DayZPhysics.RayCastBullet(Position, Position + Direction.Normalized() * distance, layers, ignore, raycast.Hit, output_ray.Position, output_ray.Direction, fraction)) {			
			return null;
		}
		
		return raycast;
	}
	
	// Raycast bullet that supports multiple ignores
	Raycast PerformRaycastMulti(array<Object> ignores = null, float distance = 1000.0, PhxInteractionLayers layers = -1)
	{
		Ray output_ray = new Ray();
		
		Raycast raycast = new Raycast();
		raycast.Source = this;
		raycast.Bounce = output_ray;
		raycast.Distance = distance;
#ifdef DIAG_DEVELOPER
		raycast.Radius = 0.01; // Bullet has no radius. Arbitrary
#endif	
		float fraction;

		vector position = Position;
		Object ignore = null;
		while (DayZPhysics.RayCastBullet(position, position + Direction.Normalized() * distance, layers, ignore, raycast.Hit, output_ray.Position, output_ray.Direction, fraction)) {
			if (!ignores || ignores.Find(raycast.Hit) == -1) {
				return raycast;
			}
			
			distance -= vector.Distance(position, output_ray.Position);
			
			// incase we've somehow reached the end of the rainbow
			if (distance <= Math.EPSILON) {
				return raycast;
			}
			
			ignore = raycast.Hit;
			position = output_ray.Position;
		}
		
		return null;
	}
	
	Raycast PerformRaycastSphere(float radius, Object ignore = null, float distance = 1000.0, PhxInteractionLayers layers = -1)
	{
		Ray output_ray = new Ray();
		
		Raycast raycast = new Raycast();
		raycast.Source = this;
		raycast.Bounce = output_ray;
		raycast.Distance = distance;
#ifdef DIAG_DEVELOPER
		raycast.Radius = radius;
#endif	
		
		float fraction;
		if (!DayZPhysics.SphereCastBullet(Position, Position + Direction.Normalized() * distance, radius, layers, ignore, raycast.Hit, output_ray.Position, output_ray.Direction, fraction)) {
			return null;
		}
		
		return raycast;
	}
	
	Raycast PerformRaycastRVEX(float radius = 0.0, float distance = 1000.0, int interaction_type = ObjIntersectView, array<Object> excludes = null, bool ground_only = false)
	{
		set<Object> rv_results = new set<Object>();
		int hit_component;
		
		Raycast raycast = new Raycast();
		raycast.Source = this;	
		raycast.Distance = distance;
#ifdef DIAG_DEVELOPER
		raycast.Radius = radius;
#endif	
		
		vector direction;
		vector position;
		
		RaycastRVParams raycast_params = new RaycastRVParams(Position, Position + Direction.Normalized() * distance);
		raycast_params.radius = radius;
		raycast_params.flags = CollisionFlags.NEARESTCONTACT;
		raycast_params.type = interaction_type;
		raycast_params.groundOnly = ground_only;
		raycast_params.sorted = true;
		array<ref RaycastRVResult> results = {};
		if (!DayZPhysics.RaycastRVProxy(raycast_params, results, excludes) || results.Count() == 0) {
			return null;
		}
	
		RaycastRVResult result = results[0];	
		if (!result) {
			return null;
		}
			
		raycast.Bounce = new Ray(result.pos, result.dir.Normalized());
		raycast.HitComponent = result.component;
		raycast.Hit = result.obj;
		return raycast;
	}
	
	Raycast PerformRaycastRV(Object ignore = null, Object with = null, float radius = 0.0, float distance = 1000.0, int interaction_type = ObjIntersectView, bool ground_only = false)
	{		
		Raycast raycast = new Raycast();
		raycast.Source = this;	
		raycast.Distance = distance;
#ifdef DIAG_DEVELOPER
		raycast.Radius = radius;
#endif
				
		RaycastRVParams raycast_params = new RaycastRVParams(Position, Position + Direction.Normalized() * distance);
		raycast_params.ignore = ignore;
		raycast_params.with = with;
		raycast_params.radius = radius;
		raycast_params.flags = CollisionFlags.NEARESTCONTACT;
		raycast_params.type = interaction_type;
		raycast_params.sorted = true;
		raycast_params.groundOnly = ground_only;
		array<ref RaycastRVResult> results = {};
		if (!DayZPhysics.RaycastRVProxy(raycast_params, results, null) || results.Count() == 0) {
			return null;
		}
		
#ifdef DIAG_DEVELOPER
		if (results.Count() > 1) {
			debug;
		}
#endif
				
		RaycastRVResult result = results[0];
		raycast.Bounce = new Ray(result.pos, result.dir.Normalized());
		raycast.HitComponent = result.component;
		raycast.Hit = result.obj;
		return raycast;
	}
	
	void Debug(float length = 1, LinearColor color = -1, ShapeFlags flags = 104/*ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.NOOUTLINE*/)
	{
		//Shape.CreateArrow(Position, GetPoint(0.5), 1.0, color, flags);		
		//Debug.DrawArrow(, 0.5, color, flags);
#ifdef DIAG_DEVELOPER
		vector camera_direction = GetGame().GetCurrentCameraDirection();
		
		vector perpend;
		if (Math.AbsFloat(vector.Dot(Direction, camera_direction)) > 0.999) {
			perpend = Direction * (camera_direction * vector.Aside);
		} else {
			perpend = Direction * camera_direction;
		}
		
		perpend.Normalize();
		
		vector end = GetPoint(length);
		vector lines[3] = {
			Position, end
		};
		
		Shape shape = Shape.CreateLines(color, flags, lines, 2);
		Debug.AddShape(shape, flags);
		
		lines = {
			Position + perpend * 0.1 * length, Position - perpend * 0.1 * length
		};
		
		shape = Shape.CreateLines(color, flags, lines, 2);
		Debug.AddShape(shape, flags);
		
		lines = {
			end + perpend * length * 0.1 - Direction * length * 0.1, end, end - perpend * length * 0.1 - Direction * length * 0.1
		};
		
		shape = Shape.CreateLines(color, flags, lines, 3);
		Debug.AddShape(shape, flags);
#endif
		
		/*	
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
		}*/
	}
}