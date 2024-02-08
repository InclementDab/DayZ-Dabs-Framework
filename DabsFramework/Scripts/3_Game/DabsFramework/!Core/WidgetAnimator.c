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
	
	static void Animate(notnull Widget source, WidgetAnimatorProperty property, float end_value, TimeSpan time)
	{				
		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}
		
		WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
		animation_timer.Run(source, property, WidgetAnimationTimer.GetProperty(source, property), end_value, time, false);
		m_RunningTimers.Insert(animation_timer);
	}
	
	static void Animate(notnull Widget source, WidgetAnimatorProperty property, float start_value, float end_value, int time)
	{		
		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}
		
		WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
		animation_timer.Run(source, property, start_value, end_value, time, false);
		m_RunningTimers.Insert(animation_timer);
	}
	
	static void AnimateLoop(notnull Widget source, WidgetAnimatorProperty property, float end_value, int time)
	{				
		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}
		
		WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
		animation_timer.Run(source, property, WidgetAnimationTimer.GetProperty(source, property), end_value, time, true);
		m_RunningTimers.Insert(animation_timer);
	}
	
	static void AnimateLoop(notnull Widget source, WidgetAnimatorProperty property, float start_value, float end_value, int time)
	{				
		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}
		
		WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
		animation_timer.Run(source, property, start_value, end_value, time, true);
		m_RunningTimers.Insert(animation_timer);
	}
		
	static void AnimateColor(notnull Widget source, LinearColor end_color, TimeSpan time, bool loop = false)
	{				
		WidgetAnimatorProperty property = WidgetAnimatorProperty.COLOR_B;
		for (int i = 0; i < 4; i++) {
			if (HasAnimation(source, property)) {
				CancelAnimate(source, property);
			}
			
			int start_value = WidgetAnimationTimer.GetProperty(source, property) * 255;
			int end_value = ((end_color >> (i * 8)) & 255);
			
			if (start_value != end_value) {
				WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
				animation_timer.Run(source, property, (float)start_value / 255.0, (float)end_value / 255.0, time, loop);
				m_RunningTimers.Insert(animation_timer);
			}
			
			// they are bitwise flags so halving the value will return us 
			property /= 2;
		}
	}
	
	static void AnimateColor(notnull Widget source, LinearColor start_color, LinearColor end_color, TimeSpan time, bool loop = false)
	{			
		WidgetAnimatorProperty property = WidgetAnimatorProperty.COLOR_B;
		for (int i = 0; i < 4; i++) {
			if (HasAnimation(source, property)) {
				CancelAnimate(source, property);
			}
			
			int start_value = ((start_color >> (i * 8)) & 255);
			int end_value = ((end_color >> (i * 8)) & 255);
			
			if (start_value != end_value) {
				WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
				animation_timer.Run(source, property, (float)start_value / 255.0, (float)end_value / 255.0, time, loop); // WidgetAnimationTimer.GetProperty(source, property)
				m_RunningTimers.Insert(animation_timer);
			}
			
			// they are bitwise flags so halving the value will return us 
			property /= 2;
		}
	}	
	
	static void AnimateColorHSV(notnull Widget source, vector start_color, vector end_color, TimeSpan time, bool loop = false)
	{				
		WidgetAnimatorProperty property = WidgetAnimatorProperty.COLOR_V;
		for (int i = 2; i >= 0; i--) {
			WidgetAnimationTimer hue_timer = new WidgetAnimationTimer();
			hue_timer.Run(source, property, start_color[i], end_color[i], time, loop);
			m_RunningTimers.Insert(hue_timer);
			property /= 2;
		}
	}
	
	// O(n) for now
	static void CancelAnimate(notnull Widget source, WidgetAnimatorProperty property = int.MIN, bool reset = false)
	{
		foreach (WidgetAnimationTimer widget_animation_timer: m_RunningTimers) {
			if (widget_animation_timer && widget_animation_timer.GetWidget() == source && (widget_animation_timer.GetWidgetAnimatorProperty() & property) == widget_animation_timer.GetWidgetAnimatorProperty()) {
				if (reset) {
					WidgetAnimationTimer.SetProperty(source, widget_animation_timer.GetWidgetAnimatorProperty(), widget_animation_timer.GetStartValue());
				}

				delete widget_animation_timer;
				return;
			}
		}
	}
		
	static WidgetAnimatorProperty GetAnimations(notnull Widget source)
	{		
		WidgetAnimatorProperty properties;
		foreach (WidgetAnimationTimer widget_animation_timer: m_RunningTimers) {
			if (widget_animation_timer && widget_animation_timer.GetWidget() == source) {
				properties |= widget_animation_timer.GetWidgetAnimatorProperty();
			}
		}
		
		return properties;
	}
	
	static bool HasAnimation(notnull Widget source, WidgetAnimatorProperty property)
	{
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