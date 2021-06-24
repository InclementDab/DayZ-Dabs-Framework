Widget GetChildAtIndex(Widget widget, int index)
{
	Widget result = widget.GetChildren();
	while (index > 0)
	{
		result = result.GetSibling();
		index--;
	}

	return result;
}

bool IsWidgetChild(Widget widget, Widget child)
{
	Widget result = widget.GetChildren();
	while (result) {
		if (result == child) {
			return true;
		}
		
		result = result.GetSibling();
	}

	return false;
}

class WidgetController
{
	bool CanTwoWayBind()
	{
		return false;
	}

	// Base Controller Stuff
	void Set(TypeConverter type_converter);
	void Get(out TypeConverter type_converter);

	// Collection Stuff
	void SetSelection(TypeConverter type_converter)
	{
		NotImplementedError("SetSelection");
	}

	void GetSelection(out TypeConverter type_converter)
	{
		NotImplementedError("GetSelection");
	}

	void Insert(TypeConverter type_converter)
	{
		NotImplementedError("Insert");
	}

	void InsertAt(int index, TypeConverter type_converter)
	{
		NotImplementedError("InsertAt");
	}

	void Remove(int index, TypeConverter type_converter)
	{
		NotImplementedError("Remove");
	}

	void Replace(int index, TypeConverter type_converter)
	{
		NotImplementedError("Replace");
	}

	void Move(int index, TypeConverter type_converter)
	{
		NotImplementedError("Move");
	}

	void Swap(int index_a, int index_b)
	{
		NotImplementedError("Swap");
	}

	void Clear()
	{
		NotImplementedError("Clear");
	}

	int Find(TypeConverter type_converter)
	{
		NotImplementedError("Find");
		return -1;
	}

	int Count()
	{
		NotImplementedError("Count");
		return -1;
	}

	private	void NotImplementedError(string function)
	{
		Error(string.Format("%1 does not support function %2", Type(), function));
	}
}

class WidgetControllerTemplate<Class T> : WidgetController
{
	protected T m_Widget;

	void SetWidget(T w)
	{
		Class.CastTo(m_Widget, w);
	}
}

class WidgetBaseController : WidgetControllerTemplate<Widget>
{
	override void Set(TypeConverter type_converter)
	{
		if (!m_Widget) {
			return;
		}

		Widget parent = m_Widget.GetParent();
		int sort = m_Widget.GetSort();
		if (type_converter.GetWidget()) {
			m_Widget = type_converter.GetWidget();

			if (parent) {
				parent.AddChild(m_Widget, false);
			}

			m_Widget.SetSort(sort);
		}
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetWidget(m_Widget);
	}
}

class ButtonWidgetController : WidgetControllerTemplate<ButtonWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetState(type_converter.GetBool());
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetBool(m_Widget.GetState());
	}

	override void SetSelection(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		// Since SetSelection is being used. This needs to override to avoid errors
	}
}

class EditBoxWidgetController : WidgetControllerTemplate<EditBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetString(m_Widget.GetText());
	}
}

class CheckBoxWidgetController : WidgetControllerTemplate<CheckBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetChecked(type_converter.GetBool());
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetBool(m_Widget.IsChecked());
	}

	override void SetSelection(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		// Since SetSelection is being used. This needs to override to avoid errors
	}
}

class SliderWidgetController : WidgetControllerTemplate<SliderWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetCurrent(type_converter.GetFloat());
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetFloat(m_Widget.GetCurrent());
	}
}

class ProgressBarController : WidgetControllerTemplate<ProgressBarWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetCurrent(type_converter.GetFloat());
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetFloat(m_Widget.GetCurrent());
	}
}

class TextWidgetController : WidgetControllerTemplate<TextWidget>
{
	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}
}

class ImageWidgetController : WidgetControllerTemplate<ImageWidget>
{
	override void Set(TypeConverter type_converter)
	{
		if (type_converter.GetString())
		{
			m_Widget.LoadImageFile(0, type_converter.GetString());
			m_Widget.SetImage(0);
		}
	}
}

