// --------------------------------------------------------------------------------------
// Pane::Pane() 
//	
//	Constructor for a Pane object.
// --------------------------------------------------------------------------------------

function Pane(elem)
{
	try
	{
		this.elem = elem;
		this.name = elem.id;
	}
	catch( err )
	{
		HANDLE_ERROR("Pane::Pane", err.number, err.description);
	}
}

// --------------------------------------------------------------------------------------
// Pane::Save()
//
//	Allows the pane to save itself to the 'bag' persistence object
// --------------------------------------------------------------------------------------

Pane.prototype.Save = function(bag)
{
	try
	{
		if( (typeof(this.elem) == "object") && (typeof(bag) == "object") )
		{
			var strName = this.name;
			var style = this.elem.style;
			bag.setAttribute(strName, true);
			bag.setAttribute(strName + "_WIDTH", style.width);
			bag.setAttribute(strName + "_HEIGHT", style.height);
			bag.setAttribute(strName + "_LEFT", style.left);
			bag.setAttribute(strName + "_TOP", style.top);
			bag.setAttribute(strName + "_DISPLAY", style.display);
		}
	}
	catch( err )
	{
		HANDLE_ERROR("Pane::Save", err.number, err.description);
	}
};

// --------------------------------------------------------------------------------------
// Pane::Load()
//
//	Allows the pane to load itself to the 'bag' persistence object
// --------------------------------------------------------------------------------------

Pane.prototype.Load = function(bag)
{
	try
	{
		if( (typeof(this.elem) == "object") && (typeof(bag) == "object") )
		{
			if( null != bag.getAttribute(this.name) )
			{
				var strName = this.name;
				var style = this.elem.style;
				var value = bag.getAttribute(strName + "_WIDTH");
				if (value != "")
					style.width	= value;
				value = bag.getAttribute(strName + "_HEIGHT");
				if (value != "")
					style.height = value;
				value = bag.getAttribute(strName + "_LEFT");
				if (value != "")
					style.left = value;
				value = bag.getAttribute(strName + "_TOP");
				if (value != "")
					style.top = value;
				value = bag.getAttribute(strName + "_DISPLAY");
				if (value != "")
					style.display = value;
			}
		}		
	}
	catch( err )
	{
		HANDLE_ERROR("Pane::Load", err.number, err.description);
	}
};

  