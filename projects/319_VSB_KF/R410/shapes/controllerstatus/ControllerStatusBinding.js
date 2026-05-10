define(
[
    'knockout',
    'asari/util/QualityRendering'
],
function(ko, QualityRendering) {

    var ControllerStatusBinding = {
        shapesPath: null
    }

    ControllerStatusBinding.init = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
    }

    ControllerStatusBinding.update = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {

        var config = valueAccessor();
        var value = ko.utils.unwrapObservable(config.value);
        var quality = ko.utils.unwrapObservable(config.quality);

        var renderInfo = QualityRendering.getRenderInfo({value: value, quality: quality});
        if (renderInfo.state === QualityRendering.STATE_HIDDEN)
        {
            element.display = 'none';
        }
        else
        {
            element.display = '';

            // Map the value to an image to display.
            // Assumes that the data type of the value is an integer.
            var imgFile = null;
            switch (value) {
                case 12: imgFile = 'led_green.svg'; break;
                case 13: imgFile = 'led_yellow.svg'; break;
                case 14: imgFile = 'led_red.svg'; break;
                case 15: imgFile = 'led_darkgreen.svg'; break;
                default: imgFile = 'led_black.svg'; break;
            }

            var path = ControllerStatusBinding.shapesPath + '/controllerstatus/' + imgFile;
            element.setAttribute("src",path);
        }
    }

    return ControllerStatusBinding;
});
