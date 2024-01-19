
/*
typedef int Color;

//@ Storage format of color is ARGB
class Color: int
{	
	private void Color();
	private void ~Color();
	
	void SetAlpha(int alpha)
	{
		value = (alpha << 24) ^ value;
	}
	
	void SetRed(int red)
	{
		value = (red << 16) ^ value;	
	}	
	
	void SetGreen(int green)
	{
		value = (green << 8) ^ value;	
	}	
	
	void SetBlue(int green)
	{
		value = (green << 0) ^ value;
	}
	
	static Color FromHSV(float hue, float saturation, float value, int alpha = 255)
	{
		int hexasphere = Math.FMod(hue, 360.0) / 60;
		int trientsphere = Math.FMod(hue, 360.0) / 120;
		int hemisphere = Math.FMod(hue, 360.0) / 180;
		Print(hexasphere);
		Print(trientsphere);
		Print(hemisphere);
		
		hue 		= Math.FMod(hue, 60.0);		
		saturation 	= Math.FMod(saturation, 100.0) / 100.0;
		value 		= Math.FMod(value, 100.0) / 100.0;
		
	    float c = saturation * value;
	    float x = c * (1.0 - Math.AbsFloat(Math.FMod(hue / 60.0, 2.0) - 1.0));
	    float m = value - c;
		
		int argb;
		//argb = (x << (trientsphere * 8 + 
		
		
		int r, g, b;
		// trientsphere 0
		// hemisphere 0
	    if (hue >= 0 && hue < 60) {
	       	r = c + m;  
			g = x + m;  
			b = 0 + m; 
	    }
		
		// trientsphere 0
		// hemisphere 0
		if (hue >= 180 && hue < 240) {
	        r = 0 + m; 
			g = x + m;   
			b = c + m; 
	    }
		
		// trientsphere 1
		// hemisphere
		if (hue >= 60 && hue < 120) {
			r = x + m; 
			g = c + m; 
			b = 0 + m; 
	    }
		
		// trientsphere 1
		if (hue >= 240 && hue < 300) {
			r = x + m;  
			g = 0 + m;  
			b = c + m; 
	    }
		
		// trientsphere 2
		if (hue >= 120 && hue < 180) {
			r = 0 + m; 
			g = c + m; 
			b = x + m; 
	    }
	    
		// trientsphere 2
		if (hue >= 300 && hue < 360) {
			r = c + m; 
			g = 0 + m; 
			b = x + m;
		}
		
		return ARGB(alpha, r, g, b);
	}
}
*/

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
	
	static void Animate(Widget source, WidgetAnimatorProperty property, float end_value, int time)
	{
		if (!source) {
			return;
		}
		
		if (!m_RunningTimers) {
			m_RunningTimers = {};
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
		
		if (!m_RunningTimers) {
			m_RunningTimers = {};
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
		
		if (!m_RunningTimers) {
			m_RunningTimers = {};
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
		
		if (!m_RunningTimers) {
			m_RunningTimers = {};
		}
		
		if (HasAnimation(source, property)) {
			CancelAnimate(source, property);
		}
		
		WidgetAnimationTimer animation_timer = new WidgetAnimationTimer();
		animation_timer.Run(source, property, start_value, end_value, time, true);
		m_RunningTimers.Insert(animation_timer);
	}
	
	static void AnimateColor(Widget source, int end_color, int time, bool loop = false)
	{
		if (!source) {
			return;
		}
		
		if (!m_RunningTimers) {
			m_RunningTimers = {};
		}
		
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
	
	static void AnimateColor(Widget source, int start_color, int end_color, int time, bool loop = false)
	{
		if (!source) {
			return;
		}
		
		if (!m_RunningTimers) {
			m_RunningTimers = {};
		}
				
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
	
	static void AnimateColorHSV(Widget source, vector start_color, vector end_color, int time, bool loop = false)
	{
		if (!source) {
			return;
		}
		
		if (!m_RunningTimers) {
			m_RunningTimers = {};
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
			if (widget_animation_timer && widget_animation_timer.GetWidget() == source && (widget_animation_timer.GetWidgetAnimatorProperty() & property) == widget_animation_timer.GetWidgetAnimatorProperty()) {
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