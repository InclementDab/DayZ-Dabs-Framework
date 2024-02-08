class PhysicsConstraint: Managed
{
	PhysicsBody m_BodyA;
	PhysicsBody m_BodyB;
	
	PhysicsConstraint m_Parent;
	
	vector m_ContactNormalA;
	vector m_ContactNormalB;

	vector m_AngularA;
	vector m_AngularB;

	vector m_RelPosACrossNormal;
	vector m_RelPosBCrossNormal;

	float m_AppliedImpulse;
	
	float m_Friction;

	float m_LowerLimit;
	float m_UpperLimit;

	float m_CFM;

	float m_RHS;
	float m_RHSPenetration;

	float m_jacDiagABInv;

	void PhysicsConstraint(PhysicsBody bodyA, PhysicsBody bodyB, vector relativePosA, vector relativePosB, vector normal, float relaxation, float cfm)
	{
		m_BodyA = bodyA;
		m_BodyB = bodyB;
		
		m_RelPosACrossNormal = (relativePosA * normal);
		m_RelPosBCrossNormal = -(relativePosB * normal);

		m_BodyA.GetAngularComponent(m_RelPosACrossNormal, m_AngularA);
		m_BodyB.GetAngularComponent(m_RelPosBCrossNormal, m_AngularB);

		float denomA = 0.0;
		float denomB = 0.0;

		if (m_BodyA.IsValid())
		{
			denomA = m_BodyA.m_InvMass + vector.Dot(normal, m_AngularA * relativePosA);
			
			m_ContactNormalA = normal;
		}
		else
		{
			m_ContactNormalA = vector.Zero;
		}

		if (m_BodyB.IsValid())
		{
			denomB = m_BodyB.m_InvMass + vector.Dot(normal, m_AngularB * relativePosB);
			
			m_ContactNormalB = -normal;
		}
		else
		{
			m_RelPosBCrossNormal = vector.Zero;
			m_ContactNormalB = vector.Zero;
		}

		m_jacDiagABInv = relaxation / (denomA + denomB + cfm);
		m_CFM = cfm * m_jacDiagABInv;
	}

	float ResolveSingleConstraintRowGeneric()
	{
		//float rhs = m_RHS + (m_RHSPenetration * m_Friction);
		float rhs = (m_RHS + m_RHSPenetration) * m_Friction;
		
		float deltaImpulse = rhs - (m_AppliedImpulse * m_CFM);
		float deltaVel1Dotn = vector.Dot(m_ContactNormalA, m_BodyA.m_LinearVelocity) + vector.Dot(m_RelPosACrossNormal, m_BodyA.m_AngularVelocity);
		float deltaVel2Dotn = vector.Dot(m_ContactNormalB, m_BodyB.m_LinearVelocity) + vector.Dot(m_RelPosBCrossNormal, m_BodyB.m_AngularVelocity);

		deltaImpulse -= deltaVel1Dotn * m_jacDiagABInv;
		deltaImpulse -= deltaVel2Dotn * m_jacDiagABInv;

		float sum = m_AppliedImpulse + deltaImpulse;
		if (sum < m_LowerLimit)
		{
			deltaImpulse = m_LowerLimit - m_AppliedImpulse;
			m_AppliedImpulse = m_LowerLimit;
		}
		else if (sum > m_UpperLimit)
		{
			deltaImpulse = m_UpperLimit - m_AppliedImpulse;
			m_AppliedImpulse = m_UpperLimit;
		}
		else
		{
			m_AppliedImpulse = sum;
		}
		
		deltaImpulse *= 1.0 / m_BodyA.m_NumConstraints;

		m_BodyA.ApplyImpulse(m_ContactNormalA * m_BodyA.m_InvMass, m_AngularA, deltaImpulse);
		m_BodyB.ApplyImpulse(m_ContactNormalB * m_BodyB.m_InvMass, m_AngularB, deltaImpulse);

		return deltaImpulse * (1.0 / m_jacDiagABInv);
	}

	float ResolveSingleConstraintRowLowerLimit()
	{
		float rhs = m_RHS + (m_RHSPenetration * m_Friction);
		
		float deltaImpulse = rhs - (m_AppliedImpulse * m_CFM);
		float deltaVel1Dotn = vector.Dot(m_ContactNormalA, m_BodyA.m_LinearVelocity) + vector.Dot(m_RelPosACrossNormal, m_BodyA.m_AngularVelocity);
		float deltaVel2Dotn = vector.Dot(m_ContactNormalB, m_BodyB.m_LinearVelocity) + vector.Dot(m_RelPosBCrossNormal, m_BodyB.m_AngularVelocity);

		deltaImpulse -= deltaVel1Dotn * m_jacDiagABInv;
		deltaImpulse -= deltaVel2Dotn * m_jacDiagABInv;
		
		float sum = m_AppliedImpulse + deltaImpulse;
		if (sum < m_LowerLimit)
		{
			deltaImpulse = (m_LowerLimit - m_AppliedImpulse) * m_Friction;
			m_AppliedImpulse = m_LowerLimit;
		}
		else
		{
			m_AppliedImpulse = sum;
		}

		m_BodyA.ApplyImpulse(m_ContactNormalA * m_BodyA.m_InvMass, m_AngularA, deltaImpulse);
		m_BodyB.ApplyImpulse(m_ContactNormalB * m_BodyB.m_InvMass, m_AngularB, deltaImpulse);

		return deltaImpulse * (1.0 / m_jacDiagABInv);
	}
}