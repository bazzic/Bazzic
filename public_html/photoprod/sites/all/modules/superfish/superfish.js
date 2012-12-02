(function ($) {
  /**
   * The Superfish Drupal Behavior to apply the Superfish jQuery plugin to lists.
   */
  Drupal.behaviors.superfish = {
    attach: function (context, settings) {
      // Take a look at each list to apply Superfish to.
      $.each(settings.superfish || {}, function(index, options) {
        // Process all Superfish lists.
        $('#superfish-' + options.id, context).once('superfish', function() {
          var list = $(this);
          // Apply Superfish to the list.
          list.superfish(options.sf);

          // Check if we are to apply any plugins to it.
          if (options.plugins || false) {
            if (options.plugins.supersubs || false) {
              list.supersubs(options.plugins.supersubs);
            }
            if (options.plugins.touchscreen || false) {
              list.sftouchscreen();
            }
            if (options.plugins.supposition || false) {
              list.supposition();
            }
            if (options.plugins.bgiframe || false) {
              list.find('ul').bgIframe({opacity:false});
            }
          }
        });
      });
    }
  };
})(jQuery);