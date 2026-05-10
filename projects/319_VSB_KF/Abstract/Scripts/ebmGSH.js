// COPYRIGHT (c) 2019
// HONEYWELL INTERNATIONAL Sàrl,
// ALL RIGHTS RESERVED
//
// This software is a copyrighted work and/or information protected
// as a trade secret. Legal rights of Honeywell International Sàrl in this
// software is distinct from ownership of any medium in which the
// software is embodied. Copyright or trade secret notices included
// must be reproduced in any copies authorized by Honeywell International Sàrl
// The information in this software is subject to change without
// notice and should not be considered as a commitment by Honeywell International Sàrl.
// ------------------------------------------------------------------------------------
// JavaScript Document
//
// Sequence Shape Scripts
// ------------------------------------------------------------------------------------
var SEQ_RELEASE = "R511.1";
var SEQ_VERSION_DATE = "June 2019";
//
// Define myEbms as a global variable
//
var myEbms = [];
var glbEbm;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Global shape update attribute index - to allow/control shape updates with multiple instances
var attrIdx = 0;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  Set up the graphics parameters
//
var myCSS = findCSSFile("Batch");
var ebmShapeHeight = myCSS? parseInt(findCSSValue(myCSS, ".ebmShapeParams", "ebmShapeHeight")):20;
var ebmShapeWidth = myCSS? parseInt(findCSSValue(myCSS, ".ebmShapeParams", "ebmShapeWidth")):100;
var ebmShapeTopMargin = myCSS? parseInt(findCSSValue(myCSS, ".ebmShapeParams", "ebmShapeTopMargin")):10;
var ebmShapeSideMargin = myCSS? parseInt(findCSSValue(myCSS, ".ebmShapeParams", "ebmShapeSideMargin")):10;
var ebmXoffset = myCSS? parseInt(findCSSValue(myCSS, ".ebmShapeParams", "ebmXoffset")):5;
var ebmYoffset = myCSS? parseInt(findCSSValue(myCSS, ".ebmShapeParams", "ebmYoffset")):0;
//
var moreTargets = 0;
var nextP = 1;
//
//
//
// Define myRootPoint as a global variable.
// myRootPoint holds the root (original point) that called the flow shape for a particular shape canvas.
//
var myRootPoint = [];
var g_maxDataBlocks = 400;
//
//  Stage Shape parameters
//
var g_maxStages = 20;
var g_maxNodesInStage = 30;
var g_showStages = [];
//
//
// SCM/RCM States Array
var arrState = ["Loading","Loaded","Inactive","Validated","Running","Complete","Checking","Idle","Interrupting","Interrupted","Restarting","Restarted","Holding","Held","Stopping","Stopped","Aborting","Aborted","CommErr"];
// PHASE States Array
var arrStatePhase = ["Edit","Inactive","","","Active","ActiveComplt","","","ProcMinWait","ProcAcquire","ProcFormula","ProcExec","ProcState","ProcReport","ProcReset","ProcRelease","ProcModeTrk"];
// HANDLER/STEP/TRANSITION States Array
var arrStateHST = ["Edit","Inactive","Disabled","Enabled","Active","ActiveComplt","ProcOutputs","ProcMessages","ProcMinWait"];
// R/SCM,ACT,HANDLER/STEP/TRANSITION ExcSts Array
var arrExecSts= ["Ok","Bypass","Info","Warning","Fail","Error","OkPaused","BypassPaused","InfoPaused","WarnPaused","FailPaused","ErrorPaused","Disabled","NotTransition"];
// R/SCM,HANDLER/STEP/TRANSITION ConfigSts Array
var arrConfigSts = ["Ok","Information","Warning","SevereWarning","Fail","Error","SevereError"];
// R/SCM,PERIOD enumeration in ms
var arrExecPeriod = [1000,,,,50,100,200,500,1000,2000,10000,20000];
//
//	Define Debugging variable
//	If true debugging will print values when tested and an error occurs.
//
var myDebug = myCSS? findCSSValue(myCSS, ".ebmShapeParams", "debugWindow"):"false";
myDebug = myDebug?(myDebug.toLowerCase() == "true"):false;
var myDebugMode = myCSS? findCSSValue(myCSS, ".ebmShapeParams", "debugMode"):"false";
myDebugMode = myDebugMode?(myDebugMode.toLowerCase() == "true"):false;
var myDebugLevel = myCSS? findCSSValue(myCSS, ".ebmShapeParams", "debugLevel"):0;
myDebugLevel = (typeof (myDebugLevel) == "string")? parseInt(myDebugLevel): 0;
//
function debugWrite(myCallString, myString) {
	var d, s;
	var c = ":";
	d = new Date();
	s = " " + d.getHours() + c;
	s += d.getMinutes() + c;
	s += d.getSeconds() + c;
	s += d.getMilliseconds() + " ";
	var myErrorTable = document.getElementById("myErrorTable");
	if(myErrorTable){
		myErrorTable.innerHTML = myErrorTable.innerHTML + s + myCallString +"<br>"; // myString.substring(0,1500)+"<br>";
		myDiv.scrollTop = myDiv.scrollHeight-1;
	} else {
		myDiv = document.createElement("div");
		myDiv.id = "myErrorTable";
		document.body.appendChild(myDiv);
		myDiv.innerHTML = s + myCallString + "<br>"; //myString.substring(0,1500)+"<br>";
		myDiv.scrollTop = myDiv.scrollHeight-1;
	}
}
//
// Custom LogMsg debugging function - uses Experion Client Log
//
function LogMsg(str,logLvl){
	
	if(typeof (logLvl) != "undefined"){
		if(logLvl > myDebugLevel){
			window.external.Application.LogMessage(">>> " + str); 
		}
	}else{
		window.external.Application.LogMessage(">>> " + str); 
	}
}
//
//  Return Unique elements in list
//
function uniqueList(myArray){
	var o = {}, i, l = myArray.length, r = [];
	for(i=0; i<l;i+=1) {o[myArray[i]] = myArray[i];}
	for(i in o) { r.push(o[i]); }
	return r;
}
//
//	ebm - a point that decodes the order parameter that is passed to it.
//	order - this is a string that defines a linked list of all the blocks on an SCM or RCM or Activity point.
//	Here ebm is being used a class object.
//
//	ebm:
//	Properties
//		blocks - an array of all the elements of the linked list provided by the order parameter
//		len - a count of all the elements of the linked list
//		used - a dynamic array that keeps track of blocks that have been traversed when decoding the linked list
//		todo - a dynamic array that keeps track of paths that are yet to be traversed
//		layoutArray - a dynamic array that holds the relative X,Y location of the decoded linked list.
//				the index of this array is a encoded string which contains the relative X,Y location of the block.
//		rBlocks - depreciated array for holding information about the display blocks
//
//	Methods
//		getIndex - given a block name returns the blocks index
//		getName - given an index returns the name of the block
//		getType - given a block name returns the block type (step=S, transition=T, phase=P, sync=Y, handler=H)
//		getNext - given an index returns the next item in the linked list.  If number of outputs is greater than 1 then
//				the other paths are added to the todo list
//		getPath - given a starting block index, getPath will traverse the blocks until the end of the path is reached,
//				the path returned is only the primary path, if brances are detected they are added to the todo list,
//				the path traversed is returned as an array of block indexes
//		isUsed - given a block index, returns true if the used array contains that block other wise returns false.
//		contains - function that tests if an element is part of an array.
//		addToLayout - function that adds an path (array) to the layoutArray starting from a start index.
//				This function determines the relative X,Y coordinates of each block added
//		getLayout - given a block name returns (if defined) the layoutArray index
//		isColumnClear - funtion that is used by addToLayout to determine if a column has room to add the path.
//		getMaxLayoutRow - function that takes an array of blocks and returns the largest row index of the blocks
//		addToLayoutRow - function that increases the layout row of a block by a offset amount.
//		listBlocks - function that given a start block and end block will return a list all blocks between them.
//		normalizeLayout - function that aligns block indexs to make sync blocks all end on same block row number.
//
//
function ebm(order) {
	this.blocks = order.split(";");
	this.len = this.blocks.length;
	this.used = [];
	this.todo = [];
	this.layoutArray = {};
	this.rBlocks = [];
}
ebm.prototype.getIndex = function(blockName){
	var tArray;
	if(blockName == "") { return 0; }
	for (var i in this.blocks) {
		tArray = this.blocks[i].split(",");
		if (tArray.length > 2) {
			if (tArray[2].toUpperCase() == blockName.toUpperCase()){
				return i;
			}
		}
	}
	return -1;
};
ebm.prototype.getInputs = function(blockName){
	var inputs, myIndex, tArray;
	inputs = [];
	myIndex = this.getIndex(blockName);
	if (myIndex == -1) {
		return inputs;
	}
	for (var i in this.blocks){
		tArray = this.blocks[i].split(",");
		for(var j = 3;j<tArray.length;j++){
			if(tArray[j] == myIndex){
				inputs.push(tArray[2]);
			}
		}
	}
	return inputs;
};
ebm.prototype.getOutputs = function(blockName){
	var outputs, myIndex, tArray;
	outputs = [];
	myIndex = this.getIndex(blockName);
	if (myIndex == -1) {
		return outputs;
	}
	tArray = this.blocks[myIndex].split(",");
	for(var j = 3;j<tArray.length;j++){
		outputs.push(tArray[j]);
	}
	return outputs;
};
ebm.prototype.getName = function(myIndex){
	if (myIndex < this.blocks.length && myIndex > -1) {
		return this.blocks[myIndex].split(",")[2];
	}
	return "Unknown Block";
};
ebm.prototype.getType = function(tmpName){
	return this.blocks[this.getIndex(tmpName)].split(",")[0];
};

ebm.prototype.getNext = function(currentIndex){
	var tArray;
	if (currentIndex < this.blocks.length && currentIndex > -1) {
		tArray = this.blocks[currentIndex].split(",");
		if (tArray.length > 3){
			if (this.isUsed(tArray[3])){
				return -1;
			}
			if(tArray.length>4){
				for(var i = 4; i<tArray.length; i++){
					this.todo.push([currentIndex, tArray[i]]);
				}
			}
			this.used.push(tArray[3]);
			return tArray[3];
		}
		return -1;
	}
	return -99;
};
ebm.prototype.getPath = function(startIndex){
	var path, cIndex;
	path = [];
	if(this.isUsed(startIndex)){
//		path.push(startIndex);
		return path;
	}
	cIndex = startIndex;
	this.used.push(cIndex);
	do{
		path.push(cIndex);
		cIndex = this.getNext(cIndex);
	} while (cIndex>0);
	return path;
};
ebm.prototype.isUsed = function(index){
	return this.contains(this.used, index);
};
ebm.prototype.contains = function(a, obj){
	for(var i=0; i<a.length;i++){
		if(a[i] == obj){
			return true;
		}
	}
	return false;
};
ebm.prototype.addToLayout = function(startIndex, pathtoadd){
	var startRow, startCol, startName, startAddress, trow, tcol, tindex;
	startRow = 0;
	startCol = 0;
	if(startIndex != -1){
		startName = this.getName(startIndex);
		startAddress = this.getLayout(startName);
		startRow = parseInt(startAddress.split(",")[1])+1;
		startCol = parseInt(startAddress.split(",")[0])+1;
	}
	// Check Ok to add to column
	while(!(this.isColumnClear(startRow, startCol, pathtoadd.length))){
		startCol = startCol + 1;
	}
	for(i=0;i<pathtoadd.length;i++){
		trow = startRow + i;
		tcol = startCol;
//		tindex = trow+","+tcol;
		tindex = tcol+","+trow;
		this.layoutArray[tindex] = this.getName(pathtoadd[i]);
	}
};
ebm.prototype.getLayout = function(blockName){
	var i;
	for( i in this.layoutArray){
		if(this.layoutArray[i].toUpperCase() == blockName.toUpperCase()){
			return i;
		}
	}
	return "";
};
ebm.prototype.isColumnClear =function(startRow,tColumn, tLength){
	var tclear, i, rowNumber, tindex;
	tclear = true;
	for(i=0; i<tLength; i++){
		rowNumber = startRow + i;
		tindex = tColumn+","+rowNumber;
		if(tindex in this.layoutArray){
			tclear = false;
		}
	}
	return tclear;
};
ebm.prototype.getMaxLayoutRow = function(blocks){
	var maxRow, i, j, rowNumber;
	maxRow = -1;
	for ( i in blocks ){
		for (j in this.layoutArray ){
			if (blocks[i].toUpperCase() == this.layoutArray[j].toUpperCase()){
				rowNumber = parseInt(j.split(",")[1]);
				if (rowNumber > maxRow ){maxRow = rowNumber;}
			}
		}
	}
	return maxRow;
};
ebm.prototype.addToLayoutRow = function(blockName, offset){
	var j, tArray, newAttr;

	for(j in this.layoutArray){
		if (blockName.toUpperCase() == this.layoutArray[j].toUpperCase()){
			tArray = j.split(",");
			delete this.layoutArray[j];
			tArray[1] = parseInt(tArray[1]) + offset;
			newAttr = tArray.join();
			this.layoutArray[newAttr] = blockName;
			break;
		}
	}
};
//routine to shift a given path to left as long as there's space for
ebm.prototype.addToLayoutCol = function(startIndex, pathtoadd){
	var startRow, startCol, startName, startAddress, trow, tcol, tindex;
	startRow = 0;
	startCol = 0;
	if(startIndex != -1){
		startName = this.getName(startIndex);
		startAddress = this.getLayout(startName);
		startRow = parseInt(startAddress.split(",")[1]);
		startCol = parseInt(startAddress.split(",")[0]);
	}
	// Check Ok to add to column
	while((this.isColumnClear(startRow, startCol-1, pathtoadd.length)) && startCol>1){
		startCol = startCol - 1;
	}
	for(i=0;i<pathtoadd.length;i++){
		blockName = this.getName(pathtoadd[i]);
		var j, tArray, newAttr;
		for(j in this.layoutArray){
			if (blockName.toUpperCase() == this.layoutArray[j].toUpperCase()){
				tArray = j.split(",");
				delete this.layoutArray[j];
				tArray[0] = startCol;
				newAttr = tArray.join();
				this.layoutArray[newAttr] = blockName;
				break;
			}
		}
	}
};
ebm.prototype.listBlocks = function(startBlocks, endBlocks){
	var myPath, nextPath, path, names, startBlock, endBlock;

	startBlock = startBlocks.split(" ");
	endBlock = endBlocks.split(" ");
	//
	// Test for existing Blocks
	//
	if(this.getIndex(startBlock[0]) == -1 || this.getIndex(endBlock[0]) == -1) return [];
	for( var i = 0 ; i<startBlock.length; i++){
		this.todo.push([-1,this.getIndex(startBlock[i])]);
	}
	myPath =[];
	this.used = [];
	for( var j = 0; j< endBlock.length; j++){
		this.used.push(this.getIndex(endBlock[j]));
	}
	do{
		nextPath = this.todo.shift();
		path = this.getPath(nextPath[1]);
		myPath = myPath.concat(path);
	}while(this.todo.length > 0);
	names = [];
	for(var k=0; k<myPath.length; k++){
		names.push(this.getName(myPath[k]));
	}
	if(endBlocks.length > 0 ){
		for(var m=0; m<endBlock.length; m++){
			names.push(endBlock[m]);
		}
	}
	return names;
};
ebm.prototype.listIndexBlocks = function(startBlocks, endBlocks){
	var myPath, nextPath, path, names, startBlock, endBlock;

	startBlock = startBlocks.split(" ");
	endBlock = endBlocks.split(" ");
	//
	// Test for existing Blocks
	//
	if(this.getIndex(startBlock[0]) == -1 || this.getIndex(endBlock[0]) == -1) return [];
	for( var i = 0 ; i<startBlock.length; i++){
		this.todo.push([-1,this.getIndex(startBlock[i])]);
	}
	myPath =[];
	this.used = [];
	for( var j = 0; j< endBlock.length; j++){
		this.used.push(this.getIndex(endBlock[j]));
	}
	do{
		nextPath = this.todo.shift();
		path = this.getPath(nextPath[1]);
		myPath = myPath.concat(path);
	}while(this.todo.length > 0);


	names = [];
	for(var k=0; k<myPath.length; k++){
		names.push(this.getName(myPath[k]));
	}

	if(endBlocks.length > 0 ){
		for(var m=0; m<endBlock.length; m++){
			names.push(endBlock[m]);
			myPath.push(this.getIndex(endBlock[m]));
		}
	}
	return [myPath,names];
};

