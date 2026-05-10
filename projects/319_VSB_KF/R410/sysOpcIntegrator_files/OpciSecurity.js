var g_oEventSink = null;

if( g_oEventSink == null )
		{
			g_oEventSink = new ActiveXObject("HDXElements.EventSink");
			g_oEventSink.Advise(window.external.Application);
			g_oEventSink.AttachEvent("OnOperatorChange", OnOperatorChange);
		}
		
function OnOperatorChange()
{
	ApplySecurity();
}