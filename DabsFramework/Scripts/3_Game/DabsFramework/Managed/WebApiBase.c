class WebApiBase: Managed
{
	protected RestApi m_Rest;
	protected RestContext m_RestContext;
	
	void WebApiBase()
	{		
		m_Rest = GetRestApi();
		if (!m_Rest) {
			m_Rest = CreateRestApi();
		}

#ifdef DIAG_DEVELOPER
		m_Rest.EnableDebug(true);
#endif
		m_RestContext = m_Rest.GetRestContext(GetBaseUrl());
	}
	
	// Overriden in child classes
	string GetBaseUrl()
	{
		return string.Empty;
	}
	
	RestApi GetRestApi()
	{
		return m_Rest;
	}
	
	RestContext GetRestContext()
	{
		return m_RestContext;
	}
}