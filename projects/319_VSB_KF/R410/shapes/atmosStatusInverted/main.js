define(
[
    './AtmosStatusInvertedBinding',
    'text!./AtmosStatusInvertedTemplate.htm'
],
function(AtmosStatusInvertedBinding, AtmosStatusInvertedTemplate) {

    var ShapeModule = {
        _config: {}
    };

    ShapeModule.init = function(config) {
        ShapeModule._config = config;
        return true;
    }

    ShapeModule.getViewTemplates = function() {
        return {
            extAtmosStatusInvertedTemplate: AtmosStatusInvertedTemplate
        };
    }

    ShapeModule.getBindings = function() {
        AtmosStatusInvertedBinding.shapesPath = ShapeModule._config.shapesPath;
        return {
            extAtmosStatusInverted: AtmosStatusInvertedBinding
        }
    }

    ShapeModule.getStyleSheets = function() {
        return [
            ShapeModule._config.shapesPath + '/atmosStatusInverted/AtmosStatusInvertedStyle.css'
        ];
    }

    ShapeModule.createViewModel = function() {
        return 'PointParamViewModel';
    }

    return ShapeModule;
});
