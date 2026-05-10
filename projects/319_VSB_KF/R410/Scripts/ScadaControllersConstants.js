define('scripts/ScadaControllersConstants', function() {
	function ScadaControllersConstants() {}
	
	ScadaControllersConstants.RTU_CONTEXT = {type: 'controllers', point:'table'};
	ScadaControllersConstants.COUNTS_CONTEXT = {type: 'controllers', point:'counts'};
	ScadaControllersConstants.ROW_HEIGHT = 26;
	ScadaControllersConstants.HEADER_FOOTER_ROWS = 5;
	
	// These constants must match those in \WORK\pcdm\std\Gdafilerep\inc\TableCursorIf.h
	ScadaControllersConstants.RTU_NAME = 'RTU_TABLE';
	ScadaControllersConstants.COUNTS_NAME = 'COUNTS_TABLE';
	
	return ScadaControllersConstants;
});