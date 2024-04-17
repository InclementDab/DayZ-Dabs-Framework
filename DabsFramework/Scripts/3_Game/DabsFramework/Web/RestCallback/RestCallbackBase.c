// Cant mod RestCallback and the logging is god awful
class RestCallbackBase: RestCallback
{
    protected int m_CallTime;
	
	void RestCallbackBase()
	{
		if (GetGame()) {
			m_CallTime = GetGame().GetTime();
		}
	}
		
    override void OnSuccess(string data, int dataSize)
	{
		WebApiLog.Trace(this, "OnSuccess");
		WebApiLog.Log(string.Format("%1 returned success from api, completed in %2ms", Type(), GetGame().GetTime() - m_CallTime));
	}

	override void OnTimeout()
	{
		WebApiLog.Trace(this, "OnTimeout");
		WebApiLog.Log(string.Format("%1 returned timeout from api, completed in %2ms", Type(), GetGame().GetTime() - m_CallTime));
	}
	
    // often error code 5: EREST_ERROR is the response. can occur from lack of api key
    override void OnError(int errorCode)
	{		
		WebApiLog.Trace(this, "OnError");
        Error(string.Format("%1 returned error from api: %2, completed in %3ms", Type(), typename.EnumToString(ERestResultState, errorCode), GetGame().GetTime() - m_CallTime));
	}
	
	override void OnFileCreated(string fileName, int dataSize)
	{
		WebApiLog.Trace(this, "OnFileCreated");
		WebApiLog.Log(string.Format("%1 returned filecreated from api, completed in %2ms", Type(), GetGame().GetTime() - m_CallTime));
	}

    int GetCallTime()
    {
        return m_CallTime;
    }
}