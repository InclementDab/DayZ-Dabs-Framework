modded class NotificationSystem
{
	override NotificationData GetNotificationData(NotificationType type)
	{
		if (m_DataArray.Contains(type))
			return m_DataArray.Get(type);
		
		return null;
	}
}