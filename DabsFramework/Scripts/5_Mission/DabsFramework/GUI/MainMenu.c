modded class MainMenu
{
	protected ButtonWidget m_DabsPatreonButton;
	
	override void Refresh()
	{
		super.Refresh();
		
		if (!DF_DisablePatreon()) {
			DF_LoadPatreon();
		}
	}
	
	protected void DF_LoadPatreon()
	{
		Widget settings_panel = layoutRoot.FindAnyWidget("settings_panel_root");
		if (!settings_panel || m_DabsPatreonButton) {
			return;
		}
		
		float s_sp_w, s_sp_h, sp_w, sp_h;
		settings_panel.GetSize(sp_w, sp_h);
		settings_panel.GetScreenSize(s_sp_w, s_sp_h);
		s_sp_h *= (37.0 / 40.0);
		
		int children_count;
		Widget child = settings_panel.GetChildren();
		while (child) {
			children_count++;
			float c_x, c_y;
			child.GetPos(c_x, c_y);
			child.SetPos(c_x * 0.823, c_y);

			float cs_x, cs_y;
			child.GetScreenSize(cs_x, cs_y);
			
			child = child.GetSibling();
		}
		
		settings_panel.SetSize(sp_w + (sp_w / children_count), s_sp_h);
		
		m_DabsPatreonButton = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidget(ButtonWidgetTypeID, 0, 0, s_sp_h, s_sp_h, WidgetFlags.HEXACTSIZE | WidgetFlags.VEXACTSIZE | WidgetFlags.VEXACTPOS | WidgetFlags.NOFOCUS | WidgetFlags.VISIBLE | WidgetFlags.CLIPCHILDREN, -1, 0, settings_panel));
		if (!m_DabsPatreonButton) {
			return;
		}
		
		m_DabsPatreonButton.SetName("patreon_button");
		m_DabsPatreonButton.SetPos(0.15 - (s_sp_h / sp_w), 0);
		
		ImageWidget patreon_image_widget = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidget(ImageWidgetTypeID, 0, 0, 1, 1, WidgetFlags.STRETCH | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.VISIBLE | WidgetFlags.IGNOREPOINTER, -1, 0, m_DabsPatreonButton));		
		if (patreon_image_widget) {
			patreon_image_widget.SetSize(1, 1);
			patreon_image_widget.SetName("patreon_button_image");
			Symbols.HAND_HOLDING_DOLLAR.Load(patreon_image_widget);
			// what a terrible icon patreon
			//patreon_image_widget.LoadImageFile(0, "set:brands image:patreon");
			//patreon_image_widget.SetImage(0);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (super.OnClick(w, x, y, button)) {
			return true;
		}
		
		if (button == MouseState.LEFT) {
			if (w == m_DabsPatreonButton) {
				//string title = "Support Dabs Framework";
				//string caption = "Open External Link?\n\n(https:\/\/patreon.com\/inclementdab)";
				//GetDayZGame().GetUIManager().ShowDialog(title, caption, 3840, DBT_YESNO, DBB_YES, DMT_INFO, this);
				GetGame().OpenURL("https:\/\/patreon.com\/inclementdab?utm_medium=unknown&utm_source=join_link&utm_campaign=creatorshare_creator&utm_content=copyLink");
				return true;
			}
		}

		return false;
	}
	
	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		switch (code) {
			case 3840: {
				if (result == DBB_YES) {
					GetGame().OpenURL("https:\/\/patreon.com\/inclementdab?utm_medium=unknown&utm_source=join_link&utm_campaign=creatorshare_creator&utm_content=copyLink");
				}
				
				break;
			}
		}
		
		
		return super.OnModalResult(w, x, y, code, result);
	}
	
	override bool IsFocusable(Widget w)
	{
		if (super.IsFocusable(w)) {
			return true;
		}

		return (w == m_DabsPatreonButton);
	}
	
	// There you go guys.
	bool DF_DisablePatreon()
	{
		return false;
	}
}