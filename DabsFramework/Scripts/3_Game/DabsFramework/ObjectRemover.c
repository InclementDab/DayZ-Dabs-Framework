class ObjectRemoverLink: OLinkT
{
	int Flags;
	int Events;
}

class ObjectRemover
{	
	private static ref const map<Object, ref ObjectRemoverLink> REGISTERED_OBJECTS = new map<Object, ref ObjectRemoverLink>;

	private void ObjectRemover();
	private void ~ObjectRemover();

	/**
	 * @brief Registers obj to be exempt of being affected by the network bubble and the view distance.
	 *
	 * @param obj	Object pointer.
	 * @return bool	true of the object registered, false otherwise.
	 */
	static bool Register(Object obj)
	{
		if (!obj || IsObjectRegistered(obj)) {
			return false;
		}

		ObjectRemoverLink object_link = new ObjectRemoverLink(obj);
		object_link.Flags = obj.GetFlags();
		object_link.Events = obj.GetEventMask();
		REGISTERED_OBJECTS.Insert(obj, object_link);
		return true;
	}

	/**
	 * @brief Registers objects to be exempt of being affected by the network bubble and the view distance.
	 *
	 * @param objects	array<Object> array of objects to register.
	 */
	static void Register(array<Object> objects)
	{
		foreach (Object obj: objects) {
			Register(obj);
		}
	}

	/**
	 * @brief Unregisters obj so that it can be affected by the network bubble and the view distance.
	 *
	 * @param obj	Object pointer.
	 * @return bool	true of the object unregistered, false otherwise.
	 */
	static bool Unregister(Object obj)
	{
		if (!obj) {
			return false;
		}

		if (!REGISTERED_OBJECTS[obj]) {
			return false;
		}

		GetGame().ObjectRelease(obj);	
		
		delete REGISTERED_OBJECTS[obj];
		REGISTERED_OBJECTS.Remove(obj);		
		return true;
	}

	/**
	 * @brief Unregisters objects so that it can be affected by the network bubble and the view distance.
	 *
	 * @param objects	array<Object> array of objects to unregister.
	 */
	static void Unregister(array<Object> objects)
	{
		foreach (Object obj: objects) {
			Unregister(obj);
		}
	}

	/**
	 * @brief Checks if obj is registered.
	 *
	 * @param obj	Object pointer
	 * @return bool	true of the object registered, false otherwise.
	 */
	static bool IsObjectRegistered(Object obj)
	{
		if (!obj) {
			return false;
		}

		return REGISTERED_OBJECTS.Contains(obj);
	}

	/**
	 * @brief Checks if obj is a map object.
	 *
	 * @param obj	Object pointer
	 * @return bool	true of the object is a map object, false otherwise.
	 */
	static bool IsMapObject(Object obj)
	{
		if (!obj) {
			return false;
		}

		// Added via p3d in TB with no config.
		bool isStatic = (obj.GetType() == string.Empty) && (obj.Type() == Object);
		// Inherits from House in Cfg class.
		// Building, House, Wreck, Well, Tree, Bush, etc.
		bool isHouse = obj.IsKindOf("House");
		bool isVegetation = obj.IsTree() || obj.IsBush();
        bool isRock = obj.IsRock();

		return (isStatic || isHouse || isVegetation || isRock);
	}

	/**
	 * @brief Removes static object from the map.
	 *
	 * @param obj	Object pointer
	 */
	static void RemoveObject(Object obj)
	{
		if (!obj || !IsMapObject(obj)) {
			return;
		}

		Register(obj);
		EntityFlags flags = obj.GetFlags();
		EntityEvent events = obj.GetEventMask();
		obj.ClearFlags(flags, true);
		obj.ClearEventMask(events);
		obj.SetEventMask(EntityEvent.NOTVISIBLE);
		obj.SetScale(0.0);
		dBodyDestroy(obj); //! Needed for disabling some extra collisions.
		
		//dBodyActive(objectHere, ActiveState.INACTIVE);
		//dBodyDynamic(objectHere, false);
	}
	
	/**
	 * @brief Restores static objects to the map.
	 *
	 * @param obj	Object pointer
	 */
	static void RestoreObject(Object obj)
	{
		if (!obj || !IsMapObject(obj)) {
			return;
		}
		
		ObjectRemoverLink object_link = REGISTERED_OBJECTS[obj];
		if (!object_link) {
			return;
		}
			
		obj.SetFlags(object_link.Flags, true);
		obj.SetEventMask(object_link.Events);
		obj.SetScale(1.0);
		Unregister(obj);
	}
	
	/**
	 * @brief Restores all static Objects to the map.
	 *
	 */
	static void RestoreAllMapObjects()
	{
		foreach (Object obj, ObjectRemoverLink link: REGISTERED_OBJECTS) {
			RestoreObject(obj);
		}
	}

	/**
	 * @brief Removes static objects at certain position and radius from the map (No height).
	 *
	 * @param position	vector coordinates of the center.
	 * @param radius	float radius of the query.
	 */
	static void RemoveObjectsInRadius2D(vector position, float radius)
	{
		array<Object> objects = new array<Object>();
		GetGame().GetObjectsAtPosition(position, radius, objects, null);
		foreach (Object obj: objects) {
			RemoveObject(obj);
		}
	}

	/**
	 * @brief Removes static objects at certain position and radius from the map.
	 *
	 * @param position	vector coordinates of the center.
	 * @param radius	float radius of the query.
	 */
	static void RemoveObjectsInRadius3D(vector position, float radius)
	{
		array<Object> objects = new array<Object>();
		GetGame().GetObjectsAtPosition3D(position, radius, objects, null);
		foreach (Object obj: objects) {
			RemoveObject(obj);
		}
	}
		
	/**
	 * @brief Get all current registered objects.
	 *
	 * @return array<Object>	array of current registered objects.
	 */
	static array<Object> GetRegisteredObjects()
	{
		return REGISTERED_OBJECTS.GetKeyArray();
	}
}