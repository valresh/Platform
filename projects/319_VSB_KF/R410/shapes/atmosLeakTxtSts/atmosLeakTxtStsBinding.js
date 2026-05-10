// ## Status Binding

define(
[
    'knockout'
],
function(ko) {

  var atmosLeakTxtStsBinding = {
    shapesPath: null
  }

  atmosLeakTxtStsBinding.init = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
  }

  atmosLeakTxtStsBinding.update = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {

    var config = ko.utils.unwrapObservable(valueAccessor());
    var value = ko.utils.unwrapObservable(config.value);
    var pntprmflag = ko.utils.unwrapObservable(config.pntprmflag);
    var imgFile = 'SSDUnknown.png';

    if (pntprmflag === true)
    {
	    if (value >= 0 && value <= 7) 
        imgFile = 'blank.png';   
	    if (value >= 8 && value <= 11) 
        imgFile = 'SSDDisabled.png';
	    if (value == 12) 
	      imgFile = 'SSDOK.png'; 
	    if (value == 13) 
        imgFile = 'SSDMarginal.png'; 
	    if (value >= 14 && value <= 15) 
	      imgFile = 'SSDFailed.png';
    }
    else
    {
      // Map the value to an image to display.
      // Assumes that the data types of the value is an integer.
      switch (value) {
            case 0:      imgFile = 'SSDOK.png';  break;
            case 1:      imgFile = 'Warning.png';  break;
            case 2:      imgFile = 'Detected.png';  break;
            case 3:      imgFile = 'SSDDisabled.png';  break;
            default:     imgFile = 'SSDUnknown.png';  break;
      }
    }
    
    //var path = StatusBinding.shapesPath + '/status/' + imgFile;
    var path = atmosLeakTxtStsBinding.shapesPath + '/atmosLeakTxtSts/' + imgFile;
    element.src = path;
  }

  return atmosLeakTxtStsBinding;
});