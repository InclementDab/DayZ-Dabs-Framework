class PositionWithRadius2D: Managed
{
	static const ref PositionWithRadius2D INVALID = new PositionWithRadius2D(vector.Zero, -1);
	
	vector Position;
	float Radius;
	
	void PositionWithRadius2D(vector position, float radius)
	{
		Position = position;
		Radius = radius;
	}
	
	vector GetRandomPointInside(bool snap_y_to_surface = false)
	{
		float x = Position[0] + Math.Cos(Math.RandomFloat01() * Math.PI2) * Radius;
		float z = Position[2] + Math.Sin(Math.RandomFloat01() * Math.PI2) * Radius;
		float y = Position[1];
		if (snap_y_to_surface) {
			if (GetGame().SurfaceIsSea(x, z)) {
				y = GetGame().SurfaceGetSeaLevel();
			} else {
				y = GetGame().SurfaceY(x, z);
			}
		}
		
		return Vector(x, y, z);
	}
	
	vector GetRandomPointOnEdge(bool snap_y_to_surface = false)
	{
		for (int i = 0; i < 5; i++) {
			float random = Math.RandomFloat01();
			float x = Position[0] + Math.Cos(random * Math.PI2) * Radius;
			float z = Position[2] + Math.Sin((1 - random) * Math.PI2) * Radius;
			float y = Position[1];
			if (snap_y_to_surface) {
				if (GetGame().SurfaceIsSea(x, z)) {
					y = GetGame().SurfaceGetSeaLevel();
				} else {
					y = GetGame().SurfaceY(x, z);
				}
			}

			array<Object> excludedObjects = new array<Object>;
			array<Object> nearbyObjects = new array<Object>;
			if (GetGame().IsBoxColliding(Vector(x, y, z), "0 0 0", "3 5 3", excludedObjects, nearbyObjects)) {
				continue;
			}

			break;
		}
		
		return Vector(x, y, z);
	}
	
	bool IsInside(vector position)
	{
		return (vector.Distance(Position, position) < Radius);
	}
	
	static PositionWithRadius2D CalculateMean(notnull array<ref PositionWithRadius2D> positions)
	{
		if (positions.Count() == 0) {
			return INVALID;
		}
		
		// sum positions
		PositionWithRadius2D result = new PositionWithRadius2D(vector.Zero, 0);
		foreach (PositionWithRadius2D position: positions) {
			result.Position += position.Position;
		}
		
		// acquire the mean position
		for (int i = 0; i < 3; i++) {
			result.Position[i] = result.Position[i] / positions.Count();
		}
		
		// find radius that is furthest from center, this will be our assigned radius
		foreach (PositionWithRadius2D position_radius_check: positions) {
			float distance = vector.Distance(position_radius_check.Position, result.Position);
			if (distance + position_radius_check.Radius > result.Radius) {
				result.Radius = distance + position_radius_check.Radius;
			}
		}
		
		return result;
	}
	
		
#ifdef DIAG_DEVELOPER
	void DrawDebug(int color = 0x1fff7f7f)
	{
		Debug.DrawSphere(Position, Radius, color, ShapeFlags.TRANSP | ShapeFlags.ADDITIVE);
		Debug.DrawSphere(Position, 0.25, color);
	}
#endif
}