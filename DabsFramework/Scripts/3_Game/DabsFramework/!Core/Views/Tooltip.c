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
    SpacerWidget Panel;
    Widget ImgFr;
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
	
	static TooltipView CreateOnWidget(Widget widget, string text, TooltipPosition position, string desc = "", Symbols icon = "")
	{
		float pos_x, pos_y;
		float b_x, b_y, b_w, b_h;
		widget.GetScreenPos(b_x, b_y);
		widget.GetScreenSize(b_w, b_h);
		
		TooltipView tooltip = new TooltipView(text, desc, icon);
		
		float tooltip_w, tooltip_h;
		tooltip.GetLayoutRoot().GetScreenSize(tooltip_w, tooltip_h);

        int tt_s_x, tt_s_y, td_s_x, td_s_y;
        tooltip.Text.GetTextSize(tt_s_x, tt_s_y);
        tooltip.Desc.GetTextSize(td_s_x, td_s_y);
        tt_s_x = Math.Max(tt_s_x, td_s_x);
        tt_s_y = Math.Max(tt_s_y, td_s_y);
		
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
		
		vector screen_direction;
		switch (position) {
			case TooltipPosition.BOTTOM_LEFT: {
                screen_direction = Vector(-b_w, -b_h, 0);
				break;
			}		
			
			case TooltipPosition.TOP_LEFT: {
                screen_direction = Vector(-b_w, b_h, 0);
				break;
			}
			
			case TooltipPosition.BOTTOM_RIGHT: {
                screen_direction = Vector(b_w, b_h, 0);
				break;
			}
			
			case TooltipPosition.TOP_RIGHT: {
                screen_direction = Vector(b_w, -b_h, 0);
				break;
			}
		}

        // offset position from center
        vector screen_position = Vector(b_x + b_w / 2, b_y + b_h / 2, 0);

        // corner position
        screen_position = screen_position + screen_direction * 0.8;

       // screen_position[0] = screen_position[0] - tooltip_w;
        
        ///tooltip.Panel.SetContentAlignmentH(WidgetAlignment.WA_LEFT);		
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);

        vector tooltip_vec = Vector(tt_s_x, tt_s_y, 0);
        vector screen_vec = Vector(screen_x, screen_y, 0);
        for (int i = 0; i < 3; i++) {
            if (screen_position[i] < screen_vec[i] * 0.1) {
                screen_position[i] = screen_vec[i] * 0.1;
            }

            if (screen_position[i] + tooltip_vec[i] > screen_vec[i] * 0.9) {
                screen_position[i] = screen_vec[i] * 0.9 - tooltip_vec[i];
            }
        }
		
		tooltip.GetLayoutRoot().SetScreenPos(screen_position[0], screen_position[1]);
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