enum TooltipPosition
{
	BOTTOM_LEFT,
	TOP_LEFT,
	BOTTOM_RIGHT,
	TOP_RIGHT,
	
	INSIDE,
	OUTSIDE
};

class TooltipView: ScriptView
{
    Widget Panel, ImgFr;
	TextWidget Text, Desc;
    ImageWidget Img;
	
	void TooltipView(string text, string desc, Symbols icon)
	{
		Text.SetText(text);
		m_LayoutRoot.Show(text != string.Empty);
		Desc.SetText(desc);
		Desc.Show(desc != string.Empty);

        ImgFr.Show(icon != string.Empty);
        if (icon) {
            Img.LoadImageFile(0, icon.Solid());
            Img.SetImage(0);
        }

        Panel.SetColor(GetColor());
	}
	
	static TooltipView CreateOnWidget(Widget widget, string text, TooltipPosition position, Symbols icon = "", string desc = "")
	{
		float pos_x, pos_y;
		float b_x, b_y, b_w, b_h;
		widget.GetScreenPos(b_x, b_y);
		widget.GetScreenSize(b_w, b_h);
		
		TooltipView tooltip = new TooltipSlotView(text, desc, icon);
		
		float w, h;
		tooltip.GetLayoutRoot().GetScreenSize(w, h);
		
		int s_x, s_y;
		GetScreenSize(s_x, s_y);
		switch (position) {
			case TooltipPosition.INSIDE: {
				if (b_x >= s_x / 2) {
					if (b_y >= s_y / 2) {
						position = TooltipPosition.TOP_LEFT;
					} else {
						position = TooltipPosition.BOTTOM_LEFT;
					}
				} else {
					if (b_y >= s_y / 2) {
						position = TooltipPosition.TOP_RIGHT;
					} else {
						position = TooltipPosition.BOTTOM_RIGHT;
					}
				}
				
				break;
			}
		}
				
		
		switch (position) {
			case TooltipPosition.BOTTOM_LEFT: {
				pos_x = b_x - w;
				pos_y = b_y + b_h;
				break;
			}		
			
			case TooltipPosition.TOP_LEFT: {
				pos_x = b_x - w;
				pos_y = b_y - h;
				break;
			}
			
			case TooltipPosition.BOTTOM_RIGHT: {
				pos_x = b_x;
				pos_y = b_y + b_h;
				break;
			}
			
			case TooltipPosition.TOP_RIGHT: {
				pos_x = b_x;
				pos_y = b_y - h;
				break;
			}
		}
		
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		if (pos_x + b_w > screen_x) {
			pos_x = (screen_x - b_w) - 25;
		}
		
		if (pos_y + b_h > screen_y) {
			pos_y = (screen_y - b_h) - 25;
		}
		
		if (pos_x < 0) {
			pos_x = 25;
		}
		
		if (pos_y < 0) {
			pos_y = 25;
		}
		
		tooltip.GetLayoutRoot().SetScreenPos(pos_x, pos_y);
		return tooltip;
	}

    protected int GetColor()
    {
        return LinearColor.Create(240, 0, 0, 0);
    }
	
	override string GetLayoutFile()
	{
		return "DabsFramework\\GUI\\layouts\\tooltip\\tooltip_view.layout";
	}
}