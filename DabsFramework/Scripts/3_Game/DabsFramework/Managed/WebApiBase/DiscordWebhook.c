class DiscordWebhook: WebApiBase
{		
	protected string m_WebhookId, m_WebhookToken;
	
	void DiscordWebhook(string webhook_id, string webhook_token)
	{
		m_WebhookId = webhook_id;
		m_WebhookToken = webhook_token;
		
		m_RestContext.SetHeader("application/json");
	}
	
	void Send(notnull DiscordPayload_Webhook data, string thread_id = string.Empty)
	{
		string string_data;
		JsonSerializer json_serializer = new JsonSerializer();
		if (!json_serializer.WriteToString(data, false, string_data)) {
			Error("Failed to serialize webhook");
			return;
		}
		
		HttpArguments arguments = {
			new HttpArgument("thread_id", thread_id)
		};
		
		m_RestContext.POST(new RestCallbackBase(), arguments.ToQuery(string.Format("/%1/%2", m_WebhookId, m_WebhookToken)), string_data);
	}
	
	string GetId()
	{
		return m_WebhookId;
	}
	
	string GetToken()
	{
		return m_WebhookToken;
	}
	
	override string GetBaseUrl()
	{
		return "https:\/\/discord.com/api/webhooks";
	}
}