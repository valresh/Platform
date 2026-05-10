//COPYRIGHT (c) 2012
//HONEYWELL INTERNATIONAL INC.
//ALL RIGHTS RESERVED

//Legal rights of Honeywell International Inc. in this software is distinct
//from ownership of any medium in which the software is embodied. Copyright
//notices must be reproduced in any copies authorized by Honeywell International Inc.
(function() {
  var tableDefinition;
  var hmiwebJsBasePath;

  hmiwebJsBasePath = window.external.FrameworkServices.HMIWebJSBasePath;

  if (!hmiwebJsBasePath) {
      throw new Error('Cannot find HMIWebJS base path');
  }

  require.config({
	baseUrl: hmiwebJsBasePath
  });

  require(['asari/requireConfig'], function() {
    return require(['knockout', 'scripts/ScadaControlApp', 'asari/bindings/TabularDataBinding', 'scripts/ActiveFilters', 'scripts/ScadaControllersConstants'], function(ko, ScadaControlApp, TabularDataBinding, ActiveFilters, Constants) {
  
  tableDefinition = {
    /*
      this will be the table ID passed to CreateTableCursor
    */

    id: 'RTU',
    /*
      ensure this is tabular
    */

    type: 'tabular',
    title: 'Controllers',
    context: Constants.RTU_CONTEXT,
    name: Constants.RTU_NAME,
	initialPageSize: Math.floor(document.body.clientHeight / Constants.ROW_HEIGHT) - Constants.HEADER_FOOTER_ROWS,
	defaultSort: {
        column: 'name',
        direction: 'ascending'
    },

    /*
      ensure this is expanded in order to retrieve table data on page load
    */

    expanded: 'true',
    columnDefinition: [
      {
        elementsKey: 'Enable',
        label: 'Enable',
        name: 'enable',
        width: 4,
        units: '',
        elementType: 'ChkChk'
      }, {
        elementsKey: 'Name',
        label: 'Name',
	name: 'name',
        width: 12,
        units: '',
        elementType: 'link',
        sortable: true
      }, {
        elementsKey: 'Status',
        label: 'Status',
	name: 'status',
        width: 9,
        units: '',
        elementType: 'StsStsTxt'
      }, {
        elementsKey: 'Description',
        label: 'Description',
	name: 'description',
        width: 12,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'Number',
        label: 'Number',
	name: 'number',
        width: 5,
        units: '',
        elementType: 'alphanumeric',
        sortable: true
      }, {
        elementsKey: 'Server',
        label: 'Server',
	name: 'server',
        width: 12,
        units: '',
        elementType: 'StsTxt'
      }, {
        elementsKey: 'Channel',
        label: 'Channel',
	name: 'channel',
        width: 13,
        units: '',
        elementType: 'StsStsLnk'
      }
    ]
  };
  
  countsDefinition = {
    /*
      this will be the table ID passed to CreateTableCursor
    */

    id: 'COUNTS',
    /*
      ensure this is tabular
    */

    type: 'tabular',
    title: 'Counts',
    context: Constants.COUNTS_CONTEXT,
    name: Constants.COUNTS_NAME,
			
    /*
      ensure this is expanded in order to retrieve table data on page load
    */

    expanded: 'true',
    columnDefinition: [
      {
        elementsKey: 'TotalOK',
        label: 'Total OK',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'TotalMarg',
        label: 'Total Marginal',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'TotalFail',
        label: 'Total Failed',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'TotalDisabled',
        label: 'Total Disabled',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'TotalUnknown',
        label: 'Total Unknown',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'TotalSimulation',
        label: 'Total Simulation',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'FilterOK',
        label: 'Filter OK',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'FilterMarg',
        label: 'Filter Marginal',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'FilterFail',
        label: 'Filter Failed',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'FilterDisabled',
        label: 'Filter Disabled',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'FilterUnknown',
        label: 'Filter Unknown',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'FilterSimulation',
        label: 'Filter Simulation',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }, {
        elementsKey: 'FilterAll',
        label: 'Filter All',
        width: 6,
        units: '',
        elementType: 'alphanumeric'
      }
    ]
  };

        TabularDataBinding.getDefaultRowCount = function(viewModel) {            
            return totalRows = Math.floor(document.body.clientHeight / Constants.ROW_HEIGHT) - Constants.HEADER_FOOTER_ROWS;           
        };

        var app = new ScadaControlApp(tableDefinition, countsDefinition);
        app.run();
		
        var af = new ActiveFilters();
		af.ClearStatusCheckboxes();

     });
  });

}).call(this);

