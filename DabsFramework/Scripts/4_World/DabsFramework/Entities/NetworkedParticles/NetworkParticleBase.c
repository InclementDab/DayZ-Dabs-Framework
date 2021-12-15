class NetworkParticleBase: Building
{
	protected Particle m_Particle;
	
	int ParticleType = ParticleList.NONE;
	
	void NetworkParticleBase()
	{
		if (GetGame().IsMultiplayer()) {
			RegisterNetSyncVariableInt("ParticleType");
			
			// Booleans
			/*RegisterNetSyncVariableBool("CastShadow");
			RegisterNetSyncVariableBool("EnableSpecular");
			RegisterNetSyncVariableBool("EnableLinear");
			RegisterNetSyncVariableBool("VisibleDuringDay");
			RegisterNetSyncVariableBool("HeatHaze");
			RegisterNetSyncVariableBool("FlareVisible");
			
			// Ints
			RegisterNetSyncVariableInt("DiffuseColor");
			RegisterNetSyncVariableInt("AmbientColor");
			
			// Floats
			RegisterNetSyncVariableFloat("PulseCoef");
			RegisterNetSyncVariableFloat("Radius");
			RegisterNetSyncVariableFloat("HeatHazeRadius");
			RegisterNetSyncVariableFloat("HeatHazePower");
			RegisterNetSyncVariableFloat("Brightness");
			RegisterNetSyncVariableFloat("SpotLightAngle");*/
		}
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {			
			// Update for clients
			//if (GetGame().IsMultiplayer()) {
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OnVariablesSynchronized, 1000, false);
			//}
		}
	}
	
	Particle CreateParticle(vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_world_position = false)
	{
		return Particle.PlayOnObject(ParticleType, this, local_pos, local_ori, force_world_position);
	}
	
	Particle GetParticle()
	{
		return m_Particle;
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
				
		// we have to delay the creation since the Particle Type is required for CreateParticle
		if ((GetGame().IsClient() || !GetGame().IsMultiplayer()) && !m_Particle) {
			m_Particle = CreateParticle();
		}
		
		/*if (!m_Particle) { // !GetGame().IsMultiplayer()
			return;
		}*/
	}
	
	void Write(inout map<string, ref SerializableParam> serializable_data)
	{
		serializable_data["ParticleType"] = SerializableParam1<int>.Create(ParticleType);
		
		/*
		serializable_data["CastShadow"] = SerializableParam1<bool>.Create(CastShadow);
		serializable_data["EnableSpecular"] = SerializableParam1<bool>.Create(EnableSpecular);
		serializable_data["EnableLinear"] = SerializableParam1<bool>.Create(EnableLinear);
		serializable_data["VisibleDuringDay"] = SerializableParam1<bool>.Create(VisibleDuringDay);
		serializable_data["HeatHaze"] = SerializableParam1<bool>.Create(HeatHaze);
		serializable_data["FlareVisible"] = SerializableParam1<bool>.Create(FlareVisible);
		
		serializable_data["DiffuseColor"] = SerializableParam1<int>.Create(DiffuseColor);
		serializable_data["AmbientColor"] = SerializableParam1<int>.Create(AmbientColor);
		
		serializable_data["PulseCoef"] = SerializableParam1<float>.Create(PulseCoef);
		serializable_data["Radius"] = SerializableParam1<float>.Create(Radius);
		serializable_data["HeatHazeRadius"] = SerializableParam1<float>.Create(HeatHazeRadius);
		serializable_data["HeatHazePower"] = SerializableParam1<float>.Create(HeatHazePower);
		serializable_data["Brightness"] = SerializableParam1<float>.Create(Brightness);
		serializable_data["SpotLightAngle"] = SerializableParam1<float>.Create(SpotLightAngle);
		*/
	}
	
	void Read(map<string, ref SerializableParam> serializable_data)
	{		
		ParticleType = SerializableParam1<int>.Cast(serializable_data["ParticleType"]).param1;
		
		/*
		CastShadow = SerializableParam1<bool>.Cast(serializable_data["CastShadow"]).param1;
		EnableSpecular = SerializableParam1<bool>.Cast(serializable_data["EnableSpecular"]).param1;
		EnableLinear = SerializableParam1<bool>.Cast(serializable_data["EnableLinear"]).param1;
		VisibleDuringDay = SerializableParam1<bool>.Cast(serializable_data["VisibleDuringDay"]).param1;
		HeatHaze = SerializableParam1<bool>.Cast(serializable_data["HeatHaze"]).param1;
		FlareVisible = SerializableParam1<bool>.Cast(serializable_data["FlareVisible"]).param1;
		
		DiffuseColor = SerializableParam1<int>.Cast(serializable_data["DiffuseColor"]).param1;
		AmbientColor = SerializableParam1<int>.Cast(serializable_data["AmbientColor"]).param1;
		
		PulseCoef = SerializableParam1<float>.Cast(serializable_data["PulseCoef"]).param1;
		Radius = SerializableParam1<float>.Cast(serializable_data["Radius"]).param1;
		HeatHazeRadius = SerializableParam1<float>.Cast(serializable_data["HeatHazeRadius"]).param1;
		HeatHazePower = SerializableParam1<float>.Cast(serializable_data["HeatHazePower"]).param1;
		Brightness = SerializableParam1<float>.Cast(serializable_data["Brightness"]).param1;
		SpotLightAngle = SerializableParam1<float>.Cast(serializable_data["SpotLightAngle"]).param1;
		*/
		if (GetGame().IsServer() && GetGame().IsMultiplayer()) {
			SetSynchDirty();
			return;
		}
		
		// Update
		/*
		m_Light.SetCastShadow(CastShadow);
		m_Light.EnableSpecular(EnableSpecular);
		m_Light.EnableLinear(EnableLinear);
		m_Light.SetPulseCoef(PulseCoef);
		m_Light.SetVisibleDuringDaylight(VisibleDuringDay);	
		m_Light.SetRadiusTo(Radius);
		m_Light.EnableHeatHaze(HeatHaze);
		m_Light.SetHeatHazeRadius(HeatHazeRadius);
		m_Light.SetHeatHazePower(HeatHazePower);

		float a, r, g, b;
		InverseARGBF(DiffuseColor, a, r, g, b);
		m_Light.SetDiffuseColor(r, g, b);
		m_Light.SetDiffuseAlpha(a);

		InverseARGBF(AmbientColor, a, r, g, b);
		m_Light.SetAmbientColor(r, g, b);
		m_Light.SetAmbientAlpha(a);
		m_Light.SetBrightnessTo(Brightness);
		m_Light.SetFlareVisible(FlareVisible);
		//m_Light.SetFlareRelPosition(FlareRelativePosition);
		m_Light.SetSpotLightAngle(SpotLightAngle);
		*/
	}
	
	
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "ParticleType": {
				if (m_Particle) {
					m_Particle.Stop();
					GetGame().ObjectDelete(m_Particle);
				}
				
				if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {
					m_Particle = CreateParticle();
				}
			}
		}
	}
}