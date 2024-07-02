modded class ItemBase
{
    override int GetItemSize()
	{
		int width, height;
		GetGame().GetInventoryItemSize(this, width, height);
		return width * height;
	}
}