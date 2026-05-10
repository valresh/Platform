var arrAlarmLimitsInfo = [];
function Standard_AlarmLimit(oSource, sFBName)
{
   if (oSource.QualityGood("TagName." + sFBName + ".pveuhi") && oSource.QualityGood("TagName." + sFBName + ".pveulo"))
    {
        var oPVHighHigh = oSource.parentElement.parentElement.Objects("indpvHighHigh");
        var oPVHigh = oSource.parentElement.parentElement.Objects("indpvHigh");
        var oPVLow = oSource.parentElement.parentElement.Objects("indpvLow");
        var oPVLowLow = oSource.parentElement.parentElement.Objects("indpvLowLow");
        //Reset all like FillColour, LevelFillColour, Visibility , ToolTip
        oPVHighHigh.Level = 0;
        oPVHighHigh.levelFillColor = 'transparent';
        oPVHighHigh.FillColor = 'transparent';
        oPVHighHigh.style.visibility = "hidden";
        oPVHigh.Level = 0;
        oPVHigh.levelFillColor = 'transparent';
        oPVHigh.FillColor = 'transparent';
        oPVHigh.style.visibility = "hidden";
        oPVLow.Level = 0;
        oPVLow.levelFillColor = 'transparent';
        oPVLow.FillColor = 'transparent';
        oPVLow.style.visibility = "hidden";
        oPVLowLow.Level = 0;
        oPVLowLow.levelFillColor = 'transparent';
        oPVLowLow.FillColor = 'transparent';
        oPVLowLow.style.visibility = "hidden";
        arrAlarmLimitsInfo[0] = "";
        arrAlarmLimitsInfo[1] = "";
        arrAlarmLimitsInfo[5] = "";
        arrAlarmLimitsInfo[6] = "";

        if (oSource.QualityGood("TagName." + sFBName + ".pveuhi") && oSource.QualityGood("TagName." + sFBName + ".pveulo"))
        {
            var pvEUHi = oSource.DataValue("TagName." + sFBName + ".pveuhi");
            var pvEULo = oSource.DataValue("TagName." + sFBName + ".pveulo");
            var pvRange = pvEUHi - pvEULo;
            var sEUDesc = oSource.DataValue("TagName." + sFBName + ".eudesc");
            var pvFormat = oSource.DataValue("TagName." + sFBName + ".pvformat");
        }

        if ((oSource.DataValue("TagName." + sFBName + ".pvhhalmpr") != 1) && (oSource.DataValue("TagName." + sFBName + ".pvhhalmpr") != 2))
        {
            var iTrip = oSource.DataValue("TagName." + sFBName + ".pvhhalmtp");
            var iPriority = oSource.DataValue("TagName." + sFBName + ".pvhhalmpr");
            if (pvEULo <= iTrip && iTrip <= pvEUHi && iTrip != null && isNaN(iTrip) == false && isFinite(iTrip) == true)
            {
                var tripRange = pvEUHi - iTrip;
                oPVHighHigh.Level = tripRange / pvRange;
                if (oPVHighHigh.Level > 0 && oPVHighHigh.Level < 0.01)
                {
                    oPVHighHigh.Level = 0.01;
                }
                if (iPriority == 3)
                {
                    oPVHighHigh.levelFillColor = AlarmStateIconFactory.lowcolor;
                }
                else if (iPriority == 4)
                {
                    oPVHighHigh.levelFillColor = AlarmStateIconFactory.highcolor;
                }
                else if (iPriority == 5)
                {
                    oPVHighHigh.levelFillColor = AlarmStateIconFactory.urgentcolor;
                }
                oPVHighHigh.style.visibility = "inherit";
                arrAlarmLimitsInfo[0] = oPVHighHigh.title + iTrip.toFixed(pvFormat) + " " + sEUDesc;
            }
        }
		
        if ((oSource.DataValue("TagName." + sFBName + ".pvhialmpr") != 1) && (oSource.DataValue("TagName." + sFBName + ".pvhialmpr") != 2))
        {
            var iTrip = oSource.DataValue("TagName." + sFBName + ".pvhialmtp");
            var iPriority = oSource.DataValue("TagName." + sFBName + ".pvhialmpr");
            if (pvEULo <= iTrip && iTrip <= pvEUHi && iTrip != null && isNaN(iTrip) == false && isFinite(iTrip) == true)
            {
                var tripRange = pvEUHi - iTrip;
                oPVHigh.Level = tripRange / pvRange;
                if (oPVHigh.Level > 0 && oPVHigh.Level < 0.01)
                {
                    oPVHigh.Level = 0.01;
                }
                if (oPVHighHigh.Level > 0 && oPVHigh.Level > 0 && oPVHigh.Level - oPVHighHigh.Level < 0.01)
                {
                    oPVHigh.Level = oPVHigh.Level + 0.01;
                }
                if (iPriority == 3)
                {
                    oPVHigh.levelFillColor = AlarmStateIconFactory.lowcolor;
                }
                else if (iPriority == 4)
                {
                    oPVHigh.levelFillColor = AlarmStateIconFactory.highcolor;
                }
                else if (iPriority == 5)
                {
                    oPVHigh.levelFillColor = AlarmStateIconFactory.urgentcolor;
                }
                oPVHigh.style.visibility = "inherit";
                arrAlarmLimitsInfo[1] = oPVHigh.title + iTrip.toFixed(pvFormat) + " " + sEUDesc;
            }
        }
        if ((oSource.DataValue("TagName." + sFBName + ".pvllalmpr") != 1) && (oSource.DataValue("TagName." + sFBName + ".pvllalmpr") != 2))
        {
            var iTrip = oSource.DataValue("TagName." + sFBName + ".pvllalmtp");
            var iPriority = oSource.DataValue("TagName." + sFBName + ".pvllalmpr");
            if (pvEULo <= iTrip && iTrip <= pvEUHi && iTrip != null && isNaN(iTrip) == false && isFinite(iTrip) == true)
            {
                var tripRange = iTrip - pvEULo;
                oPVLowLow.Level = tripRange / pvRange;
                if (oPVLowLow.Level > 0 && oPVLowLow.Level < 0.01)
                {
                    oPVLowLow.Level = 0.01;
                }
                if (iPriority == 3)
                {
                    oPVLowLow.levelFillColor = AlarmStateIconFactory.lowcolor;
                }
                else if (iPriority == 4)
                {
                    oPVLowLow.levelFillColor = AlarmStateIconFactory.highcolor;
                }
                else if (iPriority == 5)
                {
                    oPVLowLow.levelFillColor = AlarmStateIconFactory.urgentcolor;
                }
                oPVLowLow.style.visibility = "inherit";
                arrAlarmLimitsInfo[6] = oPVLowLow.title + iTrip.toFixed(pvFormat) + " " + sEUDesc;
            }
        }
        if ((oSource.DataValue("TagName." + sFBName + ".pvloalmpr") != 1) && (oSource.DataValue("TagName." + sFBName + ".pvloalmpr") != 2))
        {
            var iTrip = oSource.DataValue("TagName." + sFBName + ".pvloalmtp");
            var iPriority = oSource.DataValue("TagName." + sFBName + ".pvloalmpr");
            if (pvEULo <= iTrip && iTrip <= pvEUHi && iTrip != null && isNaN(iTrip) == false && isFinite(iTrip) == true)
            {
                var tripRange = iTrip - pvEULo;
                oPVLow.Level = tripRange / pvRange;
                if (oPVLow.Level > 0 && oPVLow.Level < 0.01)
                {
                    oPVLow.Level = 0.01;
                }
                if (oPVHighHigh.Level > 0 && oPVLow.Level > 0 && oPVLow.Level - oPVLowLow.Level < 0.01)
                {
                    oPVLow.Level = oPVLow.Level + 0.01;
                }
                if (iPriority == 3)
                {
                    oPVLow.levelFillColor = AlarmStateIconFactory.lowcolor;
                }
                else if (iPriority == 4)
                {
                    oPVLow.levelFillColor = AlarmStateIconFactory.highcolor;
                }
                else if (iPriority == 5)
                {
                    oPVLow.levelFillColor = AlarmStateIconFactory.urgentcolor;
                }
                oPVLow.style.visibility = "inherit";
                arrAlarmLimitsInfo[5] = oPVLow.title + iTrip.toFixed(pvFormat) + " " + sEUDesc;
            }
        }
		if (oSource.QualityGood("TagName." + sFBName + ".PV.$Limits") && oSource.DataChanged("TagName." + sFBName + ".PV.$Limits"))
    {
		LoadJsonParser(oSource, sFBName);
	}
        SetAlarmLimitsInfo(oSource, sFBName);
    }
}

