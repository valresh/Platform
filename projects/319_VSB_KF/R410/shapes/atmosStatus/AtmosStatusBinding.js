define(
[
    'knockout',
    'jquery',
    'asari/util/QualityRendering'
],
function(ko, $, QualityRendering) {

    var AtmosStatusBinding = {
        shapesPath: null
    }

    AtmosStatusBinding.init = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
    }

    AtmosStatusBinding.update = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {

        var config = valueAccessor();
        var value = ko.utils.unwrapObservable(config.value);
        var quality = ko.utils.unwrapObservable(config.quality);
        var $elem = $(element);

        var renderInfo = QualityRendering.getRenderInfo({value: value, quality: quality});
        if (renderInfo.state === QualityRendering.STATE_HIDDEN)
        {
            $elem.hide();
        }
        else
        {
            $elem.show();

            // Map the value to an image to display.
            // Assumes that the data type of the value is an integer.
            var imgFile = null;
            switch (value) {
                case 0: imgFile = 'fail.png'; break;
                case 1: imgFile = 'ok.png'; break;
                default: imgFile = 'unknown.png'; break;
            }

            var path = AtmosStatusBinding.shapesPath + '/atmosstatus/' + imgFile;
            $elem.attr('src', path)
        }
    }

    return AtmosStatusBinding;
});