// ## Status Binding

define(
[
    'knockout'
],
function(ko) {

    var StatusBinding = {
        shapesPath: null
    }

    StatusBinding.init = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
    }

    StatusBinding.update = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {

      var bindingConfig = valueAccessor();
        var status = ko.utils.unwrapObservable(bindingConfig.value1.source.value);
        var description = ko.utils.unwrapObservable(bindingConfig.value2.source.value);

        // Map the value to an image to display.
        // Assumes that the data types of the value is an integer.
        var imgFile = null;
        switch (status) {
      case 0:    
        imgFile = 'SSDOK.png'; 
        break;
      case 2:
      case 3:
      case 7:
      case 8:
      case 11:
        imgFile = 'SSDMarginal.png';
        break;
      case 5:
      case 6: 
      case 9:
        imgFile = 'SSDFailed.png';  
        break;
      case 10:
        imgFile = 'SSDDisabled.png';
        break;
      default:     
        imgFile = 'SSDUnknown.png';  
        break;
        }

    var path = StatusBinding.shapesPath + '/calcstatus/' + imgFile;
        element.src = path;
    element.title = description;
    }

    return StatusBinding;
});