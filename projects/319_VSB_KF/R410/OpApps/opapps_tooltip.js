// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/
// opapps_tooltip.js

(function($) {
	var hscOpAppsTooltip = window.hscOpAppsTooltip = {};

	hscOpAppsTooltip.show = function(pos, content, gravity, dist) {
		var tooltips = $('.hscOpAppsTooltip');
		tooltips.css('opacity', 0);
		tooltips.remove();

		var container = $('<div class="hscOpAppsTooltip">');

		gravity = gravity || 's';
		dist = dist || 20;

		container
			.html(content)
			.css({left: -1000, top: -1000, opacity: 0})
			.appendTo('body');

		var height = container.height() + parseInt(container.css('padding-top')) + parseInt(container.css('padding-bottom'));
		var width = container.width() + parseInt(container.css('padding-left')) + parseInt(container.css('padding-right'));
		var windowWidth = $(window).width();
		var windowHeight = $(window).height();
		var scrollTop = $('body').scrollTop();
		var left, top;

		switch (gravity) {
			case 'e':
			case 'w':
			case 'n':
				left = pos[0] - (width / 2);
				top = pos[1] + dist;
				if (left < 0)
					left = 5;
				if (left + width > windowWidth)
					left = windowWidth - width - 5;
				if (scrollTop + windowHeight < top + height)
					top = pos[1] - height - dist;
				break;
			case 's':
				left = pos[0] - (width / 2);
				top = pos[1] - height - dist;
				if (left < 0)
					left = 5;
				if (left + width > windowWidth)
					left = windowWidth - width - 5;
				if (scrollTop > top)
					top = pos[1] + dist;
				break;
		}

		container
			.css({
				left: left,
				top: top,
				opacity: 1
				});
	};

	hscOpAppsTooltip.cleanup = function() {
		var tooltips = $('.hscOpAppsTooltip');

		tooltips.css({
			'transition-delay': '0 !important',
			'-moz-transition-delay': '0 !important',
			'-webkit-transition-delay': '0 !important'
		});

		tooltips.css('opacity',0);

		setTimeout(function() { tooltips.remove() }, 500);
	};

})(jQuery);

// /************************************************************************/
// /*****     COPYRIGHT 2013    HONEYWELL INTERNATIONAL SARL            ****/
// /************************************************************************/