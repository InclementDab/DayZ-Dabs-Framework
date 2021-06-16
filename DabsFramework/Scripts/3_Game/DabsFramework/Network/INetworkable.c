// RPC Manager? PauseChamp
class Networkable
{
	protected NetworkManager m_NetworkManager;
	protected Class m_Container;
	
	// I doubt this works
	protected ref array<string> m_FunctionWeb = {};
	
	void Networkable(Class container)
	{
		m_Container = container;
		
		// Register to the NetworkManager
		m_NetworkManager = NetworkManager.GetInstance();
		if (!m_NetworkManager) {
			m_NetworkManager = NetworkManager.Start();
		}
		
		m_NetworkManager.RegisterNetworkable(this);	
	}
	
	void RegisterNetworkFunction(string function)
	{
		m_FunctionWeb.Insert(function);
	}
	
	Class GetContainer()
	{
		return m_Container;
	}
}

// Singleton Network Manager
class NetworkManager
{	
	protected static ref NetworkManager m_Instance;
	
	static NetworkManager Start()
	{
		m_Instance = new NetworkManager();
		return m_Instance;
	}
	
	static void Stop()
	{
		delete m_Instance;
	}
	
	static NetworkManager GetInstance()
	{
		return m_Instance;
	}

	private void NetworkManager();
	
	protected ref map<Class, Networkable> m_NetworkWeb = new map<Class, Networkable>();	
	
	void RegisterNetworkable(Networkable networkable)
	{
		m_NetworkWeb[networkable.GetContainer()] = networkable; 
	}
}