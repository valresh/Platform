// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// opapps_navigation.js

hscOpAppsNavControl = function () {
	var src = "opapps_navigation.js";

	var _displayCollection;
	var _displayHolder;
	var _currentDisplay;

	var _persistKey = window.location.pathname + "NavControlData";

	var _ready = false;

	function OnReady(holder, displays) {
		var tempStr;

		try {
			if (!(typeof holder === 'object'))
				throw "holder passed in is not an object.";
			if (!(typeof displays === 'object'))
				throw "displays passed in is not an object.";

			_displayHolder = holder;
			if (!(typeof _displayHolder.ChangeChart === 'function'))
				throw "holder doesn't have a ChangeChart function.";
			if (!(typeof _displayHolder.OnSilverlightPluginLoaded === 'function'))
				throw "holder doesn't have a ChangeChart function.";
			if (!(typeof _displayHolder.RequestData === 'function'))
				throw "holder doesn't have a ChangeChart function.";

			_displayCollection = displays;
			_displayCollection.forEach(function (value) {
				if (!(typeof value.AdjustTimeControl === 'function'))
					throw "A display doesn't have an AdjustTimeControl function.";
				if (!(typeof value.ClearChart === 'function'))
					throw "A display doesn't have a ClearChart function.";
				if (!(typeof value.DrawChart === 'function'))
					throw "A display doesn't have a DrawChart function.";
				if (!(typeof value.FormRequest === 'function'))
					throw "A display doesn't have a FormRequest function.";
				if (!(typeof value.OnReady === 'function'))
					throw "A display doesn't have an OnReady function.";
				if (!(typeof value.ParseResponse === 'function'))
					throw "A display doesn't have a ParseResponse function.";
				if (!(typeof value.Title === 'function'))
					throw "A display doesn't have a Title function.";
				if (!(typeof value.Type === 'function'))
					throw "A display doesn't have a Type function.";
			});
		}
		catch (err) {
			var errorString = err + "<BR>Unable to make nav control ready.";
			LogErrorMessage(src, errorString.replace("<BR>", " "));
			throw errorString;
		}

		_ready = true;
	}

	function FindDisplayType(displayType) {
		var foundDisplayType;
		_displayCollection.forEach(function (value) {
			if (displayType.toLowerCase() === value.Type().toLowerCase())
				foundDisplayType = value;
		});
		return foundDisplayType;
	}

	function NavigateTo(displayType) {
		var navDisplayType = FindDisplayType(displayType);

		if (navDisplayType) {
			if (_currentDisplay)
				_currentDisplay.ClearChart();
			_currentDisplay = navDisplayType;
			_displayHolder.ChangeChart(_currentDisplay);
			_displayHolder.RequestData();

			PersistNavControlData(displayType);
		}
		else {
			throw "Unknown 'displaytype' of '" + displayType + "' specified.";
		}
	}

	function NavigateToDefault(hardCodedDisplayType) {
		var displayType = (hardCodedDisplayType == null ? GetURLParam("displaytype") : hardCodedDisplayType);
		var point = GetURLParam("currentpoint");

		if (displayType != "") {
			// Non-blank displayType URL parameter overrides persisted data
			if (point == "")
				throw "URL parameter 'currentpoint' not supplied.";

			if (point === hscDummyPointName)
				// Sample data to be used
				hscGlobalCompressorPoint = hscGlobalPipelinePoint = hscGlobalSegmentPoint = hscGlobalRoutePoint = hscDummyPointName;

			switch (displayType.toLowerCase()) {
				case _DISPLAYTYPE_PipelinePressureProfile.toLowerCase():
				case _DISPLAYTYPE_PipelineInferredFlowProfile.toLowerCase():
					hscGlobalPipelinePoint = point;
					break;
				case _DISPLAYTYPE_SegmentInferredFlow.toLowerCase():
				case _DISPLAYTYPE_DifferentialPressure.toLowerCase():
				case _DISPLAYTYPE_SegmentMassBalance.toLowerCase():
					hscGlobalSegmentPoint = point;
					break;
				case _DISPLAYTYPE_CompressorMap.toLowerCase():
					hscGlobalCompressorPoint = point;
					break;
				case _DISPLAYTYPE_RouteProfile.toLowerCase():
					hscGlobalRoutePoint = point;
					break;
				default:
					throw "URL parameter 'displaytype' specifies an unknown display type of '" + displayType + "'.";
			}
		}
		else {
			// Attempt to get from persisted data
			displayType = GetPersistedNavControlData();
		}

		if (displayType == "")
			throw "URL parameter 'displaytype' not supplied.";

		// Attempt to navigate to the resultant display type
		NavigateTo(displayType);
	}

	function PersistNavControlData(displayType) {
		if (PersistData) {
			var NavControlData = {
				"displayType": displayType,
				"pipelinePoint": hscGlobalPipelinePoint,
				"routePoint": hscGlobalRoutePoint,
				"segmentPoint": hscGlobalSegmentPoint
			}

			PersistData(_persistKey, JSON.stringify(NavControlData));
		}
	}

	function GetPersistedNavControlData() {
		var NavControlDataDefaults = {
			"displayType": "",
			"pipelinePoint": "",
			"routePoint": "",
			"segmentPoint": ""
		}

		var NavControlData;
		if (PersistedData) {
			try {
				var NavControlData = JSON.parse(PersistedData(_persistKey, JSON.stringify(NavControlDataDefaults)));
				hscCheckRequiredFields(NavControlDataDefaults, NavControlData);
			}
			catch (err) {
				NavControlData = NavControlDataDefaults;
			}
		}

		hscGlobalPipelinePoint = NavControlData.pipelinePoint;
		hscGlobalRoutePoint = NavControlData.routePoint;
		hscGlobalSegmentPoint = NavControlData.segmentPoint;

		return NavControlData.displayType;
	}

	return {
		// Public methods - Note all except OnReady() check _ready to ensure things are setup properly
		OnReady: function (holder, displays) {
			return OnReady(holder, displays);
		},
		NavigateTo: function (displayType) {
			if (!_ready)
				throw "Nav control is not ready.";
			return NavigateTo(displayType);
		},
		NavigateToDefault: function (hardCodedDisplayType) {
			if (!_ready)
				throw "Nav control is not ready.";
			return NavigateToDefault(hardCodedDisplayType);
		}
	}
}();

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/