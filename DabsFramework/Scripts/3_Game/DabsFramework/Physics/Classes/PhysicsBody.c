class PhysicsBody: Managed
{
	protected IEntity m_Body;

	vector m_LinearVelocity;
	vector m_AngularVelocity;

	vector m_DeltaLinearVelocity;
	vector m_DeltaAngularVelocity;

	vector m_Transform[4];
	vector m_InvTensorWorld[3];

	float m_Mass;
	float m_InvMass;

	vector m_CenterOfMass;
	vector m_CenterOfMassMS;
	vector m_CenterOfMassOffset;

	int m_NumConstraints;
	PhysicsConstraint m_ContactConstraints[32];
	PhysicsConstraint m_FrictionConstraints[32];

	void PhysicsBody(IEntity body)
	{
		m_Body = body;
	}
		
	void AdjustPointForCenterOfMass(inout vector position)
	{
		position = position + m_CenterOfMassOffset;
	}

	void GetVelocityInLocalPoint(vector position, out vector velocity)
	{		
		velocity = m_LinearVelocity + (m_AngularVelocity * position);
	}

	void GetAngularComponent(vector axis, out vector component)
	{
		component = axis.InvMultiply3(m_InvTensorWorld);
		//component = axis.Multiply3(m_InvTensorWorld);
	}

	void ApplyImpulse(vector linear, vector angular, float impulse)
	{
		m_DeltaLinearVelocity = m_DeltaLinearVelocity + (linear * impulse);
		m_DeltaAngularVelocity = m_DeltaAngularVelocity + (angular * impulse);
	}

	void PreSimulate(float dt)
	{

	}

	void Simulate(float dt)
	{

	}

	void PostSimulate(float dt)
	{

	}

	void OnComputeAndSimulate(float dt)
	{
		PreSimulate(dt);

		m_Body.GetTransform(m_Transform);

		m_LinearVelocity = GetVelocity(m_Body);
		m_AngularVelocity = dBodyGetAngularVelocity(m_Body);

		m_Mass = dBodyGetMass(m_Body);
		m_InvMass = 1.0 / m_Mass;

		m_CenterOfMassMS = dBodyGetCenterOfMass(m_Body);

		m_CenterOfMass = m_Transform[3] - GetCenterOfMass().Multiply3(m_Transform);

		dBodyGetInvInertiaTensorWorld(m_Body, m_InvTensorWorld);

		Simulate(dt);

		for (int contact = 0; contact < m_NumConstraints; contact++)
		{
			m_ContactConstraints[contact].m_Friction = 1.0;// / m_NumConstraints;
			m_ContactConstraints[contact].ResolveSingleConstraintRowGeneric();
		}

		for (int friction = 0; friction < m_NumConstraints; friction++)
		{
			auto parent = m_FrictionConstraints[friction].m_Parent;
			float appliedImpulse = Math.Max(parent.m_AppliedImpulse, m_Mass);
			
			float frictionVal = Math.AbsFloat(parent.m_Friction * appliedImpulse);
		
			m_FrictionConstraints[friction].m_Friction = 1.0 / m_NumConstraints;
			m_FrictionConstraints[friction].m_LowerLimit = -frictionVal;
			m_FrictionConstraints[friction].m_UpperLimit = frictionVal;

			m_FrictionConstraints[friction].ResolveSingleConstraintRowLowerLimit();
		}

		m_NumConstraints = 0;

		m_LinearVelocity = GetVelocity(m_Body);
		m_AngularVelocity = dBodyGetAngularVelocity(m_Body);
		
		//Print(m_DeltaLinearVelocity);
		//Print(m_DeltaAngularVelocity);

		SetVelocity(m_Body, m_LinearVelocity + m_DeltaLinearVelocity);
		dBodySetAngularVelocity(m_Body, m_AngularVelocity + m_DeltaAngularVelocity);

		m_DeltaLinearVelocity = vector.Zero;
		m_DeltaAngularVelocity = vector.Zero;

		m_LinearVelocity = GetVelocity(m_Body);
		m_AngularVelocity = dBodyGetAngularVelocity(m_Body);

		PostSimulate(dt);
	}
	
	bool IsValid()
	{
		return true;
	}
		
	vector GetCenterOfMass()
	{
		return dBodyGetCenterOfMass(m_Body);
	}
}