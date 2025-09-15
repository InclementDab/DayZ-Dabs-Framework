class Plane: Managed
{	
	vector Position, Normal;
	
	void Plane(vector position = vector.Zero, vector normal = vector.Up)
	{
		Position = position;
		Normal = normal;
	}
			
	void ToMatrix(out vector mat[4])
	{
		vector aside = GetAside();
		mat = {
			aside,
			Normal,
			aside * Normal,
			Position
		};
				
		Math3D.MatrixOrthogonalize4(mat);
	}
	
	vector GetAside()
	{
		vector normal = Normal.Normalized();
		if (Math.AbsFloat(vector.Dot(vector.Up, normal)) - 1.0 <= Math.EPSILON) {
			return normal * vector.Aside;
		} else {
			return normal * vector.Up;
		}
	}
		
	vector Intersect(notnull Ray source)
    {
		vector intersection = vector.Zero;
        IntersectEx(source, intersection);
		return intersection;
    }
	
	bool IntersectEx(notnull Ray source, out vector intersection)
	{
		float d1 = vector.Dot(Normal, source.Position - Position);
		float d2 = vector.Dot(Normal, -source.Direction);
		if (d2 == 0) {
			return false;
		}
		
        intersection = source.Position + (d1 / d2) * source.Direction;
		return true;
	}

	void Debug(float size = 1, LinearColor color = 0x1fff7f7f, ShapeFlags flags = ShapeFlags.TRANSP | ShapeFlags.ADDITIVE)
	{
		vector plane_matrix[4];
		ToMatrix(plane_matrix);
		
		Shape.CreateMatrix(plane_matrix);		
		Shape bbox = Shape.Create(ShapeType.BBOX, color, flags, Vector(1, 0, 1) * size, Vector(-1, 0, -1) * size);
		bbox.SetMatrix(plane_matrix);
		Debug.AddShape(bbox, flags);
	}
}