function Extended_AlarmLimit(oSource, sFBName)
{
        var oOperatingHigh = oSource.parentElement.parentElement.Objects("indOperatingHigh");
        var oOperatingLow = oSource.parentElement.parentElement.Objects("indOperatingLow");
        var oOperatingAim = oSource.parentElement.parentElement.Objects("indOperatingAim");
        var oExceedanceHigh = oSource.parentElement.parentElement.Objects("indExceedanceHigh");
        var oExceedanceLow = oSource.parentElement.parentElement.Objects("indExceedanceLow");
        var sOperatingColour = "#FFFFFF";
        var sOperatingAimColour = "#303030";
        var sOperatingName = "";
        var sExceedanceColour = window.external.FrameworkServices.getFrameworkSetting("LimitExceedanceColour");
        sExceedanceColour = 'rgb(' + (sExceedanceColour & 0xff) + ', ' + ((sExceedanceColour >> 8) & 0xff) + ', ' + ((sExceedanceColour >> 16) & 0xff) + ')';
        //Reset all like FillColour, LevelFillColour, Visibility , ToolTip
        oOperatingLow.Level = 0;
        oOperatingLow.levelFillColor = 'transparent';
        oOperatingLow.FillColor = 'transparent';
        oOperatingLow.style.visibility = "hidden";
        oOperatingHigh.Level = 0;
        oOperatingHigh.levelFillColor = 'transparent';
        oOperatingHigh.FillColor = 'transparent';
        oOperatingHigh.style.visibility = "hidden";
        oExceedanceLow.Level = 0;
        oExceedanceLow.levelFillColor = 'transparent';
        oExceedanceLow.FillColor = 'transparent';
        oExceedanceLow.style.visibility = "hidden";
        oExceedanceHigh.Level = 0;
        oExceedanceHigh.levelFillColor = 'transparent';
        oExceedanceHigh.FillColor = 'transparent';
        oExceedanceHigh.style.visibility = "hidden";
        oOperatingAim.lineColor = 'transparent';
        oOperatingAim.style.visibility = "hidden";
        arrAlarmLimitsInfo[2] = "";
        arrAlarmLimitsInfo[3] = "";
        arrAlarmLimitsInfo[4] = "";
        if (oSource.QualityGood("TagName." + sFBName + ".pveuhi") && oSource.QualityGood("TagName." + sFBName + ".pveulo"))
        {
            var pvEUHi = oSource.DataValue("TagName." + sFBName + ".pveuhi");
            var pvEULo = oSource.DataValue("TagName." + sFBName + ".pveulo");
            var pvRange = pvEUHi - pvEULo;
			var sEUDesc = oSource.DataValue("TagName." + sFBName + ".eudesc");
            var pvFormat = oSource.DataValue("TagName." + sFBName + ".pvformat");
        }
        try
        {
            var olimitsData = JSON.parse(oSource.DataValue("TagName." + sFBName + ".PV.$Limits"));
        }
        catch (e)
        {
            return;
        }
        for (var i = 0; i < olimitsData.limitsData.limits.length; i++)
        {
            if (olimitsData.limitsData.limits[i].targetRange == true)
            {
                if (typeof(olimitsData.limitsData.limits[i].name) != "undefined")
                {
                    sOperatingName = olimitsData.limitsData.limits[i].name;
                }
                if (typeof(olimitsData.limitsData.limits[i].low) != "undefined")
                {
                    var iTrip = olimitsData.limitsData.limits[i].low.value.data;
                    if (pvEULo <= iTrip && iTrip <= pvEUHi && iTrip != null && isNaN(iTrip) == false && isFinite(iTrip) == true)
                    {
                        var tripRange = pvEUHi - iTrip;
                        oOperatingLow.Level = tripRange / pvRange;
                        if (oOperatingLow.Level > 0 && oOperatingLow.Level < 0.01)
                        {
                            oOperatingLow.Level = 0.01;
                        }
                        oExceedanceLow.Level = 1 - oOperatingLow.Level;
                        oOperatingLow.levelFillColor = sOperatingColour;
                        oOperatingLow.style.visibility = "Inherit";
                        oExceedanceLow.levelFillColor = sExceedanceColour;
                        oExceedanceLow.style.visibility = "Inherit";
                        arrAlarmLimitsInfo[4] = sOperatingName + oOperatingLow.title + iTrip.toFixed(pvFormat) + " " + sEUDesc;
                    }
                }
                if (typeof(olimitsData.limitsData.limits[i].high) != "undefined")
                {
                    var iTrip = olimitsData.limitsData.limits[i].high.value.data;
                    if (pvEULo <= iTrip && iTrip <= pvEUHi && iTrip != null && isNaN(iTrip) == false && isFinite(iTrip) == true)
                    {
                        var tripRange = iTrip - pvEULo;
                        oOperatingHigh.Level = tripRange / pvRange;
                        if (oOperatingHigh.Level > 0 && oOperatingHigh.Level < 0.01)
                        {
                            oOperatingHigh.Level = 0.01;
                        }
                        oExceedanceHigh.Level = 1 - oOperatingHigh.Level;
                        oOperatingHigh.levelFillColor = sOperatingColour;
                        oOperatingHigh.style.visibility = "Inherit";
                        oExceedanceHigh.levelFillColor = sExceedanceColour;
                        oExceedanceHigh.style.visibility = "Inherit";
                        arrAlarmLimitsInfo[2] = sOperatingName + oOperatingHigh.title + iTrip.toFixed(pvFormat) + " " + sEUDesc;
                    }
                }
                if ((typeof(olimitsData.limitsData.limits[i].aim) != "undefined"))
                {
                    var iTrip = olimitsData.limitsData.limits[i].aim.value.data;
                    if (pvEULo <= iTrip && iTrip <= pvEUHi && iTrip != null && isNaN(iTrip) == false && isFinite(iTrip) == true)
                    {
                        var iHeight = oSource.parentElement.parentElement.style.pixelHeight;
                        var tripRange = iTrip - pvEULo;
                        oOperatingAim.style.pixelHeight = 1;
                        oOperatingAim.style.pixelTop = iHeight - (tripRange / pvRange) * iHeight;
                        oOperatingAim.lineColor = sOperatingAimColour;
                        oOperatingAim.style.visibility = "Inherit";
                        arrAlarmLimitsInfo[3] = sOperatingName + oOperatingAim.title + iTrip.toFixed(pvFormat) + " " + sEUDesc;
                    }
                }
            }
        }
}

function SetAlarmLimitsInfo(oSource, sFBName)
{
    var strAlarmLimits = "";
    for (var i = 0; i < arrAlarmLimitsInfo.length; i++)
    {
        if ((typeof(arrAlarmLimitsInfo[i]) != "undefined") && (arrAlarmLimitsInfo[i] != ""))
        {
            if (strAlarmLimits == "")
            {
                strAlarmLimits = strAlarmLimits + arrAlarmLimitsInfo[i];
            }
            else
            {
                strAlarmLimits = strAlarmLimits + '\n' + arrAlarmLimitsInfo[i];
            }
        }
    }
    if (strAlarmLimits != "")
    {
        oSource.parentElement.parentElement.title = strAlarmLimits;
    }
}

function LoadJsonParser(oSource, sFBName)
{
  var hmiwebJSBasePath = window.external.FrameworkServices.HMIWebJSBasePath;
  var elem = document.createElement('script');
  elem.src = hmiwebJSBasePath + "/lib/json2.js";
  elem.type = "text/javascript";
  document.getElementsByTagName('head')[0].appendChild(elem);

  Extended_AlarmLimit(oSource, sFBName);
}