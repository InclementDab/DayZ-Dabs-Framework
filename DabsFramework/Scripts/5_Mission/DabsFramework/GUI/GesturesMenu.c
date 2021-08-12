modded class GesturesMenu
{
	override void OnMenuRelease()
	{
		super.OnMenuRelease();
	}

	override void GetGestureItems(out ref array<ref GestureMenuItem> gesture_items, GestureCategories category)
	{
		super.GetGestureItems(gesture_items, category);
		
		if (category == GestureCategories.CATEGORY_3) {
			gesture_items.Insert(new GestureMenuItem(EmoteConstants.ID_EMOTE_DABBING, 	"#STR_USRACT_ID_EMOTE_DABBING", 	GestureCategories.CATEGORY_2));
		}
	}
}