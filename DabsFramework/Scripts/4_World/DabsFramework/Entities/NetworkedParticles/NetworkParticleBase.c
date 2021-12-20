class NetworkParticleBase: SerializedBuilding
{
	protected Particle m_Particle;
	
	int ParticleType = ParticleList.NONE;
	
	void NetworkParticleBase()
	{
		if (GetGame().IsMultiplayer()) {
			// Ints
			RegisterNetSyncVariableInt("ParticleType");
		}
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {			
			// Update for clients
			if (GetGame().IsMultiplayer()) {
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OnVariablesSynchronized, 1000, false);
			}
		}
	}
	
	void ~NetworkParticleBase()
	{
		if (m_Particle) {
			m_Particle.Stop();
			m_Particle.Delete();
		}	
	}
	
	Particle CreateParticle(vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_world_position = false)
	{
		// very important
		if (ParticleType == ParticleList.NONE) {
			return null;
		}
		
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
	
	override void Write(inout map<string, ref SerializableParam> serializable_data)
	{
		serializable_data["ParticleType"] = SerializableParam1<int>.Create(ParticleType);
	}
	
	override void Read(map<string, ref SerializableParam> serializable_data)
	{		
		ParticleType = SerializableParam1<int>.Cast(serializable_data["ParticleType"]).param1;	
		if (GetGame().IsServer() && GetGame().IsMultiplayer()) {
			SetSynchDirty();
			return;
		}
		
		// Update
		// todo probably update this here, not sure havent tested finals tomorrow bye
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