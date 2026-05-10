requirejs.config({
	baseUrl: window.external.FrameworkServices.HMIWebJSBasePath + '/lib/',
	paths: 
	{
		rxjs: 'rxjs.umd.min',
		expsvcdsa: 'HMIWebExperionServicesDsaAdvSec',
	},
	shim: {
		'expsvcdsa': {
            deps: ['rxjs']
		}
	},
	waitseconds: 0
});

// stores the parameters and their values passed in the URL into a map indexed by param name
function getParamsFromUrl() 
{
	// declare map for param and values
	var paramMap = {};
	
	// iterate over each parameter in the URL
	var parts = window.location.href.replace(
		/[?&]+([^=&]+)=([^&]*)/gi, 
		function(m,paramName,paramValue) 
		{
			// store the param and its value in the map
        	paramMap[paramName] = paramValue;
		});
	
	// return the map
    return paramMap;
}

// returns the value of a parameter passed in the URL, with defaultValue used if parameter not found
function getParamFromUrl(paramName, defaultValue)
{
	// set the default value
	var paramValue = defaultValue;
	
	// check if param is in URL
	if(window.location.href.indexOf(paramName) > -1)
	{
		// get param value for param from map
        paramValue = getParamsFromUrl()[paramName];
	}
	
	// return param value (or default value if param not found)
    return paramValue;
}


// the main routine
require(['expsvcdsa'], function (ExpSvcDsa) 
{
	var EngrSecLvl = 4;
	var Visible = "visible";
	var Hidden = "hidden";

	// get set password button object
	var cmdSetPassword = document.getElementById("cmdSetPassword");
		
	// get page object
	var currentPage = document.getElementById("Page");
	currentPage.onpagecomplete = function()
	{
		// get current security level
		var currentSecLvl = window.external.application.SecurityLevel;

		// check if user has sufficient security
		if(currentSecLvl >= EngrSecLvl)
		{
			// show set password button as user is >= ENGR
			cmdSetPassword.style.visibility = Visible;
		}
		else
		{  
			// hide set password button as user is < ENGR
			cmdSetPassword.style.visibility = Hidden;
		}
	}

	currentPage.onoperatorchange = function()
	{
		// get current security level
		var currentSecLvl = window.external.application.SecurityLevel;
		
		// check if user has sufficient security
		if(currentSecLvl >= EngrSecLvl)
		{
			// show set password button as user is >= ENGR
			cmdSetPassword.style.visibility = Visible;
		}
		else
		{  
			// hide set password button as user is < ENGR
			cmdSetPassword.style.visibility = Hidden;
		}
	}

	// create and register the GRPC service endpoint
	var dsaService = new ExpSvcDsa();
	dsaService.register();

	// handle the set password onclick
	cmdSetPassword.onclick = function() 
	{
		// get current security level
		var currentSecLvl = window.external.application.SecurityLevel;

		// get values for set password call
		var pwd = document.getElementById("txtPassword").value;  
		//var dsaconn = window.external.application.CurrentRecordOffset;  // always returns 0
		var dsaconn = parseInt(getParamFromUrl('hscrecord','0'));	// so get it from the URL

		// check if user has sufficient security
		if(currentSecLvl >= EngrSecLvl)
		{
			// user is >= ENGR so set password
			dsaService.dsaAdvancedSecurityService.setConnectionPassword(dsaconn,pwd).subscribe(
				function onNext(data) {
					// handle normal case

					// log event
					console.log("Received onNext. " + data.result);

					// output message from server so user knows password was saved (or not)
					window.external.application.TemporaryMessageZoneText(data.message);

					// close popup
					window.external.close();
				},
				function onError(data) {
					// handle error case

					// log event
					console.log("Received onError. " + data);

					// close popup
					window.external.close();
				},
				function onComplete(data) {
					// handle complete case

					// log event
					console.log("Received onComplete. No more data updates will be received");

					// close popup
					window.external.close();
				}
			);
		}
	};

	// handle the user cancelling the operation
	var cmdCancel = document.getElementById("cmdCancel");
	cmdCancel.onclick = function() 
	{
		// close popup
		window.external.close();
	}

});