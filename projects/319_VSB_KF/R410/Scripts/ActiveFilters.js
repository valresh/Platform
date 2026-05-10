//COPYRIGHT (c) 2013
//HONEYWELL INTERNATIONAL INC.
//ALL RIGHTS RESERVED

//Legal rights of Honeywell International Inc. in this software is distinct
//from ownership of any medium in which the software is embodied. Copyright
//notices must be reproduced in any copies authorized by Honeywell International Inc.
define('scripts/ActiveFilters', ['knockout', 'knockout-postbox', 'asari/util/AsariFunctionalLogger', 'asari/ViewDefinition', 'asari/PostboxMessages', 'scripts/ScadaControllersConstants'], function(ko, postbox, log, ViewDefinition, PostboxMessages, Constants) {

	function ActiveFilters() {
		this.treeFilter = null;
		this.statusFilter = null;
		window.ActiveFilters = this;  
		log.param(function() {
			return "ActiveFilters initiated";
		});
	}

	/* Filter selection from the tree */
	ActiveFilters.prototype.selectTree = function (data, event)
	{
		var $tree, index, tagName, remoteType, subType, type, val, tree, key, tableMessage, countMessage, filter, tableKey, countKey;
		log.debug(function() {
			return "ActiveFilters::selectTree | Entering";
		});
	        $tree = $(event.srcElement).closest('#Tree');
	        tree = $tree[0];
	        if (tree) {
			index = tree.SelectedRowIndex - tree.ViewPosn;
			remoteType = GetSelectedData(tree, "RemoteType", index);
			subType = GetSelectedData(tree, "SubType", index);
			
			if (remoteType === 8)
			{
				if (subType === 1001)
				{
					// "System Components" node, clear the filter
					this.ClearTreeFilters();
				}
				else if (subType === 1002)
				{
					// Server node
					type = "Server";
					val = GetSelectedData(tree, "ItemNumber", index);
				}
				else if (subType === 1003)
				{
					// "Channels" node, use server node above for filtering
					type = "Server";
					val = GetSelectedData(tree, "ItemNumber", index - 1);
				}
				else if (subType === 1004)
				{
					// Channel node
					type = "Channel";
					val = GetSelectedData(tree, "TagName", index);
				}
				else
				{
					log.warn(function() {
						return "ActiveFilters::selectTree | subType error";
					});
					return;
				}
			}
			else if (remoteType === 6)
			{
				type = "Asset";
				val = GetSelectedData(tree, "TagName", index);
			}
			else
			{
				log.warn(function() {
					return "ActiveFilters::selectTree | remoteType error";
				});
				return;
			}
			log.debug(function() {
				return "ActiveFilters::selectTree | type is '" + type + "', val is '" + val + "'";
			});
			
			this.treeFilter = type + " = '" + val + "'";
			
			log.debug(function() {
				return "ActiveFilters::selectTree | Tree Filter:[" + this.treeFilter + "]";
			});
			filter = combineFilters(this.treeFilter, this.statusFilter);
			
			log.info(function() {
				return "ActiveFilters::selectTree | Total Filter:[" + filter + "]";
			});
			
			tableKey = ViewDefinition.Utils.getTableKey(Constants.RTU_CONTEXT);
			tableMessage = new PostboxMessages.TypeSpecificFilterMessage(tableKey);
			tableMessage.addFilter(tableKey, filter);
			tableMessage.post();
		
			countKey = ViewDefinition.Utils.getTableKey(Constants.COUNTS_CONTEXT);
			countMessage = new PostboxMessages.TypeSpecificFilterMessage(countKey);
			countMessage.addFilter(countKey, this.treeFilter);
			countMessage.post();
	        } else {
			log.error(function() {
				return "ActiveFilters::selectTree | could not find parent tree container";
			});
	        }
	        log.debug(function() {
	                return "ActiveFilters::selectTree | Leaving";
	        });
    	};
	
	function GetSelectedData(tree, dataType, index)
	{
		var data;
		try {
			data = tree.Fields.Item(dataType).GetValue(index);
			log.info(function() {
				return "ActiveFilters::GetSelectedData | selected item's " + dataType + ": '" + data + "' at index " + index;
			});
		} catch (error) {
			log.error(function() {
				return "ActiveFilters::GetSelectedData | Failed to retrieve '" + dataType + "' of tree node at index=" + index + " (SelectedRowIndex=" + tree.SelectedRowIndex + ", ViewPosn=" + tree.ViewPosn + ") | Error: " + (error.toString());
			});
			return null;
		}
		return data;
	}
	
	ActiveFilters.prototype.selectStatus = function ()
	{
		var filter, key, message;
		var statusFilter = "";
		var isFirst = true;
		var filterOptions = document.getElementsByName('FilterOption');
		
		log.debug(function() {
			return "ActiveFilters::selectStatus | Entering";
		});
		
		for (i = 0; i < filterOptions.length; i++)
		{
			if (filterOptions[i].checked)
			{
				if (!isFirst)
				{
					statusFilter += " or ";
				}
				statusFilter += "AggregateStatus = '" + filterOptions[i].value + "'";
				isFirst = false;
			}
		}
		this.statusFilter = statusFilter;
				
		log.debug(function() {
			return "ActiveFilters::selectStatus | Aggregate Status Filter:[" + this.statusFilter + "]";
		});
		filter = combineFilters(this.treeFilter, this.statusFilter);
		log.info(function() {
			return "ActiveFilters::selectStatus | Total Filter:[" + filter + "]";
		});
		
		key = ViewDefinition.Utils.getTableKey(Constants.RTU_CONTEXT);
		message = new PostboxMessages.TypeSpecificFilterMessage(key);
		message.addFilter(key, filter);
		message.post();
		
		log.debug(function() {
			return "ActiveFilters::selectStatus | Leaving";
		});
	};
	
	function combineFilters(tree, status)
	{
		var filterApplied = document.getElementById('FilterApplied');
		if (tree !== null && tree !== 'undefined' && tree !== '' && status !== null && status !== 'undefined' && status !== '')
		{
			filterApplied.style.visibility = 'visible';
			return tree + " and (" + status + ")";
		}
		else if (tree !== null && tree !== 'undefined' && tree !== '')
		{
			filterApplied.style.visibility = 'visible';
			return tree;
		}
		else if (status !== null && status !== 'undefined' && status !== '')
		{
			filterApplied.style.visibility = 'visible';
			return status;
		}
		filterApplied.style.visibility = 'hidden';
		return "";
	}

	ActiveFilters.prototype.ClearAllFilters = function ()
	{
		var tableMessage, countMessage, filter, tableKey, countKey;
		log.debug(function() {
			return "ActiveFilters::ClearAllFilters | Entering";
		});
		
		this.ClearTreeFilters();
		this.ClearStatusFilters();
		
		log.debug(function() {
			return "ActiveFilters::ClearAllFilters | All filters cleared";
		});
	};
	
	ActiveFilters.prototype.ClearTreeFilters = function ()
	{
		var tableMessage, countMessage, filter, tableKey, countKey, tree;
		log.debug(function() {
			return "ActiveFilters::ClearTreeFilters | Entering";
		});
	
		this.treeFilter = "";
		filter = combineFilters(this.treeFilter, this.statusFilter);
			
		log.info(function() {
			return "ActiveFilters::ClearTreeFilters | Total Filter:[" + filter + "]";
		});
		
		tableKey = ViewDefinition.Utils.getTableKey(Constants.RTU_CONTEXT);
		tableMessage = new PostboxMessages.TypeSpecificFilterMessage(tableKey);
		tableMessage.addFilter(tableKey, filter);
		tableMessage.post();
	
		countKey = ViewDefinition.Utils.getTableKey(Constants.COUNTS_CONTEXT);
		countMessage = new PostboxMessages.TypeSpecificFilterMessage(countKey);
		countMessage.addFilter(countKey, this.treeFilter);
		countMessage.post();
		
		// Deselect the tree node
		tree = document.getElementById('Tree');
		if (tree) {
			tree.Binding.DataObjects.Item(0).SelectNone();
		}
		else
		{
			log.error(function() {
				return "ActiveFilters::ClearTreeFilters | Missing tree node";
			});
		}
		
		log.debug(function() {
			return "ActiveFilters::ClearTreeFilters | Tree filter cleared";
		});
	};

	ActiveFilters.prototype.ClearStatusFilters = function ()
	{
		var filter;
		log.debug(function() {
			return "ActiveFilters::ClearStatusFilters | Entering";
		});
	
		this.statusFilter = "";
		this.ClearStatusCheckboxes();
		filter = combineFilters(this.treeFilter, this.statusFilter);
			
		log.info(function() {
			return "ActiveFilters::ClearStatusFilters | Total Filter:[" + filter + "]";
		});
		
		tableKey = ViewDefinition.Utils.getTableKey(Constants.RTU_CONTEXT);
		tableMessage = new PostboxMessages.TypeSpecificFilterMessage(tableKey);
		tableMessage.addFilter(tableKey, filter);
		tableMessage.post();
		
		log.debug(function() {
			return "ActiveFilters::ClearStatusFilters | Status filters cleared";
		});
	};
	
	ActiveFilters.prototype.ClearStatusCheckboxes = function ()
	{
		var filterOptions = document.getElementsByName('FilterOption');
		
		log.debug(function() {
			return "ActiveFilters::ClearStatusCheckboxes | Entering";
		});
		
		for (i = 0; i < filterOptions.length; i++)
		{
			filterOptions[i].checked = false;
		}
		
		log.debug(function() {
			return "ActiveFilters::ClearStatusCheckboxes | Leaving";
		});
	};
	
	return ActiveFilters;
});
