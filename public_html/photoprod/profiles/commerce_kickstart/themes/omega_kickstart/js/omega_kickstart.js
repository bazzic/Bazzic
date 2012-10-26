(function ($) {
  /**
   *  Add a relevant body class when Commerce Kickstart Demo mode is on.
   */
  Drupal.behaviors.activeBarClass = {
    attach: function(context, settings) {            
      var isDemo = $('div#activebar-container').size();
      //console.log('isDemo: ' + isDemo);
      if(isDemo) {
        $('body').addClass('activebar-container');
      }
    }
  };
  
  Drupal.behaviors.mainMenuToggle = {
    attach: function(context, settings) {            
      $('.region-menu .navigation').before('<a href="#" class="menu-toggle" title="Toggle Menu"><span class="line first-line first"></span><span class="line"></span><span class="line"></span><span class="line last-line last"></span><span class="toggle-help">Menu</span></a>');
      $('.navigation .primary-menu h2, .navigation .second-menu h2').removeClass('element-invisible');
      
      $('.region-menu .menu-toggle').click(function(){
        $('.navigation').slideToggle();
      });
    }
  };
  
  Drupal.behaviors.betterEventSlider = {
    attach: function(context, settings) {            
      
      $(window).load(function(){
        var liheight = $('ul.event-slider li:first-child').height();
        $('.bx-wrapper, .bx-wrapper .bx-window').css('height', liheight);
      });
      
      //console.log('woot!');
      
      $(window).resize(function(){
        var liheight = $('ul.event-slider li:first-child').height();
        $('.bx-wrapper, .bx-wrapper .bx-window').css('height', liheight);
      });
      
      //$('ul.event-slider li').addClass('clearfix');
    }
  };

  // Handle user toolbar when user is admin and have admin toolbar enabled.
  Drupal.behaviors.customToolbar = {
    attach: function(context, settings) {
      if ($('body').hasClass('toolbar')) {
        $(window, context).resize(function() {
          var toolbarHeight = $('div#toolbar').height();
          $('.zone-user-wrapper').css('top', toolbarHeight + 'px');
        });
      }
    }
  }

})(jQuery);
