class WidgetAnimationProperty: Managed
{
	private void WidgetAnimationProperty();
	private void ~WidgetAnimationProperty();
	
	const int SIZE 		= 1;
	const int POSITION 	= 2;
	const int ROTATION 	= 3;
	const int COLOR 	= 4;
}

//@ this is version 1, the node system above should be version 2
class WidgetAnimator: Managed
{
	protected static ref array<ref WidgetAnimationTimer> m_RunningTimers = {};
	
	static void Animate(Widget source, WidgetAnimatorProperty property, float end_value, TimeSpan time)
	{				
		if (!source) {
			return;
		}

		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}
		
		WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
		animation_timer.Run(source, property, WidgetAnimationTimer.GetProperty(source, property), end_value, time, false);
		m_RunningTimers.Insert(animation_timer);
	}
	
	static void Animate(Widget source, WidgetAnimatorProperty property, float start_value, float end_value, int time)
	{		
		if (!source) {
			return;
		}

		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}
		
		WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
		animation_timer.Run(source, property, start_value, end_value, time, false);
		m_RunningTimers.Insert(animation_timer);
	}
	
	static void AnimateLoop(Widget source, WidgetAnimatorProperty property, float end_value, int time)
	{				
		if (!source) {
			return;
		}

		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}
		
		WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
		animation_timer.Run(source, property, WidgetAnimationTimer.GetProperty(source, property), end_value, time, true);
		m_RunningTimers.Insert(animation_timer);
	}
	
	static void AnimateLoop(Widget source, WidgetAnimatorProperty property, float start_value, float end_value, int time)
	{				
		if (!source) {
			return;
		}

		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}
		
		WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
		animation_timer.Run(source, property, start_value, end_value, time, true);
		m_RunningTimers.Insert(animation_timer);
	}
		
	static void AnimateColor(Widget source, LinearColor end_color, TimeSpan time, BlendMode blend_mode = BlendMode.NORMAL, bool loop = false)
	{				
		if (!source) {
			return;
		}

		WidgetAnimatorProperty property = WidgetAnimatorProperty.LINEAR_COLOR;
		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}

		if (source.GetColor() != end_color) {
			ColorWidgetAnimationTimer animation_timer = new ColorWidgetAnimationTimer();
			animation_timer.RunWithBlend(source, property, source.GetColor(), end_color, blend_mode, time, loop);
			m_RunningTimers.Insert(animation_timer);
		}
	}
	
	static void AnimateColor(Widget source, LinearColor start_color, LinearColor end_color, TimeSpan time, BlendMode blend_mode = BlendMode.NORMAL, bool loop = false)
	{			
		if (!source) {
			return;
		}

		WidgetAnimatorProperty property = WidgetAnimatorProperty.LINEAR_COLOR;
		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}

		if (start_color != end_color) {
			ColorWidgetAnimationTimer animation_timer = new ColorWidgetAnimationTimer();
			animation_timer.RunWithBlend(source, property, start_color, end_color, blend_mode, time, loop);
			m_RunningTimers.Insert(animation_timer);
		}
	}	
	
	static void AnimateColorHSV(Widget source, vector start_color, vector end_color, TimeSpan time, bool loop = false)
	{				
		if (!source) {
			return;
		}

		WidgetAnimatorProperty property = WidgetAnimatorProperty.COLOR_V;
		for (int i = 2; i >= 0; i--) {
			WidgetAnimationTimer hue_timer = new WidgetAnimationTimer();
			hue_timer.Run(source, property, start_color[i], end_color[i], time, loop);
			m_RunningTimers.Insert(hue_timer);
			property /= 2;
		}
	}
	
	// O(n) for now
	static void CancelAnimate(Widget source, WidgetAnimatorProperty property = int.MIN, bool reset = false)
	{
		if (!source) {
			return;
		}

		foreach (WidgetAnimationTimer widget_animation_timer: m_RunningTimers) {
			if (widget_animation_timer && widget_animation_timer.GetWidget() == source && (widget_animation_timer.GetWidgetAnimatorProperty() & property)) {
				if (reset) {
					WidgetAnimationTimer.SetProperty(source, widget_animation_timer.GetWidgetAnimatorProperty(), widget_animation_timer.GetStartValue());
				}

				delete widget_animation_timer;
				return;
			}
		}
	}
		
	static WidgetAnimatorProperty GetAnimations(Widget source)
	{		
		WidgetAnimatorProperty properties;
		if (!source) {
			return properties;
		}

		foreach (WidgetAnimationTimer widget_animation_timer: m_RunningTimers) {
			if (widget_animation_timer && widget_animation_timer.GetWidget() == source) {
				properties |= widget_animation_timer.GetWidgetAnimatorProperty();
			}
		}
		
		return properties;
	}
	
	static bool HasAnimation(Widget source, WidgetAnimatorProperty property)
	{
		if (!source) {
			return false;
		}

		foreach (WidgetAnimationTimer widget_animation_timer: m_RunningTimers) {
			if (widget_animation_timer && widget_animation_timer.GetWidget() == source) {
				if ((property & widget_animation_timer.GetWidgetAnimatorProperty()) == property) {
					return true;
				}
			}
		}
		
		return false;
	}
}