modded class Debug
{	
	static const int RPC_UPDATE_DEBUG_SHAPE = 26893386;
	
	static void OnDebugShapeRPC(ParamsReadContext ctx)
	{
		if (!GetGame().IsClient() || !GetGame().IsMultiplayer()) {
			return;
		}
		
		int shape_type;
		if (!ctx.Read(shape_type)) {
			Error("Invalid Shape Type");
			return;
		}
		
		vector v1, v2;
		int color, flags;
		float size, radius, height;
		
		switch (shape_type) {			
			case ShapeType.LINE: {
				ctx.Read(v1);
				ctx.Read(v2);
				ctx.Read(color);
				ctx.Read(flags);
				DrawLine(v1, v2, color, flags);
				break;
			}
			
			case ShapeType.SPHERE: {
				ctx.Read(v1);
				ctx.Read(size);
				ctx.Read(color);
				ctx.Read(flags);
				DrawSphere(v1, size, color, flags);
				break;
			}
			
			case ShapeType.CYLINDER: {
				ctx.Read(v1);
				ctx.Read(radius);
				ctx.Read(height);
				ctx.Read(color);
				ctx.Read(flags);
				DrawCylinder(v1, radius, height, color, flags);
				break;
			}
						
			case 7: {
				ctx.Read(v1);
				ctx.Read(v2);
				ctx.Read(size);
				ctx.Read(color);
				ctx.Read(flags);
				DrawArrow(v1, v2, size, color, flags);
				break;
			}
			
			case 8: {
				DestroyAllShapes();
			}
			
			default: {
				ctx.Read(v1);
				ctx.Read(v2);
				ctx.Read(color);
				ctx.Read(flags);
				m_DebugShapes.Insert(Shape.Create(shape_type, color, flags, v1, v2));
			}
		}
	}
	
	static override Shape DrawBox(vector pos1, vector pos2, int color = 0x1fff7f7f)
	{
		return DrawBoxEx(pos1, pos2, color, ShapeFlags.TRANSP|ShapeFlags.NOZWRITE);
	}
	
	static override Shape DrawBoxEx(vector pos1, vector pos2, int color = 0x1fff7f7f, ShapeFlags flags = ShapeFlags.TRANSP|ShapeFlags.NOZWRITE)
	{
		if (GetGame().IsDedicatedServer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(ShapeType.BBOX);
			rpc.Write(pos1);
			rpc.Write(pos2);
			rpc.Write(color);
			rpc.Write(flags);
			rpc.Send(null, RPC_UPDATE_DEBUG_SHAPE, false);
			return null;
		}
		
		Shape shape = Shape.Create(ShapeType.BBOX, color, flags, pos1, pos2);
		if (( flags & ShapeFlags.ONCE ) == 0)
			m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static override Shape DrawCube(vector pos, float size = 1, int color = 0x1fff7f7f)
	{		
		vector min = pos;
		vector max = pos;
		
		float size_h = size * 0.5;
	
		min[0] = min[0] - size_h;
		min[1] = min[1] - size_h;
		min[2] = min[2] - size_h;
		
		max[0] = max[0] + size_h;
		max[1] = max[1] + size_h;
		max[2] = max[2] + size_h;
		
		if (GetGame().IsDedicatedServer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(ShapeType.DIAMOND);
			rpc.Write(min);
			rpc.Write(max);
			rpc.Write(color);
			rpc.Write(ShapeFlags.TRANSP|ShapeFlags.NOZWRITE);
			rpc.Send(null, RPC_UPDATE_DEBUG_SHAPE, false);
			return null;
		}
		
		Shape shape = Shape.Create(ShapeType.DIAMOND, color, ShapeFlags.TRANSP|ShapeFlags.NOZWRITE, min, max);
		m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static override Shape DrawSphere(vector pos, float size = 1, int color = 0x1fff7f7f, ShapeFlags flags = ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE)
	{
		if (GetGame().IsDedicatedServer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(ShapeType.SPHERE);
			rpc.Write(pos);
			rpc.Write(size);
			rpc.Write(color);
			rpc.Write(flags);
			rpc.Send(null, RPC_UPDATE_DEBUG_SHAPE, false);
			return null;
		}
		
		Shape shape = Shape.CreateSphere(color, flags, pos, size);
		if (( flags & ShapeFlags.ONCE ) == 0)
			m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static override Shape DrawCylinder(vector pos, float radius, float height = 1, int color = 0x1fff7f7f, ShapeFlags flags = ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE )
	{
		if (GetGame().IsDedicatedServer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(ShapeType.CYLINDER);
			rpc.Write(pos);
			rpc.Write(radius);
			rpc.Write(height);
			rpc.Write(color);
			rpc.Write(flags);
			rpc.Send(null, RPC_UPDATE_DEBUG_SHAPE, false);
			return null;
		}
		
		Shape shape = Shape.CreateCylinder(color, flags, pos, radius, height);
		if (( flags & ShapeFlags.ONCE ) == 0)
			m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static override array<Shape> DrawCone(vector pos, float lenght, float halfAngle, float offsetAngle, int color = 0xFFFFFFFF, int flags = 0)
	{
		array<Shape> shapes = new array<Shape>;
		
		vector endL, endR;
		Math3D.ConePoints(pos, lenght, halfAngle, offsetAngle, endL, endR);

		// Left side
		shapes.Insert( Debug.DrawLine(pos, endL, color, flags) );
		// Rigth side
		shapes.Insert( Debug.DrawLine(pos, endR, color, flags) );
		// Top side
		shapes.Insert( Debug.DrawLine(endL, endR, color, flags) );
		// Middle (height) line		
		shapes.Insert( Debug.DrawLine(pos, pos + Vector(Math.Cos(offsetAngle), 0, Math.Sin(offsetAngle)).Normalized() * lenght, color, flags) );
		
		return shapes;
	}
	
	static override Shape DrawLine(vector from, vector to, int color = 0xFFFFFFFF, int flags = 0)
	{
		if (GetGame().IsDedicatedServer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(ShapeType.LINE);
			rpc.Write(from);
			rpc.Write(to);
			rpc.Write(color);
			rpc.Write(flags);
			rpc.Send(null, RPC_UPDATE_DEBUG_SHAPE, false);
			return null;
		}
		
		vector pts[2]
		pts[0] = from;
		pts[1] = to;
		
		Shape shape = Shape.CreateLines(color, flags, pts, 2);
		if (( flags & ShapeFlags.ONCE ) == 0)
			m_DebugShapes.Insert(shape);
		//m_DebugShapes.Debug();
		return shape;
	}

	static override Shape DrawLines(vector[] positions, int count, int color = 0xFFFFFFFF, int flags = 0)
	{
		if (GetGame().IsDedicatedServer()) {
			for (int i = 0; i < count; i++) {
				ScriptRPC rpc = new ScriptRPC();
				rpc.Write(ShapeType.LINE);
				rpc.Write(positions[i]);
				rpc.Write(positions[i + 1]);
				rpc.Write(color);
				rpc.Write(flags);
				rpc.Send(null, RPC_UPDATE_DEBUG_SHAPE, false);
				i++; // extra i++ bc i cant do i += 2
			}
			
			return null;
		}
		
		
		Shape shape = Shape.CreateLines(color, flags, positions, count);
		if (( flags & ShapeFlags.ONCE ) == 0)
			m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static override Shape DrawArrow(vector from, vector to, float size = 0.5, int color = 0xFFFFFFFF, int flags = 0)
	{
		if (GetGame().IsDedicatedServer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(7); // imaginary shapetype
			rpc.Write(from);
			rpc.Write(to);
			rpc.Write(size);
			rpc.Write(color);
			rpc.Write(flags);
			rpc.Send(null, RPC_UPDATE_DEBUG_SHAPE, false);		
			return null;
		}
		
		Shape shape = Shape.CreateArrow(from, to, size, color, flags);
		m_DebugShapes.Insert(shape);
		return shape;
	}
	
	static override void DestroyAllShapes()
	{
		if (GetGame().IsDedicatedServer()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(8); // 8 means destroy
			rpc.Send(null, RPC_UPDATE_DEBUG_SHAPE, false);		
			return;
		}
		
		for (int i = 0; i < m_DebugShapes.Count(); ++i) {
			if (m_DebugShapes[i]) {
				m_DebugShapes[i].Destroy();
			}
		}
		
		m_DebugShapes.Clear();
	}
}