(function ($) {
	Drupal.behaviors.photobook = {
			attach: function() {
				var obj = {0:1, 1:2};
				var pricetotal = 0;
				$('#edit-submit-photobooks').hide();
				$.each(obj, function(key2, value2) { 
					var quantity = $('#edit-add-to-cart-quantity-' + key2).val();
					if (!$('#quantity' + key2).length) {
						$('<input>').attr({
						    type: 'hidden',
						    id: 'quantity' + key2,
						    name: 'quantity' + key2
						}).appendTo('#views-exposed-form-photobooks-page');
					}
					$('#quantity' + key2).val(quantity);
					var price = $('td.col-' + value2 + ' div.field-item.even').text();
	    			// Remove the £ sign.  We need to use unicode.
	    			var pricenum = price.replace(/\u00A3/g, "");
	    			var pricefloat = parseFloat(pricenum);
	    			var total = quantity * pricefloat;
	    			pricetotal += total;
				});
				var pricetotaltext = '\u00A3' + pricetotal.toFixed(2);
				$('div.view-header').text(pricetotaltext);
				
				$('#edit-field-pages-list-value').change(function(){
					$('#edit-submit-photobooks').click();
				});

				$.each(obj, function(key1, value1) { 
					$('#edit-add-to-cart-quantity-' + key1).change(function() {
						var pricetotal = 0;
						$.each(obj, function(key2, value2) { 
							var quantity = $('#edit-add-to-cart-quantity-' + key2).val();
							$('#quantity' + key2).val(quantity);
							var price = $('td.col-' + value2 + ' div.field-item.even').text();
			    			// Remove the £ sign.  We need to use unicode.
			    			var pricenum = price.replace(/\u00A3/g, "");
			    			var pricefloat = parseFloat(pricenum);
			    			var total = quantity * pricefloat;
			    			pricetotal += total;
						});
						var pricetotaltext = '\u00A3' + pricetotal.toFixed(2);
						$('div.view-header').text(pricetotaltext);
					});
				});
				


			}
	}
})(jQuery);