class MultilineEditBoxWidgetController : WidgetControllerTemplate<MultilineEditBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}

	override void Get(out TypeConverter type_converter)
	{
		string out_text;
		m_Widget.GetText(out_text);
		type_converter.SetString(out_text);
	}

	override void Insert(TypeConverter type_converter)
	{
		m_Widget.SetLine(m_Widget.GetLinesCount(), type_converter.GetString());
	}
	
	override void InsertAt(int index, TypeConverter type_converter)
	{
		m_Widget.SetLine(index, type_converter.GetString());
	}

	override void Replace(int index, TypeConverter type_converter)
	{
		InsertAt(index, type_converter);
	}

	override void Remove(int index, TypeConverter type_converter)
	{
		m_Widget.SetLine(index, string.Empty);
	}

	override void Clear()
	{
		for (int i = 0; i < m_Widget.GetLinesCount(); i++) {
			m_Widget.SetLine(i, string.Empty);
		}
	}
}

class SpacerBaseWidgetController : WidgetControllerTemplate<SpacerBaseWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void SetSelection(TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			SetFocus(type_converter.GetWidget());
		}
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		if (IsWidgetChild(m_Widget, GetFocus())) {
			type_converter.SetWidget(GetFocus());
		}
	}

	override void Insert(TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			m_Widget.AddChild(type_converter.GetWidget());
		}
	}

	override void InsertAt(int index, TypeConverter type_converter)
	{
		if (index == Count()) {
			Insert(type_converter);
		} 
		
		else if (type_converter.GetWidget()) {
			Widget widget_a = GetChildAtIndex(m_Widget, index);
			m_Widget.AddChildAfter(type_converter.GetWidget(), widget_a);
		}
	}

	override void Replace(int index, TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			Widget widget_a = GetChildAtIndex(m_Widget, index);
			m_Widget.AddChildAfter(type_converter.GetWidget(), widget_a);
			m_Widget.RemoveChild(widget_a);
		}
	}

	override void Remove(int index, TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			m_Widget.RemoveChild(type_converter.GetWidget());
		}
	}

	override void Move(int index, TypeConverter type_converter)
	{
		Widget widget_a = GetChildAtIndex(m_Widget, index - 1);
		if (type_converter.GetWidget() && widget_a) {
			m_Widget.RemoveChild(type_converter.GetWidget());
			m_Widget.AddChildAfter(type_converter.GetWidget(), widget_a);
		}
	}

	override void Swap(int index_a, int index_b)
	{
		if (index_a == index_b || index_a < 0 || index_b < 0) {
			return;
		}

		if (index_a > index_b) {
			int temp = index_a;
			index_a = index_b;
			index_b = temp;
		}

		Widget widget_a = GetChildAtIndex(m_Widget, index_a);
		Widget widget_b = GetChildAtIndex(m_Widget, index_b);
		Widget widget_c = GetChildAtIndex(m_Widget, index_b - 1);

		m_Widget.RemoveChild(widget_a);
		m_Widget.AddChildAfter(widget_a, widget_b);
		m_Widget.RemoveChild(widget_b);
		m_Widget.AddChildAfter(widget_b, widget_c);
	}

	override void Clear()
	{
		Widget widget_a = m_Widget.GetChildren();
		while (widget_a) {
			m_Widget.RemoveChild(widget_a);
			widget_a = widget_a.GetSibling();
		}
	}

	override int Find(TypeConverter type_converter)
	{
		Widget widget_a = m_Widget.GetChildren();
		int result;
		while (widget_a) {
			if (widget_a == type_converter.GetWidget()) {
				return result;
			}

			widget_a = widget_a.GetSibling();
			result++;
		}

		return -1;
	}

	override int Count()
	{
		Widget widget_a = m_Widget.GetChildren();
		int result;
		while (widget_a) {
			widget_a = widget_a.GetSibling();
			result++;
		}

		return result;
	}
}

