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
// Sequence Shape Scripts for Popups Support
// ------------------------------------------------------------------------------------
var SEQPP_RELEASE = "R511.1";
var SEQPP_VERSION_DATE = "June 2019";
//
var dfInit = false;
var myCSS = findCSSFile("Batch");
var oldBgColor,oldTxtColor,oldLineColor,g_oldStyle;
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
//-----------------------------------------------------------------------------------------------------------------------------------------------
// <Step Detail> popup support functions
//-----------------------------------------------------------------------------------------------------------------------------------------------
//
//	initStepTable()
//	Initializes and draws step details table structure
//
function initStepTable(){
	// Declare variables and create the header, footer, and caption.
	var oTable = document.createElement("TABLE");
	var oTHead = document.createElement("THEAD");
	var oTBody0 = document.createElement("TBODY");
	var oTBody1 = document.createElement("TBODY");
	var oTFoot = document.createElement("TFOOT");
	var oCaption = document.createElement("CAPTION");
	var oRow, oCell;

	var enbBypass;
	//[C[i] BYPASS] Check global CSS setting
	enbBypass = myCSS? findCSSValue(myCSS, '.Step_Popup_Options', 'sp-enable-bypass'):"false";
	enbBypass = (enbBypass.toLowerCase() == "true");

	var heading = [];
	var percents = [];
	
	if(enbBypass){
		heading[0] = "#";
		heading[1] = "I";
		heading[2] = "Description";
		heading[3] = "BYP";
		//heading[2] = "Low";
		//heading[3] = "Close";

		percents[0] = '5%';
		percents[1] = '5%';
		percents[2] = '85%';
		percents[3] = '5%';
	}else{
		heading[0] = "#";
		heading[1] = "I";
		heading[2] = "Description";
		//heading[2] = "Low";
		//heading[3] = "Close";

		percents[0] = '5%';
		percents[1] = '5%';
		percents[2] = '90%';
	}

	// Insert the created elements into oTable.
	oTable.appendChild(oTHead);
	oTable.appendChild(oTBody0);
	oTable.appendChild(oTBody1);
	oTable.appendChild(oTFoot);
	oTable.appendChild(oCaption);

	// Set the table's border ID
	oTable.id = "ebmStepTable";

	// Insert a row into the header and set its background color.
	oRow = document.createElement("TR");
	oTHead.appendChild(oRow);
	
	// Create and insert cells into the header row.
	for (var i=0; i<heading.length; i++){
		oCell = document.createElement("TH");
		oCell.innerHTML = heading[i];
		oCell.width = percents[i];
		if (i!=2){oCell.align = 'center';}
		oRow.appendChild(oCell);
	}

	// Insert rows and cells into bodies.
	for (var ii=0; ii<17; ii++){
		//var oBody = (i<2) ? oTBody0 : oTBody1;
		var myVar, oBody = oTBody0;
		oRow = document.createElement("TR");
		//oRow.style.fontSize = "14px";
		oRow.style.display = "none";
		oBody.appendChild(oRow);
		myVar = oRow.attachEvent('onmouseover', myMouseOverStep);
		myVar = oRow.attachEvent('onmouseout', myMouseOutStep);
		myVar = oRow.attachEvent('onclick', myDoubleClickStep);
		for (var j=0; j<heading.length; j++){
			oCell = document.createElement("TD");
			oCell.innerHTML = ii;
			if (j!=2){oCell.align = 'center';}
			oRow.appendChild(oCell);
			 //Bypass (if exists)
			 if(heading[j]=="BYP"){
				//[OP[j] BYPASS] append checkbox
				oCell.innerHTML = "";
				oRow.appendChild(oCell);	
				
				//OPTION 1 - checkbox
				var chkBypass = document.createElement("input"); 
				chkBypass.setAttribute("id","chkbox"+ii); 
				chkBypass.setAttribute("type","checkbox"); 
					
				myVar = chkBypass.attachEvent('onclick', myBypReqStep);
				//chkBypass.onclick = myBypReqStep;
				oCell.appendChild(chkBypass);
				chkBypass.className = "Checkbox_Bypass_Perm";
				
/* 				//OPTION 2 - listbox/select - similar to ControlBuilder
				var cboBypReq = document.createElement("select"); 
				cboBypReq.setAttribute("id","slct"+i);
				cboBypReq.options[0] = new Option('None','None');
				cboBypReq.options[1] = new Option('Skip','Skip');
					
					myVar = cboBypReq.attachEvent('onchange', myBypReqChangeStep);
					oCell.appendChild(cboBypReq);
					cboBypReq.className = "Combo_Bypass_Req"; */
			}else{
				oRow.appendChild(oCell);
			}
		}
	}
	//
	// Create and insert rows and cells into the footer row.
	oRow = document.createElement("TR");
	oTFoot.appendChild(oRow);
	oCell = document.createElement("TD");
	oRow.appendChild(oCell);
	oCell.innerHTML = "STEP ExecStatus : " + ConAlpState.value ;
	oCell.colSpan = "4";
	//
	// Set the innerHTML of the caption and position it at the bottom of the table.
	//oCaption.innerHTML = "Honeywell ProcOps & Batch";//"Honeywell Batch Center of Excellence"
	//
	// Insert the table into the document tree.
	myTable.appendChild(oTable);
	return true;
}
//
//	updateStepList()
//
function updateStepList(oSource){
	var tmpByPReq,myCursor;
	var oShape = oSource.parentElement;

	var mTable = document.getElementById('ebmStepTable');
	var tmpPointName = oShape.GetCustomProperty('point','pointName');
	var tmpStepName = oShape.GetCustomProperty('value','stepName');
	var tmpStepIndex = oShape.GetCustomProperty('value','stepIndex');

	var tmpString = tmpPointName+"."+tmpStepName+".OP["+tmpStepIndex+"]";

	var tmpState = oSource.DataValue(tmpString+'.STATE');
		
	var tmpInstructpend = oSource.DataValue(tmpString+'.INSTRUCTPEND');
	var tmpInstructtype = oSource.DataValue(tmpString+'.INSTRUCTTYPE');
	var tmpDesc = oSource.DataValue(tmpString+'.DESC');
	//Exit in case Activity is destroyed when the popup is still open
	if(!oSource.QualityGood(tmpString+'.DESC')) { return; }
	//
	if (oSource.DataExists(tmpString+'.BYPREQ')){
		tmpByPReq = oSource.DataValue(tmpString+'.BYPREQ');
	}else{
		tmpByPReq = "";
	}
	//tmpInstructType = shape001.Objects('data').DataValue(tmpString+'.INSTRUCTTYPE');
	//tmpNote = shape001.Objects('data').DataValue(tmpString+'.NOTE');
	//tmpWarning = shape001.Objects('data').DataValue(tmpString+'.WARNING');

	switch(tmpInstructtype){
		case 1:
			mTable.tBodies[0].rows[parseInt(tmpStepIndex)].cells[1].innerText = "T";
			break;
		case 2:
			mTable.tBodies[0].rows[parseInt(tmpStepIndex)].cells[1].innerText = "C";
			break;
		default:
			mTable.tBodies[0].rows[parseInt(tmpStepIndex)].cells[1].innerText = "-";
			break;
	}
	mTable.tBodies[0].rows[parseInt(tmpStepIndex)].cells[2].innerText = tmpDesc+" ";
	mTable.tBodies[0].rows[parseInt(tmpStepIndex)].style.display = "";
	var myRowTitle = "";
	var stepInstrClass = "Step_Instruction";
	switch(parseInt(tmpState)){
		case 0:
		case 1:
		case 8:
			stepInstrClass =  "Step_Instruction_State"+tmpState;
			myCursor = "default";
			break;
		case 6:
			stepInstrClass =  "Step_Instruction_State"+tmpState;
			myCursor = "default";
			if(parseInt(tmpInstructpend) == 1){
				stepInstrClass =  "Step_Instruction_Pending";
				myRowTitle = "Click to invoke confirmation dialog";
				myCursor = "pointer";
			}
			break;
		default:
	}
	//Set/Clear tooltip
	mTable.tBodies[0].rows[parseInt(tmpStepIndex)].title = myRowTitle;
	for(var i = 0;i< mTable.rows[parseInt(tmpStepIndex)].cells.length;i++){
		mTable.tBodies[0].rows[parseInt(tmpStepIndex)].cells[i].className = stepInstrClass;
		mTable.tBodies[0].rows[parseInt(tmpStepIndex)].cells[i].style.cursor = myCursor;
	}
	//[OP[i] BYPASS] Update BYPREQSTS
	tmpobj = document.getElementById('chkbox'+parseInt(tmpStepIndex));
	tmpobj2 = document.getElementById('slct'+parseInt(tmpStepIndex));
	if (tmpByPReq.toUpperCase() == 'SKIP'){
		if(tmpobj) tmpobj.checked = true;
		//if(tmpobj2) tmpobj2.disabled = false;
	}else{
		if(tmpobj) tmpobj.checked = false;
		//if(tmpobj2) tmpobj2.disabled = true;
	}		
	/*switch(tmpByPReq){
		case'None': if(tmpobj2) tmpobj2.value = 'None'; break;
		case'Skip': if(tmpobj2) tmpobj2.value = 'Skip'; break;
	}*/
	
}
//
// myMousOver() function - Step List 
//
function myMouseOverStep(e){
	var rowNumber;
	myTBL = document.getElementById('ebmStepTable').tBodies[0];
	if(e.srcElement.parentNode.tagName == "TR"){
		rowNumber =  e.srcElement.parentNode.rowIndex - 1;
	}else if(e.srcElement.parentNode.tagName == "TD"){
		rowNumber =  e.srcElement.parentNode.parentNode.rowIndex - 1;
	}
	//Temporarily save current style
	g_oldStyle =  myTBL.rows[rowNumber].cells[0].className;
	//Apply hover class
	for(var i = 0;i< myTBL.rows[rowNumber].cells.length;i++){
		myStyle = "Step_Instruction_Hover";
		myTBL.rows[rowNumber].cells[i].className = myStyle;
	}
	myPointName = DisplayDataRepository.GetValue('pointName');
	myStepName = DisplayDataRepository.GetValue('stepName');
	tmpString = 'shape0'+('0'+rowNumber).slice(-2);
	myShape = document.getElementById(tmpString);
	myStepIndex = myShape.GetCustomProperty('value','stepIndex');
	tmpString = myPointName+"."+myStepName+".OP["+myStepIndex+"]";
	tmpInstructType = myShape.Objects('data').DataValue(tmpString+'.INSTRUCTTYPE');
	//Exit in case Activity is destroyed when the popup is still open
	if(!myShape.Objects('data').QualityGood(tmpString+'.INSTRUCTTYPE')) { return; }
	//
	if(tmpInstructType=='2'){
		tmpInstruction = myShape.Objects('data').DataValue(tmpString+'.INSTRUCTION');
		//myTBL.rows[rowNumber].title = "Instruction:\r\n\r\n" + tmpInstruction;
		myMessages.innerHTML = tmpInstruction;
	}else{
		//myMessages.style.fontSize = "8px";
		tmpExpression = myShape.Objects('data').DataValue(tmpString+'.SRCEXPR');
		//myTBL.rows[rowNumber].title = "Expression:\r\n\r\n" + tmpExpression;
		myMessages.innerHTML = '<span>'+tmpExpression+'</span>';
		if(tmpExpression != "") myMessages.style.visibility = "inherit";
	}
	myMessages.style.pixelLeft = window.event.x+10;
	myMessages.style.pixelTop = window.event.y+10;
}
//
// myMouseOutStep() function - Step List 
//
function myMouseOutStep(e){
	var rowNumber;
	myTBL = document.getElementById('ebmStepTable').tBodies[0];
	if(e.srcElement.parentNode.tagName == "TR"){
		rowNumber =  e.srcElement.parentNode.rowIndex - 1;
	}else if(e.srcElement.parentNode.tagName == "TD"){
		rowNumber =  e.srcElement.parentNode.parentNode.rowIndex - 1;
	}
	//Revert to previous style
	for(var i = 0;i< myTBL.rows[rowNumber].cells.length;i++){
		myTBL.rows[rowNumber].cells[i].className = g_oldStyle;
	}
	myMessages.innerText = '';
	myMessages.style.visibility = "hidden";
}
//
// myDoubleClickStep() function
//
function myDoubleClickStep(e){
	var rowNumber,myBypReq;
	myPointName = DisplayDataRepository.GetValue('pointName');
	myStepName = DisplayDataRepository.GetValue('stepName');
	if(e.srcElement.parentNode.tagName == "TR"){
		rowNumber =  e.srcElement.parentNode.rowIndex - 1;
		myBypReq = false;
	}else if(e.srcElement.parentNode.tagName == "TD"){
		rowNumber =  e.srcElement.parentNode.parentNode.rowIndex - 1;
		myBypReq = true;
	}
	tmpString = 'shape0'+('0'+rowNumber).slice(-2);
	myShape = document.getElementById(tmpString);
	myStepIndex = myShape.GetCustomProperty('value','stepIndex');

	tmpString = myPointName+"."+myStepName+".OP["+myStepIndex+"]";
	tmpInstructType = myShape.Objects('data').DataValue(tmpString+'.INSTRUCTTYPE');
	tmpInstructPend = myShape.Objects('data').DataValue(tmpString+'.INSTRUCTPEND');
	if(tmpInstructType=='2' && tmpInstructPend && !myBypReq){

		var myCheck = ChangeDSDTable(myPointName,myStepName,myStepIndex);
		if(!myCheck) { 
			window.external.application.TemporaryMessageZoneText = "Alerts associated with " + myPointName + "." + myStepName + ".OP["+ myStepIndex + "]" + " will not be acknowledged!";
		}
		//df = wshell.run(changeAlertCommand, 0, true);
		var alertPopup=DisplayDataRepository.GetValue('AlertConfirmDisplay');
		var popupCommand = alertPopup+"?CurrrentPoint="+myPointName+"&cPoint="+myPointName+"&myStepName="+myStepName+"&myStepIndex="+myStepIndex;
		HDXPopupBehavior.CreatePopupWindow2(popupCommand, 0, 0, 0, true);
	}else{
	}
}
//'------------------------------------------------------------
//' ChangeDSDTable function
//'------------------------------------------------------------
function ChangeDSDTable(sPntName, sStepName, sStepIndex) {
var xmldoc, xmldoc1;
LogMsg("Popup ChangeDSDable -> Application.securitylevel = " +window.external.parentDocument.parentWindow.external.securitylevel);
	if (typeof (window.external.parentDocument.parentWindow.external.CreateWhitelistedObject ) != "undefined") { 
		//R43x.x/R5xx.x
		try{
			xmldoc = window.external.parentDocument.parentWindow.external.Application.CreateWhitelistedObject("msxml2.DOMDocument.6.0",1);
		}catch(e){
			if (window.external.Application.environment == 0){window.external.Application.LogMessage(window.external.Application.currentpage + ">> Unable to whitelist msxml object! -> " + e.description);}
		}
	} else {
		if (typeof(SafetyWrapper)!="undefined") {
			//R41x.x legacy
			if(typeof(SafetyWrapper.MakeObjectSafe)!="undefined"){
				xmldoc = SafetyWrapper.MakeObjectSafe("msxml2.DOMDocument.6.0");
			}else{
				if (window.external.Application.environment == 0){window.external.Application.LogMessage(window.external.Application.currentpage + ">> SafetyWrapper.MakeObjectSafe not implemented!");}
				//return false;
			}
		}else{
			if (window.external.Application.environment == 0){window.external.Application.LogMessage(window.external.Application.currentpage + ">> SafetyWrapper object not available!");}
			//return false;
		}
	}
	if (xmldoc == null) {
		LogMsg("ChangeDSDTable -> xmldoc is not a XML object, exiting....");
		return false;
	}
	xmldoc1 = new ActiveXObject("msxml2.DOMDocument.6.0");

	try{
		if(xmldoc.async) xmldoc.async = false;
	}catch(e){
		LogMsg("ChangeDSDTable -> xmldoc = " + typeof xmldoc + " : err = " + e.description);	
	}
	
	
	if(xmldoc1.async) xmldoc1.async = false;
	

	var myPopup = DisplayDataRepository.GetValue("AlertConfirmDisplay");

	var DisplayPath = window.external.Application.FindFile(myPopup);

	if(DisplayPath){
		sDSDFilePath = DisplayPath.replace(".htm","_files\\DS_datasource1.dsd");
	}else{
		if (window.external.Application.environment == 0){window.external.application.LogMessage(window.external.Application.currentpage + ">> AlertConfirm Popup not found!");}
		return false;
	}
	try{
		xmldoc.load(sDSDFilePath);
	}catch(e){
		if (window.external.Application.environment == 0){window.external.Application.LogMessage(window.external.Application.currentpage + ">> Unable to load DSD file as XML : " + sDSDFilePath);}
		return false;
	}

	var oTemplateXML = xmldoc.selectSingleNode("//dataobject/property[@name=\'TemplateXML\']");
 
	xmldoc1.loadXML(oTemplateXML.text);

	var oColRef = xmldoc1.selectSingleNode("//BaseFilterXML/BaseFilterDef/colref[@FilterField=\'Source\']");
	oColRef.setAttribute("FilterState", "0$$DELIM$$" + sPntName);
	var strFilter = oColRef.getAttribute("FilterField") + " like \'" + sPntName.replace("_", "[_]") + "\'";
	oColRef.setAttribute("FilterString", strFilter);

	oColRef = xmldoc1.selectSingleNode("//BaseFilterXML/BaseFilterDef/colref[@FilterField=\'Description\']");
	oColRef.setAttribute("FilterState", "0$$DELIM$$" + sStepName + ".OP[" + sStepIndex + "]");
	strFilter = oColRef.getAttribute("FilterField") + " like \'" + sStepName.replace("_", "[_]") + ".OP[[]" + sStepIndex + "]%\'";
	oColRef.setAttribute("FilterString", strFilter);

	oTemplateXML.text = xmldoc1.xml;

	try{
		xmldoc.save(sDSDFilePath);
		return true;
	}catch(e){
		LogMsg("ChangeDSDTable -> unable to save XML :  err = " + e.description);
		return false;
	}
}
//
// StepPopup_pagecomplete() function
// Step Popup onpagecomplete handler function
//
function StepPopup_pagecomplete(){
	//myTitle.innerText = DisplayDataRepository.GetValue('pointName').toUpperCase()+ '.' + DisplayDataRepository.GetValue('stepName').toUpperCase();
	dfInit = initStepTable();

	myPointName = DisplayDataRepository.GetValue('pointName');
	myStepName = DisplayDataRepository.GetValue('stepName');
	document.title += " - " + myPointName + '.' + myStepName;
	myTitle.innerText = ConAlpDESC.value;
	myTitle.title = "Step description";
if(myDebugMode) LogMsg("StepPopup_pagecomplete -> pnt = " + myPointName + " : step = " + myStepName);
	for(i=0;i<ConAlpNUM.value;i++){
		tmpString = 'shape0'+('0'+(i+1)).slice(-2);
	//	myTitle.innerText = '>'+tmpString+'<';
		myShape = document.getElementById(tmpString);
if(myDebugMode) LogMsg("StepPopup_pagecomplete -> stepOPshape = " + myShape.id + ": NUMOUTPUTS = " + ConAlpNUM.value);		
	//	myShape =alpha001.parentElement.Objects('tmpString');
	//	myShape = document.getElementById(tmpString);
		myShape.autoReloadContent = false;
		myShape.SetCustomProperty('point', 'pointName',myPointName);
		myShape.SetCustomProperty('value', 'stepName', myStepName);
		myShape.ReloadContent();
	//	dObj = myShape.Objects('data');

		}
}
// BYPASS conditions
function myBypReqStep(e){
	var srcElm = e.srcElement.id;
	var re = /\d{1,2}$/g;
	var CondNo = srcElm.match(re);
	oShape = document.getElementById('shape0'+('0'+CondNo[0]).slice(-2));
	tmpPointName = oShape.GetCustomProperty('point','pointName');
	tmpStepName = oShape.GetCustomProperty('value','stepName');
	tmpStepIndex = oShape.GetCustomProperty('value','stepIndex');
	tmpString = tmpPointName+"."+tmpStepName+".OP["+tmpStepIndex+"]";
if(myDebug) debugWrite("Step OP BYP : " + tmpString, srcElm);
	if (e.srcElement.checked){
		oShape.Objects('data').SetDataValueSynchronous(tmpString+'.BYPREQ', 'SKIP');
	}else{
		oShape.Objects('data').SetDataValueSynchronous(tmpString+'.BYPREQ', 'NONE');
	}	
if(myDebug) debugWrite("Step OP BYP : BYPREQ =  " + oShape.Objects('data').GetDataValue(tmpString+'.BYPREQ'), srcElm);
	//if(myDebug) debugWrite("AAA: ", oShape.Objects('data').DataValue(tmpString+'.BYPPERM'));	
}
function myBypReqChangeStep(e){

	var srcElm = e.srcElement.id;
	var re = /\d{1,2}$/g;
	var CondNo = srcElm.match(re);
	oShape = document.getElementById('shape0'+('0'+CondNo[0]).slice(-2));
	tmpPointName = oShape.GetCustomProperty('point','pointName');
	tmpStepName = oShape.GetCustomProperty('value','stepName');
	tmpStepIndex = oShape.GetCustomProperty('value','stepIndex');
	tmpString = tmpPointName+"."+tmpStepName+".OP["+tmpStepIndex+"]";

	switch(e.srcElement.value){
		case'None': oShape.Objects('data').SetDataValueSynchronous(tmpString+'.BYPREQ', 'None'); break;
		case'Skip': oShape.Objects('data').SetDataValueSynchronous(tmpString+'.BYPREQ', 'Skip'); break;
	}
	//if(myDebug) debugWrite("Chng: ", e.srcElement.value+" "+ oShape.Objects('data').DataValue(tmpString+'.BYPREQ') );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// <Transition Detail> popup support functions
//-----------------------------------------------------------------------------------------------------------------------------------------------
//
// TransPopup_pagecomplete()
// Transition Popup onpagecomplete handler function
//
function TransPopup_pagecomplete(){
	dfInit = initTransitionTable();
	myPointName = DisplayDataRepository.GetValue('pointName');
	myTransitionName = DisplayDataRepository.GetValue('transitionName');
	document.title += " - " +  myPointName + '.' + myTransitionName;
	myTitle.innerText = ConAlpDESC.value;
	myTitle.title = "Transition description";
	numConds = 0;
	//numConds = data.DataValue(myPointName + '.' + myTransitionName + '.NUMCONDS');
	for(i=0;i<ConAlpNUM.value;i++){
		tmpString = 'shape0'+('0'+(i+1)).slice(-2);
		myShape = document.getElementById(tmpString);
		myShape.autoReloadContent = false;
		myShape.SetCustomProperty('point', 'pointName',myPointName);
		myShape.SetCustomProperty('value', 'transName', myTransitionName);
		myShape.ReloadContent();
	//	dObj = myShape.Objects('data');
	}
}
//
// initTransitionTable() function
//
function initTransitionTable(){
	// Declare variables and create the header, footer, and caption.
	var oTable = document.createElement("TABLE");
	var oTHead = document.createElement("THEAD");
	var oTBody0 = document.createElement("TBODY");
	var oTBody1 = document.createElement("TBODY");
	var oTFoot = document.createElement("TFOOT");
	var oCaption = document.createElement("CAPTION");
	var oRow, oCell;
	var i, j;
	
	//[C[i] BYPASS] Check global CSS setting
	var enbBypass = myCSS? findCSSValue(myCSS, '.Transition_Popup_Options', 'sp-enable-bypass'):"false";
	enbBypass = (enbBypass.toLowerCase() == "true");

	var enbSetPerm = myCSS? findCSSValue(myCSS, '.Transition_Popup_Options', 'sp-allow-set-perm'):"false";
	enbSetPerm = (enbSetPerm.toLowerCase() == "true");

	var heading = [];
	var percents = [];
	var cellPercents = [];
	
	if(enbBypass && enbSetPerm){
		heading[0] = "#";
		heading[1] = "Descriptions";
		heading[2] = "PERM";
		heading[3] = "BYP";
		heading[4] = "P-Gates";
		heading[5] = "S-Gate";
		
		percents[0] = '5%';
		percents[1] = '50%';
		percents[2] = '5%';
		percents[3] = '5%';
		percents[4] = '8%';
		percents[5] = '8%';
		
		cellPercents[0] = "5%";
		cellPercents[1] = "50%";
		cellPercents[2] = "5%";
		cellPercents[3] = "5%";
		cellPercents[4] = "8%";
		cellPercents[5] = "8%";
	}else if(enbBypass && !enbSetPerm){
		heading[0] = "#";
		heading[1] = "Descriptions";
		heading[2] = "BYP";
		heading[3] = "P-Gates";
		heading[4] = "S-Gate";
		
		percents[0] = '5%';
		percents[1] = '55%';
		percents[2] = '5%';
		percents[3] = '8%';
		percents[4] = '8%';
		
		cellPercents[0] = "5%";
		cellPercents[1] = "55%";
		cellPercents[2] = "5%";
		cellPercents[3] = "8%";
		cellPercents[4] = "8%";
	}else{
		heading[0] = "#";
		heading[1] = "Descriptions";
		heading[2] = "P-Gates";
		heading[3] = "S-Gate";

		percents[0] = '5%';
		percents[1] = '60%';
		percents[2] = '8%';
		percents[3] = '8%';

		cellPercents[0] = "5%";
		cellPercents[1] = "60%";
		cellPercents[2] = "8%";
		cellPercents[3] = "8%";
	}

	// Insert the created elements into oTable.
	oTable.appendChild(oTHead);
	oTable.appendChild(oTBody0);
	oTable.appendChild(oTBody1);
	oTable.appendChild(oTFoot);
	oTable.appendChild(oCaption);

	// Set the table's border width and colors.
	oTable.border=1;
	oTable.style.border = '1px';
	oTable.id = "ebmStepTable";

	// Insert a row into the header and set its background color.
	oRow = document.createElement("TR");
	oTHead.appendChild(oRow);
	// Create and insert cells into the header row.
	for (i=0; i<heading.length; i++)
	{
		oCell = document.createElement("TH");
		oCell.innerHTML = heading[i];
		oCell.width = percents[i];
		//if (i==1){oCell.colSpan = 3;}
		oRow.appendChild(oCell);
	}

	// Insert rows and cells into bodies.
	var myVar, oBody = oTBody0;
	for (i=0; i< 11; i++){
		oRow = document.createElement("TR");
		//oRow.style.fontSize = "10px";
		oRow.style.display = "none";
		oBody.appendChild(oRow);
		myVar = oRow.attachEvent('onmouseover', myMouseOverTrans);
		myVar = oRow.attachEvent('onmouseout', myMouseOutTrans);
		//myVar = oRow.attachEvent('ondblclick', myDoubleClickTrans);
		for (j=0; j<heading.length; j++){
			oCell = document.createElement("TD");
			oCell.width = cellPercents[j];
			if (j!=1){oCell.align = 'center';}
			if(j<2){
				oCell.innerHTML = i;
				oRow.appendChild(oCell);
			}else if(heading[j]=="PERM"){
				//Bypass (if exists)
				//[C[i] BYPASS] append checkbox
				oCell.innerHTML = "";
				//oCell.bgColor = myRowColor;	
				oRow.appendChild(oCell);	
				
					var chkBypPerm = document.createElement("input"); 
					chkBypPerm.setAttribute("id","chkbox"+i); 
					chkBypPerm.setAttribute("type","checkbox"); 
					
					myVar = chkBypPerm.attachEvent('onclick', myBypReqTrans);
					oCell.appendChild(chkBypPerm);
					chkBypPerm.className = "Checkbox_Bypass_Perm";
			}else if(heading[j]=="BYP"){
				//[C[i] BYPASS] append combo
				oCell.innerHTML = "";
				oRow.appendChild(oCell);	
					var cboBypReq = document.createElement("select"); 
					cboBypReq.setAttribute("id","slct"+i);

					cboBypReq.options[0] = new Option('-','-');
					cboBypReq.options[1] = new Option('0','0');
					cboBypReq.options[2] = new Option('1','1');
					myVar = cboBypReq.attachEvent('onchange', myBypReqChange);
					// oCell.appendChild(cboBypReq); 
					oCell.appendChild(cboBypReq); 
					cboBypReq.className = "Combo_Bypass_Req";
			}else if(heading[j]=="P-Gates"){
				//first level of gates
				if(i == G2FirstCond.value && (parseInt(G2FirstCond.value)>0)){
					oCell.innerHTML = G2AlgoType.innerText;
					oCell.rowSpan = G2NumInpts.value;
					oRow.appendChild(oCell);
				}
				if(i == G3FirstCond.value && (parseInt(G3FirstCond.value)>0)){
					oCell.innerHTML = G3AlgoType.innerText;
					oCell.rowSpan = G3NumInpts.value;
					oCell.bgColor = myRowColor;
					oRow.appendChild(oCell);
				}
				if(i == G4FirstCond.value  && (parseInt(G4FirstCond.value)>0)){
					oCell.innerHTML = G4AlgoType.innerText;
					oCell.rowSpan = G4NumInpts.value;
					oRow.appendChild(oCell);
				}
			}else if(heading[j]=="S-Gate"){
				// Final Gate Column set up
				if(i==1 && (parseInt(G1FirstCond.value)>0)){
					oCell.innerHTML = G1AlgoType.innerText;
					if (ConAlpNUM.value > 0) {oCell.rowSpan = ConAlpNUM.value;}
					oRow.appendChild(oCell);
				}
			}	
		}
	}
	// Create and insert rows and cells into the footer row.
	oRow = document.createElement("TR");
	oTFoot.appendChild(oRow);
	oCell = document.createElement("TD");
	oRow.appendChild(oCell);
	oCell.innerHTML = "TRANSITION ExecSts : " + ConAlpState.value;
	//oCell.align = 'center';
	oCell.colSpan = heading.length.toString();
	//oCell.bgColor = "#C0C0C0";

	// Set the innerHTML of the caption and position it at the bottom of the table.
	//oCaption.innerHTML = "Honeywell ProcOps & Batch";//"Honeywell Batch Center of Excellence"
	//oCaption.style.fontSize = "10px";
	//oCaption.align = "bottom";

	// Insert the table into the document tree.
	myTable.appendChild(oTable);
	return true;
}
//
// updateTransitionList() function
//
function updateTransitionList(oShape){
	mTable = document.getElementById('ebmStepTable');
	tmpPointName = oShape.parentElement.GetCustomProperty('point','pointName');
	tmpTransName = oShape.parentElement.GetCustomProperty('value','transName');
	tmpTransIndex = oShape.parentElement.GetCustomProperty('value','transIndex');
	tmpString = tmpPointName+"."+tmpTransName+".C["+tmpTransIndex+"]";
	//Exit in case Activity is destroyed when the popup is still open
	if(!oShape.QualityGood(tmpString+'.DESC')) { return; }
	//
	tmpDesc = oShape.DataValue(tmpString+'.DESC');
	tmpState = oShape.DataValue(tmpString+'.FL');
	tmpExpr = oShape.DataValue(tmpString+'.EXPR');
	//tmpGateAssign = oShape.DataValue(tmpString+'.GATEASGN');
	//BYPASS confitions - get data
	tmpByPPerm = oShape.DataValue(tmpString+'.BYPPERM');
if(myDebug) debugWrite("TransBypReq : ", "C[" + tmpTransIndex + "].BYPPERM = " + tmpByPPerm);
	tmpByPReq = oShape.DataValue(tmpString+'.BYPREQ');
	tmpExpr = tmpExpr.replace(/\$/g,tmpPointName);
	tmpExpr = tmpExpr.replace(/[^\w|^\.]DATA/g,tmpPointName+'.DATA');
	var re = /\w*[a-zA-Z_]+\w*(\.\w*[a-zA-Z_]+(?:\w|\[\d+\])*)+/g; 
	tmpArray = tmpExpr.match(re);
	//mTable.tBodies[0].rows[parseInt(tmpTransIndex)].cells[2].innerText = tmpGateAssign.toString();
	mTable.tBodies[0].rows[parseInt(tmpTransIndex)].cells[1].innerText = tmpDesc;
	mTable.tBodies[0].rows[parseInt(tmpTransIndex)].style.display = "";
	//Apply state class
	var transCondClass= "Transition_Condition_State"+tmpState;
	mTable.tBodies[0].rows[parseInt(tmpTransIndex)].cells[0].className = transCondClass;
	mTable.tBodies[0].rows[parseInt(tmpTransIndex)].cells[1].className = transCondClass;
	//Check if RegEx returned an array of matches
	if(Object.prototype.toString.call(tmpArray)==='[object Array]'){
		for(var i = 0; i<tmpArray.length; i++){
			var tArray = tmpArray[i].split('.');
			var tptName = tArray.splice(0,1);
			var tParam = tArray.join('.');
			df = addToDataArray(tptName, tParam);
		}
	}else{
		if(myDebug){window.external.Application.logmessage(">> Expression ["+tmpExpr+"] not recognized");}
	}
	//[C[i] BYPASS] Update BYPREQSTS
	tmpobj = document.getElementById('chkbox'+parseInt(tmpTransIndex));
	tmpobj2 = document.getElementById('slct'+parseInt(tmpTransIndex));
	if (tmpByPPerm == 'ON'){
		if(tmpobj) tmpobj.checked = true;
		if(tmpobj2) tmpobj2.disabled = false;
	}else{
		if(tmpobj) tmpobj.checked = false;
		if(tmpobj2) tmpobj2.disabled = true;
	}
	switch(tmpByPReq){
		case'None': if(tmpobj2) tmpobj2.value = '-'; break;
		case'ToOff': if(tmpobj2) tmpobj2.value = '0'; break;
		case'ToOn': if(tmpobj2) tmpobj2.value = '1'; break;
	}
	//Update condition gates status
	updateStateConditions();
}
//
// updateStateConditions() function
//
function updateStateConditions(){
	mTable = document.getElementById('ebmStepTable').tBodies[0];
	// Overall Condition
	mTable.rows[1].cells[mTable.rows[1].cells.length-1].className = "t_gate_" + (G1State.value == 1);

	if(parseInt(G2FirstCond.value)>0){
		mTable.rows[G2FirstCond.value].cells[mTable.rows[G2FirstCond.value].cells.length-2].className = "t_gate_" + (G2State.value == 1);
	}
	if(parseInt(G3FirstCond.value)>0){
		mTable.rows[G3FirstCond.value].cells[mTable.rows[G3FirstCond.value].cells.length-1].className = "t_gate_" + (G3State.value == 1);
	}
	if(parseInt(G4FirstCond.value)>0){
		mTable.rows[G4FirstCond.value].cells[mTable.rows[G4FirstCond.value].cells.length-1].className = "t_gate_" + (G4State.value == 1);
	}
}
//
// myMousOver() function - Transition List
// 
function myMouseOverTrans(e){
	var rowNumber;
	var myTBL = document.getElementById('ebmStepTable').tBodies[0];
	if(e.srcElement.parentNode.tagName == "TR"){
		rowNumber =  e.srcElement.parentNode.rowIndex - 1;
	}else if(e.srcElement.parentNode.tagName == "TD"){
		rowNumber =  e.srcElement.parentNode.parentNode.rowIndex - 1;
	}
if(myDebug) debugWrite ("TransBypReq : src = ", e.srcElement.id + " : row = " + rowNumber );
	if (rowNumber){
		g_oldStyle = myTBL.rows[rowNumber].cells[0].className;
		//Apply hover class
		myTBL.rows[rowNumber].cells[0].className = "Transition_Condition_Hover";
		myTBL.rows[rowNumber].cells[1].className = "Transition_Condition_Hover";
	}
	var myPointName = DisplayDataRepository.GetValue('pointName');
	var myTransName = DisplayDataRepository.GetValue('transitionName');
	var tmpString = 'shape0'+('0'+rowNumber).slice(-2);
	var myShape = document.getElementById(tmpString);
	var myTransIndex = myShape.GetCustomProperty('value','transIndex');
	tmpString = myPointName+"."+myTransName+".C["+myTransIndex+"]";
	var tmpExpression = myShape.Objects('data').DataValue(tmpString+'.EXPR');
	var tmpGateAssign = myShape.Objects('data').DataValue(tmpString+'.GATEASGN');
	var tmpExpr2;
	//Exit in case Activity is destroyed when the popup is still open
	if(!myShape.Objects('data').QualityGood(tmpString+'.EXPR')) { return; }
	//
	//replace "$" in expression with actula point name
	tmpExpression = tmpExpression.replace(/\$/g,myPointName);
	// ? 
	tmpExpression = tmpExpression.replace(/[^\w|^\.]DATA/g,tmpPointName+'.DATA');
	// add space around logical operators for hyphenation
	tmpExpression = tmpExpression.replace(/(\w|\])(>|=|<|>=|<=|<>)(\w|\d)/g, '$1 $2 $3');

	if (tmpExpression.match(/\.[a-zA-Z_]/g)){
		var re = /\w*[a-zA-Z_]+\w*(\.\w*[a-zA-Z_]+(?:\w|\[\d+\])*)+/g; 
		var tmpArray = tmpExpression.match(re);
		tmpExpr2 = tmpExpression;
		for(var i = 0; i<tmpArray.length; i++){
			try{
				//Get parameter(operand) values and replace in condition expression
				var tmpValue = dataShape.Objects('data').DataValue(tmpArray[i]);
				tmpExpr2 = tmpExpr2.replace(tmpArray[i], tmpValue.toFixed(2));
			}catch(err){
				if(myDebug){debugWrite('myMouseOver: ', i+':Error - DataValue('+tmpArray[i]+')=' + err.number +':'+err.message + '<');}
			}
		}
	}else{
		tmpExpr2 = "";
	}
        var msgDiv1 = '<div style="height:45%;text-align:center;">Expr : ' + tmpExpression + '</div>';
	var msgDiv2 = '<hr style="width:100%;"><div style="text-align:center;">Eval :' + tmpExpr2 + '</div>';
	if (tmpExpr2 != ""){
		myMessages.innerHTML = msgDiv1 + msgDiv2; 
	}else{
		myMessages.innerHTML = msgDiv1;
	}
	myMessages.style.visibility = "inherit";
	myMessages.style.pixelLeft = window.event.x+10;
	myMessages.style.pixelTop = window.event.y+10;
}
//
// myMouseOutTrans() function - Transition List 
//
function myMouseOutTrans(e){
	var rowNumber;
	myTBL = document.getElementById('ebmStepTable').tBodies[0];
	if(e.srcElement.parentNode.tagName == "TR"){
		rowNumber =  e.srcElement.parentNode.rowIndex - 1;
	}else if(e.srcElement.parentNode.tagName == "TD"){
		rowNumber =  e.srcElement.parentNode.parentNode.rowIndex - 1;
	}
	//Revert to previous style
	myTBL.rows[rowNumber].cells[0].className = g_oldStyle;
	myTBL.rows[rowNumber].cells[1].className = g_oldStyle;
	myMessages.innerText = "";
	myMessages.style.visibility = "hidden";
}
//
// addToDataArray() function
//
function addToDataArray(ptName, paramName){
	//"dataShape" - an object (shape) in the Transition Popup
	//dataShape.autoReloadContent = false;
	for (var i = 1; i< 21;i++){
		tptName = dataShape.GetCustomProperty('point','pt'+i);
		tParam = dataShape.GetCustomProperty('Parameter', 'par'+i);
		if(tptName == ptName && tParam == paramName) return true;
		if(tptName == ''){
			dataShape.SetCustomProperty('point', 'pt'+i, ptName); 
			dataShape.SetCustomProperty('parameter', 'par'+i, paramName);
			//dataShape.ReloadContent();
			return true; 
		}
	}
	//if(myDebug) debugWrite('AddToDataArray: ', 'Adding Point ' + ptName + '.' + paramName);
	return false;
}
// BYPASS conditions
function myBypReqTrans(e){
	var srcElm = e.srcElement.id;
	var re = /\d{1,2}$/g;
	var CondNo = srcElm.match(re);
	oShape = document.getElementById('shape0'+('0'+CondNo[0]).slice(-2));
	tmpPointName = oShape.GetCustomProperty('point','pointName');
	tmpTransName = oShape.GetCustomProperty('value','transName');
	tmpTransIndex = oShape.GetCustomProperty('value','transIndex');
	tmpString = tmpPointName+"."+tmpTransName+".C["+tmpTransIndex+"]";

	if (e.srcElement.checked){
		oShape.Objects('data').SetDataValueSynchronous(tmpString+'.BYPPERM', 'ON');
	}else{
		oShape.Objects('data').SetDataValueSynchronous(tmpString+'.BYPPERM', 'OFF');
	}	
	//if(myDebug) debugWrite("AAA: ", oShape.Objects('data').DataValue(tmpString+'.BYPPERM'));	
}
function myBypReqChange(e){

	var srcElm = e.srcElement.id;
	var re = /\d{1,2}$/g;
	var CondNo = srcElm.match(re);
	oShape = document.getElementById('shape0'+('0'+CondNo[0]).slice(-2));
	tmpPointName = oShape.GetCustomProperty('point','pointName');
	tmpTransName = oShape.GetCustomProperty('value','transName');
	tmpTransIndex = oShape.GetCustomProperty('value','transIndex');

	tmpString = tmpPointName+"."+tmpTransName+".C["+tmpTransIndex+"]";

	switch(e.srcElement.value){
		case'-': oShape.Objects('data').SetDataValueSynchronous(tmpString+'.BYPREQ', 'None'); break;
		case'0': oShape.Objects('data').SetDataValueSynchronous(tmpString+'.BYPREQ', 'ToOff'); break;
		case'1': oShape.Objects('data').SetDataValueSynchronous(tmpString+'.BYPREQ', 'ToOn'); break;
	}
	//if(myDebug) debugWrite("Chng: ", e.srcElement.value+" "+ oShape.Objects('data').DataValue(tmpString+'.BYPREQ') );
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// <Confirmation Picker> popup support functions
//-----------------------------------------------------------------------------------------------------------------------------------------------
//
// confirmationPicker_pagecomplete()
// Confirmations Picker Popup onpagecomplete handler function
//
function confirmationPicker_pagecomplete(){
	var sv;
	var bRtn;
	var pointNames=[];
	try{

		myHash = 0;
		cHash = DisplayDataRepository.GetValue("cHash");
		for(i=1;i<=30;i++){
			pointNames[i] = DisplayDataRepository.GetValue("pt"+i);
			if(pointNames[i].length > 0){
				for(j = 0;j<pointNames[i].length;j++){
					myHash = myHash + parseInt(pointNames[i].toUpperCase().charCodeAt(j));
				}
			}
		}
		tmpStr = '';
		Initialize_Table(pointNames);
		Initialize_Grid(pointNames);
	}catch(e){ 
		df=testforError('pagecomplete ',e); 
	}
}
//
// updatePicker() function
//
function updatePicker(pointName, stepName, indices){
	var tmpFlag = clearRows(pointName, stepName);

	if(indices == "Clear"){
		return;
	}
	idxArray = indices.split(',');

	for(var i=0;i<idxArray.length;i++){
		tmpFlag = addRow(pointName, stepName, idxArray[i]);
	}
	if(myTable.firstChild.getElementsByTagName('TFOOT')[0].innerText.length > 0){
		myTable.firstChild.getElementsByTagName('TFOOT')[0].innerText = "";
	}
	//df = testforError("Update Picker", Err);
}
//
// clearRows() function
//
function clearRows(pointName, stepName) {
	//clearRows = false;
	var myBody = myTable.getElementsByTagName('TBODY')[0];

	for (var i = myBody.rows.length - 1; i >= 0; i--) {   
		if (myBody.rows[i].cells[0].innerHTML == pointName && myBody.rows[i].cells[1].innerHTML == stepName) {
			myBody.deleteRow(myBody.rows[i].sectionRowIndex);
		}
	}
	return true;
	//df = testforError("clearRows", Err);
}
//
// clearAll() function
//
function clearAll(pointName){
	var myBody = myTable.getElementsByTagName('TBODY')[0];
	for(var i = myBody.rows.length - 1 ;i>=0; i--){
		if(myBody.rows[i].cells[0].innerHTML == pointName && myBody.rows[i].cells[1].innerHTML!="ListStart"){
			myBody.deleteRow(myBody.rows[i].sectionRowIndex);
		}
	}
	//df = testforError("clearAll", Err);
}
//
// addRow() function
//
function addRow(pointName, stepName, idx){
	//addRow = false
	for(var i = 0;i<= myTable.firstChild.rows.length - 1;i++){
		if(myTable.firstChild.rows[i].cells[0].innerHTML == pointName){
			var newRow = myTable.firstChild.insertRow(i+1);
			newRow.title = pointName + ":" + stepName + ":" + idx;
			newRow.attachEvent('onclick' , myClick);
			newRow.attachEvent('onmouseover' , myMouseOver);
			newRow.attachEvent('onmouseout' , myMouseOut);
			//newRow.style.display = "";
			newRow.style.fontSize = "10px";
			newRow.className = "Alert_Pending";
			var oCell = document.createElement("TD");
			oCell.innerHTML = pointName;
			oCell.className = "Alert_Pending";
			newRow.appendChild(oCell);
			oCell = document.createElement("TD");
			oCell.innerHTML = stepName;
			oCell.className = "Alert_Pending";
			newRow.appendChild(oCell);
			oCell = document.createElement("TD");
			oCell.className = "Alert_Pending";
			oCell.innerHTML = idx;
			newRow.appendChild(oCell);
			//df = testforError("addRow loop", Err);
			return true;
		}
	}
	//df = testforError("addRow", Err);
}
//
// Initialize_Grid() function
//
function Initialize_Grid(pNames) {
	var i, tmpPointName, oShape, currPointName;    
	for (i = 1; i <= 30; i++) {
		tmpPointName = pNames[i];
		if ((typeof(tmpPointName) != 'undefined') && (tmpPointName.length > 0)) {
			x=Page.children.tags('DIV');
			for (j=0;j<x.length;j++ ) {
				if (typeof(x[i].id) != 'undefined'){
					if (x[j].id && x[j].id.indexOf("shape") >= 0) {
						currPointName = x[j].GetCustomProperty("point", "pointName");
						if (currPointName.length === 0) {
							x[j].SetCustomProperty("point", "pointName", tmpPointName);
							break;
						}
					}
				}
			}
		}
	}
	//df = testforError("Initialize_Grid", Err);
}
//
// Initialize_Table() function
//
function Initialize_Table(pNames) {

	var oTable = document.createElement("TABLE");
	var oHead = document.createElement("THEAD");
	var oBody = document.createElement("TBODY");
	var oFoot = document.createElement("TFOOT");
	var oCaption = document.createElement("CAPTION");
	var oRow, oCell, i, j, heading = [], percents = [], stock = new Array(30);

	heading[0] = "Procedures";
	stock[0]=[];
	stock[1]=[];
	stock[2]=[];
	
	percents[0] = '70%';
	percents[1] = '25%';
	percents[2] = '5%';
	
	for(i = 1; i < 31; i++) {
		//stock(0,i) = DisplayDataRepository.GetValue("pt"&i)
		stock[0][i] = pNames[i];
		stock[1][i] ="ListStart";
		stock[2][i] = "";
	}

	oTable.appendChild(oHead);
	oTable.appendChild(oBody);
	oTable.appendChild(oFoot);
	oTable.appendChild(oCaption);

	oTable.id = "ebmStepTable";

	oRow = document.createElement("TR");
	oHead.appendChild(oRow);

	for(i = 0; i <1; i++) {
		oCellH = document.createElement("TH");
		oCellH.innerHTML = heading[i];
		oCellH.style.width = '380px';
		oCellH.colspan = '3';
		oRow.appendChild(oCellH);
	}
	//Add rows for each pending instruction
	for(i = 1; i <= 30; i++){
		oRow = document.createElement("TR");
		oRow.title = stock[0][i];
		// Add event handlers
		oRow.attachEvent('onclick' , myClick);
		//oRow.attachEvent('onmouseover' , myMouseOver);
		//oRow.attachEvent('onmouseout' , myMouseOut);
		oRow.style.display = "none";
		oRow.style.visibility = "hidden";
		oRow.style.fontSize = "10px";
		oBody.appendChild(oRow);
		oRow.className = "Alert_Pending";
		for(j = 0; j<= 2; j++){
			oCell = document.createElement("TD");
			oCell.innerHTML = stock[j][i];
			//oCell.width = percents[j];
			oRow.appendChild(oCell);
LogMsg("Confirm Table -> rowIndex = " + oRow.rowIndex);	
			//oCell.className = "Alert_Pending";
		}
	}
	//Add end row (internal use)
	oRow = document.createElement("TR");
	oRow.title = "END";
	// Here will add the connection to the event
	oRow.attachEvent('onclick' , myClick);
	oRow.attachEvent('onmouseover' , myMouseOver);
	oRow.attachEvent('onmouseout' , myMouseOut);
	oRow.style.display = "none";
	oRow.style.fontSize = "10px";
	oBody.appendChild(oRow);
	for (j = 0; j <= 2; j++) {
		oCell = document.createElement("TD");
		oCell.innerHTML = "END";
		oRow.appendChild(oCell);
	}

	//oBody.setAttribute("bgColor", "#FFFACD"); //"lemonchiffon"
	oRow = document.createElement("TR");
	oFoot.appendChild(oRow);
	oCell = document.createElement("TD");


	oCell.innerHTML = "Working Please Wait... Updating List";
	oCell.colSpan = heading.length.toString(); //"4";
	oCell.width = "100%";
	oCell.bgColor = "#87CEFA";
	oRow.appendChild(oCell);
	oCaption.innerHTML = "Click on table row to invoke confirmation dialog";
	oCaption.style.fontSize = "10px";
	oCaption.align = "bottom";
	myTable.appendChild(oTable);
	document.getElementsByTagName('TH')[0].colSpan="3";
	//df = testforError("Initialize_Table", Err);
}
//
// myClick() handler - Confirmation Picker popup
//
function myClick(){
	tmpStr = window.event.srcElement.parentElement.title;
	
	tmp1Array = tmpStr.split(':');
	//df = testforError("myClick A", Err);

	tmpPointName = tmp1Array[0];
	tmpStepName = tmp1Array[1];
	tmpStepNumber = tmp1Array[2];
	if(tmpPointName.length < 1 || tmpStepName.length < 1 || tmpStepNumber.length < 1){ 
		return;
	}

	var myCheck = ChangeDSDTable(tmpPointName,tmpStepName,tmpStepNumber);
	if(!myCheck) { 
 		window.external.application.TemporaryMessageZoneText = "Alerts associated with " + tmpPointName + "." + tmpStepName + ".OP["+ tmpStepNumber+ "]" + " will not be acknowledged!";
	}
	//df = testforError("myClick B", Err);
	tmpAlertPopupName = DisplayDataRepository.GetValue('AlertConfirmDisplay');	
	if(!tmpAlertPopupName||tmpAlertPopupName.length<1){tmpAlertPopupName='seqShpAlertConfirm.htm';}
	popupCommand = tmpAlertPopupName+'?cPoint='+tmpPointName+'&myStepName='+tmpStepName+'&myStepIndex='+tmpStepNumber;

	var sv, AlertL, AlertLFocus, AlertR, AlertRFocus;
	//sv = new ActiveXObject("honeywell.workspace.client");
	//df = testforError("myClick C", Err);
	asFacePlate = 0;
	//if(sv.IsWindowManaged(hwnd)){
	//	asFacePlate = 1;
	//}
	//df = testforError("myClick D", Err);
	HDXPopupBehavior.CreatePopupWindow2(popupCommand, 0, 0, asFacePlate, false);
	//df = testforError("myClick", Err);
}
//
// myMouseOver() handler - Confirmation/Alert Picker popup
//
function myMouseOver(e){
	var rowNumber;
	var aTable = document.getElementById('ebmStepTable').tBodies[0];
	if(e.srcElement.parentNode.tagName == "TR"){
		rowNumber =  e.srcElement.parentNode.rowIndex - 1;
	}else if(e.srcElement.parentNode.tagName == "TD"){
		rowNumber =  e.srcElement.parentNode.parentNode.rowIndex - 1;
	}
	//g_oldStyle = aTable.rows[rowNumber].cells[i].className;
	for(i = 0;i< aTable.rows[rowNumber].cells.length;i++){
		aTable.rows[rowNumber].cells[i].className = "Alert_Hover";
	}
	//aTable.rows[rowNumber].style.cursor = "hand";
	//df = testforError("myMouseOver", Err);
}
//
// myMouseOut() handler - Confirmation Picker/Alert popup
//
function myMouseOut(e){
	var rowNumber;
	var aTable = document.getElementById('ebmStepTable').tBodies[0];
	if(e.srcElement.parentNode.tagName == "TR"){
		rowNumber =  e.srcElement.parentNode.rowIndex - 1;
	}else if(e.srcElement.parentNode.tagName == "TD"){
		rowNumber =  e.srcElement.parentNode.parentNode.rowIndex - 1;
	}
	for(i = 0;i< aTable.rows[rowNumber].cells.length;i++){
		aTable.rows[rowNumber].cells[i].className = g_oldStyle;
	}
	aTable.rows[rowNumber].style.cursor = "default";
	//df = testforError("myMouseOut", Err);
}
// 
//	getStepAndIndex2_ondatachange
//
function getStepAndIndex2_ondatachange(oSource) {
	var oShape = oSource.parentElement.parentElement;
	var pointName = oShape.GetCustomProperty('point', 'pointName');
	var i;
	if (oSource.DataValue('pointName.INSTRUCTPEND') == 0) {
		clearAll(pointName);
	}

	var tmpArray = oShape.objects('shape001').id.split("_");
	var objName = tmpArray[0];

	for (i = 1; i <= 10; i++) {
		var  tmpString = oSource.DataValue("pointName.ACTIVELOC.STEP[" + i.toString() + "]");
		var numStr = "00" + i.toString();
		numStr = numStr.slice(-3);
		var tmpObject = objName + "_shape" + numStr;
		var myObj = document.getElementById(tmpObject);
		//myObj.autoReloadContent = false;
LogMsg("");	
		if(typeof (tmpString) != "string") {break;}
		tmpArray = tmpString.split(".");
		pointNameDel = myObj.GetCustomProperty("point", "pointName");
		stepNameDel = myObj.GetCustomProperty("value", "stepName");
		if(pointNameDel==tmpArray[0]&&stepNameDel!=tmpArray[1]){
			clearRows(pointNameDel,stepNameDel);
		}
		if (tmpString.length > 0) {
			myObj.SetCustomProperty("point", "pointName", tmpArray[0]);
			myObj.SetCustomProperty("value", "stepName", tmpArray[1]);
			myObj.SetCustomProperty("parameter", "stpINSTRUCTPEND", tmpArray[1] + ".INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpNUMOUTPUTS", tmpArray[1] + ".NUMOUTPUTS");
			myObj.SetCustomProperty("parameter", "stpOP1INSTPEND", tmpArray[1] + ".OP[1].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP2INSTPEND", tmpArray[1] + ".OP[2].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP3INSTPEND", tmpArray[1] + ".OP[3].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP4INSTPEND", tmpArray[1] + ".OP[4].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP5INSTPEND", tmpArray[1] + ".OP[5].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP6INSTPEND", tmpArray[1] + ".OP[6].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP7INSTPEND", tmpArray[1] + ".OP[7].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP8INSTPEND", tmpArray[1] + ".OP[8].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP9INSTPEND", tmpArray[1] + ".OP[9].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP10INSTPEND", tmpArray[1] + ".OP[10].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP11INSTPEND", tmpArray[1] + ".OP[11].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP12INSTPEND", tmpArray[1] + ".OP[12].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP13INSTPEND", tmpArray[1] + ".OP[13].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP14INSTPEND", tmpArray[1] + ".OP[14].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP15INSTPEND", tmpArray[1] + ".OP[15].INSTRUCTPEND");
			myObj.SetCustomProperty("parameter", "stpOP16INSTPEND", tmpArray[1] + ".OP[16].INSTRUCTPEND");
		} else {
			myObj.SetCustomProperty("point", "pointName", "");
			myObj.SetCustomProperty("value", "stepName", "");
			myObj.SetCustomProperty("parameter", "stpINSTRUCTPEND", "");
			myObj.SetCustomProperty("parameter", "stpNUMOUTPUTS", "");
			myObj.SetCustomProperty("parameter", "stpOP1INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP2INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP3INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP4INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP5INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP6INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP7INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP8INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP9INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP10INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP11INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP12INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP13INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP14INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP15INSTPEND", "");
			myObj.SetCustomProperty("parameter", "stpOP16INSTPEND", "");
		}
	//myObj.ReloadContent();
	}
}
//
//	getStepIndex2_ondatachange
//
function getStepIndex2_ondatachange(oSource) {
	var idx = "Clear";
	var oShape = oSource.parentElement;
	var tmpPointName = oShape.GetCustomProperty("point", "pointName");
	var tmpStepName = oShape.GetCustomProperty("value", "stepName");
	var i, maxo;

	if (oSource.DataValue("pointName.stpINSTRUCTPEND")) { idx = ""; }

	for (i = 1, maxo = oSource.DataValue("pointName.stpNUMOUTPUTS"); i <= maxo; i++) {
		if (oSource.DataValue("pointName.stpOP" + i + "INSTPEND") == 1) {
			if (idx == '') {
				idx = i.toString();
			} else {
				idx = idx + ',' + i.toString();
			}
		}
	}
	updatePicker(tmpPointName, tmpStepName, idx);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// <Alert Confirmation> popup support functions
//-----------------------------------------------------------------------------------------------------------------------------------------------
//
// alertConfirm_pagecomplete() function
// Alert Confirmation popup onpagecomplete handler
//
function alertConfirm_pagecomplete(){
	var tmpString;
	var mPointName = DisplayDataRepository.GetValue('cPoint');
	var mStepName = DisplayDataRepository.GetValue('myStepName');
	var mStepNumber = DisplayDataRepository.GetValue('myStepIndex');
	document.title += " - " + mPointName + '.' + mStepName;
	var oTitle = document.getElementById("txtTitle");
	if(oTitle){
		if(ConAlpDESC.value != ""){
			txtTitle.innerText = ConAlpDESC.value;
		}else{
			txtTitle.innerText = mPointName+'.'+mStepName;
		}
	}
	var mStepNameOP = mStepName+'.OP['+mStepNumber+']';
	tmpString = mStepName + '.OP[' + mStepNumber + '].INSTRUCTCNFM';
	DisplayDataRepository.PutValue('StepOPConfirm', tmpString);
	// Set additional Display Custom Properties for TARGET/CURRENT/ENTRY
	tmpString = mStepName + '.OP[' + mStepNumber + '].INSTRUCTION';
	DisplayDataRepository.PutValue('stepOPInstruction', tmpString);
	DisplayDataRepository.PutValue('stepOP', mStepNameOP);
	DisplayDataRepository.PutValue('stepOPTargetValRef', mStepNameOP+'.TARGETVALREF');
	DisplayDataRepository.PutValue('stepOPTargetChange', mStepNameOP+'.TARGETCHANGE');
	DisplayDataRepository.PutValue('stepOPCurrentValRef', mStepNameOP+'.CURRVALREF');
	DisplayDataRepository.PutValue('stepOPEntryValRef', mStepNameOP+'.ENTRYVALREF');
	 //Init ACTENTITY binding if point is Activity
	if(mPointName.indexOf("$")>-1){
		DisplayDataRepository.PutValue('ACTENTITY', "ACTENTITY");
	}
	//Changes based on Wolfgang Kammerer comments 2016-07-26 and do not work
	//DisplayDataRepository.PutValue('stepOPTargetChange', mStepNameOP+'.TARGETCHANGE');
}
//
// sd_alert_onupdate()
// Alert Confirm popup reference values init function
//
function sd_alert_onupdate(oSource){
	var currMRName, tmpData, ppos, refPntName, refParamName;
	var o = document.getElementById("myInstrBox");
	o.className = "txtInstruction";
	o.innerHTML = oSource.datavalue("cPoint.stepOPInstruction");
	var alpVisible=false;
	var currPntName = DisplayDataRepository.GetValue("cPoint");
	//Check for activity references
	if(oSource.QualityGood("cPoint.ActEntity")){
		currMRName = oSource.DataValue("cPoint.ActEntity");
	}else{
		currMRName = "";
	}
	// Check TARGETVALREF is configured
	if(oSource.dataExists("cPoint.stepOPTargetValRef")){
		var iTargetChange;
		tmpData = oSource.dataValue("cPoint.stepOPTargetValRef").toString();
		var alpEntryTarget = document.getElementById("ConAlpTargetValChange");
		var alpEntryTxtTarget = document.getElementById("ConAlpTargetValTxtChange");
		var cboEntryTarget = document.getElementById("ConCboTargetValChange");
		var alpTarget = document.getElementById("ConAlpTargetVal");
		//Added TARGETCHANGE option
		var alpTargetChange = document.getElementById("ConAlpTargetValChange");
		if(tmpData!="" && tmpData != "NaN"){
			ppos = tmpData.indexOf('.');
			refPntName = tmpData.slice(0,ppos);
			refParamName = tmpData.slice(ppos+1);
			if( currMRName.toUpperCase() == refPntName.toUpperCase() ){ refPntName = currPntName;}
			
			DisplayDataRepository.PutValue("stepOPTargetPnt",refPntName);
			DisplayDataRepository.PutValue("stepOPTargetVal",refParamName);
			//Enable TARGETVAL edit if TARGETCHANGE is enabled
			if ( oSource.dataExists("cPoint.stepOPTargetChange")) {
				iTargetChange = oSource.dataValue("cPoint.stepOPTargetChange");
			}
			if (iTargetChange==1) {
				//alpTargetChange.style.visibility = "inherit";
				//alpTarget.style.visibility = "hidden";
				cboEntryTarget.attachEvent("onupdate",alpEntryTarget_update);
				alpVisible = true;
			}else{
				alpTargetChange.style.visibility = "hidden";
				alpTarget.style.visibility = "inherit";
				alpVisible = true;
			}
		}else{
			alpTarget.style.visibility = "hidden";
			alpEntryTarget.style.visibility = "hidden";
			alpEntryTxtTarget.style.visibility = "hidden";
			cboEntryTarget.style.visibility = "hidden";
		}
	}
	// Check CURRENTVALREF is configured
	if(oSource.dataExists("cPoint.stepOPCurrentValRef")){
		tmpData = oSource.dataValue("cPoint.stepOPCurrentValRef").toString();
		var alpCurrent = document.getElementById("ConAlpCurrentVal");
		if(tmpData!="" && tmpData != "NaN"){
			ppos = tmpData.indexOf('.');
			refPntName = tmpData.slice(0,ppos);
			refParamName = tmpData.slice(ppos+1);
			if( currMRName.toUpperCase() == refPntName.toUpperCase() ){ refPntName = currPntName;}
			DisplayDataRepository.PutValue("stepOPCurrentPnt",refPntName);
			DisplayDataRepository.PutValue("stepOPCurrentVal",refParamName);
			alpCurrent.style.visibility = "inherit";
			alpVisible = true;
		}else{	
			alpCurrent.style.visibility = "hidden";
		}
	}
	// Check ENTRYVALREF is configured
	if(oSource.dataExists("cPoint.stepOPEntryValRef")){
	
		tmpData = oSource.dataValue("cPoint.stepOPEntryValRef").toString();
		var alpEntry = document.getElementById("ConAlpEntryVal");
		var alpEntryTxt = document.getElementById("ConAlpEntryValTxt");
		var cboEntry = document.getElementById("ConCboEntryVal");

		if(tmpData !="" && tmpData != "NaN"){
			ppos = tmpData.indexOf('.');
			refPntName = tmpData.slice(0,ppos);
			refParamName = tmpData.slice(ppos+1);

			if( currMRName.toUpperCase() == refPntName.toUpperCase() ){ refPntName = currPntName;}
		
			DisplayDataRepository.PutValue("stepOPEntryPnt",refPntName);
			DisplayDataRepository.PutValue("stepOPEntryVal",refParamName);
			//alpEntry.attachEvent("onupdate",alpEntry_update);
			cboEntry.attachEvent("onupdate",alpEntry_update);
			alpVisible = true;
		}else{
			alpEntry.style.visibility = "hidden";
			cboEntry.style.visibility = "hidden";
			alpEntryTxt.style.visibility = "hidden";
		}
	}
	// Hide all fields if no REF is configured
	if(!alpVisible){
		document.getElementById("grpValues").style.visibility = "hidden";
	}else{	
		document.getElementById("grpValues").style.visibility = "inherit";
	}
}
//
// alpEntry_update()
//	Alert Confirm popup - show/hide ENTRYVAL alpha/combo - executes on alpha/combo update.
//
function alpEntry_update(){
	var alpEntry,cboEntry,alpEntryTxt;
	var o = window.event.srcElement;
	//Get Entry objects
	if(o.id.toUpperCase()=="CONALPENTRYVAL"){
		alpEntry = o;
		cboEntry = document.getElementById("ConCboEntryVal");
		alpEntryTxt = document.getElementById("ConAlpEntryValTxt");
	}else{
		cboEntry = o;
		alpEntry = document.getElementById("ConAlpEntryVal");
		alpEntryTxt = document.getElementById("ConAlpEntryValTxt");
	}
	// Auto select Alpha/Combo (analog/status)
	if(cboEntry.innerText.indexOf(alpEntry.innerText)>-1 && alpEntry.innerText != ""){
		//Entry is numeric - show alpha displayed as "Numeric"
		alpEntry.style.visibility = "inherit";
		alpEntryTxt.style.visibility = "hidden";
		cboEntry.style.visibility = "hidden";
	}else{
		if(alpEntry.value === ""){
			//Entry is string (numeric alpha shows empty string) - show alpha displayed as "State Descriptor".
			alpEntry.style.visibility = "hidden";
			alpEntryTxt.style.visibility = "inherit";
			cboEntry.style.visibility = "hidden";
		}else{
			//Entry is Enumeration - show combobox.
			alpEntry.style.visibility = "hidden";
			alpEntryTxt.style.visibility = "hidden";
			cboEntry.style.visibility = "inherit";
		}
	}
	if(alpEntry.innerText == "NaN"){
		alpEntry.style.visibility = "inherit";
		alpEntryTxt.style.visibility = "hidden";
		cboEntry.style.visibility = "hidden";
	}
}
//
// alpEntryTarget_update()
//	Alert Confirm popup - show/hide ENTRYVAL alpha/combo - executes on alpha/combo update.
//
function alpEntryTarget_update(){
	var alpEntry,cboEntry,alpEntryTxt;
	var o = window.event.srcElement;
	//Get Entry objects
	if(o.id.toUpperCase()=="CONALPTARGETVALCHANGE"){
		alpEntry = o;
		cboEntry = document.getElementById("ConCboTargetValChange");
		alpEntryTxt = document.getElementById("ConAlpTargetValTxtChange");
	}else{
		cboEntry = o;
		alpEntry = document.getElementById("ConAlpTargetValChange");
		alpEntryTxt = document.getElementById("ConAlpTargetValTxtChange");
	}
	// Auto select Alpha/Combo (analog/status)
	if(cboEntry.innerText.indexOf(alpEntry.innerText)>-1 && alpEntry.innerText != ""){
		//Entry is numeric - show alpha displayed as "Numeric"
		alpEntry.style.visibility = "inherit";
		alpEntryTxt.style.visibility = "hidden";
		cboEntry.style.visibility = "hidden";
	}else{
		if(alpEntry.value === ""){
			//Entry is string (numeric alpha shows empty string) - show alpha displayed as "State Descriptor".
			alpEntry.style.visibility = "hidden";
			alpEntryTxt.style.visibility = "inherit";
			cboEntry.style.visibility = "hidden";
		}else{
			//Entry is Enumeration - show combobox.
			alpEntry.style.visibility = "hidden";
			alpEntryTxt.style.visibility = "hidden";
			cboEntry.style.visibility = "inherit";
		}
	}
	if(alpEntry.innerText == "NaN"){
		alpEntry.style.visibility = "inherit";
		alpEntryTxt.style.visibility = "hidden";
		cboEntry.style.visibility = "hidden";
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// General support/utility functions
//-----------------------------------------------------------------------------------------------------------------------------------------------
//
//	findCSSFile
//	This function finds a CSS sheet based on the passed cssFileName else it returns a null.
//
function findCSSFile(cssFileName){
	var tmpString, re, i, j;
	tmpString = "[\\,\/]" + cssFileName + ".*\\.css";
	re = new RegExp(tmpString,'ig');
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
	return "";
}
//
// popupClose() function - used in "seqShpAlertConfirm" popup
//
function popupClose(){
	try {
			window.external.close();
		} catch(err) {
			try {
				var sv = new ActiveXObject("Honeywell.Workspace.Client");
				var hwnd = window.parent.external.application.appwindow.hwnd;
				sv.CloseWindow(hwnd);
			}catch(e){
				if (window.external.Application.environment == 0){window.external.Application.LogMessage(window.external.Application.currentpage + '>> Error found, error number = ' + e.number + ' Description = ' + e.description);}
			}
	}
}
//
function debugWrite(myCallString, myString) {
	var d, s;
	var c = ":";
	d = new Date();
	s = " " + d.getHours() + c;
	s += d.getMinutes() + c;
	s += d.getSeconds() + c;
	s += d.getMilliseconds() + " ";
	if(myErrorTable == document.getElementById('myErrorTable')){
		myErrorTable.innerHTML = myErrorTable.innerHTML + myCallString + s +"<br>"; //+ myString.substring(0,500)
	} else {
		myDiv = document.createElement('div');
		myDiv.id = 'myErrorTable';
		myDiv.style.position = 'relative';
		myDiv.style.overflow = 'auto';
		myDiv.style.border = '1px solid red';
		myDiv.style.left = '0%';
		myDiv.style.backgroundColor = '#808080';
		myDiv.style.top = '70%';
		myDiv.style.width = '80%';
		myDiv.style.height = '30%';
		document.body.appendChild(myDiv);
		myDiv.innerHTML = myCallString + s + myString +"<br>"; //.substring(0,500)
	}
}
//
// testforError() function - used for debugging
//
function testforError(Name, e){
	if(myDebug && e.number != 0){
		if (window.external.Application.environment == 0){window.external.Application.LogMessage(window.external.Application.currentpage + ">> Function:" + Name + " Error number:" + e.number + " Description: " + e.description);}
		return true;
	}else{
		return false;
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
if(window.external.Application){window.external.Application.logmessage(window.external.Application.currentpage + " : >>> ebmPopupGSH.js parsed - Sequence Shape Popups release : " + SEQPP_RELEASE + " version date: " + SEQPP_VERSION_DATE);}