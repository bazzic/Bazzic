<?php

/**
 * @file 
 * Field handler to provide title links to drupal nodes
 * @ingroup views_field_handlers
 */
class activitystream_handler_activitystream_icon extends views_handler_field {
  
  function render($values) {
    return theme($values->activitystream_module . '_icon');
  }

}