class XComboBoxWidgetController : WidgetControllerTemplate<XComboBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void SetSelection(TypeConverter type_converter)
	{
		m_Widget.SetCurrentItem(type_converter.GetInt());
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		type_converter.SetInt(m_Widget.GetCurrentItem());
	}

	override void Insert(TypeConverter type_converter)
	{
		m_Widget.AddItem(type_converter.GetString());
	}

	override void InsertAt(int index, TypeConverter type_converter)
	{
		m_Widget.SetItem(index, type_converter.GetString());
	}

	override void Replace(int index, TypeConverter type_converter)
	{
		InsertAt(index, type_converter);
	}

	override void Remove(int index, TypeConverter type_converter)
	{
		m_Widget.RemoveItem(index);
	}

	override void Clear()
	{
		m_Widget.ClearAll();
	}

	override int Count()
	{
		return m_Widget.GetNumItems();
	}
}

class TextListboxController : WidgetControllerTemplate<TextListboxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void SetSelection(TypeConverter type_converter)
	{
		for (int i = 0; i < m_Widget.GetNumItems(); i++)
		{
			string row_text;
			m_Widget.GetItemText(i, 0, row_text);
			if (row_text == type_converter.GetString()) {
				m_Widget.SelectRow(i);
				return;
			}
		}
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		string selection;
		m_Widget.GetItemText(m_Widget.GetSelectedRow(), 0, selection);
		type_converter.SetString(selection);
	}

	override void Insert(TypeConverter type_converter)
	{
		m_Widget.AddItem(type_converter.GetString(), type_converter, 0);
	}

	override void InsertAt(int index, TypeConverter type_converter)
	{
		m_Widget.SetItem(index, type_converter.GetString(), type_converter, 0);
	}

	override void Remove(int index, TypeConverter type_converter)
	{
		m_Widget.SetItem(index, string.Empty, type_converter, 0);
	}

	override void Swap(int index_a, int index_b)
	{
		string textA, textB;
		Class dataA, dataB;

		if (index_a < 0 || index_a > m_Widget.GetNumItems() || index_b < 0 || index_b > m_Widget.GetNumItems()) {
			return;
		}

		m_Widget.GetItemText(index_a, 0, textA);
		m_Widget.GetItemData(index_a, 0, dataA);

		m_Widget.GetItemText(index_b, 0, textB);
		m_Widget.GetItemData(index_b, 0, dataB);

		m_Widget.SetItem(index_b, textA, dataA, 0);
		m_Widget.SetItem(index_a, textB, dataB, 0);
	}

	override void Clear()
	{
		m_Widget.ClearItems();
	}

	override int Count()
	{
		return m_Widget.GetNumItems();
	}
}

class ItemPreviewWidgetController : WidgetControllerTemplate<ItemPreviewWidget>
{
	override void Set(TypeConverter type_converter)
	{
		EntityAI entity;
		if (Class.CastTo(entity, type_converter.GetObject())) {
			m_Widget.SetView(0);
			m_Widget.SetItem(entity);
			m_Widget.SetModelPosition(vector.Zero);
		}
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.Set(m_Widget.GetItem());
	}
}

class PlayerPreviewWidgetController : WidgetControllerTemplate<PlayerPreviewWidget>
{
    override void Set(TypeConverter type_converter)
    {
        DayZPlayer player;
        if (Class.CastTo(player, type_converter.GetObject())) {
            m_Widget.SetPlayer(player);
            m_Widget.SetModelPosition(vector.Zero);
        }
    }

    override void Get(out TypeConverter type_converter)
    {
        type_converter.Set(m_Widget.GetDummyPlayer());
    }
	
	override void SetSelection(TypeConverter type_converter)
	{
		m_Widget.UpdateItemInHands(EntityAI.Cast(type_converter.GetObject()));
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		DayZPlayer player;
		if (Class.CastTo(player, m_Widget.GetDummyPlayer())) {
			type_converter.SetObject(player.GetHumanInventory().GetEntityInHands());
		} 
	}
}

class HtmlWidgetController : WidgetControllerTemplate<HtmlWidget>
{
    override void Set(TypeConverter type_converter)
    {
        m_Widget.SetText(type_converter.GetString());
    }
}