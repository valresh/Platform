// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// opapps_timecontrol.js

hscOpAppsTimeControl = function () {
	var src = "opapps_timecontrol.js";
	var _persistKey = window.location.pathname + "TimeControlData";

	var _playing = true;
	var _playingContinously = false;
	var _ready = false;
	var _timerId;
	var _timerContId;
	var _form;

	var _datePicker;
	var _timePicker;
	var _baselineDatePicker;
	var _baselineTimePicker;

	var _period;
	var _interval;
	var _baselineOffset;
	var _pauseButton;
	var _playButton;
	var _pausedButton;
	var _liveButton;
	var _baselineLabel;
	var _intervalLabel;
	var _periodLabel;

	var _displayHolder;
	var _keepMediumNumSamples = true;	// if true, ValidateInterval() and ValidatePeriod() will maintain a "medium" number of samples so time series plots will have a reasonable number of samples.
										// if false, ValidateInterval() and ValidatePeriod() will allow the number of samples to be low but will not allow interval to be > period.
	var _usingPeriodInterval = false;
	var _usingBaseline = false;

	var _resizing = false;

	var _periods = [	// If these change then _period.on('change') needs to change also
		{ "value": 1, "label": "1 Minute" },
		{ "value": 5, "label": "5 Minutes" },
		{ "value": 20, "label": "20 Minutes" },
		{ "value": 60, "label": "1 Hour" },
		{ "value": 120, "label": "2 Hours" },
		{ "value": 240, "label": "4 Hours" },
		{ "value": 480, "label": "8 Hours" },
		{ "value": 720, "label": "12 Hours" },
		{ "value": 1440, "label": "1 Day" },
		{ "value": 2880, "label": "2 Days" },
		{ "value": 7200, "label": "5 Days" },
		{ "value": 10080, "label": "1 Week" },
		{ "value": 20160, "label": "2 Weeks" },
		{ "value": 80640, "label": "4 Weeks" },
		{ "value": 120960, "label": "3 Months" },
		{ "value": 241920, "label": "6 Months" },
		{ "value": 525600, "label": "1 Year" }
	];

	var _intervals = [	// If these change then _interval.on('change') needs to change also
		{ "value": 5000, "label": "5 Second" },
		{ "value": 60000, "label": "1 Minute" },
		{ "value": 3600000, "label": "1 Hour" },
		{ "value": 28800000, "label": "8 Hour" },
		{ "value": 86400000, "label": "24 Hour" },
	];

	var _baselineOffsets = [	// If these change then _interval.on('change') needs to change also
		{ "value": 0, "label": "Fixed" },
		{ "value": 60, "label": "Time - 1 hour" },
		{ "value": 480, "label": "Time - 8 hours" },
		{ "value": 1440, "label": "Time - 1 day" },
		{ "value": 10080, "label": "Time - 1 week" },
		{ "value": 20160, "label": "Time - 2 weeks" },
		{ "value": 80640, "label": "Time - 4 weeks" },
		{ "value": 120960, "label": "Time - 3 months" },
		{ "value": 241920, "label": "Time - 6 months" },
		{ "value": 525600, "label": "Time - 1 year" }
	];

	var _TimeControlDataDefaults = {
		"playing": true,
		"time": "iso",			// NB "iso" causes hscCheckRequiredFields to check for date
		"baselineTime": "iso",
		"baselineOffset": 1440,
		"period": 60,
		"interval": 60000
	}

	var _lastRequestDate;

	function OnReady(holder, keepMediumNumSamples) {
		// Throws exception if time control cannot be made ready
		var DatePickerId = "datepicker";
		var TimePickerId = "timepicker";
		var BaselineDatePickerId = "datepicker0";
		var BaselineTimePickerId = "timepicker0";

		var BaselineOffsetId = "#offsetpicker";
		var PeriodId = "#period";
		var IntervalId = "#Interval";
		var PauseId = "#btnPause";
		var PausedId = "#btnPaused";
		var LiveId = "#btnLive";
		var PlayId = "#btnPlay";
		var BaselineLabelId = "#lblDatetimepicker0";
		var IntervalLabelId = "#lblInterval";
		var PeriodLabelId = "#lblPeriod";
		var FormId = "#frmTimeControl";

		_ready = false;

		if (keepMediumNumSamples != null)
			_keepMediumNumSamples = keepMediumNumSamples;

		try {
			if (!(typeof holder === 'object'))
				throw "holder passed in is not an object.";

			_datePicker = document.getElementById(DatePickerId);
			if (_datePicker === null)
				throw "Unable to find element with id of " + DatePickerId + " on the page.";

			_timePicker = document.getElementById(TimePickerId);
			if (_timePicker === null)
				throw "Unable to find element with id of " + TimePickerId + " on the page.";

			_baselineDatePicker = document.getElementById(BaselineDatePickerId);
			if (_baselineDatePicker === null)
				throw "Unable to find element with id of " + BaselineDatePickerId + " on the page.";

			_baselineTimePicker = document.getElementById(BaselineTimePickerId);
			if (_baselineTimePicker === null)
				throw "Unable to find element with id of " + BaselineTimePickerId + " on the page.";

			_baselineOffset = $(BaselineOffsetId);
			if (_baselineOffset.length == 0)
				throw "Unable to find element with id of " + BaselineOffsetId + " on the page.";

			_period = $(PeriodId);
			if (_period.length == 0)
				throw "Unable to find element with id of " + PeriodId + " on the page.";

			_interval = $(IntervalId);
			if (_interval.length == 0)
				throw "Unable to find element with id of " + IntervalId + " on the page.";

			_pauseButton = $(PauseId);
			if (_pauseButton.length == 0)
				throw "Unable to find element with id of " + PauseId + " on the page.";

			_playButton = $(PlayId);
			if (_playButton.length == 0)
				throw "Unable to find element with id of " + PlayId + " on the page.";

			_pausedButton = $(PausedId);
			if (_pausedButton.length == 0)
				throw "Unable to find element with id of " + PausedId + " on the page.";

			_liveButton = $(LiveId);
			if (_liveButton.length == 0)
				throw "Unable to find element with id of " + LiveId + " on the page.";

			_baselineLabel = $(BaselineLabelId);
			if (_baselineLabel.length == 0)
				throw "Unable to find element with id of " + BaselineLabelId + " on the page.";

			_intervalLabel = $(IntervalLabelId);
			if (_intervalLabel.length == 0)
				throw "Unable to find element with id of " + IntervalLabelId + " on the page.";

			_periodLabel = $(PeriodLabelId);
			if (_periodLabel.length == 0)
				throw "Unable to find element with id of " + PeriodLabelId + " on the page.";

			_form = $(FormId);
			if (_form.length == 0)
				throw "Unable to find element with id of " + FormId + " on the page.";

			_displayHolder = holder;

			_datePicker.attachEvent('Change', function () {	// TODO may also need to detachEvent at some point?
				// TODO Refactor as it's the same as below
				Pause();
				UpdateBaselineTime();
				RequestData();
			});

			_timePicker.attachEvent('Change', function () {	// TODO may also need to detachEvent at some point?
							Pause();
							UpdateBaselineTime();
							RequestData();
			});

			_baselineDatePicker.attachEvent('Change', function () {	// TODO may also need to detachEvent at some point?
				// TODO Refactor as it's the same as below
							_baselineOffset.val(0);
							UpdateBaselineOffsetDisabled();
							RequestData();
			});

			_baselineTimePicker.attachEvent('Change', function () {	// TODO may also need to detachEvent at some point?
				_baselineOffset.val(0);
				UpdateBaselineOffsetDisabled();
				RequestData();
			});

			_pausedButton.mouseup(function(event) {
				event.preventDefault();				// this prevents only a default action but previously assigned listeners will be called
				event.stopImmediatePropagation()	// if there are  others listeners which that shouldn't call 
			});

			_pausedButton.click(function (event) {
				event.preventDefault();				// this prevents only a default action but previously assigned listeners will be called
				event.stopImmediatePropagation()	// if there are  others listeners which that shouldn't call 
			});

			_pausedButton.mouseenter(function (event) {
				event.preventDefault();				// this prevents only a default action but previously assigned listeners will be called
				event.stopImmediatePropagation()	// if there are  others listeners which that shouldn't call 
			});

			_liveButton.mouseup(function (event) {
				event.preventDefault();				// this prevents only a default action but previously assigned listeners will be called
				event.stopImmediatePropagation()	// if there are  others listeners which that shouldn't call 
			});

			_liveButton.click(function (event) {
				event.preventDefault();				// this prevents only a default action but previously assigned listeners will be called
				event.stopImmediatePropagation()	// if there are  others listeners which that shouldn't call 
			});

			_liveButton.mouseenter(function (event) {
				event.preventDefault();				// this prevents only a default action but previously assigned listeners will be called
				event.stopImmediatePropagation()	// if there are  others listeners which that shouldn't call 
			});

			_form.submit(function (event) {
				// This is to prevent the user pressing enter on the time control form and causing a page navigation
				event.preventDefault();
				event.stopImmediatePropagation();
			});

			PopulateControls();
			_ready = true;
		}

		catch (err) {
			var errorString = err + "<BR>Unable to make time control ready.";
			LogErrorMessage(src, errorString.replace("<BR>", " "));
			throw errorString;
		}

		_pauseButton.on('click', function (e) {
			try {
				Pause();
				return false;
			}
			catch (err) {
				LogErrorMessage(src, "_pauseButton click: " + err);
			}
		});

		_playButton.on('click', function (e) {
			try {
				Play();
				return false;
			}
			catch (err) {
				LogErrorMessage(src, "_playButton click: " + err);
			}
		});

		_period.on('change', function (e) {
			ValidateInterval();
			RequestData();
		});

		_interval.on('change', function (e) {
			ValidatePeriod();
			RequestData();
		});

		_baselineOffset.on('change', function (e) {
			UpdateBaselineOffsetDisabled();
			if (parseInt(_baselineOffset.val()) != 0) {
				// Only update if baseline is linked to the main time
				UpdateBaselineTime();
				RequestData();
			}
			else
				PersistTimeControlData();
		});
	}

	function PopulateControls() {
		try {
			// Set default contents and values for date time controls
			var TimeControlData = _TimeControlDataDefaults;
			if (PersistedData) {
				try {
					var TimeControlData = JSON.parse(PersistedData(_persistKey, JSON.stringify(_TimeControlDataDefaults)));
					hscCheckRequiredFields(_TimeControlDataDefaults, TimeControlData);
				}
				catch (err) {
					TimeControlData = _TimeControlDataDefaults;
				}
			}

			if ((TimeControlData.time == "iso") || (TimeControlData.baselineTime == "iso")) {
				// Eliminate the "iso"s which will be there on the first callup
				TimeControlData.time = new Date();
				TimeControlData.baselineTime = new Date(TimeControlData.time - 24 * 60 * 60 * 1000);
			}
			else {
				// Parse the date/time strings
				TimeControlData.time = hscOpAppsISO.parse(TimeControlData.time);
				TimeControlData.baselineTime = hscOpAppsISO.parse(TimeControlData.baselineTime);
			}

			// Populate period select element options and check that period is a valid choice
			var found = false;
			_periods.forEach(function (choice) {
				_period.append(new Option(choice.label, choice.value));
				if (choice.value == TimeControlData.period) {
					_period.val(TimeControlData.period);
					found = true;
				}
			});
			if (!found)
				throw "Persisted period of '" + TimeControlData.period + "' is not a valid choice.";

			// Populate interval select element options and check that interval is a valid choice
			found = false;
			_intervals.forEach(function (choice) {
				_interval.append(new Option(choice.label, choice.value));
				if (choice.value == TimeControlData.interval) {
					_interval.val(TimeControlData.interval);
					found = true;
				}
			});
			if (!found)
				throw "Persisted interval of '" + TimeControlData.interval + "' is not a valid choice.";

			// Ensure the period/interval combo is OK as we don't want to fetch too much data
			ValidateInterval();

			// Populate baseline offset select element options and check that baseline offset is a valid choice
			found = false;
			_baselineOffsets.forEach(function (choice) {
				_baselineOffset.append(new Option(choice.label, choice.value));
				if (choice.value == TimeControlData.baselineOffset) {
					_baselineOffset.val(TimeControlData.baselineOffset);
					found = true;
				}
			});

			if (!found)
				throw "Persisted baseline offset of '" + TimeControlData.baselineOffset + "' is not a valid choice.";

			UpdateBaselineOffsetDisabled();

			if (TimeControlData.playing) {
				UpdateBaselineTime(TimeControlData.baselineTime);
				Play();
			}
			else {
				Pause();
				UpdateTime(TimeControlData.time);
				UpdateBaselineTime(TimeControlData.baselineTime);
			}
		}
		catch (err) {
			LogErrorMessage(src, "PopulateControls: " + err);
			throw "PopulateControls: " + err;
		}
	}

	function UpdateTime(_) {
		try {
			_datePicker.Value = _.getVarDate();
			_timePicker.Value = _.getVarDate();
		}
		catch (err) {
			LogErrorMessage(src, "UpdateTime: " + err);
			throw "UpdateTime: " + err;
		}
	}

	function UpdateLastRequestTime(_){
	    try {
	        _lastRequestDate = new Date();
	    }
	    catch (err) {
	        LogErrorMessage(src, "UpdateLastRequestTime: " + err);
	        throw "UpdateLastRequestTime: " + err;
	    }
	}

	function UpdateBaselineTime(_) {
		try {
			if (parseInt(_baselineOffset.val()) != 0) {
				// Baseline time is dependant upon the main time
				if (parseDateTime() != null) {
					var temp = calculateOffsetTime(_baselineOffset.val());
					_baselineDatePicker.Value = temp.getVarDate();
					_baselineTimePicker.Value = temp.getVarDate();
				}
			}
			else
				// Baseline time is independant of the main time
				if (arguments.length == 1) {
					// Assume single argument provided is the baseline date/time
					_baselineDatePicker.Value = _.getVarDate();
					_baselineTimePicker.Value = _.getVarDate();
				}
		}
		catch (err) {
			LogErrorMessage(src, "UpdateBaselineTime: " + err);
			throw "UpdateBaselineTime: " + err;
		}
	}

	function UpdateBaselineOffsetDisabled() {
		try {
			// If the baseline time is dependant upon the main time, then disable editing of the baseline input element.
			// This prevents clashes any between the user editing it and play mode automatically updating it.
			_baselineDatePicker.Enabled = _baselineTimePicker.Enabled = (parseInt(_baselineOffset.val()) == 0);
		}
		catch (err) {
			LogErrorMessage(src, "UpdateBaselineOffsetDisabled: " + err);
			throw "UpdateBaselineOffsetDisabled: " + err;
		}
	}

	function UpdatePausedVisibility() {
		try {
			if (_playing) {
				// Hide the paused button
				_pausedButton.hide();
				if (_usingBaseline || _usingPeriodInterval)
					// Show the hidden live button so controls to the right done jump around
					_liveButton.show();
				else
					// Hide the hidden live button so that the loading indicator is immediately to the right of this "Time" controls
					_liveButton.hide();
			}
			else {
				// Show the paused button
				_pausedButton.show();
				_liveButton.hide();
			}
		}
		catch (err) {
			LogErrorMessage(src, "UpdatePausedVisibility: " + err);
			throw "UpdatePausedVisibility: " + err;
		}
	}

	function ValidateInterval() {
		// Validates the chosen Interval against the chosen period and adjusts it if necessary
		// The larger the interval, the fewer the number of samples
		try {
			var interval = parseInt(_interval.val());
			if (_keepMediumNumSamples) {
				switch (parseInt(_period.val())) {
					case 1: 		// 1 Minute
					case 5: 		// 5 Minutes
						interval = 5000;							// Force interval to be 5 seconds
						break;
					case 20:		// 20 Minutes
						interval = Math.min(interval, 60000);		// At most an interval of 1 minute
						break;
					case 60:		// 1 Hour
						interval = Math.min(interval, 60000);		// At most an interval of 1 minute
						break;
					case 120:		// 2 Hours
					case 240:		// 4 Hours
					case 480:		// 8 Hours
						interval = 60000;							// Force interval to be 1 minute
						break;
					case 720:		// 12 Hours
					case 1440:		// 1 Day
						interval = Math.min(interval, 3600000);		// At most an interval of 1 hour
						interval = Math.max(interval, 60000);		// At least an interval of 1 minute
						break;
					case 2880:		// 2 Days
						interval = 3600000;							// Force interval to be 1 hour
						break;
					case 7200:		// 5 Days
					case 10080:		// 1 Week
						interval = Math.min(interval, 28800000);	// At most an interval of 8 hrs
						interval = Math.max(interval, 3600000);		// At least an interval of 1 hour
						break;
					case 20160:		// 2 Weeks
						interval = Math.max(interval, 3600000);		// At least an interval of 1 hour
						break;
					case 80640:		// 4 Weeks
					case 120960:	// 3 Months
					case 241920:	// 6 Months
						interval = Math.max(interval, 28800000);	// At least an interval of 8hrs
						break;
					case 525600:	// 1 Year
						interval = Math.max(interval, 86400000);	// At least an interval of 24hrs
						break;
					default:
						LogErrorMessage(src, "Using defaults because of unexpected period of " + parseInt(_period.val()));
						// Use defaults
						_interval.val(_TimeControlDataDefaults.interval);
						_period.val(_TimeControlDataDefaults.period);
						return;
				}
			}
			else {
				switch (parseInt(_period.val())) {
					case 1: 		// 1 Minute
					case 5: 		// 5 Minutes
					case 20:		// 20 Minutes
						interval = Math.min(interval, 60000);		// At most an interval of 1 minute
						break;
					case 60:		// 1 Hour
						interval = Math.min(interval, 3600000);		// At most an interval of 1 hour
						break;
					case 120:		// 2 Hours
					case 240:		// 4 Hours
					case 480:		// 8 Hours
					case 720:		// 12 Hours
						interval = Math.min(interval, 3600000);		// At most an interval of 1 hour
						interval = Math.max(interval, 60000);		// At least an interval of 1 minute
						break;
					case 1440:		// 1 Day
						interval = Math.max(interval, 60000);		// At least an interval of 1 minute
						break;
					case 2880:		// 2 Days
					case 7200:		// 5 Days
					case 10080:		// 1 Week
					case 20160:		// 2 Weeks
						interval = Math.max(interval, 3600000);		// At least an interval of 1 hour
						break;
					case 80640:		// 4 Weeks
					case 120960:	// 3 Months
					case 241920:	// 6 Months
						interval = Math.max(interval, 28800000);	// At least an interval of 8hrs
						break;
					case 525600:	// 1 Year
						interval = Math.max(interval, 86400000);	// At least an interval of 24hrs
						break;
					default:
						LogErrorMessage(src, "Using defaults because of unexpected period of " + parseInt(_period.val()));
						// Use defaults
						_interval.val(_TimeControlDataDefaults.interval);
						_period.val(_TimeControlDataDefaults.period);
						return;
				}
			}
			_interval.val(interval);
		}
		catch (err) {
			LogErrorMessage(src, "ValidateInterval(): " + err);
		}
	}

	function ValidatePeriod() {
		// Validates the chosen Period against the chosen interval and adjusts it if necessary
		// The larger the period, the greater the number of samples
		try {
			var period = parseInt(_period.val());
			if (_keepMediumNumSamples) {
				switch (parseInt(_interval.val())) {
					case 5000:		// 5 second
						period = Math.min(period, 60);		// At most 1 hour of 5 second history
						break;
					case 60000:		// 1 minute
						period = Math.max(period, 20);		// At least 20 minutes of 1 minute history
						period = Math.min(period, 1440);	// At most 1 days of 1 minute history
						break;
					case 3600000:	// 1 hour
						period = Math.max(period, 720);		// At least 12 hours of 1 hour history
						period = Math.min(period, 20160);	// At most 2 weeks of 1 hour history
						break;
					case 28800000:	// 8 hour
						period = Math.max(period, 7200);	// At least 5 days of 8 hour history
						break;
					case 86400000:	// 24 hour
						period = Math.max(period, 20160);	// At least 2 weeks of 24 hour history
						break;
					default:
						LogErrorMessage(src, "Using defaults because of interval of " + parseInt(_interval.val()));
						// Use defaults
						_period.val(_TimeControlDataDefaults.period);
						_interval.val(_TimeControlDataDefaults.interval);
						return;
				}
			}
			else {
				switch (parseInt(_interval.val())) {
					case 5000:		// 5 second
						period = Math.min(period, 60);		// At most 1 hour of 5 second history
						break;
					case 60000:		// 1 minute
						period = Math.min(period, 1440);	// At most 1 day of 1 minute history
						break;
					case 3600000:	// 1 hour
						period = Math.max(period, 60);		// At least 1 hour of 1 hour history
						period = Math.min(period, 20160);	// At most 2 weeks of 1 hour history
						break;
					case 28800000:	// 8 hour
						period = Math.max(period, 1440);	// At least 1 day of 8 hour history
						break;
					case 86400000:	// 24 hour
						period = Math.max(period, 1440);	// At least 1 day of 8 hour history
						break;
					default:
						LogErrorMessage(src, "Using defaults because of interval of " + parseInt(_interval.val()));
						// Use defaults
						_period.val(_TimeControlDataDefaults.period);
						_interval.val(_TimeControlDataDefaults.interval);
						return;
				}
			}
			_period.val(period);
		}
		catch (err) {
			LogErrorMessage(src, "ValidatePeriod(): " + err);
		}
	}

	function UpdateNow() {
		// Returns true if main dateTimePicker time has changed, false if Now has stayed the same
		var oldNow = parseDateTime();
		var newNow = new Date();

		if (oldNow == null) {
			// Nothing in there yet
			UpdateTime(newNow);
			UpdateBaselineTime();
			return true;
		}

		var startOfDay = new Date(0);
		startOfDay.setHours(7);						// By default, 7:00 start of day

		var interval = 5000;						// By default

		if (_usingPeriodInterval)
			interval = parseInt(_interval.val());

		// Snap the new Now so that it is equal to a multiple of the interval from the start of the day
		newNow = new Date(Math.floor((newNow.getTime() - startOfDay.getTime()) / interval) * interval + startOfDay.getTime());

		if (newNow.getTime() != oldNow.getTime()) {
			UpdateTime(newNow);
			UpdateBaselineTime();
			return true;
		}
		else
			return false;
	}

	function PersistTimeControlData() {
		try {
			if (PersistData) {
				var TimeControlData = {
					"playing": _playing,
					"time": hscOpAppsISO(parseDateTime()),
					"baselineTime": hscOpAppsISO(parseBaselineDateTime()),
					"baselineOffset": parseInt(_baselineOffset.val()),
					"period": parseInt(_period.val()),
					"interval": parseInt(_interval.val())
				}
				PersistData(_persistKey, JSON.stringify(TimeControlData));
			}
		}
		catch (err) {
			// If it can't be persisted there is no great loss and probably a reason why
			LogMessage(L_PRINT, src, "PersistTimeControlData: " + err);
		}
	}

	function Play() {
		try {
			_pauseButton[0].disabled = false;
			_playButton[0].disabled = true;
			_timePicker.Enabled = _datePicker.Enabled = false;

			_playing = true;
			UpdatePausedVisibility();

			UpdateNow();
			RequestData();	// Transitioning to play mode, so request data regardless of whether current time has changed

			if (_timerId != null) {
				clearTimeout(_timerId);
				_timerId = null;
			}

			_timerId = setInterval(function () {
				if (_playing) {
				    if (UpdateNow()) {
						RequestData();
					}
				}
			}, 1000);
		}
		catch (err) {
			LogErrorMessage(src, "Play: " + err);
			throw "Play: " + err;
		}
	}

	function PlayContinously() {

	    _playingContinously = true;
	    try {
            	        
	        if (_lastRequestDate == null) {
	            _lastRequestDate = new Date();
	        }

	        // Continually make server request, without being subject to of changing the current leak detection time control
	        if (_timerContId != null) {
	            clearTimeout(_timerContId);
	            _timerContId = null;
	        }

	        var interval = 5; // request interval

	        _timerContId = setInterval(function () {

	            var tdate = new Date(_lastRequestDate);
	            var newNow = new Date();
                	            
	            tdate.setSeconds(_lastRequestDate.getSeconds() + interval);
	            

	            if (newNow.getTime() >= tdate.getTime()) {
	                LogErrorMessage(src, "PlayContinuously display request made");
	                UpdateLastRequestTime();
	                RequestData();
	            }
	        }, 1000);
	    }
	    catch (err) {
	        LogErrorMessage(src, "PlayContinously: " + err);
	        throw "PlayContinously: " + err;
	    }
	}

	function Pause() {
		try {
			_pauseButton[0].disabled = true;
			_playButton[0].disabled = false;

			_timePicker.Enabled = _datePicker.Enabled = true;

			_playing = false;
			UpdatePausedVisibility();

			PersistTimeControlData();

			if (_timerId != null) {
				clearTimeout(_timerId);
				_timerId = null;
			}
		}
		catch (err) {
			LogErrorMessage(src, "Pause: " + err);
			throw "Pause: " + err;
		}
	}

	function parseDateTime(dateTimeElem) {
		var theTime = new Date(_timePicker.Value);
		var theDateTime = new Date(_datePicker.Value);
		theDateTime.setHours(theTime.getHours(), theTime.getMinutes(), theTime.getSeconds(), theTime.getMilliseconds());

		return theDateTime;
	}

	function parseBaselineDateTime(dateTimeElem) {
		var theTime = new Date(_baselineTimePicker.Value);
		var theDateTime = new Date(_baselineDatePicker.Value);
		theDateTime.setHours(theTime.getHours(), theTime.getMinutes(), theTime.getSeconds(), theTime.getMilliseconds());

		return theDateTime;
	}

	function calculateOffsetTime(selectedPeriod) {	// selectedPeriod is in minutes
		var endTime = parseDateTime();
		var shiftMsecs = 60 * 1000;		// 1 minute's worth of milliseconds

		var numMonths = 0;				// Leave as 0 if no month manipulation is required
		switch (selectedPeriod) {
			case "120960":
				numMonths = 3;
				break;
			case "241920":
				numMonths = 6;
				break;
			case "525600":
				numMonths = 12;
				break;
			default:
				shiftMsecs *= selectedPeriod;
				break;
		}

		var startTime;
		if (numMonths > 0) {
			startTime = new Date(endTime);
			var startMonth = startTime.getMonth() - numMonths;
			if (startMonth < 0) {
				// Note this won't work for multiple years back
				startTime.setMonth(startMonth + 12);
				startTime.setFullYear(startTime.getFullYear() - 1);
			}
			else
				startTime.setMonth(startMonth);
		}
		else
			startTime = new Date(endTime.getTime() - shiftMsecs);

		return startTime;
	}

	function RequestData() {
	    PersistTimeControlData();
		_displayHolder.RequestData();
	}

	return {
		// Public methods - Note all except OnReady() check _ready to ensure things are setup properly
		OnReady: function (holder, keepMediumNumSamples) {
			return OnReady(holder, keepMediumNumSamples);
		},
		endTime: function () {
			if (!_ready)
				throw "Time control is not ready in endTime().";
			return parseDateTime();
		},
		startTime: function () {
			if (!_ready)
				throw "Time control is not ready in startTime().";
			return calculateOffsetTime(_period.val());
		},
		baselineTime: function () {
			if (!_ready)
				throw "Time control is not ready in baselineTime().";
			return parseBaselineDateTime();
		},
		interval: function () {
			if (!_ready)
				throw "Time control is not ready in interval().";
			return parseInt(_interval.val());
		},
		playing: function () {
			if (!_ready)
				throw "Time control is not ready in playing().";
			return _playing;
		},
		playingContinously: function () {
		    if (!_ready)
		        throw "Time control is not ready in playingContinously().";
		    return _playingContinously;
		},
		ShowBaseline: function () {
			if (!_ready)
				throw "Time control is not ready in ShowBaseline().";
			$(_baselineDatePicker).show();
			$(_baselineTimePicker).show();
			_baselineOffset.show();
			_baselineLabel.show();
			_usingBaseline = true;
			UpdatePausedVisibility();
		},
		HideBaseline: function () {
			if (!_ready)
				throw "Time control is not ready in HideBaseline().";
			$(_baselineDatePicker).hide();
			$(_baselineTimePicker).hide();
			_baselineOffset.hide();
			_baselineLabel.hide();
			_usingBaseline = false;
			UpdatePausedVisibility();
		},
		ShowPeriodInterval: function () {
			if (!_ready)
				throw "Time control is not ready in ShowPeriodInterval().";
			_interval.show();
			_intervalLabel.show();
			_period.show();
			_periodLabel.show();
			_usingPeriodInterval = true;
			UpdatePausedVisibility();
		},
		HidePeriodInterval: function () {
			if (!_ready)
				throw "Time control is not ready in HidePeriodInterval().";
			_interval.hide();
			_intervalLabel.hide();
			_period.hide();
			_periodLabel.hide();
			_usingPeriodInterval = false;
			UpdatePausedVisibility();
		},
		Pause: function () {
			if (!_ready)
				throw "Time control is not ready in Pause().";
			Pause();
		},
		PlayContinously: function () {
			if (!_ready)
				throw "Time control is not ready in PlayContinously().";
			PlayContinously();
		}
		

	}
}();

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/