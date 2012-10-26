(function($) {

/**
 * Trigger shipping quote recalculation when customer profile fields change.
 */
Drupal.behaviors.commerceShipping = {
  attach: function (context, settings) {
    $('[id^="edit-customer-profile-"] .form-item', context).children('.form-select, .form-text, .form-radio, .form-checkbox:not([name*="[commerce_customer_profile_copy]"])').change(function() {
      // Use setTimeout to allow time for the browser to autofill to the form.
      return setTimeout($.fn.commerceCheckShippingRecalculation, 100);
    });
  }
}

$.fn.commerceCheckShippingRecalculation = function() {
  var recalculate = true;

  // Check each of the required form elements in the shipping pane for values.
  $('[id^="edit-customer-profile-"] .required').each(function() {
    if (!$(this).val() || $(this).val() == '') {
      recalculate = false;
    }
  });

  if (recalculate == true) {
    // Trigger the click event on the shipping recalculation button.
    $('[id^="edit-commerce-shipping-recalculate"]').trigger('click');
  }
}

})(jQuery);
