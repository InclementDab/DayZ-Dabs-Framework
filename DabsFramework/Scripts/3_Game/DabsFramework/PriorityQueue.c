class PriorityQueue<Class TElement, Class TPriority>
{
	static const TElement ELEMENT_DEFAULT;

	protected int m_Capacity;
	protected ref array<ref Managed> m_StrongRefElements = {};
	protected ref array<ref Param2<TElement, TPriority>> m_Elements = {};

	ref ScriptInvoker Event_OnEnqueue = new ScriptInvoker();
	ref ScriptInvoker Event_OnDequeue = new ScriptInvoker();

	void PriorityQueue(int capacity = 0)
	{
		m_Capacity = capacity;

		if (capacity > 0) {
			// resize the queue arrays internally, optimization
			m_StrongRefElements.Reserve(capacity);
			m_Elements.Reserve(capacity);
		}
	}

	// adds the specified element with associated priority
	void Enqueue(TElement item, TPriority priority)
	{
		if (TemplateType<TElement>.GetType().IsInherited(Managed)) {
			// insert anonymously, elsewise compile time error
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(m_StrongRefElements.Insert, item);
		}

		m_Elements.Insert(new Param2<TElement, TPriority>(item, priority));
		Event_OnEnqueue.Invoke(item, priority);

		// check if the queue exceeds the capacity, and if so, remove the lowest priority element
		if (m_Capacity > 0 && m_Elements.Count() > m_Capacity) {
			int index = GetLowestPriorityIndex();
			if (index != -1) {
				Param2<TElement, TPriority> lowestPriorityItem = m_Elements[index];
				m_Elements.Remove(index);

				if (TemplateType<TElement>.GetType().IsInherited(Managed)) {
					GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(m_StrongRefElements.RemoveItem, lowestPriorityItem.param1);
					Event_OnDequeue.Invoke(lowestPriorityItem.param1, lowestPriorityItem.param2);
				}
			}
		}
	}

	// enqueues a sequence of elements pairs to the queue, all associated with the specified priority
	void EnqueueRange(array<TElement> elements, TPriority priority)
	{
		foreach (TElement element : elements) {
			Enqueue(element, priority);
		}
	}

	// enqueues a sequence of element-priority pairs to the queue
	void EnqueueRange(array<ref Param2<TElement, TPriority>> elements)
	{
		foreach (Param2<TElement, TPriority> element : elements) {
			Enqueue(element.param1, element.param2);
		}
	}

	// removes and returns the minimal element from the queue; the element with the highest priority
	TElement Dequeue()
	{
		int index = GetHighestPriorityIndex();
		if (index == -1) {
			return ELEMENT_DEFAULT;
		}

		Param2<TElement, TPriority> highestPriorityItem = m_Elements[index];
		m_Elements.Remove(index);

		if (TemplateType<TElement>.GetType().IsInherited(Managed)) {
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(m_StrongRefElements.RemoveItem, highestPriorityItem.param1);
		}

		Event_OnDequeue.Invoke(highestPriorityItem.param1, highestPriorityItem.param2);

		return highestPriorityItem.param1;
	}

	// adds the specified element with associated priority to the queue, and immediately removes the minimal element, returning the result
	TElement EnqueueDequeue(TElement item, TPriority priority)
	{
		Enqueue(item, priority);
		return Dequeue();
	}

	// returns the minimal element from the queue without removing it
	TElement Peak()
	{
		int index = GetHighestPriorityIndex();
		if (index == -1) {
			return ELEMENT_DEFAULT;
		}

		Param2<TElement, TPriority> item = m_Elements[index];
		return item.param1;
	}

	// returns max capacity of the queue
	int GetCapacity()
	{
		return m_Capacity;
	}

	// return number of elements
	int Count()
	{
		return m_Elements.Count();
	}

	// destroys all elements
	void Clear()
	{
		m_StrongRefElements.Clear();
		m_Elements.Clear();
	}

	// highest priority is a low value (1)
	protected int GetHighestPriorityIndex()
	{
		if (Count() == 0) {
			return -1;
		}

		int highestIndex;
		for (int i = 1; i < m_Elements.Count(); i++) {
			if (m_Elements[i].param2 < m_Elements[highestIndex].param2) {
				highestIndex = i;
			}
		}

		return highestIndex;
	}

	// lowest priority is a high value (100)
	protected int GetLowestPriorityIndex()
	{
		if (Count() == 0) {
			return -1;
		}

		int lowestIndex;
		for (int i = 1; i < m_Elements.Count(); i++) {
			if (m_Elements[i].param2 > m_Elements[lowestIndex].param2) {
				lowestIndex = i;
			}
		}

		return lowestIndex;
	}
}
