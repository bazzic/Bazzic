<?php

/**
 * Defines an order total area handler that shows the order total field with its
 *   components listed in the footer of a View.
 */
class photobook_handler_area_price_total extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();

    // TODO: Update with option definitions if the display formatter itself
    // gets support for them.

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    unset($form['empty']);

    // TODO: Update with option form elements if the display formatter itself
    // gets support for them.
  }

  function render($empty = FALSE) {
    //return krumo($this->view);
    
    foreach ($this->view->result as $key => $result) {
      $commerce_price = $result->_field_data['product_id']['entity']->commerce_price['und'][0];
      //$price = commerce_currency_amount_to_decimal($commerce_price['amount'], $commerce_price['currency_code']);
      $format_price = commerce_currency_format($commerce_price['amount'], $commerce_price['currency_code']);
      return $format_price;
    }
    

    /*if (!$empty || !empty($this->options['empty'])) {
      // First look for an order_id argument.
      foreach ($this->view->argument as $name => $argument) {
        if ($argument instanceof commerce_order_handler_argument_order_order_id) {
          // If it is single value...
          if (count($argument->value) == 1) {
            // Load the order.
            $order = commerce_order_load(reset($argument->value));

            // Prepare a display settings array.
            $display = array(
              'label' => 'hidden',
              'type' => 'commerce_price_formatted_components',
              'settings' => array(
                'calculation' => FALSE,
              ),
            );

            // Render the order's order total field with the current display.
            $field = field_view_field('commerce_order', $order, 'commerce_order_total', $display);

            return '<div class="commerce-order-handler-area-order-total">' . drupal_render($field) . '</div>';
          }
        }
      }
    }*/

    return 'wassup!';
  }
}
