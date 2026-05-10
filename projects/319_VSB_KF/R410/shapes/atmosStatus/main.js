define(
[
    './AtmosStatusBinding',
    'text!./AtmosStatusTemplate.htm'
],
function(AtmosStatusBinding, AtmosStatusTemplate) {

    var ShapeModule = {
        _config: {}
    };

    ShapeModule.init = function(config) {
        ShapeModule._config = config;
        return true;
    }

    ShapeModule.getViewTemplates = function() {
        return {
            extAtmosStatusTemplate: AtmosStatusTemplate
        };
    }

    ShapeModule.getBindings = function() {
        AtmosStatusBinding.shapesPath = ShapeModule._config.shapesPath;
        return {
            extAtmosStatus: AtmosStatusBinding
        }
    }

    ShapeModule.getStyleSheets = function() {
        return [
            ShapeModule._config.shapesPath + '/atmosStatus/AtmosStatusStyle.css'
        ];
    }

    ShapeModule.createViewModel = function() {
        return 'PointParamViewModel';
    }

    return ShapeModule;
});
