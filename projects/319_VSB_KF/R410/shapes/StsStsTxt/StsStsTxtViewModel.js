
(function() {
  var __hasProp = {}.hasOwnProperty,
    __extends = function(child, parent) { for (var key in parent) { if (__hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; },
    __bind = function(fn, me){ return function(){ return fn.apply(me, arguments); }; };

  define(['asari/viewmodels/PointParamViewModel'], function(PointParamViewModel) {
    var StsStsTxtViewModel;
    StsStsTxtViewModel = (function(_super) {

      __extends(StsStsTxtViewModel, _super);

      function StsStsTxtViewModel() {
        StsStsTxtViewModel.__super__.constructor.apply(this, arguments);
        // Extend the base viewmodel with our custom flag
        this.CONFIG.format.isPointParam = {default: null, dataType: 'bool'};
        this._createProperties(this.CONFIG.format, 'format');
      }

      return StsStsTxtViewModel;

    })(PointParamViewModel.PointParamViewModel);

    return StsStsTxtViewModel;
  });

}).call(this);
