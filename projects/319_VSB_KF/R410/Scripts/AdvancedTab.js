document.onreadystatechange=fnStartInit;
function fnStartInit()
{
   if (document.readyState=="complete")
   {
    var protocol = window.external.WebServerProtocol;
	var serverName = window.external.ServerName;
	var path = "/PointDataWeb/pointdetail.html?PointName=";
	var pointId = window.external.DetailPointID;
	var url = protocol + serverName + path + pointId;
	document.getElementById("AdvancedTab").setAttribute("src", url);
	protocol = null;
	serverName = null;
	path = null;
	pointId = null;
	url = null;
   }
}