
(function() {
  var __hasProp = {}.hasOwnProperty,
    __extends = function(child, parent) { for (var key in parent) { if (__hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; },
    __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; };

  define(['asari/viewmodels/PointParamViewModel'], function(PointParamViewModel) {
    var ExtViewModel;
    ExtViewModel = (function(_super) {

      __extends(ExtViewModel, _super);

      function ExtViewModel() {
        ExtViewModel.__super__.constructor.apply(this, arguments);
		// Extend the base viewmodel with our custom flag
		this.CONFIG.format.imagelist = {default: null, dataType: 'string'};
		this._createProperties(this.CONFIG.format, 'format');
		

      }

      return ExtViewModel;

    })(PointParamViewModel.PointParamViewModel);

    return ExtViewModel;
  });

}).call(this);
