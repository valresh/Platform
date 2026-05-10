//COPYRIGHT (c) 2013
//HONEYWELL INTERNATIONAL INC.
//ALL RIGHTS RESERVED

//Legal rights of Honeywell International Inc. in this software is distinct
//from ownership of any medium in which the software is embodied. Copyright
//notices must be reproduced in any copies authorized by Honeywell International Inc.

define(['knockout'], function(ko) {

    var CheckboxVisibility = {
    }

    CheckboxVisibility.init = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
    }

    CheckboxVisibility.update = function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {

        var config = ko.utils.unwrapObservable(valueAccessor());
        var quality = ko.utils.unwrapObservable(config.quality);
        var impl = ko.utils.unwrapObservable(config.impl);
	if (impl === 1){
            element.style.visibility = 'visible';
        } else if (impl === 0){
            element.style.visibility = 'hidden';
        } else if ((quality & 0x8) > 0) {
            element.style.visibility = 'hidden';
        } else {
            element.style.visibility = 'visible';
        }
    }

    return CheckboxVisibility;
});
