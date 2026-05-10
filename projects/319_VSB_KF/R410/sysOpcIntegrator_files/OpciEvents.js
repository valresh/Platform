var g_oOnResponseEventSink = null;

if(g_oOnResponseEventSink == null)
{
	g_oOnResponseEventSink = new ActiveXObject("HDXElements.EventSink");
	g_oOnResponseEventSink.Advise(window.external.Application);
	g_oOnResponseEventSink.AttachEvent("OnResponse", OnResponse);
}

function OnResponse()
{
	// Fire the event to OpciFunctions.vbs
	GotResponse();
}