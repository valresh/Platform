// ## Binding

define(
[
    'knockout',
	'asari/util/AsariFunctionalLogger'
],
function(ko,log) {

    var Binding = {
        shapesPath: null
    }

    Binding.init = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
    }

    Binding.update = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {

        var config = ko.utils.unwrapObservable(valueAccessor());
		log.error("Got to here 1 " + ko.toJSON(config));
        log.error("Got to here 2 " + ko.toJSON(viewModel));
        var value = ko.utils.unwrapObservable(config.value);
        var imagelist = ko.utils.unwrapObservable(config.imagelist);
		var imgFile = '';

		if ((imagelist != null) && (value != null))
		{
			imagelist = imagelist.split(",");
			if (value < imagelist.length)
			{
				imgFile = imagelist[value];
			}
		}
        log.error("Got to here 3 " + imgFile);
		if (imgFile === '')
		{
			element.display = 'none';
		}
		else
		{
			element.display = '';
			var path = Binding.shapesPath + '/ImageTxt/' + imgFile;
      element.setAttribute('src', path)
		}
    }

    return Binding;
});