function isdone(array, n){
	for (var i in array){
		if (array[i] == n){
			return true;
		}
	}
	return false;
}
function FindMin(list){

	if (list.length<=2){
		if (list.length == 1){
			return list[0];
		}
		var a = parseInt(list[0]);
		var b = parseInt(list[1]);
		if (a<=b){
			return list[0];
		}else{
			return list[1];
		}
	}else{
		var t = (list.length/2>>0);
		var list1 = list.slice(0,t);
		var list2 = list.slice(t,list.length);

		var f1 = FindMin(list1);
		var f2 = FindMin(list2);
		var c = parseInt(f1);
		var d = parseInt(f2);

		if (c<=d){
			return f1;
		}else{
			return f2;
		}
	}
}
//
// Normalize the layout (align sync outputs)
//
ebm.prototype.normalizeLayout = function(){
	var tmpArray, cRow, maxRow, deltaI, tBlocks;
	shiftArray = [];
	syncBlocksToShift = [];
	tmpArray = this.layoutArray;
	for(var ii in tmpArray){
		shiftArray[ii] = 0;
	}
	for(var i in tmpArray){
		if(this.getType(tmpArray[i]) == "Y"){
			if(this.getInputs(tmpArray[i]).length > 1 ){
				cRow = parseInt(this.getMaxLayoutRow([tmpArray[i]]));
				maxRow = parseInt(this.getMaxLayoutRow(this.getInputs(tmpArray[i])));
				if(cRow <= maxRow){
					deltaI = maxRow - cRow + 1;
					syncBlocksToShift.push(tmpArray[i]);
					tBlocks = this.listBlocks(tmpArray[i], "");
					for(var k in tmpArray){
						for(var j in tBlocks){
							if(tmpArray[k].toUpperCase() == tBlocks[j].toUpperCase()){
								iy = parseInt(k.split(",")[1]);
								if(iy > cRow){
									shiftArray[k] = shiftArray[k] + deltaI;
								}
							}
						}
					}
					shiftArray[i] = shiftArray[i] + deltaI;
				}
			}
		}

	}
	var tmpLayoutArray = {};
	for (i in shiftArray){
		ix = parseInt(i.split(",")[0]);
		iy = parseInt(i.split(",")[1]) + shiftArray[i];
		tmpIndex = ix+","+iy;
		tmpLayoutArray[tmpIndex] = this.layoutArray[i];
	}
	this.layoutArray = tmpLayoutArray;
};
//
//	placeBlock Function
//	This function will place a block at X, Y position on a particular container.
//	Based on the layoutArray index it determines the X (x index *30) and the Y (y index *110 +5) coordinates
//	It also determines the block type then calls the appropriate drawing routine.
//	Finally it adds a unique [DOM Block] ID and links the object to the click event method.
//	The function returns the object that was added to the container.
//
function placeBlock(container, layout, theEbm){
	var blockPosition, myTop, myLeft, pointName, myShape, type, myColor, o;

	try {
		blockPosition = layout.split(",");

		myTop = blockPosition[1]*(ebmShapeTopMargin+ebmShapeHeight) + ebmYoffset;
		myLeft = blockPosition[0]*(ebmShapeWidth+ebmShapeSideMargin) + ebmXoffset;
		type =theEbm.getType(theEbm.layoutArray[layout]);
		switch (type) {
			case "Y":
				o = drawSync(container, myLeft, myTop, theEbm.layoutArray[layout]);
				break;
			case "T": //TRANSITION
				o = drawTransition(container, myLeft, myTop, theEbm.layoutArray[layout]);
				break;
			case "P": //PHASE
				o = drawPhase(container, myLeft, myTop, theEbm.layoutArray[layout]);
				break;
			case "S": //STEP
				o = drawStep(container, myLeft, myTop, theEbm.layoutArray[layout]);
				break;
			case "X": //STAGE
				o = drawStage(container, myLeft, myTop, theEbm.layoutArray[layout]);
				break;
		}
		theEbm.rBlocks[container.id+theEbm.layoutArray[layout]] = o;
		o.id = container.id + ":"+theEbm.layoutArray[layout]+":"+type;
		addEventHandler(o,"click", boxClickEvent);
		addEventHandler(o,"contextmenu",PhaseCMenuEvent);
		//addEventHandler(o,"mouseover", function(){o.style.cursor="pointer"});
		addEventHandler(o,"dblclick", function(){window.event.cancelBubble = true;});
	} catch(e) {
		if(myDebug){debugWrite("placeBlock ERROR","Error: "+e.description);}
	}
	return o;
}
//
//	PhaseCMenuEvent - handles custom context menu for phase elements
//
var PhaseCMenuEvent = function(event){
	var objPhase, sCMfile;
	event.cancelBubble = true;
	//Canvas selection indication
	objSelSeq();
	if(typeof event.srcElement == "undefined") return;

	if (event.srcElement.id == ""){
		if(event.srcElement.tagName.toUpperCase()=="SPAN"){
			objPhase = event.srcElement.parentElement.parentElement.id;
			Page.setAttribute("cmenusource",event.srcElement.parentElement.parentElement.id);
		}else{
			objPhase = event.srcElement.parentElement.id;
			Page.setAttribute("cmenusource",event.srcElement.parentElement.id);
		}
	} else {
		objPhase = event.srcElement.id;
	}

	oShape = document.getElementById(objPhase).parentElement.parentElement.parentElement;
	Page.setAttribute("lastselection",objPhase);
	Page.setAttribute("lastselection",objPhase);
	sCMfile=oShape.GetCustomProperty("text","ContextMenuFile");
	if(sCMfile!="" && sCMfile!="-"){
		HDXPopupBehavior.InvokeShortcutMenu(sCMfile,0,0);
	}
};
//
//	connectBlocks
//	This function draws the (output) connector(s) off the passed blockName
//	It is passed the container name, the blockName and the ebm object.
//	It determines the number of output connnections and then draws each one
//
function connectBlocks(container, blockName, theEbm){
	var inBlockName, tArray, i, outBlockName;
	try {
		inBlockName = theEbm.rBlocks[container.id+blockName];
		tArray = theEbm.blocks[theEbm.getIndex(blockName)].split(",");
		for(i=3;i<tArray.length;i++){
			outBlockName = theEbm.rBlocks[container.id + theEbm.getName(tArray[i])];
			connectObjects(container, inBlockName, outBlockName);
		}
	} catch(e) {
		if(myDebug){ debugWrite("connectBlocks", "Error: " + e.description+" "+container.id + theEbm.getName(tArray[i]));}
	}
}
//
//	drawSequence
//	This function takes the order parameter, the container name, and pointName and draws the flow shape.
//	This is the primary/ main function in drawing the shape.
//	It first starts by determining if this container has been used before, if not it resets the connections
//	and shapes arrays, and it unlinks all data objects.
//	It tests to see if the point is of the correct type (the type of point should be a string but this value will be
//	a number if an activity was being displayed and it completes and no longer exists)
//	It then creates a new ebm object based on the order passed.
//	The function then loops through the order parameter to create the layoutArray, once complete it normalizes the
//	layoutArray.
//	It then draws the shapes on the container and finally it draws the connectors between the shapes.
//
//
function drawSequence(myOrder, container, pointName, startBlock){
	var myEbm, nextPath, path, connections, shapes, vmlObj, b;
	// Limiting unnecessary shape redrawing
	// if Starting Block is the same as displayed then no need to redraw
	if (container.startBlock === startBlock) return;

	container.startBlock = startBlock;
	try {
		startBlock = ((typeof startBlock === "undefined")||startBlock=="") ? "MAIN" : startBlock;
		container.attachEvent("ondblclick", canvasClickEvent);
		container.attachEvent("oncontextmenu", canvasCMenuEvent);
		container.attachEvent("onclick", canvasClickEventSel);
		container.attachEvent("onscroll", canvasScrollEvent);
	} catch (e) {
		if (myDebug) {
			debugWrite("drawSequence", "Error in drawSequence with Initialization " + e.description);
		}
	}
	//try{
		if(typeof(pointName) == "undefined" || (typeof(pointName) == "number") || pointName.length < 1){
			return "";
		}
		if(typeof(myOrder)=="undefined" || myOrder.length == 0){
			return "";
		}
		if(typeof(myRootPoint[container.id]) == "undefined" || myRootPoint[container.id] == ""){
			myRootPoint[container.id] = pointName;
		}

		try {
			container.innerHTML="";
			connections = [];
			shapes = [];
			//unLinkAllDataShapes(container);
		} catch(e) {
			if (myDebug) {
				debugWrite("drawSequence", "Error in drawSequence with unLinking Points");
			}
		}
		if ((typeof(pointName) == "number") || pointName.length < 1) {
			return "";
		}
		if (myOrder.length == 0) {
			return "";
		}
		myEbm = new ebm(myOrder);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
for(var blockOrderIndex in myEbm.blocks){
	var ebmBlockName = myEbm.getName(blockOrderIndex);
	if((typeof ebmBlockName) != "undefined"){
		var ebmBlockType = myEbm.getType(ebmBlockName);
		var ebmBlockPrevious = myEbm.getInputs(ebmBlockName);
		var ebmBlockNext = myEbm.getOutputs(ebmBlockName);
	//try{
		//if(myDebugMode) LogMsg("ebmgsh -> src = " + container.id + " : index = " + blockOrderIndex + " : block = " + ebmBlockType + ":" + ebmBlockName + " ->  prev = " + ebmBlockPrevious.length );
		//if(myDebugMode) LogMsg("ebmgsh -> src = " + container.id + " : index = " + blockOrderIndex + " : block = " + ebmBlockName);		
	//}catch(e){
	//if(myDebugMode) LogMsg("ebmgsh -> src = " + container.id + " error ");
	//}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
		var offset = 0;
		// Define offset of two ORDER elements if startBlock is a HANDLER (not restricted only to "MAIN")
		// Invoke Transition appears as the second block from the Handler in the ORDER.
		if (myEbm.getType(startBlock) == "H") offset = 2;
		myEbm.todo.push([-1, parseInt(myEbm.getIndex(startBlock)) + offset]);
		try{
			do{
				nextPath = myEbm.todo.shift();
				path = myEbm.getPath(nextPath[1]);
				myEbm.addToLayout(nextPath[0],path);
			}while(myEbm.todo.length > 0);
		} catch(e){
			if (myDebug) {
				debugWrite("Popping List:", "Error Description: " + e.description);
			}
		}
		myEbm.normalizeLayout();
		//
		// Draw Shapes
		//
		//var myErr = container.getAttribute("layoutIndex");
		//if(!myErr){
			container.setAttribute("layoutIndex",1);
		//}
		
		var oMsg = document.getElementById(container.id + "_tmpDebug");
		if(myDebugMode &&  oMsg==null ) {
			oMsg = document.createElement("DIV");
			oMsg.id = container.id + "_tmpDebug";
			oMsg.style.backgroundColor = "#FFFFCA";
			oMsg.style.color = "#FF0000";
			oMsg.style.fontSize = "large";
			oMsg.style.textAlign = "center";
			oMsg.style.position = "relative";
			oMsg.style.top = "-24px";
			//oMsg.styleClass = "myStatusMsg";
			oMsg.innerText = "Drawing...";
			
			//container.appendChild(oMsg);
			//container.insertBefore(oMsg,null);
			container.parentElement.insertBefore(oMsg,null);
			
		}
		//Draw R/SCM elements and link to data
		for(var myLayout in myEbm.layoutArray){
			//try {
				//Place block - returns the VML element object
				vmlObj=placeBlock(container, myLayout, myEbm);

				//Set "InView" attribute for all blocks to false - initialization
				vmlObj.setAttribute("inView",false);
				
				//Get Block type from VML object ID (formed by placeBlock)
				var myBlkType = vmlObj.id.slice(-1);
				//Increment index for S,P and T only
				if (myBlkType != "Y") {
					var myBlkIdx = container.getAttribute("layoutIndex");
					//Apppend block index to ID
					vmlObj.id = vmlObj.id + ":" + ("00" + myBlkIdx).slice(-3);
					//Increment index for next block
					myBlkIdx++;
					//Store index in shape attribute
					container.setAttribute("layoutIndex",myBlkIdx);
				}else{
					//append "0" index for SYNC blocks - not subscribed to any data
					vmlObj.id = vmlObj.id + ":" + "0";
				}
				if(myDebugMode) vmlObj.title = vmlObj.id;
				vmlObj.setAttribute("identifier",vmlObj.id);
				//append datashape id to VML object id
				//a.id = a.id + ":" + b.id;
//LogMsg("ebmgsh -> drawSequence -> id = <" + a.id + "> | top = " + a.style.pixelTop + " : left = " + a.style.pixelLeft + " : inView =  " + isBlockInView(a,container));				
				//Store objects in array
				shapes.push(vmlObj);
			//} catch(e) {
				//if (myDebug) {
				//	debugWrite("Place and Link Blocks:", "Error Description: " + e.description);
				//}
			//}
		}
if(myDebugMode) oMsg.innerText = "End Draw...";
		//
		// Connect Shapes
		//
		for (var i in myEbm.layoutArray){
			try {
				connectBlocks(container, myEbm.layoutArray[i], myEbm);
			} catch(e) {
				if (myDebug) {
					debugWrite("Connect Blocks:", "Error Description: " + e.description);
				}
			}
		}
		myEbms[container.id] = myEbm;
	
//====================================================================================================		
		return myEbm;
	
	//} catch(e) {
	//	if (myDebug) {
	//		debugWrite("drawSequence", "Error Description: " + e.description);
	//	}
	//	return "";
	//}
}
//
//	updateFlow
//	This function is called by the data objects whenever a datavalue changes.
//	This function is called with the data object reference.
//	This function determines the pointName, parameterName, blockName, container/canvas name and the name of the shape to update
//	Depending on the block type and its state the corresponding flow display object (shape to update) is redrawn.
//
//
function updateFlow(obj){

	var type, pointName, paramName, blkName, canvasName, shapeToUpdate, myOrderObject, selected;
	var aString, paramName2, transitionState, stepState, phaseState;
	var parentID, tmpA, graphID, rName, p1Name, i, myColor, myOpacity, myMode, myFillColorTransition, myFillColorOpacity;
	var paramName3, stepProcessed, blkIdentifier;

	//try{
		var oData = obj.parentElement;
		if(oData == undefined){return null;}
		type = oData.GetCustomProperty("Value","blockType");
		//selected = oData.GetCustomProperty("Value","isSelected");

		pointName = oData.GetCustomProperty("point","pointName");
		paramName = oData.GetCustomProperty("Parameter","param1");
//=============================================================================
		var objDS = decodeBlockData(oData);
//=============================================================================		
		try {
			if(typeof(paramName) != "undefined"){
				blockName = paramName.split(".")[0];
			} else {
				blockName = oData.GetCustomProperty("Value","stageName");
			}
		} catch(e){
			if(myDebug){debugWrite("updateFlow: ", "Error in paramName " + e.description + ">" +paramName);}
		}
//if (obj.parentElement.parentElement.id.toLowerCase().indexOf("canvas")==-1) {return null;}
		canvasName = oData.parentElement.parentElement.Objects("myCanvas").id;


		shapeToUpdate = document.getElementById(oData.getAttribute("shapeToUpdateID"));
		if(!shapeToUpdate) return;
if(myDebugMode) LogMsg("updateFlow -> oData = " + oData.id + " : shp2upd = " + oData.getAttribute("shapeToUpdateID")+ " : type = " + typeof shapeToUpdate + " : pnt = " + pointName);		
		if(pointName == ""){
			//if(myDebugMode && shapeToUpdate) {
			//if(shapeToUpdate) {
				blkIdentifier = shapeToUpdate.getAttribute("identifier");
				if(blkIdentifier){
					shapeToUpdate.title = blkIdentifier + "\r\n\r\n" + "No Data";
					
					myColor = myCSS? findCSSValue(myCSS, ".ebmStep", "color"):"#FFFFFF";
					myStrokeColor = myCSS? findCSSValue(myCSS, ".ebmStep", "strokecolor"):"#FFFFFF";
					myFillColor = myCSS? findCSSValue(myCSS, ".ebmStep", "fill-color"):"#FFFFFF";
					myOpacity = myCSS? findCSSValue(myCSS, ".ebmStep", "opacity"):0;
					
					shapeToUpdate.style.color = myColor;
					shapeToUpdate.strokeColor = myStrokeColor;
					shapeToUpdate.fillColor = myFillColor;
					shapeToUpdate.fill.opacity = myOpacity;
					oData.removeAttribute("shapeToUpdateID");
				}
			//}
			return null;
		}
		//Debug subscriptions information
		if(myDebugMode){
			var oMsg = document.getElementById(canvasName+"_tmpDebug");
			var bWasInView = shapeToUpdate.getAttribute("inView");
			var bIsInView = isInViewport(shapeToUpdate,oData.parentElement.parentElement.Objects("myCanvas"));
			var iS = oMsg.getAttribute("subs");
			if(pointName != ""){
				oMsg.setAttribute("subs",++iS);
			}else{
				oMsg.setAttribute("subs",--iS);
			}
			updateDebugMsg(oMsg);
		}

		var oShape = obj.parentElement.parentElement.parentElement;
		var myModeObject = oShape.Objects("mode");
		//check compatibility for SingleStep
		if(myModeObject){ 
			myMode = myModeObject.DataValue("pointName.MODE");
		}else{
			myMode = 1;
		}
//if(myDebugMode) LogMsg("updateFlow -> shape to update = " + canvasName+":"+blockName+":"+type+":"+ oData.id.slice(-3))		
		if(shapeToUpdate.id.search(/:/) > -1){
			blkName = shapeToUpdate.id.split(":")[1];
		}else{
			blkName = ";";
		}
		//Check if block is in TARGETSTEP list
		var strTargetSteps = oShape.getAttribute("TargetSteps");	
		if (strTargetSteps.indexOf("." + blkName) > -1){
			selected = 1;
		}else{
			selected = 0;
		}
if(myDebugMode) LogMsg("updateFlow SINGLESTEP -> " + obj.id + " : " + blkName + " : " + strTargetSteps);
		//Switch by block type (Phase,Step,Transition,Xstage)
		switch(type){
			case "S": //STEP
				//try {
					stepState = objDS.state;
					stepProcessed = objDS.processed;
					stepExecSts = objDS.execsts;
					
					if (selected == 1 && myMode == 3) {
							//SINGLESTEP
							myColor = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpTo", "color"):"#FFFFFF";
							myFillColor = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpTo", "fill-color"):"#FFA500";
							myOpacity = 1;
//if(myDebugMode) LogMsg("updateFlow SINGLESTEP [\"S\"] -> " + myFillColor + "\r\n" + shapeToUpdate.id);
					}else{
						// if (selected == 1 && myMode != 3){
							// obj.parentElement.SetCustomProperty("Value","isSelected",0);
							// selected = 0
						// }
						myColor = myCSS? findCSSValue(myCSS, ".ebmStepFill", "color"):"#FFFFFF";
						myFillColor = myCSS? findCSSValue(myCSS, ".ebmStepFill", "fill-color"):"#FFFFFF";
						myOpacity = myCSS? findCSSValue(myCSS, ".ebmStepFill", "opacity"):0;

						//changed to handle colors and parallel Steps
						if (stepProcessed == 1){

							if(stepState > 3 ){
								switch(stepExecSts){
									case 0:
									case 2:
										myColor = myCSS? findCSSValue(myCSS, ".ebmStepFillProc", "color"):"#FFFFFF";
										myFillColor = myCSS? findCSSValue(myCSS, ".ebmStepFillProc", "fill-color"):"#72A77B";
										myOpacity = myCSS? findCSSValue(myCSS, ".ebmStepFillProc", "opacity"):0.5; 
										break;
									case 3:
										myColor = myCSS? findCSSValue(myCSS, ".ebmStepFillWarn", "color"):"#00FF00";
										myFillColor = myCSS? findCSSValue(myCSS, ".ebmStepFillWarn", "fill-color"):"#00FF00";
										myOpacity = myCSS? findCSSValue(myCSS, ".ebmStepFillWarn", "opacity"):0.5; 
										break;
									case 4:
										myColor = myCSS? findCSSValue(myCSS, ".ebmStepFillFail", "color"):"#00FF00";
										myFillColor = myCSS? findCSSValue(myCSS, ".ebmStepFillFail", "fill-color"):"#00FF00";
										myOpacity = myCSS? findCSSValue(myCSS, ".ebmStepFillFail", "opacity"):0.5;
										break;
								}
							}else{
								myColor = myCSS? findCSSValue(myCSS, ".ebmStepFillProcessed", "color"):"#FFFFFF";
								myFillColor = myCSS? findCSSValue(myCSS, ".ebmStepFillProcessed", "fill-color"):"#0000FF";
								myOpacity = myCSS? findCSSValue(myCSS, ".ebmStepFillProcessed", "opacity"):0.5;
							}
						}

						//changed to handle colors and parallel Steps
						if (stepProcessed == 0 && stepExecSts == 0){
							if(stepState > 3 ){ //== 4 || stepState == 5 || stepState == 6 || stepState == 7 || stepState == 8){
								switch(stepExecSts){
									case 0:
									case 2:
										myColor = myCSS? findCSSValue(myCSS, ".ebmStepFillProc", "color"):"#FFFFFF";
										myOpacity = myCSS? findCSSValue(myCSS, ".ebmStepFillProc", "opacity"):0.5; 
										break;
									case 3:
										myColor = myCSS? findCSSValue(myCSS, ".ebmStepFillWarn", "color"):"#00FF00";
										myOpacity = myCSS? findCSSValue(myCSS, ".ebmStepFillWarn", "opacity"):0.5; 
										break;
									case 4:
										myFillColor = myCSS? findCSSValue(myCSS, ".ebmStepFillFail", "fill-color"):"#00FF00";
										myOpacity = myCSS? findCSSValue(myCSS, ".ebmStepFillFail", "opacity"):0.5;
										break;
								}
							}
						}
						if (myMode == 3){
							//SINGLESTEP
							if(stepState > 3 ){ //== 4 || stepState == 5 || stepState == 6 || stepState == 7 || stepState == 8){
								myColor = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpOff", "color"):"#FFFFFF";
								myFillColor = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpOff", "fill-color"):"#FFFF00";
								myOpacity = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpOff", "opacity"):0.5;
							}
						}
					}
					shapeToUpdate.style.color = myColor;
					shapeToUpdate.fillColor = myFillColor;
					shapeToUpdate.fill.opacity = myOpacity;
					//shapeToUpdate.title = stepDesc; // + "_" + type + "_" + stepProcessed + "_" + stepState;
				//} catch(e) {
				//	debugWrite("Update Step", "Error in Step Update:" + e.description);
				//}
				break;
			case "P": //PHASE
				try {
					phaseState = objDS.state;
					phaseExecSts = objDS.execsts;
					phaseProcessed = objDS.processed;
					//temporary Equipstate Simulation
					//phaseEquipState = 12;
					if (selected == 1 && myMode == 3) {
						//SINGLESTEP
						myColor = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpTo", "color"):"#FFA500";
						myFillColor = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpTo", "fill-color"):"#FFA500";
						myOpacity = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpTo", "opacity"):0.5;
						myCursor = "pointer";
					}else{
						// if (selected == 1 && myMode != 3){
							// obj.parentElement.SetCustomProperty("Value","isSelected",0);
							// selected = 0
						// }
						//Default style (init)
						myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFill", "color"):"#99ff88";
						myFillColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFill", "fill-color"):"#99ff88";
						myOpacity = myCSS? findCSSValue(myCSS, ".ebmPhaseFill", "opacity"):0.5;
						myCursor = "no-drop";
						//
						//if (phaseState <9 || phaseState >14){  //Phase not "Active"
						//	if(phaseProcessed==1){
						//		myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillProcessed", "color"):"#808080";
						//		myFillColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillProcessed", "fill-color"):"#0000ff"; //blue
						//		myOpacity = myCSS? findCSSValue(myCSS, ".ebmPhaseFillProcessed", "opacity"):0.5;
						//	}else{
						//		myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFill", "color"):"#808080";
						//		myFillColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFill", "fill-color"):"#A0A0A0";
						//		myOpacity = myCSS? findCSSValue(myCSS, ".ebmPhaseFill", "opacity"):0.5;
						//	}
						//}
						//
						if (phaseState > 1 && phaseExecSts == 4){
							// Child fail
							myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillFail", "color"):"#808080";
							myFillColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillFail", "fill-color"):"#bbee55";
							myOpacity = myCSS? findCSSValue(myCSS, ".ebmPhaseFillFail", "opacity"):0.5;
							myCursor = "pointer";
						}else if (phaseState > 1 && phaseExecSts == 3){	
							// Child warning
							myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillWarn", "color"):"#808080";
							myFillColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillWarn", "fill-color"):"#bbee55";
							myOpacity = myCSS? findCSSValue(myCSS, ".ebmPhaseFillWarn", "opacity"):0.5;
							myCursor = "pointer";
						}else if (phaseState > 1 && phaseExecSts == 1){	
							//Future
						}else if(phaseState == 4 && phaseProcessed ==1){
							//Processed, but still State = Active
							myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillProc", "color"):"#808080";
							myFillColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillProc", "fill-color"):"#bbee55";
							myOpacity = myCSS? findCSSValue(myCSS, ".ebmPhaseFillProc", "opacity"):0.5;	
						}else if(phaseState == 1 && phaseProcessed ==0){
							//Future
						}else if(phaseState > 1 && phaseExecSts != 4 && phaseExecSts != 3){
							//Phase "active" independent of PhaseEquip
							if (phaseState > 7){
								myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillProc", "color"):"#808080";
								myFillColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillProc", "fill-color"):"#bbee55";
								myOpacity = myCSS? findCSSValue(myCSS, ".ebmPhaseFillProc", "opacity"):0.5;
								//
								if (myMode == 3){
									//SINGLESTEP
									myColor = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpOff", "color"):"#808080";
									myFillColor = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpOff", "fill-color"):"#00FF00";
									myOpacity = myCSS? findCSSValue(myCSS, ".ebmSingleStepJumpOff", "opacity"):0.5;
								}
							/*	if (phaseState == 12){ //Phase in ProcState EquipState leads for color
									switch(phaseEquipState){ //diffrent EquipStates possible
										case 12: myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillHolding", "color"):"#EEEEEE"; break;
										case 13: myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillHeld", "color"):"#EEEEEE"; break;
										case 14: myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillStopping", "color"):"#EEEEEE"; break;
										case 15: myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillStopped", "color"):"#EEEEEE"; break;
										case 16: myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillAborting", "color"):"#FF0000"; break;
										case 17: myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillAborted", "color"):"#FF0000"; break;
										case 18: myColor = myCSS? findCSSValue(myCSS, ".ebmPhaseFillCommErr", "color"):"#FF0000"; break;
									}
								} */
								myCursor = "pointer";
							}
							myOpacity = 0.5;
						}
					}
					//Apply styles to the vml (phase) object
					shapeToUpdate.style.color = myColor;
					shapeToUpdate.fillColor = myFillColor;
					shapeToUpdate.fill.opacity = myOpacity;
					shapeToUpdate.style.cursor = myCursor;
					//shapeToUpdate.title = stepDesc; //+ "_" + type + "_" + phaseEquipState;
				} catch(e) {
					if(myDebug){debugWrite("Update Phase", "Error in Phase Update:" + e.description);}
				}
				break;
			case "T": //TRANSITION
			
				transitionStatus = objDS.so; //uses G[1].SO instead of STATE
				transitionProcessed = objDS.processed;
				transitionExecSts = objDS.execsts;
				transitionState = objDS.state;
				
				myColor = myCSS? findCSSValue(myCSS, ".ebmTransitionFill", "color"):"#FFFFFF";
				myFillColor = myCSS? findCSSValue(myCSS, ".ebmTransitionFill", "fill-color"):"#99ff88";
				myOpacity = myCSS? findCSSValue(myCSS, ".ebmTransitionFill", "opacity"):0;
				//myOpacity = 0;

				//Transition Processed indication - indicated via linecolor
				if (transitionProcessed == 1){
					if(transitionState == 2){	
					myColor = myCSS? findCSSValue(myCSS, ".ebmTransitionFillProcessed", "color"):"#0000FF";
					myOpacity = myCSS? findCSSValue(myCSS, ".ebmTransitionFillProcessed", "opacity"):0.5;
					}else if(transitionState >2){	//== 4 || transitionState == 5 || transitionState == 6 || transitionState == 7 || transitionState == 8){
						myColor = myCSS? findCSSValue(myCSS, ".ebmTransitionFillProc", "color"):"#bbee55";
						myOpacity = myCSS? findCSSValue(myCSS, ".ebmTransitionFillProc", "opacity"):0.5;
					}
				}else if (transitionProcessed == 0){
					if(transitionState == 2){
						myColor = myCSS? findCSSValue(myCSS, ".ebmTransitionFill", "color"):"#696969";
						myOpacity = myCSS? findCSSValue(myCSS, ".ebmTransitionFill", "opacity"):0.5;
					}else if (transitionState > 2){
						myColor = myCSS? findCSSValue(myCSS, ".ebmTransitionFillProc", "color"):"#bbee55";
						myOpacity = myCSS? findCSSValue(myCSS, ".ebmTransitionFillProc", "opacity"):0.5;
					}
				}
				// Proper Transition coloring from CSS when True/False - indicated via fillcolor
				if(transitionExecSts == 0 ){
					//transition true/false check
					if (transitionStatus == 1){
						myFillColorTransition = myCSS? findCSSValue(myCSS, ".ebmTransitionTrue", "fill-color"):"#696969";
						myFillColorOpacity = myCSS? findCSSValue(myCSS, ".ebmTransitionTrue", "opacity"):0.4;
					}else if (transitionStatus == 0){
						myFillColorTransition = myCSS? findCSSValue(myCSS, ".ebmTransitionFalse", "fill-color"):"#C0C0C0";
						myFillColorOpacity = myCSS? findCSSValue(myCSS, ".ebmTransitionFalse", "opacity"):0;
					}
				}else if(transitionExecSts == 4){
					myFillColorTransition = myCSS? findCSSValue(myCSS, ".ebmTransitionFail", "fill-color"):"#C0C0C0";
					myFillColorOpacity = myCSS? findCSSValue(myCSS, ".ebmTransitionFail", "opacity"):0;
				}
				shapeToUpdate.strokeColor = myColor;
				shapeToUpdate.stroke.opacity = myOpacity;
				if (myFillColorTransition) {
						shapeToUpdate.fillColor = myFillColorTransition;
					}
				if (myFillColorOpacity) {
					shapeToUpdate.fill.opacity = myFillColorOpacity;
					}//myOpacity;
				//shapeToUpdate.title = stepDesc; //+ "_" + type;
				break;
			case "X": //STAGE
				stepState = 0;
				for(var ii=1; i<maxNodesInStage+1;ii++){
					paramName = "seq" + ii;

					var paramNameValue = obj.parentElement.GetCustomProperty("Parameter",paramName);
					if(paramNameValue.length > 0){
						stateValue = obj.DataValue(pointName + "."+ paramNameValue);
						if(stateValue == 4 || stateValue == 5 || stateValue == 6 || stateValue == 12){  //STEP.STATE : 4-Active,5-ActiveComplt,6-ProcOutputs,7-ProcMessages,8-ProcMinWait
							stepState = 1;
						}
					}
				}
				myOpacity = 0;
				
				if(stepState == 1){
					myColor = myCSS? findCSSValue(myCSS, ".ebmStepFillProc", "color"):"#00FF00";
					myOpacity =  myCSS? findCSSValue(myCSS, ".ebmStepFillProc", "opacity"):0.5;
				}
				shapeToUpdate.fillColor = myColor;
				shapeToUpdate.fill.opacity = myOpacity;
				break;
			default:
				break;
			}
	//}catch(e){
	//	if(myDebug){debugWrite("updateFlow() -> ","Error in update: "+e.description + " >>>" + pointName + " >>>" + paramName + "<<<");}
	//}
	blkIdentifier = shapeToUpdate.getAttribute("identifier");
	if(myDebugMode) {
		if(blkIdentifier){
			shapeToUpdate.title = blkIdentifier + "\r\n\r\n" + testDecode(oData,type);
		}
	}else{
			if(blkIdentifier){
				shapeToUpdate.title = blkIdentifier.split(":")[2] + ":" + blkIdentifier.split(":")[1] + "\r\n" + testDecode(oData,type);
			}
	}
	return null;
}
//
//	printAttributes
//	Debug function for printing attributes of an object to screen.
//
function printAttributes(tArray){
	var tString, i;
	tString = "";
	for(i in tArray){
		tString = tString + i + ": " +tArray[i] +"\n";
	}
	alert(tString);
}
//
//	boxClickEvent
//	This function is assigned to the display objects and is run whenever a double click is pressed
//	If the type of display object is either a step or a transition then this function will call a popup
//	(either ebmstepdetail.htm or embtransitiondetail.htm).  If the block type is phase then the current display is changed
//	to show the flow information of the RCM/SCM/Activity that that phase is controlling
//
//
var boxClickEvent = function(event){
	var myType, tmpArray, objectId, phaseName, stepName, transitionName, myDataObject, pointName, tmpString, myDesc, SCMName, myMode, stnZoom;
	var alertPopup;
	event.cancelBubble = true;

	//Canvas selection indication
	objSelSeq();

	if(typeof event.srcElement == "undefined") return;
	//try{ //boxClickEvent
		if (event.srcElement.id == ""){
			if(event.srcElement.tagName.toUpperCase()=="SPAN"){
				sElement = event.srcElement.parentElement.parentElement;
			}else{
				sElement = event.srcElement.parentElement;
			}
		} else {
			sElement = event.srcElement;
		}
		//exit if not clicked on an R/SCM block (id contains ":")
		if(sElement.id.search(/:/) == -1)  return;
	
		tmpArray = sElement.id.split(":");
		myType = tmpArray[2]; //block type - "S"-step,"T"-trans,"P"-phase,"X"-stage
		objectId = tmpArray[3];
	
		//Get the dataBlock shape id
		var oShape = sElement.parentElement.parentElement.parentElement;
		myDataObject = oShape.objects("dataBlock" + objectId);//document.getElementById(tmpArray[0].split("_")[0] + "_dataBlock" + objectId);
		var objDS = decodeBlockData(myDataObject);
	
		if(myDataObject){
			pointName = myDataObject.parentElement.parentElement.GetCustomProperty("point", "pointName");
		}else{
			return;
		}
		var myOrderObject = oShape.Objects("order");
		var myModeObject = oShape.Objects("mode");

		myPointName = myOrderObject.DataValue("pointName.NAME");
		if (myModeObject) {
			myMode = myModeObject.DataValue("pointName.MODE");
		}else{
			myMode = 1;	
		}

		if(myType == "P"){
			if (myMode == 3){ //SINGLESTEP
				SingleStep_handler(sElement);
			}else{
				phaseName = tmpArray[1];
				tmpString = myPointName+"."+phaseName+".EQUIPNAME";
				if (myDataObject.Objects("data").DataExists(tmpString)){
					SCMName = myDataObject.Objects("data").DataValue(tmpString);
					if(SCMName.length < 1){
						window.external.Application.Parent.TemporaryMessageZoneText("EQUIPNAME not available for selected PHASE block!"	);
						window.external.Application.logmessage(window.external.Application.currentpage + ">> phase= " + phaseName + " : EQUIPNAME not available for selected PHASE block!");
						return;
					}
					objectName = tmpArray[0].replace("_myCanvas",""); //shapes with "_" in the name
					ebmflowObject = document.getElementById(objectName);
					//changeShapePointName(ebmflowObject, SCMName);
				}
				//++++ PROPOSED MODIFICATION ++++
				//++++ Call up a phase SCM detail popup instead of loading it in the same canvas (avoid additional SetCustomProperty)
				//var phaseDetail="seqShpPhaseDetail.htm";
				var phaseDetail= oShape.GetCustomProperty("Text","PhaseDetailFile");
				if(objDS){
					var phaseState=objDS.state;
					if(myPointName != "" && phaseState > 7){
						HDXPopupBehavior.CreatePopupWindow2(phaseDetail+"?currentpoint="+myPointName+"&PhaseName="+phaseName+"&AlertConfirmDisplay="+alertPopup,(window.event.clientX)/stnZoom,window.event.clientY/stnZoom,0,false);
					}
				}
				//++++
			}
		}
		if(myType == "S"){
			stepName = tmpArray[1];
			var stepDetail = oShape.GetCustomProperty("Text","StepDetailFile");
			alertPopup = oShape.GetCustomProperty("Text","AlertConfirmDisplay");
			stnZoom = document.getElementById("Page").style.zoom;
			if (myMode == 3){ //SINGLESTEP
				SingleStep_handler(sElement);
			}else{
				HDXPopupBehavior.CreatePopupWindow2(stepDetail+"?pointName="+myPointName+"&stepName="+stepName+"&AlertConfirmDisplay="+alertPopup,(window.event.clientX)/stnZoom,window.event.clientY/stnZoom,0,false);
				//HDXPopupBehavior.CreatePopupWindow2(stepDetail+"?pointName="+myPointName+"&stepName="+stepName+"&AlertConfirmDisplay="+alertPopup,window.event.x+10,window.event.y+5,0,false);
				//window.event.x+10, window.event.y+5 opens Popup near the current cursor position. 0,0 opened the Popup where the Shape was initiated, which could be outside the screen
			}
		}
		if(myType == "T"){
			var y = window.event.y;
			transitionName = tmpArray[1];
			var transitionDetail = oShape.GetCustomProperty("Text","TransitionDetailFile");
			stnZoom = document.getElementById("Page").style.zoom;
			HDXPopupBehavior.CreatePopupWindow2(transitionDetail+"?pointName="+myPointName+"&transitionName="+transitionName,(window.event.clientX)/stnZoom,window.event.clientY/stnZoom,0,false);
		}
		if(myType == "X"){
			objectName = tmpArray[0].replace("_myCanvas",""); //shapes with "_" in the name
			objectNumber = getStageShapeNumber(objectId);
			g_showStages[tmpArray[0]][objectNumber] = false;
			ebmflowObject = document.getElementById(objectName);

			//changeShapePointName(ebmflowObject,"");
			//g_showStages[tmpArray[0]][objectNumber] = false;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LogMsg("boxClickEvent :X: -> pnt = " + pointName + ": src = " + event.srcElement.id);	
		LogMsg("boxClickEvent :X: -> STG[" + objectNumber + "] = " + g_showStages[tmpArray[0]][objectNumber]);		
	//---changeShapePointName(ebmflowObject, pointName);
	//Re-Draw
LogMsg("boxClickEvent :X: -> before re-draw");	
	ReDrawCanvas(ebmflowObject,false);
LogMsg("boxClickEvent :X: -> after re-draw");	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//changeShapePointName(ebmflowObject,pointName);
		}
	//} catch(e) {
	//	if(myDebug){debugWrite("boxClickEvent: ", e.description);}
	//}
};
function getStageShapeNumber(objId){
	var stageIndex;
	stageObject = document.getElementById(objId);
	oShape = stageObject.parentElement.parentElement;
	stageName = stageObject.GetCustomProperty("Value","stageName");

	for(var i=1; i<=g_maxStages; i++){
		try{
			tmpStageName = oShape.GetCustomProperty("Value", "stageShapeName"+i);
		} catch(e) {
			break;
		}
		if(stageName == tmpStageName){
			stageIndex = i;
		}
	}
	return stageIndex;
}
//
//	canvasCMenuEvent
//
//  Canvas right click function.  Invokes Shortcut Menu
//
var canvasCMenuEvent = function(event){
	var sCMfile="";
	//event.cancelBubble = true;
	window.event.returnValue=false;
	//Canvas selection indication
	objSelSeq();
	if(event.srcElement.tagName=="SPAN"){
		objCanvasId = event.srcElement.parentElement.parentElement.parentElement.id;
		Page.setAttribute("cmenusource",event.srcElement.parentElement.parentElement.id);
	}else if(event.srcElement.id=="" || event.srcElement.id.indexOf("|")>-1){
		objCanvasId = event.srcElement.parentElement.id;
		Page.setAttribute("cmenusource","empty");
	}else if(event.srcElement.id.indexOf(":")>-1){
		objCanvasId = event.srcElement.id.split(":")[0];
		Page.setAttribute("cmenusource",event.srcElement.id);
	}else{
		objCanvasId = event.srcElement.id;
		Page.setAttribute("cmenusource","-");
	}
	oShape = document.getElementById(objCanvasId).parentElement.parentElement;
	Page.setAttribute("lastselection",objCanvasId);
	Page.setAttribute("lastselection",objCanvasId);
	sCMfile=oShape.GetCustomProperty("text","ContextMenuFile");

	if(sCMfile!="" && sCMfile!="-"){
		HDXPopupBehavior.InvokeShortcutMenu(sCMfile,0,0);
	}
};
//
//	canvasClickEvent
//
//  Canvas (double) click function.  Redraws the shape and sets the flag to use the stage shapes.
//
var canvasClickEvent = function(event){
	event.cancelBubble = true;
	event.returnValue = false;
	return;
	/*var lastseq = Page.getAttribute("lastselectionseq");
	var ebmflowObject = document.getElementById(lastseq).parentElement.parentElement;
	var pointName = ebmflowObject.GetCustomProperty("point", "pointName"); //event.srcElement.parentElement.parentElement.GetCustomProperty("point", "pointName");
	
	if (pointName.indexOf("%")>0) {
		pointName = DisplayDataRepository.GetValue(pointName.split("%")[1]);
	}
	
	//ebmflowObject = event.srcElement.parentElement.parentElement;
	//--- changeShapePointName(ebmflowObject, "");
	//---changeShapePointName(ebmflowObject, pointName);
LogMsg("canvasClickEvent -> pnt = " + pointName + ": src = " + event.srcElement.id);	
	//Re-Draw (all stages will appear collapsed)
	ReDrawCanvas(ebmflowObject, true);*/
};
//
// Left Click action - calls the objSelSeq() function to select the Sequence Shape clicked
//
var canvasClickEventSel = function(event){
	objSelSeq();
};
//
// Sequence shape selection
// objSelSeq() - equivalent to ObjSel() in Solution Pack - provides selection functionality for Sequence Shape
//
//      Sets/Retreives a Page attribute <lastselectionseq> to keep track of last selected Sequence shape and to keep only one selected at a time.
//      <TO BE INTEGRATED WITH THE SOLUTION PACK SELECTION>
//
function objSelSeq() {
	var srcElem = window.event.srcElement;
	if(srcElem){
		while(srcElem.parentElement){
			if(srcElem.id!=""&&srcElem.id.indexOf("myCanvas")>-1&&srcElem.id.indexOf(":")==-1){break;}
			srcElem = srcElem.parentElement;
		}
		var oCanvas = srcElem;//window.event.srcElement;
		var lastseq = Page.getAttribute("lastselectionseq");
		if(lastseq){
			document.getElementById(lastseq).style.borderStyle="solid";
			document.getElementById(lastseq).style.borderColor=findCSSValue(myCSS, ".ebmCanvas", "borderColor");
		}
		oCanvas.style.borderStyle = findCSSValue(myCSS, ".ebmCanvas_Selected", "borderStyle");
		oCanvas.style.borderColor = findCSSValue(myCSS, ".ebmCanvas_Selected", "borderColor");
		Page.setAttribute("lastselectionseq",srcElem.id);
	}
}
//
//	findNextDataShape
//	This function will loop through the data objects until it finds one that is not used.  That shape is returned if no shape is found
//	the function will then returnn a null value.  It test the 1st parameter for a empty value to determined if used.
//
function findNextDataShape(container){
	var myShape, i, tmpString, param1;
	try {
		myShape = container.parentElement.parentElement;

		for(i=1;i<=g_maxDataBlocks;i++){
			tmpString = "dataBlock"+("00"+i).slice(-3);
			param1 = myShape.objects(tmpString).GetCustomProperty("Parameter","param1");
			if( param1 == ""){
				return myShape.Objects(tmpString);
			}
		}
	} catch(e) {
		if(myDebug){debugWrite("findNextDataShape -> ","Error :" + e.description);}
	}
	return null;
}
//
//	findNextStageDataShape
//	This function will loop through the stage data objects until it finds one that is not used.  That shape is returned if no shape is found
//	the function will then returnn a null value.  It test the 1st parameter for a empty value to determined if used.
//
function findNextStageDataShape(container){
	var myShape, i, tmpString, param1;
	try {
		myShape = container.parentElement.parentElement;
		for(i=1;i<=g_maxStages;i++){
			tmpString = "dataStage"+("00"+i).slice(-3);
			try{
				param1 = myShape.Objects(tmpString).GetCustomProperty("Parameter","seq1");
				if( param1 == ""){
					return myShape.Objects(tmpString);
				}
			}catch(e){
				break;
			}
		}
	} catch(e) {
		if(myDebug){debugWrite("findNextStageDataShape -> ","Error :" + e.description);}
	}
	return null;
}
//
//	unLinkAllDataShapes
//	This function will loop through all data objects and remove all data associations.  This routine is used to clean up te shape before
//	assigning new data to the containers.
//
function unLinkAllDataShapes(container){
	var myShape, tmpString, param1, myObject;
	try {
		myShape = container.parentElement.parentElement;
		for(var i=1;i<=g_maxDataBlocks;i++){
			tmpString = "dataBlock"+("00"+i).slice(-3);
			myObject = myShape.Objects(tmpString);
			tmpPointName = myObject.GetCustomProperty("point","pointName");
			if(tmpPointName!=""){
				myObject.autoReloadContent = false;
				param1 = myObject.SetCustomProperty("point","pointName","");
				param1 = myObject.SetCustomProperty("Parameter","param1","");
				param1 = myObject.SetCustomProperty("Parameter","param2","");
				param1 = myObject.SetCustomProperty("Parameter","param3","");
				param1 = myObject.SetCustomProperty("Parameter","param4","");
				param1 = myObject.SetCustomProperty("Parameter","param5","");
				//param1 = myObject.SetCustomProperty("Parameter","param6","");
				//param1 = myObject.SetCustomProperty("Parameter","param7","");
				param1 = myObject.SetCustomProperty("Value","blockType","");
				param1 = myObject.SetCustomProperty("Value","isSelected","");
				myObject.ReloadContent();
			}else{
				break;
			}
		}
		for(var ii=1;ii<=g_maxStages;ii++){
			tmpString = "dataStage"+("00"+ii).slice(-3);
			try{
				myObject = myShape.Objects(tmpString);
				if(myObject){
					myObject.autoReloadContent = false;
					param1 = myObject.SetCustomProperty("point","pointName","");
					for(var j=1;j<=g_maxNodesInStage;j++){
						sequenceNumber = "seq"+j;
						param1 = myObject.SetCustomProperty("Parameter",sequenceNumber,"");
					}
				myObject.ReloadContent();
				}
			}catch(e){
				break;
			}
		}
	} catch(e) {
		if(myDebug){debugWrite("unLinkAllDataShapes", "Error in unLinkAllDataShapes" + e.description);}
	}
	return null;
}
//
//	findCSSFile
//	This function finds a CSS sheet based on the passed cssFileName else it returns a null.
//
function findCSSFile(cssFileName){
	var tmpString, re, i, j;
	tmpString = "[\\,\/]" + cssFileName + ".*\\.css";
	re = new RegExp(tmpString,"ig");
	for(i=0; i< document.styleSheets.length; i++){
		if(document.styleSheets[i].imports.length > 0){
			for(j=0;j<document.styleSheets[i].imports.length;j++){
				if(document.styleSheets[i].imports[j].href.search(re)>0){
					return document.styleSheets[i].imports[j];
				}
			}
		}
	}
	return null;
}
//
//	findCSSValue
//	This function finds a CSS sheet value based on the passed cssRule, selector, and style else it returns a null.
//

function findCSSValue(cssRule, selector, myStyle){
	var i;
	if(cssRule){
		for (i=0;i<cssRule.rules.length;i++){
			if(cssRule.rules[i].selectorText.toLowerCase() == selector.toLowerCase()){
				return cssRule.rules[i].style[myStyle];
			}
		}
	}
	return null;
}
//
//	Drawing Routines
//	The following routines draw the flow shape.
//	These shapes use VML to contruct their diagrams.
//
//	These shapes have common variables of
//	h - height in pixels
//	w - width in pixels
//	x - starting x position in pixels
//	y - starting y position in pixels
//	myClass - class name of shape
//	myCSS -	CSS file name
//	myStrokeColor - Line color
//	myFillColor - fill color
//	o - Shape object created
//
//
//	drawStep
//	This fuction takes the container name, the starting x, and y coordinates and the name of the block and draws the step shape.
//
function drawStep(container, x, y, myname){
	var h, w, myclass, o, myStrokeColor, myFillColor;
	h = ebmShapeHeight;
	w = ebmShapeWidth;
	myclass = "ebmStep";
	o = drawRect(container, x, y, h, w, myname, myclass);
//	o.title = container.id+myname;
	myStrokeColor = findCSSValue(myCSS, ".ebmStep","borderColor");
	o.strokecolor = myStrokeColor? myStrokeColor:"#808080";
	myStrokeWeight = findCSSValue(myCSS, ".ebmStep","strokeweight");
	o.strokeweight = myStrokeWeight? myStrokeWeight:"2px";
	myFillColor = findCSSValue(myCSS, ".ebmStep","fillColor");
	o.fillColor =myFillColor? myFillColor: "#808080";
	o.fill.opacity = 0.0;
	o.id = container.id+myname;
	o = addText(o,myname);
	return o;
}
//
//	drawStage
//	This fuction takes the container name, the starting x, and y coordinates and the name of the block and draws the step shape.
//
function drawStage(container, x, y, myname){
	var h, w, myclass, o, myStrokeColor, myFillColor;
	h = ebmShapeHeight;
	w = ebmShapeWidth;
	myclass = "ebmStage";
	o = drawRect2(container, x, y, h, w, myname, myclass);
	myStrokeColor = findCSSValue(myCSS, ".ebmStage","borderColor");
	o.strokecolor = myStrokeColor? myStrokeColor:"#008000";
	myStrokeWeight = findCSSValue(myCSS, ".ebmStage","strokeweight");
	o.strokeweight = myStrokeWeight? myStrokeWeight:"2px";
	myFillColor = findCSSValue(myCSS, ".ebmStage","fill-color");
	o.fillColor =myFillColor? myFillColor: "#008000";
	o.fill.opacity = 0.0;
	o.id = container.id+myname;
	o = addText(o,myname);
	o.title = myname;
	return o;
}
//
//	drawPhase
//	This fuction takes the container name, the starting x, and y coordinates and the name of the block and draws the phase shape.
//
function drawPhase(container, x, y, myname){
	var h, w, myclass, o, myStrokeColor, myFillColor;
	h = ebmShapeHeight;
	w = ebmShapeWidth;
	pointName = container.parentElement.parentElement.GetCustomProperty("point","pointName");
	myDataObject = container.parentElement.parentElement.Objects("order");
	//myProcLevel = myDataObject.DataValue(pointName+".ProceduralLevel");
	myProcLevel = 1;
	switch(myProcLevel){
		case 1:
			myclass = "ebmPhase";
			break;
		case 2:
			myclass = "ebmOperation";
			break;
		case 3:
			myclass = "ebmUnitProcedure";
			break;
		case 4:
			myclass = "ebmProcedure";
			break;
		default:
			myclass = "ebmProcedure";
	}
	o = drawRect2(container, x, y, h, w, myname, myclass);
	myStrokeColor = findCSSValue(myCSS, "."+myclass,"borderColor");
	o.strokecolor = myStrokeColor? myStrokeColor:"#808080";
	myStrokeWeight = findCSSValue(myCSS, "."+myclass,"strokeweight");
	o.strokeweight = myStrokeWeight? myStrokeWeight:"2px";
	myFillColor = findCSSValue(myCSS, "."+myclass,"fill-color");
	o.fillColor = myFillColor? myFillColor: "#808080";
	o.fill.opacity = 0.0;
//	o.title = container.id+myname;
	o = addText(o,myname);
	o.id = container.id+myname;
	return o;
}
//
//	drawTransition
//	This fuction takes the container name, the starting x, and y coordinates and the name of the block and draws the transition shape.
//
function drawTransition(container, x, y, myname){
	var h, w, myclass, o, myStrokeColor, myFillColor;
	h = ebmShapeHeight;
	w = ebmShapeWidth;
	myclass = "ebmTransition";
	o = drawDiamond(container, x, y, h, w, myname, myclass);
	myStrokeColor = findCSSValue(myCSS, ".ebmTransition","borderColor");
	o.strokecolor = myStrokeColor? myStrokeColor:"#808080";
	myStrokeWeight = findCSSValue(myCSS, ".ebmTransition","strokeweight");
	o.strokeweight = myStrokeWeight? myStrokeWeight:"2px";
	myFillColor = findCSSValue(myCSS, ".ebmTransition","fill-color");
	o.fillColor =myFillColor? myFillColor: "#808080";
	o.fill.opacity = 0.0;
	o.id = container.id+myname;
	return o;

}
//
//	drawSync
//	This fuction takes the container name, the starting x, and y coordinates and the name of the block and draws the sync shape.
//
function drawSync(container, x, y, myname){
	var h, w, myclass, o, myStrokeColor;
	h = ebmShapeHeight;
	w = ebmShapeWidth;
	myclass = "ebmSync";
	o = drawParallel(container, x, y, h, w, myname, myclass);
	myStrokeColor = findCSSValue(myCSS, ".ebmSync","borderColor");
	o.strokecolor = myStrokeColor? myStrokeColor:"#000000";
	myStrokeWeight = findCSSValue(myCSS, ".ebmSync","strokeweight");
	o.strokeweight = myStrokeWeight? myStrokeWeight:"2px";
	o.id = container.id+myname;
	return o;

}
//
//	drawShape
//	This fuction draws the shape based on the path passed to it.  It is a generic drawing routine used by each of the drawXXX routines.
//
function drawShape(container, x, y, h, w, myname, myclass, path){
	var o;
	o = document.createElement("v:shape");
	o = container.appendChild(o);
	o.className = myclass;
	o.strokeweight = "2px";
	o.style.left = x+"px";
	o.style.top = y + "px";
	o.coordOrigin ="0 0";
	o.coordSize= w+" "+h;
	o.style.width = w + "px";
	o.style.height = h + "px";
	o.style.position = "absolute";
	o.path = path;
	return o;
}
//
// 	drawRect
//	This function draws a rectangle or a path based on the myclass variable
//
function drawRect(container, x, y, h, w, myname, myclass){
	var o, r, myPath;
	r = 10;
	cssPath = findCSSValue(myCSS, "."+myclass,"path");
	myPath = "m "+r+",0 "+"l "+(w-r) +",0 qx "+w+","+r+" l "+w+","+(h-r)+" qy "+(w-r)+","+h+" l "+r+","+h+" qx 0,"+(h-r)+" l 0,"+r+" qy "+r+",0 e";
	myPath = cssPath ? eval(cssPath) : myPath;
	o = drawShape(container, x, y, h, w, myname, myclass, myPath);
	return o;
}
//
// 	drawRect2
//	This function draws a rectangle for stage shape or a path based on the myclass variable
//
function drawRect2(container, x, y, h, w, myname, myclass){
	var o, r, myPath;
	r = 10;
	cssPath = findCSSValue(myCSS, "."+myclass,"path");
	myPath = "m "+r+",0 "+"l "+(w-r) +",0 qx "+w+","+r+" l "+w+","+(h-r)+" qy "+(w-r)+","+h+" l "+r+","+h+" qx 0,"+(h-r)+" l 0,"+r+" qy "+r+",0"+"m -3,0 l -3, "+ h + "m " +(w+3) + ",0 l "+(w+3)+"," + h +" e";
	myPath = cssPath ? eval(cssPath) : myPath;
	o = drawShape(container, x, y, h, w, myname, myclass, myPath);
	return o;
}
//
// 	drawDiamond
//	This function draws a diamond or a path based on the myclass variable
//
function drawDiamond(container, x, y, h, w, myname, myclass){
	var o, myPath, cssPath;
	cssPath = findCSSValue(myCSS, ".ebmTransition","path");
	myPath = "m "+ Math.round(w/2)+",0 l "+ Math.round((w+h)/2)+","+Math.round(h/2)+", "+Math.round(w/2)+","+h+", "+Math.round((w-h)/2)+","+Math.round(h/2)+" x e";
	myPath = cssPath ? eval(cssPath) : myPath;
	o = drawShape(container, x, y, h, w, myname, myclass, myPath);
	return o;
}
//
// 	drawParallel
//	This function draws a parallel lines or a path based on the myclass variable
//
function drawParallel(container, x, y, h, w, myname, myclass){
	var o, myPath, y1, y2;
	y1 = h/2 -2;
	y2 = y1 + 4;
	myPath = "m 0,"+y1+" l "+w+","+y1+" m 0,"+y2+" l "+w+","+y2+" m "+w/2+", -1 l "+w/2+", "+y1+" m "+w/2+", "+y2+" l "+w/2+", "+(h+1)+" e";
	o = drawShape(container, x, y, h, w, myname, myclass, myPath);
	return o;
}
//
//	addText
//	This function adds text to a shape
//
function addText(shape,myText){
	var myDiv, mySpan;
	o = document.createTextNode(myText);
	myDiv = document.createElement("div");
	myDiv.style.width = shape.style.width;
	myDiv.style.height = shape.style.height;
	myDiv.className  = "ebmBlockLabelHolder";
	mySpan = document.createElement("span");
	mySpan.className  = "ebmBlockLabelText";
	mySpan.style.width = shape.style.width;
	mySpan.style.height = shape.style.height;
	mySpan.appendChild(o);
	myDiv.appendChild(mySpan);
	shape.appendChild(myDiv);
	shape.style.width=shape.style.width;
	return shape;
}
//
//	connectObjects
//	This function draws the connections between the fromObject and the toObject.
//	Note there are cases of lines depending on the relative relationship between the objects.
//
//
function connectObjects(container, fromObject, toObject){
	var startX, startY, endX, endY, x0, x1, x2 ,xe, y0, y1, y2, ye, myTop, myLeft, myPath, w, h, o;
	var x = [], y = [], numVertices, cornerSize, icornerSize;
	startX = parseInt(fromObject.coordSize.x)/2 + parseInt(fromObject.style.left);
	startY = parseInt(fromObject.coordSize.y) + parseInt(fromObject.style.top);
	endX = parseInt(toObject.coordSize.x)/2 + parseInt(toObject.style.left);
	endY = parseInt(toObject.style.top);
	cornerSize = 4;
	icornerSize = cornerSize;
	//get fromObject NextComp
	//iNextComp = fromObject.getAttribute("nextComp");
	//if(iNextComp){
	//	var iOffset = iNextComp*5;
	//}else{
		var iOffset=0;
	//}
	//
	// Case for end block positioned lower than start block 
	//
	if(startY<endY){
//if(myDebug){debugWrite("ConnectObjects : " + fromObject.id.split(":")[1] + " -> " + toObject.id.split(":")[1],"");}
//if(myDebug){debugWrite("ConnectObjects : " + iNextComp,"");}
	numVertices = 10;
		halfWidth = parseInt(fromObject.coordSize.x + ebmShapeSideMargin)/2;
		if((Math.abs(startX-endX) < 1) &&((endY - startY) <= 2*ebmShapeTopMargin)){
			cornerSize = 0;
			halfWidth = 0;
		}
		if(Math.abs(startY -endY) <= 2*ebmShapeTopMargin){
			icornerSize = 0;
		}
		y[0] = 0;
		y[1] = y[0] + (5 - cornerSize);
		y[2] = y[1] + cornerSize;
		y[3] = y[2];
		y[4] = y[3] + icornerSize;
		y[5] = y[4] + endY- startY - 2* icornerSize - ebmShapeTopMargin;
		y[6] = y[5] + icornerSize;
		y[7] = y[6];
		y[8] = y[7] + cornerSize;
		ye = endY - startY;
		myTop = startY;
		myLeft = startX;
		//
		// End blocks is directly below start block
		//
		if(startX <= endX){
//if(myDebug){debugWrite("ConnectObjects : " + fromObject.id.split(":")[1] + " -> " + toObject.id.split(":")[1],"");}
			x[0] = iOffset;
			x[1] = x[0];
			x[2] = x[1] + cornerSize;
			x[3] = x[2] + halfWidth - 2 * cornerSize - iOffset;
			x[4] = x[3] + cornerSize;
			x[5] = x[4];
			x[6] = x[5] + cornerSize;
			x[7] = x[6] + endX - startX - (halfWidth + 2* cornerSize);
			x[8] = x[7] + cornerSize;
			xe = x[8];
		}
//if(myDebug){debugWrite("ConnectObjects : " + fromObject.id.split(":")[1] + " -> " + toObject.id.split(":")[1],"");		}
		//
		// Start block is to the right of the End block
		//
		if(startX > endX){
//if(myDebug){debugWrite("ConnectObjects : " + fromObject.id.split(":")[1] + " -> " + toObject.id.split(":")[1],"");}
			myLeft = endX;
			x[0] = startX - endX +iOffset;
			x[1] = x[0];
			x[2] = x[1]- cornerSize;
			x[3] = x[2] - (halfWidth - 2 * cornerSize);
			x[4] = x[3] - cornerSize;
			x[5] = x[4];
			x[6] = x[5] - cornerSize;
			x[7] = cornerSize;
			x[8] = x[7] - cornerSize;
			xe = x[8];
		}
		//
		// Case for node being directly below node but being more than 1 node apart  have routed line around other nodes.
		//
		if(startX == endX && (Math.abs(endY - startY) > 2*ebmShapeTopMargin)){
//if(myDebug){debugWrite("ConnectObjects : " + fromObject.id.split(":")[1] + " -> " + toObject.id.split(":")[1],"");}
			x[0] = iOffset;
			x[1] = x[0];
			x[2] = x[1] + cornerSize;
			x[3] = x[2] + halfWidth - 2 * cornerSize;
			x[4] = x[3] + cornerSize;
			x[5] = x[4];
			x[6] = x[5] - cornerSize;
			x[7] = x[6] + endX - startX - (halfWidth - 2* cornerSize) - iOffset;
			x[8] = x[7] - cornerSize;
			xe = x[8];
		}
//if(myDebug){debugWrite("Drawing Lines:", fromObject.id +" corner size = " + cornerSize + " halfWidth = " + halfWidth +"<br>" + startX +" " + startY +"<br>" + endX +" " + endY + "<br> x[0] = " + x[0]+ "<br> x[1] = " + x[1]+ "<br> x[2] = " + x[2]+ "<br> x[3] = " + x[3]+ "<br> x[4] = " + x[4]+ "<br> x[5] = " + x[5]+ "<br> x[6] = " + x[6]+ "<br> x[7] = " + x[7]+ "<br> x[8] = " + x[8])}
		myPath = "m "+ x[0] + ","+ y[0] + " l ";
		for( var i = 1; i < numVertices - 1 ; i++){
			myPath = myPath + x[i] + "," + y[i] + ",";
		}
		myPath = myPath + xe + "," + ye;
		myPath = myPath + " e";
		w = Math.abs(endX - startX);
		h = Math.abs(endY - startY);
	} else {
	//
	// Case for end block positioned higher than start block 
	//		
//if(myDebug){debugWrite("ConnectObjects : " + fromObject.id.split(":")[1] + " -> " + toObject.id.split(":")[1],"");}
		w=Math.abs(endX-startX) +parseInt(toObject.coordSize.x) + 5 - parseInt(fromObject.coordSize.x)/2;
		h=Math.abs(endY-startY) + ebmShapeTopMargin;
		numVertices = 10;
		// Vertice points Y-coordinates
		y[0] = 5 + parseInt(toObject.coordSize.y) + parseInt(fromObject.style.top) - parseInt(toObject.style.top);
		y[1] = y[0] + (5 - cornerSize);
		y[2] = y[1] + cornerSize;
		y[3] = y[2];
		y[4] = y[3] - cornerSize;
		y[5] = 0 + cornerSize;
		y[6] = 0;
		y[7] = y[6];
		y[8] = cornerSize;
		ye = 5;
		// Vertice points X-coordinates
		x[0] = (parseInt(fromObject.coordSize.x) + parseInt(toObject.coordSize.x))/2 +  parseInt(fromObject.style.left) - (parseInt(toObject.style.left)+parseInt(toObject.coordSize.x));
		x[1] = x[0];
		x[2] = x[1] + cornerSize;
		x[3] = w - cornerSize;
		x[4] = w;
		x[5] = x[4];
		x[6] = x[5] - cornerSize;
		x[7] = 0 + cornerSize;
		x[8] = 0;
		xe = 0;
		// Path formation
		myPath = "m "+ x[0] + ","+ y[0] + " l ";
		for( var ii = 1; ii < numVertices - 1  ; ii++){
			myPath = myPath + x[ii] + "," + y[ii] + ",";
		}
		myPath = myPath + xe + "," + ye;
		myPath = myPath + " e";
		myTop = parseInt(toObject.style.top)-5;
		myLeft = parseInt(toObject.style.left) + parseInt(toObject.coordSize.x)/2;
	}
	o = document.createElement("v:shape");
	o = container.appendChild(o);
	o.className = "connection";
	var myStrokeWeight = findCSSValue(myCSS, ".connection","strokeweight");
	var myStrokeColor = findCSSValue(myCSS, ".connection","strokecolor");
	o.strokeweight = myStrokeWeight?myStrokeWeight:"2px";
	o.strokecolor = myStrokeColor?myStrokeColor:"#000";
	o.fillcolor = "none";
	o.style.left = myLeft+"px";
	o.style.top = myTop + "px";
	o.coordOrigin ="0 0";
	o.coordSize= (w+3)+" "+h;
	o.style.width = (w+3) + "px";
	o.style.height = h + "px";
	o.style.position = "absolute";
	o.path = myPath;
	
	iNextComp = fromObject.getAttribute("nextComp");
	if(iNextComp) {
		fromObject.setAttribute("nextComp",parseInt(iNextComp)+1);
	}else{
		fromObject.setAttribute("nextComp",1);
	}
	//set from,to attribute string to the connection line
	o.setAttribute("route",fromObject.id.split(":")[1] + "," + toObject.id.split(":")[1]);
	o.setAttribute("nextComp",fromObject.getAttribute("nextComp"));
	o.id = fromObject.id.split(":")[1] + "|" + toObject.id.split(":")[1];
//if(myDebug){debugWrite("ConnectObjects : line = " + o.getAttribute("route") + " : nextComp = " + o.getAttribute("nextComp"),"");}
}
//
//	changeShapePointName
//	This function is used when the flow shape needs to draw a different SCM/RCM/Activity based on moving up or down the heirarchy of the sequence.
//
function changeShapePointName(shapeObj, newPointName){
	var tmpPnt = shapeObj.getCustomProperty("Point","pointName");
	//FIX 2018/02/07 - added case for Activity config, when Activity name is set via display CP (for better performance)
	if (typeof (tmpPnt) != "undefined"){
		if(tmpPnt.indexOf("%")>0){
			sDspCP = tmpPnt.split("%")[1];
			DisplayDataRepository.PutValue(sDspCP,"");
			//Push new point name (with timeout).
			window.setTimeout(function(){DisplayDataRepository.PutValue(sDspCP,newPointName);},1000); //[TIMEOUT] - changeShapePointName
		}else{
			shapeObj.setCustomProperty("point", "pointName", newPointName);
		}
	
	}
}
//
// Stage Shape Functions
//
function getStageStepPhaseList(container, blockName, tEBM){
	myShape = container.parentElement.parentElement;
	for(var j = 1; j<=g_maxStages ; j++){
		//
		//  Try/catch to take care of flowshapes with less than g_maxStages value properties
		//
		try{
			if(blockName == myShape.getCustomProperty("value", "stageShapeName"+j)){
				startStage =  myShape.getCustomProperty("value", "stageShapeStart"+j);
				endStage =  myShape.getCustomProperty("value", "stageShapeEnd"+j);
			}
		} catch(e) {
			break;
		}
	}
	blocks = tEBM.listBlocks(startStage,endStage);
	names = [];
	for(var i in blocks){
		if(tEBM.getType(blocks[i]).match(/^[SP]/)){
			names.push(blocks[i]);
		}
	}
	return names;
}
//
// Stage Order Creation
//
function addStageToOrder(myOrder, stageShapeName, stageShapeStart, stageShapeEnd){
	var tmpArray, newNode, removeNode, stageOrder;
	if(stageShapeName.length == 0) return myOrder;
	if(myOrder != ""){
		testEBM = new ebm(myOrder);
		stageList = testEBM.listBlocks(stageShapeStart, stageShapeEnd);
		if(stageList.length < 1 || stageList.length > 30){
			return myOrder;
		}
		shapeStartList = stageShapeStart.split(" ");
		shapeEndList = stageShapeEnd.split(" ");
		if(stageList.length == 0) return myOrder;
		var newOrder = "";
		var foundStageNode = false;
		for(var myNode in testEBM.blocks) {
			newNode = testEBM.blocks[myNode];
			removeNode = false;
			if(testEBM.blocks[myNode].split(",")[0].match(/^[STYPX]/) ){
				tmpArray = testEBM.blocks[myNode].split(",");
				// not sure if next line is used at all
				if(tmpArray.length < 3) break;
				// check to see if node is part of stagelist and set a remove node flag if it is...
				for (var i = 0; i < stageList.length; i++){
					if(stageList[i].toUpperCase() == tmpArray[2].toUpperCase()) {
						removeNode = true;
					}
				}
				// find all references to start list and set to new stage node.
				for (var ii = 3; ii < tmpArray.length; ii++) {
					tmpArray[i] = testEBM.getName(tmpArray[ii]);
					for( var j = 0; j< shapeStartList.length; j++){
						if(tmpArray[ii].toUpperCase() == shapeStartList[j].toUpperCase()){
							tmpArray[ii] = stageShapeName;
						}
					}
				}
				if((tmpArray[2].toUpperCase() == shapeEndList[0].toUpperCase() || shapeEndList[0] == "") && !foundStageNode){
					removeNode = false;
					tmpArray = [];
					tmpArray[0] = "X";
					tmpArray[1] = "0";
					tmpArray[2] = stageShapeName;
					outputs = [];
					for(var k = 0; k<shapeEndList.length; k++){
						outputs.push(testEBM.getOutputs(shapeEndList[k]));
					}
					var o = uniqueList(outputs).toString("").split(",");
					for(var kk = 0;kk<o.length;kk++){
						if(o[kk] != ""){
							tmpArray[kk+3] = testEBM.getName(o[kk]);
						}
					}
					foundStageNode = true;
				}
				if(tmpArray.length > 3){
					for( var m = tmpArray.length-1; m > 3; m--){
						if(tmpArray[3] == tmpArray[m]){
							tmpArray.splice(m,1);
						}
					}
				}
				newNode = tmpArray.join(",");
			}
			if(!removeNode){
				if(newNode !=" "){
					newOrder = newOrder + newNode + ";";
				}
			}
		}
		//
		// Update Order with index numbers instead of names
		//
		test2EBM = new ebm(newOrder);
		stageOrder = "";
		for(var myNode2 in test2EBM.blocks){
			newNode = test2EBM.blocks[myNode2];
			if(test2EBM.blocks[myNode2].split(",")[0].match(/^[STYPX]/) ){
				tmpArray = test2EBM.blocks[myNode2].split(",");
				for(var mm = 3; mm< tmpArray.length; mm++){
					tmpArray[mm] = test2EBM.getIndex(tmpArray[mm]);
				}
				newNode = tmpArray.join(",");
			}
			// null length test
			if(newNode.length >0){
				stageOrder += newNode + ";";
			}
		}
	} else {
		stageOrder = myOrder;
	}
	return stageOrder;
}
//Dummy - for backward compatibility
function updateStageOrder(stageOrder, stageShapeName, stageShapeStart, stageShapeEnd){
	addStageToOrder(stageOrder, stageShapeName, stageShapeStart, stageShapeEnd);
}
//
function updateOrderWithStages(myOrder,myHandler,oShape,bResetStages){
	var stageOrder, starNode, oCanvas;	
		
		oCanvas = oShape.objects("myCanvas");
LogMsg("updateOrderWithStages -> ResetStages? = " + bResetStages);	
		if(!g_showStages[oCanvas.id] || bResetStages){
			//Reset stage visibility
			g_showStages[oCanvas.id] = [];
			//Set all stages to visible (collapsed)
			for(var i=1; i<=g_maxStages; i++){
				g_showStages[oCanvas.id][i] = true;
			}
		}
		stageOrder = myOrder;
		startNode = myHandler;
		for(var j=1;j<=g_maxStages;j++){
LogMsg("updateOrderWithStages -> Stage[" + j + "] = " + g_showStages[oCanvas.id][j]);
			if(g_showStages[oCanvas.id][j]) {
	
				try{
					var stageShapeName = oShape.GetCustomProperty("Value","stageShapeName"+j);
					var stageShapeStart = oShape.GetCustomProperty("Value","stageShapeStart"+j);
					var stageShapeEnd = oShape.GetCustomProperty("Value","stageShapeEnd"+j);
					startNode = stageShapeStart.match(/invokemain/i) ? stageShapeName : startNode;
					if(stageShapeName != ""){
						stageOrder = addStageToOrder(stageOrder, stageShapeName, stageShapeStart, stageShapeEnd);
					}
				}catch(e){
					break;
				}
			}
		}
		myOrder=stageOrder;
		//
		return myOrder;
}

//
// Context Menu Support functions - flowDetail()
//
function flowDetail(){
	var sname;
	sElement = Page.getAttribute("lastselectionseq");

	if(sElement.search(/:/) == -1){
		oshape = document.getElementById(sElement).parentElement.parentElement;
		sname=oshape.GetCustomProperty("Point","pointName");
		if(sname.indexOf("%") > 0){ //CP configured with Display CP
			sname = DisplayDataRepository.GetValue(sname.split("%")[1]);
		}else{
			sname = tmpPnt;
		}
	}else{
		tmpArray = sElement.split(":");
		objectId =  tmpArray[3];
		myObject = document.getElementById(objectId);
		pointName = myObject.parentElement.parentElement.parentElement.GetCustomProperty("point", "pointName");
		myOrderObject = myObject.parentElement.parentElement.parentElement.Objects("order");
		myPointName = myOrderObject.DataValue(pointName+".NAME");
		phaseName = tmpArray[1];
		//tmpString = myPointName+"."+phaseName+".EQUIPNAME";
		//sname = myObject.Objects("data").DataValue(tmpString);
	}
	window.external.Application.CurrentPage= "sysdtlscma.htm?point="+sname;
}
//
// Context Menu Support functions - flowFaceplate()
//
function flowFaceplate(){
	var pntName;
	oShape = document.getElementById(Page.getAttribute("lastselectionseq")).parentElement.parentElement;
	var tmpName=oShape.GetCustomProperty("Point","pointName");
	if(tmpName.indexOf("%")>0){
		pntName = DisplayDataRepository.GetValue(tmpName.split("%")[1]);
	}else{
		pntName = tmpName;
	}
	window.setTimeout(function(){HDXPopupBehavior.CreatePopupWindow("sysdtlscma_fp.htm?currentpoint="+pntName,0,0,1);},1,"JScript");
}
//
// Context Menu Support functions - flowExpand()
//
function flowExpand(){
	var sObjSel = Page.getAttribute("lastselectionseq");
	var oObjSel = document.getElementById(sObjSel);
	
	if (oObjSel){
		var oShape = oObjSel.parentElement.parentElement;

	}
	//oshape = document.getElementById(Page.getAttribute("lastselectionseq")).parentElement.parentElement;

	vshape = document.getElementById(Page.getAttribute("cmenusource"));
LogMsg("flowExpand -> src = " +  vshape.id );
	if(vshape.id.indexOf(":X:")>-1){vshape.click();}
}
//
// Set "Go" attribute to the respective SequenceShape to allow further updates
function goShp(i){
if(myDebugMode) LogMsg("ebmgsh goShp LaunchDefer -> GO = " + i + ": shapeID = " + Page.getAttribute("SeqShpID" + i));	
	document.getElementById(Page.getAttribute("SeqShpID" + i)).setAttribute("Go_update", "Go");
}
//
// Force updates on SequenceShape
function updShp(i){
if(myDebugMode) LogMsg("ebmgsh updShp LaunchDefer -> UPD = " + i  + ": shapeID = " + Page.getAttribute("SeqShpID" + i));		
	document.getElementById(Page.getAttribute("SeqShpID" + i) + "_order").onupdate();
}
//
//
// ebm_Order_OnUpdate
//
function ebm_Order_Onupdate(oSource){
	var oState, myZoomLevel, myState;
	var oShape =  oSource.parentElement.parentElement;
	var oCanvas =  oShape.objects("myCanvas");
	var oTxtUp = oShape.objects("TxtUp");
	var oTxtTop = oShape.objects("TxtTop");
	var oTxtPlus = oShape.objects("TxtPlus");
	var oTxtMinus = oShape.objects("TxtMinus");
	var oTxtZoom = oShape.objects("zoomLevel");
	var oModeObj = oShape.objects("order");
	var oDataTargets = oShape.objects("ConRec_SingleStep");
	var oCancelButton = oShape.objects("BtnCnclSel");
	var oldOrderSize,isNewOrder,isStageShape;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  TAC optimization 08 May 2018
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//Do not execute on first update and if no change has happened
	var orderFirstUpdate = oShape.getAttribute("orderFirstUpdate");
	var attrUpd = oShape.getAttribute("Go_update");
	if(typeof(orderFirstUpdate)=="undefined" || orderFirstUpdate==null){	
		oShape.setAttribute("orderFirstUpdate",true);
		//Set SeqShp IDs to page attributes for use with delayed initialization
			if(attrIdx < 4) {
				attrIdx++;
				Page.setAttribute("SeqShpID" + attrIdx , oShape.id);
if(myDebugMode) LogMsg("ebmgsh LaunchDefer  onupdate -> attrIdx = " + attrIdx + ": set SeqShpID("+ attrIdx + ") = " + Page.getAttribute("SeqShpID" + attrIdx) + " : Go = " + attrUpd);
			}
		//Attach TARGETSTEP[] datasource onupdate
		oDataTargets.onupdate = function(){ updateTargetStepList(oShape); };
		//---
		return; //OrderFirstUpdate

	}

	//if (attrUpd == null){	return; }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//if (!oCanvas.style.zoom){

		//myZoomLevel = parseFloat(oShape.GetCustomProperty("Value","zoomLevel"));
		//if (typeof (myZoomLevel) == "undefined") myZoomLevel = 1;
		//oCanvas.style.zoom = myZoomLevel;
	 
		oShape.Objects("zoomLevel").Value = (oCanvas.style.zoom * 100).toFixed(0) + "%";
	//}
	//try {
		var myPointName;
		var tmpPnt = oShape.GetCustomProperty("Point","pointName");
		var sMainHandler = oShape.GetCustomProperty("Text","cp_mainHandler");
		var sUseHandlers = findCSSValue(myCSS, ".ebm_Display_Handlers","key");
		//
		if(tmpPnt.indexOf("{%")>-1){
			myPointName=DisplayDataRepository.GetValue(tmpPnt.split("%")[1]);
		}else{
			myPointName = tmpPnt;
		}

		if(myPointName!="" && myPointName.indexOf("NaN")==-1){
			myPointName2 = oSource.DataValue("pointName.NAME");

			if (typeof myPointName2 == "undefined" || myPointName2.toString()=="NaN"){
				//if(myDebug){debugWrite("ebm_Order_OnupdateX : ", "Invalid point name : " + myPointName.toString());}
				return;
			}
			myOrder = oSource.DataValue("pointname.ORDER");			
			myHandler= oSource.DataValue("pointname.ACTIVELOC.HANDLER");

//===========================================================================		
			//Check if order has changed
			myOrderChanged = oSource.DataChanged("pointname.ORDER");
if(myDebugMode) LogMsg("ebm_order_onupdate -> src = " + oShape.id + " : myOrderChanged = " + myOrderChanged + " : oldOrderSize = " + oldOrderSize + " : isNewOrder = " + isNewOrder);
			try{
				oldOrderSize = oShape.getAttribute("oldOrderSize");
			}catch(e){
				oldOrderSize = false;
			}
			if(!oldOrderSize){
				if(myOrderChanged && typeof(myOrder)=="string"){

					oShape.setAttribute("oldOrderSize",myOrder.length);
					isNewOrder=true;
				}
			}else{
				if(myOrderChanged && typeof(myOrder)=="string" && myOrder.length!=oldOrderSize){
					isNewOrder=true;
				}else{
					isNewOrder=false;
				}
			}
if(myDebugMode) LogMsg("orderUpdate = " + typeof myHandler + " : value = " + myHandler);			
if(myDebugMode) LogMsg("ebm_order_onupdate -> src = " + oShape.id + " : myOrderChanged = " + myOrderChanged + " : oldOrderSize = " + oldOrderSize + " : isNewOrder = " + isNewOrder);

			//if(!isNewOrder) return;
//===========================================================================				
			//myHandler = myPointName+".MAIN";				// Only Main Handler
			//debugWrite("Check for MODE");
/* 			if (oModeObj.DataExists("pointname.MODE")){
				var myMode = oModeObj.DataValue("pointname.MODE");
			}else{
				var myMode=1;
			}

			if (myMode == 3){ //SINGLESTEP
				
				CnclBtn.style.visibility = "visible";
			}else{
				
				CnclBtn.style.visibility = "hidden";
			} */

			//"---------------
			myState= oSource.DataValue("pointname.STATE");
			// Additional State Object presentation (Canvas frame used for Selection)
			if(oShape.objects("recState")){
				oState=oShape.objects("recState");
				if(isNaN(myState)){
					oState.styleClass = "ebmCanvas_State0";
				}else{
					oState.styleClass = "ebmCanvas_State"+myState;
				}
				//place State indication rectangle around canvas
				var pad = oState.lineWidth;
				oState.style.pixelTop=oCanvas.style.pixelTop-pad;
				oState.style.pixelLeft=oCanvas.style.pixelLeft-pad;
				oState.style.pixelWidth=oCanvas.style.pixelWidth+2*pad;
				oState.style.pixelHeight=oCanvas.style.pixelHeight+2*pad;
			}else{
				oState = oCanvas;
				if(isNaN(myState)){
					oState.className = "ebmCanvas_State0";
				}else{
					oState.className = "ebmCanvas_State"+myState;
				}
			}
			var myOwnerID = "";
			if (oSource.DataExists("pointName.OWNERID[1]")){
				myOwnerID = oSource.DataValue("pointName.OWNERID[1]");
			}
			if(myOwnerID != "" ){
				myOwnerID +="\r\n";
			}
			if(isNaN(myState)){
				oCanvas.title = myPointName2 + "\r\n"+myOwnerID +"State = Unknown";
			}else{
				oCanvas.title = myPointName2 +"\r\n"+myOwnerID +"State = " + arrState[myState];
			}
			//----------------
			if(myHandler!="" && myHandler!=null){
				if(sUseHandlers.toLowerCase()=="yes"){
					myHandler=myHandler.split(".")[1];
				}else{
					myHandler = sMainHandler;
				}
			}else{
				myHandler = sMainHandler;
			}


		//Detect shape with stages support
		var tmpCP = oShape.GetCustomProperty("Value","StageShapeName1");
		if (typeof(tmpCP)!=="undefined"){
			isStageShape = true;
		}else{
			isStageShape = false;
		}
		if(isStageShape){
			if(!showStages[oCanvas.id]){
				showStages[oCanvas.id] = [];
				for(var i=1; i<=maxStages; i++){
					showStages[oCanvas.id][i] = true;
				}
			}
			stageOrder = myOrder;
			startNode = myHandler;
			for(var j=1;j<=maxStages;j++){
				if(showStages[oCanvas.id][j]) {
					try{
						var stageShapeName = oShape.GetCustomProperty("Value","stageShapeName"+j);
						var stageShapeStart = oShape.GetCustomProperty("Value","stageShapeStart"+j);
						var stageShapeEnd = oShape.GetCustomProperty("Value","stageShapeEnd"+j);
						startNode = stageShapeStart.match(/invokemain/i) ? stageShapeName : startNode;
						if(stageShapeName != ""){
							stageOrder = updateStageOrder(stageOrder, stageShapeName, stageShapeStart, stageShapeEnd);
						}
					}catch(e){
						break;
					}
				}
			}
			myOrder=stageOrder;
		}
		if( isNewOrder ){ //oSource.DataChanged("pointName.ORDER")
			myZoomLevel = parseFloat(oShape.GetCustomProperty("Value","zoomLevel"));
			if (typeof (myZoomLevel) == "undefined") myZoomLevel = 1;
			oCanvas.style.zoom = myZoomLevel;
			//------------------------------------------------------------------
			// Added initializations needed when switching points via Display CP/Activity Table (R510+)
			//------------------------------------------------------------------
			oCanvas.startBlock = "";
			myRootPoint[oCanvas.id] = "";
			//oCanvas.removeAttribute("pointName");
			oShape.removeAttribute("getBlockDataOnce");
			oShape.setAttribute("oldOrderSize",0);
			oCanvas.setAttribute("initGetDataComplete",null);
			//--------------------------------------------------------
			//Draw the R/SCM structure
			drawSequence(myOrder, oCanvas, myPointName2,myHandler);
if(myDebugMode) LogMsg("drawSeq LaunchDefer -> container  = " + oCanvas.id + " : len = " + oCanvas.children.length);	

			
			var btWidth = 40;
			var btHeight = 40;
			oTxtUp.style.visibility = "hidden";
			oTxtTop.style.visibility = "hidden";

			if(myPointName2.toLowerCase() != myRootPoint[oCanvas.id].toLowerCase()){
				window.setTimeout(function(){
					//oTxtTop.style.pixelWidth = btWidth;
					//oTxtTop.style.pixelHeight = btHeight;
					//oTxtTop.style.pixelTop = oCanvas.style.pixelTop+2;	
					oTxtTop.style.pixelLeft = oCanvas.style.pixelLeft+oCanvas.clientWidth*myZoomLevel-btWidth-Math.floor(2/myZoomLevel);////-16
					oTxtTop.style.visibility = "inherit";

					//oTxtUp.style.pixelWidth = btWidth;
					//oTxtUp.style.pixelHeight = btHeight;
					//oTxtUp.style.pixelTop = oCanvas.style.pixelTop+2;
					oTxtUp.style.pixelLeft = oCanvas.style.pixelLeft+oCanvas.clientWidth*myZoomLevel-2*btWidth-Math.floor(2/myZoomLevel);
					if(oSource.DataValue("pointname.OWNERID[1]")){
						oTxtUp.style.visibility = "inherit";
					}
				},200); 
			}
		
			window.setTimeout(function(){
				ebm_SeqShape_Toolbar(oShape,oCanvas.style.zoom);
			},250);			
		
			}
//		showStages = true;


			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//====================================================================================================
			//	
			//	GET SHAPES DATA - Loop through all blocks and decide to link data or not based on InView
			//
			//	call "getBlockData" function for each blockName
			//  Arguments:
			//	 - block ID (contains block name, block type and block index)
			//	 - Point Name
			//====================================================================================================		

			//Make sure to get blocks date once after Go_update is set
			var getBlockDataOnce = oShape.getAttribute("getBlockDataOnce");
//if(myDebugMode)	LogMsg("orderUpdate GetData Loop -> container  = " + oCanvas.id + " : len = " + oCanvas.children.length + ": getOnce = " + getBlockDataOnce);
			if((typeof(getBlockDataOnce)=="undefined" || getBlockDataOnce==null)) {//&& (attrUpd != null)){	
				oShape.setAttribute("getBlockDataOnce",true);
				//---
if(myDebugMode) LogMsg("orderUpdate GetBlockDataOnce -> container  = " + oCanvas.id + ": pntName = " + myPointName2);
				oCanvas.setAttribute("pointName",myPointName2);
				//Initial data subscribe for visible blocks only
				window.setTimeout(function(){
											updateBlocksData(oCanvas,myPointName2);
											},200,"JScript");

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			

			}
		
		}else{
			oTxtUp.style.visibility = "hidden";
		    oTxtTop.style.visibility = "hidden";
			oTxtPlus.style.visibility = "hidden";
			oTxtMinus.style.visibility = "hidden";
			oTxtZoom.style.visibility = "hidden";
			//
			myOrder = "";
			myPointName2 = "";
			myHandler = "";
			//Clear contents when empty point
			oCanvas.innerHTML = "";

			myState = oSource.DataValue("pointName.STATE");

			if(oShape.objects("recState")){
				oState = oShape.objects("recState");
			}else{
				oState = oCanvas;
			}
			if(isNaN(myState)){
				oState.styleClass = "ebmCanvas_State5";
			}
			oCanvas.title = "";
			oCanvas.startBlock = "";
			oShape.setAttribute("oldOrderSize",0);

			//Clear getBlockDataOnce and Go_update attributes to allow get data at next redraw
			oShape.setAttribute("getBlockDataOnce",null);
			oShape.setAttribute("Go_update",null);
			oShape.setAttribute("orderFirstUpdate",null);
			oShape.setAttribute("lastPointName",null);
			oShape.setAttribute("layoutIndex",0);
			oCanvas.setAttribute("initGetDataComplete",null);
			
			/********************************************************/
			if(myDebugMode) LogMsg("orderUpdate -> CLEAR ALL");		
			if(myDebugMode) {
				var oMsg = document.getElementById(oCanvas.id+"_tmpDebug");
				if(oMsg){
					oMsg.innerText = "Clear sequence data...";
					oMsg.setAttribute("total",0);
					oMsg.setAttribute("vis",0);
					oMsg.setAttribute("subs",0);
					oMsg.setAttribute("unsub",0);
				}
			}
			/********************************************************/
			//unLinkAllDataShapes(oCanvas);
		}
		

//	} catch(e){
//		if (window.external.Application.environment == 0) {window.external.Application.LogMessage(window.external.Application.currentpage + " : >> ebm_Order_Onupdate() -> " + oSource.id + " "  + e.description );}
//	}
}
//
// ebm_FlowShape_NavTxt_OnClick - handle drill down/up navigation from buttons at the top
//
function ebm_FlowShape_NavTxt_OnClick(oSource){
	var newPointName;
	var oShape = oSource.parentElement.parentElement;
	var oCanvas = oShape.objects("myCanvas");
	var oOrder = oShape.objects("order");
	try{ // ebm_FlowShape_NavTxt_OnClick
		oCanvas.click();
		if(oSource.id.indexOf("Top")>0){
			newPointName = myRootPoint[oCanvas.id];
		}
		if(oSource.id.indexOf("Up")>0){
			newPointName = oOrder.DataValue("pointName.OWNERID[1]");
		}
		if(newPointName.length > 0){
			changeShapePointName(oShape, newPointName);
		}
	} catch(e){
		window.external.Application.LogMessage(window.external.Application.currentpage + ">> ebm_FlowShape_NavTxt_OnClick() -> " + oSource.id + " "  + e.description );
	}
}
//
// ebm_FlowShape_ZoomPlus_OnClick - handle canvas zoom from +/- buttons at the top
//
function ebm_FlowShape_Zoom_OnClick(oSource){
	var oShape =  oSource.parentElement.parentElement;
	var oCanvas =  oShape.objects("myCanvas");
	var oOrder = oShape.objects("order");
	var zoom;
	try{
		oSource.disabled = false;
		if(oSource.id.indexOf("Plus")>0){
			if(zoom>1.95) return;
			zoom = (parseFloat(oCanvas.style.zoom)+0.1).toFixed(1);//oShape.GetCustomProperty("Value","zoomLevel");

		}
		if(oSource.id.indexOf("Minus")>0){
			if(zoom<0.35) return;
			zoom = (parseFloat(oCanvas.style.zoom)-0.1).toFixed(1);//oShape.GetCustomProperty("Value","zoomLevel");
		}
		//Set the zoom level to canvas
		oCanvas.style.zoom = zoom;
		//Fire "onscroll" event to trigger data re-subscription for visible blocks
		oCanvas.fireEvent("onscroll");
		//update zoom value in the toolbar
		oShape.Objects("zoomLevel").Value = (oCanvas.style.zoom * 100).toFixed(0) + "%";
		//update buttons position after zoom change
		ebm_SeqShape_Toolbar(oShape,zoom);
		oCanvas.click();
	} catch(e){
		window.external.Application.LogMessage(">> ebm_FlowShape_Zoom_OnClick() -> " + oSource.id + " "  + e.description );
	}
}
//
// ebm_SeqShape_Zoom_DblClick - sink doubleclick on toolbar buttons
//
function ebm_SeqShape_Zoom_DblClick(){
	window.event.cancelBubble = true;
	window.event.returnValue = false;
	window.external.Application.LogMessage("Test DblClick");
}
//
// ebm_SeqShape_Toolbar
//
function ebm_SeqShape_Toolbar(oShape,myZoomLevel){
	var oTxtPlus = oShape.objects("TxtPlus");
	var oTxtMinus = oShape.objects("TxtMinus");
	var oTxtZoom = oShape.objects("zoomLevel");
	var oCancelButton = oShape.objects("BtnCnclSel");
	var oTxtUp = oShape.objects("TxtUp");
	var oTxtTop = oShape.objects("TxtTop");
	var oCanvas = oShape.objects("myCanvas");
	var btWidth = 40;
	var btHeight = 40;
	var btShift =0;

	if (oTxtUp.style.visibility != "hidden") {
			btShift+=btWidth;
	}
	if (oTxtTop.style.visibility != "hidden") {
			btShift+=btWidth;
	}
	if(oTxtPlus){
		oTxtPlus.style.pixelLeft = oCanvas.style.pixelLeft+oCanvas.clientWidth*myZoomLevel-btWidth-4-btShift;
		oTxtPlus.style.visibility = "inherit";
		if(myZoomLevel>1.95) {
			oTxtPlus.disabled = true;
		}else{
			oTxtPlus.disabled = false;
		}
	}
	if(oTxtZoom){
		oTxtZoom.style.pixelLeft = oCanvas.style.pixelLeft+oCanvas.clientWidth*myZoomLevel-7*btWidth/3-4-btShift;
		oTxtZoom.style.visibility = "inherit";
	}
	if(oTxtMinus){
		oTxtMinus.style.pixelLeft = oCanvas.style.pixelLeft+oCanvas.clientWidth*myZoomLevel-10*btWidth/3-4-btShift;
		oTxtMinus.style.visibility = "inherit";
		if(myZoomLevel<0.35) {
			oTxtMinus.disabled = true;
		}else{
			oTxtMinus.disabled = false;
		}
	}
	if(oCancelButton){
		//if(oCancelButton.style.visibility != "hidden"){
			//oCancelButton.style.visibility = "hidden";
			oCancelButton.style.pixelWidth =  oCanvas.style.pixelLeft+oCanvas.clientWidth*myZoomLevel- oTxtMinus.style.pixelLeft-4;// btWidth*6;
			//oCancelButton.style.pixelHeight = btHeight;
			//oCancelButton.style.pixelTop = oCanvas.style.pixelTop+oTxtPlus.style.pixelHeight*2+4;
			oCancelButton.style.pixelLeft = oTxtMinus.style.pixelLeft; //oCanvas.style.pixelLeft+oCanvas.clientWidth*myZoomLevel-12*btWidth-Math.floor(4/myZoomLevel);
			//oCancelButton.style.visibility = "inherit";
		//}
	}
}
//
// ebm_AlertPicker_onclick
//
function ebm_AlertPicker_onclick(oSource){
	var i, j, myHash, popupCommand, ptnName, truePtnName;
	myHash = 0;
	var oShape=oSource.parentElement;

	try{ //ebm_AlertPicker_onclick
		popupCommand = oShape.GetCustomProperty("value", "seqPickerPopup") + "?CurrentPoint=dummy";
		for(i = 1;i< 31;i++){
			ptnName = oShape.GetCustomProperty("point","pt"+i);
			if(ptnName.length > 0){
				truePtnName = oSource.DataValue("pt"+i+".NAME");
				popupCommand = popupCommand+"&pt"+i+"="+truePtnName;
				for(j = 0;j<truePtnName.length;j++){
					myHash = myHash + parseInt(truePtnName.toUpperCase().charCodeAt(j));
				}
			}
		}

		popupCommand += "&AlertConfirmDisplay="+oShape.GetCustomProperty("value","AlertConfirmDisplay");
		popupCommand += "&cHash="+ myHash;
		HDXPopupBehavior.CreatePopupWindow2(popupCommand, 0, 0, 0, true);
	}catch(e){
		if (window.external.Application.environment == 0) {window.external.Application.LogMessage(window.external.Application.currentpage + " : >> ebm_AlertPicker_onclick() -> " + oSource.id + " "  + e.description );}
	}
}
//
// ebm_AlertPicker_ondatachange
//
function ebm_AlertPicker_ondatachange(oSource){
	var i, tmpPointName, tmpStr, tmpHide;
	var oShape=oSource.parentElement;

	tmpHide = "Normal";
	try{ //ebm_AlertPicker_ondatachange
		for(i = 1;i< 31;i++){
			tmpPointName = oShape.GetCustomProperty("Point", "pt" + i.toString() );
			if(tmpPointName.length > 0){
				tmpStr = "pt"+ i.toString() +".INSTRUCTPEND";
				if(oSource.DataValue(tmpStr)==1){
					tmpHide = "Alert";
					break;
				}
			}
		}
		oSource.styleClass = "ScTxtAlert_" + oShape.GetCustomProperty("Text","cp_StyleFor"+tmpHide);
	} catch(e){
		if(window.external.Application.environment == 0){window.external.Application.LogMessage(window.external.Application.currentpage + " : >> ebm_AlertPicker_ondatachange() -> " + oSource.id + " "  + e.description );}
	}
}
//
// ebm_UnitBatchId_ondatachange
//
function ebm_UnitBatchId_ondatachange(oSource){
	var myOwnerID, recName, sDspCP, tmpCP;
	var oShape=oSource.parentElement;

	try{ // ebm_UnitBatchId_ondatachange
		//---
		//check shape version
		var myEbmShape = oShape.GetCustomProperty("text","ebmShapeName");
		var myDispCP = oShape.GetCustomProperty("text","DspCPName");
		//Get configured point name
		var tmpName = oShape.GetCustomProperty("point","recipeName");
		if(tmpName.indexOf("%") > 0){ //CP configured with Display CP
			recName = DisplayDataRepository.GetValue(tmpName.split("%")[1]);
		}else{
			recName = tmpName;
		}
		myOwnerID = oSource.DataValue("UnitName.OwnerID[1]");
		//Get Display CP name if available in shape config
		if ( typeof(myEbmShape) == "undefined" ){
			tmpCP = oShape.GetCustomProperty("Text","DspCPName");
			if(tmpCP.indexOf("%") > 0){
				sDspCP = tmpCP.split("%")[1];
			}else{
				sDspCP = tmpCP;
			}
		}
		if(myOwnerID.length > 0){
			if (myDispCP == ""){
				oShape.SetCustomProperty("point","recipeName",myOwnerID);
			}else{
				//Set the OwnerID to the Display CP for the Activity
				DisplayDataRepository.PutValue(sDspCP,"");
				for(var shpID in myRootPoint){
					if(myRootPoint[shpID] != myOwnerID) {
						myRootPoint[shpID] = "";
					}
				}
				window.setTimeout(function(){DisplayDataRepository.PutValue(sDspCP,myOwnerID);},500);
				//DisplayDataRepository.PutValue(sDspCP,myOwnerID);
				oShape.style.visibility = "inherit";
			}
		} else {
			oShape.style.visibility = "hidden";
			DisplayDataRepository.PutValue(sDspCP,"");
		}
		//---
		if(recName != ""){
			if(myEbmShape!=""&&document.getElementById(myEbmShape)!=null){
				var oEbmShape = document.getElementById(myEbmShape);
				tmpPnt = oEbmShape.getCustomProperty("point","pointName");
				if(tmpCP.indexOf("%") == -1){
					oEbmShape.autoreloadcontent = false;
					oEbmShape.SetCustomProperty("point","pointName",myOwnerID);
					oEbmShape.ReloadContent();
				}
			}
		}
	}catch(e){
		if (window.external.Application.environment == 0) {window.external.Application.LogMessage(window.external.Application.currentpage + " : >> ebm_UnitBatchId_ondatachange() -> " + oSource.id + " "  + e.description );}
	}
}
//
// PhaseParameter - Handles Parameter Popup out of Contextmenu for Phases and Canvas
//
function PhaseParameter(select){
	var SCMName;
	var sElement = Page.getAttribute("lastselection");

	if(sElement.search(/:/) == -1){
		oShape = document.getElementById(sElement).parentElement.parentElement;
		var tmpPnt=oShape.GetCustomProperty("Point","pointName");
		//FIX 2018-02-06 - Check if shape point name configured from Display CP
		if(tmpPnt.indexOf("%") > 0){ //CP configured with Display CP
			SCMName = DisplayDataRepository.GetValue(tmpPnt.split("%")[1]);
		}else{
			SCMName = tmpPnt;
		}
	}else{

		var tmpArray = sElement.split(":");

		//objectID = tmpArray[3];
		//myObject = document.getElementById(objectID);
		myObject = document.getElementById(sElement);

		//pointName = myObject.parentElement.parentElement.GetCustomProperty("point", "pointName");
		myOrderObject = myObject.parentElement.parentElement.parentElement.Objects("order");
		myPointName = myOrderObject.DataValue("pointName.NAME");
		phaseName = tmpArray[1];

		SCMName = myPointName+"."+phaseName;

		if (select == "rep"){
			window.setTimeout(function(){
				//window.external.Application.invokepopup("RPhaseParam_Flow.htm?MapBlock="+myPointName+"&MapParam="+phaseName,1,1,0);
				HDXPopupBehavior.CreatePopupWindow("RPhaseParam_Flow.htm?MapBlock="+myPointName+"&MapParam="+phaseName,0,0,0);
			}, 1, "JScript");
			return;
		}else if (select == "form"){
			window.setTimeout(function(){
				//window.external.Application.invokepopup("PhaseParam_Flow.htm?MapBlock="+myPointName+"&MapParam="+phaseName,1,1,0);
				HDXPopupBehavior.CreatePopupWindow("PhaseParam_Flow.htm?MapBlock="+myPointName+"&MapParam="+phaseName,0,0,0);
			}, 1, "JScript");
			return;
		}
	}

	if (select == "rep"){
			window.setTimeout(function(){
					//window.external.Application.invokepopup("RParam_Flow.htm?currentpoint="+SCMName,0,0,0);
					HDXPopupBehavior.CreatePopupWindow("RParam_Flow.htm?currentpoint="+SCMName,0,0,0);
			}, 1, "JScript");
	}else if(select == "form"){
			window.setTimeout(function(){
					//window.external.Application.invokepopup("Param_Flow.htm?currentpoint="+SCMName,0,0,0);
					HDXPopupBehavior.CreatePopupWindow("Param_Flow.htm?currentpoint="+SCMName,0,0,0);
			}, 1, "JScript");

	}
}
//
//Handles if Option in Contextmenu is available
//
function pageloadParam(osource){

	var sElement, strElm, can;
	sElement = Page.getAttribute("lastselection");
	strElm = sElement.toString();
	if ( strElm.search(/:P:/)>0 || strElm.search(/myCanvas/)==strElm.length-8){
		osource.disabled=false;
	}else{
		osource.disabled=true;
	}
}
//
// SingleStep_handler - handles interaction in SINGLESTEP mode
//
function SingleStep_handler(sElement){
	var tmpArray, myType, stepName, objectId, pointName, oldObject, len, n;
	var oShape = sElement.parentElement.parentElement.parentElement;
	var oButton = oShape.objects("BtnCnclSel");
	var iTargetsLength = 0;
	
	//Get TARGETSTEPs count from shape attribute
	iTargetsLength = oShape.getAttribute("TargetStepsCount");

	if (iTargetsLength>9){
		window.alert("You have reached the maximum amount of TARGETSTPEPS!");
		return;
	}

	//if(sElement.id.search(/:/) ==  -1 || addTarget == 0)  return;
	tmpArray = sElement.id.split(":");
	stepName = tmpArray[1];
	myType = tmpArray[2];
	objectId = tmpArray[3];

//if(myDebugMode) LogMsg("SingleStep_handler : dataobj = " + tmpArray[0] + "_dataBlock" + objectId);		
	var myDataShape = oShape.objects("dataBlock" + objectId);
	
//if(myDebugMode) LogMsg("SingleStep_handler : elem = " + myDataShape.parentElement.id);
//if(myDebugMode) LogMsg("SingleStep_handler : shp = " + oShape.id + ": elem = " + sElement.id);
	var myOrderObject = oShape.Objects("order");
	var mySinglStepObject = oShape.Objects("ConRec_SingleStep");
	var myPointName = myOrderObject.DataValue("pointName.NAME");

	//Newly selected step 
	newStep = myPointName+"."+stepName;
	//Write to TARGETSTEPS[]
	var check = mySinglStepObject.SetDataValueSynchronous("pointName.TARGETSTEP["+(iTargetsLength+1)+"]", newStep);

	//Check write datavalue successful
	if (check){
		//Initialize reload
		var myDataObject = myDataShape.Objects("data");
		//force update object color
		window.setTimeout(function(){updateFlow(myDataObject);},50);
	}
}
//
//	OnMode_Change - mode update handler (SINGLESTEP functionality)
//
function OnMode_Change(oSource){

	var oShape = oSource.parentElement.parentElement;
	var oCancelButton = oShape.Objects("BtnCnclSel");
	var oCanvas = oShape.Objects("myCanvas");
	var PointName = oShape.GetCustomProperty("point", "pointName");
	var myMode = oSource.DataValue("pointName.MODE");
	var myModeChanged = oSource.DataChanged("pointName.MODE");
	var myExSt = oSource.DataValue("pointName.EXECSTS");
	var myExStChanged = oSource.DataChanged("pointName.EXECSTS");
	
	//SINGLESTEP
	if (myMode == 3){ 
		//clear target steps to remove the active from list
		if(myModeChanged && !myExStChanged ){
			window.setTimeout(function(){
								clearTargets(oSource);
							}, 4000, "JScript");
		}
		if (myExSt<6 || myExSt>11){
			window.external.Application.TemporaryMessageZoneText("NOTICE: RESUME will be rejected, if EXECSTS is not xxPaused!");
		}
		//Get TARGETSTEPs count from shape attribute
		var iTargetsLength = oShape.getAttribute("TargetStepsCount");
		if (iTargetsLength>0){
			oCancelButton.style.visibility = "inherit";
		}else{
			oCancelButton.style.visibility = "hidden";
		}
	}else{
		oCancelButton.style.visibility = "hidden";
	}
	//
	// Refresh blocks status after
	try {
		if((myModeChanged && !isNaN(myMode)) || (myExStChanged && !isNaN(myExSt)) ){

			var initGetDataComplete = oCanvas.getAttribute("initGetDataComplete");
if(myDebugMode) LogMsg("OnMode -> mode changed = " + myModeChanged + " : src = " + oShape.id + " : mode = " + myMode + " : initData =  " + (initGetDataComplete!=null)); // " : initData =  " + ((initGetDataComplete)?initGetDataComplete:"null")			
			if(initGetDataComplete){
if(myDebugMode) LogMsg("OnMode ->  force onscroll");				
				window.setTimeout( function(){oCanvas.fireEvent("onscroll");} , 4100);
			}
		}
	} catch(e) {
			if(myDebug){window.external.Application.LogMessage(">> OnMode_Change() -> " + "Error in UpdateFlow after Mode-change : " +  e.description );}
	}
}
//
//Clears TARGETSTEP-List when the button is clicked
//
function CancelStepSelection(oSource){
	
	//if(window.external.Application.Parent.RuntimeStatus.SecurityLevel>2){
		var oShape = oSource.parentElement.parentElement;
		var	mySinglStepObject = oShape.objects("ConRec_SingleStep");
		var oCanvas =  oShape.objects("myCanvas");
		//Clear TARGETSTEPS[]
		clearTargets(oSource);
		//Debug subscriptions information
		if(myDebugMode){
			var oMsg = document.getElementById(oCanvas.id+"_tmpDebug");
			var iS = oMsg.getAttribute("subs");
			oMsg.setAttribute("subs",0);
			updateDebugMsg(oMsg);
		}
		//Trigger scroll to force update blocks
		oCanvas.fireEvent("onscroll");
		//Hide cancel button
		oSource.style.visibility="hidden";
	//}
}
//
//	clearTargets - clear TARGETSTEP array
//
function clearTargets(oSource){
		var oShape = oSource.parentElement.parentElement;
		var	mySinglStepObject = oShape.objects("ConRec_SingleStep");
		//Clear TARGETSTEPS[]
		for (var i=1; i<11;i++){
			if (mySinglStepObject.DataExists("pointName.TARGETSTEP["+i+"]")){
				if (mySinglStepObject.DataValue("pointName.TARGETSTEP["+i+"]")!=""){
					mySinglStepObject.DataValue("pointName.TARGETSTEP["+i+"]")="";
				}
			}
		}
}
//
// updateTargetStepList - store a (delimited) list & count of TARGETSTEPs to shape attributes whenever it updates
//
function updateTargetStepList (oShape){
	var strTargetSteps = "";
	var iTargetStepsCount = 0;
	var oDataTargets = oShape.objects("ConRec_SingleStep");
	var oCanvas = oShape.objects("myCanvas");
	var oCancelButton = oShape.objects("BtnCnclSel");
	var oModeObj = oShape.objects("mode");
	var myMode;

	if(oDataTargets){
		for(var i=1;i<=10;i++){
			var iTargetStep = oDataTargets.DataValue("pointName.TARGETSTEP["+i+"]");
			strTargetSteps += iTargetStep + "|";
			if(iTargetStep != ""){
				iTargetStepsCount+=1;
			}
		}
		oShape.setAttribute("TargetSteps",strTargetSteps);
		oShape.setAttribute("TargetStepsCount",iTargetStepsCount);
	}
	if(oModeObj){
		if (oModeObj.DataExists("pointname.MODE")){
			myMode = oModeObj.DataValue("pointname.MODE");
		}else{
			myMode=1;
		}
	}
	if(oCancelButton){
		if(iTargetStepsCount>0 && myMode == 3){
			oCancelButton.style.visibility = "inherit";
		}else{
			oCancelButton.style.visibility = "hidden";
		}
	}
}
//
// AbletoHighlightPath - used in context menu "onpage" to enable/disable highlight path if not R/SCM block
//
function AbletoHighlightPath(osource){
	var sElement, strElm;

	sElement = Page.getAttribute("lastselection");
	strElm = sElement.toString();
	if (strElm.search(/myCanvas/)==strElm.length-8){
		osource.disabled=true;
	}else{
	 	osource.disabled=false;
	}
}
//
//	showNext - highlight path to next block (called from context menu)
//
function showNext(){

	var inBlockName, tArray, i, outBlockName, sElement, container, tmpArray;

	sElement = Page.getAttribute("lastselection");

	if(sElement.search(/:/) ==  -1){
		window.alert("Not possible");
	}else{

		tmpArray = sElement.split(":");
		objectID = tmpArray[3];
		blockName = tmpArray[1];
		myObject = document.getElementById(objectID);
		container = document.getElementById(tmpArray[0]);//myObject.parentElement.parentElement.Objects("myCanvas");
		try {
			inBlockName = myEbms[container.id].rBlocks[container.id+blockName];
			tArray = myEbms[container.id].blocks[myEbms[container.id].getIndex(blockName)].split(",");
			for(i=3;i<tArray.length;i++){
				outBlockName = myEbms[container.id].rBlocks[container.id + myEbms[container.id].getName(tArray[i])];
				HighlightPath(container, inBlockName, outBlockName);
			}
		}catch(e){
			if(window.external.Application.environment == 0){window.external.Application.LogMessage(">> showNext() -> " + e.description );}
		}
	}
}
//
//	showPrevious - highlight path to previous block (called from context menu)
//
function showPrevious(){

	var inBlockName, tArray, i, outBlockName, sElement, container, tmpArray, myIndex;

	sElement = Page.getAttribute("lastselection");

	if(sElement.search(/:/) ==  -1){
		window.alert("Not possible");
	}else{

		tmpArray = sElement.split(":");
		objectID = "dataBlock" + tmpArray[3];
		blockName = tmpArray[1];
		myObject = document.getElementById(objectID);
		container = document.getElementById(tmpArray[0]); //myObject.parentElement.parentElement.Objects("myCanvas");
		try {
			outBlockName = myEbms[container.id].rBlocks[container.id+blockName];
			myIndex = myEbms[container.id].getIndex(blockName);
			for (var prev in myEbms[container.id].blocks){
				tArray = myEbms[container.id].blocks[prev].split(",");
				for(i=3;i<tArray.length;i++){
					if (myIndex == tArray[i]){
						inBlockName = myEbms[container.id].rBlocks[container.id + myEbms[container.id].getName(prev)];
						HighlightPath(container, inBlockName, outBlockName);
						i = tArray.length;
					}
				}
			}
		}catch(e){
			if(window.external.Application.environment == 0){window.external.Application.LogMessage(">> showPrevious() -> " + e.description );}
		}
	}
}
//
// Highlight path - called from context menu - used to temporarily connection line to previous/next block
//
function HighlightPath(container, fromObject, toObject){
	var startX, startY, endX, endY, x0, x1, x2 ,xe, y0, y1, y2, ye, myTop, myLeft, myPath, w, h, o, a;
	var x = [], y = [], numVertices, cornerSize, icornerSize;
	var myStrokeColor, myStrokeWeight;
	startX = parseInt(fromObject.coordSize.x)/2 + parseInt(fromObject.style.left);
	startY = parseInt(fromObject.coordSize.y) + parseInt(fromObject.style.top);
	endX = parseInt(toObject.coordSize.x)/2 + parseInt(toObject.style.left);
	endY = parseInt(toObject.style.top);

	if(startY<endY){
		myTop = startY;
		myLeft = startX;
		//
		// Start block is to the right of the End block
		//
		if(startX > endX){
			myLeft = endX;
		}
		w = Math.abs(endX - startX);
		h = Math.abs(endY - startY);
	} else {
	//
	// Case for end block positioned higher than start block 
	//		
		w=Math.abs(endX-startX) +parseInt(toObject.coordSize.x) + 5 - parseInt(fromObject.coordSize.x)/2;
		h=Math.abs(endY-startY) + ebmShapeTopMargin;
		myTop = parseInt(toObject.style.top) -5;
		myLeft = parseInt(toObject.style.left) + parseInt(toObject.coordSize.x)/2;
	}
	o = document.createElement("v:shape");
	o = container.appendChild(o);
	o.className = "connection";
	myStrokeWeight = "4px";//findCSSValue(myCSS, ".connection","strokeweight");
	switch(nextP){
		case 1:
			myStrokeColor = "#FFFF57";
			a = nextP;
			nextP = 2;
			break;
		case 2:
			myStrokeColor = "#FF6E28";
			a = nextP;
			nextP = 3;
			break;
		case 3:
			myStrokeColor = "#FF00FF";
			a = nextP;
			nextP = 4;
			break;
		case 4:
			myStrokeColor = "#FFAAFF";
			a = nextP;
			nextP = 1;
			break;
		default:
			break;
	}
	o.strokeweight = myStrokeWeight?myStrokeWeight:"4px";
	o.strokecolor = myStrokeColor?myStrokeColor:"#FFFF57";
	o.fillcolor = "none";
	o.style.left = myLeft+"px";
	o.style.top = myTop + "px";
	o.coordOrigin ="0 0";
	o.coordSize= (w+3)+" "+h;
	o.style.width = (w+3) + "px";
	o.style.height = h + "px";
	o.style.position = "absolute";
	//o.path = myPath;
//-------------------------------------------------------------
	var oLine = document.getElementById(fromObject.id.split(":")[1] + "|" + toObject.id.split(":")[1]);
	if(oLine){
		o.path = oLine.path;
	}
//-------------------------------------------------------------	
	window.setTimeout(function(){
						container.removeChild(o);
						if (nextP > a){nextP = a;}
					},5000,"JScript");
}
// addEventHandler (http://www.javascripter.net/faq/addeventlistenerattachevent.htm)
function addEventHandler(elem,eventType,handler) {
 if (elem.addEventListener)
	 elem.addEventListener (eventType,handler,false);
 else if (elem.attachEvent)
	 elem.attachEvent ("on"+eventType,handler); 
}
// removeEventHandler http://www.javascripter.net/faq/removeeventlistenerdetachevent.htm
function removeEventHandler(elem,eventType,handler) {
 if (elem.removeEventListener) 
	elem.removeEventListener (eventType,handler,false);
 if (elem.detachEvent)
	elem.detachEvent ("on"+eventType,handler); 
}
//
//	isInViewport - check if a block is in shape visible area
//
var isInViewport = function (elem,container) {

	try{
		var zoom = parseFloat(container.style.zoom);
		//container coordinates
		var cTop = container.parentElement.parentElement.style.pixelTop;
		var cLeft = container.parentElement.parentElement.style.pixelLeft;
		var cBottom = cTop+container.clientHeight*zoom;
		var cRight = cLeft+container.clientWidth*zoom;
		//element coordinates
		var bounding = elem.getBoundingClientRect();
		var elTop = bounding.top;
		var elLeft = bounding.left;
		var elBottom = bounding.bottom;
		var elRight = bounding.right;    //Math.floor(bounding.right/zoom);
		var elHeight = elBottom-elTop;
		var elWidth = elRight-elLeft;
		//
		var elOffsetH = Math.floor(elHeight/2);
		var elOffsetW = Math.floor((elWidth)*0.75);

	//if(myDebugMode) LogMsg("isInViewport -> id = " + elem.id + ": zoom = " + zoom + " : t= " + elTop +" : b= " + elBottom +" : l= " + elLeft + " : r= " +  elRight + " : containerH = " + cTop+ " : containerW = " + cLeft);	
	//if(myDebugMode) LogMsg("isInViewport -> id = " + elem.id + ": zoom = " + zoom + " : l= " + elLeft +" : r= " + elRight + " : cRight = " + cRight + " : cHeight = " + container.clientWidth*zoom);	
	//if(myDebugMode) LogMsg("isInViewport -> id = "+ elem.id + ": zoom = " + zoom + " : (b-h/2)= " + (elBottom-elOffsetH) + ": cBottom = " + cBottom );	

		return (
				( (elTop + elOffsetH - cTop) >= 0 ) &&
				( (elLeft + elOffsetW - cLeft) >= 0 ) &&
				( (elBottom - elOffsetH - cBottom) <= 0 ) &&
				( (elRight - elOffsetW - cRight) <= 0 )
		);
	}catch(e){
if(myDebugMode) 		LogMsg("isInViewport -> id = "+ elem.id + ": err = " + e.desc );
	}
};
//
// SeqShpLaunchDefer - defer data subscriptions for multiple shape instances in one display (up to 3)
//
//	each shape instace would have a delay defined in CSS
//
function SeqShpLaunchDefer(){
    var iDelay;
if(myDebugMode) LogMsg("ebmgsh -> SeqShpLaunchDefer -> PAGECOMPLETE");
	if(attrIdx>0){

		for(i=1;i<=attrIdx;i++){
			if(i==1){
				iDelay = myCSS? parseInt(findCSSValue(myCSS, ".ShapeDelays", "shp-delay-"+i)):500;
				if(iDelay == null) iDelay = 500;
			}else if(i==2){
				iDelay = myCSS? parseInt(findCSSValue(myCSS, ".ShapeDelays", "shp-delay-"+i)):2500;
				if(iDelay == null) iDelay = 2500;
			}else if(i==3){
				iDelay = myCSS? parseInt(findCSSValue(myCSS, ".ShapeDelays", "shp-delay-"+i)):4500;
				if(iDelay == null) iDelay = 4500;
			}
if(myDebugMode) LogMsg("ebmgsh -> SeqShpLaunchDefer -> idx = " + i + " : delay = " + iDelay );		
			//Enable updates
			window.setTimeout("goShp("+i+")", iDelay, "JScript");
			//Force updates (if needed) 
			window.setTimeout("updShp("+i+")", iDelay+3000, "JScript");
		}
	}
}
//
// Attach custom pagecomplete handler that executes the deferred Sequence shape initialization/launch.
// 
function ebm_ondocstatechange(){

	if (document.readyState == "complete") {
		//Attach to "onpagecomplete" to execute delayed lauch of multiple shape instances
		var oPage = document.getElementById("Page");
		if(oPage){
			oPage.attachEvent("onpagecomplete", SeqShpLaunchDefer);
			oPage.attachEvent("onoperatorchange", SeqShpLaunchDefer);
		}
	}
}
//
//Execute immediately on parse (wrap in anonymous self invoking function)
//
(function(){
		if(document){
			document.attachEvent("onreadystatechange", ebm_ondocstatechange);
		}else{
if(myDebugMode) LogMsg("ebmgsh -> document not available!");
		}
	}
)();

//
// decodeBlockData - Get DISPLAYSTATE value for a R/SCM block and decode parameters 
//
function decodeBlockData(oDataShape){
	var iDisplayState;
	var pointName = oDataShape.GetCustomProperty("Point","pointName");
	var paramName = oDataShape.GetCustomProperty("Point","param1");
	var blockType = oDataShape.GetCustomProperty("Value","blockType");
	var oSource =  oDataShape.objects("data");
	if (pointName == "" || paramName == "") {
			return;
		}
	if(oSource){
		//oSource.onupdate = function(){return;};
		if(oSource.DataExists("pointName.param1")){
			iDisplayState = oSource.DataValue("pointName.param1");
		}else{
			return {};
		}
		if(isNaN(iDisplayState)){
			iDisplayState = 0;
		}
	}
	//return object with decoded values in properties
	return {
		displaystate: iDisplayState,
		state: maskBits(iDisplayState,8,15),//iState,
		execsts: maskBits(iDisplayState,16,19),//iExecSts,
		configsts: maskBits(iDisplayState,20,23),//iCfgSts,
		inalm: maskBits(iDisplayState,32,32),//iInAlm,
		processed: maskBits(iDisplayState,33,33),//iProcessed,
		completed: maskBits(iDisplayState,34,34),//iCompleted,
		instructpend: maskBits(iDisplayState,35,35),//iInstrPend,
		instructcnfm: maskBits(iDisplayState,36,36),//iInstrCnfm,
		instructcomp: maskBits(iDisplayState,37,37),//iInstrComp,
		so: maskBits(iDisplayState,38,38)//iSO
	};
}
//
//	Mask parameters from the DisplayState 64-bit value
//
function maskBits(intValue,bitStart,bitEnd){
	mask = 0;
	//Shift Higher 32 bits
	if(bitEnd > 31){
			bitStart-=32;
			bitEnd-=32;
			intValue/=Math.pow(2,32);
	}
	for(i=bitStart;i<=bitEnd;i++){
		mask += Math.pow(2,i);
	}
	//
	shift = Math.pow(2,bitStart);
	// Return value represented by masked bits
	return (intValue & mask)/shift;
}
//
//Test debug function - unmask all data from display state and return it in a string
//
function testDecode(oData,blkType){
	var str = "";
	var arr = [];
	if (oData){
		var objDS = decodeBlockData(oData);
		if (blkType == "P"){
			arr = arrStatePhase;
		}else{
			arr = arrStateHST;
		}
		if(objDS){
			if (myDebugMode) str += "DisplayState\t = " + objDS.displaystate + "\r\n";
			if (myDebugMode || blkType == "S") str += "\r\nSTATE\t\t = " + arr[objDS.state] + " ( " + objDS.state + " )";
			if (myDebugMode || blkType == "S") str += "\r\nEXECSTS\t\t = " + arrExecSts[objDS.execsts] + " ( " + objDS.execsts + " )";
			if (myDebugMode) str += "\r\nCONFIGSTS\t = " + arrConfigSts[objDS.configsts] + " ( " + objDS.configsts + " )";
			if (myDebugMode) str += "\r\nINALM\t\t = " + objDS.inalm;
			if (myDebugMode) str += "\r\nPROCESSED\t = " + objDS.processed;
			if (myDebugMode) str += "\r\nCOMPLETED\t = " + objDS.processed;
			if (myDebugMode) str += "\r\nINSTRUCTPEND\t = " + objDS.instructpend;
			if (myDebugMode) str += "\r\nINSTRUCTCNFM\t = " + objDS.instructcnfm;
			if (myDebugMode) str += "\r\nINSTRUCTCOMP\t = " + objDS.instructcomp;
			if (myDebugMode || blkType == "T") str += "\r\nSO\t\t = " + objDS.so;
		}else{
			if (myDebugMode) str = "No display state obj";
		}
	}else{
		if (myDebugMode) str = "Invalid Data shape";
	}
	return str;
}
// New function to get data for visible blocks - separate from drawing
function getBlockData(strBlockID, objCanvas, strPointName){
		
		var arr = strBlockID.split(":");	
		var blkName = arr[1];
		var blkIndex = arr[3];
		var oBlock = document.getElementById(strBlockID);
		var oShape = objCanvas.parentElement.parentElement;
		var oMsg, iT, iS, iU, iV;
		if ( blkIndex > 0 && oBlock){
			var bWasInView = oBlock.getAttribute("inView");
			var bIsInView = isInViewport(oBlock,objCanvas);
if(myDebugMode) LogMsg("getBlockData : oData = " + "dataBlock" + blkIndex);
			var oData = oShape.objects("dataBlock" + blkIndex);
			if(oData) {
				oData.setAttribute("shapeToUpdateID",strBlockID);
			}else{
				if(myDebugMode) LogMsg("getBlockData : src = " + "dataBlock" + blkIndex);
			}
//if(myDebugMode) LogMsg("getBlockData LaunchDefer-> id = " + arr[0] + ":" + arr[1] + ":" + arr[2] + ":" + arr[3] + " | " + oData.id);				
	if(myDebugMode){
		oMsg = document.getElementById(objCanvas.id+"_tmpDebug");
		iT = oMsg.getAttribute("total");
		iS = oMsg.getAttribute("subs");
		iU = oMsg.getAttribute("unsub");
		iV = oMsg.getAttribute("vis");
	}

//if(myDebugMode) LogMsg("getBlockData -> id = " + arr[0] + ":" + arr[1] + ":" + arr[2] + ":" + arr[3] + " | " + oData.id + " | wasInView = " + bWasInView + " : isInView = " + bIsInView);				
			if(bWasInView && bIsInView){
				//
if(myDebugMode) LogMsg(">>> getBlockData [1](bWasInView && bIsInView)-> id = " + strBlockID + ": inInView = " + bIsInView);
				//oData.objects("data").onupdate();
				updateFlow(oData.objects("data"));
				if(myDebugMode){
					//iV++;
					//iS++;
				}
				//return;
			}else if(bWasInView && !bIsInView){
if(myDebugMode) LogMsg(">>> getBlockData [2](bWasInView && !bIsInView)-> id = " + strBlockID + ": inInView = " + bIsInView);
				oData.autoReloadContent = false;
				oData.setCustomProperty("Point","pointName","");
				oData.setCustomProperty("Parameter","param1","");
				oData.setCustomProperty("Value","blockType", "");
				oData.ReloadContent();
				oBlock.setAttribute("inView",false);
				//window.setTimeout("forceDataUpdate("+oData.id+")",500);
				forceDataUpdate(oData.id);
				if(myDebugMode) {
					iU++;
					iV--;
					iS--;
				}
			}else if(!bWasInView && bIsInView){
if(myDebugMode) LogMsg(">>> getBlockData [3](!bWasInView && bIsInView)-> id = " + strBlockID + ": inInView = " + bIsInView);
				oData.autoReloadContent = false;
				oData.setCustomProperty("Point","pointName", strPointName);
				oData.setCustomProperty("Parameter","param1", blkName + ".DISPLAYSTATE");
				oData.setCustomProperty("Value","blockType", arr[2]);
				oData.ReloadContent();
				oBlock.setAttribute("inView",true);
				if(myDebugMode) {
					//iS++;
					iV++;
				}
			}
if(myDebugMode){
	oMsg.setAttribute("subs",iS);
	oMsg.setAttribute("unsub",iU);
	oMsg.setAttribute("vis",iV);
	updateDebugMsg(oMsg);
}
		}
	
}
//
// Test debug function - update datasubscription debug line from attributes
//
function updateDebugMsg(oMsg){
	oMsg.innerText = "t:" + oMsg.getAttribute("total") + "|s:(" + oMsg.getAttribute("subs")+ "/" +  oMsg.getAttribute("vis") + ")|u:" + oMsg.getAttribute("unsub");	
}
//
//	forceDataUpdate - force data update once on unsubscribed data shapes (CP "pointName"="")
//
var forceDataUpdate = function(dataID){
if(myDebugMode) LogMsg("getBlockData -> force update when unsubscribe" );
		var oData = document.getElementById(dataID);
		if(oData){
			//oData.objects("data").onupdate();
			updateFlow(oData.objects("data"));
		}
	};
//
//	Scroll functionality
// 	Global hasScrolled flag 
var hasScrolled;
var elemScrolled;
var scrollTimerId = 0;
//
// canvasScrollEvent - handle "onscroll" (subscribe/unsubscribe data for visible blocks)
//
var canvasScrollEvent = function(){
	var oSource = window.event.srcElement;
	var scrT =  oSource.scrollTop;
	var scrL =  oSource.scrollLeft;
	var scrW =  oSource.scrollWidth;
	var scrH =  oSource.scrollHeight;
	var zoom = oSource.style.zoom;
	
	oSource.setAttribute("lastScrollTop",scrT);
	oSource.setAttribute("lastScrollLeft",scrL);
	oSource.setAttribute("lastScrollWidth",scrW);
	oSource.setAttribute("lastScrollHeight",scrH);
	oSource.setAttribute("lastZoom",zoom);
	var firstScroll = oSource.getAttribute("firstTimeScolled");
	if(firstScroll==null){
		oSource.setAttribute("firstTimeScolled",true);
		window.setInterval(scrollChecker,2000);
	}
	// Clear our timeout throughout the scroll
	window.clearTimeout( scrollTimerId );
	hasScrolled = false;
	elemScrolled = oSource.id;
	// Set a timeout to run after scrolling ends
	scrollTimerId = setTimeout(function(oSource) {

		// Run the callback
if(myDebugMode) LogMsg("onscroll -> Scrolling has stopped. : ");
		hasScrolled = true;
	},500);
//debug only
if (hasScrolled){
	if(myDebugMode) LogMsg("onscroll -> id= " + oSource.id + " : Top = " + scrT+ ": Left = " + scrL + " : Width = " + scrW + " : Height = " + scrH + " : Zoom = " + zoom + " : hasScrolled = " + hasScrolled);
}else{
	if(myDebugMode) LogMsg("onscroll -> id= " + oSource.id + " continuous scrolling...");	
}
};
//
// scrollChecker - execute on timer and wait for "hasScrolled" flag to execute data un-/re-subscribe
//
var scrollChecker = function(){
	var oSource = document.getElementById(elemScrolled);
	var scrT =  oSource.getAttribute("lastScrollTop");
	var scrL =  oSource.getAttribute("lastScrollLeft");
	var scrW =  oSource.getAttribute("lastScrollWidth");
	var scrH =  oSource.getAttribute("lastScrollHeight");
	var zoom = oSource.getAttribute("lastZoom");
	var pointName = oSource.getAttribute("lastPointName");
if(myDebugMode) LogMsg("scrollChecker -> id= " + oSource.id + " : Top = " + scrT+ ": Left = " + scrL + " : Width = " + scrW + " : Height = " + scrH + " : Zoom = " + zoom + " : hasScrolled = " + hasScrolled);
	//Try to resubscribe data after scroll
	if(hasScrolled){
		var container = oSource;
		if(myDebugMode){
			var oMsg = document.getElementById(container.id+"_tmpDebug");
			var iT = oMsg.getAttribute("total");
			var iS = oMsg.getAttribute("subs");
			var iU = oMsg.getAttribute("unsub");
			oMsg.setAttribute("unsub",0);
		}
		//protectScroll(container, true);
		//Loop through all elements in the canvas and filter P,S,T from the block ID
		for (var i=container.children.length-1;i>=0;i--){
			var oObj = container.children[i];		
			if ( oObj.id != "" && oObj.id != null) {		
				if(oObj.id.indexOf(":")>-1 && oObj.id.indexOf(":Y")==-1) {
//if(myDebugMode) LogMsg("drawSeq GetData Loop -> id = " + oObj.id);				
					getBlockData(oObj.id,container,pointName);
				}
			}
		}
		if(myDebugMode) LogMsg("LoopVisible scrollChecker -> ");
		//window.setTimeout(function(){protectScroll(container, false);},5000);
	}
	hasScrolled=false;
};
//
// updateBlocksData - called once after shape is drawn - subscribe data for visible blocks
//
var updateBlocksData = function(container,pointName2){
		var tmpDataCount=0;
		var oMsg;
	//Debug info - number of total/visible/subscribed/unsubscribed blocks
	if(myDebugMode) {
		oMsg = document.getElementById(container.id+"_tmpDebug");
		oMsg.innerText = "Start Data Subscribe...";
		oMsg.setAttribute("total",tmpDataCount);
		oMsg.setAttribute("vis",tmpDataCount);
		oMsg.setAttribute("subs",tmpDataCount);
		oMsg.setAttribute("unsub",tmpDataCount);
	}

		var pointName = container.getAttribute("pointName");
if(myDebugMode) LogMsg("updateBlocksData -> id = " + container.id + " : pntName = " + pointName);		
		container.setAttribute("lastPointName",pointName);
		//protectScroll(container, true);
		for (var i=container.children.length-1;i>=0;i--){
			var oObj = container.children[i];		
			if ( oObj.id != "" && oObj.id != null) {
				if(oObj.id.indexOf(":")>-1 && oObj.id.indexOf(":Y")==-1) {
//if(myDebugMode) LogMsg("drawSeq GetData Loop -> id = " + oObj.id);

					if(myDebugMode) updateDebugMsg(oMsg);
					getBlockData(oObj.id,container,pointName);

					tmpDataCount++;
					if(myDebugMode) oMsg.setAttribute("total",tmpDataCount);
				}
			}
		}
	//Attribute to block onmode refresh until initial data get is complete	
	container.setAttribute("initGetDataComplete",true);	
if(myDebugMode) LogMsg("LoopVisible updateBlocksData -> ");
};
//
// protectScroll - [NOT NECESSARY ???] - disable zoom buttons/scrollbars while waiting on data to be subscribed
//
function protectScroll(container, bDisable){
		
		var oShape = container.parentElement.parentElement;
		var oZoomPlus = oShape.objects("TxtPlus");
		var oZoomMinus = oShape.objects("TxtMinus");
			
		if(bDisable){
			container.style.overflow = "hidden";
		}else{
			container.style.overflow = "auto";
		}
		oZoomPlus.disabled = bDisable;
		oZoomMinus.disabled = bDisable;
}
//
//Scroll to Active steps functionality
//
function scrollActiveToView(oSource){
	
	//var strShapeName = DisplayDataRepository.GetValue("ShapeName");
	//var container = window.external.parentDocument.parentWindow.document.getElementById(strShapeName + "_myCanvas");
	
	var container = document.getElementById(Page.getAttribute("lastselectionseq"));
	try{	
		sBlockName = oSource.value; 

			for (var i=container.children.length-1;i>=0;i--){

				var oObj = container.children[i];
			
				if ( oObj.id != "" && oObj.id != null) {
		
					if(oObj.id.indexOf(":")>-1 && oObj.id.indexOf(":"+sBlockName+":")>-1) {

						var attrIsInView = oObj.getAttribute("inView");
	//window.external.Application.logmessage("Popup -> scrollActiveIntoView -> blk = " + oObj.id + " : isInView = " + bIsInView + " : attrInView = " + attrIsInView);
						if(!attrIsInView){
							oObj.scrollIntoView();

						}
					}
				}
			}
			oSource.style.visibility="hidden";
			oSource.parentElement.style.visibility="hidden";
	}catch(e){
			window.external.Application.logmessage("Popup -> scrollActiveIntoView -> error = " + e.desc);
	}
}
//
//	ConAlpActive_onupdate - push ACTIVELOC.STEP[1-15] in an external "helper" shape to get the names of active steps
//
function ConAlpActive_onupdate(oSource){
		var activeName;
		var idx = parseInt(oSource.id.slice(-2),"10");
		if(oSource.dataexists("pointName.activeloc.step[" + idx +"]")){
			activeName = oSource.datavalue("pointName.activeloc.step[" + idx +"]");
		}
		if(activeName){
			if(activeName.indexOf(".")>-1){
				oSource.value = activeName.split(".")[1];
			}
		}
	
}
//
// showActiveList - show the helper shape next to the selected sequence shape
//
function showActiveList(){

//alert(pntName);
	var objActSteps = document.getElementById(Page.getAttribute("activeStepList"));

	objActSteps.children[0].style.visibility = "inherit";
	
	objActSteps.style.pixelTop = oShape.style.pixelTop;
	objActSteps.style.pixelLeft = oShape.style.pixelLeft;
	
	for(var i=1; i<=15; i++){
		strIdx = ("00" + i).slice(-3);
	
		var oButton = objActSteps.objects("ConalpActiveloc"+strIdx);
		
//if(myDebugMode) LogMsg("showActiveList -> Shape =" + objActSteps.style.pixelTop + " : Button = " + oButton.id + " : Top=" + oButton.style.pixelTop );	
		if(oButton.value != ""){
			oButton.style.pixelTop = (i-1)* (oButton.style.pixelHeight+1);
			oButton.style.visibility = "inherit";
			oButton.style.cursor = "hand";
if(myDebugMode) LogMsg("showActiveList ->  Button = " + oButton.id + " : value = <"+  oButton.value +"> : Top=" + oButton.style.pixelTop );				
		}else{
			oButton.style.visibility = "hidden";
		}
//if(myDebugMode) LogMsg("showActiveList -> Shape =" + objActSteps.style.pixelTop + " : Button = " + oButton.id + " : Top=" + oButton.style.pixelTop );			

	}

}
//Debug external script parsed
if(window.external.Application){window.external.Application.logmessage(window.external.Application.currentpage + " : >>> ebmGSH.js parsed - Sequence Shape release : " + SEQ_RELEASE + " version date:" + SEQ_VERSION_DATE);}