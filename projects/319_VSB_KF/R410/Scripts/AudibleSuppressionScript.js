// /************************************************************************/
// /****     COPYRIGHT 2011           HONEYWELL INTERNATIONAL SARL      ****/
// /************************************************************************/
var g_isClientSideAdjustingValue = false;   // this value would be set to true if operator is
                                            // adjusting audible suppression state in Station.
// Audible Suppression State Flags (lower 16 bits are reserved for countdown)
var AUDSUPPR_FLG_URGVISIBLE             = 0x80000000;
var AUDSUPPR_FLG_HGHVISIBLE             = 0x40000000;
var AUDSUPPR_FLG_LOWVISIBLE             = 0x20000000;
var AUDSUPPR_FLG_CANSUPPRESSHIGH		= 0x00040000;
var AUDSUPPR_FLG_ABOVESECURITYLEVEL     = 0x00020000;
var AUDSUPPR_FLG_ENABLED                = 0x00010000;
var AUDSUPPR_FLG_COUNTDOWNMASK          = 0x0000FFFF;
var g_lAudibleSuppressionState;                                   
var g_iBlinkingSeconds	= 10;	// 10 seconds before session end will start blinking
var g_iLockInSeconds	= 2;	// 2 seconds after state rotation will lock in new state
var MILLI_IN_SECOND	= 1000;	// there are 1000 milliseconds in one second
// The states used in g_currSupprState
var SOUND_UHL           = 1;    // 01 - Sound Urgent, high and low
var SOUND_UH            = 2;    // 02 - Sound Urgent, high
var SOUND_U             = 3;    // 03 - Sound Urgent
var ROTATING_THRESHOLD  = 10;   // 10 - The state threshold between not rotating and rotating
var SOUND_UHL_ROTATING  = 11;   // 11 - Rotating to sound urgent, high and low
var SOUND_UH_ROTATING   = 12;   // 12 - Rotating to sound urgent, high
var SOUND_U_ROTATING    = 13;   // 13 - Rotating to sound urgent
var g_currSupprState = SOUND_UHL;  
var g_isCloseToSessionEnd = false;
var g_isHighSuppressible  = false;
// Icon image names
var GIF_URGENT 		= "Images/Urgent.gif";
var GIF_URGENT_INACTIVE = "Images/Urgent-inactive.gif";
var GIF_HIGH		= "Images/High.gif";
var GIF_HIGH_INACTIVE	= "Images/High-inactive.gif";
var GIF_HIGH_DISAPPEAR  = "Images/High-disappear.gif";
var GIF_HIGH_APPEAR	= "Images/High-appear.gif";
var GIF_HIGH_BLINK	= "Images/High-blink.gif";
var GIF_LOW		= "Images/Low.gif";
var GIF_LOW_INACTIVE	= "Images/Low-inactive.gif";
var GIF_LOW_DISAPPEAR   = "Images/Low-disappear.gif";
var GIF_LOW_APPEAR	= "Images/Low-appear.gif";
var GIF_LOW_BLINK	= "Images/Low-blink.gif";
function UpdateIconImages()
{
	// only update audible suppression icons if the button is enabled
	if (g_lAudibleSuppressionState & AUDSUPPR_FLG_ENABLED)
	{
		var imgUrgent 	= document.getElementById("ImgUrgent");
		var imgHigh 	= document.getElementById("ImgHigh");
		var imgLow 	= document.getElementById("ImgLow");
		var btnAudSuppr = document.getElementById('btnAudibleSuppression');
		if (g_lAudibleSuppressionState & AUDSUPPR_FLG_ABOVESECURITYLEVEL)
		{
			btnAudSuppr.disabled = false;
			btnAudSuppr.className = 'audibleSuppressionEnabled';
			imgUrgent.src = GIF_URGENT;
			imgUrgent.style.visibility = "visible";
			if (g_isCloseToSessionEnd)
			{
				// close to session end, start blinking animation if it's not already started
				switch (g_currSupprState)
				{
				case SOUND_UHL:
					// if current state is already showing everything and session is ending
					// nothing else to do. in fact, it shouldn't be in this state.
					imgLow.src = GIF_LOW;
					imgLow.style.visibility = "visible";
					imgHigh.src = GIF_HIGH;
					imgHigh.style.visibility = "visible";
					break;
				case SOUND_UH:
					if (imgLow.src.toLowerCase() != GIF_LOW_BLINK.toLowerCase())	// start animation if not started
						imgLow.src = GIF_LOW_BLINK;
					imgLow.style.visibility = "visible";
					imgHigh.src = GIF_HIGH;
					imgHigh.style.visibility = "visible";
					break;
				case SOUND_U:
					if (imgLow.src.toLowerCase() != GIF_LOW_BLINK.toLowerCase())	// start animation if not started
						imgLow.src = GIF_LOW_BLINK;
					imgLow.style.visibility = "visible";
					if (imgHigh.src.toLowerCase() != GIF_HIGH_BLINK.toLowerCase())
						imgHigh.src = GIF_HIGH_BLINK;
					imgHigh.style.visibility = "visible";
					break;
				default:
					break;
				}
			}
			else
			{
				switch (g_currSupprState)
				{
				case SOUND_UHL_ROTATING:
					if (!g_isHighSuppressible)
					{
						imgHigh.src = GIF_HIGH;
						imgHigh.style.visibility = "visible";
					}
					if (imgLow.src.toLowerCase() != GIF_LOW_APPEAR.toLowerCase())
						imgLow.src = GIF_LOW_APPEAR;
					imgLow.style.visibility = "visible";
					break;
				case SOUND_UH_ROTATING:
					if (g_isHighSuppressible)   // came from SOUND_U_ROTATING
					{
						if (imgHigh.src.toLowerCase() != GIF_HIGH_APPEAR.toLowerCase())
							imgHigh.src = GIF_HIGH_APPEAR;
						imgHigh.style.visibility = "visible";
					}
					else                        // came from SOUND_UHL_ROTATING
					{
						if (imgLow.src.toLowerCase() != GIF_LOW_DISAPPEAR.toLowerCase())
							imgLow.src = GIF_LOW_DISAPPEAR
						imgLow.style.visibility = "visible";
						imgHigh.src = GIF_HIGH;
						imgHigh.style.visibility = "visible";
					}
					break;
				case SOUND_U_ROTATING:
					if (g_isHighSuppressible)   // came from SOUND_UHL_ROTATING
					{
						if (imgLow.src.toLowerCase() != GIF_LOW_DISAPPEAR.toLowerCase())
							imgLow.src = GIF_LOW_DISAPPEAR
						imgLow.style.visibility = "visible";
					}
					if (imgHigh.src.toLowerCase() != GIF_HIGH_DISAPPEAR.toLowerCase())
						imgHigh.src = GIF_HIGH_DISAPPEAR
					imgHigh.style.visibility = "visible";
					break;
				case SOUND_UHL:
					imgLow.src = GIF_LOW;
					imgLow.style.visibility = "visible";
					imgHigh.src = GIF_HIGH;
					imgHigh.style.visibility = "visible";
					break;
				case SOUND_UH:
					imgLow.src = GIF_LOW;
					imgLow.style.visibility = "hidden";
					imgHigh.src = GIF_HIGH;
					imgHigh.style.visibility = "visible";
					break;
				case SOUND_U:
					imgLow.src = GIF_LOW;
					imgLow.style.visibility = "hidden";
					imgHigh.src = GIF_HIGH;
					imgHigh.style.visibility = "hidden";
					break;
				default:
					break;
				}
			}
		}
		else
		{
			// current CRT is below security level, show current state in grey, no animation
			imgUrgent.src = GIF_URGENT_INACTIVE;
			switch (g_currSupprState)
			{
			case SOUND_UHL:
				imgLow.src = GIF_LOW_INACTIVE;
				imgLow.style.visibility = "visible";
				imgHigh.src = GIF_HIGH_INACTIVE;
				imgHigh.style.visibility = "visible";
				break;
			case SOUND_UH:
				imgLow.src = GIF_LOW_INACTIVE;
				imgLow.style.visibility = "hidden";
				imgHigh.src = GIF_HIGH_INACTIVE;
				imgHigh.style.visibility = "visible";
				break;
			case SOUND_U:
			default:
				imgLow.src = GIF_LOW_INACTIVE;
				imgLow.style.visibility = "hidden";
				imgHigh.src = GIF_HIGH_INACTIVE;
				imgHigh.style.visibility = "hidden";
				break;
			}
			btnAudSuppr.disabled = true;
			btnAudSuppr.className = 'audibleSuppressionDisabled';
		}
	}
}
function SessionEnd()
{
    g_isCloseToSessionEnd = true;
    g_isClientSideAdjustingValue = false;
}
function EndSession()
{
    g_isCloseToSessionEnd = false;
    g_isClientSideAdjustingValue = false;
    g_currSupprState = SOUND_UHL;
}   
var g_StartSuppressionTimer;
function StartSuppression()
{
    clearTimeout(g_StartSuppressionTimer);
    g_isClientSideAdjustingValue = false;
    if(g_currSupprState > ROTATING_THRESHOLD)
        g_currSupprState -= ROTATING_THRESHOLD; // move back in the normal/suppression state
    g_isCloseToSessionEnd = false;              // a new session is starting
    g_TDO.SuppressAudible(g_currSupprState);
    UpdateIconImages();
}
function OnAudibleSuppressionButtonClick()
{
    g_isClientSideAdjustingValue = true;
    if (g_isCloseToSessionEnd)
    {
        clearTimeout(g_StartSuppressionTimer);
        StartSuppression();    
    }
    else
    {
        if (g_currSupprState < ROTATING_THRESHOLD ) // currently in normal/suppression state, going into rotate mode
        {
            g_currSupprState += ROTATING_THRESHOLD; // go into rotate mode
        }
		// check if system wide setting allows suppressing high audible
        if (g_isHighSuppressible)
        {
            g_currSupprState = --g_currSupprState < SOUND_UHL_ROTATING ?    // switch to next rotating state
                               SOUND_U_ROTATING : g_currSupprState;
        }
        else
        {
            g_currSupprState = ++g_currSupprState > SOUND_UH_ROTATING ?     // switch to next rotating state
                               SOUND_UHL_ROTATING : g_currSupprState;
        }
        clearTimeout(g_StartSuppressionTimer);
        g_StartSuppressionTimer = setTimeout("StartSuppression()", g_iLockInSeconds * MILLI_IN_SECOND);
    }
	UpdateIconImages();
}
function UpdateTooltip()
{
    var btnAudibleSuppression = document.getElementById('btnAudibleSuppression');
	if (g_isHighSuppressible)
		btnAudibleSuppression.title =  "Suppress high and low annunciation";
	else
		btnAudibleSuppression.title =  "Suppress low annunciation";
}
function OnAudibleSuppressionUpdate(lAudibleSuppressionState)
{
	g_lAudibleSuppressionState = lAudibleSuppressionState;
	if (g_SummaryType == SUMMARY_ALARM)
	{
		var btnAudSuppr = document.getElementById('btnAudibleSuppression');
		if(lAudibleSuppressionState & AUDSUPPR_FLG_ENABLED)
		{
			btnAudSuppr.style.visibility = 'visible';
			// check if high priority audible can be suppressed
			if (lAudibleSuppressionState & AUDSUPPR_FLG_CANSUPPRESSHIGH)
				g_isHighSuppressible = true;
			else
				g_isHighSuppressible = false;
			// OnAudibleSuppressionUpdate is sent once every second from server
			// into Station during the audible suppression period. While operator
			// is interacting with the suppression button (change state, or last
			// 10 second flashing), try not to update the button state
			if ( !g_isClientSideAdjustingValue )
			{
				var iCountDown = lAudibleSuppressionState & AUDSUPPR_FLG_COUNTDOWNMASK;
				if (iCountDown == 0)        // current session ended
				{
					EndSession();
				}
				else if (iCountDown <= g_iBlinkingSeconds && !g_isCloseToSessionEnd)  
				{
					// last 10 second, start blinking
					SessionEnd();
				}
				else
				{
					if (iCountDown > g_iBlinkingSeconds)
						g_isCloseToSessionEnd = false;
					if(lAudibleSuppressionState & AUDSUPPR_FLG_LOWVISIBLE)
						g_currSupprState = SOUND_UHL;   // sound urgent, high and low;
					else if(lAudibleSuppressionState & AUDSUPPR_FLG_HGHVISIBLE)
						g_currSupprState = SOUND_UH;    // sound urgent, high;
					else
						g_currSupprState = SOUND_U;     // only sound urgent;
				}
				UpdateTooltip();
				UpdateIconImages();
			}
		}
		else
		{
			btnAudSuppr.style.visibility = 'hidden';
		}
	}
}
// /************************************************************************/
// /****     COPYRIGHT 2011           HONEYWELL INTERNATIONAL SARL      ****/
// /************************************************************************/
