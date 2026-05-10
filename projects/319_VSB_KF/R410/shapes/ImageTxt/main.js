// ## Shape Module Main Entry Point

// ### Background
// A shape module consists of:
//
// 1. A `main.js` Javascript file (mandatory) which implements the defined shape interface.
// 2. A view template (mandatory), a HTML snippet which defines the visualisation of the shape.
// 3. A view model (mandatory), which transforms cell data obtained from the server into observable properties,
//    which can be bound to the view template.  A shape module may provide its own view model implementation or
//    elect to use a system-defined view model, such as the `PointParamViewModel` class.
// 4. Custom Knockout binding handlers which bind the view template to the view model.
// 5. Other resources such as images, svg files, etc.
//
// The shape module must adhere to the following in order for the framework to locate and load it:
//
// 1. The shape's `main.js` file must be located at: `<Station search path>\shapes\<Shape name>\main.js`.
// 2. `<Station search path>` must be a directory listed under the Station connection's search paths.
// 3. `<Shape name>` must match exactly the `elementType` field of the column definition in the view definition.
//
// ### Example
// This example is a shape which uses the system-defined `PointParamViewModel` class as the view model.
// It implements a custom Knockout binding to set the source URI of an image in the view template based
// on the view model's `value` property.

// ### Module Definition
// All javascript files must be AMD modules in order to be loaded by RequireJS. For more information
// see: [RequireJS documentation](http://requirejs.org).
define(
// Specify dependencies, i.e. all javascript files and text files that must be loaded before this AMD module.
// Framework libraries and utilities can be specified as dependencies, e.g. `jquery`, `knockout`.
[
    // Javascript dependencies do not require the file extension.  The dependency's path must be relative to
    // the path of the current file and include a `./` prefix.
	'./Binding', 
    // Text files can be loaded using the RequireJS `text` plugin.  The `text!` prefix and the file extension
    // are both required.  The content of the file will be available as a string.
    'text!./Template.htm', 
	'asari/viewmodels/PointParamViewModel',
	'./ExtViewModel'
],
// Dependencies will be available as function arguments.
function(Binding, Template, PointParamViewModel, ExtViewModel) {
  var __hasProp = {}.hasOwnProperty,
    __extends = function(child, parent) { for (var key in parent) { if (__hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; },
    __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; };
	
    // ### Initialisation
    var Shape = {
        // The shape config will be set in the init() function.
        _config: {}
    };

    // ### init() function
    // Initialise the shape module and return a boolean indicating success or failure. This
    // function will be called once upon registration of the shape module.
    //
    // The config object will consist of the following properties:
    //
    // * `shapesPath`: The absolute path of the parent shapes directory, i.e. `<Station search path>\shapes`.
    Shape.init = function(config) {
        Shape._config = config;
        return true;
    };

    // ### getViewTemplates() function
    // Return a map of `<View template name>, <HTML template as a string>` to be injected as
    // script blocks in the DOM. This function will be called once upon registration of the
    // shape module.
    //
    // #### Notes:
    // * Paths to any files referenced in the HTML template must be prefixed with `{%SHAPES_PATH%}`, which will
    //   be substituted with `<Station search path>\shapes`. The URI to these resources must not be absolute
    //   paths, to allow the shapes to be deployed to any directory.
    // * View template names must be unique, duplicates will be ignored. System-defined view templates
    //   will be prefixed with `hsc`.  Custom shapes are encouraged to adopt similar naming conventions.
    // * This function must return at least one template.
    // * Multiple view templates can be returned as view templates can be nested. If so, the returned map must
    //   contain an additional property named `MAIN_VIEW_TEMPLATE`, specifying the name of the top-level view
    //   template.
    Shape.getViewTemplates = function() {
        return {
            extTemplate: Template,
            MAIN_VIEW_TEMPLATE: 'extTemplate' // This is not strictly required for a single view template.
        };
    };

    // ### getBindings() function
    // Return a map of `<Binding name>, <Knockout custom binding implementation>` to be registered
    // as Knockout binding handlers. This function will be called once upon registration of
    // the shape module.
    //
    // #### Notes:
    // * Binding names must be unique, duplicates will be ignored. System-defined bindings will be prefixed
    //   with `hsc`. Custom shapes are encouraged to adopt similar naming conventions.
    // * This function can return zero bindings or omitted altogether.
    // * For more information see: [Knockout documentation](http://knockoutjs.com/documentation/custom-bindings.html).
    // * The framework provides Knockout data bindings which can be attached to HTML elements in order to
    //   apply control functionality:
    //   * `hscShape` – provides selection and context menu behavior. The binding provides options to disable
    //     either one of these features, but by default both are enabled.
    //   * `hscFaceplate` – launches a faceplate when the HTML element is clicked.
    //   * `hscPopup` – launches a popup when the HTML element is clicked.
    Shape.getBindings = function() {
        Binding.shapesPath = Shape._config.shapesPath;
        return {
            extBinding: Binding
        };
    };
	

    // ### createViewModel() function
    // This function will be called for every table cell.
    // The return value must be one of the following:
    //
    // 1. A string specifying the name of a system-defined view model, e.g. `PointParamViewModel`, `MultiPointParamViewModel`.
    // 2. An object which implements the view model interface:
    //   * `initialise`: function({object} columnDefinition);
    //   * `onDataUpdate`: function({object} cellData);
    //
    // #### Notes:
    // * The need to write a custom view model implementation should be rare.  The recommended approach is to use a
    //   system-defined view model and write a custom Knockout binding handler which binds the view model’s properties
    //   to the view template.
    Shape.createViewModel = function() {
        return new ExtViewModel();
    };

    return Shape;
});
