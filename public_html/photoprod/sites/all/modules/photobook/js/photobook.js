(function ($) {
	Drupal.behaviors.photobook = {
			attach: function() {
		
				var q2 = $("#edit-2").val();
				var q3 = $("#edit-3").val();
				var q4 = $("#edit-4").val();
				var q5 = $("#edit-5").val();
				var q6 = $("#edit-6").val();
				var q7 = $("#edit-7").val();
				
				if (q2 != 0 || q3 != 0 || q4 != 0 || q5 != 0 || q6 != 0 || q7 != 0) {
					$("#edit-0").removeAttr("disabled");
					$("#edit-1").removeAttr("disabled");
				} else {
					$("#edit-0").attr("disabled", "disabled");
					$("#edit-1").attr("disabled", "disabled");
				}
				
				$('#edit-add-to-cart-pages').change(function() {
					var pages = parseInt($('#edit-add-to-cart-pages').val());
					var min = pages + 2;
					var max = pages * 6 + 2;
					$("li.image-numbers").text('You have chosen ' + pages + ' pages, so you can have between ' + min + ' and ' + max + ' photos.');
				});
				
				//You have chosen 12 pages, so you can have between 14 and 74 photos.
		
		
				/*var obj = {0:1, 1:2};
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
				$('div.order-total').text(pricetotaltext);
				
				$('#edit-field-pages-list-value').change(function(){
					$('#edit-submit-photobooks').click();
				});

				$.each(obj, function(key1, value1) { 
					$('#edit-add-to-cart-quantity-' + key1).change(function() {
						//$('#edit-submit').click();
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
						$('div.order-total').text(pricetotaltext);
					});
				});*/
				


			}
	}
})(jQuery);